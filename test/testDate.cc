#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "Date.h"

class DurationTest : public testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            std::cout << "-------- Start Testing Duration Class --------" << std::endl;
        }

        static void TearDownTestCase()
        {
            std::cout << "-------- Finish Testing Duration Class --------"
                << std::endl << std::endl;
        }
};

TEST_F(DurationTest, DurationConstruction)
{
    std::string str;
    std::string& rstr = str;

    // All True CASES
    str="O/N";
    Duration d1(str);
    EXPECT_EQ(Duration::DAY, d1.type());
    EXPECT_EQ(1.0, d1.getDuration(Duration::DAY));

    str="ON";
    Duration d2(str);
    EXPECT_EQ(Duration::DAY, d2.type());
    EXPECT_EQ(1.0, d2.getDuration(Duration::DAY));

    str="T/N";
    Duration d3(str);
    EXPECT_EQ(Duration::DAY, d3.type());
    EXPECT_EQ(2.0, d3.getDuration(Duration::DAY));

    str="TN";
    Duration d4(str);
    EXPECT_EQ(Duration::DAY, d4.type());
    EXPECT_EQ(2.0, d4.getDuration(Duration::DAY));

    str="o/N";
    Duration d5(str);
    EXPECT_EQ(Duration::DAY, d5.type());
    EXPECT_EQ(1.0, d5.getDuration(Duration::DAY));

    str="On";
    Duration d6(str);
    EXPECT_EQ(Duration::DAY, d6.type());
    EXPECT_EQ(1.0, d6.getDuration(Duration::DAY));

    str="t/n";
    Duration d7(str);
    EXPECT_EQ(Duration::DAY, d7.type());
    EXPECT_EQ(2.0, d7.getDuration(Duration::DAY));

    str="tn";
    Duration d8(str);
    EXPECT_EQ(Duration::DAY, d8.type());
    EXPECT_EQ(2.0, d8.getDuration(Duration::DAY));

    str="1D";
    Duration d9(str);
    EXPECT_EQ(Duration::DAY, d9.type());
    EXPECT_EQ(1.0, d9.getDuration(Duration::DAY));

    str="1W";
    Duration d10(str);
    EXPECT_EQ(Duration::WEEK, d10.type());
    EXPECT_EQ(1.0, d10.getDuration(Duration::WEEK));

    str="1M";
    Duration d11(str);
    EXPECT_EQ(Duration::MONTH, d11.type());
    EXPECT_EQ(1.0, d11.getDuration(Duration::MONTH));

    str="1Q";
    Duration d12(str);
    EXPECT_EQ(Duration::QUARTER, d12.type());
    EXPECT_EQ(1.0, d12.getDuration(Duration::QUARTER));

    str="1Y";
    Duration d13(str);
    EXPECT_EQ(Duration::YEAR, d13.type());
    EXPECT_EQ(1.0, d13.getDuration(Duration::YEAR));

    str="10D";
    Duration d14(str);
    EXPECT_EQ(Duration::DAY, d14.type());
    EXPECT_EQ(10.0, d14.getDuration(Duration::DAY));

    str="12W";
    Duration d15(str);
    EXPECT_EQ(Duration::WEEK, d15.type());
    EXPECT_EQ(12.0, d15.getDuration(Duration::WEEK));

    str="23M";
    Duration d16(str);
    EXPECT_EQ(Duration::MONTH, d16.type());
    EXPECT_EQ(23.0, d16.getDuration(Duration::MONTH));

    str="331Q";
    Duration d17(str);
    EXPECT_EQ(Duration::QUARTER, d17.type());
    EXPECT_EQ(331.0, d17.getDuration(Duration::QUARTER));

    str="112345678Y";
    Duration d18(str);
    EXPECT_EQ(Duration::YEAR, d18.type());
    EXPECT_EQ(112345678.0, d18.getDuration(Duration::YEAR));

    str="1.1D";
    Duration d19(str);
    EXPECT_EQ(Duration::DAY, d19.type());
    EXPECT_EQ(1.1, d19.getDuration(Duration::DAY));

    str="1.2W";
    Duration d119(str);
    EXPECT_EQ(Duration::WEEK, d119.type());
    EXPECT_EQ(1.2, d119.getDuration(Duration::WEEK));

    str="2.23M";
    Duration d20(str);
    EXPECT_EQ(Duration::MONTH, d20.type());
    EXPECT_EQ(2.23, d20.getDuration(Duration::MONTH));

    str="3.31Q";
    Duration d21(str);
    EXPECT_EQ(Duration::QUARTER, d21.type());
    EXPECT_EQ(3.31, d21.getDuration(Duration::QUARTER));

    str="112.345678Y";
    Duration d22(str);
    EXPECT_EQ(Duration::YEAR, d22.type());
    EXPECT_EQ(112.345678, d22.getDuration(Duration::YEAR));

    // All False CASES
    str="O";
    EXPECT_ANY_THROW(Duration d23(str));

    str="ONT";
    EXPECT_ANY_THROW(Duration d24(str));

    str="ONTN";
    EXPECT_ANY_THROW(Duration d25(str));

    str="TN ";
    EXPECT_ANY_THROW(Duration d26(str));

    str="o/DN";
    EXPECT_ANY_THROW(Duration d27(str));

    str="0N";
    EXPECT_ANY_THROW(Duration d28(str));

    str="S/n";
    EXPECT_ANY_THROW(Duration d29(str));

    str=" tn";
    EXPECT_ANY_THROW(Duration d30(str));

    str="1E";
    EXPECT_ANY_THROW(Duration d31(str));

    str="1WW";
    EXPECT_ANY_THROW(Duration d32(str));

    str="1MM";
    EXPECT_ANY_THROW(Duration d33(str));

    str="1QP";
    EXPECT_ANY_THROW(Duration d34(str));

    str="1";
    EXPECT_ANY_THROW(Duration d35(str));

    str="10D]";
    EXPECT_ANY_THROW(Duration d36(str));

    str="12W.";
    EXPECT_ANY_THROW(Duration d37(str));

    str=",23M";
    EXPECT_ANY_THROW(Duration d38(str));

    str="3|31Q";
    EXPECT_ANY_THROW(Duration d39(str));

    str="11=2345678Y";
    EXPECT_ANY_THROW(Duration d40(str));

    str=".1.1D";
    EXPECT_ANY_THROW(Duration d41(str));

    str=".12W";
    EXPECT_ANY_THROW(Duration d42(str));

    str="2.2";
    EXPECT_ANY_THROW(Duration d43(str));

    str=".33Q";
    EXPECT_ANY_THROW(Duration d44(str));

    str="1.12345678R";
    EXPECT_ANY_THROW(Duration d45(str));
}

