TEMPLATE = lib

QT += core gui widgets

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
