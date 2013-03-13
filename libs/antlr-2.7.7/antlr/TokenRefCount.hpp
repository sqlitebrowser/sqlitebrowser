#ifndef INC_TokenRefCount_hpp__
# define INC_TokenRefCount_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id:$
 */

# include <antlr/config.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

class Token;

struct ANTLR_API TokenRef
{
	Token* const ptr;
	unsigned int count;

	TokenRef(Token* p);
	~TokenRef();
	TokenRef* increment()
	{
		++count;
		return this;
	}
	bool decrement()
	{
		return (--count==0);
	}

	static TokenRef* getRef(const Token* p);
private:
	TokenRef( const TokenRef& );
	TokenRef& operator=( const TokenRef& );
};

template<class T>
	class ANTLR_API TokenRefCount
{
private:
	TokenRef* ref;

public:
	TokenRefCount(const Token* p=0)
	: ref(p ? TokenRef::getRef(p) : 0)
	{
	}
	TokenRefCount(const TokenRefCount<T>& other)
	: ref(other.ref ? other.ref->increment() : 0)
	{
	}
	~TokenRefCount()
	{
		if (ref && ref->decrement())
			delete ref;
	}
	TokenRefCount<T>& operator=(Token* other)
	{
		TokenRef* tmp = TokenRef::getRef(other);

		if (ref && ref->decrement())
			delete ref;

		ref=tmp;

		return *this;
	}
	TokenRefCount<T>& operator=(const TokenRefCount<T>& other)
	{
		if( other.ref != ref )
		{
			TokenRef* tmp = other.ref ? other.ref->increment() : 0;

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

typedef TokenRefCount<Token> RefToken;

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_TokenRefCount_hpp__
