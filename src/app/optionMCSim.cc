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
//        std::vector<InstrumentDefinition *> generatedInstrDefs = ycDef.getAllDefinitions();
//        std::vector<InstrumentDefinition *>& gdefs = generatedInstrDefs;

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
        Date today = Date::today();
        std::ifstream finOptionDesc(inOptionDescFilename.c_str());
        getline(finOptionDesc, line);
        int optionIndex = 1;
        while(finOptionDesc.good())
        {
            double strike, expireTradePrice, currTradePrice;
            std::string expireDateStr;
            char comma;

            finOptionDesc >> currTradePrice >> comma >> strike >> comma >>
                expireTradePrice >> comma >> expireDateStr;

            if(!finOptionDesc.good())
                break;


            Date expireDate(expireDateStr);

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
            std::vector<std::pair<Date, double> > futurePrices;

            futurePrices = MonteCarloSimulation<boxMullerM2RNG>(currTradePrice, today,
                    duration, 10, *yci, volatility,
                    boxMullerM2RNG(boxMullerM2RNG::ANTITHETIC));


            double payout1 = payOutFunc1(futurePrices);
            double payout2 = payOutFunc2(futurePrices);

            std::cout << "Option "  << optionIndex << std::endl;
            std::cout << "Current Trading Price: " << currTradePrice << std::endl;
            std::cout << "Strike: " << strike << std::endl;
            std::cout << "Expire Date: " << expireDate.toString() << std::endl;
            std::cout << "Expire Trading price: " << expireTradePrice << std::endl;
            std::cout << "Volatility: " << volatility << std::endl;
            std::cout << "Price Predictions:" << std::endl; 
            for(int i = 0; i < (int) futurePrices.size(); i ++)
            {
                std::cout << "\t" << futurePrices[i].first.toString() << "\t"
                    << futurePrices[i].second << std::endl;
            }

            std::cout << "Pay out according to Method 1: " << payout1 << std::endl; 
            std::cout << "Pay out according to Method 2: " << payout2 << std::endl; 
            std::cout << std::endl;
        }
        finOptionDesc.close();

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
