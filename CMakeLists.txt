cmake_minimum_required(VERSION 3.5)

project(LiLo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Sql)

set(TS_FILES LiLo_de_DE.ts)

set(PROJECT_SOURCES
    about.cpp
    clock.cpp
    databasesupport.cpp
    enterreceiver.cpp
    main.cpp
    mainwindow.cpp
    qsotablemodel.cpp
    qsotablemodelitem.cpp
    singlelinelabel.cpp
    timestampdelegate.cpp
    ${TS_FILES}
)

set(PROJECT_HEADERS
    about.hpp
    clock.hpp
    databasesupport.hpp
    enterreceiver.hpp
    iclocktick.hpp
    mainwindow.hpp
    qsotablemodel.hpp
    qsotablemodelitem.hpp
    singlelinelabel.hpp
    timestampdelegate.hpp
    version.hpp.in)

set(PROJECT_UIS
    about.ui
    clock.ui
    mainwindow.ui)

set(PROJECT_RESOURCES
    LiLo.ico
    lilo.rc
    resources.qrc)

qt_add_executable(LiLo
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_UIS}
    ${PROJECT_RESOURCES}
)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

configure_file(version.hpp.in version.hpp)

target_link_libraries(LiLo PRIVATE Qt6::Widgets Qt6::Sql)

set_target_properties(LiLo PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(LiLo)
