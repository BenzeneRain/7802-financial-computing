#ifndef _INCLUDE_UTILITY_H_
#define _INCLUDE_UTILITY_H_

#include <utility>
#include <functional>

#include "Date.h"

struct Interpolation
{
    Interpolation();
    ~Interpolation();

    static inline double
    linearInterpolation(
            const std::pair<Date, double>& p1,
            const std::pair<Date, double>& p2,
            const Date& xVal)
    {
        Duration xToP1 = xVal - p1.first;
        Duration p2ToP1 = p2.first - p1.first;

        return p1.second + xToP1.getDuration(Duration::DAY) * 
            (p2.second - p1.second) / p2ToP1.getDuration(Duration::DAY);
    }

    static inline double
    linearInterpolation(
            const Date& p1Date, double p1Val,
            const Date& p2Date, double p2Val,
            const Date& xVal)
    {
        Duration xToP1 = xVal - p1Date;
        Duration p2ToP1 = p2Date - p1Date;

        return p1Val + xToP1.getDuration(Duration::DAY) *
            (p2Val - p1Val) / p2ToP1.getDuration(Duration::DAY);
    }

    template<class XAXIS, class YAXIS> static inline
    YAXIS linearInterpolation(
            const std::pair<XAXIS, YAXIS>& p1,
            const std::pair<XAXIS, YAXIS>& p2,
            const XAXIS& xVal); 
};


template<class XAXIS, class YAXIS>
YAXIS Interpolation::linearInterpolation(
        const std::pair<XAXIS, YAXIS>& p1,
        const std::pair<XAXIS, YAXIS>& p2,
        const XAXIS& xVal)
{
    return p1.second + YAXIS(
            ((p2.second - p1.second) / (YAXIS)(p2.first - p1.first)) * 
              (YAXIS)(xVal - p1.first));
}


namespace Volatility {
    class FormulaClass
    {
        public:
            // this is f(x)
            virtual double f(double x) const = 0;
            // this is f'(x)
            virtual double fprime(double x) const = 0;
            // should provide a function for initial value guessing
            virtual double getInitialGuess() const = 0;
    };

    class VolatilityFromEuroCallPriceFormula:
        public FormulaClass
    {
        public:
            // _S: current stock price
            // _K: strike price
            // _T: time to expiration
            // _r: continues time risk free rate
            // _C: European call price
            VolatilityFromEuroCallPriceFormula(double S, double K,
                    double T, double r, double C):
                _S(S), _K(K), _T(T), _r(r), _C(C){};
            // f(x) = C - S*N(d1(x)) - K*e^{-rT}*N(d2(x))
            // d1(x) = (ln(S/K) + (r + x^2 / 2) * T) / (x * T^(1/2))
            // d2(x) = d1(x) - x * T^(1/2)
            virtual double f(double x) const;
            virtual double fprime(double x) const;
            virtual double getInitialGuess() const;

        private:
            double _N(double x) const;
            double _Nprime(double x) const;
            // this is exactly d1(x)
            double _g(double x) const;
            double _gprime(double x) const;

            double _S, _K, _T, _r, _C;
    };

    struct NewtonRaphsonMethod:
        public std::binary_function<FormulaClass, double, double>
    {
        double operator()(const FormulaClass& formula,
                double maxError) const;
    };

}

#endif // _INCLUDE_UTILITY_H_
