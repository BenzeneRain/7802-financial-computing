#include <string>

#include "gtest/gtest.h"
#include "Date.h"


TEST(DurationTest, isValidDuration)
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

    str="10D";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="12W";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="23M";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="331Q";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="112345678Y";
    EXPECT_TRUE(Duration::isValidDuration(rstr));

    str="1.1D";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="1.2W";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="2.23M";
    EXPECT_TRUE(Duration::isValidDuration(rstr));
    str="3.31Q";
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

int main(int argc, char * argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
