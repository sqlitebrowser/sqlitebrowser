cmake_minimum_required(VERSION 3.15)
project(sqlitebrowser
    VERSION 3.13.99
    DESCRIPTION "GUI editor for SQLite databases"
)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if(POLICY CMP0025)
    # https://cmake.org/cmake/help/latest/policy/CMP0025.html
    cmake_policy(SET CMP0025 NEW)
endif()

# Fix warning of AUTOMOC behavior
if(POLICY CMP0071)
    # https://cmake.org/cmake/help/latest/policy/CMP0071.html
    cmake_policy(SET CMP0071 NEW)
endif()

# Fix warning of Cached Variables
if(POLICY CMP0102)
    # https://cmake.org/cmake/help/latest/policy/CMP0102.html
    cmake_policy(SET CMP0102 NEW)
endif()

include(GNUInstallDirs)

OPTION(BUILD_STABLE_VERSION "Don't build the stable version by default" OFF) # Choose between building a stable version or nightly (the default), depending on whether '-DBUILD_STABLE_VERSION=1' is passed on the command line or not.
OPTION(ENABLE_TESTING "Enable the unit tests" OFF)
OPTION(FORCE_INTERNAL_QSCINTILLA "Don't use the distribution's QScintilla library even if there is one" OFF)
OPTION(FORCE_INTERNAL_QCUSTOMPLOT "Don't use distribution's QCustomPlot even if available" ON)
OPTION(FORCE_INTERNAL_QHEXEDIT "Don't use distribution's QHexEdit even if available" ON)
OPTION(ALL_WARNINGS "Enable some useful warning flags" OFF)
OPTION(sqlcipher "Build with SQLCipher library" OFF)
OPTION(customTap "Using SQLCipher, SQLite and Qt installed through our custom Homebrew tap" OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE)
elseif(WIN32)
    add_executable(${PROJECT_NAME} WIN32)
else()
    add_executable(${PROJECT_NAME})
endif()

# Determine the git commit hash
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

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(MSVC)
    if(CMAKE_CL_64)
        # Paths for 64-bit windows builds
        set(OPENSSL_PATH "C:/dev/OpenSSL-Win64" CACHE PATH "OpenSSL Path")
        set(QT5_PATH "C:/dev/Qt/5.12.12/msvc2017_64" CACHE PATH "Qt5 Path")

        # Choose between SQLCipher or SQLite, depending whether
        # -Dsqlcipher=on is passed on the command line
        if(sqlcipher)
            set(SQLITE3_PATH "C:/git_repos/SQLCipher-Win64" CACHE PATH "SQLCipher Path")
        else()
            set(SQLITE3_PATH "C:/dev/SQLite-Win64" CACHE PATH "SQLite Path")
        endif()
    else()
        # Paths for 32-bit windows builds
        set(OPENSSL_PATH "C:/dev/OpenSSL-Win32" CACHE PATH "OpenSSL Path")
        set(QT5_PATH "C:/dev/Qt/5.12.12/msvc2017" CACHE PATH "Qt5 Path")

        # Choose between SQLCipher or SQLite, depending whether
        # -Dsqlcipher=on is passed on the command line
        if(sqlcipher)
            set(SQLITE3_PATH "C:/git_repos/SQLCipher-Win32" CACHE PATH "SQLCipher Path")
        else()
            set(SQLITE3_PATH "C:/dev/SQLite-Win32" CACHE PATH "SQLite Path")
        endif()
    endif()

    list(PREPEND CMAKE_PREFIX_PATH ${QT5_PATH} ${SQLITE3_PATH})
endif()


