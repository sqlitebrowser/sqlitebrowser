project(sqlitebrowser)
cmake_minimum_required(VERSION 2.8.7)

OPTION(USE_QT5 FALSE "Build with qt5")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(ANTLR_DIR libs/antlr-2.7.7)
set(QHEXEDIT_DIR libs/qhexedit)
set(QCUSTOMPLOT_DIR libs/qcustomplot-source)

add_subdirectory(${ANTLR_DIR})
add_subdirectory(${QHEXEDIT_DIR})
add_subdirectory(${QCUSTOMPLOT_DIR})

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

set(SQLB_HDR
	src/gen_version.h
	src/sqlitetypes.h
    src/csvparser.h
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
	src/SQLiteSyntaxHighlighter.h
	src/SqlExecutionArea.h
	src/VacuumDialog.h
	src/sqlitetablemodel.h
	src/sqltextedit.h
	src/DbStructureModel.h
	src/Application.h
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
    src/csvparser.cpp
	src/DbStructureModel.cpp
	src/grammar/Sqlite3Lexer.cpp
	src/grammar/Sqlite3Parser.cpp
	src/main.cpp
	src/Application.cpp
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
)

set(SQLB_RESOURCES
	src/icons/icons.qrc
)

# Translation files
set(SQLB_TSS
	"${CMAKE_SOURCE_DIR}/src/translations/sqlb_de.ts"
	"${CMAKE_SOURCE_DIR}/src/translations/sqlb_ru.ts"
	"${CMAKE_SOURCE_DIR}/src/translations/sqlb_fr.ts"
)

if(USE_QT5)
    qt5_wrap_ui(SQLB_FORM_HDR ${SQLB_FORMS})
    qt5_add_resources(SQLB_RESOURCES_RCC ${SQLB_RESOURCES})
    if(SQLB_TSS)
        # add translations
        foreach(SQLB_TS ${SQLB_TSS})
            SET_SOURCE_FILES_PROPERTIES("${SQLB_TS}" PROPERTIES OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/translations")
        endforeach(SQLB_TS ${SQLB_TSS})
        qt5_add_translation(SQLB_QMS ${SQLB_TSS})
    endif(SQLB_TSS)
else()
    QT4_WRAP_CPP(SQLB_MOC ${SQLB_MOC_HDR})
    QT4_WRAP_UI(SQLB_FORM_HDR ${SQLB_FORMS})
    QT4_ADD_RESOURCES(SQLB_RESOURCES_RCC ${SQLB_RESOURCES})
    if(SQLB_TSS)
        # add translations
        foreach(SQLB_TS ${SQLB_TSS})
            SET_SOURCE_FILES_PROPERTIES("${SQLB_TS}" PROPERTIES OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/translations")
        endforeach(SQLB_TS ${SQLB_TSS})
        QT4_ADD_TRANSLATION(SQLB_QMS ${SQLB_TSS})
    endif(SQLB_TSS)
endif()

set(gv "${CMAKE_SOURCE_DIR}/src/gen_version.h")

# get git version hash
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
	add_custom_command(OUTPUT "${gv}"
        COMMAND echo "#ifndef GEN_VERSION_H" > "${gv}"
        COMMAND echo "#define GEN_VERSION_H" >> "${gv}"
        COMMAND git log -n1 "--format=#define APP_VERSION \"%h_git\"" >> "${gv}"
        COMMAND echo "#define MAJOR_VERSION 999" >> "${gv}"
        COMMAND echo "#define MINOR_VERSION 0" >> "${gv}"
        COMMAND echo "#define PATCH_VERSION 0" >> "${gv}"
        COMMAND echo "#endif" >> "${gv}"
        DEPENDS .git/HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        VERBATIM)
else()
	file(WRITE "${gv}"
		 "#ifndef GEN_VERSION_H\n"
		 "#define GEN_VERSION_H\n"
		 "#define APP_VERSION \"999.0.0\"\n"
		 "#define MAJOR_VERSION 999\n"
		 "#define MINOR_VERSION 0\n"
		 "#define PATCH_VERSION 0\n"
		 "#endif\n")
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
		set(ADDITIONAL_LIBS lcms lzma)
	ELSE( MINGW )
		set(SQLB_SRC ${SQLB_SRC} "${CMAKE_CURRENT_SOURCE_DIR}src/winapp.rc")
	ENDIF( MINGW )
endif(WIN32)

#enable version check for MacOS
if(APPLE)
	add_definitions(-DCHECKNEWVERSION)
endif(APPLE)

# add extra library path for MacOS and FreeBSD
set(EXTRAPATH APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
if(EXTRAPATH) 
	find_library(LIBSQLITE sqlite3 HINTS /usr/local/lib /usr/local/opt/sqlite/lib)
	set(ADDITIONAL_INCLUDE_PATHS /usr/local/include /usr/local/opt/sqlite/include)
else(EXTRAPATH)
	find_library(LIBSQLITE sqlite3)
endif(EXTRAPATH)

include_directories(
		"${CMAKE_CURRENT_BINARY_DIR}"
		${ANTLR_DIR}
		${QHEXEDIT_DIR}
		${QCUSTOMPLOT_DIR}
		${ADDITIONAL_INCLUDE_PATHS}
		src)

add_executable(${PROJECT_NAME}
		${SQLB_HDR}
		${SQLB_SRC}
		${SQLB_FORM_HDR}
		${SQLB_MOC}
		${SQLB_RESOURCES_RCC}
		${SQLB_QMS})

if(USE_QT5)
    qt5_use_modules(${PROJECT_NAME} Gui Widgets Network Test PrintSupport)
    set(QT_LIBRARIES "")
endif()
add_dependencies(${PROJECT_NAME} antlr qhexedit qcustomplot)

link_directories(
		"${CMAKE_CURRENT_BINARY_DIR}/${ANTLR_DIR}"
		"${CMAKE_CURRENT_BINARY_DIR}/${QHEXEDIT_DIR}"
		"${CMAKE_CURRENT_BINARY_DIR}/${QCUSTOMPLOT_DIR}")

target_link_libraries(${PROJECT_NAME}
		antlr
		qhexedit
		qcustomplot
		${QT_LIBRARIES}
		${WIN32_STATIC_LINK}
		${LIBSQLITE}
		${ADDITIONAL_LIBS})

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)

#cpack
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Database Browser for SQLite")
set(CPACK_PACKAGE_VENDOR "oldsch00l")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
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
	set(CPACK_NSIS_DISPLAY_NAME "Database Browser for SQLite")
	set(CPACK_NSIS_HELP_LINK "https:\\\\\\\\github.com\\\\sqlitebrowser\\\\sqlitebrowser")
	set(CPACK_NSIS_URL_INFO_ABOUT "https:\\\\\\\\github.com\\\\sqlitebrowser\\\\sqlitebrowser")
	set(CPACK_NSIS_CONTACT "peinthor@gmail.com")
	set(CPACK_NSIS_MODIFY_PATH OFF)
	set(CPACK_NSIS_MUI_FINISHPAGE_RUN "sqlitebrowser.exe")
else(WIN32 AND NOT UNIX)
	set(CPACK_STRIP_FILES "bin/sqlitebrowser")
	set(CPACK_SOURCE_STRIP_FILES "")
endif(WIN32 AND NOT UNIX)
set(CPACK_PACKAGE_EXECUTABLES "sqlitebrowser" "SqliteBrowser")
include(CPack)
