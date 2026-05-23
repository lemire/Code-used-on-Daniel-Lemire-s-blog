cmake_minimum_required(VERSION 3.10)

project(BenchmarkProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(cmake/CPM.cmake)

include_directories(benchmarks src include)

add_executable(benchmark benchmarks/benchmark.cpp)
target_include_directories(benchmark PRIVATE include)
target_compile_options(benchmark PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-march=native>
)

CPMAddPackage("gh:lemire/counters#v3.1.0")
target_link_libraries(benchmark PRIVATE counters)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
