
cmake_minimum_required(VERSION 3.21)

project(challenge CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # todo review
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0207)
    cmake_policy(SET CMP0207 NEW) # stop CPack cluttering output with normalized paths
endif()

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "0.1")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)

set(EXECUTABLE_NAME challenge)

# executables → bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# shared libs → lib (Linux), bin (Windows)
if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
else()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

# if(UNIX AND NOT APPLE)
# set(CMAKE_BUILD_RPATH "$ORIGIN/../lib")
# set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# endif()

# move plugin into subdirectory
add_subdirectory(plugin)

set(CRASH_LOG_DIR "${CMAKE_BINARY_DIR}" CACHE PATH "Where crash logs go")
set(CRASH_LOG_PATH "${CRASH_LOG_DIR}/crash.log")
add_subdirectory(plugin_segfault_detect)

add_executable(${EXECUTABLE_NAME} app/src/main.cpp)

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/plugin/include)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/plugin_segfault_detect/include)

add_dependencies(${EXECUTABLE_NAME} plugin)
add_dependencies(${EXECUTABLE_NAME} plugin_segfault_detect)

if(WIN32)
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:plugin>
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:plugin_segfault_detect>
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
    )
endif()

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_DL_LIBS}
)

install(TARGETS plugin
    RUNTIME_DEPENDENCIES
    PRE_EXCLUDE_REGEXES "api-ms-.*" "ext-ms-.*"
    POST_EXCLUDE_REGEXES ".*[Ss]ystem32.*"
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(TARGETS plugin_segfault_detect
    RUNTIME_DEPENDENCIES
    PRE_EXCLUDE_REGEXES "api-ms-.*" "ext-ms-.*"
    POST_EXCLUDE_REGEXES ".*[Ss]ystem32.*"
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(TARGETS ${EXECUTABLE_NAME}
    RUNTIME DESTINATION bin
)

enable_testing()
add_subdirectory(tests)
