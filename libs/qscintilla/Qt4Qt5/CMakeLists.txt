cmake_minimum_required(VERSION 2.8.7)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED)

if(APPLE)
    find_package(Qt5MacExtras)
endif()

add_definitions(-DSCINTILLA_QT)
add_definitions(-DSCI_LEXER)

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
	../lexers/LexSQL.cpp
	../lexlib/Accessor.cpp
	../lexlib/CharacterCategory.cpp
	../lexlib/CharacterSet.cpp
	../lexlib/LexerBase.cpp
	../lexlib/LexerModule.cpp
	../lexlib/LexerNoExceptions.cpp
	../lexlib/LexerSimple.cpp
	../lexlib/PropSetSimple.cpp
	../lexlib/StyleContext.cpp
	../lexlib/WordList.cpp
	../src/AutoComplete.cpp
	../src/CallTip.cpp
	../src/CaseConvert.cpp
	../src/CaseFolder.cpp
	../src/Catalogue.cpp
	../src/CellBuffer.cpp
	../src/CharClassify.cpp
	../src/ContractionState.cpp
	../src/Decoration.cpp
	../src/Document.cpp
	../src/Editor.cpp
	../src/EditModel.cpp
	../src/EditView.cpp
	../src/ExternalLexer.cpp
	../src/Indicator.cpp
	../src/KeyMap.cpp
	../src/LineMarker.cpp
	../src/MarginView.cpp
	../src/PerLine.cpp
	../src/PositionCache.cpp
	../src/RESearch.cpp
	../src/RunStyles.cpp
	../src/ScintillaBase.cpp
	../src/Selection.cpp
	../src/Style.cpp
	../src/UniConversion.cpp
	../src/ViewStyle.cpp
	../src/XPM.cpp
)

set(QSCINTILLA_HDR
	./Qsci/qsciglobal.h
	./Qsci/qscicommand.h
	./Qsci/qscicommandset.h
	./Qsci/qscidocument.h
	./Qsci/qsciprinter.h
	./Qsci/qscistyle.h
	./Qsci/qscistyledtext.h
	ListBoxQt.h
	SciNamespace.h
	../include/ILexer.h
	../include/Platform.h
	../include/SciLexer.h
	../include/Scintilla.h
	../include/ScintillaWidget.h
	../include/Sci_Position.h
	../lexlib/Accessor.h
	../lexlib/CharacterCategory.h
	../lexlib/CharacterSet.h
	../lexlib/LexAccessor.h
	../lexlib/LexerBase.h
	../lexlib/LexerModule.h
	../lexlib/LexerNoExceptions.h
	../lexlib/LexerSimple.h
	../lexlib/OptionSet.h
	../lexlib/PropSetSimple.h
	../lexlib/StyleContext.h
	../lexlib/SubStyles.h
	../lexlib/WordList.h
	../src/AutoComplete.h
	../src/CallTip.h
	../src/CaseConvert.h
	../src/CaseFolder.h
	../src/Catalogue.h
	../src/CellBuffer.h
	../src/CharClassify.h
	../src/ContractionState.h
	../src/Decoration.h
	../src/Document.h
	../src/Editor.h
	../src/ExternalLexer.h
	../src/FontQuality.h
	../src/Indicator.h
	../src/KeyMap.h
	../src/LineMarker.h
	../src/Partitioning.h
	../src/PerLine.h
	../src/PositionCache.h
	../src/RESearch.h
	../src/RunStyles.h
	../src/ScintillaBase.h
	../src/Selection.h
	../src/SplitVector.h
	../src/Style.h
	../src/UnicodeFromUTF8.h
	../src/UniConversion.h
	../src/ViewStyle.h
	../src/XPM.h
	../src/Position.h
	../src/SparseVector.h
)

set(QSCINTILLA_MOC_HDR
	./Qsci/qsciscintilla.h
	./Qsci/qsciscintillabase.h
	./Qsci/qsciabstractapis.h
	./Qsci/qsciapis.h
	./Qsci/qscilexer.h
	./Qsci/qscilexercustom.h
	./Qsci/qscilexersql.h
	./Qsci/qscimacro.h
	SciClasses.h
	ScintillaQt.h
)

include_directories(. ../include ../lexlib ../src)

add_library(qscintilla2 ${QSCINTILLA_SRC} ${QSCINTILLA_HDR} ${QSCINTILLA_MOC_HDR} ${QSCINTILLA_MOC})

if (APPLE)
	qt5_use_modules(qscintilla2 Widgets PrintSupport MacExtras)
else()
	qt5_use_modules(qscintilla2 Widgets PrintSupport)
endif()
