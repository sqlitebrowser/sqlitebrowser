project(sqlitebrowser)
cmake_minimum_required(VERSION 2.8.12.2)

# BUILD_VERSION is the current date in YYYYMMDD format. It is only
# used by the nightly version to add the date of the build.
string(TIMESTAMP BUILD_VERSION "%Y%m%d")

# Choose between building a stable version or nightly (the default), depending
# on whether '-DBUILD_STABLE_VERSION=1' is passed on the command line or not.
option(BUILD_STABLE_VERSION "Don't build the stable version by default" OFF)
if(NOT BUILD_STABLE_VERSION)
    add_definitions(-DBUILD_VERSION=${BUILD_VERSION})
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

OPTION(ENABLE_TESTING "Enable the unit tests" OFF)
OPTION(FORCE_INTERNAL_ANTLR "Don't use the distribution's Antlr library even if there is one" OFF)
OPTION(FORCE_INTERNAL_QSCINTILLA "Don't use the distribution's QScintilla library even if there is one" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

add_definitions(-std=c++11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()

if(WIN32 AND MSVC)
	if(CMAKE_CL_64)
		# Paths for 64-bit windows builds
		set(OPENSSL_PATH "C:/dev/OpenSSL-Win64" CACHE PATH "OpenSSL Path")
		set(QT5_PATH "C:/dev/Qt/5.11.2/msvc2017_64" CACHE PATH "Qt5 Path")

                # Choose between SQLCipher or SQLite, depending whether
                # -Dsqlcipher=1 is passed on the command line
                if(sqlcipher)
                        set(SQLITE3_PATH "C:/git_repos/SQLCipher-Win64" CACHE PATH "SQLCipher Path")
                else()
                        set(SQLITE3_PATH "C:/dev/SQLite-Win64" CACHE PATH "SQLite Path")
                endif()
	else()
		# Paths for 32-bit windows builds
		set(OPENSSL_PATH "C:/dev/OpenSSL-Win32" CACHE PATH "OpenSSL Path")
		set(QT5_PATH "C:/dev/Qt/5.7/msvc2013" CACHE PATH "Qt5 Path")

                # Choose between SQLCipher or SQLite, depending whether
                # -Dsqlcipher=1 is passed on the command line
                if(sqlcipher)
                        set(SQLITE3_PATH "C:/git_repos/SQLCipher-Win32" CACHE PATH "SQLCipher Path")
                else()
                        set(SQLITE3_PATH "C:/dev/SQLite-Win32" CACHE PATH "SQLite Path")
                endif()
	endif()
	set(CMAKE_PREFIX_PATH "${QT5_PATH};${SQLITE3_PATH}")
endif()

if(NOT FORCE_INTERNAL_ANTLR)
    find_package(Antlr2 QUIET)
endif()
if(NOT FORCE_INTERNAL_QSCINTILLA)
    find_package(QScintilla QUIET)
endif()

set(QHEXEDIT_DIR libs/qhexedit)
set(QCUSTOMPLOT_DIR libs/qcustomplot-source)

if(NOT ANTLR2_FOUND)
    set(ANTLR_DIR libs/antlr-2.7.7)
    add_subdirectory(${ANTLR_DIR})
endif()
if(NOT QSCINTILLA_FOUND)
    set(QSCINTILLA_DIR libs/qscintilla/Qt4Qt5)
    add_subdirectory(${QSCINTILLA_DIR})
endif()
add_subdirectory(${QHEXEDIT_DIR})
add_subdirectory(${QCUSTOMPLOT_DIR})

find_package(Qt5 REQUIRED COMPONENTS Concurrent Gui LinguistTools Network PrintSupport Test Widgets Xml)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(ENABLE_TESTING)
    enable_testing()
endif()

set(SQLB_HDR
	src/version.h
	src/sql/sqlitetypes.h
	src/sql/Query.h
	src/csvparser.h
	src/sqlite.h
	src/grammar/sqlite3TokenTypes.hpp
	src/grammar/Sqlite3Lexer.hpp
	src/grammar/Sqlite3Parser.hpp
	src/Data.h
)

set(SQLB_MOC_HDR
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
	src/CipherSettings.h
	src/DotenvFormat.h
	src/Palette.h
	src/CondFormat.h
	src/RunSql.h
)

set(SQLB_SRC
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
	src/sqltextedit.cpp
	src/docktextedit.cpp
	src/csvparser.cpp
	src/DbStructureModel.cpp
	src/grammar/Sqlite3Lexer.cpp
	src/grammar/Sqlite3Parser.cpp
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
	src/Data.cpp
	src/CipherSettings.cpp
	src/DotenvFormat.cpp
	src/Palette.cpp
	src/CondFormat.cpp
	src/RunSql.cpp
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
)

set(SQLB_RESOURCES
	src/icons/icons.qrc
	src/translations/flags/flags.qrc
	src/translations/translations.qrc
	src/certs/CaCerts.qrc
)

set(SQLB_MISC
    src/grammar/sqlite3.g
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
		SET_SOURCE_FILES_PROPERTIES("${SQLB_TS}" PROPERTIES OUTPUT_LOCATION "${CMAKE_SOURCE_DIR}/src/translations")
	endforeach(SQLB_TS ${SQLB_TSS})
	qt5_add_translation(SQLB_QMS ${SQLB_TSS})
endif(SQLB_TSS)
qt5_add_resources(SQLB_RESOURCES_RCC ${SQLB_RESOURCES})

#icon and correct libs/subsystem for windows
if(WIN32)
        #enable version check for windows
        add_definitions(-DCHECKNEWVERSION)

	IF( MINGW )
	# resource compilation for MinGW
		ADD_CUSTOM_COMMAND(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/sqlbicon.o"
			COMMAND windres "-I${CMAKE_CURRENT_SOURCE_DIR}" "-i${CMAKE_CURRENT_SOURCE_DIR}/src/winapp.rc" -o "${CMAKE_CURRENT_BINARY_DIR}/sqlbicon.o" VERBATIM)
		set(SQLB_SRC ${SQLB_SRC} "${CMAKE_CURRENT_BINARY_DIR}/sqlbicon.o")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
		set(WIN32_STATIC_LINK -Wl,-Bstatic -lssl -lcrypto -lws2_32)
		set(ADDITIONAL_LIBS lzma)
	ELSE( MINGW )
		set(SQLB_SRC ${SQLB_SRC} "${CMAKE_CURRENT_SOURCE_DIR}/src/winapp.rc")
	ENDIF( MINGW )
else()
	set(LPTHREAD pthread)
endif(WIN32)

#enable version check for MacOS
if(APPLE)
	add_definitions(-DCHECKNEWVERSION)
endif(APPLE)

# SQLCipher option
if(sqlcipher)
	add_definitions(-DENABLE_SQLCIPHER)
	set(LIBSQLITE_NAME sqlcipher)
else(sqlcipher)
	set(LIBSQLITE_NAME sqlite3)
endif(sqlcipher)

# add extra library path for MacOS and FreeBSD
set(EXTRAPATH APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
if(EXTRAPATH)
	find_library(LIBSQLITE ${LIBSQLITE_NAME} HINTS /usr/local/lib /usr/local/opt/sqlite/lib)
	set(ADDITIONAL_INCLUDE_PATHS /usr/local/include /usr/local/opt/sqlite/include)
else(EXTRAPATH)
	find_library(LIBSQLITE ${LIBSQLITE_NAME})
endif(EXTRAPATH)

if(WIN32 AND MSVC)
	find_path(SQLITE3_INCLUDE_DIR sqlite3.h)
	if(sqlcipher)
		find_file(SQLITE3_DLL sqlcipher.dll)
	else(sqlcipher)
		find_file(SQLITE3_DLL sqlite3.dll)
	endif(sqlcipher)
	include_directories(${SQLITE3_INCLUDE_DIR})
endif()

include_directories(
		"${CMAKE_CURRENT_BINARY_DIR}"
		${QHEXEDIT_DIR}
		${QCUSTOMPLOT_DIR}
		${ADDITIONAL_INCLUDE_PATHS}
		src)
if(ANTLR2_FOUND)
    include_directories(${ANTLR2_INCLUDE_DIRS})
else()
    include_directories(${ANTLR_DIR})
endif()
if(QSCINTILLA_FOUND)
    include_directories(${QSCINTILLA_INCLUDE_DIR})
else()
    include_directories(${QSCINTILLA_DIR})
endif()

add_executable(${PROJECT_NAME}
		${SQLB_HDR}
		${SQLB_SRC}
		${SQLB_FORM_HDR}
		${SQLB_MOC}
		${SQLB_RESOURCES_RCC}
		${SQLB_MISC})

add_dependencies(${PROJECT_NAME} qhexedit qcustomplot)
if(NOT ANTLR2_FOUND)
    add_dependencies(${PROJECT_NAME} antlr)
endif()
if(NOT QSCINTILLA_FOUND)
    add_dependencies(${PROJECT_NAME} qscintilla2)
endif()

link_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/${QHEXEDIT_DIR}"
	"${CMAKE_CURRENT_BINARY_DIR}/${QCUSTOMPLOT_DIR}"
)
if(NOT ANTLR2_FOUND)
    link_directories("${CMAKE_CURRENT_BINARY_DIR}/${ANTLR_DIR}")
endif()
if(NOT QSCINTILLA_FOUND)
    link_directories("${CMAKE_CURRENT_BINARY_DIR}/${QSCINTILLA_DIR}")
endif()

set(QT_LIBS Qt5::Gui Qt5::Test Qt5::PrintSupport Qt5::Widgets Qt5::Network Qt5::Concurrent Qt5::Xml)

target_link_libraries(${PROJECT_NAME}
		qhexedit
		qcustomplot
    ${LPTHREAD}
    ${QT_LIBS}
		${WIN32_STATIC_LINK}
		${LIBSQLITE}
		${ADDITIONAL_LIBS})
if(ANTLR2_FOUND)
    target_link_libraries(${PROJECT_NAME} ${ANTLR2_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} antlr)
endif()
if(QSCINTILLA_FOUND)
    target_link_libraries(${PROJECT_NAME} ${QSCINTILLA_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} qscintilla2)
endif()

if(WIN32 AND MSVC)
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

if(NOT WIN32)
	install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib)
endif()

if(ENABLE_TESTING)
	add_subdirectory(src/tests)
endif()

if(UNIX AND NOT APPLE)
	install(FILES src/icons/${PROJECT_NAME}.png
		DESTINATION share/icons/hicolor/256x256/apps/)

	install(FILES distri/${PROJECT_NAME}.desktop
		DESTINATION share/applications/)

	install(FILES distri/${PROJECT_NAME}.desktop.appdata.xml
		DESTINATION share/appdata/)
endif(UNIX AND NOT APPLE)

if(WIN32 AND MSVC)
	install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION "/"
		LIBRARY DESTINATION lib)

	set(QT5_BIN_PATH ${QT5_PATH}/bin)
	# The Qt5 Debug configuration library files have a 'd' postfix
	install(FILES
			${QT5_BIN_PATH}/Qt5Cored.dll
			${QT5_BIN_PATH}/Qt5Guid.dll
			${QT5_BIN_PATH}/Qt5Networkd.dll
			${QT5_BIN_PATH}/Qt5PrintSupportd.dll
			${QT5_BIN_PATH}/Qt5Widgetsd.dll
			${QT5_BIN_PATH}/Qt5Concurrentd.dll
		DESTINATION "/"
		CONFIGURATIONS Debug)
	# The Qt5 Release configuration files don't have a postfix
	install(FILES
			${QT5_BIN_PATH}/Qt5Core.dll
			${QT5_BIN_PATH}/Qt5Gui.dll
			${QT5_BIN_PATH}/Qt5Network.dll
			${QT5_BIN_PATH}/Qt5PrintSupport.dll
			${QT5_BIN_PATH}/Qt5Widgets.dll
			${QT5_BIN_PATH}/Qt5Concurrent.dll
		DESTINATION "/"
		CONFIGURATIONS Release)
	# The files below are common to all configurations
	install(FILES
			${SQLITE3_DLL}
			${OPENSSL_PATH}/libeay32.dll
			${OPENSSL_PATH}/ssleay32.dll
		DESTINATION "/")
	install(FILES
		${QT5_PATH}/plugins/platforms/qwindows.dll
		DESTINATION platforms)

	# The XML dll
	install(FILES
			"${QT5_PATH}/bin/Qt5Xmld.dll"
		DESTINATION "/"
		CONFIGURATIONS Debug)
	install(FILES
			"${QT5_PATH}/bin/Qt5Xml.dll"
		DESTINATION "/"
		CONFIGURATIONS Release)

	# The image format plugins
	install(FILES
			${WIN_IMG_PLUGINS_DEBUG}
		DESTINATION imageformats
		CONFIGURATIONS Debug)
	install(FILES
			${WIN_IMG_PLUGINS}
		DESTINATION imageformats
		CONFIGURATIONS Release)

	# The license files
	install(FILES
			${LICENSE_FILES}
		DESTINATION licenses)

	# The batch file launcher
	install(FILES
			distri/winlaunch.bat
		DESTINATION "/")
endif()
