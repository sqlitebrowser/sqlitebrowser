#ifndef INC_LLkParser_hpp__
#define INC_LLkParser_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/LLkParser.hpp#2 $
 */

#include <antlr/config.hpp>
#include <antlr/Parser.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/**An LL(k) parser.
 *
 * @see antlr.Token
 * @see antlr.TokenBuffer
 * @see antlr.LL1Parser
 */
class ANTLR_API LLkParser : public Parser {
public:
	LLkParser(const ParserSharedInputState& lexer, int k_);

	LLkParser(TokenBuffer& tokenBuf, int k_);

	LLkParser(TokenStream& lexer, int k_);

	/** Consume another token from the input stream.  Can only write sequentially!
	 * If you need 3 tokens ahead, you must consume() 3 times.
	 * <p>
	 * Note that it is possible to overwrite tokens that have not been matched.
	 * For example, calling consume() 3 times when k=2, means that the first token
	 * consumed will be overwritten with the 3rd.
	 */
	virtual inline void consume()
	{
		inputState->getInput().consume();
	}

	virtual inline int LA(unsigned int i)
	{
		return inputState->getInput().LA(i);
	}

	virtual inline RefToken LT(unsigned int i)
	{
		return inputState->getInput().LT(i);
	}
protected:
	/// the lookahead this LL(k) parser is using.
	int k;
private:
	void trace(const char* ee, const char* rname);
public:
	virtual void traceIn(const char* rname);
	virtual void traceOut(const char* rname);
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_LLkParser_hpp__
