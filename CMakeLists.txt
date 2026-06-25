
cmake_minimum_required(VERSION 3.21)

project(challenge CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # todo review
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_NAME challenge)

# mirror the installation directory structure in the build directory
if(WIN32)
    # Windows native behavior: Exe and DLLs must be in the same folder
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  # .dll goes to bin/
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  # .lib goes to lib/
else()
    # Linux native behavior: Exe in bin/, Shared Objects in lib/
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  # .so goes to lib/
endif()

# move plugin into subdirectory
add_subdirectory(plugin)

add_executable(${EXECUTABLE_NAME} app/src/main.cpp)

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/plugin/include)

add_dependencies(${EXECUTABLE_NAME} plugin)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_DL_LIBS}
)

add_subdirectory(tests)
