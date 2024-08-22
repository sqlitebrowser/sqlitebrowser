cmake_minimum_required(VERSION 3.16)

project(sqlitebrowser
    VERSION 3.13.99
    DESCRIPTION "GUI editor for SQLite databases"
    LANGUAGES CXX
)

include(GNUInstallDirs)

include(config/options.cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
add_executable(${PROJECT_NAME})


if(QT_MAJOR STREQUAL "Qt5")
    set(CMAKE_CXX_STANDARD 14)
elseif(QT_MAJOR STREQUAL "Qt6")
    set(CMAKE_CXX_STANDARD 17)
else()
    message(FATAL_ERROR "Uknown Qt Version: ${QT_MAJOR}")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

execute_process(
    COMMAND git -C ${CMAKE_CURRENT_SOURCE_DIR} rev-parse --short --verify HEAD
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if (GIT_COMMIT_HASH STREQUAL "")
    MESSAGE(WARNING "Could not determine git commit hash")
    set(GIT_COMMIT_HASH "Unknown")
endif()

add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")
if(NOT BUILD_STABLE_VERSION)
    # BUILD_VERSION is the current date in YYYYMMDD format. It is only
    # used by the nightly version to add the date of the build.
    # Default defined in Version.h.in
    string(TIMESTAMP BUILD_VERSION "%Y%m%d")
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_VERSION=${BUILD_VERSION})
endif()

find_package(${QT_MAJOR} REQUIRED COMPONENTS Concurrent Gui LinguistTools Network PrintSupport Test Widgets Xml)
set(QT_LIBS
    ${QT_MAJOR}::Gui
    ${QT_MAJOR}::Test
    ${QT_MAJOR}::PrintSupport
    ${QT_MAJOR}::Widgets
    ${QT_MAJOR}::Network
    ${QT_MAJOR}::Concurrent
    ${QT_MAJOR}::Xml
)
if(QT_MAJOR STREQUAL "Qt6")
    find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
    list(APPEND QT_LIBS Qt6::Core5Compat)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        AUTOUIC_OPTIONS "--connections=string"
    )
endif()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${CMAKE_CURRENT_LIST_DIR}/libs/json)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR} src)

include(config/3dparty.cmake)
include(config/platform.cmake)

# SQLCipher option
if(sqlcipher)
    add_definitions(-DENABLE_SQLCIPHER)
    find_package(SQLCipher REQUIRED)
    include_directories(SYSTEM "${SQLCIPHER_INCLUDE_DIR}/sqlcipher")
    set(LIBSQLITE_NAME SQLCipher::SQLCipher)
else()
    find_package(SQLite3 REQUIRED)
    set(LIBSQLITE_NAME SQLite::SQLite3)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

target_sources(${PROJECT_NAME}
    PRIVATE
    src/sql/sqlitetypes.h
    src/sql/Query.h
    src/sql/ObjectIdentifier.h
    src/csvparser.h
    src/sqlite.h
    src/Data.h
    src/IconCache.h
    src/sql/parser/ParserDriver.h
    src/sql/parser/sqlite3_lexer.h
    src/sql/parser/sqlite3_location.h
    src/sql/parser/sqlite3_parser.hpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
    src/sqlitedb.h
    src/AboutDialog.h
    src/EditIndexDialog.h
    src/EditDialog.h
    src/EditTableDialog.h
    src/AddRecordDialog.h
    src/ExportDataDialog.h
    src/ExtendedTableWidget.h
    src/FilterTableHeader.h
    src/ImportCsvDialog.h
    src/MainWindow.h
    src/Settings.h
    src/PreferencesDialog.h
    src/SqlExecutionArea.h
    src/VacuumDialog.h
    src/sqlitetablemodel.h
    src/RowLoader.h
    src/RowCache.h
    src/sqltextedit.h
    src/docktextedit.h
    src/DbStructureModel.h
    src/dbstructureqitemviewfacade.h
    src/Application.h
    src/CipherDialog.h
    src/ExportSqlDialog.h
    src/SqlUiLexer.h
    src/FileDialog.h
    src/ColumnDisplayFormatDialog.h
    src/FilterLineEdit.h
    src/RemoteDatabase.h
    src/ForeignKeyEditorDelegate.h
    src/PlotDock.h
    src/RemoteDock.h
    src/RemoteModel.h
    src/RemotePushDialog.h
    src/FindReplaceDialog.h
    src/ExtendedScintilla.h
    src/FileExtensionManager.h
    src/CondFormatManager.h
    src/CipherSettings.h
    src/Palette.h
    src/CondFormat.h
    src/RunSql.h
    src/ProxyDialog.h
    src/SelectItemsPopup.h
    src/TableBrowser.h
    src/ImageViewer.h
    src/RemoteLocalFilesModel.h
    src/RemoteCommitsModel.h
    src/RemoteNetwork.h
    src/TableBrowserDock.h
)

