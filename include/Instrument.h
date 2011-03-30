#ifndef _INCLUDE_INSTRUMENT_H_
#define _INCLUDE_INSTRUMENT_H_

#include <string>

// Forward Declarations
class Date; 
class ACT365;

// Classes
class Instrument
{
    public:
        ~Instrument();

        static Instrument* parseString(std::string& instrDefStr);

        inline void rate(double value){_compIntRate = value;}
        inline double rate() const {return _compIntRate;} 

        inline int index() const {return _index;}
        inline const Date& date() const {return _date;}
    protected:
        Instrument(Date& date, int index);

        double _compIntRate;
        int _index;
        Date& _date;
};

class CASHInstr:
    public Instrument
{
    public:
        CASHInstr(Date& date, int index);
        ~CASHInstr();
};

class FRAInstr:
    public Instrument
{
    public:
        FRAInstr(Date& date, int index);
        ~FRAInstr();
};

class SWAPInstr:
    public Instrument

{
    public:
        SWAPInstr(Date& date, int index);
        ~SWAPInstr();
};


class InstrumentException
{
    public:
        InstrumentException(std::string& errorStr);
        ~InstrumentException();

        inline std::string message(){return _errorMessage;}
    private:
        std::string _errorMessage;
};
#endif // _INCLUDE_INSTRUMENT_H_
