cmake_minimum_required(VERSION 3.10)

project(SubnormalBenchmark VERSION 1.0)

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
# GCC and clang accept -march=native on x86-64, on 64-bit ARM Linux and on
# Apple Silicon (the source file #errors out on any other architecture).
target_compile_options(benchmark PRIVATE -march=native)
