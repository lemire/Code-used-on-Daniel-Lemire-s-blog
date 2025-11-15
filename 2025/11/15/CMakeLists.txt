cmake_minimum_required(VERSION 3.10)

project(BenchmarkProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(cmake/CPM.cmake)

include_directories(benchmarks src include)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
add_executable(benchmark benchmarks/benchmark.cpp)
CPMAddPackage("gh:lemire/counters#v1.0.1")
CPMAddPackage("gh:simdutf/simdutf#07906a7d3fba0c24c8bb4b5de4889b790753eee5")
target_link_libraries(benchmark PRIVATE counters simdutf)

