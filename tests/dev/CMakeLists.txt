cmake_minimum_required(VERSION 3.16)

project(test_dev LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(COMPILE_FLAGS -Wall -Wextra -Werror)

add_definitions()
include_directories(
    "../include"
    "../../dev"
    "../../include"
    "../../devices/i2c"
    "../../info"
)
add_executable(${PROJECT_NAME} test_dev.cpp)

add_test(NAME test_dev COMMAND test_dev config/units_config.json WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(Boost 1.65 REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()
