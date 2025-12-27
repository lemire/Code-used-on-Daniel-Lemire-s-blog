cmake_minimum_required(VERSION 3.10)

project(BenchmarkProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(cmake/CPM.cmake)

include_directories(benchmarks src include)

add_executable(benchmark benchmarks/benchmark.cpp)
CPMAddPackage("gh:lemire/counters#v2.2.0")
CPMAddPackage("gh:fastfloat/fast_float@8.2.0")
CPMAddPackage(
  NAME Boost
  VERSION 1.90.0
  URL https://github.com/boostorg/boost/releases/download/boost-1.90.0/boost-1.90.0-cmake.tar.xz
  OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_SKIP_INSTALL_RULES ON" # Set `OFF` for installation
          "BUILD_SHARED_LIBS OFF" "BOOST_INCLUDE_LIBRARIES asio" # Note the escapes!
)
target_link_libraries(benchmark PRIVATE counters::counters FastFloat::fast_float Boost::asio)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
