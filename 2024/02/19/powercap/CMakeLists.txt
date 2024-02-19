
cmake_minimum_required(VERSION 3.12)

project(powercap VERSION 0.6.1
                 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
add_compile_options(-Wall)

include(GNUInstallDirs)

# See powercap-common.h for enumeration
set(POWERCAP_LOG_LEVEL 4 CACHE STRING "Set the log level: 0=DEBUG, 1=INFO, 2=WARN, 3=ERROR, 4=OFF (default)")

set(POWERCAP_CMAKE_CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/powercap)

# Libraries
add_library(powercap src/powercap.c
                     src/powercap-sysfs.c
                     src/powercap-rapl.c
                     src/powercap-rapl-sysfs.c
                     src/powercap-common.c)
target_include_directories(powercap PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
                                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>)
set_target_properties(powercap PROPERTIES PUBLIC_HEADER "inc/powercap.h;inc/powercap-sysfs.h;inc/powercap-rapl.h;inc/powercap-rapl-sysfs.h")
target_compile_definitions(powercap PRIVATE POWERCAP_LOG_LEVEL=${POWERCAP_LOG_LEVEL})
