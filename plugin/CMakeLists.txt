cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TARGET_NAME plugin)

# get the project name from the subdirectory name
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${PROJECT_NAME} CXX)

# create a filegroup for source files and header files
set(PLUGIN_SOURCES
    src/plugin.cpp

    # Add other .cpp files here as you create them
)

set(PLUGIN_HEADERS
    include/plugin.h

    # Add other .h files here
)

# Build the plugin as a shared library
add_library(${PROJECT_NAME} SHARED
    ${PLUGIN_SOURCES}
    ${PLUGIN_HEADERS}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE PLUGIN_BUILDING)
target_include_directories(${TARGET_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Linking dependencies
# todo add boost log to conan
find_package(Boost REQUIRED COMPONENTS log)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::log)
