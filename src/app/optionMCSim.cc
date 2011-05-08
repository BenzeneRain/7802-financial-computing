#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#include <sys/times.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

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

double payOutFuncBenchmark(std::vector<std::pair<Date, double> >& prices, double strike)
{
    double finalPrice = prices[prices.size() - 1].second;

    if(finalPrice > strike)
        return finalPrice - strike;
    else
        return 0;
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

    // Variables for measuring the cpu time cost
    struct rusage usage;
    unsigned long t1, t2;

    try
    {
        std::cout << "Parsing Yield Curve Definitions ...";
        getrusage(RUSAGE_SELF, &usage);
        t1 = (unsigned long long)usage.ru_utime.tv_sec * 1000000ULL +  // this is user time
            (unsigned long long)usage.ru_utime.tv_usec;
        t1 += (unsigned long long)usage.ru_stime.tv_sec * 1000000ULL + // this is system time
            (unsigned long long)usage.ru_stime.tv_usec;
        
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

        getrusage(RUSAGE_SELF, &usage);
        t2 = (unsigned long long)usage.ru_utime.tv_sec * 1000000ULL +  // this is user time
            (unsigned long long)usage.ru_utime.tv_usec;
        t2 += (unsigned long long)usage.ru_stime.tv_sec * 1000000ULL + // this is system time
            (unsigned long long)usage.ru_stime.tv_usec;
        std::cout << " Time used " << t2 - t1 << "us" << std::endl;

        std::cout << "Binding Yield Curve Data to the definition ...";
        getrusage(RUSAGE_SELF, &usage);
        t1 = (unsigned long long)usage.ru_utime.tv_sec * 1000000ULL +  // this is user time
            (unsigned long long)usage.ru_utime.tv_usec;
        t1 += (unsigned long long)usage.ru_stime.tv_sec * 1000000ULL + // this is system time
            (unsigned long long)usage.ru_stime.tv_usec;
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

        getrusage(RUSAGE_SELF, &usage);
        t2 = (unsigned long long)usage.ru_utime.tv_sec * 1000000ULL +  // this is user time
            (unsigned long long)usage.ru_utime.tv_usec;
        t2 += (unsigned long long)usage.ru_stime.tv_sec * 1000000ULL + // this is system time
            (unsigned long long)usage.ru_stime.tv_usec;
        std::cout << " Time used " << t2 - t1 << "us" << std::endl;

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


            // Calculate Volatility
            std::cout << "Calculating volatility ...";
            getrusage(RUSAGE_SELF, &usage);
            t1 = (unsigned long long)usage.ru_utime.tv_sec * 1000000ULL +  // this is user time
                (unsigned long long)usage.ru_utime.tv_usec;
            t1 += (unsigned long long)usage.ru_stime.tv_sec * 1000000ULL + // this is system time
                (unsigned long long)usage.ru_stime.tv_usec;

            Date expireDateUnModified(expireDateStr);
            Date expireDate = WorkDate(expireDateUnModified);

            double deltaT = normDiffDate(today, expireDate, Date::ACT365); 
            double dfAtExpire = yci->getDf(expireDate);
            double crfRate = - log(dfAtExpire) / deltaT; // continuous time risk free rate

            Volatility::FormulaClass *formula = new Volatility::VolatilityFromEuroCallPriceFormula(
                    currTradePrice, strike, deltaT, crfRate, expireTradePrice); 
            double volatility = Volatility::NewtonRaphsonMethod()(*formula, 1e-9);

            delete formula;
            getrusage(RUSAGE_SELF, &usage);
            t2 = (unsigned long long)usage.ru_utime.tv_sec * 1000000ULL +  // this is user time
                (unsigned long long)usage.ru_utime.tv_usec;
            t2 += (unsigned long long)usage.ru_stime.tv_sec * 1000000ULL + // this is system time
                (unsigned long long)usage.ru_stime.tv_usec;
            std::cout << " Time used " << t2 - t1 << "us" << std::endl;
            std::cout << "Volatility: " << std::setprecision(4) << 
                volatility << std::endl;

            
            // Forecast the price using Monte-Carlo Method
            // Antithetice method
            std::cout << "Pricing the option using Monte-Carlo Simulation ...";
            getrusage(RUSAGE_SELF, &usage);
            t1 = (unsigned long long)usage.ru_utime.tv_sec * 1000000ULL +  // this is user time
                (unsigned long long)usage.ru_utime.tv_usec;
            t1 += (unsigned long long)usage.ru_stime.tv_sec * 1000000ULL + // this is system time
                (unsigned long long)usage.ru_stime.tv_usec;
            Duration duration = expireDate - today;
            double sumPayout1 = 0;
            double sumPayout2 = 0;
            double sumPayout3 = 0;

            // Use this to copy one group of price prediction as output
            std::vector<std::pair<Date, double> > tFuturePrices;
            for(uint64_t i = 0; i < rounds; i ++)
            {
                std::vector<std::pair<Date, double> > futurePrices;

                futurePrices = MonteCarloSimulation<boxMullerM2RNG>(currTradePrice, today,
                        duration, steps, *yci, volatility,
                        boxMullerM2RNG(boxMullerM2RNG::ANTITHETIC));


                double payout1 = payOutFunc1(futurePrices);
                double payout2 = payOutFunc2(futurePrices);
                double payout3 = payOutFuncBenchmark(futurePrices, strike);
                sumPayout1 += payout1;
                sumPayout2 += payout2;
                sumPayout3 += payout3;

                if(i == (rounds - 1))
                    tFuturePrices = futurePrices;
            }
            getrusage(RUSAGE_SELF, &usage);
            t2 = (unsigned long long)usage.ru_utime.tv_sec * 1000000ULL +  // this is user time
                (unsigned long long)usage.ru_utime.tv_usec;
            t2 += (unsigned long long)usage.ru_stime.tv_sec * 1000000ULL + // this is system time
                (unsigned long long)usage.ru_stime.tv_usec;
            std::cout << " Time used " << t2 - t1 << "us" << std::endl;

            std::cout << "Using Antithetic Random Number Generator" << std::endl;
            std::cout << "Option "  << optIndex << std::endl;
            std::cout << "Rounds: " << rounds << std::endl;
            std::cout << "Steps: " << steps << std::endl;
            std::cout << "Current Trading Price: " << currTradePrice << std::endl;
            std::cout << "Strike: " << strike << std::endl;
            std::cout << "Expire Date: " << expireDate.toString() << std::endl;
            std::cout << "Expire Trading price: " << expireTradePrice << std::endl;
            std::cout << "One Group of Price Predictions:" << std::endl; 
            std::cout.setf(std::ios::fixed);
            for(int i = 0; i < (int) tFuturePrices.size(); i ++)
            {
                std::cout << "\t" << tFuturePrices[i].first.toString() << "\t"
                    << std::setprecision(4) << tFuturePrices[i].second << std::endl;
            }

            std::cout << "Discount factor at expiration: " << 
                std::setprecision(4) << dfAtExpire << std::endl;
            std::cout << "Average pay out of Benchmark Option: " << 
                std::setprecision(4) << sumPayout3 / (double)rounds << std::endl; 
            std::cout << "Average pay out according to Method 1 (Option A): " << 
                std::setprecision(4) << sumPayout1 / (double)rounds << std::endl; 
            std::cout << "Average pay out according to Method 2 (Option B): " << 
                std::setprecision(4) << sumPayout2 / (double)rounds << std::endl; 
            std::cout << std::endl;
            std::cout.unsetf(std::ios::fixed);

            // Forecast the price using Monte-Carlo Method
            // NonAntithetice method
            std::cout << "Pricing the option using Monte-Carlo Simulation ...";
            getrusage(RUSAGE_SELF, &usage);
            t1 = (unsigned long long)usage.ru_utime.tv_sec * 1000000ULL +  // this is user time
                (unsigned long long)usage.ru_utime.tv_usec;
            t1 += (unsigned long long)usage.ru_stime.tv_sec * 1000000ULL + // this is system time
                (unsigned long long)usage.ru_stime.tv_usec;
            sumPayout1 = 0;
            sumPayout2 = 0;
            sumPayout3 = 0;
            for(uint64_t i = 0; i < rounds; i ++)
            {
                std::vector<std::pair<Date, double> > futurePrices;

                futurePrices = MonteCarloSimulation<boxMullerM2RNG>(currTradePrice, today,
                        duration, steps, *yci, volatility,
                        boxMullerM2RNG(boxMullerM2RNG::NONANTITHETIC));


                double payout1 = payOutFunc1(futurePrices);
                double payout2 = payOutFunc2(futurePrices);
                double payout3 = payOutFuncBenchmark(futurePrices, strike);
                sumPayout1 += payout1;
                sumPayout2 += payout2;
                sumPayout3 += payout3;

                if(i == (rounds - 1))
                    tFuturePrices = futurePrices;
            }
            getrusage(RUSAGE_SELF, &usage);
            t2 = (unsigned long long)usage.ru_utime.tv_sec * 1000000ULL +  // this is user time
                (unsigned long long)usage.ru_utime.tv_usec;
            t2 += (unsigned long long)usage.ru_stime.tv_sec * 1000000ULL + // this is system time
                (unsigned long long)usage.ru_stime.tv_usec;
            std::cout << " Time used " << t2 - t1 << "us" << std::endl;

            std::cout << "Using Non-Antithetic Random Number Generator" << std::endl;
            std::cout << "Option "  << optIndex << std::endl;
            std::cout << "Rounds: " << rounds << std::endl;
            std::cout << "Steps: " << steps << std::endl;
            std::cout << "Current Trading Price: " << currTradePrice << std::endl;
            std::cout << "Strike: " << strike << std::endl;
            std::cout << "Expire Date: " << expireDate.toString() << std::endl;
            std::cout << "Expire Trading price: " << expireTradePrice << std::endl;
            std::cout << "One Group of Price Predictions:" << std::endl; 
            std::cout.setf(std::ios::fixed);
            for(int i = 0; i < (int) tFuturePrices.size(); i ++)
            {
                std::cout << "\t" << tFuturePrices[i].first.toString() << "\t"
                    << std::setprecision(4) << tFuturePrices[i].second << std::endl;
            }

            std::cout << "Discount factor at expiration: " << 
                std::setprecision(4) << dfAtExpire << std::endl;
            std::cout << "Average pay out of Benchmark Option: " << 
                std::setprecision(4) << sumPayout3 / (double)rounds << std::endl; 
            std::cout << "Average pay out according to Method 1 (Option A): " << 
                std::setprecision(4) << sumPayout1 / (double)rounds << std::endl; 
            std::cout << "Average pay out according to Method 2 (Option B): " << 
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
