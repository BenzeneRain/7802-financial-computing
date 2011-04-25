#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/tuple/tuple.hpp>

#include "Instrument.h"
#include "YieldCurve.h"

void 
printUsage()
{
    std::cout << "Usage: " << std::endl;
    std::cout << "\t./generateYieldCurve <input curve definition csv filename> " <<
        "<input curve data csv filename> <input query csv file> <output csv filename>" << std::endl;
}

int 
main(int argc, char * argv[])
{
    if(argc != 5)
    {
        printUsage();
        exit(0);
    }

    std::string inCVDefFilename(argv[1]); 
    std::string inCVDataFilename(argv[2]);
    std::string inQueryFilename(argv[3]);
    std::string outFilename(argv[4]);

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
        std::vector<InstrumentDefinition *> generatedInstrDefs = ycDef.getAllDefinitions();
        std::vector<InstrumentDefinition *>& gdefs = generatedInstrDefs;

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

        std::cout << "Dumping the curve data to the output file " << outFilename << " ..." << std::endl;
        std::vector<boost::tuple<Date, double, double> > queryResults;
        Date startDate = WorkDate(yci->startDate());

        for(int i = 0; i < (int)gdefs.size(); i ++)
        {
            Date maturityDate = WorkDate(startDate + gdefs[i]->maturity());
            double df = yci->getDf(maturityDate);
            double rate = (*yci)[maturityDate];

            queryResults.push_back(boost::tuple<Date, double, double>(maturityDate, df, rate));

        }

        std::cout << "Querying the zero coupon rate ..." << std::endl;
        std::ifstream finQuery(inQueryFilename.c_str());
        getline(finQuery, line);
        while(finQuery.good())
        {
            std::string dateStr;
            finQuery >> dateStr;
            if(!finQuery.good())
                break;

            try
            {
                Date dateUnModified(dateStr);
                Date date = WorkDate(dateUnModified);
                double df = yci->getDf(date);
                double rate = (*yci)[date];
                queryResults.push_back(boost::tuple<Date, double, double>(date, df, rate));
            }
            catch(DateException& e)
            {
            }
            catch(YieldCurveException& e)
            {
            }

        }
        finQuery.close();

        

        std::cout << "Writing the query result to the output file " << outFilename << " ..." << std::endl; 
        std::ofstream fout(outFilename.c_str());
        fout << "\"Date\",\"Discount Factor\",\"Zero Coupon Rate\"" << std::endl;
        for(int i = 0; i < (int)queryResults.size(); i ++)
        {
            Date& date = (Date&)queryResults[i].get<0>();
            fout << "\"" << date.toString() << "\",\"";
            
            fout.setf(std::ios::fixed);
            fout << std::setprecision(4) << queryResults[i].get<1>() << "\",\"" 
                << queryResults[i].get<2>() << "\"" << std::endl;
            fout.unsetf(std::ios::fixed);
        }
        fout.close();

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