if(APPLE)
    # For Intel Mac's
    if(EXISTS /usr/local/opt/qt5)
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
    endif()

    # For Apple Silicon Mac's
    if(EXISTS /opt/homebrew/opt/qt5)
        list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt5")
    endif()
    if(EXISTS /opt/homebrew/opt/sqlitefts5)
        list(PREPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sqlitefts5")
    endif()

    # For Apple Silicon Mac's and install dependencies via our Homebrew tap(sqlitebrowser/homebrew-tap)
    if(customTap AND EXISTS /opt/homebrew/opt/)
        list(PREPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/db4subqt@5")
        list(PREPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/db4subsqlitefts@5")

        if(sqlcipher)
            list(APPEND SQLCIPHER_INCLUDE_DIR "/opt/homebrew/include")
            list(APPEND SQLCIPHER_LIBRARY "/opt/homebrew/opt/db4subsqlcipher/lib/libsqlcipher.0.dylib")
        endif()
    endif()
endif()

find_package(Qt5 REQUIRED COMPONENTS Concurrent Gui LinguistTools Network PrintSupport Test Widgets Xml)

if(NOT FORCE_INTERNAL_QSCINTILLA)
    find_package(QScintilla 2.8.10)
endif()
if(NOT FORCE_INTERNAL_QCUSTOMPLOT)
    find_package(QCustomPlot)
endif()
if(NOT FORCE_INTERNAL_QHEXEDIT)
    find_package(QHexEdit)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/libs/json)

if(NOT QSCINTILLA_FOUND)
    add_subdirectory(libs/qscintilla/Qt4Qt5)
endif()
if(NOT QHexEdit_FOUND)
    add_subdirectory(libs/qhexedit)
endif()
if(NOT QCustomPlot_FOUND)
    add_subdirectory(libs/qcustomplot-source)
endif()

if(ENABLE_TESTING)
    enable_testing()
endif()

# generate file with version information
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
    src/DotenvFormat.h
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
    src/DotenvFormat.cpp
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

set(SQLB_FORMS
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

set(SQLB_RESOURCES
    src/icons/icons.qrc
    src/translations/flags/flags.qrc
    src/translations/translations.qrc
    src/certs/CaCerts.qrc
    src/qdarkstyle/dark/darkstyle.qrc
    src/qdarkstyle/light/lightstyle.qrc
)

set(SQLB_MISC
    src/sql/parser/sqlite3_parser.yy
    src/sql/parser/sqlite3_lexer.ll
)

# Translation files
set(SQLB_TSS
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_ar_SA.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_cs.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_zh.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_zh_TW.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_de.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_es_ES.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_fr.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_ru.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_pl.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_pt_BR.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_en_GB.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_ko_KR.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_tr.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_uk_UA.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_it.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_ja.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_nl.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_sv.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_id.ts"
)

# Windows image format plugin files
set(WIN_IMG_PLUGINS
    "${QT5_PATH}/plugins/imageformats/qgif.dll"
    "${QT5_PATH}/plugins/imageformats/qicns.dll"
    "${QT5_PATH}/plugins/imageformats/qico.dll"
    "${QT5_PATH}/plugins/imageformats/qjpeg.dll"
    "${QT5_PATH}/plugins/imageformats/qsvg.dll"
    "${QT5_PATH}/plugins/imageformats/qtga.dll"
    "${QT5_PATH}/plugins/imageformats/qtiff.dll"
    "${QT5_PATH}/plugins/imageformats/qwbmp.dll"
    "${QT5_PATH}/plugins/imageformats/qwebp.dll"
)
set(WIN_IMG_PLUGINS_DEBUG
    "${QT5_PATH}/plugins/imageformats/qgifd.dll"
    "${QT5_PATH}/plugins/imageformats/qicnsd.dll"
    "${QT5_PATH}/plugins/imageformats/qicod.dll"
    "${QT5_PATH}/plugins/imageformats/qjpegd.dll"
    "${QT5_PATH}/plugins/imageformats/qsvgd.dll"
    "${QT5_PATH}/plugins/imageformats/qtgad.dll"
    "${QT5_PATH}/plugins/imageformats/qtiffd.dll"
    "${QT5_PATH}/plugins/imageformats/qwbmpd.dll"
    "${QT5_PATH}/plugins/imageformats/qwebpd.dll"
)

# License files
set(LICENSE_FILES
    LICENSE
    LICENSE-PLUGINS
)

qt5_wrap_ui(SQLB_FORM_HDR ${SQLB_FORMS})
if(SQLB_TSS)
    # add translations
    foreach(SQLB_TS ${SQLB_TSS})
        set_source_files_properties("${SQLB_TS}" PROPERTIES OUTPUT_LOCATION "${CMAKE_SOURCE_DIR}/src/translations")
    endforeach()
    qt5_add_translation(SQLB_QMS ${SQLB_TSS})
endif()
qt5_add_resources(SQLB_RESOURCES_RCC ${SQLB_RESOURCES})

#icon and correct libs/subsystem for windows
if(WIN32)
    #enable version check for windows
    add_definitions(-DCHECKNEWVERSION)

    if(MINGW)
        # resource compilation for MinGW
        add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/sqlbicon.o"
            COMMAND windres "-I${CMAKE_CURRENT_BINARY_DIR}" "-i${CMAKE_CURRENT_SOURCE_DIR}/src/winapp.rc" -o "${CMAKE_CURRENT_BINARY_DIR}/sqlbicon.o" VERBATIM
        )
        target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/sqlbicon.o")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
        set(WIN32_STATIC_LINK -Wl,-Bstatic -lssl -lcrypto -lws2_32)
        set(ADDITIONAL_LIBS lzma)
    else()
        target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/winapp.rc")
    endif()
else()
    set(LPTHREAD pthread)
endif()

#enable version check for macOS
if(APPLE)
    add_definitions(-DCHECKNEWVERSION)
endif()

# SQLCipher option
if(sqlcipher)
    add_definitions(-DENABLE_SQLCIPHER)
    set(LIBSQLITE_NAME SQLCipher)
else()
    set(LIBSQLITE_NAME SQLite3)
endif()

# add extra library path for MacOS and FreeBSD
set(EXTRAPATH APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
if(EXTRAPATH)
    list(PREPEND CMAKE_PREFIX_PATH /usr/local/opt/sqlite/lib)
    list(PREPEND CMAKE_PREFIX_PATH /usr/local/opt/sqlitefts5/lib)
endif()

find_package(${LIBSQLITE_NAME})
if (sqlcipher)
    target_link_libraries(${PROJECT_NAME} SQLCipher::SQLCipher)
else()
    target_link_libraries(${PROJECT_NAME} SQLite::SQLite3)
endif()

if(MSVC)
    if(sqlcipher)
        find_file(SQLITE3_DLL sqlcipher.dll)
    else()
        find_file(SQLITE3_DLL sqlite3.dll)
    endif()
endif()

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${SQLB_FORM_HDR}
    ${SQLB_MOC}
    ${SQLB_RESOURCES_RCC}
    ${SQLB_MISC}
)

# Warnings
if(ALL_WARNINGS AND CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wdouble-promotion -Wformat=2 -Wlogical-op -Wuseless-cast)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7.0)
        target_compile_options(${PROJECT_NAME} PRIVATE -Wnull-dereference -Wduplicated-cond -Wduplicated-branches)
    endif()
endif()

set(QT_LIBS Qt5::Gui Qt5::Test Qt5::PrintSupport Qt5::Widgets Qt5::Network Qt5::Concurrent Qt5::Xml)

target_link_libraries(${PROJECT_NAME}
    ${LPTHREAD}
    ${QT_LIBS}
    ${WIN32_STATIC_LINK}
    ${ADDITIONAL_LIBS}
)

target_link_libraries(${PROJECT_NAME}
    QHexEdit::QHexEdit
    QCustomPlot::QCustomPlot
    QScintilla::QScintilla
)

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "DB Browser for SQLite")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
    if(CMAKE_CL_64)
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS,5.02 /ENTRY:mainCRTStartup")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS,5.02")
    else()
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS,5.01 /ENTRY:mainCRTStartup")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS,5.01")
    endif()
