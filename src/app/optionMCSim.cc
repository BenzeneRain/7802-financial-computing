#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#include "Instrument.h"
#include "YieldCurve.h"
#include "Utility.h"
#include "Stock.h"

using namespace Stock::PricePredictionModel;
using namespace RandomNumberGenerator;

void 
printUsage()
{
    std::cout << "Usage: " << std::endl;
    std::cout << "\t./optionMCSim <input curve definition csv filename> " <<
        "<input curve data csv filename> <input option description csv file>" << std::endl;
}

double payOutFunc1(std::vector<std::pair<Date, double> >& prices)
{
    double finalPrice = prices[prices.size() - 1].second;

    if((finalPrice >= 75  && finalPrice <= 125))
        return abs(finalPrice - 100);
    else
        return 0;
}

double payOutFunc2(std::vector<std::pair<Date, double> >& prices)
{
    double smin = prices[0].second;
    double smax = prices[0].second;

    for(int i = 1; i < (int)prices.size(); i ++)
    {
        if(prices[i].second < smin)
            smin = prices[i].second;

        if(prices[i].second > smax)
            smax = prices[i].second;
    }

    double diff = smax - smin;
    
    if(diff >= 50)
        return 0.5 * diff;
    else if(diff >= 20)
        return diff;
    else
        return 0;
}

