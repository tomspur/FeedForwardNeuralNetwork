#ifndef OFFSET_UNIT
#define OFFSET_UNIT

#include "NetworkUnit.hpp"

// Offset Unit
class OffsetUnit: public NetworkUnit
{
protected:

public:
    OffsetUnit(){_pv = 1.; _v = 1.;}

    // string code methods
    virtual std::string getIdCode(){return "off";} // return identifier for unit type

    // Computation
    void computeFeed(){}
    void computeActivation(){}
    void computeDerivatives(){}

    void computeValues() {_v = _pv;}
};


#endif
