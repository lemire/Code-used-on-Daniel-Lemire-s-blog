cmake_minimum_required(VERSION 3.10)
project(safememcpy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

include(CheckCXXCompilerFlag)
unset(SAFEMEMCPY_COMPILER_SUPPORTS_SANI CACHE)
CHECK_CXX_COMPILER_FLAG(-fsanitize=thread SAFEMEMCPY_COMPILER_SUPPORTS_SANI)

add_executable(cpy src/cpy.cpp)
add_executable(safecpy src/safecpy.cpp)
target_compile_options(cpy PUBLIC -fsanitize=thread) 
target_link_options(cpy PUBLIC -fsanitize=thread) 

target_compile_options(safecpy PUBLIC -fsanitize=thread)
target_link_options(safecpy PUBLIC -fsanitize=thread)

add_executable(benchmark src/benchmark.cpp)