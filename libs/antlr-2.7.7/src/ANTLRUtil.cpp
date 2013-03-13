/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id:$
 */

#include <antlr/config.hpp>
#include <antlr/IOException.hpp>

#include <iostream>
#include <cctype>
#include <string>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** Eat whitespace from the input stream
 * @param is the stream to read from
 */
ANTLR_USE_NAMESPACE(std)istream& eatwhite( ANTLR_USE_NAMESPACE(std)istream& is )
{
	char c;
	while( is.get(c) )
	{
#ifdef ANTLR_CCTYPE_NEEDS_STD
		if( !ANTLR_USE_NAMESPACE(std)isspace(c) )
#else
		if( !isspace(c) )
#endif
		{
			is.putback(c);
			break;
		}
	}
	return is;
}

/** Read a string enclosed by '"' from a stream. Also handles escaping of \".
 * Skips leading whitespace.
 * @param in the istream to read from.
 * @returns the string read from file exclusive the '"'
 * @throws IOException if string is badly formatted
 */
ANTLR_USE_NAMESPACE(std)string read_string( ANTLR_USE_NAMESPACE(std)istream& in )
{
	char ch;
	ANTLR_USE_NAMESPACE(std)string ret("");
	// States for a simple state machine...
	enum { START, READING, ESCAPE, FINISHED };
	int state = START;

	eatwhite(in);

	while( state != FINISHED && in.get(ch) )
	{
		switch( state )
		{
		case START:
			// start state: check wether starting with " then switch to READING
			if( ch != '"' )
				throw IOException("string must start with '\"'");
			state = READING;
			continue;
		case READING:
			// reading state: look out for escape sequences and closing "
			if( ch == '\\' )		// got escape sequence
			{
				state = ESCAPE;
				continue;
			}
			if( ch == '"' ) 		// close quote -> stop
			{
				state = FINISHED;
				continue;
			}
			ret += ch;				// else append...
			continue;
		case ESCAPE:
			switch(ch)
			{
			case '\\':
				ret += ch;
				state = READING;
				continue;
			case '"':
				ret += ch;
				state = READING;
				continue;
			case '0':
				ret += '\0';
				state = READING;
				continue;
			default:						// unrecognized escape is not mapped
				ret += '\\';
				ret += ch;
				state = READING;
				continue;
			}
		}
	}
	if( state != FINISHED )
		throw IOException("badly formatted string: "+ret);

	return ret;
}

/* Read a ([A-Z][0-9][a-z]_)* kindoff thing. Skips leading whitespace.
 * @param in the istream to read from.
 */
ANTLR_USE_NAMESPACE(std)string read_identifier( ANTLR_USE_NAMESPACE(std)istream& in )
{
	char ch;
	ANTLR_USE_NAMESPACE(std)string ret("");

	eatwhite(in);

	while( in.get(ch) )
	{
#ifdef ANTLR_CCTYPE_NEEDS_STD
		if( ANTLR_USE_NAMESPACE(std)isupper(ch) ||
			 ANTLR_USE_NAMESPACE(std)islower(ch) ||
			 ANTLR_USE_NAMESPACE(std)isdigit(ch) ||
			 ch == '_' )
#else
		if( isupper(ch) || islower(ch) || isdigit(ch) || ch == '_' )
#endif
			ret += ch;
		else
		{
			in.putback(ch);
			break;
		}
	}
	return ret;
}

/** Read a attribute="value" thing. Leading whitespace is skipped.
 * Between attribute and '=' no whitespace is allowed. After the '=' it is
 * permitted.
 * @param in the istream to read from.
 * @param attribute string the attribute name is put in
 * @param value string the value of the attribute is put in
 * @throws IOException if something is fishy. E.g. malformed quoting
 * or missing '='
 */
void read_AttributeNValue( ANTLR_USE_NAMESPACE(std)istream& in,
									ANTLR_USE_NAMESPACE(std)string& attribute,
									ANTLR_USE_NAMESPACE(std)string& value )
{
	attribute = read_identifier(in);

	char ch;
	if( in.get(ch) && ch == '=' )
		value = read_string(in);
	else
		throw IOException("invalid attribute=value thing "+attribute);
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif
