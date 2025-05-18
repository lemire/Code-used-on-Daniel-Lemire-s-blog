cmake_minimum_required(VERSION 3.20)
project(DemoBinaries LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(demo1 src/demo1.cpp)
add_executable(demo2 src/demo2.cpp)
add_executable(demo3 src/demo3.cpp)
