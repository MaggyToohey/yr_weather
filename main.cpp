/**
 * @file main.cpp
 * @author Mags Toohey
 * @brief main() function for the yr weather class
 * @date 14/11/2020
 */

#include <iostream>
#include "YR_forecast.h"


int main() {
    
    float lat, lon, alt = 0;
    std::cout << "Please enter location in latitude, longitude and altitude:" << std::endl;
    std::cout << "Note coordinates for Detroit, MI are: 42.3314 N, 83.0458 W ";
    std::cout << "and coordinates for Galway, Ireland are 53.2707 N, 9.0568 W" << std::endl;
    std::cin >> lat;
    std::cin >> lon;
    std::cin >>alt;
    // Create a handler for lat
    yr::HandleGeoCoords<decltype(lat)> checkLatCoords;
    // Create a handler for lon
    yr::HandleGeoCoords<decltype(lon)> checkLonCoords;
    // Create a handler for alt
    yr::HandleGeoCoords<decltype(alt)> checkAltCoords;
    // Validate the coords before passing to Yr weather class
    auto lat_1 = checkLatCoords.validateLatitude(lat);
    auto lon_1 = checkLonCoords.validateLongitude(lon);
    int alt_1 = checkAltCoords.validateAltitude(alt);
    yr::YrForecast weather_app (lat_1, lon_1, alt_1);
    weather_app.runProgram();
    return 0;
}

