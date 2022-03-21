# SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: LGPL-3.0-or-later

add_library(SchauerQt${QT_VERSION_MAJOR})

if (NOT WITH_KDE)
    target_sources(SchauerQt${QT_VERSION_MAJOR}
        PRIVATE
            sjob.cpp
            sjob.h
            sjob_p.h
    )
    set_property(TARGET SchauerQt${QT_VERSION_MAJOR}
        APPEND PROPERTY PUBLIC_HEADER sjob.h
    )
endif (NOT WITH_KDE)

target_sources(SchauerQt${QT_VERSION_MAJOR}
    PRIVATE
        abstractbasemodel.cpp
        abstractbasemodel.h
        abstractbasemodel_p.h
        abstractconfiguration.cpp
        abstractconfiguration.h
        abstractcontainermodel.cpp
        abstractcontainermodel.h
        abstractcontainermodel_p.h
        abstractimagemodel.cpp
        abstractimagemodel.h
        abstractimagemodel_p.h
        abstractnamfactory.cpp
        abstractnamfactory.h
        abstractversionmodel.cpp
        abstractversionmodel.h
        abstractversionmodel_p.h
        containerlistmodel.cpp
        containerlistmodel.h
        containerlistmodel_p.h
        createcontainerjob.cpp
        createcontainerjob.h
        createcontainerjob_p.h
        getversionjob.cpp
        getversionjob.h
        getversionjob_p.h
        imagelistmodel.cpp
        global.cpp
        global.h
        imagelistmodel.h
        imagelistmodel_p.h
        job.cpp
        job.h
        job_p.h
        listcontainersjob.cpp
        listcontainersjob.h
        listcontainersjob_p.h
        listimagesjob.cpp
        listimagesjob.h
        listimagesjob_p.h
        logging.h
        schauer_exports.h
        startcontainerjob.cpp
        startcontainerjob.h
        startcontainerjob_p.h
        stopcontainerjob.cpp
        stopcontainerjob.h
        stopcontainerjob_p.h
        removecontainerjob.cpp
        removecontainerjob.h
        removecontainerjob_p.h
        versionlistmodel.cpp
        versionlistmodel.h
        versionlistmodel_p.h
)

set_property(TARGET SchauerQt${QT_VERSION_MAJOR}
    APPEND PROPERTY PUBLIC_HEADER
        abstractbasemodel.h
        abstractcontainermodel.h
        abstractconfiguration.h
        AbstractConfiguration
        abstractimagemodel.h
        abstractnamfactory.h
        AbstractNamFactory
        abstractversionmodel.h
        containerlistmodel.h
        ContainerListModel
        createcontainerjob.h
        CreateContainerJob
        getversionjob.h
        GetVersionJob
        global.h
        Global
        imagelistmodel.h
        ImageListModel
        job.h
        listcontainersjob.h
        ListContainersJob
        listimagesjob.h
        ListImagesJob
        startcontainerjob.h
        StartContainerJob
        stopcontainerjob.h
        StopContainerJob
        removecontainerjob.h
        RemoveContainerJob
        schauer_exports.h
        versionlistmodel.h
        VersionListModel
)

add_library(SchauerQt${QT_VERSION_MAJOR}::Core ALIAS SchauerQt${QT_VERSION_MAJOR})

if(CMAKE_VERSION GREATER_EQUAL "3.16.0")
    target_precompile_headers(SchauerQt${QT_VERSION_MAJOR}
        PRIVATE
            <QString>
            <QStringList>
            <QTimer>
            <QJsonDocument>
            <QJsonObject>
            <QJsonArray>
            <QJsonValue>
            <QVariant>
            <vector>
    )
endif(CMAKE_VERSION GREATER_EQUAL "3.16.0")

target_compile_features(SchauerQt${QT_VERSION_MAJOR}
    PUBLIC cxx_std_14
    PRIVATE cxx_std_17
)

target_link_libraries(SchauerQt${QT_VERSION_MAJOR}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Network
)

target_compile_definitions(SchauerQt${QT_VERSION_MAJOR}
    PRIVATE
        QT_NO_KEYWORDS
        QT_NO_CAST_TO_ASCII
        QT_NO_CAST_FROM_ASCII
        QT_STRICT_ITERATORS
        QT_NO_URL_CAST_FROM_STRING
        QT_NO_CAST_FROM_BYTEARRAY
        QT_USE_QSTRINGBUILDER
        QT_NO_SIGNALS_SLOTS_KEYWORDS
        QT_USE_FAST_OPERATOR_PLUS
        QT_DISABLE_DEPRECATED_BEFORE=0x050500
        SCHAUER_VERSION="${PROJECT_VERSION}"
        DOCKER_API_VERSION="${DOCKER_API_VERSION}"
)

if (WITH_KDE)
    message(STATUS "KDE support enabled")
    target_compile_definitions(SchauerQt${QT_VERSION_MAJOR}
        PUBLIC
            SCHAUER_WITH_KDE
    )
    target_link_libraries(SchauerQt${QT_VERSION_MAJOR}
        PUBLIC
            KF5::CoreAddons
    )
endif (WITH_KDE)

if(ENABLE_MAINTAINER_FLAGS)
    target_compile_definitions(SchauerQt${QT_VERSION_MAJOR}
        PRIVATE
            QT_DEPRECATED_WARNINGS
    )
    target_compile_options(SchauerQt${QT_VERSION_MAJOR}
        PRIVATE
            -Wall
            -Wcast-align
            -Wno-uninitialized
            -Wempty-body
            -Wformat-security
            -Wformat
            -Winit-self
    )
endif(ENABLE_MAINTAINER_FLAGS)

get_property(_current_parent_dir DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY PARENT_DIRECTORY)

target_include_directories(SchauerQt${QT_VERSION_MAJOR} PUBLIC
    $<BUILD_INTERFACE:${_current_parent_dir}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/schauer-qt${QT_VERSION_MAJOR}>
)

set_target_properties(SchauerQt${QT_VERSION_MAJOR} PROPERTIES
    EXPORT_NAME Core
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(TARGETS SchauerQt${QT_VERSION_MAJOR}
    EXPORT SchauerTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/schauer-qt${QT_VERSION_MAJOR}/Schauer COMPONENT development
)
