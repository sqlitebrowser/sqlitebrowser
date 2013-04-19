TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sqlitebrowser

CONFIG += debug_and_release
CONFIG += qt
CONFIG += warn_on

# create a unittest option
CONFIG(unittest) {
  CONFIG += qtestlib
  SOURCES += tests/testsqlobjects.cpp
} else {
  SOURCES += main.cpp
}

HEADERS += \
    sqlitedb.h \
    MainWindow.h \
    SQLiteSyntaxHighlighter.h \
    CreateIndexDialog.h \
    AboutDialog.h \
    EditTableDialog.h \
    PreferencesDialog.h \
    EditDialog.h \
    ExportCsvDialog.h \
    ImportCsvDialog.h \
    sqltextedit.h \
    sqlitetypes.h \
    ExtendedTableWidget.h \
    grammar/Sqlite3Lexer.hpp \
    grammar/Sqlite3Parser.hpp \
    grammar/sqlite3TokenTypes.hpp \
    sqlitetablemodel.h \
    FilterTableHeader.h

SOURCES += \
    sqlitedb.cpp \
    MainWindow.cpp \
    SQLiteSyntaxHighlighter.cpp \
    CreateIndexDialog.cpp \
    EditTableDialog.cpp \
    PreferencesDialog.cpp \
    AboutDialog.cpp \
    EditDialog.cpp \
    ExportCsvDialog.cpp \
    ImportCsvDialog.cpp \
    sqltextedit.cpp \
    sqlitetypes.cpp \
    ExtendedTableWidget.cpp \
    grammar/Sqlite3Lexer.cpp \
    grammar/Sqlite3Parser.cpp \
    sqlitetablemodel.cpp \
    FilterTableHeader.cpp

RESOURCES += icons/icons.qrc

FORMS += \
    MainWindow.ui \
    CreateIndexDialog.ui \
    AboutDialog.ui \
    EditTableDialog.ui \
    PreferencesDialog.ui \
    EditDialog.ui \
    ExportCsvDialog.ui \
    ImportCsvDialog.ui


QMAKE_CXXFLAGS += -DAPP_VERSION=\\\"`cd $$PWD;git log -n1 --format=%h_git`\\\"

unix { 
    LIBS += -ldl
}
win32 {
    RC_FILE = winapp.rc
}
mac { 
    RC_FILE = macapp.icns
    LIBS += -framework \
        Carbon
    QMAKE_INFO_PLIST = app.plist
}

UI_DIR = .ui
INCLUDEPATH += $$PWD/../libs/antlr-2.7.7 $$PWD/../libs/qhexedit
LIBS += -L$$PWD/../libs/qhexedit -lantlr -lqhexedit -lsqlite3
DEPENDPATH += $$PWD/../libs/antlr-2.7.7 $$PWD/../libs/qhexedit
