cmake_minimum_required(VERSION 2.8.7)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED)

set(QCUSTOMPLOT_SRC
	qcustomplot.cpp
)

set(QCUSTOMPLOT_HDR
)

set(QCUSTOMPLOT_MOC_HDR
	qcustomplot.h
)

add_library(qcustomplot ${QCUSTOMPLOT_SRC} ${QCUSTOMPLOT_HDR} ${QCUSTOMPLOT_MOC})

qt5_use_modules(qcustomplot Widgets PrintSupport)
