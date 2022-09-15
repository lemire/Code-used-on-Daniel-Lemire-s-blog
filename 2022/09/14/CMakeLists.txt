# Run cmake with
# cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ ..
# if you have googletest and benchmark installed somewhere else say $HOME/install, add
#      -DCMAKE_PREFIX_PATH=<$HOME/install>

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

cmake_minimum_required( VERSION 3.0 )
project( escape )

set( CMAKE_CXX_STANDARD 14 )

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

add_compile_options( -Wall -Werror -Wextra -march=icelake-client )

add_executable( escape escape.cpp )

add_executable( bench bench.cpp )
target_link_libraries( bench benchmark::benchmark )

enable_testing()
include(GoogleTest)
add_executable( teststr test.cpp )
target_link_libraries( teststr GTest::GTest GTest::Main )
gtest_discover_tests( teststr )

