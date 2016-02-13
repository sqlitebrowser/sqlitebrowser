project(sqlitebrowser)
cmake_minimum_required(VERSION 2.8.7)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

OPTION(USE_QT5 FALSE "Build with qt5")
OPTION(ENABLE_TESTING FALSE "Enable the unit tests")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(WIN32 AND MSVC)
	if(CMAKE_CL_64)
		set(QT5_PATH "E:/Qt64/Qt5.5.1/5.5/msvc2013_64")
		set(SQLITE3_PATH "E:/libs/sqlite3_x64")
		set(OPENSSL_PATH "E:/libs/openssl-1.0.2a-x64_86-win64")
		set(VSREDIST "vcredist_x64.exe")
	else()
		set(QT5_PATH "E:/Qt/Qt5.5.1/5.5/msvc2013")
		set(SQLITE3_PATH "E:/libs/sqlite3")
		set(OPENSSL_PATH "E:/libs/openssl-1.0.2a-i386-win32")
		set(VSREDIST "vcredist_x86.exe")
	endif()
	set(USE_QT5 TRUE)
	set(CMAKE_PREFIX_PATH "${QT5_PATH};${SQLITE3_PATH}")
endif()

find_package(Antlr2)

set(QHEXEDIT_DIR libs/qhexedit)
set(QCUSTOMPLOT_DIR libs/qcustomplot-source)
set(QSCINTILLA_DIR libs/qscintilla/Qt4Qt5)

if(NOT ANTLR2_FOUND)
    set(ANTLR_DIR libs/antlr-2.7.7)
    add_subdirectory(${ANTLR_DIR})
endif()
add_subdirectory(${QHEXEDIT_DIR})
add_subdirectory(${QCUSTOMPLOT_DIR})
add_subdirectory(${QSCINTILLA_DIR})

if(USE_QT5)
	find_package(Qt5Widgets REQUIRED)
	find_package(Qt5LinguistTools REQUIRED)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
else()
    find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
    include("${QT_USE_FILE}")
    add_definitions(${QT_DEFINITIONS})
endif()

if(ENABLE_TESTING)
    enable_testing()
endif()

set(SQLB_HDR
	src/version.h
	src/sqlitetypes.h
	src/csvparser.h
	src/sqlite.h
	src/grammar/sqlite3TokenTypes.hpp
	src/grammar/Sqlite3Lexer.hpp
	src/grammar/Sqlite3Parser.hpp
)

set(SQLB_MOC_HDR
	src/sqlitedb.h
	src/AboutDialog.h
	src/CreateIndexDialog.h
	src/EditDialog.h
	src/EditTableDialog.h
	src/ExportCsvDialog.h
	src/ExtendedTableWidget.h
	src/FilterTableHeader.h
	src/ImportCsvDialog.h
	src/MainWindow.h
	src/PreferencesDialog.h
	src/SqlExecutionArea.h
	src/VacuumDialog.h
	src/sqlitetablemodel.h
	src/sqltextedit.h
	src/DbStructureModel.h
	src/Application.h
	src/CipherDialog.h
	src/ExportSqlDialog.h
	src/SqlUiLexer.h
	src/FileDialog.h
	src/ColumnDisplayFormatDialog.h
	src/FilterLineEdit.h
)

set(SQLB_SRC
	src/AboutDialog.cpp
	src/CreateIndexDialog.cpp
	src/EditDialog.cpp
	src/EditTableDialog.cpp
	src/ExportCsvDialog.cpp
	src/ExtendedTableWidget.cpp
	src/FilterTableHeader.cpp
	src/ImportCsvDialog.cpp
	src/MainWindow.cpp
	src/PreferencesDialog.cpp
	src/SqlExecutionArea.cpp
	src/VacuumDialog.cpp
	src/sqlitedb.cpp
	src/sqlitetablemodel.cpp
	src/sqlitetypes.cpp
	src/sqltextedit.cpp
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
)

