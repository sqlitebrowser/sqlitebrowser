cmake_minimum_required(VERSION 2.8.12.2)

# Disable AUTOMOC because it cannot be made to work with QScintilla
set(CMAKE_AUTOMOC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS PrintSupport Widgets)

if(APPLE)
    find_package(Qt5 REQUIRED COMPONENTS MacExtras)
endif()

set(QSCINTILLA_SRC
    qsciscintilla.cpp
    qsciscintillabase.cpp
    qsciabstractapis.cpp
    qsciapis.cpp
    qscicommand.cpp
    qscicommandset.cpp
    qscidocument.cpp
    qscilexer.cpp
    qscilexercustom.cpp
    qscilexersql.cpp
    qscilexerjson.cpp
    qscilexerhtml.cpp
    qscilexerxml.cpp
    qscilexerjavascript.cpp
    qscilexercpp.cpp
    qscilexerpython.cpp
    qscimacro.cpp
    qsciprinter.cpp
    qscistyle.cpp
    qscistyledtext.cpp
    MacPasteboardMime.cpp
    InputMethod.cpp
    SciClasses.cpp
    ListBoxQt.cpp
    PlatQt.cpp
    ScintillaQt.cpp
    SciAccessibility.cpp
    ../scintilla/lexers/LexSQL.cpp
    ../scintilla/lexers/LexJSON.cpp
    ../scintilla/lexers/LexHTML.cpp
    ../scintilla/lexlib/Accessor.cpp
    ../scintilla/lexlib/CharacterCategory.cpp
    ../scintilla/lexlib/CharacterSet.cpp
    ../scintilla/lexlib/DefaultLexer.cpp
    ../scintilla/lexlib/LexerBase.cpp
    ../scintilla/lexlib/LexerModule.cpp
    ../scintilla/lexlib/LexerNoExceptions.cpp
    ../scintilla/lexlib/LexerSimple.cpp
    ../scintilla/lexlib/PropSetSimple.cpp
    ../scintilla/lexlib/StyleContext.cpp
    ../scintilla/lexlib/WordList.cpp
    ../scintilla/src/AutoComplete.cpp
    ../scintilla/src/CallTip.cpp
    ../scintilla/src/CaseConvert.cpp
    ../scintilla/src/CaseFolder.cpp
    ../scintilla/src/Catalogue.cpp
    ../scintilla/src/CellBuffer.cpp
    ../scintilla/src/CharClassify.cpp
    ../scintilla/src/ContractionState.cpp
    ../scintilla/src/DBCS.cpp
    ../scintilla/src/Decoration.cpp
    ../scintilla/src/Document.cpp
    ../scintilla/src/Editor.cpp
    ../scintilla/src/EditModel.cpp
    ../scintilla/src/EditView.cpp
    ../scintilla/src/ExternalLexer.cpp
    ../scintilla/src/Indicator.cpp
    ../scintilla/src/KeyMap.cpp
    ../scintilla/src/LineMarker.cpp
    ../scintilla/src/MarginView.cpp
    ../scintilla/src/PerLine.cpp
    ../scintilla/src/PositionCache.cpp
    ../scintilla/src/RESearch.cpp
    ../scintilla/src/RunStyles.cpp
    ../scintilla/src/ScintillaBase.cpp
    ../scintilla/src/Selection.cpp
    ../scintilla/src/Style.cpp
    ../scintilla/src/UniConversion.cpp
    ../scintilla/src/ViewStyle.cpp
    ../scintilla/src/XPM.cpp
)

