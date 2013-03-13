#ifndef INC_LexerSharedInputState_hpp__
#define INC_LexerSharedInputState_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/LexerSharedInputState.hpp#2 $
 */

#include <antlr/config.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/RefCount.hpp>
#include <antlr/CharBuffer.hpp>
#include <string>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** This object contains the data associated with an
 *  input stream of characters.  Multiple lexers
 *  share a single LexerSharedInputState to lex
 *  the same input stream.
 */
class ANTLR_API LexerInputState {
public:
	/** Construct a new LexerInputState
	 * @param inbuf the InputBuffer to read from. The object is deleted together
	 * with the LexerInputState object.
	 */
	LexerInputState(InputBuffer* inbuf)
	: column(1)
	, line(1)
	, tokenStartColumn(1)
	, tokenStartLine(1)
	, guessing(0)
	, filename("")
	, input(inbuf)
	, inputResponsible(true)
	{
	}

	/** Construct a new LexerInputState
	 * @param inbuf the InputBuffer to read from.
	 */
	LexerInputState(InputBuffer& inbuf)
	: column(1)
	, line(1)
	, tokenStartColumn(1)
	, tokenStartLine(1)
	, guessing(0)
	, filename("")
	, input(&inbuf)
	, inputResponsible(false)
	{
	}

	/** Construct a new LexerInputState
	 * @param in an istream to read from.
	 * @see antlr.CharBuffer
	 */
	LexerInputState(ANTLR_USE_NAMESPACE(std)istream& in)
	: column(1)
	, line(1)
	, tokenStartColumn(1)
	, tokenStartLine(1)
	, guessing(0)
	, filename("")
	, input(new CharBuffer(in))
	, inputResponsible(true)
	{
	}

	/** Reset the LexerInputState with a specified stream and filename.
	 * This method is a hack, dunno what I was thinking when I added it.
	 * This should actually be done in a subclass.
	 * @deprecated
	 */
	virtual void initialize( ANTLR_USE_NAMESPACE(std)istream& in, const char* file = "" )
	{
		column = 1;
		line = 1;
		tokenStartColumn = 1;
		tokenStartLine = 1;
		guessing = 0;
		filename = file;

		if( input && inputResponsible )
			delete input;

		input = new CharBuffer(in);
		inputResponsible = true;
	}

	/** Reset the LexerInputState to initial state.
	 * The underlying InputBuffer is also reset.
	 */
	virtual void reset( void )
	{
		column = 1;
		line = 1;
		tokenStartColumn = 1;
		tokenStartLine = 1;
		guessing = 0;
		input->reset();
	}

	/** Set the file position of the SharedLexerInputState.
	 * @param line_ line number to be set
	 * @param column_ column number to be set
	 */
	void setPosition( int line_, int column_ )
	{
		line = line_;
		column = column_;
	}

	virtual ~LexerInputState()
	{
		if (inputResponsible)
			delete input;
	}

	int column;
	int line;
	int tokenStartColumn;
	int tokenStartLine;
	int guessing;
	/** What file (if known) caused the problem? */
	ANTLR_USE_NAMESPACE(std)string filename;
	InputBuffer& getInput();
private:
	/// Input buffer we use
	InputBuffer* input;
	/// Who is responsible for cleaning up the InputBuffer?
	bool inputResponsible;

	// we don't want these:
	LexerInputState(const LexerInputState&);
	LexerInputState& operator=(const LexerInputState&);
};

inline InputBuffer& LexerInputState::getInput()
{
	return *input;
}

/// A reference counted LexerInputState object
typedef RefCount<LexerInputState> LexerSharedInputState;

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_LexerSharedInputState_hpp__
