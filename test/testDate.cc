#include <string>
#include <iostream>
#include <time.h>

#include "gtest/gtest.h"
#include "Date.h"


class DateTest : public testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            std::cout << "\t\t\t\t\t-------- Start Testing Date Class --------" << std::endl;
        }

        static void TearDownTestCase()
        {
            std::cout << "\t\t\t\t\t-------- Finish Testing Date Class --------"
                << std::endl << std::endl;
        }
};

class DurationTest : public testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            std::cout << "\t\t\t\t\t-------- Start Testing Duration Class --------" << std::endl;
        }

        static void TearDownTestCase()
        {
            std::cout << "\t\t\t\t\t-------- Finish Testing Duration Class --------"
                << std::endl << std::endl;
        }
};

class MiscTest : public testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            std::cout << "\t\t\t\t\t-------- Start Testing miscellaneous date non-member functions --------" << std::endl;
        }

        static void TearDownTestCase()
        {
            std::cout << "\t\t\t\t\t-------- Finish Testing miscellaneous date non-member functions --------"
                << std::endl << std::endl;
        }
};

TEST_F(DateTest, DateConstruction)
{
    Date todayDateTest = Date::today();
    boost::gregorian::date todayTest = todayDateTest.get();

    time_t rawTime;
    time(&rawTime);

    struct tm * timeinfo;
    struct tm todayStandard;

    timeinfo = localtime(&rawTime);
    memcpy(&todayStandard, timeinfo, sizeof(struct tm));

    EXPECT_EQ(todayStandard.tm_year + 1900, todayTest.year());
    EXPECT_EQ(todayStandard.tm_mon + 1, todayTest.month());
    EXPECT_EQ(todayStandard.tm_mday, todayTest.day());

    Date anotherDate(todayDateTest);
    todayTest = anotherDate.get();

    EXPECT_EQ(todayStandard.tm_year + 1900, todayTest.year());
    EXPECT_EQ(todayStandard.tm_mon + 1, todayTest.month());
    EXPECT_EQ(todayStandard.tm_mday, todayTest.day());
}

TEST_F(DateTest, DateArithmetics)
{
    boost::gregorian::date testDate1(2003, boost::gregorian::Jan, 31);
    boost::gregorian::date testDate2(2004, boost::gregorian::Jan, 31);
    boost::gregorian::date testDate3(2003, boost::gregorian::Feb, 1);
    boost::gregorian::date testDate4(2003, boost::gregorian::Jan, 30);
    boost::gregorian::date testDate5(2002, boost::gregorian::Dec, 31);

    Date testDate11(testDate1);
    Date testDate21(testDate2);
    Date testDate31(testDate3);
    Date testDate41(testDate4);
    Date testDate51(testDate5);

    boost::gregorian::date expectDate11(2003, boost::gregorian::Feb, 28);
    boost::gregorian::date expectDate12(2003, boost::gregorian::Mar, 28);
    boost::gregorian::date expectDate13(2003, boost::gregorian::Mar, 31);

    boost::gregorian::date expectDate21(2004, boost::gregorian::Feb, 29);
    boost::gregorian::date expectDate22(2004, boost::gregorian::Mar, 29);
    boost::gregorian::date expectDate23(2004, boost::gregorian::Mar, 31);

    Date expectDate111(expectDate11);
    Date expectDate121(expectDate12);
    Date expectDate131(expectDate13);

    Date expectDate211(expectDate21);
    Date expectDate221(expectDate22);
    Date expectDate231(expectDate23);

    Duration oneMonth(1, Duration::MONTH);
    Duration twoMonth(2, Duration::MONTH);

    // Test Operator +
    EXPECT_EQ(expectDate111, testDate11 + oneMonth);
    EXPECT_EQ(expectDate121, testDate11 + oneMonth + oneMonth) <<
        "Expect " << expectDate121.get() << ", Actual " << (testDate11 + oneMonth + oneMonth).get();
    EXPECT_EQ(expectDate131, testDate11 + twoMonth);

    EXPECT_EQ(expectDate211, testDate21 + oneMonth);
    EXPECT_EQ(expectDate221, testDate21 + oneMonth + oneMonth) <<
        "Expect " << expectDate121.get() << ", Actual " << (testDate21 + oneMonth + oneMonth).get();
    EXPECT_EQ(expectDate231, testDate21 + twoMonth);

    // Test Operator <
    EXPECT_LT(testDate11, testDate21);
    EXPECT_LT(testDate11, testDate31);
    EXPECT_LT(testDate41, testDate11);
    EXPECT_LT(testDate51, testDate11);

    // Test Operator -
    EXPECT_EQ(Duration(365.0, Duration::DAY), (testDate11 - testDate21));
    EXPECT_EQ(Duration(364.0, Duration::DAY), testDate21 - testDate31) <<
        "Expect " << 364 << " Day, Actual " << 
        (testDate21 - testDate31).getDuration(Duration::DAY) << " Day";
    EXPECT_EQ(Duration(2.0, Duration::DAY), testDate41 - testDate31) <<
        "Expect " << 2 << " Day, Actual " << 
        (testDate41 - testDate31).getDuration(Duration::DAY) << " Day";
    EXPECT_EQ(Duration(30.0, Duration::DAY), testDate51 - testDate41);
}

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

TEST_F(MiscTest, NormDiffDate)
{
    boost::gregorian::date testDate1(2003, boost::gregorian::Jan, 31);
    boost::gregorian::date testDate2(2004, boost::gregorian::Jan, 31);
    boost::gregorian::date testDate3(2003, boost::gregorian::Feb, 1);
    boost::gregorian::date testDate4(2003, boost::gregorian::Jan, 30);
    boost::gregorian::date testDate5(2002, boost::gregorian::Dec, 31);

    Date testDate11(testDate1);
    Date testDate21(testDate2);
    Date testDate31(testDate3);
    Date testDate41(testDate4);
    Date testDate51(testDate5);

    EXPECT_DOUBLE_EQ(1.0, normDiffDate(testDate11, testDate21, Date::ACT365));
    EXPECT_DOUBLE_EQ(0.00547945205479452054, normDiffDate(testDate31, testDate41, Date::ACT365));
    EXPECT_DOUBLE_EQ(0.08219178082191780821, normDiffDate(testDate41, testDate51, Date::ACT365));
}

TEST_F(DurationTest, DurationArithmetics)
{
    Duration oneYear(1, Duration::YEAR);
    Duration oneMonth(1, Duration::MONTH);
    Duration oneQuarter(1, Duration::QUARTER);

    EXPECT_DOUBLE_EQ(4.0, (oneYear / 3.0).getDuration(Duration::MONTH));
    EXPECT_DOUBLE_EQ(12.0, (oneYear / oneMonth));
    EXPECT_DOUBLE_EQ(4.0, (oneYear / oneQuarter));

    EXPECT_DOUBLE_EQ(2.5, (oneQuarter * 10.0).getDuration(Duration::YEAR));
}

class CustomedTestEnvironment : public testing::Environment
{
    public:
        virtual void SetUp()
        {
            std::cout << "\t\t\t\t\t@@@@@@@@@@@@@@\\/ Begin testing Date.cc \\/@@@@@@@@@@@@@@@" << std::endl;
        }

        virtual void TearDown()
        {
            std::cout << "\t\t\t\t\t@@@@@@@@@@@@@@@/\\ Finish testing Date.cc /\\@@@@@@@@@@@@@@@"
                << std::endl << std::endl << std::endl;
        }
};

int main(int argc, char * argv[])
{
    testing::AddGlobalTestEnvironment(new CustomedTestEnvironment);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
