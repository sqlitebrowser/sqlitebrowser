cmake_minimum_required(VERSION 3.16)
project(qscintilla2 VERSION 2.14.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(${QT_MAJOR} REQUIRED COMPONENTS PrintSupport Widgets)

if(APPLE)
    find_package(${QT_MAJOR} REQUIRED COMPONENTS MacExtras)
endif()

add_definitions(-DSCINTILLA_QT)
add_definitions(-DSCI_LEXER)

set(SRC_LIST
    ../scintilla/include/ILexer.h
    ../scintilla/include/ILoader.h
    ../scintilla/include/Platform.h
    ../scintilla/include/SciLexer.h
    ../scintilla/include/Sci_Position.h
    ../scintilla/include/Scintilla.h
    ../scintilla/include/ScintillaWidget.h
    ../scintilla/lexers/LexA68k.cpp
    ../scintilla/lexers/LexAPDL.cpp
    ../scintilla/lexers/LexASY.cpp
    ../scintilla/lexers/LexAU3.cpp
    ../scintilla/lexers/LexAVE.cpp
    ../scintilla/lexers/LexAVS.cpp
    ../scintilla/lexers/LexAbaqus.cpp
    ../scintilla/lexers/LexAda.cpp
    ../scintilla/lexers/LexAsm.cpp
    ../scintilla/lexers/LexAsn1.cpp
    ../scintilla/lexers/LexBaan.cpp
    ../scintilla/lexers/LexBash.cpp
    ../scintilla/lexers/LexBasic.cpp
    ../scintilla/lexers/LexBatch.cpp
    ../scintilla/lexers/LexBibTeX.cpp
    ../scintilla/lexers/LexBullant.cpp
    ../scintilla/lexers/LexCLW.cpp
    ../scintilla/lexers/LexCOBOL.cpp
    ../scintilla/lexers/LexCPP.cpp
    ../scintilla/lexers/LexCSS.cpp
    ../scintilla/lexers/LexCaml.cpp
    ../scintilla/lexers/LexCmake.cpp
    ../scintilla/lexers/LexCoffeeScript.cpp
    ../scintilla/lexers/LexConf.cpp
    ../scintilla/lexers/LexCrontab.cpp
    ../scintilla/lexers/LexCsound.cpp
    ../scintilla/lexers/LexD.cpp
    ../scintilla/lexers/LexDMAP.cpp
    ../scintilla/lexers/LexDMIS.cpp
    ../scintilla/lexers/LexDiff.cpp
    ../scintilla/lexers/LexECL.cpp
    ../scintilla/lexers/LexEDIFACT.cpp
    ../scintilla/lexers/LexEScript.cpp
    ../scintilla/lexers/LexEiffel.cpp
    ../scintilla/lexers/LexErlang.cpp
    ../scintilla/lexers/LexErrorList.cpp
    ../scintilla/lexers/LexFlagship.cpp
    ../scintilla/lexers/LexForth.cpp
    ../scintilla/lexers/LexFortran.cpp
    ../scintilla/lexers/LexGAP.cpp
    ../scintilla/lexers/LexGui4Cli.cpp
    ../scintilla/lexers/LexHTML.cpp
    ../scintilla/lexers/LexHaskell.cpp
    ../scintilla/lexers/LexHex.cpp
    ../scintilla/lexers/LexIndent.cpp
    ../scintilla/lexers/LexInno.cpp
    ../scintilla/lexers/LexJSON.cpp
    ../scintilla/lexers/LexKVIrc.cpp
    ../scintilla/lexers/LexKix.cpp
    ../scintilla/lexers/LexLaTeX.cpp
    ../scintilla/lexers/LexLisp.cpp
    ../scintilla/lexers/LexLout.cpp
    ../scintilla/lexers/LexLua.cpp
    ../scintilla/lexers/LexMMIXAL.cpp
    ../scintilla/lexers/LexMPT.cpp
    ../scintilla/lexers/LexMSSQL.cpp
    ../scintilla/lexers/LexMagik.cpp
    ../scintilla/lexers/LexMake.cpp
    ../scintilla/lexers/LexMarkdown.cpp
    ../scintilla/lexers/LexMatlab.cpp
    ../scintilla/lexers/LexMaxima.cpp
    ../scintilla/lexers/LexMetapost.cpp
    ../scintilla/lexers/LexModula.cpp
    ../scintilla/lexers/LexMySQL.cpp
    ../scintilla/lexers/LexNimrod.cpp
    ../scintilla/lexers/LexNsis.cpp
    ../scintilla/lexers/LexNull.cpp
    ../scintilla/lexers/LexOScript.cpp
    ../scintilla/lexers/LexOpal.cpp
    ../scintilla/lexers/LexPB.cpp
    ../scintilla/lexers/LexPLM.cpp
    ../scintilla/lexers/LexPO.cpp
    ../scintilla/lexers/LexPOV.cpp
    ../scintilla/lexers/LexPS.cpp
    ../scintilla/lexers/LexPascal.cpp
    ../scintilla/lexers/LexPerl.cpp
    ../scintilla/lexers/LexPowerPro.cpp
    ../scintilla/lexers/LexPowerShell.cpp
    ../scintilla/lexers/LexProgress.cpp
    ../scintilla/lexers/LexProps.cpp
    ../scintilla/lexers/LexPython.cpp
    ../scintilla/lexers/LexR.cpp
    ../scintilla/lexers/LexRebol.cpp
    ../scintilla/lexers/LexRegistry.cpp
    ../scintilla/lexers/LexRuby.cpp
    ../scintilla/lexers/LexRust.cpp
    ../scintilla/lexers/LexSAS.cpp
    ../scintilla/lexers/LexSML.cpp
    ../scintilla/lexers/LexSQL.cpp
    ../scintilla/lexers/LexSTTXT.cpp
    ../scintilla/lexers/LexScriptol.cpp
    ../scintilla/lexers/LexSmalltalk.cpp
    ../scintilla/lexers/LexSorcus.cpp
    ../scintilla/lexers/LexSpecman.cpp
    ../scintilla/lexers/LexSpice.cpp
    ../scintilla/lexers/LexStata.cpp
    ../scintilla/lexers/LexTACL.cpp
    ../scintilla/lexers/LexTADS3.cpp
    ../scintilla/lexers/LexTAL.cpp
    ../scintilla/lexers/LexTCL.cpp
    ../scintilla/lexers/LexTCMD.cpp
    ../scintilla/lexers/LexTeX.cpp
    ../scintilla/lexers/LexTxt2tags.cpp
    ../scintilla/lexers/LexVB.cpp
    ../scintilla/lexers/LexVHDL.cpp
    ../scintilla/lexers/LexVerilog.cpp
    ../scintilla/lexers/LexVisualProlog.cpp
    ../scintilla/lexers/LexYAML.cpp
    ../scintilla/lexlib/Accessor.cpp ../scintilla/lexlib/Accessor.h
    ../scintilla/lexlib/CharacterCategory.cpp ../scintilla/lexlib/CharacterCategory.h
    ../scintilla/lexlib/CharacterSet.cpp ../scintilla/lexlib/CharacterSet.h
    ../scintilla/lexlib/DefaultLexer.cpp ../scintilla/lexlib/DefaultLexer.h
    ../scintilla/lexlib/LexAccessor.h
    ../scintilla/lexlib/LexerBase.cpp ../scintilla/lexlib/LexerBase.h
    ../scintilla/lexlib/LexerModule.cpp ../scintilla/lexlib/LexerModule.h
    ../scintilla/lexlib/LexerNoExceptions.cpp ../scintilla/lexlib/LexerNoExceptions.h
    ../scintilla/lexlib/LexerSimple.cpp ../scintilla/lexlib/LexerSimple.h
    ../scintilla/lexlib/OptionSet.h
    ../scintilla/lexlib/PropSetSimple.cpp ../scintilla/lexlib/PropSetSimple.h
    ../scintilla/lexlib/SparseState.h
    ../scintilla/lexlib/StringCopy.h
    ../scintilla/lexlib/StyleContext.cpp ../scintilla/lexlib/StyleContext.h
    ../scintilla/lexlib/SubStyles.h
    ../scintilla/lexlib/WordList.cpp ../scintilla/lexlib/WordList.h
    ../scintilla/src/AutoComplete.cpp ../scintilla/src/AutoComplete.h
    ../scintilla/src/CallTip.cpp ../scintilla/src/CallTip.h
    ../scintilla/src/CaseConvert.cpp ../scintilla/src/CaseConvert.h
    ../scintilla/src/CaseFolder.cpp ../scintilla/src/CaseFolder.h
    ../scintilla/src/Catalogue.cpp ../scintilla/src/Catalogue.h
    ../scintilla/src/CellBuffer.cpp ../scintilla/src/CellBuffer.h
    ../scintilla/src/CharClassify.cpp ../scintilla/src/CharClassify.h
    ../scintilla/src/ContractionState.cpp ../scintilla/src/ContractionState.h
    ../scintilla/src/DBCS.cpp ../scintilla/src/DBCS.h
    ../scintilla/src/Decoration.cpp ../scintilla/src/Decoration.h
    ../scintilla/src/Document.cpp ../scintilla/src/Document.h
    ../scintilla/src/EditModel.cpp ../scintilla/src/EditModel.h
    ../scintilla/src/EditView.cpp ../scintilla/src/EditView.h
    ../scintilla/src/Editor.cpp ../scintilla/src/Editor.h
    ../scintilla/src/ElapsedPeriod.h
    ../scintilla/src/ExternalLexer.cpp ../scintilla/src/ExternalLexer.h
    ../scintilla/src/FontQuality.h
    ../scintilla/src/Indicator.cpp ../scintilla/src/Indicator.h
    ../scintilla/src/IntegerRectangle.h
    ../scintilla/src/KeyMap.cpp ../scintilla/src/KeyMap.h
    ../scintilla/src/LineMarker.cpp ../scintilla/src/LineMarker.h
    ../scintilla/src/MarginView.cpp ../scintilla/src/MarginView.h
    ../scintilla/src/Partitioning.h
    ../scintilla/src/PerLine.cpp ../scintilla/src/PerLine.h
    ../scintilla/src/Position.h
    ../scintilla/src/PositionCache.cpp ../scintilla/src/PositionCache.h
    ../scintilla/src/RESearch.cpp ../scintilla/src/RESearch.h
    ../scintilla/src/RunStyles.cpp ../scintilla/src/RunStyles.h
    ../scintilla/src/ScintillaBase.cpp ../scintilla/src/ScintillaBase.h
    ../scintilla/src/Selection.cpp ../scintilla/src/Selection.h
    ../scintilla/src/SparseVector.h
    ../scintilla/src/SplitVector.h
    ../scintilla/src/Style.cpp ../scintilla/src/Style.h
    ../scintilla/src/UniConversion.cpp ../scintilla/src/UniConversion.h
    ../scintilla/src/UniqueString.h
    ../scintilla/src/ViewStyle.cpp ../scintilla/src/ViewStyle.h
    ../scintilla/src/XPM.cpp ../scintilla/src/XPM.h
    InputMethod.cpp
    ListBoxQt.cpp ListBoxQt.h
    MacPasteboardMime.cpp
    PlatQt.cpp
    Qsci/qsciabstractapis.h
    Qsci/qsciapis.h
    Qsci/qscicommand.h
    Qsci/qscicommandset.h
    Qsci/qscidocument.h
    Qsci/qsciglobal.h
    Qsci/qscilexer.h
    Qsci/qscilexerasm.h
    Qsci/qscilexeravs.h
    Qsci/qscilexerbash.h
    Qsci/qscilexerbatch.h
    Qsci/qscilexercmake.h
    Qsci/qscilexercoffeescript.h
    Qsci/qscilexercpp.h
    Qsci/qscilexercsharp.h
    Qsci/qscilexercss.h
    Qsci/qscilexercustom.h
    Qsci/qscilexerd.h
    Qsci/qscilexerdiff.h
    Qsci/qscilexeredifact.h
    Qsci/qscilexerfortran.h
    Qsci/qscilexerfortran77.h
    Qsci/qscilexerhex.h
    Qsci/qscilexerhtml.h
    Qsci/qscilexeridl.h
    Qsci/qscilexerintelhex.h
    Qsci/qscilexerjava.h
    Qsci/qscilexerjavascript.h
    Qsci/qscilexerjson.h
    Qsci/qscilexerlua.h
    Qsci/qscilexermakefile.h
    Qsci/qscilexermarkdown.h
    Qsci/qscilexermasm.h
    Qsci/qscilexermatlab.h
    Qsci/qscilexernasm.h
    Qsci/qscilexeroctave.h
    Qsci/qscilexerpascal.h
    Qsci/qscilexerperl.h
    Qsci/qscilexerpo.h
    Qsci/qscilexerpostscript.h
    Qsci/qscilexerpov.h
    Qsci/qscilexerproperties.h
    Qsci/qscilexerpython.h
    Qsci/qscilexerruby.h
    Qsci/qscilexerspice.h
    Qsci/qscilexersql.h
    Qsci/qscilexersrec.h
    Qsci/qscilexertcl.h
    Qsci/qscilexertekhex.h
    Qsci/qscilexertex.h
    Qsci/qscilexerverilog.h
    Qsci/qscilexervhdl.h
    Qsci/qscilexerxml.h
    Qsci/qscilexeryaml.h
    Qsci/qscimacro.h
    Qsci/qsciscintilla.h
    Qsci/qsciscintillabase.h
    Qsci/qscistyle.h
    Qsci/qscistyledtext.h
    Qsci/qsciprinter.h
    SciAccessibility.cpp SciAccessibility.h
    SciClasses.cpp SciClasses.h
    ScintillaQt.cpp ScintillaQt.h
    qsciabstractapis.cpp
    qsciapis.cpp
    qscicommand.cpp
    qscicommandset.cpp
    qscidocument.cpp
    qscilexer.cpp
    qscilexerasm.cpp
    qscilexeravs.cpp
    qscilexerbash.cpp
    qscilexerbatch.cpp
    qscilexercmake.cpp
    qscilexercoffeescript.cpp
    qscilexercpp.cpp
    qscilexercsharp.cpp
    qscilexercss.cpp
    qscilexercustom.cpp
    qscilexerd.cpp
    qscilexerdiff.cpp
    qscilexeredifact.cpp
    qscilexerfortran.cpp
    qscilexerfortran77.cpp
    qscilexerhex.cpp
    qscilexerhtml.cpp
    qscilexeridl.cpp
    qscilexerintelhex.cpp
    qscilexerjava.cpp
    qscilexerjavascript.cpp
    qscilexerjson.cpp
    qscilexerlua.cpp
    qscilexermakefile.cpp
    qscilexermarkdown.cpp
    qscilexermasm.cpp
    qscilexermatlab.cpp
    qscilexernasm.cpp
    qscilexeroctave.cpp
    qscilexerpascal.cpp
    qscilexerperl.cpp
    qscilexerpo.cpp
    qscilexerpostscript.cpp
    qscilexerpov.cpp
    qscilexerproperties.cpp
    qscilexerpython.cpp
    qscilexerruby.cpp
    qscilexerspice.cpp
    qscilexersql.cpp
    qscilexersrec.cpp
    qscilexertcl.cpp
    qscilexertekhex.cpp
    qscilexertex.cpp
    qscilexerverilog.cpp
    qscilexervhdl.cpp
    qscilexerxml.cpp
    qscilexeryaml.cpp
    qscimacro.cpp
    qsciscintilla.cpp
    qsciscintillabase.cpp
    qscistyle.cpp
    qscistyledtext.cpp
    qsciprinter.cpp
)

add_library(qscintilla2 ${SRC_LIST})
target_include_directories(qscintilla2 PRIVATE ../scintilla/include ../scintilla/lexlib ../scintilla/src)
target_include_directories(qscintilla2 INTERFACE .)

target_link_libraries(qscintilla2 ${QT_MAJOR}::Widgets ${QT_MAJOR}::PrintSupport)

if (APPLE)
    target_link_libraries(qscintilla2 ${QT_MAJOR}::MacExtras)
endif()

add_library(QScintilla::QScintilla ALIAS qscintilla2)
