# Run cmake with
# cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ ..
# if you have googletest and benchmark installed somewhere else say $HOME/install, add
#      -DCMAKE_PREFIX_PATH=<$HOME/install>

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

cmake_minimum_required(VERSION 3.0)
project(escape)

set(CMAKE_CXX_STANDARD 14)
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
#add_library(GTest::Main INTERFACE IMPORTED)
#add_library(GTest::GTest INTERFACE IMPORTED)
#find_package(GTest REQUIRED)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.7.0
)
FetchContent_MakeAvailable(googlebenchmark)

add_compile_options( -Wall -Werror -Wextra -march=icelake-server)

add_executable(escape escape.cpp)

add_executable(bench bench.cpp)
target_link_libraries(bench benchmark::benchmark)

enable_testing()
include(GoogleTest)
add_executable(teststr test.cpp)
target_link_libraries(teststr gtest_main)
gtest_discover_tests(teststr)

