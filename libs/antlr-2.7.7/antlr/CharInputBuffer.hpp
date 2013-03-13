#ifndef INC_CharInputBuffer_hpp__
# define INC_CharInputBuffer_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id:$
 */

# include <antlr/config.hpp>
# include <antlr/InputBuffer.hpp>

# ifdef HAS_NOT_CCTYPE_H
#	include <ctype.h>
# else
#	include <cctype>
# endif

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** CharInputBuffer.hpp provides an InputBuffer for plain character arrays (buffers).
 */
class CharInputBuffer : public InputBuffer
{
public:
	/** Construct a CharInputBuffer.hpp object with a char* buffer of 'size'
	 * if 'owner' is true, then the buffer will be delete[]-ed on destruction.
	 * @note it is assumed the buffer was allocated with new[]!
	 */
	CharInputBuffer( unsigned char* buf, size_t size, bool owner = false )
	: buffer(buf)
	, ptr(buf)
	, end(buf + size)
	, delete_buffer(owner)
	{
	}

	/** Destructor
	 * @note If you're using malloced data, then you probably need to change
	 * this destructor. Or better use this class as template for your own.
	 */
	~CharInputBuffer( void )
	{
		if( delete_buffer && buffer )
			delete [] buffer;
	}

	/** Reset the CharInputBuffer to initial state
	 * Called from LexerInputState::reset.
	 * @see LexerInputState
	 */
	virtual inline void reset( void )
	{
		InputBuffer::reset();
		ptr = buffer;
	}

	virtual int getChar( void )
	{
		return (ptr < end) ? *ptr++ : EOF;
	}

protected:
	unsigned char* buffer;	///< the buffer with data
	unsigned char* ptr;		///< position ptr into the buffer
	unsigned char* end;		///< end sentry for buffer
	bool delete_buffer;		///< flag signifying if we have to delete the buffer
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif
