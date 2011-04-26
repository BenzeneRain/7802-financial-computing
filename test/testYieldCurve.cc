#include <iostream>
#include <fstream>
#include <string>

#include "gtest/gtest.h"
#include "Instrument.h"
#include "YieldCurve.h"

class YieldCurveDefinitionTest : public testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            std::cout << "\t\t\t\t\t-------- Start Testing YieldCurveDefinition Class --------"
                << std::endl;
        }

        static void TearDownTestCase()
        {
            std::cout << "\t\t\t\t\t-------- Finish Testing YieldCurveDefinition Class --------"
                << std::endl << std::endl;
        }
};

class YieldCurveInstanceTest : public testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            std::cout << "\t\t\t\t\t-------- Start Testing YieldCurveInstanceTest Class --------"
                << std::endl;
        }

        static void TearDownTestCase()
        {
            std::cout << "\t\t\t\t\t-------- Finish Testing YieldCurveInstanceTest Class --------"
                << std::endl << std::endl;
        }
};

void testEqualYieldCurveDefinition(InstrumentDefinition::TYPE stype, Duration smaturity,
        int sid, InstrumentDefinition& tInstrDef)
{
    EXPECT_EQ(stype, tInstrDef.type())
        << "Standard: " <<
        sid << ", " << smaturity.toString() << 
        ", " << stype << std::endl << "Actual: " <<
        tInstrDef.index() << ", " << 
        tInstrDef.maturity().toString() << ", " <<
        tInstrDef.type();

    EXPECT_EQ(smaturity, tInstrDef.maturity()) 
        << "Standard: " <<
        sid << ", " << smaturity.toString() << 
        ", " << stype << std::endl << "Actual: " <<
        tInstrDef.index() << ", " << 
        tInstrDef.maturity().toString() << ", " <<
        tInstrDef.type();

    EXPECT_EQ(sid, tInstrDef.index())
        << "Standard: " <<
        sid << ", " << smaturity.toString() << 
        ", " << stype << std::endl << "Actual: " <<
        tInstrDef.index() << ", " << 
        tInstrDef.maturity().toString() << ", " <<
        tInstrDef.type();
}

TEST_F(YieldCurveDefinitionTest, YieldCurveDefinitionConstruction1)
{
    std::ifstream fin("testYieldCurveData/curveSpec1.csv");
    std::string line;
    std::vector<InstrumentDefinition *> instrDefs;

    getline(fin, line);
    
    while(fin.good())
    {
        getline(fin, line);
        if(!fin.good())
            break;

        InstrumentDefinition *instrDef = InstrumentDefinition::parseString(line);
        instrDefs.push_back(instrDef);
    }

    YieldCurveDefinition ycDef(instrDefs, 4.0);
    std::vector<InstrumentDefinition *> generatedInstrDefs = ycDef.getAllDefinitions();
    std::vector<InstrumentDefinition *>& gdefs = generatedInstrDefs;

    {
        testEqualYieldCurveDefinition(InstrumentDefinition::CASH,
                Duration(1, Duration::DAY), 1, *gdefs[0]);

        testEqualYieldCurveDefinition(InstrumentDefinition::CASH,
                Duration(2, Duration::DAY), 2, *gdefs[1]);

        testEqualYieldCurveDefinition(InstrumentDefinition::CASH,
                Duration(1, Duration::WEEK), 3, *gdefs[2]);

        testEqualYieldCurveDefinition(InstrumentDefinition::CASH,
                Duration(2, Duration::WEEK), 4, *gdefs[3]);

        testEqualYieldCurveDefinition(InstrumentDefinition::CASH,
                Duration(1, Duration::MONTH), 5, *gdefs[4]);

        testEqualYieldCurveDefinition(InstrumentDefinition::CASH,
                Duration(2, Duration::MONTH), 6, *gdefs[5]);

        testEqualYieldCurveDefinition(InstrumentDefinition::CASH,
                Duration(3, Duration::MONTH), 7, *gdefs[6]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FRA,
                Duration(4, Duration::MONTH), 8, *gdefs[7]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FRA,
                Duration(5, Duration::MONTH), 9, *gdefs[8]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FRA,
                Duration(6, Duration::MONTH), 10, *gdefs[9]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FRA,
                Duration(9, Duration::MONTH), 11, *gdefs[10]);

        testEqualYieldCurveDefinition(InstrumentDefinition::SWAP,
                Duration(1, Duration::YEAR), 12, *gdefs[11]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(1.25, Duration::YEAR), -1, *gdefs[12]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(1.5, Duration::YEAR), -1, *gdefs[13]);
        
        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(1.75, Duration::YEAR), -1, *gdefs[14]);

        testEqualYieldCurveDefinition(InstrumentDefinition::SWAP,
                Duration(2, Duration::YEAR), 13, *gdefs[15]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(2.25, Duration::YEAR), -1, *gdefs[16]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(2.5, Duration::YEAR), -1, *gdefs[17]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(2.75, Duration::YEAR), -1, *gdefs[18]);

        testEqualYieldCurveDefinition(InstrumentDefinition::SWAP,
                Duration(3, Duration::YEAR), 14, *gdefs[19]);
    }

    while(!gdefs.empty())
    {
        InstrumentDefinition *instrDef = gdefs.back();
        gdefs.pop_back();
        delete instrDef;
    }

    while(!instrDefs.empty())
    {
        InstrumentDefinition *instrDef = instrDefs.back();
        instrDefs.pop_back();
        delete instrDef;
    }

    fin.close();
}

