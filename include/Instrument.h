#ifndef _INCLUDE_INSTRUMENT_H_
#define _INCLUDE_INSTRUMENT_H_

#include <string>

// Classes
class InstrumentDefinition
{
    public:
        ~InstrumentDefinition();

        static InstrumentDefinition* parseString(std::string& instrDefStr);

        enum TYPES {CASH, FRA, SWAP};
        inline int index() const {return _index;}

        inline enum TYPES type() const {return _type;}
        virtual std::string subtype() const = 0;

    protected:
        InstrumentDefinition(std::string maturity, int index);

        int _index;
        std::string  _maturity;
        enum TYPES _type;
};

class CASHInstrDefinition:
    public InstrumentDefinition
{
    public:
        CASHInstrDefinition(std::string& maturity, int index);
        ~CASHInstrDefinition();

        virtual std::string subtype() const;
};

class FRAInstrDefinition:
    public InstrumentDefinition
{
    public:
        FRAInstrDefinition(std::string& startDuration, std::string& maturity, int index);
        ~FRAInstrDefinition();

        virtual std::string subtype() const;
    protected:
        std::string _startDuration;
};

class SWAPInstrDefinition:
    public InstrumentDefinition

{
    public:
        SWAPInstrDefinition(std::string& maturity, int index);
        ~SWAPInstrDefinition();

        virtual std::string subtype() const;
};


class InstrumentValues
{
    public:
        InstrumentValues();
        ~InstrumentValues();

        // use pair(index, rate) to represent the Instrument values 
        std::vector<std::pair<int, double> > values;
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
