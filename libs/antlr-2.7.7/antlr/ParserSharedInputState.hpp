#ifndef INC_ParserSharedInputState_hpp__
#define INC_ParserSharedInputState_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/ParserSharedInputState.hpp#2 $
 */

#include <antlr/config.hpp>
#include <antlr/TokenBuffer.hpp>
#include <antlr/RefCount.hpp>
#include <string>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** This object contains the data associated with an
 * input stream of tokens.  Multiple parsers
 * share a single ParserSharedInputState to parse
 * the same stream of tokens.
 */
class ANTLR_API ParserInputState {
public:
	/** Construct a new ParserInputState
	 * @param in the TokenBuffer to read from. The object is deleted together
	 * with the ParserInputState object.
	 */
	ParserInputState( TokenBuffer* in )
	: guessing(0)
	, filename()
	, input(in)
	, inputResponsible(true)
	{
	}
	/** Construct a new ParserInputState
	 * @param in the TokenBuffer to read from.
	 */
	ParserInputState( TokenBuffer& in )
	: guessing(0)
	, filename("")
	, input(&in)
	, inputResponsible(false)
	{
	}

	virtual ~ParserInputState()
	{
		if (inputResponsible)
			delete input;
	}

	TokenBuffer& getInput( void )
	{
		return *input;
	}

	/// Reset the ParserInputState and the underlying TokenBuffer
	void reset( void )
	{
		input->reset();
		guessing = 0;
	}

public:
	/** Are we guessing (guessing>0)? */
	int guessing;
	/** What file (if known) caused the problem?
	 * @todo wrap this one..
	 */
	ANTLR_USE_NAMESPACE(std)string filename;
private:
	/** Where to get token objects */
	TokenBuffer* input;
	/// Do we need to free the TokenBuffer or is it owned by another..
	bool inputResponsible;

	// we don't want these:
	ParserInputState(const ParserInputState&);
	ParserInputState& operator=(const ParserInputState&);
};

/// A reference counted ParserInputState
typedef RefCount<ParserInputState> ParserSharedInputState;

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_ParserSharedInputState_hpp__
