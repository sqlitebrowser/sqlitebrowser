cmake_minimum_required(VERSION 2.8.12.2)

find_package(Qt5 REQUIRED COMPONENTS PrintSupport Widgets)

if(APPLE)
    find_package(Qt5 REQUIRED COMPONENTS MacExtras)
endif()

set(SOURCES
    scintilla/lexers/LexHTML.cpp
    scintilla/lexers/LexJSON.cpp
    scintilla/lexers/LexSQL.cpp
    scintilla/lexlib/Accessor.cpp
    scintilla/lexlib/CharacterCategory.cpp
    scintilla/lexlib/CharacterSet.cpp
    scintilla/lexlib/DefaultLexer.cpp
    scintilla/lexlib/LexerBase.cpp
    scintilla/lexlib/LexerModule.cpp
    scintilla/lexlib/LexerNoExceptions.cpp
    scintilla/lexlib/LexerSimple.cpp
    scintilla/lexlib/PropSetSimple.cpp
    scintilla/lexlib/StyleContext.cpp
    scintilla/lexlib/WordList.cpp
    scintilla/src/AutoComplete.cpp
    scintilla/src/CallTip.cpp
    scintilla/src/CaseConvert.cpp
    scintilla/src/CaseFolder.cpp
    scintilla/src/Catalogue.cpp
    scintilla/src/CellBuffer.cpp
    scintilla/src/CharClassify.cpp
    scintilla/src/ContractionState.cpp
    scintilla/src/DBCS.cpp
    scintilla/src/Decoration.cpp
    scintilla/src/Document.cpp
    scintilla/src/EditModel.cpp
    scintilla/src/Editor.cpp
    scintilla/src/EditView.cpp
    scintilla/src/ExternalLexer.cpp
    scintilla/src/Indicator.cpp
    scintilla/src/KeyMap.cpp
    scintilla/src/LineMarker.cpp
    scintilla/src/MarginView.cpp
    scintilla/src/PerLine.cpp
    scintilla/src/PositionCache.cpp
    scintilla/src/RESearch.cpp
    scintilla/src/RunStyles.cpp
    scintilla/src/ScintillaBase.cpp
    scintilla/src/Selection.cpp
    scintilla/src/Style.cpp
    scintilla/src/UniConversion.cpp
    scintilla/src/ViewStyle.cpp
    scintilla/src/XPM.cpp
    src/InputMethod.cpp
    src/ListBoxQt.cpp
    src/MacPasteboardMime.cpp
    src/PlatQt.cpp
    src/qsciabstractapis.cpp
    src/qsciapis.cpp
    src/qscicommand.cpp
    src/qscicommandset.cpp
    src/qscidocument.cpp
    src/qscilexer.cpp
    src/qscilexercpp.cpp
    src/qscilexercustom.cpp
    src/qscilexerhtml.cpp
    src/qscilexerjavascript.cpp
    src/qscilexerjson.cpp
    src/qscilexerpython.cpp
    src/qscilexersql.cpp
    src/qscilexerxml.cpp
    src/qscimacro.cpp
    src/qsciprinter.cpp
    src/qsciscintillabase.cpp
    src/qsciscintilla.cpp
    src/qscistyle.cpp
    src/qscistyledtext.cpp
    src/SciAccessibility.cpp
    src/SciClasses.cpp
    src/ScintillaQt.cpp
    scintilla/include/ILexer.h
    scintilla/include/ILoader.h
    scintilla/include/Platform.h
    scintilla/include/SciLexer.h
    scintilla/include/Scintilla.h
    scintilla/include/ScintillaWidget.h
    scintilla/include/Sci_Position.h
    scintilla/lexlib/Accessor.h
    scintilla/lexlib/CharacterCategory.h
    scintilla/lexlib/CharacterSet.h
    scintilla/lexlib/DefaultLexer.h
    scintilla/lexlib/LexAccessor.h
    scintilla/lexlib/LexerBase.h
    scintilla/lexlib/LexerModule.h
    scintilla/lexlib/LexerNoExceptions.h
    scintilla/lexlib/LexerSimple.h
    scintilla/lexlib/OptionSet.h
    scintilla/lexlib/PropSetSimple.h
    scintilla/lexlib/SparseState.h
    scintilla/lexlib/StringCopy.h
    scintilla/lexlib/StyleContext.h
    scintilla/lexlib/SubStyles.h
    scintilla/lexlib/WordList.h
    scintilla/src/AutoComplete.h
    scintilla/src/CallTip.h
    scintilla/src/CaseConvert.h
    scintilla/src/CaseFolder.h
    scintilla/src/Catalogue.h
    scintilla/src/CellBuffer.h
    scintilla/src/CharClassify.h
    scintilla/src/ContractionState.h
    scintilla/src/DBCS.h
    scintilla/src/Decoration.h
    scintilla/src/Document.h
    scintilla/src/EditModel.h
    scintilla/src/Editor.h
    scintilla/src/EditView.h
    scintilla/src/ElapsedPeriod.h
    scintilla/src/ExternalLexer.h
    scintilla/src/FontQuality.h
    scintilla/src/Indicator.h
    scintilla/src/IntegerRectangle.h
    scintilla/src/KeyMap.h
    scintilla/src/LineMarker.h
    scintilla/src/MarginView.h
    scintilla/src/Partitioning.h
    scintilla/src/PerLine.h
    scintilla/src/PositionCache.h
    scintilla/src/Position.h
    scintilla/src/RESearch.h
    scintilla/src/RunStyles.h
    scintilla/src/ScintillaBase.h
    scintilla/src/Selection.h
    scintilla/src/SparseVector.h
    scintilla/src/SplitVector.h
    scintilla/src/Style.h
    scintilla/src/UniConversion.h
    scintilla/src/UniqueString.h
    scintilla/src/ViewStyle.h
    scintilla/src/XPM.h
    src/ListBoxQt.h
    src/Qsci/qsciabstractapis.h
    src/Qsci/qsciapis.h
    src/Qsci/qscicommand.h
    src/Qsci/qscicommandset.h
    src/Qsci/qscidocument.h
    src/Qsci/qsciglobal.h
    src/Qsci/qscilexercpp.h
    src/Qsci/qscilexercustom.h
    src/Qsci/qscilexer.h
    src/Qsci/qscilexerhtml.h
    src/Qsci/qscilexerjavascript.h
    src/Qsci/qscilexerjson.h
    src/Qsci/qscilexerpython.h
    src/Qsci/qscilexersql.h
    src/Qsci/qscilexerxml.h
    src/Qsci/qscimacro.h
    src/Qsci/qsciprinter.h
    src/Qsci/qsciscintillabase.h
    src/Qsci/qsciscintilla.h
    src/Qsci/qscistyledtext.h
    src/Qsci/qscistyle.h
    src/SciAccessibility.h
    src/SciClasses.h
    src/ScintillaQt.h
)

add_library(qscintilla2 ${SOURCES})

target_include_directories(qscintilla2 PRIVATE scintilla/include scintilla/lexlib scintilla/src src)
target_include_directories(qscintilla2 INTERFACE src)

target_compile_definitions(qscintilla2 PRIVATE SCINTILLA_QT)
target_compile_definitions(qscintilla2 PRIVATE DSCI_LEXER)

target_link_libraries(qscintilla2 Qt5::Widgets Qt5::PrintSupport)

if (APPLE)
    target_link_libraries(qscintilla2 Qt5::MacExtras)
endif()

add_library(QScintilla::QScintilla ALIAS qscintilla2)
