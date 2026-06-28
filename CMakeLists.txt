
cmake_minimum_required(VERSION 3.21)

project(challenge CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # todo review
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0207 NEW) #stop cpack cluttering the output with normalizes paths
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "your_project")
set(CPACK_PACKAGE_VERSION "1.0.0")

# Optional but recommended
set(CPACK_GENERATOR "TGZ;ZIP")

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

if(UNIX AND NOT APPLE)
    set(CMAKE_BUILD_RPATH "$ORIGIN/../lib")
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# move plugin into subdirectory
add_subdirectory(plugin)

add_executable(${EXECUTABLE_NAME} app/src/main.cpp)

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/plugin/include)

add_dependencies(${EXECUTABLE_NAME} plugin)

if(WIN32)
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:plugin>
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:plugin>
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>

        COMMAND_EXPAND_LISTS
    )
endif()

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_DL_LIBS}
)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "" FORCE)

install(TARGETS plugin
    RUNTIME_DEPENDENCIES
    PRE_EXCLUDE_REGEXES "api-ms-.*" "ext-ms-.*"
    POST_EXCLUDE_REGEXES ".*system32.*"
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(TARGETS challenge
    RUNTIME DESTINATION bin
)

enable_testing()
add_subdirectory(tests)
