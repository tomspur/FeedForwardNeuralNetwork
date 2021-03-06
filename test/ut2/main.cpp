#include <iostream>
#include <assert.h>
#include <cmath>

#include "FeedForwardNeuralNetwork.hpp"
#include "ActivationFunctionManager.hpp"
#include "PrintUtilities.hpp"



int main(){
    using namespace std;

    // make a check while the FFNN is not connected yet
    FeedForwardNeuralNetwork * ffnn = new FeedForwardNeuralNetwork(3, 5, 3);
    ffnn->pushHiddenLayer(4);

    ffnn->getNNLayer(0)->getNNUnit(2)->setActivationFunction(std_actf::provideActivationFunction("GSS"));
    ffnn->getNNLayer(1)->getNNUnit(0)->setActivationFunction(std_actf::provideActivationFunction("GSS"));
    ffnn->getNNLayer(2)->getNNUnit(1)->setActivationFunction(std_actf::provideActivationFunction("GSS"));
    ffnn->storeOnFile("ffnn_stage1.txt");

    FeedForwardNeuralNetwork * ffnn2 = new FeedForwardNeuralNetwork("ffnn_stage1.txt");

    assert(ffnn->getNLayers() == ffnn2->getNLayers());
    assert(ffnn->getLayer(0)->getNUnits() == ffnn2->getLayer(0)->getNUnits());
    assert(ffnn->getLayer(1)->getNUnits() == ffnn2->getLayer(1)->getNUnits());
    assert(ffnn->getLayer(2)->getNUnits() == ffnn2->getLayer(2)->getNUnits());
    assert(ffnn->getLayer(3)->getNUnits() == ffnn2->getLayer(3)->getNUnits());

    assert(!ffnn2->isConnected());
    assert(!ffnn2->hasFirstDerivativeSubstrate());
    assert(!ffnn2->hasSecondDerivativeSubstrate());
    assert(!ffnn2->hasVariationalFirstDerivativeSubstrate());
    assert(!ffnn2->hasCrossFirstDerivativeSubstrate());
    assert(!ffnn2->hasCrossSecondDerivativeSubstrate());

    assert(ffnn->getNNLayer(0)->getNNUnit(0)->getActivationFunction()->getIdCode() == "LGS");
    assert(ffnn->getNNLayer(0)->getNNUnit(1)->getActivationFunction()->getIdCode() == "LGS");
    assert(ffnn->getNNLayer(0)->getNNUnit(2)->getActivationFunction()->getIdCode() == "GSS");
    assert(ffnn->getNNLayer(0)->getNNUnit(3)->getActivationFunction()->getIdCode() == "LGS");
    assert(ffnn->getNNLayer(1)->getNNUnit(0)->getActivationFunction()->getIdCode() == "GSS");
    assert(ffnn->getNNLayer(1)->getNNUnit(1)->getActivationFunction()->getIdCode() == "LGS");
    assert(ffnn->getNNLayer(1)->getNNUnit(2)->getActivationFunction()->getIdCode() == "LGS");
    assert(ffnn->getNNLayer(2)->getNNUnit(0)->getActivationFunction()->getIdCode() == "LGS");
    assert(ffnn->getNNLayer(2)->getNNUnit(1)->getActivationFunction()->getIdCode() == "GSS");

    delete ffnn2;



    // connect the FFNN and make a check
    ffnn->connectFFNN();
    ffnn->assignVariationalParameters();
    ffnn->storeOnFile("ffnn_stage2.txt");

    ffnn2 = new FeedForwardNeuralNetwork("ffnn_stage2.txt");

    assert(ffnn2->isConnected());
    assert(!ffnn2->hasFirstDerivativeSubstrate());
    assert(!ffnn2->hasSecondDerivativeSubstrate());
    assert(!ffnn2->hasVariationalFirstDerivativeSubstrate());
    assert(!ffnn2->hasCrossFirstDerivativeSubstrate());
    assert(!ffnn2->hasCrossSecondDerivativeSubstrate());

    assert(ffnn->getNBeta() == ffnn2->getNBeta());
    for (int i=0; i<ffnn2->getNBeta(); ++i){
        assert(ffnn->getBeta(i) == ffnn2->getBeta(i));
    }

    assert(ffnn->getNVariationalParameters() == ffnn2->getNVariationalParameters());
    for (int i=0; i<ffnn2->getNVariationalParameters(); ++i){
        assert(ffnn->getVariationalParameter(i) == ffnn2->getVariationalParameter(i));
    }

    delete ffnn2;


    // add first derivative substrate and make a check
    ffnn->addFirstDerivativeSubstrate();
    ffnn->storeOnFile("ffnn_stage3.txt");

    ffnn2 = new FeedForwardNeuralNetwork("ffnn_stage3.txt");

    assert(ffnn2->isConnected());
    assert(ffnn2->hasFirstDerivativeSubstrate());
    assert(!ffnn2->hasSecondDerivativeSubstrate());
    assert(!ffnn2->hasVariationalFirstDerivativeSubstrate());
    assert(!ffnn2->hasCrossFirstDerivativeSubstrate());
    assert(!ffnn2->hasCrossSecondDerivativeSubstrate());

    delete ffnn2;


    // add second derivative substrate and make a check
    ffnn->addSecondDerivativeSubstrate();
    ffnn->storeOnFile("ffnn_stage4.txt");

    ffnn2 = new FeedForwardNeuralNetwork("ffnn_stage4.txt");

    assert(ffnn2->isConnected());
    assert(ffnn2->hasFirstDerivativeSubstrate());
    assert(ffnn2->hasSecondDerivativeSubstrate());
    assert(!ffnn2->hasVariationalFirstDerivativeSubstrate());
    assert(!ffnn2->hasCrossFirstDerivativeSubstrate());
    assert(!ffnn2->hasCrossSecondDerivativeSubstrate());

    delete ffnn2;


    // add variational first derivative substrate and make a check
    ffnn->addVariationalFirstDerivativeSubstrate();
    ffnn->storeOnFile("ffnn_stage5.txt");

    ffnn2 = new FeedForwardNeuralNetwork("ffnn_stage5.txt");

    assert(ffnn2->isConnected());
    assert(ffnn2->hasFirstDerivativeSubstrate());
    assert(ffnn2->hasSecondDerivativeSubstrate());
    assert(ffnn2->hasVariationalFirstDerivativeSubstrate());
    assert(!ffnn2->hasCrossFirstDerivativeSubstrate());
    assert(!ffnn2->hasCrossSecondDerivativeSubstrate());

    delete ffnn2;


    // add cross first derivative substrate and make a check
    ffnn->addCrossFirstDerivativeSubstrate();
    ffnn->storeOnFile("ffnn_stage6.txt");

    ffnn2 = new FeedForwardNeuralNetwork("ffnn_stage6.txt");

    assert(ffnn2->isConnected());
    assert(ffnn2->hasFirstDerivativeSubstrate());
    assert(ffnn2->hasSecondDerivativeSubstrate());
    assert(ffnn2->hasVariationalFirstDerivativeSubstrate());
    assert(ffnn2->hasCrossFirstDerivativeSubstrate());
    assert(!ffnn2->hasCrossSecondDerivativeSubstrate());

    delete ffnn2;



    // add cross second derivative substrate and make a check
    ffnn->addCrossSecondDerivativeSubstrate();
    ffnn->storeOnFile("ffnn_stage7.txt");

    ffnn2 = new FeedForwardNeuralNetwork("ffnn_stage7.txt");

    assert(ffnn2->isConnected());
    assert(ffnn2->hasFirstDerivativeSubstrate());
    assert(ffnn2->hasSecondDerivativeSubstrate());
    assert(ffnn2->hasVariationalFirstDerivativeSubstrate());
    assert(ffnn2->hasCrossFirstDerivativeSubstrate());
    assert(ffnn2->hasCrossSecondDerivativeSubstrate());



    // check that, once the input is the same for the two FFNN, all outputs (including derivatives) are the same
    const double input[2] = {37.42, -18.01};
    ffnn->setInput(input);
    ffnn->FFPropagate();
    ffnn2->setInput(input);
    ffnn2->FFPropagate();

    for (int i=0; i<2; ++i) {
        assert(ffnn->getOutput(i) == ffnn2->getOutput(i));

        for (int j=0; j<2; ++j) {
            assert(ffnn->getFirstDerivative(i, j) == ffnn2->getFirstDerivative(i, j));
            assert(ffnn->getSecondDerivative(i, j) == ffnn2->getSecondDerivative(i, j));

            for (int k=0; k<ffnn->getNVariationalParameters(); ++k){
                assert(ffnn->getCrossFirstDerivative(i, j, k) == ffnn2->getCrossFirstDerivative(i, j, k));
                assert(ffnn->getCrossSecondDerivative(i, j, k) == ffnn2->getCrossSecondDerivative(i, j, k));
            }
        }

        for (int k=0; k<ffnn->getNVariationalParameters(); ++k){
            assert(ffnn->getVariationalFirstDerivative(i, k) == ffnn2->getVariationalFirstDerivative(i, k));
        }
    }

    delete ffnn2;
    delete ffnn;

    return 0;
}
