#ifndef INC_MismatchedCharException_hpp__
#define INC_MismatchedCharException_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/MismatchedCharException.hpp#2 $
 */

#include <antlr/config.hpp>
#include <antlr/RecognitionException.hpp>
#include <antlr/BitSet.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

class CharScanner;

class ANTLR_API MismatchedCharException : public RecognitionException {
public:
	// Types of chars
#ifndef NO_STATIC_CONSTS
	static const int CHAR = 1;
	static const int NOT_CHAR = 2;
	static const int RANGE = 3;
	static const int NOT_RANGE = 4;
	static const int SET = 5;
	static const int NOT_SET = 6;
#else
	enum {
		CHAR = 1,
		NOT_CHAR = 2,
		RANGE = 3,
		NOT_RANGE = 4,
		SET = 5,
		NOT_SET = 6
	};
#endif

public:
	// One of the above
	int mismatchType;

	// what was found on the input stream
	int foundChar;

	// For CHAR/NOT_CHAR and RANGE/NOT_RANGE
	int expecting;

	// For RANGE/NOT_RANGE (expecting is lower bound of range)
	int upper;

	// For SET/NOT_SET
	BitSet set;

protected:
	// who knows...they may want to ask scanner questions
	CharScanner* scanner;

public:
	MismatchedCharException();

	// Expected range / not range
	MismatchedCharException(
		int c,
		int lower,
		int upper_,
		bool matchNot,
		CharScanner* scanner_
	);

	// Expected token / not token
	MismatchedCharException(
		int c,
		int expecting_,
		bool matchNot,
		CharScanner* scanner_
	);

	// Expected BitSet / not BitSet
	MismatchedCharException(
		int c,
		BitSet set_,
		bool matchNot,
		CharScanner* scanner_
	);

	~MismatchedCharException() throw() {}

	/**
	 * Returns a clean error message (no line number/column information)
	 */
	ANTLR_USE_NAMESPACE(std)string getMessage() const;
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_MismatchedCharException_hpp__
