TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sqlitebrowser

INCLUDEPATH += sqlite_source/
CONFIG += qt \
    warn_on \
    debug

HEADERS += \
    sqlitebrowsertypes.h \
    sqlitedb.h \
    sqlbrowser_util.h \
    sqlite_source/sqlite3.h \
    choosetableform.h \
    createindexform.h \
    createtableform.h \
    deleteindexform.h \
    deletetableform.h \
    editfieldform.h \
    editform.h \
    edittableform.h \
    exporttablecsvform.h \
    findform.h \
    importcsvform.h \
    renametableform.h \
    preferencesform.h \
    mainwindow.h \
    createtabledialog.h \
    SQLLogDock.h \
    sqlitesyntaxhighlighter.h \
    dialogabout.h

SOURCES += \
    browsermain.cpp \
    sqlitedb.cpp \
    sqlbrowser_util.c \
    sqlite_source/sqlite3.c \
    choosetableform.cpp \
    createindexform.cpp \
    createtableform.cpp \
    deleteindexform.cpp \
    deletetableform.cpp \
    editfieldform.cpp \
    editform.cpp \
    edittableform.cpp \
    exporttablecsvform.cpp \
    findform.cpp \
    importcsvform.cpp \
    renametableform.cpp \
    preferencesform.cpp \
    mainwindow.cpp \
    createtabledialog.cpp \
    SQLLogDock.cpp \
    sqlitesyntaxhighlighter.cpp \
    dialogabout.cpp

QMAKE_CXXFLAGS += -DAPP_VERSION=\\\"`cd $$PWD;git log -n1 --format=%h_git`\\\"

unix { 
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
	LIBS += -ldl
}
win32:RC_FILE = winapp.rc
mac { 
    RC_FILE = macapp.icns
    LIBS += -framework \
        Carbon
    QMAKE_INFO_PLIST = app.plist
    CONFIG += x86 \
        ppc
}
RESOURCES += icons/icons.qrc

FORMS += \
    createtabledialog.ui \
    dialogabout.ui \
    preferencesform.ui \
    mainwindow.ui \
    editfieldform.ui \
    edittableform.ui
