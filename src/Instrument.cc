#include <algorithm>
#include <iostream>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include "Instrument.h"
#include "Date.h"

//////////////////////////////////////////
// Definition of Instrument class
//////////////////////////////////////////
Instrument::Instrument(Date& date, int index):
    _date(date), _index(index)
{
}

Instrument::~Instrument()
{
}

Instrument* Instrument::parseString(std::string& instrDefStr)
{
    std::istringstream iss(instrDefStr);

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

        ACT365 actDate(dateStr);
        Date& date = actDate;

        if(boost::iequals(instrumentType, std::string("CASH")))
        {
            // InstrumentType is CASH
            Instrument *instr = new CASHInstr(date, id);
            return instr;
        }
        else if(boost::iequals(instrumentType, std::string("FRA")))
        {
            // InstrumentType is FRA
            Instrument *instr = new FRAInstr(date, id);
            return instr;
        }
        else if(boost::iequals(instrumentType, std::string("SWAP")))
        {
            // InstrumentType is SWAP
            Instrument *instr = new SWAPInstr(date, id);
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
CASHInstr::CASHInstr(Date& date, int index):
    Instrument(date, index)
{
}

CASHInstr::~CASHInstr()
{
}

//////////////////////////////////////////
// Definition of FRAInstr class
//////////////////////////////////////////
FRAInstr::FRAInstr(Date& date, int index):
    Instrument(date, index)
{
}

FRAInstr::~FRAInstr()
{
}

//////////////////////////////////////////
// Definition of SWAPInstr class
//////////////////////////////////////////
SWAPInstr::SWAPInstr(Date& date, int index):
    Instrument(date, index)
{
}

SWAPInstr::~SWAPInstr()
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
