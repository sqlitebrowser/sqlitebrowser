cmake_minimum_required(VERSION 2.8.12.2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets PrintSupport)

set(QCUSTOMPLOT_SRC
	qcustomplot.cpp
)

set(QCUSTOMPLOT_HDR
)

set(QCUSTOMPLOT_MOC_HDR
	qcustomplot.h
)

add_library(qcustomplot ${QCUSTOMPLOT_SRC} ${QCUSTOMPLOT_HDR} ${QCUSTOMPLOT_MOC})

target_link_libraries(qcustomplot Qt5::Widgets Qt5::PrintSupport)
