#include <string>
#include <iostream>
#include <ctime>
#include <vector>

#include "gtest/gtest.h"
#include "Utility.h"

using namespace RandomNumberGenerator;
using namespace std;

class RNGTest : public testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            std::cout << "\t\t\t\t\t-------- Start Testing Random Number Generator --------" << std::endl;
        }

        static void TearDownTestCase()
        {
            std::cout << "\t\t\t\t\t-------- Finish Testing Random Number Generator --------"
                << std::endl << std::endl;
        }
};

// Use Chi-square testing to check if the numbers generated by Box Muller
// Random Generator using Method 2 (ANTITHETIC mode) fits the standard normal
// distribution The range is divided into 4 parts, (-inf, -1.0], (-1.0, 0], (0,
// 1.0], (1.0, inf] Significate value is chose as 1%
TEST_F(RNGTest, Test_BoxMullerM2RNG_ANTITHETIC_Fitting_StardardNormalDistribution)
{
    const uint64_t quantity = 10000000;
    vector<double> observedCounts(4);
    vector<double> expectedCounts(4);
    const int rounds = 1;
    double expectChiSquareValue = 11.34487; // Degree of Freedom = 3; Significance = 1%

    expectedCounts[0] = 0.15866 * quantity;
    expectedCounts[1] = 0.34134 * quantity;
    expectedCounts[2] = 0.34134 * quantity;
    expectedCounts[3] = 0.15866 * quantity;

    srand((unsigned int)time(NULL));

    boxMullerM2RNG rng(boxMullerM2RNG::ANTITHETIC);

    for(int i = 0; i < rounds; i ++)
    {
        for(int j = 0; j < 4; j ++)
            observedCounts[j] = 0;

        for(uint64_t j = 0; j < quantity; j ++)
        {
            double randNum = rng.get();

            // FIX: It is better not compare 
            // double variables using this method
            if(randNum <= -1.0)
                observedCounts[0] ++;
            else if(randNum <= 0)
                observedCounts[1] ++;
            else if(randNum <= 1)
                observedCounts[2] ++;
            else
                observedCounts[3] ++;
        }


        double xSquare = 0;
        for(int j = 0; j < 4; j ++)
        {
            double diff = expectedCounts[j] - observedCounts[j];
            xSquare += diff * diff / expectedCounts[j];
        }

        EXPECT_GT(expectChiSquareValue, xSquare) << "at Round "
            << i;
    }

}

// Use Chi-square testing to check if the numbers generated by Box Muller
// Random Generator using Method 2 (NONANTITHETIC mode) fits the standard normal
// distribution The range is divided into 4 parts, (-inf, -1.0], (-1.0, 0], (0,
// 1.0], (1.0, inf] Significate value is chose as 1%
TEST_F(RNGTest, Test_BoxMullerM2RNG_NONANTITHETIC_Fitting_StardardNormalDistribution)
{
    const uint64_t quantity = 10000000;
    vector<double> observedCounts(4);
    vector<double> expectedCounts(4);
    const int rounds = 1;
    double expectChiSquareValue = 11.34487; // Degree of Freedom = 3; Significance = 1%

    expectedCounts[0] = 0.15866 * quantity;
    expectedCounts[1] = 0.34134 * quantity;
    expectedCounts[2] = 0.34134 * quantity;
    expectedCounts[3] = 0.15866 * quantity;

    srand((unsigned int)time(NULL));

    boxMullerM2RNG rng(boxMullerM2RNG::NONANTITHETIC);

    for(int i = 0; i < rounds; i ++)
    {
        for(int j = 0; j < 4; j ++)
            observedCounts[j] = 0;

        for(uint64_t j = 0; j < quantity; j ++)
        {
            double randNum = rng.get();

            // FIX: It is better not compare 
            // double variables using this method
            if(randNum <= -1.0)
                observedCounts[0] ++;
            else if(randNum <= 0)
                observedCounts[1] ++;
            else if(randNum <= 1)
                observedCounts[2] ++;
            else
                observedCounts[3] ++;
        }


        double xSquare = 0;
        for(int j = 0; j < 4; j ++)
        {
            double diff = expectedCounts[j] - observedCounts[j];
            xSquare += diff * diff / expectedCounts[j];
        }

        EXPECT_GT(expectChiSquareValue, xSquare) << "at Round "
            << i;
    }

}

