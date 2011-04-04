#include <sstream>
#include <string>

#include "YieldCurve.h"
#include "Instrument.h"


//////////////////////////////////////////
// Definition of the class YieldCurve
//////////////////////////////////////////
YieldCurve::YieldCurve(std::vector<InstrumentDefinition *>& instrDefs, double compoundFreq):
    _compoundFreq(compoundFreq)
{
    _instrValues = NULL;
    _instrDefs = instrDefs;

    // Sort instrument definitions
    sort(_instrDefs.begin(), _instrDefs.end(),
            InstrumentDefinition::ptrcmp);

    // Build mapping between instrument index to 
    // the instrument definition location in the vector
    std::map<int, int>().swap(_instrDefIndicesMap);
    for(int i = 0; i < (int)_instrDefs.size(); i ++)
        _instrDefIndicesMap[i] = _instrDefs[i]->index();    
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
        if(_instrDefIndicesMap.find(val.first) == _instrDefIndicesMap.end())
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
ZeroCouponRateCurve::ZeroCouponRateCurve(std::vector<InstrumentDefinition *>& instrDefs, double compoundFreq):
    YieldCurve(instrDefs, compoundFreq)
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
