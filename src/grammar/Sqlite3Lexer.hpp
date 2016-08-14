#ifndef INC_Sqlite3Lexer_hpp_
#define INC_Sqlite3Lexer_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.7 (20160127): "sqlite3.g" -> "Sqlite3Lexer.hpp"$ */
#include <antlr/CommonToken.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/BitSet.hpp>
#include "sqlite3TokenTypes.hpp"
#include <antlr/CharScanner.hpp>
class CUSTOM_API Sqlite3Lexer : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public sqlite3TokenTypes
{
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const
	{
		return false;
	}
public:
	Sqlite3Lexer(ANTLR_USE_NAMESPACE(std)istream& in);
	Sqlite3Lexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	Sqlite3Lexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	protected: void mDIGIT(bool _createToken);
	protected: void mDOT(bool _createToken);
	public: void mID(bool _createToken);
	public: void mQUOTEDID(bool _createToken);
	public: void mQUOTEDLITERAL(bool _createToken);
	public: void mNUMERIC(bool _createToken);
	public: void mPLUS(bool _createToken);
	public: void mMINUS(bool _createToken);
	protected: void mNL(bool _createToken);
	public: void mCOMMENT(bool _createToken);
	public: void mWS(bool _createToken);
	public: void mSTRINGLITERAL(bool _createToken);
	public: void mLPAREN(bool _createToken);
	public: void mRPAREN(bool _createToken);
	public: void mCOMMA(bool _createToken);
	public: void mSEMI(bool _createToken);
	public: void mSTAR(bool _createToken);
	public: void mTILDE(bool _createToken);
	public: void mAMPERSAND(bool _createToken);
	public: void mBITOR(bool _createToken);
	public: void mOROP(bool _createToken);
	public: void mEQUAL(bool _createToken);
	public: void mEQUAL2(bool _createToken);
	public: void mGREATER(bool _createToken);
	public: void mGREATEREQUAL(bool _createToken);
	public: void mLOWER(bool _createToken);
	public: void mLOWEREQUAL(bool _createToken);
	public: void mUNEQUAL(bool _createToken);
	public: void mUNEQUAL2(bool _createToken);
	public: void mBITWISELEFT(bool _createToken);
	public: void mBITWISERIGHT(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
	static const unsigned long _tokenSet_3_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_3;
	static const unsigned long _tokenSet_4_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_4;
};

#endif /*INC_Sqlite3Lexer_hpp_*/
