/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/src/CommonToken.cpp#2 $
 */

#include "antlr/CommonToken.hpp"
#include "antlr/String.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

CommonToken::CommonToken() : Token(), line(1), col(1), text("")
{}

CommonToken::CommonToken(int t, const ANTLR_USE_NAMESPACE(std)string& txt)
: Token(t)
, line(1)
, col(1)
, text(txt)
{}

CommonToken::CommonToken(const ANTLR_USE_NAMESPACE(std)string& s)
: Token()
, line(1)
, col(1)
, text(s)
{}

ANTLR_USE_NAMESPACE(std)string CommonToken::toString() const
{
	return "[\""+getText()+"\",<"+getType()+">,line="+getLine()+",column="+getColumn()+"]";
}

RefToken CommonToken::factory()
{
	return RefToken(new CommonToken);
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

