#ifndef _INCLUDE_YIELDCURVE_H_
#define _INCLUDE_YIELDCURVE_H_

#include <vector>
#include <map>
#include <utility>
#include <cmath>

class Date; // Forward Declaration of Date class in "Date.h"
class InstrumentDefinition; // Forward Declaration of InstrumentDefinition class in "Instrument.h"
class InstrumentValues; // Forwaed Declaration of InstrumentValues class in "Instrument.h"

class YieldCurve
{
    public:
        explicit YieldCurve(std::vector<InstrumentDefinition *>& instrDefs, double compoundFreq);
        ~YieldCurve();

        // Section about Instrument Definitions

        // Section about Instrument Values
        InstrumentValues* bindData(InstrumentValues *instrVals);

        // Section about Curve Data
        virtual void updateCurve() = 0;
        virtual std::pair<Date, double> operator[](std::string dateStr);
        virtual std::pair<Date, double> operator[](Date& date);
    protected:
        // map from the instrument id to the vector index
        // of the instrument definition
        std::map<int, int> _instrDefIndicesMap;

        // map from the vector index of the instrument definition
        // to the instrument value index
        std::map<int, int> _instrValIndicesMap;

        std::vector<InstrumentDefinition *> _instrDefs;
        InstrumentValues* _instrValues;
        double _compoundFreq;

        std::vector<std::pair<Date, double> > _curveData;
};

class ZeroCouponRateCurve:
    public YieldCurve
{
    public:
        ZeroCouponRateCurve(std::vector<InstrumentDefinition *>& instrDefs, double compoundFreq);
        ~ZeroCouponRateCurve();

        // Section about Instrument Definitions

        // Section about Instrument Values

        // Section about Curve Data
        
        virtual void updateCurve();

    private:
        // Section about Curve Data
        inline double dfToZ(double df, double deltaT)
        {
            return _compoundFreq * (
                    exp(-log(df) / (_compoundFreq * deltaT)) - 1.0f);
        }
        inline double ZTodf(double Z, double deltaT)
        {
            return exp(-1.0f * deltaT * _compoundFreq *
                    log(1.0f + Z / _compoundFreq));
        }
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
