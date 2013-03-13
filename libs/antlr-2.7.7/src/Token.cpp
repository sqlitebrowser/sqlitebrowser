/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/src/Token.cpp#2 $
 */

#include "antlr/Token.hpp"
#include "antlr/String.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

int Token::getColumn() const
{
	return 0;
}

int Token::getLine() const
{
	return 0;
}

ANTLR_USE_NAMESPACE(std)string Token::getText() const
{
	return "<no text>";
}

int Token::getType() const
{
	return type;
}

void Token::setColumn(int)
{
}

void Token::setLine(int)
{
}

void Token::setText(const ANTLR_USE_NAMESPACE(std)string&)
{
}

void Token::setType(int t)
{
	type = t;
}

void Token::setFilename(const ANTLR_USE_NAMESPACE(std)string&)
{
}

ANTLR_USE_NAMESPACE(std)string emptyString("");

const ANTLR_USE_NAMESPACE(std)string& Token::getFilename() const
{
	return emptyString;
}

ANTLR_USE_NAMESPACE(std)string Token::toString() const
{
	return "[\""+getText()+"\",<"+type+">]";
}

ANTLR_API RefToken nullToken;

#ifndef NO_STATIC_CONSTS
const int Token::MIN_USER_TYPE;
const int Token::NULL_TREE_LOOKAHEAD;
const int Token::INVALID_TYPE;
const int Token::EOF_TYPE;
const int Token::SKIP;
#endif

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif
