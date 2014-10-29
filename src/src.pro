TEMPLATE = app

QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = sqlitebrowser

CONFIG += debug_and_release
CONFIG += qt
CONFIG += warn_on

# create a unittest option
CONFIG(unittest) {
  CONFIG += qtestlib
  HEADERS += tests/testsqlobjects.h tests/TestImport.h
  SOURCES += tests/testsqlobjects.cpp tests/TestImport.cpp tests/TestMain.cpp
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
    csvparser.h \
    ExtendedTableWidget.h \
    grammar/Sqlite3Lexer.hpp \
    grammar/Sqlite3Parser.hpp \
    grammar/sqlite3TokenTypes.hpp \
    sqlitetablemodel.h \
    FilterTableHeader.h \
    gen_version.h \
    SqlExecutionArea.h \
    VacuumDialog.h \
    DbStructureModel.h \
    Application.h

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
    csvparser.cpp \
    ExtendedTableWidget.cpp \
    grammar/Sqlite3Lexer.cpp \
    grammar/Sqlite3Parser.cpp \
    sqlitetablemodel.cpp \
    FilterTableHeader.cpp \
    SqlExecutionArea.cpp \
    VacuumDialog.cpp \
    DbStructureModel.cpp \
    Application.cpp

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

TRANSLATIONS += \
    translations/sqlb_cn.ts \
    translations/sqlb_de.ts \
    translations/sqlb_fr.ts \
    translations/sqlb_ru.ts

LIBPATH_QHEXEDIT=$$PWD/../libs/qhexedit
LIBPATH_ANTLR=$$PWD/../libs/antlr-2.7.7
LIBPATH_QCUSTOMPLOT=$$PWD/../libs/qcustomplot-source
unix {
    LIBS += -ldl
}
win32 {
    RC_FILE = winapp.rc
    INCLUDEPATH += $$PWD
    CONFIG(debug,debug|release) {
        LIBPATH_QHEXEDIT = $$LIBPATH_QHEXEDIT/debug
        LIBPATH_ANTLR = $$LIBPATH_ANTLR/debug
        LIBPATH_QCUSTOMPLOT = $$LIBPATH_QCUSTOMPLOT/debug
    }
    CONFIG(release,debug|release) {
        LIBPATH_QHEXEDIT = $$LIBPATH_QHEXEDIT/release
        LIBPATH_ANTLR = $$LIBPATH_ANTLR/release
        LIBPATH_QCUSTOMPLOT = $$LIBPATH_QCUSTOMPLOT/release
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
    QMAKE_CXXFLAGS += -DCHECKNEWVERSION
}

UI_DIR = .ui
INCLUDEPATH += $$PWD/../libs/antlr-2.7.7 $$PWD/../libs/qhexedit $$PWD/../libs/qcustomplot-source $$PWD/..
LIBS += -L$$LIBPATH_QHEXEDIT -L$$LIBPATH_ANTLR -L$$LIBPATH_QCUSTOMPLOT -lantlr -lqhexedit -lqcustomplot -lsqlite3
DEPENDPATH += $$PWD/../libs/antlr-2.7.7 $$PWD/../libs/qhexedit $$PWD/../libs/qcustomplot-source

# Rules for creating/updating {ts|qm}-files
include(i18n.pri)
