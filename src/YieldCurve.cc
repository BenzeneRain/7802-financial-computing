#include <sstream>
#include <string>
#include <algorithm>

#include "YieldCurve.h"
#include "Instrument.h"
#include "Date.h"
#include "Utility.h"


//////////////////////////////////////////
// Definition of the class YieldCurveDefinition
//////////////////////////////////////////
YieldCurveDefinition::YieldCurveDefinition(std::vector<InstrumentDefinition *>& instrDefs, double compoundFreq):
    _compoundFreq(compoundFreq)
{
    _instrDefs = instrDefs;
    
    // TODO: check if there are at least two instrument 
    // Definitions, and one of them is O/N

    // Sort instrument definitions
    sort(_instrDefs.begin(), _instrDefs.end(), 
            InstrumentDefinitionCompare());

    //TODO: Add fake instrument definitions by
    // interpolation

    // Build mapping between instrument index to 
    // the instrument definition location in the vector
    std::map<int, int>().swap(_instrDefIndicesMap);
    for(int i = 0; i < (int)_instrDefs.size(); i ++)
        _instrDefIndicesMap[_instrDefs[i]->index()] = i;    
}

YieldCurveDefinition::~YieldCurveDefinition()
{
}

void YieldCurveDefinition::_insertFakeInstrumentDefs()
{
    std::vector<InstrumentDefinition *>::iterator iterCurr =
        _instrDefs.begin();

    std::vector<InstrumentDefinition *>::iterator iterBegin =
        _instrDefs.begin();
    std::vector<InstrumentDefinition *>::iterator iterEnd =
        _instrDefs.end();

    iterCurr ++;

    while(iterCurr != _instrDefs.end())
    {
        InstrumentDefinition& currInstrDef =
            *(static_cast<InstrumentDefinition *>(*iterCurr));


        switch(currInstrDef.type())
        {
            case InstrumentDefinition::CASH:
                // We do not need to interpolate
                // for CASH instrument
                break;
            case InstrumentDefinition::FRA:
                {
                    // We need the df of the start Date
                    Duration startDuration(
                            dynamic_cast<FRAInstrDefinition&>
                            (currInstrDef).startDuration()
                            );
                    
                    std::pair<std::vector<InstrumentDefinition *>::iterator, 
                        std::vector<InstrumentDefinition*>::iterator> result;

                    // FIX: If this does not work, then
                    // Allocate the Fake Instrument Definition
                    // using the startDuration first,
                    // and then use InstrumentDefinitionCompare()
                    // as the comparator
                    result = std::equal_range(iterBegin, iterEnd,
                            &startDuration, 
                            InstrumentDefinitionDurationCompare());

                    // The definition is already there
                    // So break, no need to interpolate
                    if(result.first != result.second)
                        break;


                    // If the definition is not found
                    // Insert this definition
                    // Mark the index as -1 to indicate
                    // this is a manually inserted fake
                    // Instrument Definition
                    InstrumentDefinition *ptrNewInstrDef = 
                        new FAKEInstrDefinition(startDuration, -1);
                    _instrDefs.insert(result.first, ptrNewInstrDef);  
                    break;
                }
            case InstrumentDefinition::SWAP:
                {
                    // TODO:
                    break;
                }
            default:
                {
                    std::string errorMessage("Invalid Instrument"
                            "Definition Type met in ");
                    errorMessage += __func__;
                    throw YieldCurveException(errorMessage);
                }
        }

        iterCurr ++;
    }
}

