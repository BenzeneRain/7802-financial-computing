#ifndef _INCLUDE_YIELDCURVE_H_
#define _INCLUDE_YIELDCURVE_H_

#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include <functional>

class Date; // Forward Declaration of Date class in "Date.h"
struct DateCompare;
class InstrumentDefinition; // Forward Declaration of InstrumentDefinition class in "Instrument.h"
class InstrumentValues; // Forwaed Declaration of InstrumentValues class in "Instrument.h"

typedef std::pair<Date, double> CurveDataType;

class YieldCurveInstance;

class YieldCurveDefinition
{
    public:
        YieldCurveDefinition(std::vector<InstrumentDefinition *>& instrDefs, double compoundFreq);
        ~YieldCurveDefinition();

        // Section about Instrument Definitions

        // Section about Instrument Values
        enum CURVETYPE {ZEROCOUPONRATE};
        YieldCurveInstance* bindData(InstrumentValues *instrVals,
                YieldCurveDefinition::CURVETYPE type);

        // Section about Curve Data
        virtual void updateCurve() = 0;
        double operator[](Date& date) const;
    protected:
        // map from the instrument id to the vector index
        // of the instrument definition
        std::map<int, int> _instrDefIndicesMap;

        // map from the vector index of the instrument definition
        // to the instrument value index
        std::map<int, int> _instrValIndicesMap;

        // FIX: we need a way to better manage the pointers
        // maybe by using shared_ptr
        std::vector<InstrumentDefinition *> _instrDefs;
        InstrumentValues* _instrValues;
        double _compoundFreq;

        std::vector<std::pair<Date, double> > _curveData;

        // map from the Date to the vector index of 
        // _curveDate
        std::map<Date, int> _curveDataIndicesMap;

        // map from the Date to the vector index of
        // the corresponding Instrument Definition
        std::map<Date, int> _curveDataToInstrDefMap;

        int operator[](Date& date);
        void _insertCurveData(Date& date, double zVal, int instrDefIndex);
        void _insertFakeInstrumentDefs();

};

class YieldCurveInstance
{
    public:
        ~YieldCurveInstance(){};

        friend YieldCurveInstance* YieldCurveDefinition::bindData(
                InstrumentValues*,
                YieldCurveDefinition::CURVETYPE);
    protected:
        YieldCurveInstance(double compoundFreq):
            _compoundFreq(compoundFreq){};

        double _compoundFreq;
};

class ZeroCouponRateCurve:
    public YieldCurveInstance
{
    public:
        ZeroCouponRateCurve(double compoundFreq);
        ~ZeroCouponRateCurve();

        // Section about Instrument Definitions

        // Section about Instrument Values

        // Section about Curve Data
        
       // virtual void updateCurve();

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

        virtual std::pair<Date, double>& operator[](std::string dateStr);
        virtual std::pair<Date, double>& operator[](Date& date);
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

struct CurveDataCompare:
    public std::binary_function<CurveDataType, CurveDataType, bool>
{
    bool operator()(const CurveDataType& lhs, const CurveDataType& rhs) const;
};


#endif //_INCLUDE_YIELDCURVE_H_
