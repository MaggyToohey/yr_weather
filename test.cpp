#include "gtest/gtest.h"
#include "YR_forecast.h"

namespace {

TEST(MyTest, ThisTestShouldPass) {
    YrForecast test_forecast;
    float latitude = 0.5;
    float longitude = 14.762;
    float altitide = 6;
    test_forecast.runProgram();
    EXPECT_EQ(latitude, latitude);
}
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}