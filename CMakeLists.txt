project(sqlitebrowser)
cmake_minimum_required(VERSION 2.6)

option(UNITTEST OFF "Build unittest")

set(ANTLR_DIR libs/antlr-2.7.7)
set(QHEXEDIT_DIR libs/qhexedit)

add_subdirectory(${ANTLR_DIR})
add_subdirectory(${QHEXEDIT_DIR})

find_package(Qt4 COMPONENTS QtCore QtGui QtTest REQUIRED)

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
	add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/src/gen_version.h COMMAND ${CMAKE_SOURCE_DIR}/src/version.sh ${CMAKE_SOURCE_DIR}/src DEPENDS .git/HEAD)
endif(EXISTS ${CMAKE_SOURCE_DIR}/.git)
set_source_files_properties(src/AboutDialog.cpp PROPERTIES OBJECT_DEPENDS ${CMAKE_SOURCE_DIR}/src/gen_version.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${ANTLR_DIR} ${QHEXEDIT_DIR} src)

add_executable(${PROJECT_NAME} ${SQLB_HDR} ${SQLB_SRC} ${SQLB_FORM_HDR} ${SQLB_MOC} ${SQLB_RESOURCES_RCC})

add_dependencies(${PROJECT_NAME} antlr qhexedit)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/${ANTLR_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${QHEXEDIT_DIR})

target_link_libraries(${PROJECT_NAME} antlr qhexedit ${QT_LIBRARIES} sqlite3)
