
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

if(UNIX AND NOT APPLE)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_SET_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

find_package(Boost REQUIRED COMPONENTS log)

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

function(extract_link_targets input out_var)
    set(result "")

    foreach(item ${input})
        if(TARGET "${item}")
            list(APPEND result "${item}")
            continue()
        endif()

        if(item MATCHES "^\\$<LINK_ONLY:([^>]+)>$")
            set(link_only_target "${CMAKE_MATCH_1}")
            if(TARGET "${link_only_target}")
                list(APPEND result "${link_only_target}")
            endif()
            continue()
        endif()

        string(REGEX MATCHALL "\\$<\\$<CONFIG:[^>]+>:([^>$][^>]*)>" config_matches "${item}")
        foreach(config_match ${config_matches})
            string(REGEX REPLACE "\\$<\\$<CONFIG:[^>]+>:([^>]+)>" "\\1" config_targets "${config_match}")
            foreach(config_target ${config_targets})
                if(TARGET "${config_target}")
                    list(APPEND result "${config_target}")
                endif()
            endforeach()
        endforeach()

        string(REGEX MATCHALL "\\$<TARGET_NAME_IF_EXISTS:([^>]+)>" target_name_if_exists_matches "${item}")
        foreach(target_name_if_exists_match ${target_name_if_exists_matches})
            string(REGEX REPLACE "\\$<TARGET_NAME_IF_EXISTS:([^>]+)>" "\\1" target_name_if_exists "${target_name_if_exists_match}")
            if(TARGET "${target_name_if_exists}")
                list(APPEND result "${target_name_if_exists}")
            endif()
        endforeach()

        string(REGEX MATCHALL "\\$<TARGET_NAME:([^>]+)>" target_name_matches "${item}")
        foreach(target_name_match ${target_name_matches})
            string(REGEX REPLACE "\\$<TARGET_NAME:([^>]+)>" "\\1" target_name "${target_name_match}")
            if(TARGET "${target_name}")
                list(APPEND result "${target_name}")
            endif()
        endforeach()
    endforeach()

    if(result)
        list(REMOVE_DUPLICATES result)
    endif()

    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

function(collect_shared_dependency_targets root_target out_var)
    set(queue "${root_target}")
    set(visited "")
    set(result "")

    while(queue)
        list(POP_FRONT queue current_target)

        if(NOT TARGET "${current_target}")
            continue()
        endif()

        get_target_property(aliased_target "${current_target}" ALIASED_TARGET)
        if(aliased_target)
            set(current_target "${aliased_target}")
        endif()

        if("${current_target}" IN_LIST visited)
            continue()
        endif()
        list(APPEND visited "${current_target}")

        get_target_property(target_type "${current_target}" TYPE)
        get_target_property(imported_target "${current_target}" IMPORTED)
        if(imported_target AND (target_type STREQUAL "SHARED_LIBRARY" OR target_type STREQUAL "MODULE_LIBRARY" OR target_type STREQUAL "UNKNOWN_LIBRARY"))
            list(APPEND result "${current_target}")
        endif()

        foreach(link_property IN ITEMS LINK_LIBRARIES INTERFACE_LINK_LIBRARIES)
            get_target_property(linked_targets "${current_target}" "${link_property}")
            if(linked_targets AND NOT linked_targets MATCHES "-NOTFOUND$")
                extract_link_targets("${linked_targets}" extracted_targets)
                list(APPEND queue ${extracted_targets})
            endif()
        endforeach()
    endwhile()

    if(result)
        list(REMOVE_DUPLICATES result)
    endif()

    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

collect_shared_dependency_targets(plugin plugin_shared_dependency_targets)

install(TARGETS plugin
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(TARGETS ${EXECUTABLE_NAME}
    RUNTIME DESTINATION bin
)

if(WIN32)
    install(FILES $<TARGET_RUNTIME_DLLS:plugin>
        DESTINATION bin
    )
elseif(UNIX AND NOT APPLE)
    foreach(plugin_shared_dependency_target IN LISTS plugin_shared_dependency_targets)
        install(FILES $<TARGET_FILE:${plugin_shared_dependency_target}>
            DESTINATION lib
            FOLLOW_SYMLINK_CHAIN
        )
    endforeach()
endif()

enable_testing()
add_subdirectory(tests)
