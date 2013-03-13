/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/src/CommonAST.cpp#2 $
 */
#include "antlr/config.hpp"

#include <cstdlib>
#include <iostream>

#include "antlr/CommonAST.hpp"
#include "antlr/ANTLRUtil.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

const char* const CommonAST::TYPE_NAME = "CommonAST";

#ifdef ANTLR_SUPPORT_XML
void CommonAST::initialize( ANTLR_USE_NAMESPACE(std)istream& in )
{
	ANTLR_USE_NAMESPACE(std)string t1, t2, text;

	// text
	read_AttributeNValue( in, t1, text );

	read_AttributeNValue( in, t1, t2 );
#ifdef ANTLR_ATOI_IN_STD
	int type = ANTLR_USE_NAMESPACE(std)atoi(t2.c_str());
#else
	int type = atoi(t2.c_str());
#endif

	// initialize first part of AST.
	this->initialize( type, text );
}
#endif

RefAST CommonAST::factory()
{
	return RefAST(new CommonAST);
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

