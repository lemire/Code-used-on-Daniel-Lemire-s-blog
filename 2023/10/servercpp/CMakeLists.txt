cmake_minimum_required(VERSION 3.10)

project(cpp_servers
  LANGUAGES CXX C
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# add executables
add_executable(hello-libhv hello-libhv.cpp)

# add dependencies
# You can add CPM.cmake like so:
# mkdir -p cmake
# wget -O cmake/CPM.cmake https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
include(cmake/CPM.cmake)

#CPMAddPackage(
#  NAME lithium
#  GITHUB_REPOSITORY "matt-42/lithium"
#  GIT_TAG 9ce90b
#)
CPMAddPackage("gh:fmtlib/fmt#7.1.3")
CPMAddPackage("gh:ithewei/libhv@1.3.1")
#CPMAddPackage("gh:yhirose/cpp-httplib@0.14.1")

# link dependencies
target_link_libraries(hello-libhv hv fmt::fmt)
