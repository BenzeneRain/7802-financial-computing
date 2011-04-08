#include <fstream>
#include <string>

#include "gtest/gtest.h"
#include "Instrument.h"

TEST(c7802Test, InstrumentInitializationTest)
{
    std::ifstream fin("curveSpec.csv");
    std::ifstream cmpfin("curveSpecCmp.txt");
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

int main(int argc, char * argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
