#ifndef _INCLUDE_STOCK_H_
#define _INCLUDE_STOCK_H_

#include <vector>
#include <utility>
#include <string>
#include <stdexcept>

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
                    int numSteps, YieldCurveInstance& instYC,
                    double volatility, RNG instRNG); 

    }

    class StockException:
        public std::runtime_error
    {
        public:
            StockException(const std::string& errorStr):
                std::runtime_error(errorStr){};
    };

}

template<class RNG>
std::vector<std::pair<Date, double> > Stock::PricePredictionModel::
MonteCarloSimulation(double startPrice, Date& startDate,
        Duration& duration, int numSteps, 
        YieldCurveInstance& instYC, double volatility, RNG instRNG)
{
    std::vector<std::pair<Date, double> > predictions(numSteps + 1);
    Date curveStartDate = instYC.startDate();
    // Sanity check that the start date of the price
    // should be at least the yield curve instance start date

    if(startDate < curveStartDate)
    {
        std::string errorMessage("The start date of the simulation should be late than"
                " the Yield Curve start date");
        throw Stock::StockException(errorMessage);
    }


    Duration deltaDuration = duration / numSteps;
    predictions[0] = std::pair<Date, double>(startDate, startPrice);

    double currPrice = startPrice;
    Date lastDate = startDate;
    for(int i = 1; i <= numSteps; i ++)
    {
        Date futureDate = WorkDate(startDate + deltaDuration * i);
        double deltaT = normDiffDate(lastDate, futureDate, 
                Date::ACT365);
        double deltaTForR = normDiffDate(curveStartDate, futureDate,
                Date::ACT365);
        
        // FIX: Not sure about it
        double futureDf = instYC.getDf(futureDate) ;
        double rate =  - log(futureDf) / deltaTForR;

        double futurePrice = currPrice * (1.0 + rate * deltaT + 
                instRNG.get() * volatility * sqrt(deltaT));

        predictions[i] = std::pair<Date, double>(
                futureDate, futurePrice);

        currPrice = futurePrice;
        lastDate = futureDate;
    }

    return predictions;
}

#endif
