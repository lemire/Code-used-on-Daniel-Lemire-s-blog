cmake_minimum_required(VERSION 3.10)

project(cpp_servers
  LANGUAGES CXX C
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

# add executables
add_executable(hello-libhv hello-libhv.cpp)
add_executable(hello-cinatra hello-cinatra.cpp)
add_executable(hello-h2o hello-h2o.cpp)
add_executable(hello-facil hello-facil.c)

# add dependencies
# You can add CPM.cmake like so:
# mkdir -p cmake
# wget -O cmake/CPM.cmake https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
include(cmake/CPM.cmake)
CPMAddPackage("gh:boazsegev/facil.io#0.7.6")

#CPMAddPackage("gh:qicosmos/cinatra#2422dfe4fd48a668b94d01afb72a5bcbe0b28df8")
CPMAddPackage(
  NAME cinatra
  GITHUB_REPOSITORY "qicosmos/cinatra"
  GIT_TAG 2422dfe4fd48a668b94d01afb72a5bcbe0b28df8
  OPTIONS "ENABLE_SANITIZER OFF" "BUILD_UNIT_TESTS OFF" "BUILD_EXAMPLES OFF" "BUILD PRESS_TOOL OFF"
)

set(h2o_options)
list(APPEND h2o_options "WITH_MRUBY OFF")
CPMAddPackage(NAME h2o GIT_REPOSITORY https://github.com/h2o/h2o GIT_TAG v2.2.6 VERSION 2.2.6 OPTIONS "${h2o_options}")

add_library(h2o-headers INTERFACE)
target_include_directories(h2o-headers INTERFACE
  $<BUILD_INTERFACE:${h2o_SOURCE_DIR}/include/>)

add_library(cinatra-headers INTERFACE)
target_include_directories(cinatra-headers INTERFACE
  $<BUILD_INTERFACE:${cinatra_SOURCE_DIR}/include/>)
#CPMAddPackage(
#  NAME lithium
#  GITHUB_REPOSITORY "matt-42/lithium"
#  GIT_TAG 9ce90b
#)
CPMAddPackage("gh:the-moisrex/webpp#fe12840")
CPMAddPackage("gh:fmtlib/fmt#7.1.3")
CPMAddPackage("gh:ithewei/libhv@1.3.1")
#CPMAddPackage("gh:yhirose/cpp-httplib@0.14.1")

# link dependencies
target_link_libraries(hello-libhv hv fmt::fmt)
target_link_libraries(hello-cinatra cinatra-headers fmt::fmt)
target_link_libraries(hello-h2o libh2o-evloop h2o-headers fmt::fmt)
target_link_libraries(hello-facil facil.io fmt::fmt)
