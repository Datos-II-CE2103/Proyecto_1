cmake_minimum_required(VERSION 3.16)

project(musicPlayerServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencias adicionales
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Core)
find_package(RapidJSON CONFIG REQUIRED)
find_package(glog 0.7.0 REQUIRED)
find_package(gflags REQUIRED)

qt_standard_project_setup()

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        estructuras_datos/node.h
        estructuras_datos/linked_list.h
        estructuras_datos/inicializador.h
        tcp/tcpserver.h
        tcp/tcpserver.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(musicPlayerServer
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(musicPlayerServer SHARED
                ${PROJECT_SOURCES}
        )
    else()
        add_executable(musicPlayerServer
                ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(musicPlayerServer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(musicPlayerServer PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(musicPlayerServer PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(musicPlayerServer PRIVATE glog::glog)
target_link_libraries(musicPlayerServer PRIVATE gflags::gflags)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.musicPlayerServer)
endif()
set_target_properties(musicPlayerServer PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS musicPlayerServer
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(musicPlayerServer)
endif()
