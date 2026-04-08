cmake_minimum_required(VERSION 3.15)
project(LinkShield VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

find_package(CURL REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_executable(linkshield ${SOURCES})

target_link_libraries(linkshield CURL::libcurl)
