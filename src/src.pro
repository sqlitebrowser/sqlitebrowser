TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sqlitebrowser

INCLUDEPATH += sqlite_source/
CONFIG += qt \
    warn_on \
    debug

HEADERS += \
    sqlitedb.h \
    sqlbrowser_util.h \
    sqlite_source/sqlite3.h \
    SQLLogDock.h \
    CreateIndexForm.h \
    DialogAbout.h \
    EditFieldForm.h \
    EditForm.h \
    EditTableForm.h \
    ExportTableCSVForm.h \
    FindForm.h \
    ImportCSVForm.h \
    MainWindow.h \
    PreferencesForm.h \
    SQLiteSyntaxHighlighter.h

SOURCES += \
    sqlitedb.cpp \
    sqlbrowser_util.c \
    sqlite_source/sqlite3.c \
    SQLLogDock.cpp \
    main.cpp \
    CreateIndexForm.cpp \
    DialogAbout.cpp \
    EditFieldForm.cpp \
    EditForm.cpp \
    EditTableForm.cpp \
    ExportTableCSVForm.cpp \
    FindForm.cpp \
    ImportCSVForm.cpp \
    MainWindow.cpp \
    PreferencesForm.cpp \
    SQLiteSyntaxHighlighter.cpp

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
    DialogAbout.ui \
    EditFieldForm.ui \
    EditTableForm.ui \
    MainWindow.ui \
    PreferencesForm.ui
