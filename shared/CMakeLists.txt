function(export_headers TARGET HEADER_SOURCE_DIR HEADER_DEST_DIR)

    # Put all headers that are in the source directory into EXPORT_HEADERS variable
    file(GLOB_RECURSE EXPORT_HEADERS CONFIGURE_DEPENDS
        RELATIVE "${HEADER_SOURCE_DIR}"
        "${HEADER_SOURCE_DIR}/*.hpp"
    )

    # For each header that will be exported
    foreach(HEADER ${EXPORT_HEADERS})

        # Get the directory portion that needs to be created
        get_filename_component(HEADER_DIRECTORY "${HEADER}" DIRECTORY)

        # Create the directory
        add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${HEADER_DEST_DIR}/${HEADER_DIRECTORY}"
        )

        if (MSVC)

            # Make a hard link to the file
            add_custom_command(TARGET ${TARGET} POST_BUILD
                COMMAND if not exist "${HEADER_DEST_DIR}/${HEADER}" \( mklink /h "${HEADER_DEST_DIR}/${HEADER}" "${HEADER_SOURCE_DIR}/${HEADER}" \)
            )

        else()

            # Make a symbolic link to the file
            add_custom_command(TARGET ${TARGET} POST_BUILD
                COMMAND ln -sf "${HEADER_SOURCE_DIR}/${HEADER}" "${HEADER_DEST_DIR}/${HEADER}"
            )

        endif()


    endforeach(HEADER)

endfunction()

file(GLOB_RECURSE header_list 
    "include/**/*.hpp"
    "include/*.hpp")

file(GLOB_RECURSE source_list 
    "src/**/*.cpp"
    "src/*.cpp")

add_library(${PROJECT_NAME}-shared SHARED ${header_list} ${source_list})
add_library(${PROJECT_NAME}::Shared ALIAS ${PROJECT_NAME}-shared)

export_headers(${PROJECT_NAME}-shared ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include/shared)
target_include_directories(${PROJECT_NAME}-shared INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include)
target_include_directories(${PROJECT_NAME}-shared PRIVATE include)


install(TARGETS ${PROJECT_NAME}-shared)
