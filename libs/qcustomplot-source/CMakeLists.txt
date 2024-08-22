cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_MAJOR Qt5 CACHE STRING "Major QT version")

find_package(${QT_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

set(QCUSTOMPLOT_SRC
    qcustomplot.cpp
)

set(QCUSTOMPLOT_MOC_HDR
    qcustomplot.h
)

add_library(qcustomplot ${QCUSTOMPLOT_SRC} ${QCUSTOMPLOT_MOC_HDR} ${QCUSTOMPLOT_MOC})

target_include_directories(qcustomplot INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(qcustomplot ${QT_MAJOR}::Widgets ${QT_MAJOR}::PrintSupport)

add_library(QCustomPlot::QCustomPlot ALIAS qcustomplot)
