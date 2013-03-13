#ifndef INC_RecognitionException_hpp__
# define INC_RecognitionException_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/RecognitionException.hpp#2 $
 */

# include <antlr/config.hpp>
# include <antlr/ANTLRException.hpp>

# ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr
{
# endif
	class ANTLR_API RecognitionException : public ANTLRException
	{
	public:
		RecognitionException();
		RecognitionException(const ANTLR_USE_NAMESPACE(std)string& s);
		RecognitionException(const ANTLR_USE_NAMESPACE(std)string& s,
									const ANTLR_USE_NAMESPACE(std)string& fileName,
									int line, int column );

		virtual ~RecognitionException() throw()
		{
		}

		/// Return file where mishap occurred.
		virtual ANTLR_USE_NAMESPACE(std)string getFilename() const throw()
		{
			return fileName;
		}
		/**
		 * @return the line number that this exception happened on.
		 */
		virtual int getLine() const throw()
		{
			return line;
		}
		/**
		 * @return the column number that this exception happened on.
		 */
		virtual int getColumn() const throw()
		{
			return column;
		}

		/// Return complete error message with line/column number info (if present)
		virtual ANTLR_USE_NAMESPACE(std)string toString() const;

		/// See what file/line/column info is present and return it as a string
		virtual ANTLR_USE_NAMESPACE(std)string getFileLineColumnString() const;
	protected:
		ANTLR_USE_NAMESPACE(std)string fileName; // not used by treeparsers
		int line;    // not used by treeparsers
		int column;  // not used by treeparsers
	};

# ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
# endif

#endif //INC_RecognitionException_hpp__
