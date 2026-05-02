cmake_minimum_required(VERSION 3.16)

project(Kalkulators VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(
    PROJECT_SOURCES
    main.cpp
    mainWindow.cpp
    mainWindow.h
    kalkulators.ui
)

qt_add_executable(Kalkulators ${PROJECT_SOURCES})

target_link_libraries(
    Kalkulators PRIVATE
    Qt6::Widgets
)
