#include <sstream>
#include <string>

#include "YieldCurve.h"
#include "Instrument.h"
#include "Date.h"


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
        _instrDefIndicesMap[_instrDefs[i]->index()] = i;    
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

    std::map<int, int>().swap(_instrValIndicesMap);
    InstrumentValues* prevInstrValues = _instrValues;
    _instrValues = instrVals;

    for(int i = 0; i < (int)_instrValues->values.size(); i ++)
    {
        std::pair<int, double>& val = _instrValues->values[i];
        
        _instrValIndicesMap[_instrDefIndicesMap[val.first]] = i;
    }

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
    // Remove all the old curve data first
    std::vector<std::pair<Date, double> >().swap(_curveData);
    Date today = Date::today(Date::ACT365);
    
    // Assumption here is the _instrDefs is ordered

    for(int i = 0; i < (int)_instrDefs.size(); i ++)
    {
        if(_instrValIndicesMap.find(i) != _instrValIndicesMap.end())
        {
            int valueIndex = _instrValIndicesMap[i];
            InstrumentDefinition& instrDef = *_instrDefs[i];
            int deltaT = 0;
            // TODO: Calculate the point on the curve and insert
            // into curve data

            // TODO: Handle the instruments with different type
            // but have the same maturity date
            switch(instrDef.type())
            {
                case InstrumentDefinition::CASH:
                    {
                        // TODO: Calculate maturity Date
                        Date maturityDate = Date::today(Date::ACT365);
                        // TODO: calculate delta T
                        double rate = _instrValues->values[valueIndex].second;
                        double df = 1.0f / (1.0f + rate * deltaT);
                        double Z = dfToZ(df, deltaT);

                        _curveData.push_back(std::pair<Date, double>(maturityDate, Z));
                        break;
                    }
                case InstrumentDefinition::FRA:
                    {
                        break;
                    }
                case InstrumentDefinition::SWAP:
                    {
                        break;
                    }
                default:
                    {
                        // TODO: throw error
                        break;
                    }
            }
        }
        else
        {
            // No data available for this instrument
            // so just skip it.
        }
    }
}

////////////////////////////////////////////
//// Definition of the class YieldCurveException
////////////////////////////////////////////
YieldCurveException::~YieldCurveException()
{
}
