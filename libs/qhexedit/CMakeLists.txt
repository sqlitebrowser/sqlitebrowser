cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

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
target_link_libraries(qhexedit Qt5::Widgets)

add_library(QHexEdit::QHexEdit ALIAS qhexedit)
