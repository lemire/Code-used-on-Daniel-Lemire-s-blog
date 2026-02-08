cmake_minimum_required(VERSION 3.10)

project(BenchmarkProject VERSION 1.0)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(WARNING "Visual Studio is not supported")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(cmake/CPM.cmake)

include_directories(benchmarks src include)

add_library(mylib src/mylib.cpp)

add_executable(benchmark benchmarks/benchmark.cpp)
CPMAddPackage("gh:lemire/counters#v2.3.0")
target_link_libraries(benchmark PRIVATE mylib counters)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
