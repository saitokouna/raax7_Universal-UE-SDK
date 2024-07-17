cmake_minimum_required(VERSION 3.15)
project(UGSDK)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

add_subdirectory(dependencies/libhat)

set(UGSDK_SRC
    src/temp.cpp
)

add_library(UGSDK STATIC ${UGSDK_SRC})

target_include_directories(UGSDK PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)