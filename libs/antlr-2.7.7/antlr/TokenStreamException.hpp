#ifndef INC_TokenStreamException_hpp__
#define INC_TokenStreamException_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/TokenStreamException.hpp#2 $
 */

#include <antlr/config.hpp>
#include <antlr/ANTLRException.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** Baseclass for exceptions thrown by classes implementing the TokenStream
 * interface.
 * @see TokenStream
 */
class ANTLR_API TokenStreamException : public ANTLRException {
public:
	TokenStreamException() 
	: ANTLRException()	
	{
	}
	TokenStreamException(const ANTLR_USE_NAMESPACE(std)string& s)
	: ANTLRException(s)
	{
	}
	virtual ~TokenStreamException() throw()
	{
	}
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_TokenStreamException_hpp__
