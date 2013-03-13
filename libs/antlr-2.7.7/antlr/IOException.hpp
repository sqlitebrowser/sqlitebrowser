#ifndef INC_IOException_hpp__
#define INC_IOException_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id:$
 */

#include <antlr/config.hpp>
#include <antlr/ANTLRException.hpp>
#include <exception>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** Generic IOException used inside support code. (thrown by XML I/O routs)
 * basically this is something I'm using since a lot of compilers don't
 * support ios_base::failure.
 */
class ANTLR_API IOException : public ANTLRException
{
public:
	ANTLR_USE_NAMESPACE(std)exception io;

	IOException( ANTLR_USE_NAMESPACE(std)exception& e )
		: ANTLRException(e.what())
	{
	}
	IOException( const ANTLR_USE_NAMESPACE(std)string& mesg )
		: ANTLRException(mesg)
	{
	}
	virtual ~IOException() throw()
	{
	}
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_IOException_hpp__
