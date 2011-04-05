#include <sstream>
#include <string>
#include <algorithm>

#include "YieldCurve.h"
#include "Instrument.h"
#include "Date.h"
#include "Utility.h"


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

double YieldCurve::operator[](Date& date) const
{
    std::map<Date, int>::const_iterator iter =
        _curveDataIndicesMap.find(date);

    if(iter == _curveDataIndicesMap.end())
    {
        // TODO: Use linear interpolation to 
        // calculate the value
        CurveDataType fakeData(date, 0.0);

        std::vector<CurveDataType>::const_iterator low, high;

        low = std::upper_bound(
                _curveData.begin(), _curveData.end(),
                fakeData, CurveDataCompare());
        high = std::lower_bound(
                _curveData.begin(), _curveData.end(),
                fakeData, CurveDataCompare());

        double zVal = Interpolation::linearInterpolation
            <Date, double>(*low, *high, (const Date&)date);

        return zVal;
    }
    else
    {
        return _curveData[iter->second].second;
    }
}

int YieldCurve::operator[](Date& date)
{
    std::map<Date, int>::iterator iter =
        _curveDataIndicesMap.find(date);

    if(iter == _curveDataIndicesMap.end())
    {
        _curveData.push_back(std::pair<Date, double>(Date(date), 0));
        int index = (int)_curveData.size() - 1;
        _curveDataIndicesMap[date] = index; 

        return index;
    }
    else
    {
        return iter->second;
    }
}

void YieldCurve::_insertCurveData(Date& date, double zVal, int instrDefIndex)
{
    // TODO: Before we decide to use this value,
    // we actually need to check if their is another
    // definition has the same maturity Date

    unsigned int index = (*this)[date];
    _curveData[index].second = zVal;
    _curveDataToInstrDefMap[date] = instrDefIndex;

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
    std::map<Date, int>().swap(_curveDataIndicesMap);
    std::map<Date, int>().swap(_curveDataToInstrDefMap);

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

                        _insertCurveData(maturityDate, Z, i);
                        break;
                    }
                case InstrumentDefinition::FRA:
                    {
                        // TODO: Calculate maturity Date
                        Date maturityDate = Date::today(Date::ACT365);
                        // TODO: Calculate start Date
                        Date startDate = Date::today(Date::ACT365);

                        // TODO: Calcualte delte T between
                        // start Date and maturity Date
                        //deltaT = maturityDate - startDate;

                        double rate = _instrValues->values[valueIndex].second;
                        double dfMaturity;
                        double Z;

                        // TODO: Calculate the dfStart
                        double dfStart;

                        dfMaturity = dfStart / (1.0f + rate * deltaT);

                        // TODO: Re-calculate delta T, which is
                        // now the time between today to the
                        // maturity date

                        Z = dfToZ(dfMaturity, deltaT);

                        _insertCurveData(maturityDate, Z, i);
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

////////////////////////////////////////////
//// Definition of the struct CurveDataCompare
////////////////////////////////////////////
bool CurveDataCompare::operator()(const CurveDataType& lhs, const CurveDataType& rhs) const
{
    return lhs.first < rhs.first;
}


