#include <fstream>
#include <string>

#include "gtest/gtest.h"
#include "Instrument.h"

TEST(c7802Test, InstrumentInitializationTest1)
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

TEST(c7802Test, InstrumentInitializationTest2)
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

int main(int argc, char * argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