TEST_F(YieldCurveDefinitionTest, YieldCurveDefinitionConstruction2)
{
    std::ifstream fin("testYieldCurveData/curveSpec2.csv");
    std::string line;
    std::vector<InstrumentDefinition *> instrDefs;

    getline(fin, line);
    
    while(fin.good())
    {
        getline(fin, line);
        if(!fin.good())
            break;

        InstrumentDefinition *instrDef = InstrumentDefinition::parseString(line);
        instrDefs.push_back(instrDef);
    }

    YieldCurveDefinition ycDef(instrDefs, 4.0);
    std::vector<InstrumentDefinition *> generatedInstrDefs = ycDef.getAllDefinitions();
    std::vector<InstrumentDefinition *>& gdefs = generatedInstrDefs;

    {
        testEqualYieldCurveDefinition(InstrumentDefinition::CASH,
                Duration(1, Duration::DAY), 2, *gdefs[0]);

        testEqualYieldCurveDefinition(InstrumentDefinition::CASH,
                Duration(3, Duration::MONTH), 3, *gdefs[1]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(6, Duration::MONTH), -1, *gdefs[2]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(9, Duration::MONTH), -1, *gdefs[3]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(1, Duration::YEAR), -1, *gdefs[4]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(1.25, Duration::YEAR), -1, *gdefs[5]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(1.5, Duration::YEAR), -1, *gdefs[6]);
        
        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(1.75, Duration::YEAR), -1, *gdefs[7]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(2, Duration::YEAR), -1, *gdefs[8]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(2.25, Duration::YEAR), -1, *gdefs[9]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(2.5, Duration::YEAR), -1, *gdefs[10]);

        testEqualYieldCurveDefinition(InstrumentDefinition::FAKE,
                Duration(2.75, Duration::YEAR), -1, *gdefs[11]);

        testEqualYieldCurveDefinition(InstrumentDefinition::SWAP,
                Duration(3, Duration::YEAR), 1, *gdefs[12]);
    }

    while(!gdefs.empty())
    {
        InstrumentDefinition *instrDef = gdefs.back();
        gdefs.pop_back();
        delete instrDef;
    }

    while(!instrDefs.empty())
    {
        InstrumentDefinition *instrDef = instrDefs.back();
        instrDefs.pop_back();
        delete instrDef;
    }

    fin.close();
}

TEST_F(YieldCurveDefinitionTest, YieldCurveDefinitionConstruction3)
{
    std::ifstream fin("testYieldCurveData/curveSpec3.csv");
    std::string line;
    std::vector<InstrumentDefinition *> instrDefs;

    getline(fin, line);
    
    while(fin.good())
    {
        getline(fin, line);
        if(!fin.good())
            break;

        InstrumentDefinition *instrDef = InstrumentDefinition::parseString(line);
        instrDefs.push_back(instrDef);
    }

    EXPECT_ANY_THROW(YieldCurveDefinition ycDef(instrDefs, 4.0));

    while(!instrDefs.empty())
    {
        InstrumentDefinition *instrDef = instrDefs.back();
        instrDefs.pop_back();
        delete instrDef;
    }

    fin.close();
}

TEST_F(YieldCurveInstanceTest, YieldCurveInstanceConstruction1)
{
    // Build Construction
    std::ifstream deffin("testYieldCurveData/curveSpec1.csv");
    std::string line;
    std::vector<InstrumentDefinition *> instrDefs;

    getline(deffin, line);
    
    while(deffin.good())
    {
        getline(deffin, line);
        if(!deffin.good())
            break;

        InstrumentDefinition *instrDef = InstrumentDefinition::parseString(line);
        instrDefs.push_back(instrDef);
    }

    YieldCurveDefinition ycDef(instrDefs, 4.0);
    deffin.close();

    // Read Data
    std::ifstream datafin("testYieldCurveData/curveDataInput1.csv");
    InstrumentValues values;
    int id;
    double rate;

    getline(datafin, line);

    while(datafin.good())
    {
        char comma;
        datafin >> id >> comma >> rate;
        values.values.push_back(std::pair<int, double>(id, rate));
    }

    // Bind data to Yield Curve Defition, and do the test
    YieldCurveInstance *yci = ycDef.bindData(&values, YieldCurveDefinition::ZEROCOUPONRATE);
    std::vector<InstrumentDefinition *> generatedInstrDefs = ycDef.getAllDefinitions();
    std::vector<InstrumentDefinition *>& gdefs = generatedInstrDefs;

    double expectedRate;
    double actualRate;
    for(int i = 0; i < (int)values.values.size(); i ++)
    {
        std::pair<int, double>& valuepair = values.values[i];
        InstrumentDefinition *ptrDef = ycDef.getDefinitionByID(valuepair.first);
        Date maturityDate = WorkDate(Date::today() + ptrDef->maturity());

        expectedRate = valuepair.second;
        try
        {
            actualRate = getCompoundRate(*yci, maturityDate, ptrDef->type(), 4.0);
        }
        catch(YieldCurveException& e)
        {
            std::cout << e.what() << std::endl;
        }

        EXPECT_NEAR(expectedRate, actualRate, 1e-5) << "Maturity: " << 
            ptrDef->maturity().toString() << "; Maturity Date: " <<
            maturityDate.toString() << "; Type: " << 
            InstrumentDefinition::typeToString(ptrDef->type()) << std::endl;
    }


    // Dispose
    delete yci;
    while(!instrDefs.empty())
    {
        InstrumentDefinition *instrDef = instrDefs.back();
        instrDefs.pop_back();
        delete instrDef;
    }

}
