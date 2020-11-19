# yr_weather

Library to retrieve weather data from the Norwegian weather service, yr.no, based on a location, using their API.

The format of the location must be in the form Latitude, Longitude (required) and Altitude (optional, if not provided, default value of sea level will be used).

A demo executable has been provided, to build and run, please follow the steps below.

For more information about the yr.no API, please see https://developer.yr.no/doc/TermsOfService/

## Requires

### Curl

#### Linux

To install curl on linux, use `sudo apt install curl`

#### Mac OSX

To install curl on mac, use `brew install curl`

#### Windows

To install on windows, go to http://curl.haxx.se/download.html and follow instructions.*

### GTest

In order to run unit tests, GTest must be installed.
See https://github.com/google/googletest/blob/master/googletest/README.md for more.

### nlohmann

The nlohmann library is required for JSON data parsing, please see 
https://github.com/nlohmann/json for details how to install.

Please ensure nlohmann minimum version 3.1.0 is used.

## Installation

To use Yr weather app, do the following:

1. Clone repo with `git clone https://github.com/MaggyToohey/yr_weather.git`
2. Checkout develop branch `git checkout develop`
3. Build folder: `mkdir build` followed by `cd build`
4. Compile using CMake: `cmake ../ .` followed by `make`
5. To run a demo exe `bash Demo`
6. To run unit tests, ensure `test_weather_data.txt` is the one provided, and run `bash Test`
  
## Licence

Licensed under CC BY 4.0 license.

## Notes

**Note:** *The Yr Weather project has not been build for windows, please build on a unix based system.
