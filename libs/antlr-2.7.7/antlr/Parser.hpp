#ifndef INC_Parser_hpp__
#define INC_Parser_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/Parser.hpp#2 $
 */

#include <antlr/config.hpp>

#include <iostream>
#include <exception>

#include <antlr/BitSet.hpp>
#include <antlr/TokenBuffer.hpp>
#include <antlr/RecognitionException.hpp>
#include <antlr/MismatchedTokenException.hpp>
#include <antlr/ASTFactory.hpp>
#include <antlr/ParserSharedInputState.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

extern bool DEBUG_PARSER;

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
 *
 * @todo add constructors with ASTFactory.
 */
class ANTLR_API Parser {
protected:
	Parser(TokenBuffer& input)
	: inputState(new ParserInputState(input)), astFactory(0), traceDepth(0)
	{
	}
	Parser(TokenBuffer* input)
	: inputState(new ParserInputState(input)), astFactory(0), traceDepth(0)
	{
	}
	Parser(const ParserSharedInputState& state)
	: inputState(state), astFactory(0), traceDepth(0)
	{
	}
public:
	virtual ~Parser()
	{
	}

	/** Return the token type of the ith token of lookahead where i=1
	 * is the current token being examined by the parser (i.e., it
	 * has not been matched yet).
	 */
	virtual int LA(unsigned int i)=0;

	/// Return the i-th token of lookahead
	virtual RefToken LT(unsigned int i)=0;

	/** DEPRECATED! Specify the factory to be used during tree building. (Compulsory)
	 * Setting the factory is nowadays compulsory.
	 * @see setASTFactory
	 */
	virtual void setASTNodeFactory( ASTFactory *factory )
	{
		astFactory = factory;
	}
	/** Specify the factory to be used during tree building. (Compulsory)
	 * Setting the factory is nowadays compulsory.
	 */
	virtual void setASTFactory( ASTFactory *factory )
	{
		astFactory = factory;
	}
	/** Return a pointer to the ASTFactory used.
	 * So you might use it in subsequent treewalkers or to reload AST's
	 * from disk.
	 */
	virtual ASTFactory* getASTFactory()
	{
		return astFactory;
	}
	/** Get the root AST node of the generated AST. When using a custom AST type
	 * or heterogenous AST's, you'll have to convert it to the right type
	 * yourself.
	 */
	virtual RefAST getAST() = 0;

	/// Return the filename of the input file.
	virtual inline ANTLR_USE_NAMESPACE(std)string getFilename() const
	{
		return inputState->filename;
	}
	/// Set the filename of the input file (used for error reporting).
	virtual void setFilename(const ANTLR_USE_NAMESPACE(std)string& f)
	{
		inputState->filename = f;
	}

	virtual void setInputState(ParserSharedInputState state)
	{
		inputState = state;
	}
	virtual inline ParserSharedInputState getInputState() const
	{
		return inputState;
	}

	/// Get another token object from the token stream
	virtual void consume()=0;
	/// Consume tokens until one matches the given token
	virtual void consumeUntil(int tokenType)
	{
		while (LA(1) != Token::EOF_TYPE && LA(1) != tokenType)
			consume();
	}

	/// Consume tokens until one matches the given token set
	virtual void consumeUntil(const BitSet& set)
	{
		while (LA(1) != Token::EOF_TYPE && !set.member(LA(1)))
			consume();
	}

	/** Make sure current lookahead symbol matches token type <tt>t</tt>.
	 * Throw an exception upon mismatch, which is catch by either the
	 * error handler or by the syntactic predicate.
	 */
	virtual void match(int t)
	{
		if ( DEBUG_PARSER )
		{
			traceIndent();
			ANTLR_USE_NAMESPACE(std)cout << "enter match(" << t << ") with LA(1)=" << LA(1) << ANTLR_USE_NAMESPACE(std)endl;
		}
		if ( LA(1) != t )
		{
			if ( DEBUG_PARSER )
			{
				traceIndent();
				ANTLR_USE_NAMESPACE(std)cout << "token mismatch: " << LA(1) << "!=" << t << ANTLR_USE_NAMESPACE(std)endl;
			}
			throw MismatchedTokenException(getTokenNames(), getNumTokens(), LT(1), t, false, getFilename());
		}
		else
		{
			// mark token as consumed -- fetch next token deferred until LA/LT
			consume();
		}
	}