set(QSCINTILLA_HDR
    Qsci/qsciglobal.h
    Qsci/qscicommand.h
    Qsci/qscicommandset.h
    Qsci/qscidocument.h
    Qsci/qsciprinter.h
    Qsci/qscistyle.h
    Qsci/qscistyledtext.h
    ListBoxQt.h
    SciAccessibility.h
    ../scintilla/include/ILexer.h
    ../scintilla/include/ILoader.h
    ../scintilla/include/Platform.h
    ../scintilla/include/SciLexer.h
    ../scintilla/include/Scintilla.h
    ../scintilla/include/ScintillaWidget.h
    ../scintilla/include/Sci_Position.h
    ../scintilla/lexlib/Accessor.h
    ../scintilla/lexlib/CharacterCategory.h
    ../scintilla/lexlib/CharacterSet.h
    ../scintilla/lexlib/DefaultLexer.h
    ../scintilla/lexlib/LexAccessor.h
    ../scintilla/lexlib/LexerBase.h
    ../scintilla/lexlib/LexerModule.h
    ../scintilla/lexlib/LexerNoExceptions.h
    ../scintilla/lexlib/LexerSimple.h
    ../scintilla/lexlib/OptionSet.h
    ../scintilla/lexlib/PropSetSimple.h
    ../scintilla/lexlib/SparseState.h
    ../scintilla/lexlib/StringCopy.h
    ../scintilla/lexlib/StyleContext.h
    ../scintilla/lexlib/SubStyles.h
    ../scintilla/lexlib/WordList.h
    ../scintilla/src/AutoComplete.h
    ../scintilla/src/CallTip.h
    ../scintilla/src/CaseConvert.h
    ../scintilla/src/CaseFolder.h
    ../scintilla/src/Catalogue.h
    ../scintilla/src/CellBuffer.h
    ../scintilla/src/CharClassify.h
    ../scintilla/src/ContractionState.h
    ../scintilla/src/DBCS.h
    ../scintilla/src/Decoration.h
    ../scintilla/src/Document.h
    ../scintilla/src/EditModel.h
    ../scintilla/src/Editor.h
    ../scintilla/src/EditView.h
    ../scintilla/src/ElapsedPeriod.h
    ../scintilla/src/ExternalLexer.h
    ../scintilla/src/FontQuality.h
    ../scintilla/src/Indicator.h
    ../scintilla/src/IntegerRectangle.h
    ../scintilla/src/KeyMap.h
    ../scintilla/src/LineMarker.h
    ../scintilla/src/MarginView.h
    ../scintilla/src/Partitioning.h
    ../scintilla/src/PerLine.h
    ../scintilla/src/PositionCache.h
    ../scintilla/src/Position.h
    ../scintilla/src/RESearch.h
    ../scintilla/src/RunStyles.h
    ../scintilla/src/ScintillaBase.h
    ../scintilla/src/Selection.h
    ../scintilla/src/SparseVector.h
    ../scintilla/src/SplitVector.h
    ../scintilla/src/Style.h
    ../scintilla/src/UniConversion.h
    ../scintilla/src/UniqueString.h
    ../scintilla/src/ViewStyle.h
    ../scintilla/src/XPM.h
)

set(QSCINTILLA_MOC_HDR
    Qsci/qsciscintilla.h
    Qsci/qsciscintillabase.h
    Qsci/qsciabstractapis.h
    Qsci/qsciapis.h
    Qsci/qscilexer.h
    Qsci/qscilexercustom.h
    Qsci/qscilexersql.h
    Qsci/qscilexerjson.h
    Qsci/qscilexerhtml.h
    Qsci/qscilexerxml.h
    Qsci/qscilexerjavascript.h
    Qsci/qscilexercpp.h
    Qsci/qscilexerpython.h
    Qsci/qscimacro.h
    SciClasses.h
    ScintillaQt.h
)

qt5_wrap_cpp(QSCINTILLA_WRAP_MOC_HDR ${QSCINTILLA_MOC_HDR})

add_library(qscintilla2 ${QSCINTILLA_SRC} ${QSCINTILLA_HDR} ${QSCINTILLA_MOC_HDR} ${QSCINTILLA_MOC} ${QSCINTILLA_WRAP_MOC_HDR})
target_include_directories(qscintilla2 PRIVATE ../scintilla/include ../scintilla/lexlib ../scintilla/src)
target_include_directories(qscintilla2 INTERFACE .)

target_compile_definitions(qscintilla2 PRIVATE SCINTILLA_QT)
target_compile_definitions(qscintilla2 PRIVATE DSCI_LEXER)

target_link_libraries(qscintilla2 Qt5::Widgets Qt5::PrintSupport)

if (APPLE)
    target_link_libraries(qscintilla2 Qt5::MacExtras)
endif()

add_library(QScintilla::QScintilla ALIAS qscintilla2)
