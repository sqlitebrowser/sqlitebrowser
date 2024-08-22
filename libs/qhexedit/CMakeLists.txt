cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_MAJOR Qt5 CACHE STRING "Major QT version")

find_package(${QT_MAJOR} REQUIRED COMPONENTS Widgets)

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

add_library(qhexedit ${QHEXEDIT_SRC} ${QHEXEDIT_HDR} ${QHEXEDIT_MOC_HDR} ${QHEXEDIT_MOC})
target_include_directories(qhexedit INTERFACE src)
target_link_libraries(qhexedit ${QT_MAJOR}::Widgets)

add_library(QHexEdit::QHexEdit ALIAS qhexedit)
