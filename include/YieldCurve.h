#ifndef _INCLUDE_YIELDCURVE_H_
#define _INCLUDE_YIELDCURVE_H_

#include <vector>

class Date; // Forward Declaration of Date class in "Date.h"
class Instrument; // Forward Declaration of Instrument class in "Instrument.h"

class PointOnCurve
{
    public:
        PointOnCurve(Date& date, double value);
        ~PointOnCurve();

    private:
        Date& _date;
        int _time; // time since today in terms of days
        double _value;
};


class PointOnZeroCouponRateCurve:
    public PointOnCurve
{
    public:
        PointOnZeroCouponRateCurve(Date& date, double value);
        ~PointOnZeroCouponRateCurve();
};

class PointOnDiscountFactorCurve:
    public PointOnCurve
{
    public:
        PointOnDiscountFactorCurve(Date& date, double value);
        ~PointOnDiscountFactorCurve();
};

class YieldCurve
{
    public:
        explicit YieldCurve(std::vector<Instrument>& inputInstruments);
        ~YieldCurve();

        virtual void updateCurve() = 0;
        virtual PointOnCurve operator[](int daysAfter) = 0;
        virtual PointOnCurve operator[](Date& date) = 0;

        
    private:
        std::vector<Instrument>& _inputInstruments;
        bool _autoUpdateCurve;
};

class DiscountFactorCurve:
    public YieldCurve
{
    public:
        DiscountFactorCurve(std::vector<Instrument>& inputInstruments);
        ~DiscountFactorCurve();

        virtual void updateCurve();
    private:
        std::vector<PointOnDiscountFactorCurve> _points;
};

class ZeroCouponRateCurve:
    public YieldCurve
{
    public:
        ZeroCouponRateCurve(std::vector<Instrument>& inputInstruments,
                DiscountFactorCurve& discountFactorCurve);
        ~ZeroCouponRateCurve();

        virtual void updateCurve();
    private:
        DiscountFactorCurve& _discountFactorCurve;
        std::vector<PointOnZeroCouponRateCurve> _points;
};



#endif //_INCLUDE_YIELDCURVE_H_
