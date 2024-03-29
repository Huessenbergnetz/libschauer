include(CMakeParseArguments)


set(TS_FILES libschauer.ts
             libschauer_de.ts)

function(schauer_add_trans_func _qm_files)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(_LRELEASE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_lrelease_files ${_LRELEASE_UNPARSED_ARGUMENTS})

    foreach(_current_FILE ${_lrelease_files})
        get_filename_component(_abs_FILE ${_current_FILE} ABSOLUTE)
        get_filename_component(qm ${_abs_FILE} NAME)
        # everything before the last dot has to be considered the file name (including other dots)
        string(REGEX REPLACE "\\.[^.]*$" "" FILE_NAME ${qm})
        get_source_file_property(output_location ${_abs_FILE} OUTPUT_LOCATION)
        if(output_location)
            file(MAKE_DIRECTORY "${output_location}")
            set(qm "${output_location}/${FILE_NAME}.qm")
        else()
            set(qm "${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.qm")
        endif()

        add_custom_command(OUTPUT ${qm}
                    COMMAND ${Qt5_LRELEASE_EXECUTABLE}
                    ARGS ${_LRELEASE_OPTIONS} ${_abs_FILE} -qm ${qm}
                    DEPENDS ${_abs_FILE} VERBATIM
                )
        list(APPEND ${_qm_files} ${qm})
    endforeach()
    set(${_qm_files} ${${_qm_files}} PARENT_SCOPE)
endfunction()

function(schauer_add_translation _qm_files)
    if (QT_VERSION VERSION_LESS "5.11.0")
        schauer_add_trans_func("${_qm_files}" ${ARGN})
    else()
        if (QT_DEFAULT_MAJOR_VERSION EQUAL 6)
            qt6_add_translation("${_qm_files}" ${ARGN})
        else()
            qt5_add_translation("${_qm_files}" ${ARGN})
        endif()
    endif()
    set("${_qm_files}" "${${_qm_files}}" PARENT_SCOPE)
endfunction()

schauer_add_translation(QM_FILES ${TS_FILES} OPTIONS -idbased)
add_custom_target(translations ALL
                  DEPENDS ${QM_FILES}
                  COMMENT "Compiles translation messages"
                  SOURCES ${TS_FILES})

install(FILES ${QM_FILES}
        DESTINATION ${LIBSCHAUER_I18NDIR}
        COMPONENT runtime
)
