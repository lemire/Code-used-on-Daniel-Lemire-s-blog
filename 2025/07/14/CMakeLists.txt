cmake_minimum_required(VERSION 3.10)
project(BenchmarkProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
    if(COMPILER_SUPPORTS_MARCH_NATIVE)
        message(STATUS "Compiler supports -march=native, enabling it.")
        add_compile_options(-march=native)
    endif()


include(cmake/CPM.cmake)
CPMAddPackage("gh:fmtlib/fmt#10.2.1")
CPMAddPackage("gh:lemire/counters#v1.0.1")



add_executable(benchmark benchmarks/benchmark.cpp)
target_link_libraries(benchmark PRIVATE fmt::fmt)
target_link_libraries(benchmark PRIVATE counters)
