#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>

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
                    
                    std::pair<std::vector<InstrumentDefinition *>::iterator, std::vector<InstrumentDefinition*>::iterator> result;

                    // FIX: If this does not work, then
                    // Allocate the Fake Instrument Definition
                    // using the startDuration first,
                    // and then use InstrumentDefinitionCompare()
                    // as the comparator
                    // FIX: actually the upper bound can be 
                    // the current iterator instead of iterEnd
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
                    Duration maturityDuration(currInstrDef.maturity());
                    Duration deltaDuration(_compoundFreq,
                            Duration::YEAR);

                    // TODO: Add check that
                    // deltaDuration should be able to divide the 
                    // maturityDuration exactly

                    // Check all the necessary instrument, and
                    // add if there is no cooresponding definition
                    int n = floor(maturityDuration / deltaDuration);
                    for(int i = 1; i < n; i ++)
                    {
                        Duration currDuration = deltaDuration * i;

                        std::pair<std::vector<InstrumentDefinition *>::iterator, std::vector<InstrumentDefinition *>::iterator> result;

                        result = std::equal_range(iterBegin, iterEnd,
                                &currDuration,
                                InstrumentDefinitionDurationCompare());

                        if(result.first != result.second)
                            break;

                        InstrumentDefinition *ptrNewInstrDef = 
                            new FAKEInstrDefinition(currDuration, -1);
                        _instrDefs.insert(result.first, ptrNewInstrDef);
                    }
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
        double deltaTToMaturity;

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
                        deltaTToMaturity = deltaT;

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
                        deltaTToMaturity = normDiffDate(today, maturityDate,
                                Date::ACT365);

                        double dfStart = ptrNewInstance->getDf(startDate);
                        df = dfStart / (1.0f + compRate * deltaT);
                        break;
                    }
                case InstrumentDefinition::SWAP:
                    {
                        Duration maturityDuration(instrDef.maturity());
                        Duration deltaDuration(_compoundFreq,
                                Duration::YEAR);

                        Date prevDate = today;
                        double sumDeltaTxDf = 0.0; 
                        int n = floor(maturityDuration / deltaDuration);
                        for(int i = 1; i <= n; i ++)
                        {
                            Duration currDuration = deltaDuration * i;
                            Date currDate = today + currDuration;

                            deltaT = normDiffDate(prevDate, currDate,
                                    Date::ACT365);

                            if(i >= n)
                                break;
                            
                            df = ptrNewInstance->getDf(currDate);
                            sumDeltaTxDf += deltaT * df;

                            prevDate = currDate;
                        }

                        df = (1.0 - compRate * sumDeltaTxDf) / 
                             (1.0 + compRate * deltaT);

                        deltaTToMaturity = normDiffDate(today, maturityDate,
                                Date::ACT365);
                        
                        break;
                    }
                default:
                    // TODO: Throw switch unreachable error
                    break;
            }

            CurvePoint_t point(maturityDate, df, deltaTToMaturity);
            ptrNewInstance->insert(point);
        }
        else
        {
        }
    }

    return ptrNewInstance;
}

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
