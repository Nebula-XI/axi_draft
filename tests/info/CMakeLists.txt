cmake_minimum_required(VERSION 3.16)

project(test_info LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(COMPILE_FLAGS -Wall -Wextra -Werror)

add_definitions()
include_directories(
    "../include"
    "../../include"
    "../../info"
)
add_executable(${PROJECT_NAME} test_info.cpp)

add_test(NAME test_info COMMAND test_info config/units_config.json WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(Boost 1.65 REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()
