
#include <iostream>
#include <cmath>
#include <numeric>
#include <algorithm>

#include "NNTrainerGSL.hpp"
#include "PrintUtilities.hpp"


/*
  Example: Train FFNN via GSL's Non-Linear Fit with Levenberg-Marquardt solver

  In this example we want to demonstrate how a FFNN can be trained via GSL's fit routines to fit target data (in this case a gaussian).

  The fit is achieved by minimizing the mean square distance of NN to data (by Levenberg-Marquardt), where the distance and gradient are integrated on a unifrom grid.
  Hence, we have to create the NN and target data, define functions for distance and gradient, and pass all the information to the GSL lib.

  For convenience, a NNFitter1D class is created, which handles the whole process in a user-friendly way, including normalization of the data.
*/

double gaussian(const double x, const double a, const double b) {
    return exp(-a*pow(x-b, 2));
};

// first derivative of gaussian
double gaussian_ddx(const double x, const double a, const double b) {
    return 2.0*a*(b-x) * exp(-a*pow(x-b, 2));
};

// first derivative of gaussian
double gaussian_d2dx(const double x, const double a, const double b) {
    return (pow(2.0*a*(b-x), 2) - 2.0*a) * exp(-a*pow(x-b, 2));
};


// hardcoded target values of logistic actf

#define ACTF_X0 0.0 // target data mean
#define ACTF_XS 1.0 // target data standard deviation
#define ACTF_XD ACTF_XS*3.464101615 //uniform distribution [a,b]: (b-a) = sigma*sqrt(12)
#define ACTF_Y0 0.5 // target output mean
#define ACTF_YD 1.0 // target output interval size


