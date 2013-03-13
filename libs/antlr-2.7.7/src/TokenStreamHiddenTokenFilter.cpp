/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/src/TokenStreamHiddenTokenFilter.cpp#2 $
 */
#include "antlr/TokenStreamHiddenTokenFilter.hpp"
#include "antlr/CommonHiddenStreamToken.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/**This object filters a token stream coming from a lexer
 * or another TokenStream so that only certain token channels
 * get transmitted to the parser.
 *
 * Any of the channels can be filtered off as "hidden" channels whose
 * tokens can be accessed from the parser.
 */

TokenStreamHiddenTokenFilter::TokenStreamHiddenTokenFilter(TokenStream& input)
: TokenStreamBasicFilter(input)
{
}

void TokenStreamHiddenTokenFilter::consume()
{
	nextMonitoredToken = input->nextToken();
}

void TokenStreamHiddenTokenFilter::consumeFirst()
{
	consume();

	// Handle situation where hidden or discarded tokens
	// appear first in input stream
	RefToken p;
	// while hidden or discarded scarf tokens
	while ( hideMask.member(LA(1)->getType()) || discardMask.member(LA(1)->getType()) ) {
		if ( hideMask.member(LA(1)->getType()) ) {
			if ( !p ) {
				p = LA(1);
			}
			else {
				static_cast<CommonHiddenStreamToken*>(p.get())->setHiddenAfter(LA(1));
				static_cast<CommonHiddenStreamToken*>(LA(1).get())->setHiddenBefore(p); // double-link
				p = LA(1);
			}
			lastHiddenToken = p;
			if (!firstHidden)
				firstHidden = p; // record hidden token if first
		}
		consume();
	}
}

BitSet TokenStreamHiddenTokenFilter::getDiscardMask() const
{
	return discardMask;
}

/** Return a ptr to the hidden token appearing immediately after
 *  token t in the input stream.
 */
RefToken TokenStreamHiddenTokenFilter::getHiddenAfter(RefToken t)
{
	return static_cast<CommonHiddenStreamToken*>(t.get())->getHiddenAfter();
}

/** Return a ptr to the hidden token appearing immediately before
 *  token t in the input stream.
 */
RefToken TokenStreamHiddenTokenFilter::getHiddenBefore(RefToken t)
{
	return static_cast<CommonHiddenStreamToken*>(t.get())->getHiddenBefore();
}

BitSet TokenStreamHiddenTokenFilter::getHideMask() const
{
	return hideMask;
}

/** Return the first hidden token if one appears
 *  before any monitored token.
 */
RefToken TokenStreamHiddenTokenFilter::getInitialHiddenToken()
{
	return firstHidden;
}

void TokenStreamHiddenTokenFilter::hide(int m)
{
	hideMask.add(m);
}

void TokenStreamHiddenTokenFilter::hide(const BitSet& mask)
{
	hideMask = mask;
}

RefToken TokenStreamHiddenTokenFilter::LA(int)
{
	return nextMonitoredToken;
}

/** Return the next monitored token.
*  Test the token following the monitored token.
*  If following is another monitored token, save it
*  for the next invocation of nextToken (like a single
*  lookahead token) and return it then.
*  If following is unmonitored, nondiscarded (hidden)
*  channel token, add it to the monitored token.
*
*  Note: EOF must be a monitored Token.
*/
RefToken TokenStreamHiddenTokenFilter::nextToken()
{
	// handle an initial condition; don't want to get lookahead
	// token of this splitter until first call to nextToken
	if ( !LA(1) ) {
		consumeFirst();
	}

	// we always consume hidden tokens after monitored, thus,
	// upon entry LA(1) is a monitored token.
	RefToken monitored = LA(1);
	// point to hidden tokens found during last invocation
	static_cast<CommonHiddenStreamToken*>(monitored.get())->setHiddenBefore(lastHiddenToken);
	lastHiddenToken = nullToken;

	// Look for hidden tokens, hook them into list emanating
	// from the monitored tokens.
	consume();
	RefToken p = monitored;
	// while hidden or discarded scarf tokens
	while ( hideMask.member(LA(1)->getType()) || discardMask.member(LA(1)->getType()) ) {
		if ( hideMask.member(LA(1)->getType()) ) {
			// attach the hidden token to the monitored in a chain
			// link forwards
			static_cast<CommonHiddenStreamToken*>(p.get())->setHiddenAfter(LA(1));
			// link backwards
			if (p != monitored) { //hidden cannot point to monitored tokens
				static_cast<CommonHiddenStreamToken*>(LA(1).get())->setHiddenBefore(p);
			}
			p = lastHiddenToken = LA(1);
		}
		consume();
	}
	return monitored;
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

