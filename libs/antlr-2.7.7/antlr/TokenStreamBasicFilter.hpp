#ifndef INC_TokenStreamBasicFilter_hpp__
#define INC_TokenStreamBasicFilter_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/TokenStreamBasicFilter.hpp#2 $
 */

#include <antlr/config.hpp>
#include <antlr/BitSet.hpp>
#include <antlr/TokenStream.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** This object is a TokenStream that passes through all
 *  tokens except for those that you tell it to discard.
 *  There is no buffering of the tokens.
 */
class ANTLR_API TokenStreamBasicFilter : public TokenStream {
	/** The set of token types to discard */
protected:
	BitSet discardMask;

	/** The input stream */
protected:
	TokenStream* input;

public:
	TokenStreamBasicFilter(TokenStream& input_);

	void discard(int ttype);

	void discard(const BitSet& mask);

	RefToken nextToken();
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_TokenStreamBasicFilter_hpp__
