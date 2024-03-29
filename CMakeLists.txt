# SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: LGPL-3.0-or-later

cmake_minimum_required(VERSION 3.12.4 FATAL_ERROR)
cmake_policy(SET CMP0048 NEW)

project(libschauer
        VERSION 0.0.1
        DESCRIPTION "Qt based library to access the Docker HTTP API."
        LANGUAGES CXX)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Network LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} 5.6.0 REQUIRED COMPONENTS Core Network LinguistTools)

# Include our cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Auto generate moc files
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(DOCKER_API_VERSION "1.40")

# Forbid in-tree building
if(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
      message(STATUS "Please do an out-of-tree build:")
      message(STATUS "rm -f CMakeCache.txt && mkdir build && cd build; cmake .. && make")
      message(FATAL_ERROR "In-tree-build detected!")
endif(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})

include(GNUInstallDirs)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "libschauer default install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

option(BUILD_SHARED_LIBS "Enable the build of shared libraries" ON)
option(BUILD_DOCS "Enable the build of doxygen docs" OFF)
option(BUILD_DOCS_QUIET "Tell doxygen to be quiet while building the documentation." OFF)
option(ENABLE_MAINTAINER_FLAGS "Enables some build flags used for development" OFF)
option(WITH_KDE "Use the original KJobs implementation of KDE Frameworks" ON)
option(WITH_TESTS "Build the tests" OFF)
option(WITH_API_TESTS "Build API tests that require a running docker instance" OFF)

set(LIBSCHAUER_I18NDIR "${CMAKE_INSTALL_DATADIR}/libSchauerQt${QT_VERSION_MAJOR}/translations" CACHE PATH "Directory to install translations")

if (WITH_TESTS)
    enable_testing()
endif (WITH_TESTS)

if (WITH_KDE)
    find_package(KF5CoreAddons REQUIRED)
endif (WITH_KDE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/schauer-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SchauerQt${QT_VERSION_MAJOR}Config.cmake
    @ONLY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/schauer-config-version.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SchauerQt${QT_VERSION_MAJOR}ConfigVersion.cmake
    @ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SchauerQt${QT_VERSION_MAJOR}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SchauerQt${QT_VERSION_MAJOR}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SchauerQt${QT_VERSION_MAJOR}/
    COMPONENT development
)

install(EXPORT SchauerTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SchauerQt${QT_VERSION_MAJOR}/
    FILE SchauerQt${QT_VERSION_MAJOR}Targets.cmake
    NAMESPACE SchauerQt${QT_VERSION_MAJOR}::
    COMPONENT development
)

add_subdirectory(Schauer)
add_subdirectory(translations)
if (WITH_TESTS)
    add_subdirectory(tests)
endif (WITH_TESTS)

if (BUILD_DOCS)
    find_package(Doxygen REQUIRED OPTIONAL_COMPONENTS dot)

    if (BUILD_DOCS_QUIET)
        set(DOXYGEN_QUIET "YES")
        set(DOXYGEN_WARNINGS "NO")
        set(DOXYGEN_WARN_IF_UNDOCUMENTED "NO")
        set(DOXYGEN_WARN_IF_DOC_ERROR "NO")
    else(BUILD_DOCS_QUIET)
        set(DOXYGEN_QUIET "NO")
        set(DOXYGEN_WARNINGS "YES")
        set(DOXYGEN_WARN_IF_UNDOCUMENTED "YES")
        set(DOXYGEN_WARN_IF_DOC_ERROR "YES")
    endif(BUILD_DOCS_QUIET)

    set(DOXYGEN_DOT_FONTNAME "Nimbus Sans L")
    set(DOXYGEN_STRIP_FROM_PATH ${CMAKE_SOURCE_DIR})
    set(DOXYGEN_GENERATE_TAGFILE ${CMAKE_CURRENT_BINARY_DIR}/schauer.tags)
    set(DOXYGEN_BUILTIN_STL_SUPPORT "YES")
    set(DOXYGEN_VERBATIM_HEADERS "NO")
    set(DOXYGEN_ALWAYS_DETAILED_SEC "YES")
    set(DOXYGEN_HIDE_UNDOC_MEMBERS "YES")
    set(DOXYGEN_HIDE_UNDOC_CLASSES "YES")
    set(DOXYGEN_LAYOUT_FILE ${CMAKE_SOURCE_DIR}/dox/DoxygenLayout.xml)
    set(DOXYGEN_COLLABORATION_GRAPH "NO")
