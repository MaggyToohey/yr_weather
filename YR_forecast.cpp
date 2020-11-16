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


#include "YR_forecast.h"

using json = nlohmann::json;

namespace yr
{

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

    YrForecast::YrForecast(const float &latitude, 
                    const float &longitude, 
                    const int &altitude)
                    : _latitude{latitude} 
                    , _longitude{longitude}
                    , _altitude{altitude} 
                    {
                        // Init curl handle
                        curlInit();
                    }

    void YrForecast::createURL()
    {
    /** Yr.no API requires URL of the form:
     * https://api.met.no/weatherapi/locationforecast/2.0/compact.json?altitude=X&lat=Y&lon=Z
     */
        // Set coords as string
        std::string alt = "altitude=" + std::to_string(_altitude);
        std::string lat = "&lat=" + std::to_string(_latitude);
        std::string lon = "&lon=" + std::to_string(_longitude);
        // Set URL based on the user input coords
        _coords_url = _base_url + alt + lat + lon;
        _URL_complete = true;
        return;
    }
    /**
     * @brief Initialise curl params
     */
    void YrForecast::curlInit()
    {
        // Setup libcurl global constant environment
        curl_global_init(CURL_GLOBAL_ALL);

        // Create a curl object, using easy interface since
        // only single transfer is needed for this app
        _easyhandle = curl_easy_init();
        _curl_init = true;
    }
    /**
     * @brief Send request to yr.no and store result
     * @param URL String containing the URL to send request to
     * @return Forecast_data String containing forecast data in JSON format
     */
    std::string YrForecast::populateForecastData(std::string url, CURL *easyhandle, CURLcode code)
    {
        if (_curl_init)
        {
            static std::string forecast;
            // Only need to made request if URL exists
            if (_URL_complete)
            {
                // Check handle has been created
                if (easyhandle == NULL)
                {
                    std::cout << "Failed to create CURL connection." << std::endl;
                    exit(EXIT_FAILURE);
                }
                // Set buffer for curl errors
                code = curl_easy_setopt(easyhandle, CURLOPT_ERRORBUFFER, errorBuffer);
                if (code != CURLE_OK)
                {
                    std::cout << "Failed to set error buffer " << code << std::endl;
                    exit(EXIT_FAILURE);
                }
                // Set the URL
                code = curl_easy_setopt(easyhandle, CURLOPT_URL, url.c_str());
                if (code != CURLE_OK)
                {
                    std::cout << "Failed to set the URL. " << errorBuffer << std::endl;
                    exit(EXIT_FAILURE);
                }
                // Set User Agent
                code = curl_easy_setopt(easyhandle, CURLOPT_USERAGENT,
                                        &userAgent);
                if (code != CURLE_OK)
                {
                    std::cout << "Failed to set User Agent. " << errorBuffer << std::endl;
                    exit(EXIT_FAILURE);
                }
                // Set the write callback in the curl handler
                code = curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, dataWriterCallback);
                if (code != CURLE_OK)
                {
                    std::cout << "Failed to set write function: " << errorBuffer;
                    exit(EXIT_FAILURE);
                }
                // Set the forecast data storage in the handler
                code = curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &forecast);
                if (code != CURLE_OK)
                {
                    std::cout << "Failed to set the write data: " << errorBuffer;
                    exit(EXIT_FAILURE);
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
                return forecast;
            }
            // Clean global environment in the case easy clean up is not executed
            curl_global_cleanup();
            return "Error, require URL to make curl request.\n";
        }
        return "Error, curl handle has not been initialised.\n";
    }

    yr::YrForecastStruct YrForecast::parseForecastJSON(std::string forecast)
    {
        json forecast_json;
        yr::YrForecastStruct weather;
        // Try block for exceptions from nlohmann
        try
        {
            forecast_json = json::parse(forecast);
        }
        catch(...)
        {
            std::cout << "Exception thrown from nlohman, data parse failed." << std::endl;
        }
        json instant_details =
            forecast_json["properties"]["timeseries"][0]["data"]["instant"]["details"];
        json summary =
            forecast_json["properties"]["timeseries"][0]["data"]["next_6_hours"]["summary"]["symbol_code"];
        // Populate struct
        weather.forecast_summary = summary.dump();
        weather.air_pressure_at_sea_level = instant_details["air_pressure_at_sea_level"];
        weather.temperature = instant_details["air_temperature"];
        weather.cloud_area_fraction = instant_details["cloud_area_fraction"];
        weather.relative_humidity = instant_details["relative_humidity"];
        weather.wind_direction = instant_details["wind_from_direction"];
        weather.wind_speed = instant_details["wind_speed"];
        weather.precipitation_amount =
            forecast_json["properties"]["timeseries"][0]["data"]["next_6_hours"]["details"]["precipitation_amount"];

        return weather;   
    }

    void YrForecast::printForecast()
    {
        std::cout << std::endl;
        std::cout << "-------------" << std::endl;
        std::cout << "YR WEATHER" << std::endl;
        std::cout << "-------------" << std::endl;
        std::cout << "\nThe summary for lat " << _latitude;
        std::cout << " and long " << _longitude << " is: ";
        std::cout << _current_weather.forecast_summary.c_str() << std::endl;
        std::cout << "\nTemperature: " << _current_weather.temperature << std::endl;
        std::cout << "Fraction of cloud cover: " << _current_weather.cloud_area_fraction << std::endl;
        std::cout << "Relative humidity: " << _current_weather.relative_humidity << std::endl;
        std::cout << "Wind Direction: " << _current_weather.wind_direction << std::endl;
        std::cout << "Wind Speed: " << _current_weather.wind_speed << std::endl;
        std::cout << "Precipitation over the next 6 hours: " << _current_weather.precipitation_amount << std::endl;
        std::cout << std::endl;

        return;
    }
    std::string YrForecast::getURL(){
        return _coords_url;
    }
    void YrForecast::runProgram()
    {
        // Set _coords_url based on the class coordinate params
        createURL();
        // Retrieve forecast data from yr.no
        _forecast_data = populateForecastData(_coords_url, _easyhandle, _code);
        // Parse forecast data into member struct
        _current_weather = parseForecastJSON(_forecast_data);
        // Print struct to screen
        printForecast();
        return;
    }
} // namespace yr
