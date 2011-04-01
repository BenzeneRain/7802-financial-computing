#include <sstream>
#include <string>

#include "YieldCurve.h"
#include "Instrument.h"


//////////////////////////////////////////
// Definition of the class YieldCurve
//////////////////////////////////////////
YieldCurve::YieldCurve(std::vector<InstrumentDefinition *>& instrDefs)
{
    _instrValues = NULL;

    // Prepare mapping between Instrument indices and their definitions
    std::map<int, InstrumentDefinition *>().swap(_instrDefs);
    

    // FIX: Be careful stack overflow
    for(std::vector<InstrumentDefinition *>::iterator iter = instrDefs.begin();
            iter != instrDefs.end(); iter ++)
    {
        InstrumentDefinition* ptrInstrDef =
            static_cast<InstrumentDefinition *>(*iter);
        
        int index = ptrInstrDef->index(); 
        _instrDefs[index] = ptrInstrDef;
    }
}

YieldCurve::~YieldCurve()
{
}

InstrumentValues* YieldCurve::bindData(InstrumentValues *instrVals)
{
    // Sanity check for the new values
    for(std::vector<std::pair<int, double> >::iterator iter = instrVals->values.begin();
            iter != instrVals->values.end(); iter ++)
    {
        std::pair<int, double>& val = *iter;
        if(_instrDefs.find(val.first) == _instrDefs.end())
        {
            std::ostringstream oss;
            oss << "Invalid instrument data index " << val.first;
            std::string message = oss.str();
            throw YieldCurveException(message);
        }
    }

    InstrumentValues* prevInstrValues = _instrValues;
    _instrValues = instrVals;
    return prevInstrValues;
}

//////////////////////////////////////////
// Definition of the class ZeroCouponRateCurve
//////////////////////////////////////////
ZeroCouponRateCurve::ZeroCouponRateCurve(std::vector<InstrumentDefinition *>& instrDefs):
    YieldCurve(instrDefs)
{
}

ZeroCouponRateCurve::~ZeroCouponRateCurve()
{
}

void ZeroCouponRateCurve::updateCurve()
{
}

////////////////////////////////////////////
//// Definition of the class YieldCurveException
////////////////////////////////////////////
YieldCurveException::~YieldCurveException()
{
}
