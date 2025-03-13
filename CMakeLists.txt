cmake_minimum_required(VERSION 3.16)
project(DnDInitiativeTracker VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Aktiviere Tests
enable_testing()

find_package(Qt6 COMPONENTS Widgets REQUIRED)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.15 COMPONENTS Widgets REQUIRED)
endif()

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/character.cpp
    src/character.h
    src/initiativetracker.cpp
    src/initiativetracker.h
    src/mainwindow.ui
)

if (Qt6_FOUND)
    qt_add_executable(DnDInitiativeTracker ${PROJECT_SOURCES})
    target_link_libraries(DnDInitiativeTracker PRIVATE Qt6::Widgets)
else()
    add_executable(DnDInitiativeTracker ${PROJECT_SOURCES})
    target_link_libraries(DnDInitiativeTracker PRIVATE Qt5::Widgets)
endif()

# Füge das Tests-Verzeichnis hinzu
add_subdirectory(tests) 