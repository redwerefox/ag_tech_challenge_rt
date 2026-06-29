

set(TEST_TARGET test_plugin)
find_package(GTest REQUIRED)

# create a filegroup for source files and header files
set(PLUGIN_SOURCES
    test_plugin.cpp
    test_plugin_segfault_detect.cpp

    # Add other .cpp files here as you create them
)

set(PLUGIN_HEADERS

    # Add other .h files here
)

# Build the test_plugin as an executable
add_executable(${TEST_TARGET}
    ${PLUGIN_SOURCES}
    ${PLUGIN_HEADERS}
)

target_include_directories(${TEST_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../plugin/include)
target_include_directories(${TEST_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../plugin_segfault_detect/include)

target_link_libraries(${TEST_TARGET} PRIVATE
    ${CMAKE_DL_LIBS}
    GTest::gtest
    GTest::gtest_main
)

add_dependencies(${TEST_TARGET} plugin)
add_dependencies(${TEST_TARGET} plugin_segfault_detect)

if(WIN32)
    add_custom_command(TARGET ${TEST_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:plugin>
        $<TARGET_FILE_DIR:${TEST_TARGET}>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:plugin_segfault_detect>
        $<TARGET_FILE_DIR:${TEST_TARGET}>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${TEST_TARGET}>
        $<TARGET_FILE_DIR:${TEST_TARGET}>

        COMMAND_EXPAND_LISTS
    )
endif()

include(GoogleTest)

# Create a standard CMake list of your environment variables
set(TEST_ENV
    "PLUGIN_PATH=$<TARGET_FILE:plugin>"
    "DETECTOR_PATH=$<TARGET_FILE:plugin_segfault_detect>"
)

# Join them using escaped semicolons
string(JOIN "\\;" TEST_ENV_ESCAPED ${TEST_ENV})

# Pass the final escaped string to the macro
gtest_discover_tests(${TEST_TARGET}
    DISCOVERY_MODE POST_BUILD
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${TEST_TARGET}>
    PROPERTIES
    ENVIRONMENT "${TEST_ENV_ESCAPED}"
)