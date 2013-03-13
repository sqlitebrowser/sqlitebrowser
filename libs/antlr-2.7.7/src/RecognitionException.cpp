/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/src/RecognitionException.cpp#2 $
 */

#include "antlr/RecognitionException.hpp"
#include "antlr/String.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

RecognitionException::RecognitionException()
: ANTLRException("parsing error")
, line(-1)
, column(-1)
{
}

RecognitionException::RecognitionException(const ANTLR_USE_NAMESPACE(std)string& s)
: ANTLRException(s)
, line(-1)
, column(-1)
{
}

RecognitionException::RecognitionException(const ANTLR_USE_NAMESPACE(std)string& s,
                                           const ANTLR_USE_NAMESPACE(std)string& fileName_,
                                           int line_,int column_)
: ANTLRException(s)
, fileName(fileName_)
, line(line_)
, column(column_)
{
}

ANTLR_USE_NAMESPACE(std)string RecognitionException::getFileLineColumnString() const
{
	ANTLR_USE_NAMESPACE(std)string fileLineColumnString;

	if ( fileName.length() > 0 )
		fileLineColumnString = fileName + ":";

	if ( line != -1 )
	{
		if ( fileName.length() == 0 )
			fileLineColumnString = fileLineColumnString + "line ";

		fileLineColumnString = fileLineColumnString + line;

		if ( column != -1 )
			fileLineColumnString = fileLineColumnString + ":" + column;

		fileLineColumnString = fileLineColumnString + ":";
	}

	fileLineColumnString = fileLineColumnString + " ";

	return fileLineColumnString;
}

ANTLR_USE_NAMESPACE(std)string RecognitionException::toString() const
{
	return getFileLineColumnString()+getMessage();
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif
