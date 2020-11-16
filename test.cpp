#include "gtest/gtest.h"
#include "YR_forecast.h"
#include <fstream>

// Test the URL
TEST(TestURL, When_PassedIntValues_ExpectCorrectUrl){
    // Set up yr object
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
TEST(TestPopulateForecastData, When_NoURL_ExpectPrintedError){
    // Set up yr object
    int test_lat = 50;
    int test_lon =50; 
    int test_alt = 50;
    yr::YrForecast test_forecast(test_lat, test_lon, test_alt);
    std::string returned_from_test_forecast = "";
    std::string no_url = "";
    CURLcode test_code;
    CURL *test_easyhandle = NULL;
    returned_from_test_forecast = test_forecast.populateForecastData(no_url, test_easyhandle, test_code);
    // Expected
    std::string actual_response = "Error, require URL to make curl request.\n";

    EXPECT_STREQ(returned_from_test_forecast.c_str(), actual_response.c_str());

}
TEST(TestPopulateForecastData, When_CurlNotInit_Expect_PrintedError){
    int test_lat = 50;
    int test_lon =50; 
    int test_alt = 50;
    yr::YrForecast test_forecast(test_lat, test_lon, test_alt);
    std::string returned_from_test_forecast = "";
    std::string no_url = "";
    CURLcode test_code;
    CURL *test_easyhandle = NULL;
    test_forecast._curl_init = false;
    returned_from_test_forecast = test_forecast.populateForecastData(no_url,test_easyhandle, test_code);
    // Expected
    std::string actual_response = "Error, curl handle has not been initialised.\n";

    EXPECT_STREQ(returned_from_test_forecast.c_str(), actual_response.c_str());
}
// Test the weather struct
TEST(TestWeatherStruct, When_JSONParsed_Expect_WeatherStructCorrect)
{
    // Set up yr object
    int test_lat = 50;
    int test_lon =50; 
    int test_alt = 50;
    yr::YrForecast test_forecast(test_lat, test_lon, test_alt);
    yr::YrForecastStruct test_weather_struct;
    std::string data;
    std::ifstream myfile("/Users/magnoliaann2e/Documents/YR_App/yr_weather/test_weather_data.txt");
    std::getline(myfile, data);
    myfile.close();
    // Control data values taken from text file
    std::string summary = "\"cloudy\"";
    float pressure = 1041.5;
    float temperature = -6;
    float clouds = 95.3;
    float humidity = 63.4;
    float wind_dir = 98.3;
    float wind_speed = 4.5;
    float precipitation = 0;
    // Test data
    test_weather_struct = test_forecast.parseForecastJSON(data);

    EXPECT_STREQ(test_weather_struct.forecast_summary.c_str(), summary.c_str());
    EXPECT_EQ(test_weather_struct.air_pressure_at_sea_level, pressure);
    EXPECT_EQ(test_weather_struct.temperature, temperature);
    EXPECT_EQ(test_weather_struct.cloud_area_fraction, clouds);
    EXPECT_EQ(test_weather_struct.relative_humidity, humidity);
    EXPECT_EQ(test_weather_struct.wind_direction, wind_dir);
    EXPECT_EQ(test_weather_struct.wind_speed, wind_speed);
    EXPECT_EQ(test_weather_struct.precipitation_amount, precipitation);
}
// Test validateLongitude
TEST(TestHandleGeoCoords_Longitude, When_LongitudeInt_Expect_Int){
    // Set up handleCoords object
    int test_lon = 4;
    yr::HandleGeoCoords<int> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lon);
    // Should be int
    EXPECT_EQ(returned_lon, 4);
}

TEST(TestHandleGeoCoords_Longitude, When_LongitudeFloat_Expect_Float){
    // Set up handleCoords object
    float test_lon = 4.999;
    yr::HandleGeoCoords<float> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lon);
    // Should be float
    EXPECT_FLOAT_EQ(returned_lon, 4.999);
}

TEST(TestHandleGeoCoords_Longitude, When_LongitudeDouble_Expect_Float){
    // Set up handleCoords object
    double test_lon = 4.5555555;
    yr::HandleGeoCoords<double> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lon);
    // Should be float
    EXPECT_NEAR(returned_lon, 4.5555, 0.05);
}

TEST(TestHandleGeoCoords_Longitude, When_LongitudeString_Expect_Default){
    // Set up handleCoords object
    std::string test_lon = "four";
    yr::HandleGeoCoords<std::string> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lon);
    // Should be default valude of 0.0
    EXPECT_EQ(returned_lon, 0.0);
}
// Test validateLatitude
TEST(TestHandleGeoCoords_Lattitude, When_LatitudeInt_Expect_Int){
    // Set up handleCoords object
    int test_lat = 4;
    yr::HandleGeoCoords<int> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lat);
    // Should be int
    EXPECT_EQ(returned_lon, 4);
}

TEST(TestHandleGeoCoords_Lattitude, When_LatitudeFloat_Expect_Float){
    // Set up handleCoords object
    float test_lat = 4.999;
    yr::HandleGeoCoords<float> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lat);
    // Should be float
    EXPECT_FLOAT_EQ(returned_lon, 4.999);
}

TEST(TestHandleGeoCoords_Longitude, When_LatitudeDouble_Expect_Float){
    // Set up handleCoords object
    double test_lat = 4.5555555;
    yr::HandleGeoCoords<double> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lat);
    // Should be float
    EXPECT_NEAR(returned_lon, 4.5555, 0.05);
}

