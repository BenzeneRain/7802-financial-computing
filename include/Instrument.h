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
        virtual std::string subtype() const = 0;
    protected:
        Instrument(std::string maturity, int index);

        double _compIntRate;
        int _index;
        std::string  _maturity;
};

class CASHInstr:
    public Instrument
{
    public:
        CASHInstr(std::string& maturity, int index);
        ~CASHInstr();

        virtual std::string subtype() const;
};

class FRAInstr:
    public Instrument
{
    public:
        FRAInstr(std::string& startDuration, std::string& maturity, int index);
        ~FRAInstr();

        virtual std::string subtype() const;
    protected:
        std::string _startDuration;
};

class SWAPInstr:
    public Instrument

{
    public:
        SWAPInstr(std::string& maturity, int index);
        ~SWAPInstr();

        virtual std::string subtype() const;
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
