cmake_minimum_required(VERSION 2.8.12.2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/qhexedit.cpp
    src/chunks.cpp
    src/commands.cpp
    src/qhexedit.h
    src/chunks.h
    src/commands.h
)

add_library(qhexedit ${SOURCES})
target_include_directories(qhexedit INTERFACE src)
target_link_libraries(qhexedit Qt5::Widgets)

add_library(QHexEdit::QHexEdit ALIAS qhexedit)
