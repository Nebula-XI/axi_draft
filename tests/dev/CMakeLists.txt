cmake_minimum_required(VERSION 3.16)

project(test_dev LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(COMPILE_FLAGS -Wall -Wextra -Werror)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

add_definitions(-DBOOST_JSON_STANDALONE)
include_directories(
    "../../dev" "../../include" "../../devices/i2c" "../include" "../../info"
)
add_executable(${PROJECT_NAME} test_dev.cpp)

find_package(Boost 1.65 REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

add_test("test_dev" test_dev)
