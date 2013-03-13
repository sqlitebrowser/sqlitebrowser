#ifndef INC_CommonAST_hpp__
#define INC_CommonAST_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/CommonAST.hpp#2 $
 */

#include <antlr/config.hpp>
#include <antlr/BaseAST.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

class ANTLR_API CommonAST : public BaseAST {
public:
	CommonAST()
	: BaseAST()
	, ttype( Token::INVALID_TYPE )
	, text()
	{
	}

	CommonAST( RefToken t )
	: BaseAST()
	, ttype( t->getType() )
	, text( t->getText() )
	{
	}

	CommonAST( const CommonAST& other )
	: BaseAST(other)
	, ttype(other.ttype)
	, text(other.text)
	{
	}

	virtual ~CommonAST()
	{
	}

	virtual const char* typeName( void ) const
	{
		return CommonAST::TYPE_NAME;
	}

	/// Clone this AST node.
	virtual RefAST clone( void ) const
	{
		CommonAST *ast = new CommonAST( *this );
		return RefAST(ast);
	}

	virtual ANTLR_USE_NAMESPACE(std)string getText() const
	{
		return text;
	}
	virtual int getType() const
	{
		return ttype;
	}

	virtual void initialize( int t, const ANTLR_USE_NAMESPACE(std)string& txt )
	{
		setType(t);
		setText(txt);
	}

	virtual void initialize( RefAST t )
	{
		setType(t->getType());
		setText(t->getText());
	}
	virtual void initialize( RefToken t )
	{
		setType(t->getType());
		setText(t->getText());
	}

#ifdef ANTLR_SUPPORT_XML
	virtual void initialize( ANTLR_USE_NAMESPACE(std)istream& in );
#endif

	virtual void setText( const ANTLR_USE_NAMESPACE(std)string& txt )
	{
		text = txt;
	}
	virtual void setType( int type )
	{
		ttype = type;
	}

	static RefAST factory();

	static const char* const TYPE_NAME;
protected:
	int ttype;
	ANTLR_USE_NAMESPACE(std)string text;
};

typedef ASTRefCount<CommonAST> RefCommonAST;

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_CommonAST_hpp__
