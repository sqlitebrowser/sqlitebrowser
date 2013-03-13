/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/src/TokenBuffer.cpp#2 $
 */

#include "antlr/TokenBuffer.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/**A Stream of Token objects fed to the parser from a TokenStream that can
 * be rewound via mark()/rewind() methods.
 * <p>
 * A dynamic array is used to buffer up all the input tokens.  Normally,
 * "k" tokens are stored in the buffer.  More tokens may be stored during
 * guess mode (testing syntactic predicate), or when LT(i>k) is referenced.
 * Consumption of tokens is deferred.  In other words, reading the next
 * token is not done by conume(), but deferred until needed by LA or LT.
 * <p>
 *
 * @see antlr.Token
 * @see antlr.TokenStream
 * @see antlr.TokenQueue
 */

/** Create a token buffer */
TokenBuffer::TokenBuffer( TokenStream& inp )
: input(inp)
, nMarkers(0)
, markerOffset(0)
, numToConsume(0)
{
}

TokenBuffer::~TokenBuffer( void )
{
}

/** Ensure that the token buffer is sufficiently full */
void TokenBuffer::fill(unsigned int amount)
{
	syncConsume();
	// Fill the buffer sufficiently to hold needed tokens
	while (queue.entries() < (amount + markerOffset))
	{
		// Append the next token
		queue.append(input.nextToken());
	}
}

/** Get a lookahead token value */
int TokenBuffer::LA(unsigned int i)
{
	fill(i);
	return queue.elementAt(markerOffset+i-1)->getType();
}

/** Get a lookahead token */
RefToken TokenBuffer::LT(unsigned int i)
{
	fill(i);
	return queue.elementAt(markerOffset+i-1);
}

/** Return an integer marker that can be used to rewind the buffer to
 * its current state.
 */
unsigned int TokenBuffer::mark()
{
	syncConsume();
	nMarkers++;
	return markerOffset;
}

/**Rewind the token buffer to a marker.
 * @param mark Marker returned previously from mark()
 */
void TokenBuffer::rewind(unsigned int mark)
{
	syncConsume();
	markerOffset=mark;
	nMarkers--;
}

/// Get number of non-consumed tokens
unsigned int TokenBuffer::entries() const
{
	return queue.entries() - markerOffset;
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
	}
#endif
