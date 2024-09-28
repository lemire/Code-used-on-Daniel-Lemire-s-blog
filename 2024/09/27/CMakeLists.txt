cmake_minimum_required(VERSION 3.16)
project(builddemo)

get_cmake_property(is_multi_config GENERATOR_IS_MULTI_CONFIG)
if(NOT is_multi_config AND NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()


include(cmake/CPM.cmake)
CPMAddPackage("gh:fmtlib/fmt#11.0.2")
CPMAddPackage("gh:simdjson/simdjson@3.10.1")
CPMAddPackage("gh:fastfloat/fast_float@6.1.6")
add_executable(main main.cpp)
target_link_libraries(main fmt::fmt simdjson::simdjson FastFloat::fast_float)
