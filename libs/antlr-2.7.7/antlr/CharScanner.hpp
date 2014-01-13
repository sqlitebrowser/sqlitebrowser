#ifndef INC_CharScanner_hpp__
#define INC_CharScanner_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/CharScanner.hpp#2 $
 */

#include <antlr/config.hpp>

#include <map>

#ifdef HAS_NOT_CCTYPE_H
#include <ctype.h>
#else
#include <cctype>
#endif

#if ( _MSC_VER == 1200 )
// VC6 seems to need this
// note that this is not a standard C++ include file.
# include <stdio.h>
#endif
#include <cstdio>

#include <antlr/TokenStream.hpp>
#include <antlr/RecognitionException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/MismatchedCharException.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/BitSet.hpp>
#include <antlr/LexerSharedInputState.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

class ANTLR_API CharScanner;

ANTLR_C_USING(tolower)

#ifdef ANTLR_REALLY_NO_STRCASECMP
// Apparently, neither strcasecmp nor stricmp is standard, and Codewarrior
// on the mac has neither...
inline int strcasecmp(const char *s1, const char *s2)
{
	while (true)
	{
		char  c1 = tolower(*s1++),
				c2 = tolower(*s2++);
		if (c1 < c2) return -1;
		if (c1 > c2) return 1;
		if (c1 == 0) return 0;
	}
}
#else
#ifdef NO_STRCASECMP
ANTLR_C_USING(stricmp)
#else
#include <strings.h>
ANTLR_C_USING(strcasecmp)
#endif
#endif

/** Functor for the literals map
 */
class ANTLR_API CharScannerLiteralsLess : public ANTLR_USE_NAMESPACE(std)binary_function<ANTLR_USE_NAMESPACE(std)string,ANTLR_USE_NAMESPACE(std)string,bool> {
private:
	const CharScanner* scanner;
public:
#ifdef NO_TEMPLATE_PARTS
	CharScannerLiteralsLess() {} // not really used, definition to appease MSVC
#endif
	CharScannerLiteralsLess(const CharScanner* theScanner)
	: scanner(theScanner)
	{
	}
	bool operator() (const ANTLR_USE_NAMESPACE(std)string& x,const ANTLR_USE_NAMESPACE(std)string& y) const;
// defaults are good enough..
	//	CharScannerLiteralsLess(const CharScannerLiteralsLess&);
	//	CharScannerLiteralsLess& operator=(const CharScannerLiteralsLess&);
};

/** Superclass of generated lexers
 */
class ANTLR_API CharScanner : public TokenStream {
protected:
	typedef RefToken (*factory_type)();
public:
	CharScanner(InputBuffer& cb, bool case_sensitive );
	CharScanner(InputBuffer* cb, bool case_sensitive );
	CharScanner(const LexerSharedInputState& state, bool case_sensitive );

	virtual ~CharScanner()
	{
	}

	virtual int LA(unsigned int i);

	virtual void append(char c)
	{
		if (saveConsumedInput)
		{
			size_t l = text.length();

			if ((l%256) == 0)
				text.reserve(l+256);

			text.replace(l,0,&c,1);
		}
	}

	virtual void append(const ANTLR_USE_NAMESPACE(std)string& s)
	{
		if( saveConsumedInput )
			text += s;
	}

	virtual void commit()
	{
		inputState->getInput().commit();
	}

	/** called by the generated lexer to do error recovery, override to
	 * customize the behaviour.
	 */
	virtual void recover(const RecognitionException& ex, const BitSet& tokenSet)
	{
        (void)ex;
		consume();
		consumeUntil(tokenSet);
	}

	virtual void consume()
	{
		if (inputState->guessing == 0)
		{
			int c = LA(1);
			if (caseSensitive)
			{
				append(c);
			}
			else
			{
				// use input.LA(), not LA(), to get original case
				// CharScanner.LA() would toLower it.
				append(inputState->getInput().LA(1));
			}

			// RK: in a sense I don't like this automatic handling.
			if (c == '\t')
				tab();
			else
				inputState->column++;
		}
		inputState->getInput().consume();
	}