YieldCurveInstance* YieldCurveDefinition::bindData(
        InstrumentValues *instrVals,
        YieldCurveDefinition::CURVETYPE type)
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

    // TODO: We also need to make sure there are at least
    // two values, and one is O/N

    // Allocate new Yield Curve Instance
    YieldCurveInstance *ptrNewInstance = NULL;
    switch(type)
    {
        case YieldCurveDefinition::ZEROCOUPONRATE:
            ptrNewInstance = new ZeroCouponRateCurve(_compoundFreq);
            break;
        default:
            {
                std::string errorMessage("Invalid Yield Curve"
                        " Instance Type");
                throw YieldCurveException(errorMessage);
            }
    }


    // map from the vector index of the instrument definition
    // to the instrument value index
    std::map<int, int> instrValIndicesMap;

    // last instrument definition that has value
    int lastInstrDefID = -1;

    for(int i = 0; i < (int)instrVals->values.size(); i ++)
    {
        std::pair<int, double>& val = instrVals->values[i];
        
        instrValIndicesMap[_instrDefIndicesMap[val.first]] = i;
        if(_instrDefIndicesMap[val.first] > lastInstrDefID)
            lastInstrDefID = _instrDefIndicesMap[val.first];
    }

    // TODO: lastInstrDefID should never be -1, but may be we need to
    // implement a check later


    // TODO: Un-optimized brute force loop
    // Loop start
    Date today = Date::today(Date::ACT365);
    for(int i = 0; i < lastInstrDefID; i ++)
    {
        InstrumentDefinition& instrDef = *_instrDefs[i];
        double df;
        double deltaT;

        std::map<int, int>::iterator iter;
        // If the definition has possible value
        if((iter = instrValIndicesMap.find(i)) !=
                instrValIndicesMap.end())
        {
            Date maturityDate = today + instrDef.maturity();
            double compRate = instrVals->values[(*iter).second].second;
            
            switch(instrDef.type())
            {
                case InstrumentDefinition::CASH:
                    {
                        deltaT = normDiffDate(today, maturityDate,
                                Date::ACT365);
                        df = 1.0f / (1.0f + compRate * deltaT);
                        break;
                    }
                case InstrumentDefinition::FRA:
                    {
                        Duration startDuration = 
                            dynamic_cast<FRAInstrDefinition&>(instrDef).startDuration();
                        Date startDate = today + startDuration;

                        deltaT = normDiffDate(startDate, maturityDate,
                                Date::ACT365);

                        double dfStart = ptrNewInstance->getDf(startDate);
                        df = dfStart / (1.0f + compRate * deltaT);
                        break;
                    }
                case InstrumentDefinition::SWAP:
                    {
                        break;
                    }
                default:
                    // TODO: Throw switch unreachable error
                    break;
            }

            CurvePoint_t point(maturityDate, df, deltaT);
            ptrNewInstance->insert(point);
        }
        else
        {
        }
    }

    return ptrNewInstance;
}

//double YieldCurveDefinition::operator[](Date& date) const
//{
//    std::map<Date, int>::const_iterator iter =
//        _curveDataIndicesMap.find(date);
//
//    if(iter == _curveDataIndicesMap.end())
//    {
//        // Use linear interpolation to 
//        // calculate the value
//        CurveDataType fakeData(date, 0.0);
//
//        std::vector<CurveDataType>::const_iterator low, high;
//
//        low = std::upper_bound(
//                _curveData.begin(), _curveData.end(),
//                fakeData, CurveDataCompare());
//        high = std::lower_bound(
//                _curveData.begin(), _curveData.end(),
//                fakeData, CurveDataCompare());
//
//        double zVal = Interpolation::linearInterpolation
//            (*low, *high, (const Date&)date);
//
//        return zVal;
//    }
//    else
//    {
//        return _curveData[iter->second].second;
//    }
//}

//int YieldCurveDefinition::operator[](Date& date)
//{
//    std::map<Date, int>::iterator iter =
//        _curveDataIndicesMap.find(date);
//
//    if(iter == _curveDataIndicesMap.end())
//    {
//        _curveData.push_back(std::pair<Date, double>(Date(date), 0));
//        int index = (int)_curveData.size() - 1;
//        _curveDataIndicesMap[date] = index; 
//
//        return index;
//    }
//    else
//    {
//        return iter->second;
//    }
//}

//void YieldCurveDefinition::_insertCurveData(Date& date, double zVal, int instrDefIndex)
//{
//    // TODO: Before we decide to use this value,
//    // we actually need to check if their is another
//    // definition has the same maturity Date
//
//    unsigned int index = (*this)[date];
//    _curveData[index].second = zVal;
//    _curveDataToInstrDefMap[date] = instrDefIndex;
//
//}
//////////////////////////////////////////
// Definition of the class YieldCurveInstance
//////////////////////////////////////////
void YieldCurveInstance::insert(CurvePointDesc& data)
{
}

double YieldCurveInstance::operator[](Date& date)
{
    return 0.0;
}

double YieldCurveInstance::getDf(Date& date)
{
    double deltaT = 0;
    return _convertSpecificToDf(this->operator[](date), deltaT);
}

//////////////////////////////////////////
// Definition of the class ZeroCouponRateCurve
//////////////////////////////////////////
ZeroCouponRateCurve::ZeroCouponRateCurve(double compoundFreq):
    _compoundFreq(compoundFreq)
{
}

ZeroCouponRateCurve::~ZeroCouponRateCurve()
{
}
