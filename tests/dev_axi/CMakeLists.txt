cmake_minimum_required(VERSION 3.16)

project(test_dev_axi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(COMPILE_FLAGS -Wall -Wextra -Werror)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

include_directories(
    "../../dev" "../../include" "../../devices/i2c" "../include" "../../info"
)

add_executable(${PROJECT_NAME} test_dev_axi.cpp)

add_test("test_dev_axi" test_dev_axi)
