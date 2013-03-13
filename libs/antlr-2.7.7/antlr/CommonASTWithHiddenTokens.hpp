#ifndef INC_CommonASTWithHiddenTokens_hpp__
#define INC_CommonASTWithHiddenTokens_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/CommonASTWithHiddenTokens.hpp#2 $
 */

#include <antlr/config.hpp>
#include <antlr/CommonAST.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** A CommonAST whose initialization copies hidden token
 *  information from the Token used to create a node.
 */
class ANTLR_API CommonASTWithHiddenTokens : public CommonAST {
public:
	CommonASTWithHiddenTokens();
	virtual ~CommonASTWithHiddenTokens();
	virtual const char* typeName( void ) const
	{
		return CommonASTWithHiddenTokens::TYPE_NAME;
	}
	/// Clone this AST node.
	virtual RefAST clone( void ) const;

	// Borland C++ builder seems to need the decl's of the first two...
	virtual void initialize(int t,const ANTLR_USE_NAMESPACE(std)string& txt);
	virtual void initialize(RefAST t);
	virtual void initialize(RefToken t);

	virtual RefToken getHiddenAfter() const
	{
		return hiddenAfter;
	}

	virtual RefToken getHiddenBefore() const
	{
		return hiddenBefore;
	}

	static RefAST factory();

	static const char* const TYPE_NAME;
protected:
	RefToken hiddenBefore,hiddenAfter; // references to hidden tokens
};

typedef ASTRefCount<CommonASTWithHiddenTokens> RefCommonASTWithHiddenTokens;

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_CommonASTWithHiddenTokens_hpp__
