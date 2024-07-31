cmake_minimum_required(VERSION 2.8.12.2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets PrintSupport)

set(SOURCES
    qcustomplot.cpp
    qcustomplot.h
)

add_library(qcustomplot ${SOURCES})
target_include_directories(qcustomplot INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(qcustomplot Qt5::Widgets Qt5::PrintSupport)

add_library(QCustomPlot::QCustomPlot ALIAS qcustomplot)
