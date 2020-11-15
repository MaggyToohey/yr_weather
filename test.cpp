#include "gtest/gtest.h"
#include "YR_forecast.h"

// Test the URL
TEST(TestURL, TestWithIntValues){
    int test_lat = 50;
    int test_lon =50; 
    int test_alt = 50;
    yr::YrForecast test_forecast(test_lat, test_lon, test_alt);
    test_forecast.createURL();
    std::string control_url = 
    "https://api.met.no/weatherapi/locationforecast/2.0/compact.json?altitude=50&lat=50.000000&lon=50.000000";
    std::string test_url = test_forecast.getURL();
    EXPECT_STRCASEEQ(test_url.c_str(), control_url.c_str());

}

// Test validateLongitude
TEST(TestHandleGeoCoords_Longitude, TestLonWithInt){
    int test_lon = 4;
    yr::HandleGeoCoords<int> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lon);
    // Should be int
    EXPECT_EQ(returned_lon, 4);
}

TEST(TestHandleGeoCoords_Longitude, TestLonWithFloat){
    float test_lon = 4.999;
    yr::HandleGeoCoords<float> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lon);
    // Should be float
    EXPECT_FLOAT_EQ(returned_lon, 4.999);
}

TEST(TestHandleGeoCoords_Longitude, TestLonWithDouble){
    double test_lon = 4.5555555;
    yr::HandleGeoCoords<double> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lon);
    // Should be float
    EXPECT_NEAR(returned_lon, 4.5555, 0.05);
}

TEST(TestHandleGeoCoords_Longitude, TestLonWithString){
    std::string test_lon = "four";
    yr::HandleGeoCoords<std::string> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lon);
    // Should be default valude of 0.0
    EXPECT_EQ(returned_lon, 0.0);
}
// Test validateLatitude
TEST(TestHandleGeoCoords_Lattitude, TestLatWithInt){
    int test_lat = 4;
    yr::HandleGeoCoords<int> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lat);
    // Should be int
    EXPECT_EQ(returned_lon, 4);
}

TEST(TestHandleGeoCoords_Lattitude, TestLatWithFloat){
    float test_lat = 4.999;
    yr::HandleGeoCoords<float> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lat);
    // Should be float
    EXPECT_FLOAT_EQ(returned_lon, 4.999);
}

TEST(TestHandleGeoCoords_Longitude, TestLatWithDouble){
    double test_lat = 4.5555555;
    yr::HandleGeoCoords<double> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lat);
    // Should be float
    EXPECT_NEAR(returned_lon, 4.5555, 0.05);
}

TEST(TestHandleGeoCoords_Longitude, TestLatWithString){
    std::string test_lat = "four";
    yr::HandleGeoCoords<std::string> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lat);
    // Should be default valude of 0.0
    EXPECT_EQ(returned_lon, 0.0);
}
// Test validateAltitude
TEST(TestHandleGeoCoords_Altitude, TestAltWithInt){
    int test_alt = 4;
    yr::HandleGeoCoords<int> checkCoords;
    auto returned_alt = checkCoords.validateAltitude(test_alt);
    // Expect equal
    EXPECT_EQ(returned_alt, 4);
}
TEST(TestHandleGeoCoords_Altitude, TestAltWithfloat){
    float test_alt = 4.5555;
    yr::HandleGeoCoords<float> checkCoords;
    auto returned_alt = checkCoords.validateAltitude(test_alt);
    // Expect int
    EXPECT_EQ(returned_alt, 4);
}
TEST(TestHandleGeoCoords_Altitude, TestAltWithDouble){
    double test_alt = 4.555555555;
    yr::HandleGeoCoords<double> checkCoords;
    auto returned_alt = checkCoords.validateAltitude(test_alt);
    // Expect equal
    EXPECT_EQ(returned_alt, 4);
}

// Death tests
TEST(MyDeathTest, TestLonOutOfBounds_int){
    int test_lon = 400;
    yr::HandleGeoCoords<int> checkCoords;
    std::string matcher = "";
    //EXPECT_EXIT(checkCoords.validateLongitude(test_lon), testing::KilledBySignal(EXIT_FAILURE), matcher);
    EXPECT_EXIT(checkCoords.validateLongitude(test_lon), testing::ExitedWithCode(EXIT_FAILURE), matcher);

}
TEST(MyDeathTest, TestLonOutOfBounds_float){
    float test_lon = 400.788;
    yr::HandleGeoCoords<float> checkCoords;
    std::string matcher = "";
    EXPECT_EXIT(checkCoords.validateLongitude(test_lon), testing::ExitedWithCode(EXIT_FAILURE), matcher);

}
TEST(MyDeathTest, TestLonOutOfBounds_double){
    double test_lon = 400.899988;
    yr::HandleGeoCoords<double> checkCoords;
    std::string matcher = "";
    EXPECT_EXIT(checkCoords.validateLongitude(test_lon), testing::ExitedWithCode(EXIT_FAILURE), matcher);

}

TEST(MyDeathTest, TestLatOutOfBounds_int){
    int test_lat = 400;
    yr::HandleGeoCoords<int> checkCoords;
    std::string matcher = "";
    EXPECT_EXIT(checkCoords.validateLongitude(test_lat), testing::ExitedWithCode(EXIT_FAILURE), matcher);

}
TEST(MyDeathTest, TestLatOutOfBounds_float){
    float test_lat = 400.788;
    yr::HandleGeoCoords<float> checkCoords;
    std::string matcher = "";
    EXPECT_EXIT(checkCoords.validateLongitude(test_lat), testing::ExitedWithCode(EXIT_FAILURE), matcher);

}
TEST(MyDeathTest, TestLatOutOfBounds_double){
    double test_lat = 400.899988;
    yr::HandleGeoCoords<double> checkCoords;
    std::string matcher = "";
    EXPECT_EXIT(checkCoords.validateLongitude(test_lat), testing::ExitedWithCode(EXIT_FAILURE), matcher);

}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();;
}