#    set(DOXYGEN_EXAMPLE_PATH ${CMAKE_SOURCE_DIR}/dox/examples)
    set(DOXYGEN_SORT_BRIEF_DOCS "YES")
    set(DOXYGEN_SORT_MEMBERS_CTORS_1ST "YES")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/README.md)
    list(APPEND DOXYGEN_PREDEFINED "W_DOXYGEN")

    set(DOXYGEN_ALIASES "dockerAPI{1}=\"\\par Docker API Documentation^^<a href=\\\"https://docs.docker.com/engine/api/v${DOCKER_API_VERSION}/#operation/\\1\\\">\\1</a>\"")
    set(DOXYGEN_VERBATIM_VARS DOXYGEN_ALIASES)

    # Available from http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/libstdc++.tag
    set (DOCS_STLTAGFILE "${CMAKE_SOURCE_DIR}/scripts/libstdc++.tag" CACHE FILEPATH "Path to the doxygen tag file of the libstdc++")
    set (DOCS_STLLOCALHTMLDIR "${CMAKE_SOURCE_DIR}/scripts/libstdc++" CACHE PATH "Path to the directory containing the libstdc++ HTML doxygen documentation")

    if (EXISTS DOCS_STLTAGFILE)
        list(APPEND ONLINETAGS "${DOCS_STLTAGFILE}=http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/")
        if (IS_DIRECTORY "${DOCS_STLLOCALHTMLDIR}")
            list(APPEND LOCALHTMLTAGS "${DOCS_STLTAGFILE}=${DOCS_STLLOCALHTMLDIR}/")
        endif (IS_DIRECTORY "${DOCS_STLLOCALHTMLDIR}")
        list(APPEND QCHTAGS "${DOCS_STLTAGFILE}=qthelp://org.gnu.libstdcxx/libstdc++/")
    else (EXISTS DOCS_STLTAGFILE)
        set(DOXYGEN_BUILTIN_STL_SUPPORT "YES")
    endif (EXISTS DOCS_STLTAGFILE)

    find_program(QMAKE_EXE_PATH NAMES qmake-qt${QT_VERSION_MAJOR} qmake${QT_VERSION_MAJOR} qmake)
    if(QMAKE_EXE_PATH)
        execute_process(COMMAND ${QMAKE_EXE_PATH} -query QT_INSTALL_DOCS OUTPUT_VARIABLE QT_INSTALL_DOCS OUTPUT_STRIP_TRAILING_WHITESPACE)
        if (NOT IS_DIRECTORY "${QT_INSTALL_DOCS}")
            set(QT_INSTALL_DOCS "/usr/share/qt${QT_VERSION_MAJOR}/doc")
        endif (NOT IS_DIRECTORY "${QT_INSTALL_DOCS}")
    else(QMAKE_EXE_PATH)
        set(QT_INSTALL_DOCS "/usr/share/qt${QT_VERSION_MAJOR}/doc")
    endif(QMAKE_EXE_PATH)

    set(QT_ONLINE_DOCS_URL "https://doc.qt.io/qt-${QT_VERSION_MAJOR}/")
    set(QT_DOCS_COMPONENTS "qtcore" "qtnetwork")

    set(DOCS_QTDOCSDIR ${QT_INSTALL_DOCS} CACHE PATH "Path to a directory containing the Qt Documentation")

    foreach(_QTDOCCOMP ${QT_DOCS_COMPONENTS})
        find_file(_QTDOCCOMPTAGS "${_QTDOCCOMP}.tags" PATHS "${DOCS_QTDOCSDIR}" PATH_SUFFIXES "${_QTDOCCOMP}" NO_DEFAULT_PATH)
        if (_QTDOCCOMPTAGS)
            list(APPEND ONLINETAGS "${_QTDOCCOMPTAGS}=${QT_ONLINE_DOCS_URL}")
            if (IS_DIRECTORY "${DOCS_QTDOCSDIR}/${_QTDOCCOMP}")
                list(APPEND LOCALHTMLTAGS "${_QTDOCCOMPTAGS}=${DOCS_QTDOCSDIR}/${_QTDOCCOMP}/")
            endif (IS_DIRECTORY "${DOCS_QTDOCSDIR}/${_QTDOCCOMP}")
            if (EXISTS "${DOCS_QTDOCSDIR}/${_QTDOCCOMP}.qch")
                list(APPEND QCHTAGS "${_QTDOCCOMPTAGS}=qthelp://org.qt-project.${_QTDOCCOMP}.${QT_VERSION_MAJOR}${QT_VERSION_MINOR}${QT_VERSION_PATCH}/${_QTDOCCOMP}/")
            endif (EXISTS "${DOCS_QTDOCSDIR}/${_QTDOCCOMP}.qch")
        endif (_QTDOCCOMPTAGS)
        unset(_QTDOCCOMPTAGS CACHE)
    endforeach(_QTDOCCOMP ${QT_DOCS_COMPONENTS})

    set(DOXYGEN_HTML_TIMESTAMP "YES")
    set(DOXYGEN_TAGFILES ${ONLINETAGS})
    set(DOXYGEN_HTML_OUTPUT "webdocs")
    set(DOXYGEN_HTML_FOOTER "${CMAKE_SOURCE_DIR}/dox/webFooter.html")
    doxygen_add_docs(webdocs
        ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generate HTML documentation for online usage"
    )
    message(STATUS "Adding make target webdocs")

    set(DOXYGEN_HTML_TIMESTAMP "NO")
    set(DOXYGEN_TAGFILES ${LOCALHTMLTAGS})
    set(DOXYGEN_HTML_OUTPUT "htmldocs")
    set(DOXYGEN_HTML_FOOTER "")
    doxygen_add_docs(htmldocs
        ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generate HTML documentation for local usage"
    )
    message(STATUS "Adding make target htmldocs")

    find_program(DOXYGEN_QHG_LOCATION NAMES qhelpgenerator-qt5 qhelpgenerator5 qhelpgenerator)
    if(DOXYGEN_QHG_LOCATION)
        message(STATUS "qhelpgenerator found. QCH docs building available.")
        set(DOXYGEN_TAGFILES ${QCHTAGS})
        set(DOXYGEN_HTML_OUTPUT "qtdocs")
        set(DOXYGEN_HTML_FOOTER "")
        set(DOXYGEN_GENERATE_QHP "YES")
        set(DOXYGEN_QHP_NAMESPACE "de.huessenbergnetz.schauer.${PROJECT_VERSION_MAJOR}${PROJECT_VERSION_MINOR}${PROJECT_VERSION_PATCH}")
        set(DOXYGEN_QHP_VIRTUAL_FOLDER "libschauer")
        set(DOXYGEN_QCH_FILE "${CMAKE_CURRENT_BINARY_DIR}/schauer.qch")
        set(DOXYGEN_SEARCHENGINE "NO")
        doxygen_add_docs(qtdocs
            ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generate Qt QCH documentation for local usage"
        )
        message(STATUS "Adding make target qtdocs")

        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/schauer.qch
            DESTINATION ${DOCS_QTDOCSDIR}
            COMPONENT docs
            OPTIONAL
        )
    endif(DOXYGEN_QHG_LOCATION)

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/htmldocs/
        DESTINATION ${DOCS_QTDOCSDIR}/schauer
        COMPONENT docs
        OPTIONAL
    )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/schauer.tags
        DESTINATION ${DOCS_QTDOCSDIR}/schauer
        COMPONENT development
        OPTIONAL
    )
endif (BUILD_DOCS)