	/** Consume chars until one matches the given char */
	virtual void consumeUntil(int c)
	{
		for(;;)
		{
			int la_1 = LA(1);
			if( la_1 == EOF_CHAR || la_1 == c )
				break;
			consume();
		}
	}

	/** Consume chars until one matches the given set */
	virtual void consumeUntil(const BitSet& set)
	{
		for(;;)
		{
			int la_1 = LA(1);
			if( la_1 == EOF_CHAR || set.member(la_1) )
				break;
			consume();
		}
	}

	/// Mark the current position and return a id for it
	virtual unsigned int mark()
	{
		return inputState->getInput().mark();
	}
	/// Rewind the scanner to a previously marked position
	virtual void rewind(unsigned int pos)
	{
		inputState->getInput().rewind(pos);
	}

	/// See if input contains character 'c' throw MismatchedCharException if not
	virtual void match(int c)
	{
		int la_1 = LA(1);
		if ( la_1 != c )
			throw MismatchedCharException(la_1, c, false, this);
		consume();
	}

	/** See if input contains element from bitset b
	 * throw MismatchedCharException if not
	 */
	virtual void match(const BitSet& b)
	{
		int la_1 = LA(1);

		if ( !b.member(la_1) )
			throw MismatchedCharException( la_1, b, false, this );
		consume();
	}

	/** See if input contains string 's' throw MismatchedCharException if not
	 * @note the string cannot match EOF
	 */
	virtual void match( const char* s )
	{
		while( *s != '\0' )
		{
			// the & 0xFF is here to prevent sign extension lateron
			int la_1 = LA(1), c = (*s++ & 0xFF);

			if ( la_1 != c )
				throw MismatchedCharException(la_1, c, false, this);

			consume();
		}
	}
	/** See if input contains string 's' throw MismatchedCharException if not
	 * @note the string cannot match EOF
	 */
	virtual void match(const ANTLR_USE_NAMESPACE(std)string& s)
	{
		size_t len = s.length();

		for (size_t i = 0; i < len; i++)
		{
			// the & 0xFF is here to prevent sign extension lateron
			int la_1 = LA(1), c = (s[i] & 0xFF);

			if ( la_1 != c )
				throw MismatchedCharException(la_1, c, false, this);

			consume();
		}
	}
	/** See if input does not contain character 'c'
	 * throw MismatchedCharException if not
	 */
	virtual void matchNot(int c)
	{
		int la_1 = LA(1);

		if ( la_1 == c )
			throw MismatchedCharException(la_1, c, true, this);

		consume();
	}
	/** See if input contains character in range c1-c2
	 * throw MismatchedCharException if not
	 */
	virtual void matchRange(int c1, int c2)
	{
		int la_1 = LA(1);

		if ( la_1 < c1 || la_1 > c2 )
			throw MismatchedCharException(la_1, c1, c2, false, this);

		consume();
	}

	virtual bool getCaseSensitive() const
	{
		return caseSensitive;
	}

	virtual void setCaseSensitive(bool t)
	{
		caseSensitive = t;
	}

	virtual bool getCaseSensitiveLiterals() const=0;

	/// Get the line the scanner currently is in (starts at 1)
	virtual int getLine() const
	{
		return inputState->line;
	}

	/// set the line number
	virtual void setLine(int l)
	{
		inputState->line = l;
	}

	/// Get the column the scanner currently is in (starts at 1)
	virtual int getColumn() const
	{
		return inputState->column;
	}
	/// set the column number
	virtual void setColumn(int c)
	{
		inputState->column = c;
	}

