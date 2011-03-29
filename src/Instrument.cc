#include "Instrument.h"

//////////////////////////////////////////
// Definition of Instrument class
//////////////////////////////////////////
Instrument::Instrument(Date& date, double value):
    _date(date), _compIntRate(value)
{
}

Instrument::~Instrument()
{
}

//////////////////////////////////////////
// Definition of CASHInstr class
//////////////////////////////////////////
CASHInstr::CASHInstr(Date& date, double value):
    Instrument(date, value)
{
}

CASHInstr::~CASHInstr()
{
}

//////////////////////////////////////////
// Definition of FRAInstr class
//////////////////////////////////////////
FRAInstr::FRAInstr(Date& date, double value):
    Instrument(date, value)
{
}

FRAInstr::~FRAInstr()
{
}

//////////////////////////////////////////
// Definition of SWAPInstr class
//////////////////////////////////////////
SWAPInstr::SWAPInstr(Date& date, double value):
    Instrument(date, value)
{
}

SWAPInstr::~SWAPInstr()
{
}