set(SQLB_FORMS
	src/AboutDialog.ui
	src/CreateIndexDialog.ui
	src/EditDialog.ui
	src/EditTableDialog.ui
	src/ExportCsvDialog.ui
	src/ImportCsvDialog.ui
	src/MainWindow.ui
	src/PreferencesDialog.ui
	src/SqlExecutionArea.ui
	src/VacuumDialog.ui
	src/CipherDialog.ui
	src/ExportSqlDialog.ui
	src/ColumnDisplayFormatDialog.ui
)

set(SQLB_RESOURCES
	src/icons/icons.qrc
	src/translations/flags/flags.qrc
	src/translations/translations.qrc
)

set(SQLB_MISC
    src/grammar/sqlite3.g
)

# Translation files
set(SQLB_TSS
	"${CMAKE_SOURCE_DIR}/src/translations/sqlb_zh.ts"
	"${CMAKE_SOURCE_DIR}/src/translations/sqlb_zh_TW.ts"
	"${CMAKE_SOURCE_DIR}/src/translations/sqlb_de.ts"
	"${CMAKE_SOURCE_DIR}/src/translations/sqlb_fr.ts"
	"${CMAKE_SOURCE_DIR}/src/translations/sqlb_ru.ts"
	"${CMAKE_SOURCE_DIR}/src/translations/sqlb_pt_BR.ts"
	"${CMAKE_SOURCE_DIR}/src/translations/sqlb_en_GB.ts"
	"${CMAKE_SOURCE_DIR}/src/translations/sqlb_ko_KR.ts"
)

if(USE_QT5)
    qt5_wrap_ui(SQLB_FORM_HDR ${SQLB_FORMS})
    if(SQLB_TSS)
        # add translations
        foreach(SQLB_TS ${SQLB_TSS})
            SET_SOURCE_FILES_PROPERTIES("${SQLB_TS}" PROPERTIES OUTPUT_LOCATION "${CMAKE_SOURCE_DIR}/src/translations")
        endforeach(SQLB_TS ${SQLB_TSS})
        qt5_add_translation(SQLB_QMS ${SQLB_TSS})
    endif(SQLB_TSS)
    qt5_add_resources(SQLB_RESOURCES_RCC ${SQLB_RESOURCES})
else()
    QT4_WRAP_CPP(SQLB_MOC ${SQLB_MOC_HDR})
    QT4_WRAP_UI(SQLB_FORM_HDR ${SQLB_FORMS})
    if(SQLB_TSS)
        # add translations
        foreach(SQLB_TS ${SQLB_TSS})
            SET_SOURCE_FILES_PROPERTIES("${SQLB_TS}" PROPERTIES OUTPUT_LOCATION "${CMAKE_SOURCE_DIR}/src/translations")
        endforeach(SQLB_TS ${SQLB_TSS})
        QT4_ADD_TRANSLATION(SQLB_QMS ${SQLB_TSS})
    endif(SQLB_TSS)
    QT4_ADD_RESOURCES(SQLB_RESOURCES_RCC ${SQLB_RESOURCES})
endif()


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
	find_file(SQLITE3_DLL sqlite3.dll)
	include_directories(${SQLITE3_INCLUDE_DIR})
endif()

include_directories(
		"${CMAKE_CURRENT_BINARY_DIR}"
		${QHEXEDIT_DIR}
		${QCUSTOMPLOT_DIR}
		${QSCINTILLA_DIR}
		${ADDITIONAL_INCLUDE_PATHS}
		src)
if(ANTLR2_FOUND)
    include_directories(${ANTLR2_INCLUDE_DIRS})
else()
    include_directories(${ANTLR_DIR})
endif()

add_executable(${PROJECT_NAME}
		${SQLB_HDR}
		${SQLB_SRC}
		${SQLB_FORM_HDR}
		${SQLB_MOC}
		${SQLB_RESOURCES_RCC}
		${SQLB_MISC})

if(USE_QT5)
    qt5_use_modules(${PROJECT_NAME} Gui Widgets Network Test PrintSupport)
    set(QT_LIBRARIES "")
endif()
add_dependencies(${PROJECT_NAME} qhexedit qcustomplot qscintilla2)
if(NOT ANTLR2_FOUND)
    add_dependencies(${PROJECT_NAME} antlr)
