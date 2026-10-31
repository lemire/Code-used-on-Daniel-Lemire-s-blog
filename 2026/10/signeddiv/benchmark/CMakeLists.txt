cmake_minimum_required(VERSION 3.16)

project(SignedDivision32 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(cmake/CPM.cmake)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

CPMAddPackage("gh:lemire/counters#v3.2.0")
CPMAddPackage(NAME fastmod GITHUB_REPOSITORY lemire/fastmod GIT_TAG v0.2.0
              OPTIONS "BUILD_TESTING OFF")

include_directories(include)

add_executable(benchmark benchmarks/benchmark.cpp)
target_link_libraries(benchmark PRIVATE counters fastmod)
# We compare scalar code: no autovectorization.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(benchmark PRIVATE -fno-vectorize -fno-slp-vectorize)
else()
    target_compile_options(benchmark PRIVATE -fno-tree-vectorize)
endif()

add_executable(tests tests/test.cpp)
