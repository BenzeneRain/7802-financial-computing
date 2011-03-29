#ifndef _INCLUDE_INSTRUMENT_H_
#define _INCLUDE_INSTRUMENT_H_

#include <string>

class Date; // Forward Declaration of Date class in "Date.h"

class Instrument
{
    public:
        ~Instrument();

        static Instrument& parseString(std::string instrDefStr);
        
    protected:
        Instrument(Date& date, double value);

        double _compIntRate;
        Date& _date;
};

class CASHInstr:
    public Instrument
{
    public:
        CASHInstr(Date& date, double value);
        ~CASHInstr();
};

class FRAInstr:
    public Instrument
{
    public:
        FRAInstr(Date& date, double value);
        ~FRAInstr();
};

class SWAPInstr:
    public Instrument

{
    public:
        SWAPInstr(Date& date, double value);
        ~SWAPInstr();
};

#endif // _INCLUDE_INSTRUMENT_H_
