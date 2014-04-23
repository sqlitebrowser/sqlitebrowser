cmake_minimum_required(VERSION 2.8.9)

if(USE_QT5)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    find_package(Qt5Widgets REQUIRED)
else()
    find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS})
endif()

set(QCUSTOMPLOT_SRC
	qcustomplot.cpp
)

set(QCUSTOMPLOT_HDR
)

set(QCUSTOMPLOT_MOC_HDR
	qcustomplot.h
)

if(NOT USE_QT5)
    QT4_WRAP_CPP(QCUSTOMPLOT_MOC ${QCUSTOMPLOT_MOC_HDR})
endif()

add_library(qcustomplot ${QCUSTOMPLOT_SRC} ${QCUSTOMPLOT_HDR} ${QCUSTOMPLOT_MOC})

if(USE_QT5)
	qt5_use_modules(qcustomplot Widgets PrintSupport)
endif()

