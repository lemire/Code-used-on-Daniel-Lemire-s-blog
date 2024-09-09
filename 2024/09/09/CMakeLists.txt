cmake_minimum_required(VERSION 3.10)
project(float_compression_benchmark)


if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(bench benchmarks/bench.cpp)
target_include_directories(bench PRIVATE include benchmarks)