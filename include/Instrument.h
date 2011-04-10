#ifndef _INCLUDE_INSTRUMENT_H_
#define _INCLUDE_INSTRUMENT_H_

#include <string>

#include "Date.h"

// Classes
class InstrumentDefinition
{
    public:
        ~InstrumentDefinition();

        static InstrumentDefinition* parseString(std::string& instrDefStr);

        enum TYPE {CASH, FRA, SWAP};
        inline int index() const {return _index;}

        inline enum TYPE type() const {return _type;}
        inline Duration maturity() const {return _maturity;}

        virtual std::string subtype() const = 0;

        // HELPER FUNCTION for SORTING InstrumentDefinition
        static bool cmp(const InstrumentDefinition& i1,
                const InstrumentDefinition& i2);
        static bool ptrcmp(const InstrumentDefinition* pi1,
                const InstrumentDefinition* pi2);


    protected:
        InstrumentDefinition(Duration& maturity, int index);

        int _index;
        Duration  _maturity;
        enum TYPE _type;
};

class CASHInstrDefinition:
    public InstrumentDefinition
{
    public:
        CASHInstrDefinition(Duration& maturity, int index);
        ~CASHInstrDefinition();

        virtual std::string subtype() const;
};

class FRAInstrDefinition:
    public InstrumentDefinition
{
    public:
        static const Duration::TYPE defaultDurationType;
        FRAInstrDefinition(Duration& startDuration, Duration& maturity, int index);
        ~FRAInstrDefinition();

        virtual std::string subtype() const;
    protected:
        Duration _startDuration;
};

class SWAPInstrDefinition:
    public InstrumentDefinition

{
    public:
        SWAPInstrDefinition(Duration& maturity, int index);
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
