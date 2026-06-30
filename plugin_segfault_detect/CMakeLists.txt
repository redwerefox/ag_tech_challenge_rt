
set(TARGET_NAME plugin_segfault_detect)

# create a filegroup for source files and header files
set(PLUGIN_SOURCES
    src/plugin.cpp

    # Add other .cpp files here as you create them
)

set(PLUGIN_HEADERS
    include/plugin/plugin.h

    # Add other .h files here
)

# Build the plugin as a shared library
add_library(${TARGET_NAME} SHARED
    ${PLUGIN_SOURCES}
    ${PLUGIN_HEADERS}
)

target_compile_definitions(${TARGET_NAME} PRIVATE PLUGIN_BUILDING)
target_include_directories(${TARGET_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_definitions(${TARGET_NAME} PUBLIC
    CRASH_LOG_PATH="${CRASH_LOG_PATH}"
)

find_package(Boost REQUIRED)

target_link_libraries(${TARGET_NAME} PRIVATE
    boost::boost
)

if(WIN32 AND Boost_USE_STATIC_LIBS STREQUAL "OFF")
    target_compile_definitions(${TARGET_NAME} PRIVATE BOOST_ALL_DYN_LINK)
endif()
