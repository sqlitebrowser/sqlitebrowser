/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/src/String.cpp#2 $
 */

#include "antlr/String.hpp"

#include <cctype>

#ifdef HAS_NOT_CSTDIO_H
#include <stdio.h>
#else
#include <cstdio>
#endif

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

// wh: hack for Borland C++ 5.6
#if __BORLANDC__
  using std::sprintf;
#endif


// RK: should be using snprintf actually... (or stringstream)
ANTLR_C_USING(sprintf)

ANTLR_USE_NAMESPACE(std)string operator+( const ANTLR_USE_NAMESPACE(std)string& lhs, const int rhs )
{
	char tmp[100];
	sprintf(tmp,"%d",rhs);
	return lhs+tmp;
}

ANTLR_USE_NAMESPACE(std)string operator+( const ANTLR_USE_NAMESPACE(std)string& lhs, size_t rhs )
{
	char tmp[100];
	sprintf(tmp,"%zu",rhs);
	return lhs+tmp;
}

/** Convert character to readable string
 */
ANTLR_USE_NAMESPACE(std)string charName(int ch)
{
	if (ch == EOF)
		return "EOF";
	else
	{
		ANTLR_USE_NAMESPACE(std)string s;

		// when you think you've seen it all.. an isprint that crashes...
		ch = ch & 0xFF;
#ifdef ANTLR_CCTYPE_NEEDS_STD
		if( ANTLR_USE_NAMESPACE(std)isprint( ch ) )
#else
		if( isprint( ch ) )
#endif
		{
			s.append("'");
			s += ch;
			s.append("'");
//			s += "'"+ch+"'";
		}
		else
		{
			s += "0x";

			unsigned int t = ch >> 4;
			if( t < 10 )
				s += t | 0x30;
			else
				s += t + 0x37;
			t = ch & 0xF;
			if( t < 10 )
				s += t | 0x30;
			else
				s += t + 0x37;
		}
		return s;
	}
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

