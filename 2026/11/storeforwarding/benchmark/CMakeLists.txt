cmake_minimum_required(VERSION 3.16)

project(StoreForwarding VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(cmake/CPM.cmake)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Scalar stores and loads. Vectorizing the "fixed" loops would mix a second
# optimization into the comparison.
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(NO_VECTORIZE -fno-vectorize -fno-slp-vectorize)
else()
    set(NO_VECTORIZE -fno-tree-vectorize)
endif()

add_executable(benchmark bench.cpp forward.c)
target_compile_options(benchmark PRIVATE -Wall -Wextra ${NO_VECTORIZE})

CPMAddPackage("gh:lemire/counters#v4.1.0")
target_link_libraries(benchmark PRIVATE counters)
