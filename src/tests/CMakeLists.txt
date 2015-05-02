if(USE_QT5)
else()
    find_package(Qt4 COMPONENTS QtTest REQUIRED)
    include("${QT_USE_FILE}")
    add_definitions(${QT_DEFINITIONS})
endif()

set(SQLB_UNITTESTS_SRC
	../sqlitedb.cpp
	../sqlitetablemodel.cpp
	../sqlitetypes.cpp
	../csvparser.cpp
	../grammar/Sqlite3Lexer.cpp
	../grammar/Sqlite3Parser.cpp
	../PreferencesDialog.cpp
	TestImport.cpp
	testsqlobjects.cpp
	TestMain.cpp
	../FileDialog.cpp
)

set(SQLB_UNITTESTS_HDR
	../grammar/sqlite3TokenTypes.hpp
	../grammar/Sqlite3Lexer.hpp
	../grammar/Sqlite3Parser.hpp
	../csvparser.h
	../sqlitetypes.h)

set(SQLB_UNITTESTS_FORMS
	../PreferencesDialog.ui)

set(SQLB_UNITTESTS_MOC_HDR
	../sqlitedb.h
	../sqlitetablemodel.h
	../PreferencesDialog.h
	TestImport.h
	testsqlobjects.h
	../FileDialog.h
)

if(sqlcipher)
    list(APPEND SQLB_UNITTESTS_SRC ../CipherDialog.cpp)
    list(APPEND SQLB_UNITTESTS_FORMS ../CipherDialog.ui)
    list(APPEND SQLB_UNITTESTS_MOC_HDR ../CipherDialog.h)
endif()

if(USE_QT5)
    QT5_WRAP_UI(SQLB_UNITTESTS_FORM_HDR ${SQLB_UNITTESTS_FORMS})
else()
    QT4_WRAP_CPP(SQLB_UNITTESTS_MOC ${SQLB_UNITTESTS_MOC_HDR})
    QT4_WRAP_UI(SQLB_UNITTESTS_FORM_HDR ${SQLB_UNITTESTS_FORMS})
endif()

include_directories("${CMAKE_CURRENT_BINARY_DIR}" "${ANTLR_DIR}" ..)

add_executable(sqlb-unittests ${SQLB_UNITTESTS_MOC} ${SQLB_UNITTESTS_HDR} ${SQLB_UNITTESTS_SRC} ${SQLB_UNITTESTS_FORM_HDR})

if(USE_QT5)
    qt5_use_modules(sqlb-unittests Test Widgets Gui)
    set(QT_LIBRARIES "")
endif()

add_dependencies(sqlb-unittests antlr)
target_link_libraries(sqlb-unittests antlr ${QT_LIBRARIES} ${LIBSQLITE})
add_test(sqlb-unittests sqlb-unittests)
