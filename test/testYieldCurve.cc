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
            std::cout << "-------- Start Testing YieldCurveDefinition Class --------"
                << std::endl;
        }

        static void TearDownTestCase()
        {
            std::cout << "-------- Finish Testing YieldCurveDefinition Class --------"
                << std::endl << std::endl;
        }
};

class YieldCurveInstanceTest : public testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            std::cout << "-------- Start Testing YieldCurveInstanseTest Class --------"
                << std::endl;
        }

        static void TearDownTestCase()
        {
            std::cout << "-------- Finish Testing YieldCurveInstanceTest Class --------"
                << std::endl << std::endl;
        }
};

void testEqualYieldCurveDefinition(InstrumentDefinition::TYPE stype, Duration smaturity,
        int sid, InstrumentDefinition& tInstrDef)
{
    EXPECT_EQ(stype, tInstrDef.type()) << "Expect " 
        << stype << ", Actual " << tInstrDef.type();

    EXPECT_EQ(smaturity, tInstrDef.maturity()) << "Expect "
        << smaturity.toString(false, false) << ", Actual" <<
        tInstrDef.maturity().toString(false, false);

    EXPECT_EQ(sid, tInstrDef.index()) << "Expect "
        << sid << ", Actual " << tInstrDef.index();
}

TEST_F(YieldCurveDefinitionTest, YieldCurveDefinitionConstruction)
{
    std::ifstream fin("testInstrumentData/curveSpec1.csv");
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


class CustomedTestEnvironment : public testing::Environment
{
    public:
        virtual void SetUp()
        {
            std::cout << "\t\t\t\t\t@@@@@@@@@@@@@@@\\/ Begin testing "__FILE__" \\/@@@@@@@@@@@@@@@" 
                << std::endl;
        }

        virtual void TearDown()
        {
            std::cout << "\t\t\t\t\t@@@@@@@@@@@@@@@/\\ Finish testing "__FILE__" /\\@@@@@@@@@@@@@@@"
                << std::endl << std::endl << std::endl;
        }
};

int main(int argc, char * argv[])
{
    testing::AddGlobalTestEnvironment(new CustomedTestEnvironment);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
