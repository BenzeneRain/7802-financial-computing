#include "gtest/gtest.h"

//class CustomedTestEnvironment : public testing::Environment
//{
//    public:
//        virtual void SetUp()
//        {
//            std::cout << "\t\t\t\t\t@@@@@@@@@@@@@@@\\/ Begin testing Instrument.cc \\/@@@@@@@@@@@@@@@" 
//                << std::endl;
//        }
//
//        virtual void TearDown()
//        {
//            std::cout << "\t\t\t\t\t@@@@@@@@@@@@@@@/\\ Finish testing Instrument.cc /\\@@@@@@@@@@@@@@@"
//                << std::endl << std::endl << std::endl;
//        }
//};

GTEST_API_ int main(int argc, char * argv[])
{
    //testing::AddGlobalTestEnvironment(new CustomedTestEnvironment);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
