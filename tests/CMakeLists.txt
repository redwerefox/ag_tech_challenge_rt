

set(TEST_TARGET test_plugin)
find_package(GTest REQUIRED)

# create a filegroup for source files and header files
set(PLUGIN_SOURCES
    test_plugin.cpp

    # Add other .cpp files here as you create them
)

set(PLUGIN_HEADERS

    # Add other .h files here
)

# Build the plugin as a shared library
add_executable(${TEST_TARGET}
    ${PLUGIN_SOURCES}
    ${PLUGIN_HEADERS}
)

target_include_directories(${TEST_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../plugin/include)

target_link_libraries(${TEST_TARGET} PRIVATE
    ${CMAKE_DL_LIBS}
    GTest::gtest
    GTest::gtest_main
)

add_dependencies(${TEST_TARGET} plugin)

include(GoogleTest)
gtest_discover_tests(${TEST_TARGET}
    DISCOVERY_MODE PRE_TEST
    PROPERTIES
    ENVIRONMENT "PLUGIN_PATH=$<TARGET_FILE:plugin>"
)