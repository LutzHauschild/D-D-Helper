cmake_minimum_required(VERSION 3.16)

# Finde die Qt-Komponenten
find_package(Qt6 COMPONENTS Test Widgets REQUIRED)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.15 COMPONENTS Test Widgets REQUIRED)
endif()

# Setze die Compiler-Flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Definiere die gemeinsamen Quellen
set(COMMON_SOURCES
    ../src/character.cpp
    ../src/initiativetracker.cpp
)

# Definiere die Test-Quellen
set(TEST_SOURCES
    tst_character.cpp
    tst_initiativetracker.cpp
)

# Erstelle die Test-Executables
if (Qt6_FOUND)
    foreach(TEST_SOURCE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        qt_add_executable(${TEST_NAME} ${TEST_SOURCE} ${COMMON_SOURCES})
        target_link_libraries(${TEST_NAME} PRIVATE Qt6::Test Qt6::Widgets)
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
else()
    foreach(TEST_SOURCE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        add_executable(${TEST_NAME} ${TEST_SOURCE} ${COMMON_SOURCES})
        target_link_libraries(${TEST_NAME} PRIVATE Qt5::Test Qt5::Widgets)
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
endif() 