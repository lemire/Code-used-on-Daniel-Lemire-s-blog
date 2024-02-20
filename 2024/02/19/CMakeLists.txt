cmake_minimum_required(VERSION 3.15)

project(powerdemo
  DESCRIPTION "Demonstrates how to use the powercap library to measure the power consumption of a CPU."
  LANGUAGES C
  VERSION 1.0.0
)
set(CMAKE_C_FLAGS "-march=native")
if( NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

add_subdirectory(powercap)
add_executable(powerdemo src/demo.c)
target_link_libraries(powerdemo powercap)