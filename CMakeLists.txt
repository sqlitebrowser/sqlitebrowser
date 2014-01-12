project(sqlitebrowser)
cmake_minimum_required(VERSION 2.6)

option(UNITTEST OFF "Build unittest")

set(ANTLR_DIR libs/antlr-2.7.7)
set(QHEXEDIT_DIR libs/qhexedit)

add_subdirectory(${ANTLR_DIR})
add_subdirectory(${QHEXEDIT_DIR})

find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork QtTest REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(SQLB_HDR
	src/gen_version.h
	src/sqlitedb.h
	src/sqlitetypes.h
)

set(SQLB_MOC_HDR
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
	src/SQLiteSyntaxHighlighter.h
	src/SqlExecutionArea.h
	src/VacuumDialog.h
	src/sqlitetablemodel.h
	src/sqltextedit.h
	src/DbStructureModel.h
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
	src/SQLiteSyntaxHighlighter.cpp
	src/SqlExecutionArea.cpp
	src/VacuumDialog.cpp
	src/sqlitedb.cpp
	src/sqlitetablemodel.cpp
	src/sqlitetypes.cpp
	src/sqltextedit.cpp
	src/DbStructureModel.cpp
	src/grammar/Sqlite3Lexer.cpp
	src/grammar/Sqlite3Parser.cpp
)

if(UNITTEST)
	set(SQLB_SRC ${SQLB_SRC} src/tests/testsqlobjects.cpp)
	set(SQLB_MOC_HDR ${SQLB_MOC_HDR} src/tests/testsqlobjects.h)
else(UNITTEST)
	set(SQLB_SRC ${SQLB_SRC} src/main.cpp)
endif(UNITTEST)

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
)

set(SQLB_RESOURCES
	src/icons/icons.qrc
)

QT4_WRAP_CPP(SQLB_MOC ${SQLB_MOC_HDR})
QT4_WRAP_UI(SQLB_FORM_HDR ${SQLB_FORMS})
QT4_ADD_RESOURCES(SQLB_RESOURCES_RCC ${SQLB_RESOURCES})

# get git version hash
if(EXISTS ${CMAKE_SOURCE_DIR}/.git)
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/src/gen_version.h
			           COMMAND echo "#ifndef __VERSION_H__" > ${CMAKE_SOURCE_DIR}/src/gen_version.h
					   COMMAND echo "#define __VERSION_H__" >> ${CMAKE_SOURCE_DIR}/src/gen_version.h
					   COMMAND git log -n1 "--format=#define APP_VERSION \"%h_git\"" >> ${CMAKE_SOURCE_DIR}/src/gen_version.h
					   COMMAND echo "#define MAJOR_VERSION 999" >> ${CMAKE_SOURCE_DIR}/src/gen_version.h
					   COMMAND echo "#define MINOR_VERSION 0" >> ${CMAKE_SOURCE_DIR}/src/gen_version.h
					   COMMAND echo "#define PATCH_VERSION 0" >> ${CMAKE_SOURCE_DIR}/src/gen_version.h
					   COMMAND echo "#endif" >> ${CMAKE_SOURCE_DIR}/src/gen_version.h
					   DEPENDS .git/HEAD
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   VERBATIM)
else()
	file(WRITE ${CMAKE_SOURCE_DIR}/src/gen_version.h "#ifndef __GEN_VERSIONS_H__\n#define __GEN_VERSIONS_H__\n#define APP_VERSION \"3.0.1\"\n#define MAJOR_VERSION 3\n#define MINOR_VERSION 0\n#define PATCH_VERSION 1\n#endif")
endif()

#icon and correct libs/subsystem for windows
if(WIN32)
        #enable version check for windows
        add_definitions(-DCHECKNEWVERSION)

	IF( MINGW )
	# resource compilation for MinGW
		ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sqlbicon.o
			COMMAND windres -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/src/winapp.rc -o ${CMAKE_CURRENT_BINARY_DIR}/sqlbicon.o )
		set(SQLB_SRC ${SQLB_SRC} ${CMAKE_CURRENT_BINARY_DIR}/sqlbicon.o)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
		set(WIN32_STATIC_LINK -Wl,-Bstatic -lssl -lcrypto -lws2_32)
		set(ADDITIONAL_LIBS lcms lzma)
	ELSE( MINGW )
		set(SQLB_SRC ${SQLB_SRC} ${CMAKE_CURRENT_SOURCE_DIR}src/winapp.rc)
	ENDIF( MINGW )
endif(WIN32)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${ANTLR_DIR} ${QHEXEDIT_DIR} src)

add_executable(${PROJECT_NAME} ${SQLB_HDR} ${SQLB_SRC} ${SQLB_FORM_HDR} ${SQLB_MOC} ${SQLB_RESOURCES_RCC})

add_dependencies(${PROJECT_NAME} antlr qhexedit)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/${ANTLR_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${QHEXEDIT_DIR})

target_link_libraries(${PROJECT_NAME} antlr qhexedit ${QT_LIBRARIES} ${WIN32_STATIC_LINK} sqlite3 ${ADDITIONAL_LIBS})

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)

#cpack
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Sqlite database browser UI")
set(CPACK_PACKAGE_VENDOR "oldsch00l")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.rst")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "3")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "SqliteBrowser${CPACK_PACKAGE_VERSION_MAJOR}")
if(WIN32 AND NOT UNIX)
	# There is a bug in NSI that does not handle full unix paths properly. Make
	# sure there is at least one set of four (4) backlasshes.
	set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\src\\\\iconwin.ico")
	set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\sqlitebrowser.exe")
	set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
	set(CPACK_NSIS_HELP_LINK "https:\\\\\\\\github.com\\\\rp-\\\\sqlitebrowser")
	set(CPACK_NSIS_URL_INFO_ABOUT "https:\\\\\\\\github.com\\\\rp-\\\\sqlitebrowser")
	set(CPACK_NSIS_CONTACT "peinthor@gmail.com")
	set(CPACK_NSIS_MODIFY_PATH ON)
else(WIN32 AND NOT UNIX)
	set(CPACK_STRIP_FILES "bin/sqlitebrowser")
	set(CPACK_SOURCE_STRIP_FILES "")
endif(WIN32 AND NOT UNIX)
set(CPACK_PACKAGE_EXECUTABLES "sqlitebrowser" "SqliteBrowser")
include(CPack)

