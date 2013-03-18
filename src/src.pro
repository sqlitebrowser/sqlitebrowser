TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sqlitebrowser

CONFIG += qt \
    warn_on

HEADERS += \
    sqlitedb.h \
    sqlbrowser_util.h \
    MainWindow.h \
    SQLiteSyntaxHighlighter.h \
    CreateIndexDialog.h \
    AboutDialog.h \
    EditTableDialog.h \
    PreferencesDialog.h \
    FindDialog.h \
    EditDialog.h \
    ExportCsvDialog.h \
    ImportCsvDialog.h \
    sqltextedit.h \
    sqlitetypes.h \
    ExtendedTableWidget.h \
    grammar/Sqlite3Lexer.hpp \
    grammar/Sqlite3Parser.hpp \
    grammar/sqlite3TokenTypes.hpp

SOURCES += \
    sqlitedb.cpp \
    sqlbrowser_util.c \
    MainWindow.cpp \
    SQLiteSyntaxHighlighter.cpp \
    CreateIndexDialog.cpp \
    EditTableDialog.cpp \
    PreferencesDialog.cpp \
    AboutDialog.cpp \
    FindDialog.cpp \
    EditDialog.cpp \
    ExportCsvDialog.cpp \
    ImportCsvDialog.cpp \
    sqltextedit.cpp \
    sqlitetypes.cpp \
    ExtendedTableWidget.cpp \
    grammar/Sqlite3Lexer.cpp \
    grammar/Sqlite3Parser.cpp

# create a unittest option
CONFIG(unittest) {
  CONFIG += qtestlib
  SOURCES += tests/testsqlobjects.cpp
} else {
  SOURCES += main.cpp
}

QMAKE_CXXFLAGS += -DAPP_VERSION=\\\"`cd $$PWD;git log -n1 --format=%h_git`\\\"

unix { 
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj $$PWD/../libs/qhexedit
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
    EditTableDialog.ui \
    PreferencesDialog.ui \
    FindDialog.ui \
    EditDialog.ui \
    ExportCsvDialog.ui \
    ImportCsvDialog.ui

INCLUDEPATH += $$PWD/../libs/antlr-2.7.7 $$PWD/../libs/qhexedit
LIBS += -L$$OUT_PWD/../libs/antlr-2.7.7/ -lantlr -L$$OUT_PWD/../libs/qhexedit/ -lqhexedit
DEPENDPATH += $$PWD/../libs/antlr-2.7.7 $$PWD/../libs/qhexedit
