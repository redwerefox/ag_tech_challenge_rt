cmake_minimum_required(VERSION 3.21)

project(test_plugin CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # todo review
set(CMAKE_CXX_EXTENSIONS OFF)

# create a filegroup for source files and header files
set(PLUGIN_SOURCES
    test_plugincpp

    # Add other .cpp files here as you create them
)

set(PLUGIN_HEADERS

    # Add other .h files here
)

# Build the plugin as a shared library
add_executable(${PROJECT_NAME}
    ${PLUGIN_SOURCES}
    ${PLUGIN_HEADERS}
)

# Link dynamic plugin and googletest
target_link_libraries(${PROJECT_NAME} PRIVATE
    plugin
    gtest
    gtest_main
)