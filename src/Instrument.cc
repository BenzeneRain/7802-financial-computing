#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include "Instrument.h"
#include "Date.h"

//////////////////////////////////////////
// Definition of Instrument class
//////////////////////////////////////////
Instrument::Instrument(std::string maturity, int index):
    _maturity(maturity), _index(index)
{
}

Instrument::~Instrument()
{
}

Instrument* Instrument::parseString(std::string& instrDefStr)
{
    try
    {
        std::string instrumentType;
        std::string dateStr;
        int id;

        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep(",");
        tokenizer tokens(instrDefStr, sep);

        tokenizer::iterator tokIter = tokens.begin();
        instrumentType = *tokIter;

        tokIter ++;
        dateStr = *tokIter;
        
        tokIter ++;
        id = boost::lexical_cast<int>(*tokIter);

        if(boost::iequals(instrumentType, std::string("CASH")))
        {
            // InstrumentType is CASH
            Instrument *instr = new CASHInstr(dateStr, id);
            return instr;
        }
        else if(boost::iequals(instrumentType, std::string("FRA")))
        {
            // InstrumentType is FRA
            boost::char_separator<char> FRADateSep("xX");
            tokenizer FRADateTokens(dateStr, FRADateSep);

            tokIter = FRADateTokens.begin();
            std::string startDuration = *tokIter;

            tokIter ++;
            std::string maturity = *tokIter;
            
            Instrument *instr = new FRAInstr(startDuration, maturity, id);
            return instr;
        }
        else if(boost::iequals(instrumentType, std::string("SWAP")))
        {
            // InstrumentType is SWAP
            Instrument *instr = new SWAPInstr(dateStr, id);
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

}


//////////////////////////////////////////
// Definition of CASHInstr class
//////////////////////////////////////////
CASHInstr::CASHInstr(std::string& maturity, int index):
    Instrument(maturity, index)
{
}

CASHInstr::~CASHInstr()
{
}

std::string CASHInstr::subtype() const
{
}
//////////////////////////////////////////
// Definition of FRAInstr class
//////////////////////////////////////////
FRAInstr::FRAInstr(std::string& startDuration,
        std::string& maturity, int index):
    _startDuration(startDuration), Instrument(maturity, index)
{
}

FRAInstr::~FRAInstr()
{
}

std::string FRAInstr::subtype() const
{
}

//////////////////////////////////////////
// Definition of SWAPInstr class
//////////////////////////////////////////
SWAPInstr::SWAPInstr(std::string& maturity, int index):
    Instrument(maturity, index)
{
}

SWAPInstr::~SWAPInstr()
{
}

std::string SWAPInstr::subtype() const
{
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