endif()

if((NOT WIN32 AND NOT APPLE) OR MINGW)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

if(UNIX)
    target_link_libraries(${PROJECT_NAME} dl)
endif()

if(ENABLE_TESTING)
    add_subdirectory(src/tests)
endif()

if(UNIX)
    install(FILES src/icons/${PROJECT_NAME}.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps/
    )

    install(FILES images/logo.svg
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps/
        RENAME ${PROJECT_NAME}.svg
    )

    install(FILES distri/${PROJECT_NAME}.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications/
    )

    install(FILES distri/${PROJECT_NAME}.desktop.appdata.xml
        DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo/
    )
endif()

if(WIN32 AND MSVC)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "/"
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    set(QT5_BIN_PATH ${QT5_PATH}/bin)

    # The Qt5 Debug configuration library files have a 'd' postfix
    install(FILES
        ${QT5_BIN_PATH}/Qt5Cored.dll
        ${QT5_BIN_PATH}/Qt5Guid.dll
        ${QT5_BIN_PATH}/Qt5Networkd.dll
        ${QT5_BIN_PATH}/Qt5PrintSupportd.dll
        ${QT5_BIN_PATH}/Qt5Widgetsd.dll
        ${QT5_BIN_PATH}/Qt5Concurrentd.dll
        ${QT5_BIN_PATH}/Qt5Svgd.dll
        DESTINATION "/"
        CONFIGURATIONS Debug
    )

    # The Qt5 Release configuration files don't have a postfix
    install(FILES
        ${QT5_BIN_PATH}/Qt5Core.dll
        ${QT5_BIN_PATH}/Qt5Gui.dll
        ${QT5_BIN_PATH}/Qt5Network.dll
        ${QT5_BIN_PATH}/Qt5PrintSupport.dll
        ${QT5_BIN_PATH}/Qt5Widgets.dll
        ${QT5_BIN_PATH}/Qt5Concurrent.dll
        ${QT5_BIN_PATH}/Qt5Svg.dll
        DESTINATION "/"
        CONFIGURATIONS Release
    )

    # The files below are common to all configurations
    install(FILES
        ${SQLITE3_DLL}
        ${OPENSSL_PATH}/libeay32.dll
        ${OPENSSL_PATH}/ssleay32.dll
        DESTINATION "/"
    )

    install(FILES
        ${QT5_PATH}/plugins/platforms/qwindows.dll
        DESTINATION platforms
    )

    # The XML dll
    install(FILES
        "${QT5_PATH}/bin/Qt5Xmld.dll"
        DESTINATION "/"
        CONFIGURATIONS Debug
    )

    install(FILES
        "${QT5_PATH}/bin/Qt5Xml.dll"
        DESTINATION "/"
        CONFIGURATIONS Release
    )

    # The image format plugins
    install(FILES
        ${WIN_IMG_PLUGINS_DEBUG}
        DESTINATION imageformats
        CONFIGURATIONS Debug
    )

    install(FILES
        ${WIN_IMG_PLUGINS}
        DESTINATION imageformats
        CONFIGURATIONS Release
    )

    # The license files
    install(FILES
        ${LICENSE_FILES}
        DESTINATION licenses
    )

    # The batch file launcher
    install(FILES
        distri/winlaunch.bat
        DESTINATION "/"
    )
endif()

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)
    set(ENV{MACOSX_DEPLOYMENT_TARGET} 10.15)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUNDLE True
        OUTPUT_NAME "DB Browser for SQLite"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/src/app.plist
    )
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
