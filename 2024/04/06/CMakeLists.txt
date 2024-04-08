cmake_minimum_required(VERSION 3.15)
project(funserver CXX)


find_package(Crow REQUIRED)
add_executable(server src/server.cpp)
target_link_libraries(server Crow::Crow)
find_package(
    Boost 
    COMPONENTS context
)
find_package(OpenSSL REQUIRED)
if(Boost_FOUND AND OpenSSL_FOUND)
    add_executable(lithium_server src/lithium_server.cpp)
    target_link_libraries(lithium_server Boost::context)
    target_link_libraries(lithium_server OpenSSL::SSL)
    target_include_directories(lithium_server PRIVATE src)
endif(Boost_FOUND AND OpenSSL_FOUND)