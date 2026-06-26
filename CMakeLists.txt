
cmake_minimum_required(VERSION 3.21)

project(challenge CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # todo review
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_NAME challenge)

# puts the test executable to the plugin bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# move plugin into subdirectory
add_subdirectory(plugin)

add_executable(${EXECUTABLE_NAME} app/src/main.cpp)

set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}\\bin")

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/plugin/include)

add_dependencies(${EXECUTABLE_NAME} plugin)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_DL_LIBS}
)

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

enable_testing()
add_subdirectory(tests)
