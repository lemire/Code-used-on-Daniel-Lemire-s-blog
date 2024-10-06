cmake_minimum_required(VERSION 3.10) 
project(Benchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Add executable
add_executable(benchmark benchmarks/benchmark.cpp)

# Specify include directories
target_include_directories(benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks)

# Set the default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()