TEST_F(DurationTest, isValidDuration)
{
    std::string str;
    std::string& rstr = str;
    // All True CASES
    str="O/N";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="ON";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="T/N";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="TN";
    EXPECT_TRUE(Duration::isValidDuration(rstr));

    str="o/N";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="On";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="t/n";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="tn";
    EXPECT_TRUE(Duration::isValidDuration(rstr));

    str="1D";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="1W";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="1M";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="1Q";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="1Y";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="112.345678Y";
    EXPECT_TRUE(Duration::isValidDuration(rstr));

    // All False CASES
    str="O";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="ONT";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="ONTN";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="TN ";
    EXPECT_FALSE(Duration::isValidDuration(rstr));

    str="o/DN";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="0N";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="S/n";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str=" tn";
    EXPECT_FALSE(Duration::isValidDuration(rstr));

    str="1E";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="1WW";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="1MM";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="1QP";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="1";
    EXPECT_FALSE(Duration::isValidDuration(rstr));

    str="10D]";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="12W.";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str=",23M";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="3|31Q";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="11=2345678Y";
    EXPECT_FALSE(Duration::isValidDuration(rstr));

    str=".1.1D";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str=".12W";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="2.2";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str=".33Q";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
    str="1.12345678R";
    EXPECT_FALSE(Duration::isValidDuration(rstr));
}


class CustomedTestEnvironment : public testing::Environment
{
    public:
        virtual void SetUp()
        {
            std::cout << "@@@@@@@@ Begin testing Date.cc @@@@@@@@" << std::endl;
        }

        virtual void TearDown()
        {
            std::cout << "@@@@@@@@ Finish testing Date.cc @@@@@@@@"
                << std::endl << std::endl << std::endl;
        }
};

int main(int argc, char * argv[])
{
    testing::AddGlobalTestEnvironment(new CustomedTestEnvironment);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
