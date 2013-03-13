/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/src/Parser.cpp#2 $
 */

#include "antlr/Parser.hpp"

#include <iostream>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** A generic ANTLR parser (LL(k) for k>=1) containing a bunch of
 * utility routines useful at any lookahead depth.  We distinguish between
 * the LL(1) and LL(k) parsers because of efficiency.  This may not be
 * necessary in the near future.
 *
 * Each parser object contains the state of the parse including a lookahead
 * cache (the form of which is determined by the subclass), whether or
 * not the parser is in guess mode, where tokens come from, etc...
 *
 * <p>
 * During <b>guess</b> mode, the current lookahead token(s) and token type(s)
 * cache must be saved because the token stream may not have been informed
 * to save the token (via <tt>mark</tt>) before the <tt>try</tt> block.
 * Guessing is started by:
 * <ol>
 * <li>saving the lookahead cache.
 * <li>marking the current position in the TokenBuffer.
 * <li>increasing the guessing level.
 * </ol>
 *
 * After guessing, the parser state is restored by:
 * <ol>
 * <li>restoring the lookahead cache.
 * <li>rewinding the TokenBuffer.
 * <li>decreasing the guessing level.
 * </ol>
 *
 * @see antlr.Token
 * @see antlr.TokenBuffer
 * @see antlr.TokenStream
 * @see antlr.LL1Parser
 * @see antlr.LLkParser
 */

bool DEBUG_PARSER = false;

/** Parser error-reporting function can be overridden in subclass */
void Parser::reportError(const RecognitionException& ex)
{
	ANTLR_USE_NAMESPACE(std)cerr << ex.toString().c_str() << ANTLR_USE_NAMESPACE(std)endl;
}

/** Parser error-reporting function can be overridden in subclass */
void Parser::reportError(const ANTLR_USE_NAMESPACE(std)string& s)
{
	if ( getFilename()=="" )
		ANTLR_USE_NAMESPACE(std)cerr << "error: " << s.c_str() << ANTLR_USE_NAMESPACE(std)endl;
	else
		ANTLR_USE_NAMESPACE(std)cerr << getFilename().c_str() << ": error: " << s.c_str() << ANTLR_USE_NAMESPACE(std)endl;
}

/** Parser warning-reporting function can be overridden in subclass */
void Parser::reportWarning(const ANTLR_USE_NAMESPACE(std)string& s)
{
	if ( getFilename()=="" )
		ANTLR_USE_NAMESPACE(std)cerr << "warning: " << s.c_str() << ANTLR_USE_NAMESPACE(std)endl;
	else
		ANTLR_USE_NAMESPACE(std)cerr << getFilename().c_str() << ": warning: " << s.c_str() << ANTLR_USE_NAMESPACE(std)endl;
}

/** Set or change the input token buffer */
//	void setTokenBuffer(TokenBuffer<Token>* t);

void Parser::traceIndent()
{
	for( int i = 0; i < traceDepth; i++ )
		ANTLR_USE_NAMESPACE(std)cout << " ";
}

void Parser::traceIn(const char* rname)
{
	traceDepth++;

	for( int i = 0; i < traceDepth; i++ )
		ANTLR_USE_NAMESPACE(std)cout << " ";

	ANTLR_USE_NAMESPACE(std)cout << "> " << rname
		<< "; LA(1)==" << LT(1)->getText().c_str()
		<<	((inputState->guessing>0)?" [guessing]":"")
		<< ANTLR_USE_NAMESPACE(std)endl;
}

void Parser::traceOut(const char* rname)
{
	for( int i = 0; i < traceDepth; i++ )
		ANTLR_USE_NAMESPACE(std)cout << " ";

	ANTLR_USE_NAMESPACE(std)cout << "< " << rname
		<< "; LA(1)==" << LT(1)->getText().c_str()
		<<	((inputState->guessing>0)?" [guessing]":"")
		<< ANTLR_USE_NAMESPACE(std)endl;

	traceDepth--;
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif
