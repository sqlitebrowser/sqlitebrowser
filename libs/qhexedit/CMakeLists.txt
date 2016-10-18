cmake_minimum_required(VERSION 2.8.7)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED)

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

add_library(qhexedit ${QHEXEDIT_SRC} ${QHEXEDIT_HDR} ${QHEXEDIT_MOC})

qt5_use_modules(qhexedit Widgets)