	/// get the filename for the file currently used
	virtual const ANTLR_USE_NAMESPACE(std)string& getFilename() const
	{
		return inputState->filename;
	}
	/// Set the filename the scanner is using (used in error messages)
	virtual void setFilename(const ANTLR_USE_NAMESPACE(std)string& f)
	{
		inputState->filename = f;
	}

	virtual bool getCommitToPath() const
	{
		return commitToPath;
	}

	virtual void setCommitToPath(bool commit)
	{
		commitToPath = commit;
	}

	/** return a copy of the current text buffer */
	virtual const ANTLR_USE_NAMESPACE(std)string& getText() const
	{
		return text;
	}

	virtual void setText(const ANTLR_USE_NAMESPACE(std)string& s)
	{
		text = s;
	}

	virtual void resetText()
	{
		text = "";
		inputState->tokenStartColumn = inputState->column;
		inputState->tokenStartLine = inputState->line;
	}

	virtual RefToken getTokenObject() const
	{
		return _returnToken;
	}

	/** Used to keep track of line breaks, needs to be called from
	 * within generated lexers when a \n \r is encountered.
	 */
	virtual void newline()
	{
		++inputState->line;
		inputState->column = 1;
	}

	/** Advance the current column number by an appropriate amount according
	 * to the tabsize. This method needs to be explicitly called from the
	 * lexer rules encountering tabs.
	 */
	virtual void tab()
	{
		int c = getColumn();
		int nc = ( ((c-1)/tabsize) + 1) * tabsize + 1;      // calculate tab stop
		setColumn( nc );
	}
	/// set the tabsize. Returns the old tabsize
	int setTabsize( int size )
	{
		int oldsize = tabsize;
		tabsize = size;
		return oldsize;
	}
	/// Return the tabsize used by the scanner
	int getTabSize() const
	{
		return tabsize;
	}

	/** Report exception errors caught in nextToken() */
	virtual void reportError(const RecognitionException& e);

	/** Parser error-reporting function can be overridden in subclass */
	virtual void reportError(const ANTLR_USE_NAMESPACE(std)string& s);

	/** Parser warning-reporting function can be overridden in subclass */
	virtual void reportWarning(const ANTLR_USE_NAMESPACE(std)string& s);

	virtual InputBuffer& getInputBuffer()
	{
		return inputState->getInput();
	}

	virtual LexerSharedInputState getInputState()
	{
		return inputState;
	}

	/** set the input state for the lexer.
	 * @note state is a reference counted object, hence no reference */
	virtual void setInputState(LexerSharedInputState state)
	{
		inputState = state;
	}

	/// Set the factory for created tokens
	virtual void setTokenObjectFactory(factory_type factory)
	{
		tokenFactory = factory;
	}

	/** Test the token text against the literals table
	 * Override this method to perform a different literals test
	 */
	virtual int testLiteralsTable(int ttype) const
	{
		ANTLR_USE_NAMESPACE(std)map<ANTLR_USE_NAMESPACE(std)string,int,CharScannerLiteralsLess>::const_iterator i = literals.find(text);
		if (i != literals.end())
			ttype = (*i).second;
		return ttype;
	}

	/** Test the text passed in against the literals table
	 * Override this method to perform a different literals test
	 * This is used primarily when you want to test a portion of
	 * a token
	 */
	virtual int testLiteralsTable(const ANTLR_USE_NAMESPACE(std)string& txt,int ttype) const
	{
		ANTLR_USE_NAMESPACE(std)map<ANTLR_USE_NAMESPACE(std)string,int,CharScannerLiteralsLess>::const_iterator i = literals.find(txt);
		if (i != literals.end())
			ttype = (*i).second;
		return ttype;
	}

	/// Override this method to get more specific case handling
	virtual int toLower(int c) const
	{
		// test on EOF_CHAR for buggy (?) STLPort tolower (or HPUX tolower?)
		// also VC++ 6.0 does this. (see fix 422 (is reverted by this fix)
		// this one is more structural. Maybe make this configurable.
		return (c == EOF_CHAR ? EOF_CHAR : tolower(c));
	}

