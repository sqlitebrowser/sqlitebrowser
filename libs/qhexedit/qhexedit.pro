TEMPLATE = lib

QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += staticlib
CONFIG += debug_and_release

HEADERS += \
    src/qhexedit.h \
    src/chunks.h \
    src/commands.h

SOURCES += \
    src/qhexedit.cpp \
    src/chunks.cpp \
    src/commands.cpp
