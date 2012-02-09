TEMPLATE = app
LANGUAGE = C++
INCLUDEPATH += sqlite_source/
CONFIG += qt \
    warn_on
HEADERS += \
    sqlitebrowsertypes.h \
    sqlitedb.h \
    sqlbrowser_util.h \
    sqlite_source/sqlite3.h \
    aboutform.h \
    addfieldform.h \
    addfieldtypeform.h \
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
    SQLLogDock.h
SOURCES += \
    browsermain.cpp \
    sqlitedb.cpp \
    sqlbrowser_util.c \
    sqlite_source/sqlite3.c \
    aboutform.cpp \
    addfieldform.cpp \
    addfieldtypeform.cpp \
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
    SQLLogDock.cpp
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
    createtabledialog.ui