endif()

link_directories(
		"${CMAKE_CURRENT_BINARY_DIR}/${ANTLR_DIR}"
		"${CMAKE_CURRENT_BINARY_DIR}/${QHEXEDIT_DIR}"
		"${CMAKE_CURRENT_BINARY_DIR}/${QCUSTOMPLOT_DIR}"
		"${CMAKE_CURRENT_BINARY_DIR}/${QSCINTILLA_DIR}")
if(NOT ANTLR2_FOUND)
    link_directories("${CMAKE_CURRENT_BINARY_DIR}/${ANTLR_DIR}")
endif()

target_link_libraries(${PROJECT_NAME}
		qhexedit
		qcustomplot
		qscintilla2
		${QT_LIBRARIES}
		${WIN32_STATIC_LINK}
		${LIBSQLITE}
		${ADDITIONAL_LIBS})
if(ANTLR2_FOUND)
    target_link_libraries(${PROJECT_NAME} ${ANTLR2_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} antlr)
endif()

if(WIN32 AND MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
	set(CMAKE_GENERATOR_TOOLSET "v120_xp" CACHE STRING "Platform Toolset" FORCE)
endif()

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)
	

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
	set(QT5_BIN_PATH ${QT5_PATH}/bin)
	install(FILES
			${QT5_BIN_PATH}/Qt5Core.dll
			${QT5_BIN_PATH}/Qt5Gui.dll
			${QT5_BIN_PATH}/Qt5Network.dll
			${QT5_BIN_PATH}/Qt5PrintSupport.dll
			${QT5_BIN_PATH}/Qt5Widgets.dll
			${QT5_BIN_PATH}/icudt54.dll
			${QT5_BIN_PATH}/icuin54.dll
			${QT5_BIN_PATH}/icuuc54.dll
			${SQLITE3_DLL}
			${OPENSSL_PATH}/libeay32.dll
			${OPENSSL_PATH}/ssleay32.dll
		DESTINATION bin)
	install(FILES
		${QT5_PATH}/plugins/platforms/qwindows.dll
		DESTINATION bin/platforms)

	install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/deps/${VSREDIST} DESTINATION tmp)
endif()

#cpack
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "DB Browser for SQLite")
set(CPACK_PACKAGE_VENDOR "oldsch00l")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "3")
set(CPACK_PACKAGE_VERSION_MINOR "8")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "SqliteBrowser${CPACK_PACKAGE_VERSION_MAJOR}")
if(WIN32 AND NOT UNIX)
	# There is a bug in NSI that does not handle full unix paths properly. Make
	# sure there is at least one set of four (4) backlasshes.
	set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\src\\\\iconwin.ico")
	set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\sqlitebrowser.exe")
	set(CPACK_NSIS_DISPLAY_NAME "DB Browser for SQLite")
	set(CPACK_NSIS_HELP_LINK "https:\\\\\\\\github.com\\\\sqlitebrowser\\\\sqlitebrowser")
	set(CPACK_NSIS_URL_INFO_ABOUT "https:\\\\\\\\github.com\\\\sqlitebrowser\\\\sqlitebrowser")
	set(CPACK_NSIS_CONTACT "peinthor@gmail.com")
	set(CPACK_NSIS_MODIFY_PATH OFF)
	set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
	set(CPACK_NSIS_MUI_FINISHPAGE_RUN "sqlitebrowser.exe")

	# VS redist
	list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
               ExecWait '$INSTDIR\\\\tmp\\\\${VSREDIST} /install /passive /quiet'
               Delete '$INSTDIR\\\\tmp\\\\${VSREDIST}'
               ")
else(WIN32 AND NOT UNIX)
	set(CPACK_STRIP_FILES "bin/sqlitebrowser")
	set(CPACK_SOURCE_STRIP_FILES "")
endif(WIN32 AND NOT UNIX)
set(CPACK_PACKAGE_EXECUTABLES "sqlitebrowser" "SqliteBrowser")
include(CPack)
