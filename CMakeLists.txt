cmake_minimum_required(VERSION 3.15)
project(UGSDK)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

add_subdirectory(dependencies/libhat)

set(UGSDK_SRC
    
)

add_library(UGSDK STATIC ${UGSDK_SRC} "include/FMemory.hpp" "src/FMemory.cpp" "src/private/FMemory.cpp" "include/private/Private.hpp" "src/UGSDK.cpp")

target_include_directories(UGSDK PUBLIC
    dependencies/libhat/include
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(UGSDK PUBLIC
    libhat
)