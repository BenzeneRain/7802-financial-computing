#include "YieldCurve.h"

//////////////////////////////////////////
// Definition of the class PointOnCurve
//////////////////////////////////////////
PointOnCurve::PointOnCurve(Date& date, double value):
    _date(date), _value(value)
{
}

PointOnCurve::~PointOnCurve()
{
}

//////////////////////////////////////////
// Definition of the class PointOnZeroCouponRateCurve
//////////////////////////////////////////
PointOnZeroCouponRateCurve::PointOnZeroCouponRateCurve(Date& date, double value):
    PointOnCurve(date, value)
{
}

PointOnZeroCouponRateCurve::~PointOnZeroCouponRateCurve()
{
}

//////////////////////////////////////////
// Definition of the class PointOnDiscountFactorCurve
//////////////////////////////////////////
PointOnDiscountFactorCurve::PointOnDiscountFactorCurve(Date& date, double value):
    PointOnCurve(date, value)
{
}

PointOnDiscountFactorCurve::~PointOnDiscountFactorCurve()
{
}

//////////////////////////////////////////
// Definition of the class YieldCurve
//////////////////////////////////////////
YieldCurve::YieldCurve(std::vector<Instrument>& inputInstruments):
    _inputInstruments(inputInstruments)
{
}

YieldCurve::~YieldCurve()
{
}

//////////////////////////////////////////
// Definition of the class ZeroCouponRateCurve
//////////////////////////////////////////
ZeroCouponRateCurve::ZeroCouponRateCurve(std::vector<Instrument>& inputInstruments,
        DiscountFactorCurve& discountFactorCurve):
    _discountFactorCurve(discountFactorCurve), YieldCurve(inputInstruments)
{
}

ZeroCouponRateCurve::~ZeroCouponRateCurve()
{
}

void ZeroCouponRateCurve::updateCurve()
{
}

//////////////////////////////////////////
// Definition of the class DiscountFactorCurve
//////////////////////////////////////////
DiscountFactorCurve::DiscountFactorCurve(std::vector<Instrument>& inputInstruments):
    YieldCurve(inputInstruments)
{
}

DiscountFactorCurve::~DiscountFactorCurve()
{
}

void DiscountFactorCurve::updateCurve()
{
}
