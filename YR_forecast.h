/**
 * @file YR_forecast.h
 * @author Mags Toohey
 * @brief Class to retrieve weatehr data based on location
 * @date 14/11/2020
 */

#ifndef YR_FORECAST_H
#define YR_FORECAST_H

#include <string>
#include <sstream>
#include <iomanip>

/**
 * @brief Structure containing information about the forecast
 */
struct YrForecastStruct{
    float air_pressure_at_sea_level;
    float temperature;
    float cloud_area_fraction;
    float relative_humidity;
    float wind_direction;
    float wind_speed;
    float precipitation_amount;
    std::string forecast_summary;
};
/**
 * @brief Class to request weather data from yr.no based on user input location
 */
class YrForecast {

public:
    /**
     * @brief Construct a new YrForecast object
     */
	YrForecast() = default;
    /**
     * @brief Destructor
     */
	~YrForecast() = default;

    /**
     * @brief Starts the program calls
     * @param none
     * @returns none
     */
    void runProgram();

private:

    /**
     * @brief Sets the latitude, longitude and altitude members, 
     * from user input
     * @param none
     * @returns none
     */
    void setLatLongAlt();
    /**
     * @brief Create a URL based on location
     * @param none
     * @returns none
     */
    bool createURL();
    /**
     * @brief Send request to yr.no and store result
     * @param none
     * @returns none
     */
    bool populateForecastData();
    /**
     * @brief Parse JSON data returned from yr.no into struct
     * @param none
     * @returns none
     */
    void parseWeatherJSON();
    /**
     * @brief Print the forecast to console
     * @param none
     * @returns none
     */
    void printForecast();

    // URL data
    const std::string _base_url = "https://api.met.no/weatherapi/locationforecast/2.0/compact.json?";
    std::string _coords_url;

    // Structs to hold parsed data
	YrForecastStruct _current_weather;

    // Location data
    // Lat and Long can be 4 dec places, defaults to Null Island
    float _latitude = 0.0;
    float _longitude = 0.0;
    // Altitude must be int, defaults to sea level;
    int _altitude = 0;  
};

#endif //YR_FORECAST_H
