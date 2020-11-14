/**
 * @file YR_forecast.h
 * @author Mags Toohey
 * @brief Class to retrieve weather data based on location
 * @date 14/11/2020
 */

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "YR_forecast.h"

using json = nlohmann::json;

// Callback for libcurl to write the data
static int dataWriterCallback(char *data, size_t size, size_t nmemb,
                              std::string *writerData)
{
    if (writerData == NULL)
    {
        return 0;
    }
    writerData->append(data, size * nmemb);

    return size * nmemb;
}

// Storage for data returned from yr.no
static std::string _forecast_data;
json _forecast_data_json;

// Buffer to store curl error strings
static char errorBuffer[CURL_ERROR_SIZE];

void YrForecast::getUserCoords()
{
    float latitude, longitude = 0.0;
    int altitude = 0;
    std::string lat_str, lon_str, alt_str = "";

    // Lat and long are required for URL
    while (true)
    {
        std::cout << "Please enter lat: ";
        std::getline(std::cin, lat_str);
        // Convert string to float
        std::stringstream myStream(lat_str);
        if (myStream >> latitude)
            break;
        std::cout << "Latitude required as float, please try again" << std::endl;
    }
    // Check value is valid
    if ((-85.0 <= latitude && latitude <= 85.0))
    {
        _latitude = latitude;
    }
    else
    {
        throw "Latitude out of bounds!";
    }

    while (true)
    {
        std::cout << "Please enter Longitude: ";
        std::getline(std::cin, lon_str);
        // Convert string to float
        std::stringstream myStream(lon_str);
        if (myStream >> longitude)
            break;
        std::cout << "'\n' Longitude required as float, please try again" << std::endl;
    }
    // Check value is valid
    if ((-180.0 <= longitude && longitude <= 180.0))
    {
        _longitude = longitude;
    }
    else
    {
        throw "Longitude out of bounds!";
    }
    std::cout << "Altitude: ";
    std::getline(std::cin, alt_str);
    if (alt_str.compare("\n") != 0)
    {
        std::stringstream myStream(alt_str);
        myStream >> altitude;
        _altitude = static_cast<int>(altitude);
    }
    else
    {
        // default is sea level;
        _altitude = 0;
    }
    return;
}

void YrForecast::createURL()
{
    /** Yr.no API requires URL of the form:
     * https://api.met.no/weatherapi/locationforecast/2.0/compact.
     * json?altitude=X&lat=Y&lon=Z
    */
    // Set coords as string
    std::string alt = "altitude=" + std::to_string(_altitude);
    std::string lat = "&lat=" + std::to_string(_latitude);
    std::string lon = "&lon=" + std::to_string(_longitude);
    // Set URL based on the user input coords
    _coords_url = _base_url + alt + lat + lon;
    _URL_complete = true;
    std::cout << _coords_url << std::endl;
    return;
}

bool YrForecast::populateForecastData()
{

    // Setup libcurl global constant environment
    curl_global_init(CURL_GLOBAL_ALL);

    // Only need to made request if URL exists
    if (_URL_complete)
    {

        // Curl return code
        CURLcode code;

        // Create a curl object, using easy interface since
        // only single transfer is needed for this app
        CURL *easyhandle = curl_easy_init();

        // Check handle has been created
        if (easyhandle == NULL)
        {
            std::cout << "Failed to create CURL connection." << std::endl;
            exit(EXIT_FAILURE);
        }

        // Set the error buffer
        code = curl_easy_setopt(easyhandle, CURLOPT_ERRORBUFFER, errorBuffer);
        if (code != CURLE_OK)
        {
            std::cout << "Failed to set error buffer " << code << std::endl;
            return false;
        }

        // Set the URL
        code = curl_easy_setopt(easyhandle, CURLOPT_URL, _coords_url.c_str());
        if (code != CURLE_OK)
        {
            std::cout << "Failed to set the URL. " << errorBuffer << std::endl;
            return false;
        }
        // Set User Agent
        code = curl_easy_setopt(easyhandle, CURLOPT_USERAGENT,
                                "MyWeatherApp/0.1 github.com/MaggyToohey/yr_weather");
        if (code != CURLE_OK)
        {
            std::cout << "Failed to set User Agent. " << errorBuffer << std::endl;
            return false;
        }
        // Set the write callback in the curl handler
        code = curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, dataWriterCallback);
        if (code != CURLE_OK)
        {
            std::cout << "Failed to set write function: " << errorBuffer;
            return false;
        }
        // Set the data storage in the handler
        code = curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &_forecast_data);
        if (code != CURLE_OK)
        {
            std::cout << "Failed to set the write data: " << errorBuffer;
            return false;
        }
        // Send request to yr.no
        code = curl_easy_perform(easyhandle);
        // Clean up curl easy handle
        curl_easy_cleanup(easyhandle);

        if (code != CURLE_OK)
        {
            std::cout << "Curl request not successful, error: " << errorBuffer;
            exit(EXIT_FAILURE);
        }
        return true;
    }
    // Clean global environment in the case easy clean up is not executed
    curl_global_cleanup();
    return false;
}

void YrForecast::parseForecastJSON()
{
    _forecast_data_json = json::parse(_forecast_data);
    json instant_details =
        _forecast_data_json["properties"]["timeseries"][0]["data"]["instant"]["details"];
    json summary =
        _forecast_data_json["properties"]["timeseries"][0]["data"]["next_6_hours"]["summary"]["symbol_code"];

    _current_weather.forecast_summary = summary.dump();

    _current_weather.air_pressure_at_sea_level = instant_details["air_pressure_at_sea_level"];
    _current_weather.temperature = instant_details["air_temperature"];
    _current_weather.cloud_area_fraction = instant_details["cloud_area_fraction"];
    _current_weather.relative_humidity = instant_details["relative_humidity"];
    _current_weather.wind_direction = instant_details["wind_from_direction"];
    _current_weather.wind_speed = instant_details["wind_speed"];
    _current_weather.precipitation_amount =
        _forecast_data_json["properties"]["timeseries"][0]["data"]["next_6_hours"]["details"]["precipitation_amount"];

    return;
}

void YrForecast::printForecast()
{

    std::cout << "The weather summary for lat " << _latitude << " and long " << _longitude << " is: " << _current_weather.forecast_summary.c_str() << std::endl;
    std::cout << "Temperature: " << _current_weather.temperature << std::endl;
    std::cout << "Fraction of cloud cover: " << _current_weather.cloud_area_fraction << std::endl;
    std::cout << "Relative humidity: " << _current_weather.relative_humidity << std::endl;
    std::cout << "Wind Direction: " << _current_weather.wind_direction << std::endl;
    std::cout << "Wind Speed: " << _current_weather.wind_speed << std::endl;
    std::cout << "Precipitation over the next 6 hours: " << _current_weather.precipitation_amount << std::endl;
}

void YrForecast::runProgram()
{
    try
    {
        getUserCoords();
    }
    catch (const char *coord_exception)
    {
        std::cerr << coord_exception << std::endl;
        return;
    }
    createURL();
    populateForecastData();
    parseForecastJSON();
    printForecast();

    return;
}
