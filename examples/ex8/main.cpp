#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

#include "PrintUtilities.hpp"
#include "FeedForwardNeuralNetwork.hpp"



int main() {
    using namespace std;

    FeedForwardNeuralNetwork * ffnn = new FeedForwardNeuralNetwork("stored_ffnn.txt");

    cout << "The FFNN read from the file looks like this:" << endl << endl;

    printFFNNStructure(ffnn, true, 0);

    cout << endl << "Note that the stored FFNN was not connected. Hence, there are no feeders or betas." << endl;
    cin.ignore();

    cout << "Now we connect the network and store it into a new file 'connected_ffnn.txt'." << endl;
    //NON I/O CODE
    ffnn->connectFFNN();
    ffnn->storeOnFile("connected_ffnn.txt");
    //
    cout << "Done." << endl;
    cin.ignore();

    cout << "Finally we add 1st, 2nd and variational derivative substrates and store the ffn into a new file 'substrate_ffnn.txt'." << endl;
    //NON I/O CODE
    ffnn->addFirstDerivativeSubstrate();
    ffnn->addSecondDerivativeSubstrate();
    ffnn->addVariationalFirstDerivativeSubstrate();
    ffnn->storeOnFile("substrate_ffnn.txt");
    //
    cout << "Done." << endl;
    cin.ignore();

    delete ffnn;

    cout << endl << endl;
    return 0;
}
