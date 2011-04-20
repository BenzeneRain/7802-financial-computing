#ifndef _INCLUDE_STOCK_H_
#define _INCLUDE_STOCK_H_

#include <vector>
#include <utility>

#include "Date.h"
#include "YieldCurve.h"

namespace Stock
{
    namespace PricePredictionModel
    {
        template<class RNG>
        std::vector<std::pair<Date, double> >
            MonteCarloSimulation(double startPrice,
                    Date& startDate, Duration& duration,
                    double numSteps, YieldCurveInstance& instYC,
                    double volatility, RNG instRNG); 

    }
}

template<class RNG>
std::vector<std::pair<Date, double> > Stock::PricePredictionModel::
MonteCarloSimulation(double startPrice, Date& startDate,
        Duration& duration, double numSteps, 
        YieldCurveInstance& instYC, double volatility, RNG instRNG)
{
    std::vector<std::pair<Date, double> > predictions(numSteps + 1);
    Date curveStartDate = instYC.startDate();
    // TODO: Sanity check that the start date of the price
    // should be at least the yield curve instance start date

    Duration deltaDuration = duration / numSteps;
    predictions[0] = std::pair<Date, double>(startDate, startPrice);

    double currPrice = startPrice;
    for(int i = 1; i <= numSteps; i ++)
    {
        Date futureDate = startDate + deltaDuration * i;
        double deltaT = normDiffDate(startDate, futureDate, 
                Date::ACT365);
        double deltaTForR = normDiffDate(curveStartDate, futureDate,
                Date::ACT365);
        
        // FIX: Not sure about it
        double futureDf = instYC[futureDate] ;
        double rate =  - log(futureDf) / deltaTForR;

        double futurePrice = currPrice * (1.0 + rate * deltaT + 
                instRNG() * volatility * sqrt(deltaT));

        predictions[i] = std::pair<Date, double>(
                futureDate, futurePrice);

        currPrice = futurePrice;
    }

    return predictions;
}

#endif
