TEMPLATE = app

QT += core gui network widgets printsupport concurrent xml
macx: QT += opengl

TARGET = sqlitebrowser

CONFIG += debug_and_release
CONFIG += qt
CONFIG += warn_on

QMAKE_CXXFLAGS += -std=c++11

# create a unittest option
CONFIG(unittest) {
  QT += testlib

  HEADERS += tests/testsqlobjects.h tests/TestImport.h tests/TestRegex.h tests/TestRowCache.h
  SOURCES += tests/testsqlobjects.cpp tests/TestImport.cpp tests/TestRegex.cpp tests/TestRowCache.cpp
} else {
  SOURCES += main.cpp
}

HEADERS += \
    sqlitedb.h \
    MainWindow.h \
    EditIndexDialog.h \
    AboutDialog.h \
    EditTableDialog.h \
    AddRecordDialog.h \
    Settings.h \
    PreferencesDialog.h \
    EditDialog.h \
    ExportDataDialog.h \
    ImportCsvDialog.h \
    sqltextedit.h \
    sqlitetypes.h \
    csvparser.h \
    ExtendedTableWidget.h \
    grammar/Sqlite3Lexer.hpp \
    grammar/Sqlite3Parser.hpp \
    grammar/sqlite3TokenTypes.hpp \
    sqlitetablemodel.h \
    RowCache.h \
    RowLoader.h \
    FilterTableHeader.h \
    version.h \
    SqlExecutionArea.h \
    VacuumDialog.h \
    DbStructureModel.h \
    Application.h \
    sqlite.h \
    CipherDialog.h \
    ExportSqlDialog.h \
    SqlUiLexer.h \
    FileDialog.h \
    ColumnDisplayFormatDialog.h \
    FilterLineEdit.h \
    RemoteDatabase.h \
    ForeignKeyEditorDelegate.h \
    PlotDock.h \
    RemoteDock.h \
    RemoteModel.h \
    RemotePushDialog.h \
    docktextedit.h \
    FindReplaceDialog.h \
    ExtendedScintilla.h \
    FileExtensionManager.h \
    Data.h \
    CipherSettings.h \
    DotenvFormat.h \
    Palette.h \
    CondFormat.h

SOURCES += \
    sqlitedb.cpp \
    MainWindow.cpp \
    EditIndexDialog.cpp \
    EditTableDialog.cpp \
    AddRecordDialog.cpp \
    Settings.cpp \
    PreferencesDialog.cpp \
    AboutDialog.cpp \
    EditDialog.cpp \
    ExportDataDialog.cpp \
    ImportCsvDialog.cpp \
    sqltextedit.cpp \
    sqlitetypes.cpp \
    csvparser.cpp \
    ExtendedTableWidget.cpp \
    grammar/Sqlite3Lexer.cpp \
    grammar/Sqlite3Parser.cpp \
    sqlitetablemodel.cpp \
    RowLoader.cpp \
    FilterTableHeader.cpp \
    SqlExecutionArea.cpp \
    VacuumDialog.cpp \
    DbStructureModel.cpp \
    Application.cpp \
    CipherDialog.cpp \
    ExportSqlDialog.cpp \
    SqlUiLexer.cpp \
    FileDialog.cpp \
    ColumnDisplayFormatDialog.cpp \
    FilterLineEdit.cpp \
    RemoteDatabase.cpp \
    ForeignKeyEditorDelegate.cpp \
    PlotDock.cpp \
    RemoteDock.cpp \
    RemoteModel.cpp \
    RemotePushDialog.cpp \
    docktextedit.cpp \
    FindReplaceDialog.cpp \
    ExtendedScintilla.cpp \
    FileExtensionManager.cpp \
    Data.cpp \
    CipherSettings.cpp \
    DotenvFormat.cpp \
    Palette.cpp \
    CondFormat.cpp

RESOURCES += icons/icons.qrc \
             translations/flags/flags.qrc \
             translations/translations.qrc \
             certs/CaCerts.qrc

FORMS += \
    MainWindow.ui \
    EditIndexDialog.ui \
    AboutDialog.ui \
    EditTableDialog.ui \
    AddRecordDialog.ui \
    PreferencesDialog.ui \
    EditDialog.ui \
    ExportDataDialog.ui \
    ImportCsvDialog.ui \
    SqlExecutionArea.ui \
    VacuumDialog.ui \
    CipherDialog.ui \
    ExportSqlDialog.ui \
    ColumnDisplayFormatDialog.ui \
    PlotDock.ui \
    RemoteDock.ui \
    RemotePushDialog.ui \
    FindReplaceDialog.ui \
    FileExtensionManager.ui

TRANSLATIONS += \
    translations/sqlb_ar_SA.ts \
    translations/sqlb_cs.ts \
    translations/sqlb_zh.ts \
    translations/sqlb_zh_TW.ts \
    translations/sqlb_de.ts \
    translations/sqlb_es_ES.ts \
    translations/sqlb_fr.ts \
    translations/sqlb_ru.ts \
    translations/sqlb_pl.ts \
    translations/sqlb_pt_BR.ts \
    translations/sqlb_en_GB.ts \
    translations/sqlb_ko_KR.ts \
    translations/sqlb_tr.ts \
    translations/sqlb_uk_UA.ts

