TEMPLATE = lib

CONFIG += staticlib
CONFIG += debug_and_release

INCLUDEPATH += ./

HEADERS += \
    antlr/config.hpp \
    antlr/TreeParserSharedInputState.hpp \
    antlr/TreeParser.hpp \
    antlr/TokenWithIndex.hpp \
    antlr/TokenStreamSelector.hpp \
    antlr/TokenStreamRewriteEngine.hpp \
    antlr/TokenStreamRetryException.hpp \
    antlr/TokenStreamRecognitionException.hpp \
    antlr/TokenStreamIOException.hpp \
    antlr/TokenStreamHiddenTokenFilter.hpp \
    antlr/TokenStreamException.hpp \
    antlr/TokenStreamBasicFilter.hpp \
    antlr/TokenStream.hpp \
    antlr/TokenRefCount.hpp \
    antlr/TokenBuffer.hpp \
    antlr/Token.hpp \
    antlr/String.hpp \
    antlr/SemanticException.hpp \
    antlr/RefCount.hpp \
    antlr/RecognitionException.hpp \
    antlr/ParserSharedInputState.hpp \
    antlr/Parser.hpp \
    antlr/NoViableAltForCharException.hpp \
    antlr/NoViableAltException.hpp \
    antlr/MismatchedTokenException.hpp \
    antlr/MismatchedCharException.hpp \
    antlr/LexerSharedInputState.hpp \
    antlr/LLkParser.hpp \
    antlr/InputBuffer.hpp \
    antlr/IOException.hpp \
    antlr/CommonToken.hpp \
    antlr/CommonHiddenStreamToken.hpp \
    antlr/CommonASTWithHiddenTokens.hpp \
    antlr/CommonAST.hpp \
    antlr/CircularQueue.hpp \
    antlr/CharStreamIOException.hpp \
    antlr/CharStreamException.hpp \
    antlr/CharScanner.hpp \
    antlr/CharInputBuffer.hpp \
    antlr/CharBuffer.hpp \
    antlr/BitSet.hpp \
    antlr/BaseAST.hpp \
    antlr/ASTRefCount.hpp \
    antlr/ASTPair.hpp \
    antlr/ASTNULLType.hpp \
    antlr/ASTFactory.hpp \
    antlr/ASTArray.hpp \
    antlr/AST.hpp \
    antlr/ANTLRUtil.hpp \
    antlr/ANTLRException.hpp

SOURCES += \
    src/TreeParser.cpp \
    src/TokenStreamSelector.cpp \
    src/TokenStreamRewriteEngine.cpp \
    src/TokenStreamHiddenTokenFilter.cpp \
    src/TokenStreamBasicFilter.cpp \
    src/TokenRefCount.cpp \
    src/TokenBuffer.cpp \
    src/Token.cpp \
    src/String.cpp \
    src/RecognitionException.cpp \
    src/Parser.cpp \
    src/NoViableAltForCharException.cpp \
    src/NoViableAltException.cpp \
    src/MismatchedTokenException.cpp \
    src/MismatchedCharException.cpp \
    src/LLkParser.cpp \
    src/InputBuffer.cpp \
    src/CommonToken.cpp \
    src/CommonHiddenStreamToken.cpp \
    src/CommonASTWithHiddenTokens.cpp \
    src/CommonAST.cpp \
    src/CharScanner.cpp \
    src/CharBuffer.cpp \
    src/BitSet.cpp \
    src/BaseAST.cpp \
    src/ASTRefCount.cpp \
    src/ASTNULLType.cpp \
    src/ASTFactory.cpp \
    src/ANTLRUtil.cpp
