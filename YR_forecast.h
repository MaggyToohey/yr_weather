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

#include <nlohmann/json.hpp>


namespace yr
{

    /**
 * @brief Structure containing information about the forecast
 */
    struct YrForecastStruct
    {
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
    class YrForecast
    {

    public:
    /**
     * @brief Construct a new YrForecast object
     * @param latitude The latitude of the required forecast
     * @param longitude The longitude of the required forecast
     * @param altitude The altitude of the required forecast
    */     
        YrForecast(const float &latitude, 
                    const float &longitude, 
                    const int &altitude);
    
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
    /**
     * @brief Create a URL based on location
     */
    void createURL();

    /**
     * @brief Parse JSON data returned from yr.no into struct
     */
    void parseForecastJSON();

    std::string getURL();
    yr::YrForecastStruct getParsedData();

    private:

    
    /**
     * @brief Send request to yr.no and store result
     */
    bool populateForecastData();
    /**
     * @brief Print the forecast to console
     */
    void printForecast();

    // URL data
    const std::string _base_url = "https://api.met.no/weatherapi/locationforecast/2.0/compact.json?";
    std::string _coords_url;

    // Structs to hold parsed data
    YrForecastStruct _current_weather;

    // Location data
    float _latitude = 0.0;
    float _longitude = 0.0;
    // Altitude must be int, defaults to sea level;
    int _altitude = 0;
    bool _URL_complete = false;
    };

    /**
     * @brief Template struct to manage the user input coords
     */
    template <typename T>
    struct HandleGeoCoords
    {
        T coord;
        HandleGeoCoords() = default;

        float validateLatitude(int coord)
        {
            std::cout << "Latitude entered: " << coord << std::endl;
            // Check value is valid
            if ((-85 <= coord && coord <= 85))
            {
                return coord;
            }
            else
            {
                std::cout << "Latitude out of bounds. ";
                std::cout << "Valid range is between -85 and 85 degrees. ";
                std::cout << "Exiting program." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        float validateLatitude(float coord)
        {
            std::cout << "Latitude entered: " << coord << std::endl;
            // Check value is valid
            if ((-85.0 <= coord && coord <= 85.0))
            {
                return coord;
            }
            else
            {
                std::cout << "Latitude out of bounds. ";
                std::cout << "Valid range is between -85 and 85 degrees. ";
                std::cout << "Exiting program." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        float validateLatitude(double coord)
        {
            std::cout << "Latitude entered: " << coord << std::endl;
            // Check value is valid
            if ((-85.0 <= coord && coord <= 85.0))
            {
                // Cast to float, Yr needs only 4 dec places
                return static_cast<float>(coord);
            }
            else
            {
                std::cout << "Latitude out of bounds. ";
                std::cout << "Valid range is between -85 and 85 degrees. ";
                std::cout << "Exiting program." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        float validateLatitude(std::string coord)
        {
            std::cout << "Latitude entered: " << coord << std::endl;
            // URL does not except string, defaul lat
            std::cout << "Latitude must be float value, " <<
                "setting to default, 0.0" << std::endl;
            return 0.0;
        }

        // Validate Longitude
        float validateLongitude(int coord)
        {
            std::cout << "Longitude entered: " << coord << std::endl;
            // Check value is valid
            if ((-180 <= coord && coord <= 180))
            {
                return coord;
            }
            else
            {
                std::cout << "Longitude out of bounds, exiting." << std::endl;
                std::cout << "Valid range is between -180 and 180 degrees. ";
                std::cout << "Exiting program." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        float validateLongitude(float coord)
        {
            std::cout << "Longitude entered: " << coord << std::endl;
            if ((-180.0 <= coord && coord <= 180.0))
            {
                return coord;
            }
            else
            {
                std::cout << "Longitude out of bounds, exiting." << std::endl;
                std::cout << "Valid range is between -180 and 180 degrees. ";
                std::cout << "Exiting program." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        float validateLongitude(double coord)
        {
            std::cout << "Longitude entered: " << coord << std::endl;
            if ((-180.0 <= coord && coord <= 180.0))
            {
                // Cast to float, Yr needs only 4 dec places
                return static_cast<float>(coord);
            }
            else
            {
                std::cout << "Longitude out of bounds, exiting." << std::endl;
                std::cout << "Valid range is between -180 and 180 degrees. ";
                std::cout << "Exiting program." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        float validateLongitude(std::string coord)
        {
            // String not accepted, set longitude as default
            std::cout << "Longitude entered: " << coord << std::endl;
            std::cout << "Longtude must be float value, " <<
                "setting to default, 0.0" << std::endl;
            return 0.0;
        }
        // Validate Altitude
        int validateAltitude(int coord)
        {
            std::cout << "Altitude value entered: " << std::endl;
            return coord;
        }
        int validateAltitude(float coord)
        {
            std::cout << "Altitude value entered: " << coord << std::endl;
            return static_cast<int>(coord);
        }
        int validateAltitude(double coord)
        {
            std::cout << "Altitude value entered: " << coord << std::endl;
            return static_cast<int>(coord);
        }
        int validateAltitude(std::string coord)
        {
            std::cout << "Altitude value entered: " << coord << std::endl;
            std::cout << "String is not valid, ";
            std::cout << "using sea level as default." << std::endl;
            return 0;

        }
    };

} // namespace yr

#endif //YR_FORECAST_H
