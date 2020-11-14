cmake_minimum_required(VERSION 3.0)

project(YR_Weather)

add_compile_options(-std=c++11)

# Include libcurl
set(CURL_INCLUDE_DIR "/usr/include/curl")
include_directories(${CURL_INCLUDE_DIR})

# Find gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

# Find nlohmann
find_package(nlohmann_json 3.2.0 REQUIRED)

# Compiling
# Build YR_forecast target as lib
add_library(YR_forecast YR_forecast.cpp YR_forecast.h)
# Build exe for testing
add_executable (Test test.cpp)
# Build exe for demo
add_executable(Demo main.cpp)

# Linking
target_link_libraries(YR_forecast curl nlohmann_json::nlohmann_json)
target_link_libraries(Test YR_forecast GTest)
target_link_libraries(Demo YR_forecast)

# Enable testing
enable_testing ()
add_test (NAME Test COMMAND Test)