target_sources(${PROJECT_NAME}
    PRIVATE
    src/AboutDialog.cpp
    src/EditIndexDialog.cpp
    src/EditDialog.cpp
    src/EditTableDialog.cpp
    src/AddRecordDialog.cpp
    src/ExportDataDialog.cpp
    src/ExtendedTableWidget.cpp
    src/FilterTableHeader.cpp
    src/ImportCsvDialog.cpp
    src/MainWindow.cpp
    src/Settings.cpp
    src/PreferencesDialog.cpp
    src/SqlExecutionArea.cpp
    src/VacuumDialog.cpp
    src/sqlitedb.cpp
    src/sqlitetablemodel.cpp
    src/RowLoader.cpp
    src/sql/sqlitetypes.cpp
    src/sql/Query.cpp
    src/sql/ObjectIdentifier.cpp
    src/sqltextedit.cpp
    src/docktextedit.cpp
    src/csvparser.cpp
    src/DbStructureModel.cpp
    src/dbstructureqitemviewfacade.cpp
    src/main.cpp
    src/Application.cpp
    src/CipherDialog.cpp
    src/ExportSqlDialog.cpp
    src/SqlUiLexer.cpp
    src/FileDialog.cpp
    src/ColumnDisplayFormatDialog.cpp
    src/FilterLineEdit.cpp
    src/RemoteDatabase.cpp
    src/ForeignKeyEditorDelegate.cpp
    src/PlotDock.cpp
    src/RemoteDock.cpp
    src/RemoteModel.cpp
    src/RemotePushDialog.cpp
    src/FindReplaceDialog.cpp
    src/ExtendedScintilla.cpp
    src/FileExtensionManager.cpp
    src/CondFormatManager.cpp
    src/Data.cpp
    src/CipherSettings.cpp
    src/Palette.cpp
    src/CondFormat.cpp
    src/RunSql.cpp
    src/ProxyDialog.cpp
    src/IconCache.cpp
    src/SelectItemsPopup.cpp
    src/TableBrowser.cpp
    src/sql/parser/ParserDriver.cpp
    src/sql/parser/sqlite3_lexer.cpp
    src/sql/parser/sqlite3_parser.cpp
    src/ImageViewer.cpp
    src/RemoteLocalFilesModel.cpp
    src/RemoteCommitsModel.cpp
    src/RemoteNetwork.cpp
    src/TableBrowserDock.cpp
)

source_group("Qt UI" "src/.*\.ui$")
target_sources(${PROJECT_NAME}
    PRIVATE
    src/AboutDialog.ui
    src/EditIndexDialog.ui
    src/EditDialog.ui
    src/EditTableDialog.ui
    src/AddRecordDialog.ui
    src/ExportDataDialog.ui
    src/ImportCsvDialog.ui
    src/MainWindow.ui
    src/PreferencesDialog.ui
    src/SqlExecutionArea.ui
    src/VacuumDialog.ui
    src/CipherDialog.ui
    src/ExportSqlDialog.ui
    src/ColumnDisplayFormatDialog.ui
    src/PlotDock.ui
    src/RemoteDock.ui
    src/RemotePushDialog.ui
    src/FindReplaceDialog.ui
    src/FileExtensionManager.ui
    src/CondFormatManager.ui
    src/ProxyDialog.ui
    src/SelectItemsPopup.ui
    src/TableBrowser.ui
    src/ImageViewer.ui
)

include(config/translations.cmake)

source_group("Qt Resources" "src/.*\.qrc$")
target_sources(${PROJECT_NAME}
    PRIVATE

    # General
    src/certs/CaCerts.qrc
    src/sql/parser/sqlite3_parser.yy
    src/sql/parser/sqlite3_lexer.ll

    # Graphics
    src/icons/icons.qrc

    # Translations
    src/translations/flags/flags.qrc
    src/translations/translations.qrc

    # Styles
    src/qdarkstyle/dark/darkstyle.qrc
    src/qdarkstyle/light/lightstyle.qrc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${QT_LIBS}
    QHexEdit::QHexEdit QCustomPlot::QCustomPlot QScintilla::QScintilla
    ${LIBSQLITE_NAME}
)

include(config/install.cmake)

if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(src/tests)
endif()

# CPack configuration
set(CPACK_STRIP_FILES ON)
set(CPACK_DEBIAN_PACKAGE_PRIORITY optional)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Tristan Stenner <dbbrowser@nicht.dienstli.ch>")
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
if(APPLE)
    set(CPACK_DEFAULT_GEN TBZ2)
elseif(WIN32)
    set(CPACK_DEFAULT_GEN ZIP)
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_WIX_CMAKE_PACKAGE_REGISTRY ON)
    set(CPACK_WIX_UPGRADE_GUID "78c885a7-e9c8-4ded-9b62-9abe47466950")
elseif(UNIX)
    set(CPACK_DEFAULT_GEN DEB)
    set(CPACK_SET_DESTDIR 1)
    set(CPACK_INSTALL_PREFIX "/usr")
endif()
set(CPACK_GENERATOR ${CPACK_DEFAULT_GEN} CACHE STRING "CPack pkg type(s) to generate")
include(CPack)
