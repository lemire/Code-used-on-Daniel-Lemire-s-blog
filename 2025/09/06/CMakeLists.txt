cmake_minimum_required(VERSION 3.10)

project(BenchmarkProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    add_compile_options(-march=native)
endif()
include(cmake/CPM.cmake)

include_directories(benchmarks include)

add_library(scalar src/scalar.cpp)

add_executable(benchmark benchmarks/benchmark.cpp)

CPMAddPackage("gh:simdutf/simdutf@7.3.5")
CPMAddPackage("gh:fmtlib/fmt#10.2.1")
CPMAddPackage("gh:lemire/counters#v1.0.1")
target_link_libraries(benchmark PRIVATE fmt::fmt)
target_link_libraries(benchmark PRIVATE simdutf::simdutf)
target_link_libraries(benchmark PRIVATE counters)
target_link_libraries(benchmark PRIVATE scalar)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
