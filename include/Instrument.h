#ifndef _INCLUDE_INSTRUMENT_H_
#define _INCLUDE_INSTRUMENT_H_

#include <string>
#include <functional>
#include <stdexcept>

#include "Date.h"

// Classes
class InstrumentDefinition
{
    public:
        ~InstrumentDefinition();

        static InstrumentDefinition* parseString(
                std::string& instrDefStr);

        enum TYPE {CASH = 0,
                   FRA  = 1,
                   SWAP = 2,
                   FAKE = 3};
        inline int index() const {return _index;}

        inline enum TYPE type() const {return _type;}
        inline Duration maturity() const {return _maturity;}

        virtual std::string subtype() const = 0;

        InstrumentDefinition* clone();

    protected:
        InstrumentDefinition(const Duration& maturity,
                const int index);
        InstrumentDefinition(const InstrumentDefinition& rhs);

        int _index;
        Duration  _maturity;
        enum TYPE _type;
};

struct InstrumentDefinitionCompare:
    public std::binary_function<InstrumentDefinition, 
    InstrumentDefinition, bool>
{
    bool operator()(const InstrumentDefinition& lhs,
            const InstrumentDefinition& rhs) const;

    bool operator()(const InstrumentDefinition* ptrlhs,
            const InstrumentDefinition* ptrlhs) const;
};

struct InstrumentDefinitionDurationCompare:
    public std::binary_function<InstrumentDefinition, 
    Duration, bool>
{
    bool operator()(const InstrumentDefinition& lhs,
            const Duration& rhs) const;

    bool operator()(const InstrumentDefinition* ptrlhs,
            const Duration* ptrrhs) const;

    bool operator()(const Duration& lhs,
            const InstrumentDefinition& rhs) const;

    bool operator()(const Duration* ptrlhs,
            const InstrumentDefinition* ptrrhs) const;
};


class CASHInstrDefinition:
    public InstrumentDefinition
{
    public:
        CASHInstrDefinition(Duration& maturity, int index);
        explicit CASHInstrDefinition(const CASHInstrDefinition& rhs):
            InstrumentDefinition(rhs){};

        ~CASHInstrDefinition();

        virtual std::string subtype() const;
};

class FRAInstrDefinition:
    public InstrumentDefinition
{
    public:
        static const Duration::TYPE defaultDurationType;
        FRAInstrDefinition(Duration& startDuration, Duration& maturity, int index);
        explicit FRAInstrDefinition(const FRAInstrDefinition& rhs):
            _startDuration(rhs._startDuration), 
            InstrumentDefinition(rhs){};

        ~FRAInstrDefinition();

        virtual std::string subtype() const;

        inline Duration startDuration() const
        {return _startDuration;}
    protected:
        Duration _startDuration;
};

class SWAPInstrDefinition:
    public InstrumentDefinition

{
    public:
        SWAPInstrDefinition(Duration& maturity, int index);
        explicit SWAPInstrDefinition(const SWAPInstrDefinition& rhs):
            InstrumentDefinition(rhs){};

        ~SWAPInstrDefinition();

        virtual std::string subtype() const;
};

// These instrument definition are not given 
// by user, but generated by system for
// assisting building the yield curve
class FAKEInstrDefinition:
    public InstrumentDefinition
{
    public:
        FAKEInstrDefinition(Duration& maturity, int index):
            InstrumentDefinition(maturity, index)
        {
            _type = InstrumentDefinition::FAKE;
        };
        explicit FAKEInstrDefinition(const FAKEInstrDefinition& rhs):
            InstrumentDefinition(rhs){};

        ~FAKEInstrDefinition();

        virtual std::string subtype() const;
};



class InstrumentValues
{
    public:
        InstrumentValues(){};
        ~InstrumentValues(){};


        // use pair(index, rate) to represent the Instrument values 
        std::vector<std::pair<int, double> > values;
};


class InstrumentException : public std::runtime_error
{
    public:
        InstrumentException(std::string& errorStr):
            std::runtime_error(errorStr){};
};
#endif // _INCLUDE_INSTRUMENT_H_
