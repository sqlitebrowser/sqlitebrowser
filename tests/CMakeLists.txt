project(sqlb-unittests)
cmake_minimum_required(VERSION 2.8.9)

OPTION(USE_QT5 FALSE "Build with qt5")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(ANTLR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../libs/antlr-2.7.7")
add_subdirectory("${ANTLR_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/antlr")

if(USE_QT5)
    find_package(Qt5Widgets REQUIRED)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
else()
    find_package(Qt4 COMPONENTS QtCore QtTest QtGui REQUIRED)
    include("${QT_USE_FILE}")
    add_definitions(${QT_DEFINITIONS})
endif()

# add extra library path for MacOS
if(APPLE)
    find_library(LIBSQLITE sqlite3 HINTS /usr/local/lib /usr/local/opt/sqlite/lib)
    set(ADDITIONAL_INCLUDE_PATHS /usr/local/include /usr/local/opt/sqlite/include)
else(APPLE)
    find_library(LIBSQLITE sqlite3)
endif(APPLE)

set(SQLB_SRC
	../src/sqlitedb.cpp
	../src/sqlitetablemodel.cpp
	../src/sqlitetypes.cpp
    ../src/csvparser.cpp
	../src/grammar/Sqlite3Lexer.cpp
	../src/grammar/Sqlite3Parser.cpp
	../src/tests/TestImport.cpp
	../src/tests/testsqlobjects.cpp
	../src/tests/TestMain.cpp)

set(SQLB_HDR
	../src/grammar/sqlite3TokenTypes.hpp
	../src/grammar/Sqlite3Lexer.hpp
	../src/grammar/Sqlite3Parser.hpp
    ../src/csvparser.h
	../src/sqlitetypes.h)

set(SQLB_MOC_HDR
	../src/sqlitedb.h
	../src/sqlitetablemodel.h
	../src/tests/TestImport.h
	../src/tests/testsqlobjects.h)

if(NOT USE_QT5)
    QT4_WRAP_CPP(SQLB_MOC ${SQLB_MOC_HDR})
endif()

include_directories("${ANTLR_DIR}" ../src)

add_executable(${PROJECT_NAME} ${SQLB_MOC} ${SQLB_HDR} ${SQLB_SRC})

if(USE_QT5)
    qt5_use_modules(${PROJECT_NAME} Test Widgets Gui)
    set(QT_LIBRARIES "")
endif()

add_dependencies(${PROJECT_NAME} antlr)
target_link_libraries(${PROJECT_NAME} antlr ${QT_LIBRARIES} ${LIBSQLITE})

MESSAGE(STATUS "Build type is ${CMAKE_BUILD_TYPE}")
