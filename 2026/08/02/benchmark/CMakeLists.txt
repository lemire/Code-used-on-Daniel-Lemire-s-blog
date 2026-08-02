cmake_minimum_required(VERSION 3.10)

project(BenchmarkProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(cmake/CPM.cmake)

include_directories(benchmarks src include)

CPMAddPackage("gh:lemire/counters#v3.1.0")
CPMAddPackage(
    NAME plf_hive
    GITHUB_REPOSITORY mattreecebentley/plf_hive
    GIT_TAG 085899f55591e77d49ed168be4594200aa0f0c3a
    DOWNLOAD_ONLY YES
)

add_executable(benchmark benchmarks/benchmark.cpp)
target_link_libraries(benchmark PRIVATE counters)
target_include_directories(benchmark PRIVATE ${plf_hive_SOURCE_DIR})

# Same benchmark, but allowed to use every instruction the host supports
# (AVX-512 on our test machine).
add_executable(benchmark_native benchmarks/benchmark.cpp)
target_link_libraries(benchmark_native PRIVATE counters)
target_include_directories(benchmark_native PRIVATE ${plf_hive_SOURCE_DIR})
target_compile_options(benchmark_native PRIVATE -march=native)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
