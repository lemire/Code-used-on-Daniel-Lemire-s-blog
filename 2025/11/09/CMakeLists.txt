cmake_minimum_required(VERSION 3.20)

project(MyProject VERSION 1.0 LANGUAGES CXX)

include(CheckCXXCompilerFlag)

# Check for required compiler flags
set(REQUIRED_FLAGS "-freflection;-fexpansion-statements;-stdlib=libc++")


# Add the flags
add_compile_options(${REQUIRED_FLAGS} --std=c++26)

add_executable(main src/main.cpp)
target_include_directories(main PRIVATE include)