cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets PrintSupport)

set(QCUSTOMPLOT_SRC
    qcustomplot.cpp
)

set(QCUSTOMPLOT_MOC_HDR
    qcustomplot.h
)

add_library(qcustomplot ${QCUSTOMPLOT_SRC} ${QCUSTOMPLOT_MOC_HDR} ${QCUSTOMPLOT_MOC})

target_include_directories(qcustomplot INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(qcustomplot Qt5::Widgets Qt5::PrintSupport)

add_library(QCustomPlot::QCustomPlot ALIAS qcustomplot)
