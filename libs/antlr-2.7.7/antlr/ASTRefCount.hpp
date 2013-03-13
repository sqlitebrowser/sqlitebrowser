#ifndef INC_ASTRefCount_hpp__
# define INC_ASTRefCount_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/ASTRefCount.hpp#2 $
 */

# include <antlr/config.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

	class AST;

struct ANTLR_API ASTRef
{
	AST* const ptr;
	unsigned int count;

	ASTRef(AST* p);
	~ASTRef();
	ASTRef* increment()
	{
		++count;
		return this;
	}
	bool decrement()
	{
		return (--count==0);
	}

	static ASTRef* getRef(const AST* p);
private:
	ASTRef( const ASTRef& );
	ASTRef& operator=( const ASTRef& );
};

template<class T>
	class ANTLR_API ASTRefCount
{
private:
	ASTRef* ref;

public:
	ASTRefCount(const AST* p=0)
	: ref(p ? ASTRef::getRef(p) : 0)
	{
	}
	ASTRefCount(const ASTRefCount<T>& other)
	: ref(other.ref ? other.ref->increment() : 0)
	{
	}
	~ASTRefCount()
	{
		if (ref && ref->decrement())
			delete ref;
	}
	ASTRefCount<T>& operator=(AST* other)
	{
		ASTRef* tmp = ASTRef::getRef(other);

		if (ref && ref->decrement())
			delete ref;

		ref=tmp;

		return *this;
	}
	ASTRefCount<T>& operator=(const ASTRefCount<T>& other)
	{
		if( other.ref != ref )
		{
			ASTRef* tmp = other.ref ? other.ref->increment() : 0;

			if (ref && ref->decrement())
				delete ref;

			ref=tmp;
		}
		return *this;
	}

	operator T* ()  const { return ref ? static_cast<T*>(ref->ptr) : 0; }
	T* operator->() const { return ref ? static_cast<T*>(ref->ptr) : 0; }
	T* get()        const { return ref ? static_cast<T*>(ref->ptr) : 0; }
};

typedef ASTRefCount<AST> RefAST;

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_ASTRefCount_hpp__