TEST(TestHandleGeoCoords_Longitude, When_LatitudeString_Expect_Default){
    // Set up handleCoords object
    std::string test_lat = "four";
    yr::HandleGeoCoords<std::string> checkCoords;
    auto returned_lon = checkCoords.validateLongitude(test_lat);
    // Should be default valude of 0.0
    EXPECT_EQ(returned_lon, 0.0);
}
// Test validateAltitude
TEST(TestHandleGeoCoords_Altitude, TestAltWithInt){
    // Set up handleCoords object
    int test_alt = 4;
    yr::HandleGeoCoords<int> checkCoords;
    auto returned_alt = checkCoords.validateAltitude(test_alt);
    // Expect equal
    EXPECT_EQ(returned_alt, 4);
}
TEST(TestHandleGeoCoords_Altitude, When_AltIsFloat_Expect_Int){
    // Set up handleCoords object
    float test_alt = 4.5555;
    yr::HandleGeoCoords<float> checkCoords;
    auto returned_alt = checkCoords.validateAltitude(test_alt);
    // Expect int
    EXPECT_EQ(returned_alt, 4);
}
TEST(TestHandleGeoCoords_Altitude, When_AltIsDouble_Expect_Int){
    // Set up handleCoords object
    double test_alt = 4.555555555;
    yr::HandleGeoCoords<double> checkCoords;
    auto returned_alt = checkCoords.validateAltitude(test_alt);
    // Expect equal
    EXPECT_EQ(returned_alt, 4);
}
TEST(TestHandleGeoCoords_Altitude, When_AltIsString_Expect_Default){
    std::string test_alt = "four";
    yr::HandleGeoCoords<std::string> checkCoords;
    auto returned_alt = checkCoords.validateAltitude(test_alt);
    // Expect equal
    EXPECT_EQ(returned_alt, 0);
}

// Death tests

TEST(MyDeathTest, When_Emptytring_Expect_NlohmanException){
    // Set up yr object
    int test_lat = 50;
    int test_lon =50; 
    int test_alt = 50;
    yr::YrForecast test_forecast(test_lat, test_lon, test_alt);
    // String not valid for nlomann parser
    std::string empty_string = "";
    EXPECT_ANY_THROW(test_forecast.parseForecastJSON(empty_string));
}

TEST(MyDeathTest, When_CurlHandleNullPointer_Expect_Exit){
    // Set up yr object
    int test_lat = 50;
    int test_lon =50; 
    int test_alt = 50;
    yr::YrForecast test_forecast(test_lat, test_lon, test_alt);
    // Create URL sets _URL_complete true
    test_forecast.createURL();
    std::string url = test_forecast.getURL();
    CURLcode test_code;
    CURL *test_easyhandle = NULL;
    std::string matcher = "";
    // Expected to exit
    EXPECT_EXIT(test_forecast.populateForecastData(url,test_easyhandle, test_code),
        testing::ExitedWithCode(EXIT_FAILURE), matcher);

}
TEST(MyDeathTest, When_TestLonOutOfBoundsInt_Expect_Exit){
    // Set up handleCoords object
    int test_lon = 400;
    yr::HandleGeoCoords<int> checkCoords;
    std::string matcher = "";
    EXPECT_EXIT(checkCoords.validateLongitude(test_lon), testing::ExitedWithCode(EXIT_FAILURE), matcher);

}
TEST(MyDeathTest, When_TestLonOutOfBoundsFloat_Exoect_Exit){
    // Set up handleCoords object
    float test_lon = 400.788;
    yr::HandleGeoCoords<float> checkCoords;
    std::string matcher = "";
    EXPECT_EXIT(checkCoords.validateLongitude(test_lon), testing::ExitedWithCode(EXIT_FAILURE), matcher);

}
TEST(MyDeathTest, When_TestLonOutOfBoundsDouble_Expect_Exit){
    // Set up handleCoords object
    double test_lon = 400.899988;
    yr::HandleGeoCoords<double> checkCoords;
    std::string matcher = "";
    EXPECT_EXIT(checkCoords.validateLongitude(test_lon), testing::ExitedWithCode(EXIT_FAILURE), matcher);

}

TEST(MyDeathTest, When_TestLatOutOfBoundsInt_Expect_Exit){
    // Set up handleCoords object
    int test_lat = 400;
    yr::HandleGeoCoords<int> checkCoords;
    std::string matcher = "";
    EXPECT_EXIT(checkCoords.validateLongitude(test_lat), testing::ExitedWithCode(EXIT_FAILURE), matcher);

}
TEST(MyDeathTest, When_TestLatOutOfBoundsFloat_Expect_Exit){
    // Set up handleCoords object
    float test_lat = 400.788;
    yr::HandleGeoCoords<float> checkCoords;
    std::string matcher = "";
    EXPECT_EXIT(checkCoords.validateLongitude(test_lat), testing::ExitedWithCode(EXIT_FAILURE), matcher);

}
TEST(MyDeathTest, When_TestLatOutOfBoundsDouble_Expect_Exit){
    // Set up handleCoords object
    double test_lat = 400.899988;
    yr::HandleGeoCoords<double> checkCoords;
    std::string matcher = "";
    EXPECT_EXIT(checkCoords.validateLongitude(test_lat), testing::ExitedWithCode(EXIT_FAILURE), matcher);

}
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();;
}