# SQLite / SQLCipher switch pieces
CONFIG(sqlcipher) {
    QMAKE_CXXFLAGS += -DENABLE_SQLCIPHER
    LIBS += -lsqlcipher

    # Add the paths for Homebrew installed SQLCipher
    mac {
        INCLUDEPATH += /usr/local/opt/sqlcipher/include
        LIBS += -L/usr/local/opt/sqlcipher/lib
    }
} else {
    LIBS += -lsqlite3

    # Add the paths for Homebrew installed SQLite
    mac {
        INCLUDEPATH += /usr/local/opt/sqlite/include
        LIBS += -L/usr/local/opt/sqlite/lib
    }
}

LIBPATH_QHEXEDIT=$$OUT_PWD/../libs/qhexedit
LIBPATH_ANTLR=$$OUT_PWD/../libs/antlr-2.7.7
LIBPATH_QCUSTOMPLOT=$$OUT_PWD/../libs/qcustomplot-source
LIBPATH_QSCINTILLA=$$OUT_PWD/../libs/qscintilla/Qt4Qt5
unix {
    LIBS += -ldl
}
os2 {
    RC_FILE = os2app.rc
}
win32 {
    TARGET = "DB Browser for SQLite"
    RC_FILE = winapp.rc
    INCLUDEPATH += $$PWD
    CONFIG(debug,debug|release) {
        LIBPATH_QHEXEDIT = $$LIBPATH_QHEXEDIT/debug
        LIBPATH_ANTLR = $$LIBPATH_ANTLR/debug
        LIBPATH_QCUSTOMPLOT = $$LIBPATH_QCUSTOMPLOT/debug
    LIBPATH_QSCINTILLA = $$LIBPATH_QSCINTILLA/debug
    }
    CONFIG(release,debug|release) {
        LIBPATH_QHEXEDIT = $$LIBPATH_QHEXEDIT/release
        LIBPATH_ANTLR = $$LIBPATH_ANTLR/release
        LIBPATH_QCUSTOMPLOT = $$LIBPATH_QCUSTOMPLOT/release
    LIBPATH_QSCINTILLA = $$LIBPATH_QSCINTILLA/release
    }
    QMAKE_CXXFLAGS += -DCHECKNEWVERSION

    # Added SQLite installation path variables, matching our setup guide
    LIBS += -L$$PWD/../../../dev/SQLite/ -lsqlite3
    INCLUDEPATH += $$PWD/../../../dev/SQLite
    DEPENDPATH += $$PWD/../../../dev/SQLite
}
mac {
    TARGET = "DB Browser for SQLite"
    RC_FILE = macapp.icns
    QT+= macextras
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -framework Carbon
    QMAKE_INFO_PLIST = app.plist
    QMAKE_CXXFLAGS += -DCHECKNEWVERSION
}

UI_DIR = .ui
INCLUDEPATH += $$PWD/../libs/antlr-2.7.7 $$PWD/../libs/qhexedit $$PWD/../libs/qcustomplot-source $$PWD/../libs/qscintilla/Qt4Qt5 $$PWD/..
LIBS += -L$$LIBPATH_QHEXEDIT -L$$LIBPATH_ANTLR -L$$LIBPATH_QCUSTOMPLOT -L$$LIBPATH_QSCINTILLA -lantlr -lqhexedit -lqcustomplot -lqscintilla2
DEPENDPATH += $$PWD/../libs/antlr-2.7.7 $$PWD/../libs/qhexedit $$PWD/../libs/qcustomplot-source $$PWD/../libs/qscintilla/Qt4Qt5

unix {
    # Below, the user can specify where all generated file can be placed
    # through a set of variables, being them:
    #
    # PREFIX        -> the root directory where the files will be placed
    # BINDIR        -> where executables accessible by the user resides
    # DATADIR       -> where data files and resources should be placed
    #
    # The values of each variable changes between supported platforms and are describe as follow

    # Default configuration for package sqlitebrowser.
    # The default prefix is /usr/local
    !defined(PREFIX, var):        PREFIX = /usr/local
    !defined(BINDIR, var):        BINDIR = $$PREFIX/bin
    !defined(DATADIR, var):       DATADIR = $$PREFIX/share

    # The executable
    target.path = $$BINDIR
    INSTALLS += target

    # Icon
    icon.path = $$DATADIR/icons/hicolor/256x256/apps/
    icon.files = icons/sqlitebrowser.png
    INSTALLS += icon

    # Desktop metadata
    desktop.path = $$DATADIR/applications/
    desktop.files = ../distri/sqlitebrowser.desktop
    INSTALLS += desktop
    appdata.path = $$DATADIR/appdata/
    appdata.files = ../distri/sqlitebrowser.desktop.appdata.xml
    INSTALLS += appdata
}

# Rules for creating/updating {ts|qm}-files
include(i18n.pri)
