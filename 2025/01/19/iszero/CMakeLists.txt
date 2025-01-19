cmake_minimum_required(VERSION 3.10)

project(BenchmarkProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(benchmarks include)

add_executable(benchmark benchmarks/benchmark.cpp)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

