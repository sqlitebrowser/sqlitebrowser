QT += qt3support
TEMPLATE = app
LANGUAGE = C++
CONFIG += qt \
    warn_on \
    release
HEADERS += extendedmainform.h \
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
    editformwindow.h \
    edittableform.h \
    exporttablecsvform.h \
    findform.h \
    form1.h \
    importcsvform.h \
    renametableform.h \
    sqllogform.h
SOURCES += extendedmainform.cpp \
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
    editformwindow.cpp \
    edittableform.cpp \
    exporttablecsvform.cpp \
    findform.cpp \
    form1.cpp \
    importcsvform.cpp \
    renametableform.cpp \
    sqllogform.cpp

IMAGES = images/create_index.png \
    images/create_table.png \
    images/delete_index.png \
    images/delete_table.png \
    images/icone16.png \
    images/log.png \
    images/modify_table.png \
    images/new.png \
    images/open.png \
    images/revert.png \
    images/save.png \
    images/searchfind.png \
    images/whatis.png
unix { 
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}
win32:RC_FILE = winapp.rc
mac { 
    RC_FILE = macapp.icns
    LIBS += -framework \
        Carbon
    QMAKE_INFO_PLIST = app.plist
}

