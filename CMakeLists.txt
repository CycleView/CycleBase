cmake_minimum_required(VERSION 2.8.11)
set(PROJECT CycleBase)
project(${PROJECT})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -lwiringPi")

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

include_directories(src)

set(HEADERS
    src/light.h
    src/ruler.h
    src/server.h
)

set(SOURCES
    src/main.cpp
    src/light.cpp
    src/ruler.cpp
    src/server.cpp
)

add_executable(${PROJECT} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT} Qt5::Core Qt5::Network)
