cmake_minimum_required(VERSION 3.10)

project(TakenBranchBenchmark VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(cmake/CPM.cmake)

add_executable(benchmark benchmark.cpp)
CPMAddPackage("gh:lemire/counters#v4.1.0")
target_link_libraries(benchmark PRIVATE counters)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
target_compile_options(benchmark PRIVATE -march=native)
