#ifndef INC_TokenStream_hpp__
#define INC_TokenStream_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/TokenStream.hpp#2 $
 */

#include <antlr/config.hpp>
#include <antlr/Token.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** This interface allows any object to pretend it is a stream
 * of tokens.
 * @author Terence Parr, MageLang Institute
 */
class ANTLR_API TokenStream {
public:
	virtual RefToken nextToken()=0;
	virtual ~TokenStream()
	{
	}
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_TokenStream_hpp__
