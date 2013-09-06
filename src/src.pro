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
  HEADERS += tests/testsqlobjects.h
  SOURCES += tests/testsqlobjects.cpp
} else {
  SOURCES += main.cpp
}

# version reporting
#version.depends = version.sh
#CONFIG(debug,debug|release):version.target = Makefile.Debug
#CONFIG(release,debug|release):version.target = Makefile.Release
#version.commands = sh $$PWD/version.sh $$PWD
#QMAKE_EXTRA_TARGETS += version

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
    FilterTableHeader.h \
    gen_version.h \
    SqlExecutionArea.h \
    VacuumDialog.h

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
    FilterTableHeader.cpp \
    SqlExecutionArea.cpp \
    VacuumDialog.cpp

RESOURCES += icons/icons.qrc

FORMS += \
    MainWindow.ui \
    CreateIndexDialog.ui \
    AboutDialog.ui \
    EditTableDialog.ui \
    PreferencesDialog.ui \
    EditDialog.ui \
    ExportCsvDialog.ui \
    ImportCsvDialog.ui \
    SqlExecutionArea.ui \
    VacuumDialog.ui

LIBPATH_QHEXEDIT=$$PWD/../libs/qhexedit
LIBPATH_ANTLR=$$PWD/../libs/antlr-2.7.7
unix {
    LIBS += -ldl
}
win32 {
    RC_FILE = winapp.rc
    INCLUDEPATH += $$PWD
    CONFIG(debug,debug|release) {
        LIBPATH_QHEXEDIT = $$LIBPATH_QHEXEDIT/debug
        LIBPATH_ANTLR = $$LIBPATH_ANTLR/debug
    }
    CONFIG(release,debug|release) {
        LIBPATH_QHEXEDIT = $$LIBPATH_QHEXEDIT/release
        LIBPATH_ANTLR = $$LIBPATH_ANTLR/release
    }
}
mac {
    RC_FILE = macapp.icns
    INCLUDEPATH += -I/usr/local/include \
                   -I/usr/local/opt/sqlite/include
    LIBS += -L/usr/local/lib \
            -L/usr/local/opt/sqlite/lib \
            -framework Carbon
    QMAKE_INFO_PLIST = app.plist
}

UI_DIR = .ui
INCLUDEPATH += $$PWD/../libs/antlr-2.7.7 $$PWD/../libs/qhexedit
LIBS += -L$$LIBPATH_QHEXEDIT -L$$LIBPATH_ANTLR -lantlr -lqhexedit -lsqlite3
DEPENDPATH += $$PWD/../libs/antlr-2.7.7 $$PWD/../libs/qhexedit
