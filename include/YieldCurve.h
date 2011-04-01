#ifndef _INCLUDE_YIELDCURVE_H_
#define _INCLUDE_YIELDCURVE_H_

#include <vector>
#include <map>
#include <utility>

class Date; // Forward Declaration of Date class in "Date.h"
class InstrumentDefinition; // Forward Declaration of InstrumentDefinition class in "Instrument.h"
class InstrumentValues; // Forwaed Declaration of InstrumentValues class in "Instrument.h"

class YieldCurve
{
    public:
        explicit YieldCurve(std::vector<InstrumentDefinition *>& instrDefs);
        ~YieldCurve();

        // Section about Instrument Definitions

        // Section about Instrument Values
        InstrumentValues* bindData(InstrumentValues *instrVals);

        // Section about Curve Data
        virtual void updateCurve() = 0;
        virtual std::pair<Date, double> operator[](std::string dateStr);
        virtual std::pair<Date, double> operator[](Date& date);
    protected:
        std::map<int, InstrumentDefinition *> _instrDefs;
        InstrumentValues* _instrValues;
};

class ZeroCouponRateCurve:
    public YieldCurve
{
    public:
        ZeroCouponRateCurve(std::vector<InstrumentDefinition *>& instrDefs);
        ~ZeroCouponRateCurve();

        // Section about Instrument Definitions

        // Section about Instrument Values

        // Section about Curve Data
        virtual void updateCurve();
};

class YieldCurveException
{
    public:
        YieldCurveException(std::string& message):
            _message(message){};
        ~YieldCurveException();

        inline std::string message() const
        {return _message;}
    private:
        std::string _message;
};

#endif //_INCLUDE_YIELDCURVE_H_
