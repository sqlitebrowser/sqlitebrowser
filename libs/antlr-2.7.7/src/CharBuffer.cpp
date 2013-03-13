/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/src/CharBuffer.cpp#2 $
 */

#include "antlr/CharBuffer.hpp"
#include <iostream>

//#include <ios>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/* RK: Per default istream does not throw exceptions. This can be
 * enabled with:
 * stream.exceptions(ios_base::badbit|ios_base::failbit|ios_base::eofbit);
 *
 * We could try catching the bad/fail stuff. But handling eof via this is
 * not a good idea. EOF is best handled as a 'normal' character.
 *
 * So this does not work yet with gcc... Comment it until I get to a platform
 * that does..
 */

/** Create a character buffer. Enable fail and bad exceptions, if supported
 * by platform. */
CharBuffer::CharBuffer(ANTLR_USE_NAMESPACE(std)istream& input_)
: input(input_)
{
//	input.exceptions(ANTLR_USE_NAMESPACE(std)ios_base::badbit|
//						  ANTLR_USE_NAMESPACE(std)ios_base::failbit);
}

/** Get the next character from the stream. May throw CharStreamIOException
 * when something bad happens (not EOF) (if supported by platform).
 */
int CharBuffer::getChar()
{
//	try {
		return input.get();
//	}
//	catch (ANTLR_USE_NAMESPACE(std)ios_base::failure& e) {
//		throw CharStreamIOException(e);
//	}
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif
