
cmake_minimum_required(VERSION 3.21)

project(challenge CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # todo review
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_NAME challenge)

# move plugin into subdirectory
add_subdirectory(plugin)

# plugin linking comes below
# target_link_libraries(${PROJECT_NAME} PRIVATE
# plugin
# #todo
# )
add_executable(${EXECUTABLE_NAME} app/src/main.cpp)

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/plugin/include)

add_dependencies(${EXECUTABLE_NAME} plugin)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_DL_LIBS}
)

add_subdirectory(tests)
