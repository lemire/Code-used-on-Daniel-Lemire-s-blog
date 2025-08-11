cmake_minimum_required(VERSION 3.14)

project(BenchmarkProject VERSION 1.0 LANGUAGES CXX)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(cmake/CPM.cmake)

option(use_fmtlib "Use fmtlib for formatting")

add_executable(benchmark benchmarks/benchmark.cpp)
target_include_directories(benchmark PRIVATE benchmarks)
add_test(NAME BenchmarkNative COMMAND benchmark)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
  message(STATUS "Compiler supports -march=native, enabling it.")
  target_compile_options(benchmark PRIVATE -march=native)
endif()

add_executable(benchmark_generic benchmarks/benchmark.cpp)
target_include_directories(benchmark_generic PRIVATE benchmarks)
add_test(NAME BenchmarkGeneric COMMAND benchmark_generic)

set_tests_properties(BenchmarkNative BenchmarkGeneric PROPERTIES RUN_SERIAL true)

set(SIMDUTF_TESTS off)
set(SIMDUTF_TOOLS off)
set(SIMDUTF_BENCHMARKS off)
CPMAddPackage("gh:simdutf/simdutf@7.3.5")
CPMAddPackage("gh:lemire/counters#v1.0.1")

if(use_fmtlib)
# Doesn't build on macOS with Clang, the default compiler
# benchmark performance not impacted at all using <format> and <iostream> instead
  CPMAddPackage("gh:fmtlib/fmt#10.2.1")
  target_link_libraries(benchmark PRIVATE fmt::fmt)
  target_link_libraries(benchmark_generic PRIVATE fmt::fmt)
endif()

target_link_libraries(benchmark PRIVATE simdutf::simdutf)
target_link_libraries(benchmark_generic PRIVATE simdutf::simdutf)
target_link_libraries(benchmark PRIVATE counters)
target_link_libraries(benchmark_generic PRIVATE counters)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ignore build directory
file(GENERATE OUTPUT .gitignore CONTENT "*")
