cmake_minimum_required(VERSION 3.10)

project(BenchmarkProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(benchmarks include)
include(CheckCXXCompilerFlag)
unset(SIMPLE_FAST_FLOAT_BENCHMARK_COMPILER_SUPPORTS_MARCH_NATIVE CACHE)
CHECK_CXX_COMPILER_FLAG(-march=native SIMPLE_FAST_FLOAT_BENCHMARK_COMPILER_SUPPORTS_MARCH_NATIVE)
if(SIMPLE_FAST_FLOAT_BENCHMARK_COMPILER_SUPPORTS_MARCH_NATIVE)
    add_compile_options(-march=native)
else()
     message(STATUS "native target not supported")
endif()
add_executable(benchmark benchmarks/benchmark.cpp)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

