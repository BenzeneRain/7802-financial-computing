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

    // Add fake instrument definitions by
    // interpolation
    _insertFakeInstrumentDefs();

    // Build mapping between instrument index to 
    // the instrument definition location in the vector
    std::map<int, int>().swap(_instrDefIndicesMap);
    for(int i = 0; i < (int)_instrDefs.size(); i ++)
    {
        int index = _instrDefs[i]->index();
        
        if(index >= 0)
            _instrDefIndicesMap[_instrDefs[i]->index()] = i;    
    }
}

YieldCurveDefinition::~YieldCurveDefinition()
{
    // Remove all the fake instrument
}

void YieldCurveDefinition::_insertFakeInstrumentDefs()
{
    std::vector<InstrumentDefinition *> instrDefsCopy(_instrDefs);

    std::vector<InstrumentDefinition *>::iterator iterCurr =
        _instrDefs.begin();

    iterCurr ++;

    while(iterCurr != _instrDefs.end())
    {
        InstrumentDefinition& currInstrDef = *(*iterCurr);


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
                    InstrumentDefinition *stubInstr = new FAKEInstrDefinition(
                            startDuration, -1);

                    // FIX: actually the upper bound can be 
                    // the current iterator instead of iterEnd
                    result = std::equal_range(instrDefsCopy.begin(), instrDefsCopy.end(),
                            stubInstr, InstrumentDefinitionCompare());

                    delete stubInstr;

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
                    instrDefsCopy.insert(result.first, ptrNewInstrDef);  
                    break;
                }
            case InstrumentDefinition::SWAP:
                {
                    Duration maturityDuration(currInstrDef.maturity());
                    Duration deltaDuration(Duration(1, Duration::YEAR) / _compoundFreq);

                    // TODO: Add check that
                    // deltaDuration should be able to divide the 
                    // maturityDuration exactly

                    // Check all the necessary instrument, and
                    // add if there is no cooresponding definition
                    int n = floor(maturityDuration / deltaDuration);
                    for(int i = 1; i < n; i ++)
                    {
                        Duration currDuration = deltaDuration * i;

                        InstrumentDefinition *stubInstr = new FAKEInstrDefinition(
                                currDuration, -1);

                        std::pair<std::vector<InstrumentDefinition *>::iterator,
                            std::vector<InstrumentDefinition *>::iterator> result;

                        result = std::equal_range(instrDefsCopy.begin(),
                                instrDefsCopy.end(), stubInstr,
                                InstrumentDefinitionCompare());

                        delete stubInstr;

                        if(result.first != result.second)
                            continue;

                        InstrumentDefinition *ptrNewInstrDef = 
                            new FAKEInstrDefinition(currDuration, -1);
                        instrDefsCopy.insert(result.first, ptrNewInstrDef);  
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

    _instrDefs.swap(instrDefsCopy);
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

    Date today = Date::today();

    // TODO: We also need to make sure there are at least
    // two values, and one is O/N

    // Allocate new Yield Curve Instance
    YieldCurveInstance *ptrNewInstance = NULL;
    switch(type)
    {
        case YieldCurveDefinition::ZEROCOUPONRATE:
            ptrNewInstance = new ZeroCouponRateCurve(
                    _compoundFreq, today);
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
    std::vector<int> instrDefHasValue(_instrDefs.size());
    // last instrument definition that has value
    int lastInstrDefID = -1;

    for(int i = 0; i < (int)instrVals->values.size(); i ++)
    {
        std::pair<int, double>& val = instrVals->values[i];
        int instrDefIndex = _instrDefIndicesMap[val.first];

        instrValIndicesMap[instrDefIndex] = i;
        instrDefHasValue[instrDefIndex] = 1;

        if(instrDefIndex > lastInstrDefID)
            lastInstrDefID = instrDefIndex;
    }

    // TODO: lastInstrDefID should never be -1, but may be we need to
    // implement a check later

    
    int prevInstrDefHasValue = 0;
    int nextInstrDefHasValue = -1;
    for(int i = 0; i < _instrDefs.size(); i ++)
        if(instrDefHasValue[i] == 1)
        {
            nextInstrDefHasValue = i;
            break;
        }
    
    // TODO: Un-optimized brute force loop
    // Loop start
    for(int i = 0; i <= lastInstrDefID; i ++)
    {
        InstrumentDefinition& instrDef = *_instrDefs[i];
        double df;
        double deltaT;
        double deltaTToMaturity;
        double compRate;
        Date maturityDate = today + instrDef.maturity();

        if(i > nextInstrDefHasValue)
        {
            prevInstrDefHasValue = nextInstrDefHasValue;
            for(int j = prevInstrDefHasValue + 1; 
                    j <= lastInstrDefID; j ++)
                if(instrDefHasValue[j] == 1)
                {
                    nextInstrDefHasValue = j;
                    break;
                }
        }

        std::map<int, int>::iterator iter;
        if((iter = instrValIndicesMap.find(i)) !=
                instrValIndicesMap.end())
        {
            // If the definition has input compounding rate value
            compRate = instrVals->values[(*iter).second].second 
                / 100.0f;
        }
        else
        {
            // If we cannot find compounding rate value in its input,
            // then use linear-interpolation to generate this value
            std::pair<Date, double> startPoint(today + _instrDefs[prevInstrDefHasValue]->maturity(), instrVals->values[instrValIndicesMap[prevInstrDefHasValue]].second);
            std::pair<Date, double> endPoint(today + _instrDefs[nextInstrDefHasValue]->maturity(), instrVals->values[instrValIndicesMap[nextInstrDefHasValue]].second);
            compRate = Interpolation::linearInterpolation(
                    startPoint, endPoint, maturityDate) / 100.0f;
        }

        InstrumentDefinition::TYPE instrDefType = instrDef.type();
        if(instrDefType == InstrumentDefinition::FAKE)
        {
            static const Duration threeMonth(1, Duration::MONTH);
            static const Duration oneYear(1, Duration::YEAR);

            if(instrDef.maturity() < threeMonth)
                instrDefType = InstrumentDefinition::CASH;
            else if(instrDef.maturity() < oneYear)
                instrDefType = InstrumentDefinition::FRA;
            else 
                instrDefType = InstrumentDefinition::SWAP;
        }


        switch(instrDefType)
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
                    Duration startDuration; 
                    if(instrDef.type() == InstrumentDefinition::FRA)
                    {
                        startDuration = 
                            dynamic_cast<FRAInstrDefinition&>(instrDef).startDuration();
                    }
                    else
                    {
                        startDuration = 
                            instrDef.maturity() - Duration(3, Duration::MONTH);
                    }

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
                    Duration deltaDuration(Duration(1, Duration::YEAR) / _compoundFreq);

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

        // Insert the point to the Curve
        CurvePoint_t point(maturityDate, df, deltaTToMaturity,
                instrDef.type());
        ptrNewInstance->insert(point);
    }

    return ptrNewInstance;
}

std::vector<InstrumentDefinition *> YieldCurveDefinition::getAllDefinitions() const
{
    std::vector<InstrumentDefinition *> allInstrDefs;

    for(int i = 0; i < (int)_instrDefs.size(); i ++)
    {
        allInstrDefs.push_back(_instrDefs[i]->clone());
    }

    return allInstrDefs;
}

InstrumentDefinition* 
YieldCurveDefinition::getDefinitionByID(int id)
{
    std::map<int, int>::iterator iter;
    if((iter = _instrDefIndicesMap.find(id)) 
            != _instrDefIndicesMap.end())
    {
        int vecIndex = (*iter).second;
        InstrumentDefinition* ptr = _instrDefs[vecIndex];

        return ptr->clone();
    }
    else
    {
        // FIX: Throw exception
        return NULL;
    }
}
//////////////////////////////////////////
// Definition of the class YieldCurveInstance
//////////////////////////////////////////
YieldCurveInstance::YieldCurveInstance(const Date& startDate):
    _startDate(startDate)
{
}

YieldCurveInstance::YieldCurveInstance(const YieldCurveInstance& rhs):
    _startDate(rhs._startDate)
{
    *this = this->operator=(rhs);
}

YieldCurveInstance& YieldCurveInstance::operator=(
        const YieldCurveInstance& rhs)
{
    std::vector<CurvePoint_t>(rhs._curveData).swap(_curveData); 
    std::map<Date, int>(rhs._curveDataIndicesMap).swap(_curveDataIndicesMap);
    _startDate = rhs._startDate;
    return *this;
}

void YieldCurveInstance::insert(CurvePoint_t& data)
{
    CurvePoint_t newData(data);

    newData.value = _convertDfToSpecific(
            newData.value, newData.deltaT);

    std::map<Date, int>::iterator iter;

    iter = _curveDataIndicesMap.find(newData.date);


    if(iter != _curveDataIndicesMap.end())
    {
        int index = (*iter).second;
        InstrumentDefinition::TYPE iterDataType = 
            _curveData[index].instrType;

        if(data.instrType == InstrumentDefinition::SWAP ||
                (data.instrType == InstrumentDefinition::FRA && 
                 iterDataType != InstrumentDefinition::SWAP) ||
                (data.instrType == InstrumentDefinition::CASH &&
                 iterDataType == InstrumentDefinition::CASH))
        {
            _curveData[index] = newData;
        }
    } 
    else
    {
        std::vector<CurvePoint_t>::iterator ptIter;

        ptIter = upper_bound(_curveData.begin(),
                _curveData.end(), newData);
        ptIter = _curveData.insert(ptIter, newData);

        _curveDataIndicesMap[newData.date] = distance(
                _curveData.begin(), ptIter);
    }
}

double YieldCurveInstance::operator[](Date& date) const
{
    std::pair<std::vector<CurvePoint_t>::const_iterator,
        std::vector<CurvePoint_t>::const_iterator> iterRange;

    CurvePoint_t fakePoint(date, 0, 0, InstrumentDefinition::FAKE);

    iterRange = equal_range(_curveData.begin(),
            _curveData.end(), fakePoint);
     
    int distUD = distance(iterRange.first, iterRange.second);
    
    if(distUD == 0)
    {
        std::string errorMessage("Cannot get the value on the "
                "Yield Curve of the giving Date. The date is "
                "out of the range.");

        throw YieldCurveException(errorMessage);
    }
    else
    {
        double value;
        CurvePoint_t lowerElement = *(iterRange.first);
        CurvePoint_t upperElement = *(iterRange.second);

        value = Interpolation::linearInterpolation(
                lowerElement.date, lowerElement.value,
                upperElement.date, upperElement.value,
                date);

        return value;
    }
}

double YieldCurveInstance::getDf(Date& date) const
{
    std::pair<std::vector<CurvePoint_t>::const_iterator,
        std::vector<CurvePoint_t>::const_iterator> iterRange;

    CurvePoint_t fakePoint(date, 0, 0, InstrumentDefinition::FAKE);

    iterRange = equal_range(_curveData.begin(),
            _curveData.end(), fakePoint);
     
    int distUD = distance(iterRange.first, iterRange.second);
    
    if(distUD == 0)
    {
        std::string errorMessage("Cannot get the value on the "
                "Yield Curve of the giving Date. The date is "
                "out of the range.");

        throw YieldCurveException(errorMessage);
    }
    else
    {
        double value;
        CurvePoint_t lowerElement = *(iterRange.first);
        CurvePoint_t upperElement = *(iterRange.second);

        value = Interpolation::linearInterpolation(
                lowerElement.date, lowerElement.value,
                upperElement.date, upperElement.value,
                date);

        Date today = Date::today();
        double deltaT = normDiffDate(today, date, Date::ACT365);
        value = _convertSpecificToDf(value, deltaT);

        return value;
    }
}

//////////////////////////////////////////
// Definition of the class ZeroCouponRateCurve
//////////////////////////////////////////
ZeroCouponRateCurve::ZeroCouponRateCurve(
        double compoundFreq, const Date& startDate):
    _compoundFreq(compoundFreq), YieldCurveInstance(startDate)
{
}

ZeroCouponRateCurve::ZeroCouponRateCurve(const ZeroCouponRateCurve& rhs):
    YieldCurveInstance(rhs)
{
    _compoundFreq = rhs._compoundFreq;
}

ZeroCouponRateCurve::~ZeroCouponRateCurve()
{
}

YieldCurveInstance& ZeroCouponRateCurve::operator=(YieldCurveInstance& rhs)
{
    ZeroCouponRateCurve& zrhs = static_cast<ZeroCouponRateCurve&>(rhs);
    *this = ZeroCouponRateCurve::operator=(zrhs);

    return *this;
}

ZeroCouponRateCurve& ZeroCouponRateCurve::operator=(ZeroCouponRateCurve& rhs)
{
    YieldCurveInstance::operator=(rhs);
    _compoundFreq = rhs._compoundFreq;

    return *this;
}

//////////////////////////////////////////
// Definition of the struct CurvePointDesc
//////////////////////////////////////////

bool CurvePoint_t::operator<(const CurvePoint_t& rhs) const
{
    return (this->date < rhs.date);
}

//////////////////////////////////////////
// Definition of the miscellaneous non-member funcitons
//////////////////////////////////////////
double getCompoundRate(YieldCurveInstance& instYC, Date& theDate,
        InstrumentDefinition::TYPE type, double compoundFreq)
{
    Date today = Date::today();
    try
    {
        switch(type)
        {
            case InstrumentDefinition::CASH:
                {
                    double deltaT = normDiffDate(today, theDate,
                            Date::ACT365);

                    double compRate = 
                        (1.0f / instYC.getDf(theDate) - 1.0f) / 
                        deltaT;

                    return compRate * 100.0f;
                }
            case InstrumentDefinition::FRA:
                {
                    Date startDate = theDate - Duration(3, Duration::MONTH);
                    double dfStart = instYC.getDf(startDate);
                    double dfMature = instYC.getDf(theDate);
                    
                    double deltaT = normDiffDate(startDate, theDate,
                            Date::ACT365);

                    double compRate = (dfStart / dfMature - 1.0f) / deltaT;
                    return compRate * 100.0f;
                }
            case InstrumentDefinition::SWAP:
                {
                    Duration maturityDuration(theDate - today);
                    Duration deltaDuration(Duration(1, Duration::YEAR) / compoundFreq);

                    int n = floor(maturityDuration / deltaDuration);

                    double sumDeltaTxDf = 0;
                    double dfn;
                    Date prevDate = today;
                    for(int i = 1; i <= n; i ++)
                    {
                        Duration currDuration = deltaDuration * i;
                        Date currDate = today + currDuration;
                        double deltaT = normDiffDate(prevDate,
                                currDate, Date::ACT365);
                        double df = instYC.getDf(currDate);
                        sumDeltaTxDf += deltaT * df;
                        if(i == n)
                            dfn = df;

                        prevDate = currDate;
                    }

                    double compRate = (1.0f - dfn) / sumDeltaTxDf;
                    return compRate * 100.0f;
                }
            default:
                {
                    std::string errorMessage("Invalid Instrument"
                            "Definition TYPE");
                    throw YieldCurveException(errorMessage);
                }
        }
    }
    catch(YieldCurveException& e)
    {
        throw e;
    }
}
