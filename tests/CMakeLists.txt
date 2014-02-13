project(sqlb-unittests)
cmake_minimum_required(VERSION 2.8.9)

OPTION(USE_QT5 FALSE "Build with qt5")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(ANTLR_DIR ../libs/antlr-2.7.7)
add_subdirectory(${ANTLR_DIR} ${CMAKE_CURRENT_BINARY_DIR}/antlr)

if(USE_QT5)
    find_package(Qt5Widgets REQUIRED)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
else()
    find_package(Qt4 COMPONENTS QtCore QtTest REQUIRED)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS})
endif()

set(SQLB_SRC
	../src/sqlitetypes.cpp
	../src/grammar/Sqlite3Lexer.cpp
	../src/grammar/Sqlite3Parser.cpp
	../src/tests/testsqlobjects.cpp)

set(SQLB_HDR
	../src/grammar/sqlite3TokenTypes.hpp
	../src/grammar/Sqlite3Lexer.hpp
	../src/grammar/Sqlite3Parser.hpp
	../src/sqlitetypes.h)

set(SQLB_MOC_HDR
	../src/tests/testsqlobjects.h)

if(NOT USE_QT5)
    QT4_WRAP_CPP(SQLB_MOC ${SQLB_MOC_HDR})
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/${ANTLR_DIR})

add_executable(${PROJECT_NAME} ${SQLB_MOC} ${SQLB_HDR} ${SQLB_SRC})

if(USE_QT5)
    qt5_use_modules(${PROJECT_NAME} Test)
    set(QT_LIBRARIES "")
endif()

add_dependencies(${PROJECT_NAME} antlr)
target_link_libraries(${PROJECT_NAME} antlr ${QT_LIBRARIES})

MESSAGE(STATUS "Build type is ${CMAKE_BUILD_TYPE}")
