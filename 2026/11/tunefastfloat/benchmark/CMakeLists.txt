cmake_minimum_required(VERSION 3.16)

project(TuneFastFloat VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(cmake/CPM.cmake)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# fast_float with performance hints (pull request 434).
CPMAddPackage(
  NAME fast_float
  GITHUB_REPOSITORY fastfloat/fast_float
  GIT_TAG performance-hints
  OPTIONS "FASTFLOAT_TEST OFF")
CPMAddPackage("gh:lemire/counters#v4.1.0")

# One parsing loop per configuration, each in its own translation unit, as a
# program would have it: in one file, the extra copies of the parser could
# change the compiler's inlining decisions.
set(CONFIGS
  "none\;0"
  "sign\;fast_float::performance_hints::unpredictable_sign"
  "short\;fast_float::performance_hints::short_numbers"
  "nearest\;fast_float::performance_hints::assume_round_to_nearest"
  "sign_nearest\;fast_float::performance_hints::unpredictable_sign|fast_float::performance_hints::assume_round_to_nearest"
  "short_nearest\;fast_float::performance_hints::short_numbers|fast_float::performance_hints::assume_round_to_nearest"
  "all\;fast_float::performance_hints::unpredictable_sign|fast_float::performance_hints::short_numbers|fast_float::performance_hints::assume_round_to_nearest")
set(KERNELS "")
foreach(config IN LISTS CONFIGS)
  list(GET config 0 name)
  list(GET config 1 hints)
  add_library(kernel_${name} OBJECT kernel.cpp)
  target_link_libraries(kernel_${name} PRIVATE fast_float)
  target_compile_definitions(kernel_${name} PRIVATE KERNEL=sum_${name})
  if(NOT name STREQUAL "none")
    target_compile_definitions(kernel_${name} PRIVATE "HINTS=(${hints})")
  endif()
  list(APPEND KERNELS $<TARGET_OBJECTS:kernel_${name}>)
endforeach()

add_executable(benchmark bench.cpp ${KERNELS})
target_compile_options(benchmark PRIVATE -Wall -Wextra)
target_link_libraries(benchmark PRIVATE counters)
