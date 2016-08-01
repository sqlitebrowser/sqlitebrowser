cmake_minimum_required(VERSION 2.8.7)

if(USE_QT5)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    find_package(Qt5Widgets REQUIRED)
else()
    find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
    include("${QT_USE_FILE}")
    add_definitions(${QT_DEFINITIONS})
endif()

set(QHEXEDIT_SRC
    src/qhexedit.cpp
    src/chunks.cpp
    src/commands.cpp
)

set(QHEXEDIT_HDR
    src/chunks.h
    src/commands.h
)

set(QHEXEDIT_MOC_HDR
	src/qhexedit.h
	src/commands.h
)

if(NOT USE_QT5)
    QT4_WRAP_CPP(QHEXEDIT_MOC ${QHEXEDIT_MOC_HDR})
endif()

add_library(qhexedit ${QHEXEDIT_SRC} ${QHEXEDIT_HDR} ${QHEXEDIT_MOC})

if(USE_QT5)
    qt5_use_modules(qhexedit Widgets)
endif()

