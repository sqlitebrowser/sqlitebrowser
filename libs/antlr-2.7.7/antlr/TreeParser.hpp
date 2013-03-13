#ifndef INC_TreeParser_hpp__
#define INC_TreeParser_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/TreeParser.hpp#2 $
 */

#include <antlr/config.hpp>
#include <antlr/AST.hpp>
#include <antlr/ASTFactory.hpp>
#include <antlr/BitSet.hpp>
#include <antlr/RecognitionException.hpp>
#include <antlr/MismatchedTokenException.hpp>
#include <antlr/TreeParserSharedInputState.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

class ANTLR_API TreeParser {
public:
	TreeParser()
	: astFactory(0)
	, inputState(new TreeParserInputState())
	, traceDepth(0)
	{
	}

	TreeParser(const TreeParserSharedInputState& state)
	: astFactory(0)
	, inputState(state)
	, traceDepth(0)
	{
	}

	virtual ~TreeParser()
	{
	}

	/// Get the AST return value squirreled away in the parser
	virtual RefAST getAST() = 0;

	/** Make sure current lookahead symbol matches the given set
	 * Throw an exception upon mismatch, which is caught by either the
	 * error handler or by a syntactic predicate.
	 */
	virtual void match(RefAST t, const BitSet& b)
	{
		if ( !t || t==ASTNULL || !b.member(t->getType()) )
			throw MismatchedTokenException( getTokenNames(), getNumTokens(),
													  t, b, false );
	}

	/** Specify the AST factory to be used during tree building. (Compulsory)
	 * Setting the factory is compulsory (if you intend to modify
	 * the tree in the treeparser). The AST Factory is shared between
	 * parser (who builds the initial AST) and treeparser.
	 * @see Parser::getASTFactory()
	 */
	virtual void setASTFactory(ASTFactory* factory)
	{
		astFactory = factory;
	}
	/// Return pointer to ASTFactory
	virtual ASTFactory* getASTFactory() const
	{
		return astFactory;
	}
	/// Get the name for token 'num'
	virtual const char* getTokenName(int num) const = 0;
	/// Return the number of tokens defined
	virtual int getNumTokens() const = 0;
	/// Return an array of getNumTokens() token names
	virtual const char* const* getTokenNames() const = 0;

	/// Parser error-reporting function can be overridden in subclass
	virtual void reportError(const RecognitionException& ex);
	/// Parser error-reporting function can be overridden in subclass
	virtual void reportError(const ANTLR_USE_NAMESPACE(std)string& s);
	/// Parser warning-reporting function can be overridden in subclass
	virtual void reportWarning(const ANTLR_USE_NAMESPACE(std)string& s);

	/// These are used during when traceTreeParser commandline option is passed.
	virtual void traceIndent();
	virtual void traceIn(const char* rname, RefAST t);
	virtual void traceOut(const char* rname, RefAST t);

	/** The AST Null object; the parsing cursor is set to this when
	 * it is found to be null.  This way, we can test the
	 * token type of a node without having to have tests for 0
	 * everywhere.
	 */
	static RefAST ASTNULL;

protected:
	virtual void match(RefAST t, int ttype)
	{
		if (!t || t == ASTNULL || t->getType() != ttype )
			throw MismatchedTokenException( getTokenNames(), getNumTokens(),
													  t, ttype, false );
	}

	virtual void matchNot(RefAST t, int ttype)
	{
		if ( !t || t == ASTNULL || t->getType() == ttype )
			throw MismatchedTokenException( getTokenNames(), getNumTokens(),
													  t, ttype, true );
	}

	/** AST support code; parser and treeparser delegate to this object */
	ASTFactory* astFactory;

	/// The input state of this tree parser.
	TreeParserSharedInputState inputState;

	/** Used to keep track of indent depth with -traceTreeParser */
	int traceDepth;

	/** Utility class which allows tracing to work even when exceptions are
	 * thrown.
	 */
	class Tracer {
	private:
		TreeParser* parser;
		const char* text;
		RefAST tree;
	public:
		Tracer(TreeParser* p, const char* t, RefAST a)
		: parser(p), text(t), tree(a)
		{
			parser->traceIn(text,tree);
		}
		~Tracer()
		{
			parser->traceOut(text,tree);
		}
	private:
		Tracer(const Tracer&);							// undefined
		const Tracer& operator=(const Tracer&);	// undefined
	};

private:
	// no copying of treeparser instantiations...
	TreeParser(const TreeParser& other);
	TreeParser& operator=(const TreeParser& other);
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_TreeParser_hpp__
