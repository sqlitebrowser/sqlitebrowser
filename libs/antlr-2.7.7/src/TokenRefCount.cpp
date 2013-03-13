/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id:$
 */
#include "antlr/TokenRefCount.hpp"
#include "antlr/Token.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

TokenRef::TokenRef(Token* p)
: ptr(p), count(1)
{
	if (p && !p->ref)
		p->ref = this;
}

TokenRef::~TokenRef()
{
	delete ptr;
}

TokenRef* TokenRef::getRef(const Token* p)
{
	if (p) {
		Token* pp = const_cast<Token*>(p);
		if (pp->ref)
			return pp->ref->increment();
		else
			return new TokenRef(pp);
	} else
		return 0;
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

