#include "OutputNNLayer.hpp"

#include "ActivationFunctionInterface.hpp"
#include "ActivationFunctionManager.hpp"
#include "OutputNNUnit.hpp"


// --- Register Unit

void OutputNNLayer::_registerUnit(NetworkUnit * newUnit)
{
    NNLayer::_registerUnit(newUnit);
    if(OutputNNUnit * outu = dynamic_cast<OutputNNUnit *>(newUnit)) {
        _U_out.push_back(outu);
    }
}


// --- Constructor

void OutputNNLayer::construct(const int &nunits, ActivationFunctionInterface * actf)
{
    for (int i=1; i<nunits; ++i)
        {
            OutputNNUnit * newUnit = new OutputNNUnit(actf->getCopy());
            _registerUnit(newUnit);
        }
    delete actf;
}
