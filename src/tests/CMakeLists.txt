if(USE_QT5)
else()
    find_package(Qt4 COMPONENTS QtTest REQUIRED)
    include("${QT_USE_FILE}")
    add_definitions(${QT_DEFINITIONS})
endif()

include_directories("${CMAKE_CURRENT_BINARY_DIR}" ..)

# test-sqlobjects

set(TESTSQLOBJECTS_SRC
	../sqlitedb.cpp
	../sqlitetablemodel.cpp
	../sqlitetypes.cpp
	../csvparser.cpp
	../grammar/Sqlite3Lexer.cpp
	../grammar/Sqlite3Parser.cpp
	../PreferencesDialog.cpp
	testsqlobjects.cpp
	../FileDialog.cpp
	../sqltextedit.cpp
	../SqlUiLexer.cpp
)

set(TESTSQLOBJECTS_HDR
	../grammar/sqlite3TokenTypes.hpp
	../grammar/Sqlite3Lexer.hpp
	../grammar/Sqlite3Parser.hpp
	../sqlitetypes.h)

set(TESTSQLOBJECTS_FORMS
	../PreferencesDialog.ui)

set(TESTSQLOBJECTS_MOC_HDR
	../sqlitedb.h
	../sqlitetablemodel.h
	../PreferencesDialog.h
	testsqlobjects.h
	../FileDialog.h
	../sqltextedit.h
	../SqlUiLexer.h
)

if(sqlcipher)
    list(APPEND TESTSQLOBJECTS_SRC ../CipherDialog.cpp)
    list(APPEND TESTSQLOBJECTS_FORMS ../CipherDialog.ui)
    list(APPEND TESTSQLOBJECTS_MOC_HDR ../CipherDialog.h)
endif()

if(USE_QT5)
    QT5_WRAP_UI(TESTSQLOBJECTS_FORM_HDR ${TESTSQLOBJECTS_FORMS})
else()
    QT4_WRAP_CPP(TESTSQLOBJECTS_MOC ${TESTSQLOBJECTS_MOC_HDR})
    QT4_WRAP_UI(TESTSQLOBJECTS_FORM_HDR ${TESTSQLOBJECTS_FORMS})
endif()

add_executable(test-sqlobjects ${TESTSQLOBJECTS_MOC} ${TESTSQLOBJECTS_HDR} ${TESTSQLOBJECTS_SRC} ${TESTSQLOBJECTS_FORM_HDR})

if(USE_QT5)
    qt5_use_modules(test-sqlobjects Test Widgets Gui)
    set(QT_LIBRARIES "")
endif()

if(ANTLR2_FOUND)
else()
    add_dependencies(test-sqlobjects antlr)
endif()
target_link_libraries(test-sqlobjects ${QT_LIBRARIES} ${LIBSQLITE})
if(ANTLR2_FOUND)
    target_link_libraries(test-sqlobjects ${ANTLR2_LIBRARIES})
else()
    target_link_libraries(test-sqlobjects antlr)
endif()
link_directories("${CMAKE_CURRENT_BINARY_DIR}/${QSCINTILLA_DIR}")
add_dependencies(test-sqlobjects qscintilla2)
target_link_libraries(test-sqlobjects
		qscintilla2)
add_test(test-sqlobjects test-sqlobjects)

# test-import

set(TESTIMPORT_SRC
	../csvparser.cpp
	TestImport.cpp
)

set(TESTIMPORT_MOC_HDR
	TestImport.h
)

if(USE_QT5)
else()
    QT4_WRAP_CPP(TESTIMPORT_MOC ${TESTIMPORT_MOC_HDR})
endif()

add_executable(test-import ${TESTIMPORT_MOC} ${TESTIMPORT_SRC})

if(USE_QT5)
    qt5_use_modules(test-import Test Core)
    set(QT_LIBRARIES "")
endif()

target_link_libraries(test-import ${QT_LIBRARIES})
add_test(test-import test-import)
