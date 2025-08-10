cmake_minimum_required(VERSION 3.14)

project(BenchmarkProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(cmake/CPM.cmake)

add_executable(benchmark benchmarks/benchmark.cpp)
target_include_directories(benchmark PRIVATE benchmarks)

set(SIMDUTF_TESTS off)
set(SIMDUTF_TOOLS off)
set(SIMDUTF_BENCHMARKS off)
CPMAddPackage("gh:simdutf/simdutf@7.3.5")
CPMAddPackage("gh:fmtlib/fmt#10.2.1")
CPMAddPackage("gh:lemire/counters#v1.0.1")
target_link_libraries(benchmark PRIVATE fmt::fmt)
target_link_libraries(benchmark PRIVATE simdutf::simdutf)
target_link_libraries(benchmark PRIVATE counters)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

enable_testing()
add_test(NAME BenchmarkSIMDUTFfind COMMAND benchmark)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    message(STATUS "Compiler supports -march=native, enabling it.")
    target_compile_options(benchmark PRIVATE -march=native)
endif()

# ignore build directory
file(GENERATE OUTPUT .gitignore CONTENT "*")