	virtual void matchNot(int t)
	{
		if ( LA(1)==t )
		{
			// Throws inverted-sense exception
			throw MismatchedTokenException(getTokenNames(), getNumTokens(), LT(1), t, true, getFilename());
		}
		else
		{
			// mark token as consumed -- fetch next token deferred until LA/LT
			consume();
		}
	}

	/** Make sure current lookahead symbol matches the given set
	 * Throw an exception upon mismatch, which is catch by either the
	 * error handler or by the syntactic predicate.
	 */
	virtual void match(const BitSet& b)
	{
		if ( DEBUG_PARSER )
		{
			traceIndent();
			ANTLR_USE_NAMESPACE(std)cout << "enter match(" << "bitset" /*b.toString()*/
				<< ") with LA(1)=" << LA(1) << ANTLR_USE_NAMESPACE(std)endl;
		}
		if ( !b.member(LA(1)) )
		{
			if ( DEBUG_PARSER )
			{
				traceIndent();
				ANTLR_USE_NAMESPACE(std)cout << "token mismatch: " << LA(1) << " not member of "
					<< "bitset" /*b.toString()*/ << ANTLR_USE_NAMESPACE(std)endl;
			}
			throw MismatchedTokenException(getTokenNames(), getNumTokens(), LT(1), b, false, getFilename());
		}
		else
		{
			// mark token as consumed -- fetch next token deferred until LA/LT
			consume();
		}
	}

	/** Mark a spot in the input and return the position.
	 * Forwarded to TokenBuffer.
	 */
	virtual inline unsigned int mark()
	{
		return inputState->getInput().mark();
	}
	/// rewind to a previously marked position
	virtual inline void rewind(unsigned int pos)
	{
		inputState->getInput().rewind(pos);
	}
	/** called by the generated parser to do error recovery, override to
	 * customize the behaviour.
	 */
	virtual void recover(const RecognitionException& ex, const BitSet& tokenSet)
	{
		consume();
		consumeUntil(tokenSet);
	}

	/// Parser error-reporting function can be overridden in subclass
	virtual void reportError(const RecognitionException& ex);
	/// Parser error-reporting function can be overridden in subclass
	virtual void reportError(const ANTLR_USE_NAMESPACE(std)string& s);
	/// Parser warning-reporting function can be overridden in subclass
	virtual void reportWarning(const ANTLR_USE_NAMESPACE(std)string& s);

	/// get the token name for the token number 'num'
	virtual const char* getTokenName(int num) const = 0;
	/// get a vector with all token names
	virtual const char* const* getTokenNames() const = 0;
	/** Get the number of tokens defined.
	 * This one should be overridden in subclasses.
	 */
	virtual int getNumTokens(void) const = 0;

	/** Set or change the input token buffer */
//	void setTokenBuffer(TokenBuffer<Token>* t);

	virtual void traceIndent();
	virtual void traceIn(const char* rname);
	virtual void traceOut(const char* rname);
protected:
//	void setTokenNames(const char** tokenNames_);

	ParserSharedInputState inputState;

//	/// AST return value for a rule is squirreled away here
//	RefAST returnAST;

	/// AST support code; parser and treeparser delegate to this object
	ASTFactory *astFactory;

	// used to keep track of the indentation for the trace
	int traceDepth;

	/** Utility class which allows tracing to work even when exceptions are
	 * thrown.
	 */
	class Tracer { /*{{{*/
	private:
		Parser* parser;
		const char* text;
	public:
		Tracer(Parser* p,const char * t)
		: parser(p), text(t)
		{
			parser->traceIn(text);
		}
		~Tracer()
		{
#ifdef ANTLR_CXX_SUPPORTS_UNCAUGHT_EXCEPTION
			// Only give trace if there's no uncaught exception..
			if(!ANTLR_USE_NAMESPACE(std)uncaught_exception())
#endif
				parser->traceOut(text);
		}
	private:
		Tracer(const Tracer&);							// undefined
		const Tracer& operator=(const Tracer&);	// undefined
		/*}}}*/
	};
private:
	Parser(const Parser&);								// undefined
	const Parser& operator=(const Parser&);		// undefined
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_Parser_hpp__
