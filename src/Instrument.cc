#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include "Instrument.h"
#include "Date.h"

//////////////////////////////////////////
// Definition of InstrumentDefinition class
//////////////////////////////////////////
InstrumentDefinition::InstrumentDefinition(Duration& maturity, int index):
    _maturity(maturity), _index(index)
{
}

InstrumentDefinition::~InstrumentDefinition()
{
}

bool InstrumentDefinition::cmp(const InstrumentDefinition& i1,
        const InstrumentDefinition& i2)
{
    int timeI1, timeI2;

    timeI1 = i1.maturity().getDuration(Duration::DAY);
    timeI2 = i2.maturity().getDuration(Duration::DAY);

    if(timeI1 == timeI2)
    {
        if((i1.type() == CASH &&
            (i2.type() == FRA || i2.type() == SWAP)) ||
                (i1.type() == FRA && i2.type() == SWAP))
            return true;
        else
            return false;
    }
    else
        return timeI1 < timeI2;

    return true;
}

bool InstrumentDefinition::ptrcmp(const InstrumentDefinition* pi1,
        const InstrumentDefinition* pi2)
{
    return InstrumentDefinition::cmp(*pi1, *pi2);
}

InstrumentDefinition* InstrumentDefinition::parseString(std::string& instrDefStr)
{
    try
    {
        std::string trimedString = instrDefStr;
        std::string instrumentType;
        std::string dateStr;
        int id;

        boost::trim_if(trimedString, boost::algorithm::is_any_of("\r\n"));

        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep(",");
        tokenizer tokens(trimedString, sep);

        tokenizer::iterator tokIter = tokens.begin();
        if(tokIter == tokens.end())
            throw InstrumentException(instrDefStr);
        instrumentType = *tokIter;

        tokIter ++;
        if(tokIter == tokens.end())
            throw InstrumentException(instrDefStr);
        dateStr = *tokIter;
        
        tokIter ++;
        if(tokIter == tokens.end())
            throw InstrumentException(instrDefStr);
        id = boost::lexical_cast<int>(*tokIter);

        tokIter ++;
        if(tokIter != tokens.end())
            throw InstrumentException(instrDefStr);

        if(boost::iequals(instrumentType, std::string("CASH")))
        {
            // InstrumentDefinitionType is CASH
            Duration duration(dateStr);
            InstrumentDefinition *instr = new CASHInstrDefinition(duration, id);
            return instr;
        }
        else if(boost::iequals(instrumentType, std::string("FRA")))
        {
            // InstrumentDefinitionType is FRA
            boost::char_separator<char> FRADateSep("xX");
            tokenizer FRADateTokens(dateStr, FRADateSep);

            tokIter = FRADateTokens.begin();
            if(tokIter == FRADateTokens.end())
                throw InstrumentException(instrDefStr);
            std::string startDurationStr = *tokIter;

            tokIter ++;
            if(tokIter == FRADateTokens.end())
                throw InstrumentException(instrDefStr);
            std::string maturityStr = *tokIter;
            
            tokIter ++;
            if(tokIter != FRADateTokens.end())
                throw InstrumentException(instrDefStr);

            // Check if the Unit is provided or not
            static const boost::regex allDigitsFormat("^[[:digit:]]+$");
            Duration *ptrStartDuration, *ptrMaturity;
            
            if(boost::regex_match(startDurationStr, allDigitsFormat))
            {
                double startDurationNum = boost::lexical_cast<double>(startDurationStr);
                Duration startDuration(startDurationNum, FRAInstrDefinition::defaultDurationType);
                ptrStartDuration = &startDuration;
            }
            else
            {
                Duration startDuration(startDurationStr);
                ptrStartDuration = &startDuration;
            }

            if(boost::regex_match(maturityStr, allDigitsFormat))
            {
                double maturityNum = boost::lexical_cast<double>(maturityStr);
                Duration maturity(maturityNum, FRAInstrDefinition::defaultDurationType);
                ptrMaturity = &maturity;
            }
            else
            {
                Duration maturity(maturityStr);
                ptrMaturity = &maturity;
            }

            InstrumentDefinition *instr = new FRAInstrDefinition(*ptrStartDuration, *ptrMaturity, id);
            return instr;
        }
        else if(boost::iequals(instrumentType, std::string("SWAP")))
        {
            // InstrumentType is SWAP
            Duration duration(dateStr);

            InstrumentDefinition *instr = new SWAPInstrDefinition(duration, id);
            return instr;
        }
        else
        {
            throw InstrumentException(instrDefStr);
        }
        
    }
    catch(std::istringstream::failure& e)
    {
        throw;
    }
    catch(boost::bad_lexical_cast& e)
    {
        throw InstrumentException(instrDefStr);
    }

}

//////////////////////////////////////////
// Definition of CASHInstrDefinition class
//////////////////////////////////////////
CASHInstrDefinition::CASHInstrDefinition(Duration& maturity, int index):
    InstrumentDefinition(maturity, index)
{
    _type = InstrumentDefinition::CASH;
}

CASHInstrDefinition::~CASHInstrDefinition()
{
}

std::string CASHInstrDefinition::subtype() const
{
    return _maturity.toString(true, true);
}
//////////////////////////////////////////
// Definition of FRAInstrDefinition class
//////////////////////////////////////////
const Duration::TYPE FRAInstrDefinition::defaultDurationType = Duration::MONTH;

FRAInstrDefinition::FRAInstrDefinition(Duration& startDuration,
        Duration& maturity, int index):
    _startDuration(startDuration), InstrumentDefinition(maturity, index)
{
    _type = InstrumentDefinition::FRA;
}

FRAInstrDefinition::~FRAInstrDefinition()
{
}

std::string FRAInstrDefinition::subtype() const
{
    return _startDuration.toString(false, false) + 
        "x" + _maturity.toString(false, false);
}

//////////////////////////////////////////
// Definition of SWAPInstrDefinition class
//////////////////////////////////////////
SWAPInstrDefinition::SWAPInstrDefinition(Duration& maturity, int index):
    InstrumentDefinition(maturity, index)
{
    _type = InstrumentDefinition::SWAP;
}

SWAPInstrDefinition::~SWAPInstrDefinition()
{
}

std::string SWAPInstrDefinition::subtype() const
{
    return _maturity.toString(true, true);
}

//////////////////////////////////////////
// Definition of InstrumentException class
//////////////////////////////////////////
InstrumentException::InstrumentException(std::string& str):
    _errorMessage(str)
{
}

InstrumentException::~InstrumentException()
{
}
