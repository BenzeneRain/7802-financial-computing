#include <fstream>
#include <string>

#include "gtest/gtest.h"
#include "Instrument.h"

class InstrumentTest : public testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            std::cout << "-------- Start Testing Instrument Class --------"
                << std::endl;
        }

        static void TearDownTestCase()
        {
            std::cout << "-------- Finish Testing Instrument Class --------"
                << std::endl << std::endl;
        }
};

TEST_F(InstrumentTest, InstrumentInitializationTest1)
{
    std::ifstream fin("testInstrumentData/curveSpec1.csv");
    std::ifstream cmpfin("testInstrumentData/curveSpecCmp.txt");
    std::string line;

    std::string cmaturity, ctype;
    int cindex;

    getline(fin, line);
    
    while(fin.good())
    {
        cmpfin >> ctype >> cmaturity >> cindex;
        getline(fin, line);
        if(!fin.good())
            break;

        InstrumentDefinition *instrDef = InstrumentDefinition::parseString(line);

        std::string type;
        if(instrDef->type() == InstrumentDefinition::CASH)
            type = "CASH";
        else if(instrDef->type() == InstrumentDefinition::FRA)
            type = "FRA";
        else if(instrDef->type() == InstrumentDefinition::SWAP)
            type = "SWAP";
        else 
            type = "N/A";

        EXPECT_EQ(ctype, type);
        EXPECT_EQ(cmaturity, instrDef->subtype());
        EXPECT_EQ(cindex, instrDef->index());

        delete instrDef;
    }

    fin.close();
    cmpfin.close();
}

TEST_F(InstrumentTest, InstrumentInitializationTest2)
{
    std::ifstream fin("testInstrumentData/curveSpec2.csv");
    std::string line;

    getline(fin, line);
    
    while(fin.good())
    {
        getline(fin, line);
        if(!fin.good())
            break;

        EXPECT_ANY_THROW(InstrumentDefinition::parseString(line)) <<
            "Input string is: " << line;
    }

    fin.close();
}

class CustomedTestEnvironment : public testing::Environment
{
    public:
        virtual void SetUp()
        {
            std::cout << "@@@@@@@@ Begin testing Instrument.cc @@@@@@@@" 
                << std::endl;
        }

        virtual void TearDown()
        {
            std::cout << "@@@@@@@@ Finish testing Instrument.cc @@@@@@@@"
                << std::endl << std::endl << std::endl;
        }
};

int main(int argc, char * argv[])
{
    testing::AddGlobalTestEnvironment(new CustomedTestEnvironment);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
