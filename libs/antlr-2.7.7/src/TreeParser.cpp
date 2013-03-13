/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/src/TreeParser.cpp#2 $
 */

#include "antlr/TreeParser.hpp"
#include "antlr/ASTNULLType.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** The AST Null object; the parsing cursor is set to this when
 *  it is found to be null.  This way, we can test the
 *  token type of a node without having to have tests for null
 *  everywhere.
 */
RefAST TreeParser::ASTNULL(new ASTNULLType);

/** Parser error-reporting function can be overridden in subclass */
void TreeParser::reportError(const RecognitionException& ex)
{
	ANTLR_USE_NAMESPACE(std)cerr << ex.toString().c_str() << ANTLR_USE_NAMESPACE(std)endl;
}

/** Parser error-reporting function can be overridden in subclass */
void TreeParser::reportError(const ANTLR_USE_NAMESPACE(std)string& s)
{
	ANTLR_USE_NAMESPACE(std)cerr << "error: " << s.c_str() << ANTLR_USE_NAMESPACE(std)endl;
}

/** Parser warning-reporting function can be overridden in subclass */
void TreeParser::reportWarning(const ANTLR_USE_NAMESPACE(std)string& s)
{
	ANTLR_USE_NAMESPACE(std)cerr << "warning: " << s.c_str() << ANTLR_USE_NAMESPACE(std)endl;
}

/** Procedure to write out an indent for traceIn and traceOut */
void TreeParser::traceIndent()
{
	for( int i = 0; i < traceDepth; i++ )
		ANTLR_USE_NAMESPACE(std)cout << " ";
}

void TreeParser::traceIn(const char* rname, RefAST t)
{
	traceDepth++;
	traceIndent();

	ANTLR_USE_NAMESPACE(std)cout << "> " << rname
			<< "(" << (t ? t->toString().c_str() : "null") << ")"
			<< ((inputState->guessing>0)?" [guessing]":"")
			<< ANTLR_USE_NAMESPACE(std)endl;
}

void TreeParser::traceOut(const char* rname, RefAST t)
{
	traceIndent();

	ANTLR_USE_NAMESPACE(std)cout << "< " << rname
			<< "(" << (t ? t->toString().c_str() : "null") << ")"
			<< ((inputState->guessing>0)?" [guessing]":"")
			<< ANTLR_USE_NAMESPACE(std)endl;

	traceDepth--;
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif
