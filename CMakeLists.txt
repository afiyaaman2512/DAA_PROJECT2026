cmake_minimum_required(VERSION 3.15)
project(LinkShield VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

# libcurl
find_package(CURL REQUIRED)

# Gumbo
find_library(GUMBO_LIB gumbo REQUIRED)
find_path(GUMBO_INCLUDE gumbo.h)
include_directories(${GUMBO_INCLUDE})

file(GLOB SOURCES "src/*.cpp")

add_executable(linkshield ${SOURCES})

target_link_libraries(linkshield CURL::libcurl ${GUMBO_LIB})
# Copy data folder to build directory
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