int main (void) {
    using namespace std;

    FeedForwardNeuralNetwork * ffnn;
    NNTrainingData tdata;
    NNTrainingConfig tconfig;
    NNTrainerGSL * trainer;

    const double lb = -10;
    const double ub = 10;
    const int ntraining = 2000; // how many training data points
    const int nvalidation = 1000; // how many validation data points
    const int ntesting = 3000; // how many testing data points
    const int ndata = ntraining + nvalidation + ntesting;

    double ** xdata;
    double ** ydata;
    double *** d1data;
    double *** d2data;
    double ** weights;

    int nl, nhl, nhu[2], nfits = 1;
    double maxchi = 0.0, lambda_r = 0.0, lambda_d1 = 0.0, lambda_d2 = 0.0;
    bool verbose = true, flag_d1 = false, flag_d2 = false, flag_r = false;

    //manual allocation of data arrays for the data struct
    xdata = new double*[ndata];
    ydata = new double*[ndata];
    d1data = new double**[ndata];
    d2data = new double**[ndata];
    weights = new double*[ndata];
    for (int i = 0; i<ndata; ++i) {
        xdata[i] = new double[1];
        ydata[i] = new double[1];
        d1data[i] = new double*[1];
        d1data[i][0] = new double[1];
        d2data[i] = new double*[1];
        d2data[i][0] = new double[1];
        weights[i] = new double[1];
    }

    cout << "Let's start by creating a Feed Forward Artificial Neural Network (FFANN)" << endl;
    cout << "========================================================================" << endl;
    cout << endl;
    cout << "How many units should the first hidden layer have? ";
    cin >> nhu[0];
    cout << "How many units should the second hidden layer have? (<=1 for none) ";
    cin >> nhu[1];
    cout << endl;

    // NON I/O CODE
    nl = (nhu[1]>1)? 4:3;
    nhl = nl-2;
    //

    cout << "We generate a FFANN with " << nl << " layers and 2, " << nhu[0];
    if (nhu[1]>0) { cout << ", " << nhu[1];}
    cout << ", 2 units respectively" << endl;
    cout << "========================================================================" << endl;
    cout << endl;
    cout << "In the following we use GSL non-linear fit to minimize the mean-squared-distance+regularization of NN vs. target function, i.e. find optimal betas." << endl;
    cout << endl;
    cout << "Please enter the first derivative lambda. ";
    cin >> lambda_d1;
    cout << "Please enter the second derivative lambda. ";
    cin >> lambda_d2;
    cout << "Please enter the regularization lambda. ";
    cin >> lambda_r;
    cout << "Please enter the the maximum tolerable fit residual. ";
    cin >> maxchi;
    cout << "Please enter the maximum number of fitting runs. ";
    cin >> nfits;
    cout << endl << endl;
    cout << "Now we find the best fit ... " << endl;


    // NON I/O CODE

    ffnn = new FeedForwardNeuralNetwork(2, nhu[0], 2);
    for (int i = 1; i<nhl; ++i) ffnn->pushHiddenLayer(nhu[i]);
    ffnn->connectFFNN();
    ffnn->addVariationalFirstDerivativeSubstrate();
    ffnn->addFirstDerivativeSubstrate(); // we always want those derivatives for printout
    ffnn->addSecondDerivativeSubstrate();
    if (lambda_d1 > 0 || lambda_d2 > 0) {ffnn->addCrossFirstDerivativeSubstrate();};// first deriv also required for second cross deriv
    if (lambda_d1 > 0) flag_d1 = true;
    if (lambda_d2 > 0) {ffnn->addCrossSecondDerivativeSubstrate(); flag_d2 = true;};
    if (lambda_r > 0) flag_r = true;

    // generating the data to be fitted
    random_device rdev;
    mt19937_64 rgen = std::mt19937_64(rdev());
    uniform_real_distribution<double> rd(lb,ub);
    for (int i = 0; i < ndata; ++i) {
        xdata[i][0] = rd(rgen);
        ydata[i][0] = gaussian(xdata[i][0], 1, 0);
        d1data[i][0][0] = gaussian_ddx(xdata[i][0], 1, 0);
        d2data[i][0][0] = gaussian_d2dx(xdata[i][0], 1, 0);
        weights[i][0] = 1.0; // our data have no error, so set all weights to 1
        if (verbose) printf ("data: %i %g %g\n", i, xdata[i][0], ydata[i][0]);
    };

    // calculate values for normalization
    /*auto mimax = minmax_element(&xdata[0][0], &xdata[ndata-1][0]);
    auto mimay = minmax_element(&ydata[0][0], &ydata[ndata-1][0]);
    double x0 = 0.5*(*mimax.first + *mimax.second);
    double y0 = 0.5*(*mimay.first + *mimay.second);
    double xd = *mimax.second - *mimax.first;
    double yd = *mimay.second - *mimay.first;*/

    double xscale = 0.1;//ACTF_XD/xd;
    double yscale = 1.0;//ACTF_YD/yd;
    double xshift = 0.0;//ACTF_X0 - x0;
    double yshift = 0.0;//ACTF_Y0 - y0;

    cout << "xscale: " << xscale << endl;
    cout << "yscale: " << yscale << endl;
    cout << "xshift: " << xshift << endl;
    cout << "yshift: " << yshift << endl;

    for (int i = 0; i < ndata; ++i) {
        xdata[i][0] = (xdata[i][0] + xshift) * xscale;
        ydata[i][0] = (ydata[i][0] + yshift) * yscale;
        d1data[i][0][0] = d1data[i][0][0] * yscale / xscale;
        d2data[i][0][0] = d2data[i][0][0] * yscale / pow(xscale, 2);
    }


    tdata = {ndata, ndata/4, ndata/4, 1, 1, xdata, ydata, d1data, d2data, weights};
    tconfig = {flag_d1, flag_d2, flag_r, lambda_r, lambda_d1, lambda_d2};

    trainer = new NNTrainerGSL(tdata, tconfig, ffnn);

    trainer->bestFit(100, nfits, maxchi, verbose ? 2 : 1);


    cout << "Done." << endl;
    cout << "========================================================================" << endl;
    cout << endl;
    /*
    cout << "Finally we compare the best fit NN to the target function:" << endl << endl;

    // NON I/O CODE
    fitter->compareFit(0, ndata, 100);
    //

    cout << endl;
    */
    cout << "Now we print the output/NN to a file. The end." << endl;

    // NON I/O CODE
    trainer->printFitOutput(-10, 10, 200, xscale, yscale, xshift, yshift, true, true);
    trainer->printFitNN();
    //

    //! A LOT OF DELETES MISSING !

    return 0;

}