int 
main(int argc, char * argv[])
{
    if(argc != 4)
    {
        printUsage();
        exit(0);
    }

    srand((unsigned int)time(NULL));

    std::string inCVDefFilename(argv[1]); 
    std::string inCVDataFilename(argv[2]);
    std::string inOptionDescFilename(argv[3]);

    try
    {
        std::cout << "Parsing Yield Curve Definitions ..." << std::endl;
        std::ifstream finCVDef(inCVDefFilename.c_str());
        std::string line;
        std::vector<InstrumentDefinition *> instrDefs;

        getline(finCVDef, line);
        while(finCVDef.good())
        {
            getline(finCVDef, line);
            if(!finCVDef.good())
                break;

            InstrumentDefinition *instrDef = InstrumentDefinition::parseString(line);
            instrDefs.push_back(instrDef);
        }
        finCVDef.close();

        YieldCurveDefinition ycDef(instrDefs, 4.0);

        std::cout << "Binding Yield Curve Data to the definition ..." << std::endl;
        InstrumentValues values;
        std::ifstream finCVData(inCVDataFilename.c_str());
        getline(finCVData, line);
        while(finCVData.good())
        {
            char comma;
            int id;
            double rate;

            finCVData >> id >> comma >> rate;
            values.values.push_back(std::pair<int, double>(id, rate));
        }
        finCVData.close();
        YieldCurveInstance *yci = ycDef.bindData(&values, YieldCurveDefinition::ZEROCOUPONRATE);

        std::cout << "Reading and processing options ..." << std::endl;
        Date today = WorkDate(Date::today());
        std::ifstream finOptionDesc(inOptionDescFilename.c_str());
        getline(finOptionDesc, line);
        while(finOptionDesc.good())
        {
            double strike, expireTradePrice, currTradePrice;
            uint64_t rounds, steps;
            int optIndex;
            std::string expireDateStr;
            char comma;

            finOptionDesc >> optIndex >> comma >> steps >> comma >> rounds >>
                comma >> currTradePrice >> comma >> strike >> comma >>
                expireTradePrice >> comma >> expireDateStr;

            if(!finOptionDesc.good())
                break;


            Date expireDateUnModified(expireDateStr);
            Date expireDate = WorkDate(expireDateUnModified);

            // Calculate Volatility
            double deltaT = normDiffDate(today, expireDate, Date::ACT365); 
            double dfAtExpire = yci->getDf(expireDate);
            double crfRate = - log(dfAtExpire) / deltaT; // continuous time risk free rate

            Volatility::FormulaClass *formula = new Volatility::VolatilityFromEuroCallPriceFormula(
                    currTradePrice, strike, deltaT, crfRate, expireTradePrice); 
            double volatility = Volatility::NewtonRaphsonMethod()(*formula, 1e-9);

            delete formula;

            Duration duration = expireDate - today;
            // Forecast the price using Monte-Carlo Method
            // Antithetice method
            double sumPayout1 = 0;
            double sumPayout2 = 0;
            for(uint64_t i = 0; i < rounds; i ++)
            {
                std::vector<std::pair<Date, double> > futurePrices;

                futurePrices = MonteCarloSimulation<boxMullerM2RNG>(currTradePrice, today,
                        duration, steps, *yci, volatility,
                        boxMullerM2RNG(boxMullerM2RNG::ANTITHETIC));


                double payout1 = payOutFunc1(futurePrices);
                double payout2 = payOutFunc2(futurePrices);
                sumPayout1 += payout1;
                sumPayout2 += payout2;
            }

            std::cout << "Using Antithetic Random Number Generator" << std::endl;
            std::cout << "Option "  << optIndex << std::endl;
            std::cout << "Rounds: " << rounds << std::endl;
            std::cout << "Steps: " << steps << std::endl;
            std::cout << "Current Trading Price: " << currTradePrice << std::endl;
            std::cout << "Strike: " << strike << std::endl;
            std::cout << "Expire Date: " << expireDate.toString() << std::endl;
            std::cout << "Expire Trading price: " << expireTradePrice << std::endl;
            std::cout << "Volatility: " << volatility << std::endl;
//            std::cout << "Price Predictions:" << std::endl; 
//            for(int i = 0; i < (int) futurePrices.size(); i ++)
//            {
//                std::cout << "\t" << futurePrices[i].first.toString() << "\t"
//                    << futurePrices[i].second << std::endl;
//            }

            std::cout.setf(std::ios::fixed);
            std::cout << "Average pay out according to Method 1: " << 
                std::setprecision(4) << sumPayout1 / (double)rounds << std::endl; 
            std::cout << "Average pay out according to Method 2: " << 
                std::setprecision(4) << sumPayout2 / (double)rounds << std::endl; 
            std::cout << std::endl;
            std::cout.unsetf(std::ios::fixed);

            // Forecast the price using Monte-Carlo Method
            // Antithetice method
            sumPayout1 = 0;
            sumPayout2 = 0;
            for(uint64_t i = 0; i < rounds; i ++)
            {
                std::vector<std::pair<Date, double> > futurePrices;

                futurePrices = MonteCarloSimulation<boxMullerM2RNG>(currTradePrice, today,
                        duration, steps, *yci, volatility,
                        boxMullerM2RNG(boxMullerM2RNG::NONANTITHETIC));


                double payout1 = payOutFunc1(futurePrices);
                double payout2 = payOutFunc2(futurePrices);
                sumPayout1 += payout1;
                sumPayout2 += payout2;
            }

            std::cout << "Using Non-Antithetic Random Number Generator" << std::endl;
            std::cout << "Option "  << optIndex << std::endl;
            std::cout << "Rounds: " << rounds << std::endl;
            std::cout << "Steps: " << steps << std::endl;
            std::cout << "Current Trading Price: " << currTradePrice << std::endl;
            std::cout << "Strike: " << strike << std::endl;
            std::cout << "Expire Date: " << expireDate.toString() << std::endl;
            std::cout << "Expire Trading price: " << expireTradePrice << std::endl;
            std::cout << "Volatility: " << volatility << std::endl;
//            std::cout << "Price Predictions:" << std::endl; 
//            for(int i = 0; i < (int) futurePrices.size(); i ++)
//            {
//                std::cout << "\t" << futurePrices[i].first.toString() << "\t"
//                    << futurePrices[i].second << std::endl;
//            }

            std::cout.setf(std::ios::fixed);
            std::cout << "Average pay out according to Method 1: " << 
                std::setprecision(4) << sumPayout1 / (double)rounds << std::endl; 
            std::cout << "Average pay out according to Method 2: " << 
                std::setprecision(4) << sumPayout2 / (double)rounds << std::endl; 
            std::cout << std::endl;
            std::cout.unsetf(std::ios::fixed);
        }
        finOptionDesc.close();

        delete yci;

        while(!instrDefs.empty())
        {
            InstrumentDefinition * ptrInstrDef = instrDefs.back();
            instrDefs.pop_back();
            delete ptrInstrDef;
        }

        std::cout << "Program finished successfully." << std::endl;
    }
    catch(std::fstream::failure& e)
    {
    }
    catch(InstrumentException& e)
    {
    }
    catch(YieldCurveException& e)
    {
    }

    return 0;
}
