cmake_minimum_required(VERSION 3.15)
project(funserver CXX)


find_package(Crow REQUIRED)
add_executable(server src/server.cpp)
target_link_libraries(server Crow::Crow)