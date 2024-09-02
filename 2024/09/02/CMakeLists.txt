cmake_minimum_required(VERSION 3.10)
project(float_compression_benchmark)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(cmake/CPM.cmake)
CPMAddPackage("gh:fastfloat/fast_float@6.1.5")
# CURL
find_package(CURL REQUIRED)
message(STATUS "curl version " ${CURL_VERSION_STRING})

add_executable(compress benchmarks/compress.cpp)
target_include_directories(compress PRIVATE include benchmarks)
target_link_libraries(compress PRIVATE fast_float)

target_include_directories(compress PRIVATE ${CURL_INCLUDE_DIRS})
if(NOT CURL_LIBRARIES)
    target_link_libraries(compress PRIVATE CURL::libcurl)
else()
    target_link_libraries(compress PRIVATE ${CURL_LIBRARIES})
endif()