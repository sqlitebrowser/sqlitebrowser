#ifndef INC_ANTLRException_hpp__
#define INC_ANTLRException_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/ANTLRException.hpp#2 $
 */

#include <antlr/config.hpp>
#include <string>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

class ANTLR_API ANTLRException
{
public:
	/// Create ANTLR base exception without error message
	ANTLRException() : text("")
	{
	}
	/// Create ANTLR base exception with error message
	ANTLRException(const ANTLR_USE_NAMESPACE(std)string& s)
	: text(s)
	{
	}
	virtual ~ANTLRException() throw()
	{
	}

	/** Return complete error message with line/column number info (if present)
	 * @note for your own exceptions override this one. Call getMessage from
	 * here to get the 'clean' error message stored in the text attribute.
	 */
	virtual ANTLR_USE_NAMESPACE(std)string toString() const
	{
		return text;
	}

	/** Return error message without additional info (if present)
	 * @note when making your own exceptions classes override toString
	 * and call in toString getMessage which relays the text attribute
	 * from here.
	 */
	virtual ANTLR_USE_NAMESPACE(std)string getMessage() const
	{
		return text;
	}
private:
	ANTLR_USE_NAMESPACE(std)string text;
};
#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_ANTLRException_hpp__