	/** This method is called by YourLexer::nextToken() when the lexer has
	 *  hit EOF condition.  EOF is NOT a character.
	 *  This method is not called if EOF is reached during
	 *  syntactic predicate evaluation or during evaluation
	 *  of normal lexical rules, which presumably would be
	 *  an IOException.  This traps the "normal" EOF condition.
	 *
	 *  uponEOF() is called after the complete evaluation of
	 *  the previous token and only if your parser asks
	 *  for another token beyond that last non-EOF token.
	 *
	 *  You might want to throw token or char stream exceptions
	 *  like: "Heh, premature eof" or a retry stream exception
	 *  ("I found the end of this file, go back to referencing file").
	 */
	virtual void uponEOF()
	{
	}

	/// Methods used to change tracing behavior
	virtual void traceIndent();
	virtual void traceIn(const char* rname);
	virtual void traceOut(const char* rname);

#ifndef NO_STATIC_CONSTS
	static const int EOF_CHAR = EOF;
#else
	enum {
		EOF_CHAR = EOF
	};
#endif
protected:
	ANTLR_USE_NAMESPACE(std)string text; ///< Text of current token
 	/// flag indicating wether consume saves characters
	bool saveConsumedInput;
	factory_type tokenFactory;				///< Factory for tokens
	bool caseSensitive; 						///< Is this lexer case sensitive
	ANTLR_USE_NAMESPACE(std)map<ANTLR_USE_NAMESPACE(std)string,int,CharScannerLiteralsLess> literals; // set by subclass

	RefToken _returnToken;		///< used to return tokens w/o using return val

	/// Input state, gives access to input stream, shared among different lexers
	LexerSharedInputState inputState;

	/** Used during filter mode to indicate that path is desired.
	 * A subsequent scan error will report an error as usual
	 * if acceptPath=true;
	 */
	bool commitToPath;

	int tabsize; 	///< tab size the scanner uses.

	/// Create a new RefToken of type t
	virtual RefToken makeToken(int t)
	{
		RefToken tok = tokenFactory();
		tok->setType(t);
		tok->setColumn(inputState->tokenStartColumn);
		tok->setLine(inputState->tokenStartLine);
		return tok;
	}

	/** Tracer class, used when -traceLexer is passed to antlr
	 */
	class Tracer {
	private:
		CharScanner* parser;
		const char* text;

		Tracer(const Tracer& other); 					// undefined
		Tracer& operator=(const Tracer& other); 	// undefined
	public:
		Tracer( CharScanner* p,const char* t )
		: parser(p), text(t)
		{
			parser->traceIn(text);
		}
		~Tracer()
		{
			parser->traceOut(text);
		}
	};

	int traceDepth;
private:
	CharScanner( const CharScanner& other ); 			  		// undefined
	CharScanner& operator=( const CharScanner& other );	// undefined

#ifndef NO_STATIC_CONSTS
	static const int NO_CHAR = 0;
#else
	enum {
		NO_CHAR = 0
	};
#endif
};

inline int CharScanner::LA(unsigned int i)
{
	int c = inputState->getInput().LA(i);

	if ( caseSensitive )
		return c;
	else
		return toLower(c);	// VC 6 tolower bug caught in toLower.
}

inline bool CharScannerLiteralsLess::operator() (const ANTLR_USE_NAMESPACE(std)string& x,const ANTLR_USE_NAMESPACE(std)string& y) const
{
	if (scanner->getCaseSensitiveLiterals())
		return ANTLR_USE_NAMESPACE(std)less<ANTLR_USE_NAMESPACE(std)string>()(x,y);
	else
	{
#ifdef NO_STRCASECMP
		return (stricmp(x.c_str(),y.c_str())<0);
#else
		return (strcasecmp(x.c_str(),y.c_str())<0);
#endif
	}
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_CharScanner_hpp__
