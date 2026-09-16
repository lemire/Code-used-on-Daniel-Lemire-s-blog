cmake_minimum_required(VERSION 3.10)

project(FlatMapBenchmark VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(cmake/CPM.cmake)

include_directories(benchmarks src include)

add_executable(benchmark benchmarks/benchmark.cpp)
CPMAddPackage("gh:lemire/counters#v3.2.0")
target_link_libraries(benchmark PRIVATE counters)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
target_compile_options(benchmark PRIVATE -march=native)
