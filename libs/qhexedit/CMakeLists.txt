cmake_minimum_required(VERSION 2.6)

find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(QHEXEDIT_SRC
    src/commands.cpp
    src/qhexedit.cpp
    src/qhexedit_p.cpp
    src/xbytearray.cpp
	)

set(QHEXEDIT_HDR
    src/commands.h
    src/qhexedit.h
    src/qhexedit_p.h
    src/xbytearray.h
	)

QT4_WRAP_CPP(QHEXEDIT_HDR_MOC ${QHEXEDIT_HDR})

add_library(qhexedit ${QHEXEDIT_SRC} ${QHEXEDIT_HDR} ${QHEXEDIT_HDR_MOC})

