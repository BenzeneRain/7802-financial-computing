#include <cmath>
#include <string>
#include <sstream>
#include <stdexcept>
#include "Utility.h"

using namespace Volatility;
using namespace RandomNumberGenerator;

Interpolation::Interpolation()
{
}

Interpolation::~Interpolation()
{
}

double VolatilityFromEuroCallPriceFormula::f(double x) const
{
    return _C - _S * _N(_g(x)) + 
        _K * exp(-_r * _T) * _N(_g(x) - x * sqrt(_T));
}

double VolatilityFromEuroCallPriceFormula::fprime(double x) const
{
    return -_S * _Nprime(_g(x)) * _gprime(x) + 
        _K * exp(-_r * _T) * _Nprime(_g(x) - x * sqrt(_T)) * 
        (_gprime(x) - sqrt(_T));
}

double VolatilityFromEuroCallPriceFormula::getInitialGuess() const
{
    return (double)(rand() % 100 + 1) / 100.0;
}

double VolatilityFromEuroCallPriceFormula::_N(double x) const
{
    if(x < 0)
        return 1.0 - _N(-x);

    const double a1 = 0.319381530;
    const double a2 = -0.356563782;
    const double a3 = 1.781477937;
    const double a4 = -1.821255978;
    const double a5 = 1.330274429;
    const double k = 1.0 / (1.0 + 0.2316419 * x);
    return 1.0 - _Nprime(x) * (a1 * k + a2 * k * k + 
            a3 * k * k * k + a4 * k * k * k * k +
            a5 * k * k * k * k * k);
}

double VolatilityFromEuroCallPriceFormula::_Nprime(double x) const
{
    return exp( - x * x / 2.0) / sqrt(2 * acos(-1));
}

double VolatilityFromEuroCallPriceFormula::_g(double x) const
{
    return (log(_S / _K) + (_r + x * x / 2.0) * _T) / (x * sqrt(_T));
}

double VolatilityFromEuroCallPriceFormula::_gprime(double x) const
{
    return 0.5 * sqrt(_T) - _r * sqrt(_T) / (x * x) - 
        log(_S / _K) / (x * x * sqrt(_T));
}

double NewtonRaphsonMethod::operator()(const FormulaClass& formula,
        double maxError) const
{
    double currX, nextX;
    double fx, fpx;
    currX = formula.getInitialGuess();
    fx = formula.f(currX);

    while(fabs(fx) >= maxError)
    {
        fpx = formula.fprime(currX);
        if(fpx == 0)
        {
            std::ostringstream oss;
            oss << "f'(" << currX << ") == 0";
            std::string errorMessage(oss.str());
            throw std::runtime_error(errorMessage);
        }
        nextX = currX - fx / fpx;
        currX = nextX;
        fx = formula.f(currX);
    };

    return currX;
}


void boxMullerM2RNG::_genNumbers()
{
    double x;
    double y;
    double r;
    
    do
    {
        x = (double)(rand() % 10001) / 5000.0 - 1.0;
        y = (double)(rand() % 10001) / 5000.0 - 1.0;
        r = x * x + y * y;
    }while(r >= 1.0);

    double coef = sqrt(-2.0 * log(r) / r);
    _buffer[0] = x * coef;
    _buffer[1] = y * coef;
    _numItemInBuf = 1;

    return ;
}
