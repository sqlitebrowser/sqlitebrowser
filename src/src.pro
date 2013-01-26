TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sqlitebrowser

#INCLUDEPATH += sqlite_source/
CONFIG += qt \
    warn_on \
    debug

HEADERS += \
    sqlitedb.h \
    sqlbrowser_util.h \
    MainWindow.h \
    SQLiteSyntaxHighlighter.h \
    CreateIndexDialog.h \
    EditFieldDialog.h \
    AboutDialog.h \
    EditTableDialog.h \
    PreferencesDialog.h \
    FindDialog.h \
    EditDialog.h \
    ExportCsvDialog.h \
    ImportCsvDialog.h

SOURCES += \
    sqlitedb.cpp \
    sqlbrowser_util.c \
    main.cpp \
    MainWindow.cpp \
    SQLiteSyntaxHighlighter.cpp \
    CreateIndexDialog.cpp \
    EditFieldDialog.cpp \
    EditTableDialog.cpp \
    PreferencesDialog.cpp \
    AboutDialog.cpp \
    FindDialog.cpp \
    EditDialog.cpp \
    ExportCsvDialog.cpp \
    ImportCsvDialog.cpp

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
LIBS += -lsqlite3
RESOURCES += icons/icons.qrc

FORMS += \
    MainWindow.ui \
    CreateIndexDialog.ui \
    AboutDialog.ui \
    EditFieldDialog.ui \
    EditTableDialog.ui \
    PreferencesDialog.ui \
    FindDialog.ui \
    EditDialog.ui \
    ExportCsvDialog.ui \
    ImportCsvDialog.ui
