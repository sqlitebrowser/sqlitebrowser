/* $ANTLR 2.7.7 (20160127): "sqlite3.g" -> "Sqlite3Parser.cpp"$ */
#include "Sqlite3Parser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>
Sqlite3Parser::Sqlite3Parser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
}

Sqlite3Parser::Sqlite3Parser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,2)
{
}

Sqlite3Parser::Sqlite3Parser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
}

Sqlite3Parser::Sqlite3Parser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,2)
{
}

Sqlite3Parser::Sqlite3Parser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,2)
{
}

void Sqlite3Parser::id() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST id_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	switch ( LA(1)) {
	case ID:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp1_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp1_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp1_AST);
		}
		match(ID);
		id_AST = currentAST.root;
		break;
	}
	case QUOTEDID:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp2_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp2_AST);
		}
		match(QUOTEDID);
		id_AST = currentAST.root;
		break;
	}
	case QUOTEDLITERAL:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp3_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp3_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp3_AST);
		}
		match(QUOTEDLITERAL);
		id_AST = currentAST.root;
		break;
	}
	case STRINGLITERAL:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp4_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp4_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp4_AST);
		}
		match(STRINGLITERAL);
		id_AST = currentAST.root;
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = id_AST;
}

void Sqlite3Parser::databasename() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST databasename_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	id();
	if (inputState->guessing==0) {
		astFactory->addASTChild( currentAST, returnAST );
	}
	databasename_AST = currentAST.root;
	returnAST = databasename_AST;
}

void Sqlite3Parser::tablename() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST tablename_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	id();
	if (inputState->guessing==0) {
		astFactory->addASTChild( currentAST, returnAST );
	}
	tablename_AST = currentAST.root;
	returnAST = tablename_AST;
}

void Sqlite3Parser::nonkeyword_columnname() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST nonkeyword_columnname_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	id();
	if (inputState->guessing==0) {
		astFactory->addASTChild( currentAST, returnAST );
	}
	nonkeyword_columnname_AST = currentAST.root;
	returnAST = nonkeyword_columnname_AST;
}

void Sqlite3Parser::identifier() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST identifier_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	{
	if ((_tokenSet_0.member(LA(1))) && (_tokenSet_0.member(LA(2)))) {
		{
		switch ( LA(1)) {
		case ID:
		case QUOTEDID:
		case QUOTEDLITERAL:
		case STRINGLITERAL:
		{
			databasename();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		case DOT:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp5_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp5_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp5_AST);
		}
		match(DOT);
	}
	else if ((_tokenSet_1.member(LA(1))) && (LA(2) == ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE)) {
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	}
	tablename();
	if (inputState->guessing==0) {
		astFactory->addASTChild( currentAST, returnAST );
	}
	identifier_AST = currentAST.root;
	returnAST = identifier_AST;
}

void Sqlite3Parser::collationname() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST collationname_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp6_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp6_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp6_AST);
	}
	match(ID);
	collationname_AST = currentAST.root;
	returnAST = collationname_AST;
}

void Sqlite3Parser::signednumber() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST signednumber_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	{
	switch ( LA(1)) {
	case PLUS:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp7_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp7_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp7_AST);
		}
		match(PLUS);
		break;
	}
	case MINUS:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp8_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp8_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp8_AST);
		}
		match(MINUS);
		break;
	}
	case NUMERIC:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp9_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp9_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp9_AST);
	}
	match(NUMERIC);
	signednumber_AST = currentAST.root;
	returnAST = signednumber_AST;
}

void Sqlite3Parser::statementlist() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST statementlist_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	{
	switch ( LA(1)) {
	case CREATE:
	{
		statement();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	case ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE:
	case SEMI:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == SEMI)) {
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp10_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp10_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp10_AST);
			}
			match(SEMI);
			statement();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
		}
		else {
			goto _loop82;
		}
		
	}
	_loop82:;
	} // ( ... )*
	statementlist_AST = currentAST.root;
	returnAST = statementlist_AST;
}

void Sqlite3Parser::statement() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST statement_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	if ((LA(1) == CREATE) && (_tokenSet_2.member(LA(2)))) {
		createtable();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		statement_AST = currentAST.root;
	}
	else if ((LA(1) == CREATE) && (LA(2) == INDEX || LA(2) == UNIQUE)) {
		createindex();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		statement_AST = currentAST.root;
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	returnAST = statement_AST;
}

void Sqlite3Parser::createtable() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST createtable_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	if ((LA(1) == CREATE) && (LA(2) == TABLE || LA(2) == TEMPORARY || LA(2) == TEMP)) {
		{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp11_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp11_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp11_AST);
		}
		match(CREATE);
		{
		switch ( LA(1)) {
		case TEMP:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp12_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp12_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp12_AST);
			}
			match(TEMP);
			break;
		}
		case TEMPORARY:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp13_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp13_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp13_AST);
			}
			match(TEMPORARY);
			break;
		}
		case TABLE:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp14_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp14_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp14_AST);
		}
		match(TABLE);
		{
		switch ( LA(1)) {
		case IF_T:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp15_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp15_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp15_AST);
			}
			match(IF_T);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp16_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp16_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp16_AST);
			}
			match(NOT);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp17_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp17_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp17_AST);
			}
			match(EXISTS);
			break;
		}
		case ABORT:
		case ACTION:
		case ASC:
		case CASCADE:
		case CAST:
		case CONFLICT:
		case CURRENT_TIME:
		case CURRENT_DATE:
		case CURRENT_TIMESTAMP:
		case DEFERRED:
		case DESC:
		case END:
		case FAIL:
		case GLOB:
		case KEY:
		case LIKE:
		case IGNORE:
		case INITIALLY:
		case IMMEDIATE:
		case NO:
		case MATCH:
		case RAISE:
		case REGEXP:
		case REPLACE:
		case RESTRICT:
		case ROLLBACK:
		case TEMPORARY:
		case TEMP:
		case ID:
		case QUOTEDID:
		case QUOTEDLITERAL:
		case STRINGLITERAL:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case ID:
		case QUOTEDID:
		case QUOTEDLITERAL:
		case STRINGLITERAL:
		{
			tablename();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		case ABORT:
		case ACTION:
		case ASC:
		case CASCADE:
		case CAST:
		case CONFLICT:
		case CURRENT_TIME:
		case CURRENT_DATE:
		case CURRENT_TIMESTAMP:
		case DEFERRED:
		case DESC:
		case END:
		case FAIL:
		case GLOB:
		case KEY:
		case LIKE:
		case IGNORE:
		case INITIALLY:
		case IMMEDIATE:
		case NO:
		case MATCH:
		case RAISE:
		case REGEXP:
		case REPLACE:
		case RESTRICT:
		case ROLLBACK:
		case TEMPORARY:
		case TEMP:
		{
			keywordastablename();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case LPAREN:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp18_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp18_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp18_AST);
			}
			match(LPAREN);
			columndef();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			{ // ( ... )*
			for (;;) {
				if ((LA(1) == COMMA) && (_tokenSet_3.member(LA(2)))) {
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp19_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					if ( inputState->guessing == 0 ) {
						tmp19_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, tmp19_AST);
					}
					match(COMMA);
					columndef();
					if (inputState->guessing==0) {
						astFactory->addASTChild( currentAST, returnAST );
					}
				}
				else {
					goto _loop94;
				}
				
			}
			_loop94:;
			} // ( ... )*
			{ // ( ... )*
			for (;;) {
				if ((_tokenSet_4.member(LA(1)))) {
					{
					switch ( LA(1)) {
					case COMMA:
					{
						ANTLR_USE_NAMESPACE(antlr)RefAST tmp20_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
						if ( inputState->guessing == 0 ) {
							tmp20_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, tmp20_AST);
						}
						match(COMMA);
						break;
					}
					case CHECK:
					case CONSTRAINT:
					case FOREIGN:
					case PRIMARY:
					case UNIQUE:
					{
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
					tableconstraint();
					if (inputState->guessing==0) {
						astFactory->addASTChild( currentAST, returnAST );
					}
				}
				else {
					goto _loop97;
				}
				
			}
			_loop97:;
			} // ( ... )*
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp21_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp21_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp21_AST);
			}
			match(RPAREN);
			{
			switch ( LA(1)) {
			case WITHOUT:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp22_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp22_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp22_AST);
				}
				match(WITHOUT);
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp23_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp23_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp23_AST);
				}
				match(ROWID);
				break;
			}
			case ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE:
			case SEMI:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case AS:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp24_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp24_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp24_AST);
			}
			match(AS);
			selectstmt();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		if ( inputState->guessing==0 ) {
			createtable_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
			createtable_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(CREATETABLE,"CREATETABLE"))->add(createtable_AST)));
			currentAST.root = createtable_AST;
			if ( createtable_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				createtable_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = createtable_AST->getFirstChild();
			else
				currentAST.child = createtable_AST;
			currentAST.advanceChildToEnd();
		}
		}
		createtable_AST = currentAST.root;
	}
	else if ((LA(1) == CREATE) && (LA(2) == VIRTUAL)) {
		{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp25_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp25_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp25_AST);
		}
		match(CREATE);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp26_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp26_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp26_AST);
		}
		match(VIRTUAL);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp27_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp27_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp27_AST);
		}
		match(TABLE);
		{
		switch ( LA(1)) {
		case IF_T:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp28_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp28_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp28_AST);
			}
			match(IF_T);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp29_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp29_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp29_AST);
			}
			match(NOT);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp30_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp30_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp30_AST);
			}
			match(EXISTS);
			break;
		}
		case ABORT:
		case ACTION:
		case ASC:
		case CASCADE:
		case CAST:
		case CONFLICT:
		case CURRENT_TIME:
		case CURRENT_DATE:
		case CURRENT_TIMESTAMP:
		case DEFERRED:
		case DESC:
		case END:
		case FAIL:
		case GLOB:
		case KEY:
		case LIKE:
		case IGNORE:
		case INITIALLY:
		case IMMEDIATE:
		case NO:
		case MATCH:
		case RAISE:
		case REGEXP:
		case REPLACE:
		case RESTRICT:
		case ROLLBACK:
		case TEMPORARY:
		case TEMP:
		case ID:
		case QUOTEDID:
		case QUOTEDLITERAL:
		case STRINGLITERAL:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case ID:
		case QUOTEDID:
		case QUOTEDLITERAL:
		case STRINGLITERAL:
		{
			tablename();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		case ABORT:
		case ACTION:
		case ASC:
		case CASCADE:
		case CAST:
		case CONFLICT:
		case CURRENT_TIME:
		case CURRENT_DATE:
		case CURRENT_TIMESTAMP:
		case DEFERRED:
		case DESC:
		case END:
		case FAIL:
		case GLOB:
		case KEY:
		case LIKE:
		case IGNORE:
		case INITIALLY:
		case IMMEDIATE:
		case NO:
		case MATCH:
		case RAISE:
		case REGEXP:
		case REPLACE:
		case RESTRICT:
		case ROLLBACK:
		case TEMPORARY:
		case TEMP:
		{
			keywordastablename();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp31_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp31_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp31_AST);
		}
		match(USING);
		name();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		{
		switch ( LA(1)) {
		case LPAREN:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp32_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp32_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp32_AST);
			}
			match(LPAREN);
			{
			switch ( LA(1)) {
			case ABORT:
			case ACTION:
			case AUTOINCREMENT:
			case AS:
			case ASC:
			case AND:
			case OR:
			case CASCADE:
			case CASE_T:
			case CAST:
			case CREATE:
			case COLLATE:
			case CONFLICT:
			case CURRENT_TIME:
			case CURRENT_DATE:
			case CURRENT_TIMESTAMP:
			case DEFAULT:
			case DEFERRABLE:
			case DEFERRED:
			case DELETE:
			case DESC:
			case ELSE_T:
			case END:
			case ESCAPE:
			case FAIL:
			case GLOB:
			case KEY:
			case LIKE:
			case TABLE:
			case IF_T:
			case IGNORE:
			case INITIALLY:
			case IMMEDIATE:
			case IS:
			case NO:
			case NOT:
			case NULL_T:
			case MATCH:
			case EXISTS:
			case ON:
			case RAISE:
			case REFERENCES:
			case REGEXP:
			case REPLACE:
			case RESTRICT:
			case ROLLBACK:
			case ROWID:
			case SET:
			case TEMPORARY:
			case TEMP:
			case THEN:
			case UPDATE:
			case WHEN:
			case ID:
			case QUOTEDID:
			case QUOTEDLITERAL:
			case NUMERIC:
			case STRINGLITERAL:
			case LPAREN:
			case PLUS:
			case MINUS:
			case TILDE:
			{
				expr();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
				{ // ( ... )*
				for (;;) {
					if ((LA(1) == COMMA)) {
						ANTLR_USE_NAMESPACE(antlr)RefAST tmp33_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
						if ( inputState->guessing == 0 ) {
							tmp33_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, tmp33_AST);
						}
						match(COMMA);
						expr();
						if (inputState->guessing==0) {
							astFactory->addASTChild( currentAST, returnAST );
						}
					}
					else {
						goto _loop105;
					}
					
				}
				_loop105:;
				} // ( ... )*
				break;
			}
			case RPAREN:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp34_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp34_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp34_AST);
			}
			match(RPAREN);
			break;
		}
		case ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE:
		case SEMI:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		}
		createtable_AST = currentAST.root;
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	returnAST = createtable_AST;
}

void Sqlite3Parser::createindex() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST createindex_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp35_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp35_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp35_AST);
	}
	match(CREATE);
	{
	switch ( LA(1)) {
	case UNIQUE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp36_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp36_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp36_AST);
		}
		match(UNIQUE);
		break;
	}
	case INDEX:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp37_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp37_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp37_AST);
	}
	match(INDEX);
	{
	switch ( LA(1)) {
	case IF_T:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp38_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp38_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp38_AST);
		}
		match(IF_T);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp39_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp39_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp39_AST);
		}
		match(NOT);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp40_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp40_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp40_AST);
		}
		match(EXISTS);
		break;
	}
	case ABORT:
	case ACTION:
	case ASC:
	case CASCADE:
	case CAST:
	case CONFLICT:
	case CURRENT_TIME:
	case CURRENT_DATE:
	case CURRENT_TIMESTAMP:
	case DEFERRED:
	case DESC:
	case END:
	case FAIL:
	case GLOB:
	case KEY:
	case LIKE:
	case IGNORE:
	case INITIALLY:
	case IMMEDIATE:
	case NO:
	case MATCH:
	case RAISE:
	case REGEXP:
	case REPLACE:
	case RESTRICT:
	case ROLLBACK:
	case TEMPORARY:
	case TEMP:
	case ID:
	case QUOTEDID:
	case QUOTEDLITERAL:
	case STRINGLITERAL:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{
	switch ( LA(1)) {
	case ID:
	case QUOTEDID:
	case QUOTEDLITERAL:
	case STRINGLITERAL:
	{
		tablename();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	case ABORT:
	case ACTION:
	case ASC:
	case CASCADE:
	case CAST:
	case CONFLICT:
	case CURRENT_TIME:
	case CURRENT_DATE:
	case CURRENT_TIMESTAMP:
	case DEFERRED:
	case DESC:
	case END:
	case FAIL:
	case GLOB:
	case KEY:
	case LIKE:
	case IGNORE:
	case INITIALLY:
	case IMMEDIATE:
	case NO:
	case MATCH:
	case RAISE:
	case REGEXP:
	case REPLACE:
	case RESTRICT:
	case ROLLBACK:
	case TEMPORARY:
	case TEMP:
	{
		keywordastablename();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp41_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp41_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp41_AST);
	}
	match(ON);
	{
	switch ( LA(1)) {
	case ID:
	case QUOTEDID:
	case QUOTEDLITERAL:
	case STRINGLITERAL:
	{
		tablename();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	case ABORT:
	case ACTION:
	case ASC:
	case CASCADE:
	case CAST:
	case CONFLICT:
	case CURRENT_TIME:
	case CURRENT_DATE:
	case CURRENT_TIMESTAMP:
	case DEFERRED:
	case DESC:
	case END:
	case FAIL:
	case GLOB:
	case KEY:
	case LIKE:
	case IGNORE:
	case INITIALLY:
	case IMMEDIATE:
	case NO:
	case MATCH:
	case RAISE:
	case REGEXP:
	case REPLACE:
	case RESTRICT:
	case ROLLBACK:
	case TEMPORARY:
	case TEMP:
	{
		keywordastablename();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp42_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp42_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp42_AST);
	}
	match(LPAREN);
	indexedcolumn();
	if (inputState->guessing==0) {
		astFactory->addASTChild( currentAST, returnAST );
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp43_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp43_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp43_AST);
			}
			match(COMMA);
			indexedcolumn();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
		}
		else {
			goto _loop113;
		}
		
	}
	_loop113:;
	} // ( ... )*
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp44_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp44_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp44_AST);
	}
	match(RPAREN);
	{
	switch ( LA(1)) {
	case WHERE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp45_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp45_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp45_AST);
		}
		match(WHERE);
		expr();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	case ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE:
	case SEMI:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	}
	if ( inputState->guessing==0 ) {
		createindex_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		createindex_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(CREATEINDEX,"CREATEINDEX"))->add(createindex_AST)));
		currentAST.root = createindex_AST;
		if ( createindex_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			createindex_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = createindex_AST->getFirstChild();
		else
			currentAST.child = createindex_AST;
		currentAST.advanceChildToEnd();
	}
	createindex_AST = currentAST.root;
	returnAST = createindex_AST;
}

void Sqlite3Parser::create_statements() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST create_statements_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	if ((LA(1) == CREATE) && (_tokenSet_2.member(LA(2)))) {
		createtable();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		create_statements_AST = currentAST.root;
	}
	else if ((LA(1) == CREATE) && (LA(2) == INDEX || LA(2) == UNIQUE)) {
		createindex();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		create_statements_AST = currentAST.root;
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	returnAST = create_statements_AST;
}

void Sqlite3Parser::keywordastablename() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST keywordastablename_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	{
	switch ( LA(1)) {
	case ABORT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp46_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp46_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp46_AST);
		}
		match(ABORT);
		break;
	}
	case ACTION:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp47_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp47_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp47_AST);
		}
		match(ACTION);
		break;
	}
	case ASC:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp48_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp48_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp48_AST);
		}
		match(ASC);
		break;
	}
	case CASCADE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp49_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp49_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp49_AST);
		}
		match(CASCADE);
		break;
	}
	case CAST:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp50_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp50_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp50_AST);
		}
		match(CAST);
		break;
	}
	case CONFLICT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp51_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp51_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp51_AST);
		}
		match(CONFLICT);
		break;
	}
	case CURRENT_TIME:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp52_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp52_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp52_AST);
		}
		match(CURRENT_TIME);
		break;
	}
	case CURRENT_DATE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp53_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp53_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp53_AST);
		}
		match(CURRENT_DATE);
		break;
	}
	case CURRENT_TIMESTAMP:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp54_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp54_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp54_AST);
		}
		match(CURRENT_TIMESTAMP);
		break;
	}
	case DEFERRED:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp55_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp55_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp55_AST);
		}
		match(DEFERRED);
		break;
	}
	case DESC:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp56_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp56_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp56_AST);
		}
		match(DESC);
		break;
	}
	case END:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp57_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp57_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp57_AST);
		}
		match(END);
		break;
	}
	case FAIL:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp58_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp58_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp58_AST);
		}
		match(FAIL);
		break;
	}
	case GLOB:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp59_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp59_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp59_AST);
		}
		match(GLOB);
		break;
	}
	case KEY:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp60_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp60_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp60_AST);
		}
		match(KEY);
		break;
	}
	case LIKE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp61_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp61_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp61_AST);
		}
		match(LIKE);
		break;
	}
	case IGNORE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp62_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp62_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp62_AST);
		}
		match(IGNORE);
		break;
	}
	case INITIALLY:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp63_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp63_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp63_AST);
		}
		match(INITIALLY);
		break;
	}
	case IMMEDIATE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp64_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp64_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp64_AST);
		}
		match(IMMEDIATE);
		break;
	}
	case MATCH:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp65_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp65_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp65_AST);
		}
		match(MATCH);
		break;
	}
	case NO:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp66_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp66_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp66_AST);
		}
		match(NO);
		break;
	}
	case RAISE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp67_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp67_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp67_AST);
		}
		match(RAISE);
		break;
	}
	case REGEXP:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp68_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp68_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp68_AST);
		}
		match(REGEXP);
		break;
	}
	case REPLACE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp69_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp69_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp69_AST);
		}
		match(REPLACE);
		break;
	}
	case RESTRICT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp70_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp70_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp70_AST);
		}
		match(RESTRICT);
		break;
	}
	case ROLLBACK:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp71_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp71_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp71_AST);
		}
		match(ROLLBACK);
		break;
	}
	case TEMPORARY:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp72_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp72_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp72_AST);
		}
		match(TEMPORARY);
		break;
	}
	case TEMP:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp73_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp73_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp73_AST);
		}
		match(TEMP);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		keywordastablename_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		keywordastablename_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(KEYWORDASTABLENAME,"KEYWORDASTABLENAME"))->add(keywordastablename_AST)));
		currentAST.root = keywordastablename_AST;
		if ( keywordastablename_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			keywordastablename_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = keywordastablename_AST->getFirstChild();
		else
			currentAST.child = keywordastablename_AST;
		currentAST.advanceChildToEnd();
	}
	keywordastablename_AST = currentAST.root;
	returnAST = keywordastablename_AST;
}

void Sqlite3Parser::columndef() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST columndef_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	columnname();
	if (inputState->guessing==0) {
		astFactory->addASTChild( currentAST, returnAST );
	}
	{
	switch ( LA(1)) {
	case ABORT:
	case ACTION:
	case ASC:
	case CASCADE:
	case CAST:
	case CONFLICT:
	case CURRENT_TIME:
	case CURRENT_DATE:
	case CURRENT_TIMESTAMP:
	case DEFERRED:
	case DESC:
	case END:
	case FAIL:
	case GLOB:
	case KEY:
	case LIKE:
	case IGNORE:
	case INITIALLY:
	case IMMEDIATE:
	case NO:
	case MATCH:
	case RAISE:
	case REGEXP:
	case REPLACE:
	case RESTRICT:
	case ROLLBACK:
	case TEMPORARY:
	case TEMP:
	case ID:
	case QUOTEDID:
	case QUOTEDLITERAL:
	case STRINGLITERAL:
	{
		type_name();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	case CHECK:
	case COLLATE:
	case CONSTRAINT:
	case DEFAULT:
	case FOREIGN:
	case NOT:
	case NULL_T:
	case PRIMARY:
	case REFERENCES:
	case UNIQUE:
	case RPAREN:
	case COMMA:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{ // ( ... )*
	for (;;) {
		if ((_tokenSet_5.member(LA(1))) && (_tokenSet_6.member(LA(2)))) {
			columnconstraint();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
		}
		else {
			goto _loop122;
		}
		
	}
	_loop122:;
	} // ( ... )*
	if ( inputState->guessing==0 ) {
		columndef_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		columndef_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(COLUMNDEF,"COLUMNDEF"))->add(columndef_AST)));
		currentAST.root = columndef_AST;
		if ( columndef_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			columndef_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = columndef_AST->getFirstChild();
		else
			currentAST.child = columndef_AST;
		currentAST.advanceChildToEnd();
	}
	columndef_AST = currentAST.root;
	returnAST = columndef_AST;
}

void Sqlite3Parser::tableconstraint() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST tableconstraint_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	{
	switch ( LA(1)) {
	case CONSTRAINT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp74_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp74_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp74_AST);
		}
		match(CONSTRAINT);
		name();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	case CHECK:
	case FOREIGN:
	case PRIMARY:
	case UNIQUE:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{
	switch ( LA(1)) {
	case PRIMARY:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp75_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp75_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp75_AST);
		}
		match(PRIMARY);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp76_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp76_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp76_AST);
		}
		match(KEY);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp77_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp77_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp77_AST);
		}
		match(LPAREN);
		indexedcolumn();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp78_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp78_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp78_AST);
				}
				match(COMMA);
				indexedcolumn();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
			}
			else {
				goto _loop143;
			}
			
		}
		_loop143:;
		} // ( ... )*
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp79_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp79_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp79_AST);
		}
		match(RPAREN);
		{
		switch ( LA(1)) {
		case ON:
		{
			conflictclause();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		case CHECK:
		case CONSTRAINT:
		case FOREIGN:
		case PRIMARY:
		case UNIQUE:
		case RPAREN:
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	case UNIQUE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp80_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp80_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp80_AST);
		}
		match(UNIQUE);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp81_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp81_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp81_AST);
		}
		match(LPAREN);
		indexedcolumn();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp82_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp82_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp82_AST);
				}
				match(COMMA);
				indexedcolumn();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
			}
			else {
				goto _loop146;
			}
			
		}
		_loop146:;
		} // ( ... )*
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp83_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp83_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp83_AST);
		}
		match(RPAREN);
		{
		switch ( LA(1)) {
		case ON:
		{
			conflictclause();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		case CHECK:
		case CONSTRAINT:
		case FOREIGN:
		case PRIMARY:
		case UNIQUE:
		case RPAREN:
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	case CHECK:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp84_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp84_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp84_AST);
		}
		match(CHECK);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp85_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp85_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp85_AST);
		}
		match(LPAREN);
		expr();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp86_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp86_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp86_AST);
		}
		match(RPAREN);
		break;
	}
	case FOREIGN:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp87_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp87_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp87_AST);
		}
		match(FOREIGN);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp88_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp88_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp88_AST);
		}
		match(KEY);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp89_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp89_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp89_AST);
		}
		match(LPAREN);
		columnname();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp90_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp90_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp90_AST);
				}
				match(COMMA);
				columnname();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
			}
			else {
				goto _loop149;
			}
			
		}
		_loop149:;
		} // ( ... )*
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp91_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp91_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp91_AST);
		}
		match(RPAREN);
		foreignkeyclause();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		tableconstraint_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		tableconstraint_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(TABLECONSTRAINT,"TABLECONSTRAINT"))->add(tableconstraint_AST)));
		currentAST.root = tableconstraint_AST;
		if ( tableconstraint_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			tableconstraint_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = tableconstraint_AST->getFirstChild();
		else
			currentAST.child = tableconstraint_AST;
		currentAST.advanceChildToEnd();
	}
	tableconstraint_AST = currentAST.root;
	returnAST = tableconstraint_AST;
}

void Sqlite3Parser::selectstmt() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST selectstmt_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp92_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp92_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp92_AST);
	}
	match(SELECT);
	selectstmt_AST = currentAST.root;
	returnAST = selectstmt_AST;
}

void Sqlite3Parser::name() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST name_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	switch ( LA(1)) {
	case ID:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp93_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp93_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp93_AST);
		}
		match(ID);
		name_AST = currentAST.root;
		break;
	}
	case QUOTEDID:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp94_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp94_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp94_AST);
		}
		match(QUOTEDID);
		name_AST = currentAST.root;
		break;
	}
	case QUOTEDLITERAL:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp95_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp95_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp95_AST);
		}
		match(QUOTEDLITERAL);
		name_AST = currentAST.root;
		break;
	}
	case STRINGLITERAL:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp96_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp96_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp96_AST);
		}
		match(STRINGLITERAL);
		name_AST = currentAST.root;
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = name_AST;
}

void Sqlite3Parser::expr() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST expr_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	if ((LA(1) == LPAREN) && (_tokenSet_7.member(LA(2)))) {
		{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp97_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp97_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp97_AST);
		}
		match(LPAREN);
		subexpr();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		{ // ( ... )+
		int _cnt177=0;
		for (;;) {
			if ((LA(1) == COMMA)) {
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp98_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp98_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp98_AST);
				}
				match(COMMA);
				subexpr();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
			}
			else {
				if ( _cnt177>=1 ) { goto _loop177; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt177++;
		}
		_loop177:;
		}  // ( ... )+
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp99_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp99_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp99_AST);
		}
		match(RPAREN);
		binaryoperator();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp100_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp100_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp100_AST);
		}
		match(LPAREN);
		subexpr();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		{ // ( ... )+
		int _cnt179=0;
		for (;;) {
			if ((LA(1) == COMMA)) {
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp101_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp101_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp101_AST);
				}
				match(COMMA);
				subexpr();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
			}
			else {
				if ( _cnt179>=1 ) { goto _loop179; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt179++;
		}
		_loop179:;
		}  // ( ... )+
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp102_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp102_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp102_AST);
		}
		match(RPAREN);
		}
		expr_AST = currentAST.root;
	}
	else if ((_tokenSet_7.member(LA(1))) && (_tokenSet_8.member(LA(2)))) {
		{
		subexpr();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		{ // ( ... )*
		for (;;) {
			if ((_tokenSet_9.member(LA(1))) && (_tokenSet_7.member(LA(2)))) {
				{
				switch ( LA(1)) {
				case GLOB:
				case LIKE:
				case IS:
				case MATCH:
				case REGEXP:
				case PLUS:
				case MINUS:
				case STAR:
				case AMPERSAND:
				case BITOR:
				case OROP:
				case EQUAL:
				case EQUAL2:
				case GREATER:
				case GREATEREQUAL:
				case LOWER:
				case LOWEREQUAL:
				case UNEQUAL:
				case UNEQUAL2:
				case BITWISELEFT:
				case BITWISERIGHT:
				case SLASH:
				case PERCENT:
				{
					binaryoperator();
					if (inputState->guessing==0) {
						astFactory->addASTChild( currentAST, returnAST );
					}
					break;
				}
				case AND:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp103_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					if ( inputState->guessing == 0 ) {
						tmp103_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, tmp103_AST);
					}
					match(AND);
					break;
				}
				case OR:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp104_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					if ( inputState->guessing == 0 ) {
						tmp104_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, tmp104_AST);
					}
					match(OR);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				subexpr();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
			}
			else {
				goto _loop183;
			}
			
		}
		_loop183:;
		} // ( ... )*
		}
		expr_AST = currentAST.root;
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	returnAST = expr_AST;
}

void Sqlite3Parser::indexedcolumn() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST indexedcolumn_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	expr();
	if (inputState->guessing==0) {
		astFactory->addASTChild( currentAST, returnAST );
	}
	{
	switch ( LA(1)) {
	case COLLATE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp105_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp105_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp105_AST);
		}
		match(COLLATE);
		collationname();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	case AUTOINCREMENT:
	case ASC:
	case DESC:
	case RPAREN:
	case COMMA:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{
	switch ( LA(1)) {
	case ASC:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp106_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp106_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp106_AST);
		}
		match(ASC);
		break;
	}
	case DESC:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp107_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp107_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp107_AST);
		}
		match(DESC);
		break;
	}
	case AUTOINCREMENT:
	case RPAREN:
	case COMMA:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{
	switch ( LA(1)) {
	case AUTOINCREMENT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp108_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp108_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp108_AST);
		}
		match(AUTOINCREMENT);
		break;
	}
	case RPAREN:
	case COMMA:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		indexedcolumn_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		indexedcolumn_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(INDEXEDCOLUMN,"INDEXEDCOLUMN"))->add(indexedcolumn_AST)));
		currentAST.root = indexedcolumn_AST;
		if ( indexedcolumn_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			indexedcolumn_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = indexedcolumn_AST->getFirstChild();
		else
			currentAST.child = indexedcolumn_AST;
		currentAST.advanceChildToEnd();
	}
	indexedcolumn_AST = currentAST.root;
	returnAST = indexedcolumn_AST;
}

void Sqlite3Parser::keywordascolumnname() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST keywordascolumnname_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	{
	switch ( LA(1)) {
	case ABORT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp109_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp109_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp109_AST);
		}
		match(ABORT);
		break;
	}
	case ACTION:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp110_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp110_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp110_AST);
		}
		match(ACTION);
		break;
	}
	case AUTOINCREMENT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp111_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp111_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp111_AST);
		}
		match(AUTOINCREMENT);
		break;
	}
	case AS:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp112_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp112_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp112_AST);
		}
		match(AS);
		break;
	}
	case ASC:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp113_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp113_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp113_AST);
		}
		match(ASC);
		break;
	}
	case AND:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp114_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp114_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp114_AST);
		}
		match(AND);
		break;
	}
	case OR:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp115_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp115_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp115_AST);
		}
		match(OR);
		break;
	}
	case CASCADE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp116_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp116_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp116_AST);
		}
		match(CASCADE);
		break;
	}
	case CASE_T:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp117_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp117_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp117_AST);
		}
		match(CASE_T);
		break;
	}
	case CAST:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp118_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp118_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp118_AST);
		}
		match(CAST);
		break;
	}
	case CREATE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp119_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp119_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp119_AST);
		}
		match(CREATE);
		break;
	}
	case COLLATE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp120_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp120_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp120_AST);
		}
		match(COLLATE);
		break;
	}
	case CONFLICT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp121_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp121_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp121_AST);
		}
		match(CONFLICT);
		break;
	}
	case CURRENT_TIME:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp122_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp122_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp122_AST);
		}
		match(CURRENT_TIME);
		break;
	}
	case CURRENT_DATE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp123_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp123_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp123_AST);
		}
		match(CURRENT_DATE);
		break;
	}
	case CURRENT_TIMESTAMP:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp124_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp124_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp124_AST);
		}
		match(CURRENT_TIMESTAMP);
		break;
	}
	case DEFAULT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp125_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp125_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp125_AST);
		}
		match(DEFAULT);
		break;
	}
	case DEFERRABLE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp126_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp126_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp126_AST);
		}
		match(DEFERRABLE);
		break;
	}
	case DEFERRED:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp127_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp127_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp127_AST);
		}
		match(DEFERRED);
		break;
	}
	case DELETE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp128_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp128_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp128_AST);
		}
		match(DELETE);
		break;
	}
	case DESC:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp129_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp129_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp129_AST);
		}
		match(DESC);
		break;
	}
	case ELSE_T:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp130_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp130_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp130_AST);
		}
		match(ELSE_T);
		break;
	}
	case END:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp131_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp131_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp131_AST);
		}
		match(END);
		break;
	}
	case ESCAPE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp132_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp132_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp132_AST);
		}
		match(ESCAPE);
		break;
	}
	case FAIL:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp133_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp133_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp133_AST);
		}
		match(FAIL);
		break;
	}
	case GLOB:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp134_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp134_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp134_AST);
		}
		match(GLOB);
		break;
	}
	case KEY:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp135_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp135_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp135_AST);
		}
		match(KEY);
		break;
	}
	case LIKE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp136_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp136_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp136_AST);
		}
		match(LIKE);
		break;
	}
	case TABLE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp137_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp137_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp137_AST);
		}
		match(TABLE);
		break;
	}
	case IF_T:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp138_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp138_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp138_AST);
		}
		match(IF_T);
		break;
	}
	case IGNORE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp139_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp139_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp139_AST);
		}
		match(IGNORE);
		break;
	}
	case INITIALLY:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp140_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp140_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp140_AST);
		}
		match(INITIALLY);
		break;
	}
	case IMMEDIATE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp141_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp141_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp141_AST);
		}
		match(IMMEDIATE);
		break;
	}
	case IS:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp142_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp142_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp142_AST);
		}
		match(IS);
		break;
	}
	case NULL_T:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp143_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp143_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp143_AST);
		}
		match(NULL_T);
		break;
	}
	case MATCH:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp144_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp144_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp144_AST);
		}
		match(MATCH);
		break;
	}
	case EXISTS:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp145_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp145_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp145_AST);
		}
		match(EXISTS);
		break;
	}
	case NO:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp146_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp146_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp146_AST);
		}
		match(NO);
		break;
	}
	case ON:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp147_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp147_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp147_AST);
		}
		match(ON);
		break;
	}
	case RAISE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp148_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp148_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp148_AST);
		}
		match(RAISE);
		break;
	}
	case REFERENCES:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp149_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp149_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp149_AST);
		}
		match(REFERENCES);
		break;
	}
	case REGEXP:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp150_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp150_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp150_AST);
		}
		match(REGEXP);
		break;
	}
	case REPLACE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp151_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp151_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp151_AST);
		}
		match(REPLACE);
		break;
	}
	case RESTRICT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp152_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp152_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp152_AST);
		}
		match(RESTRICT);
		break;
	}
	case ROLLBACK:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp153_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp153_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp153_AST);
		}
		match(ROLLBACK);
		break;
	}
	case ROWID:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp154_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp154_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp154_AST);
		}
		match(ROWID);
		break;
	}
	case SET:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp155_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp155_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp155_AST);
		}
		match(SET);
		break;
	}
	case TEMPORARY:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp156_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp156_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp156_AST);
		}
		match(TEMPORARY);
		break;
	}
	case TEMP:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp157_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp157_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp157_AST);
		}
		match(TEMP);
		break;
	}
	case THEN:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp158_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp158_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp158_AST);
		}
		match(THEN);
		break;
	}
	case UPDATE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp159_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp159_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp159_AST);
		}
		match(UPDATE);
		break;
	}
	case WHEN:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp160_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp160_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp160_AST);
		}
		match(WHEN);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		keywordascolumnname_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		keywordascolumnname_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(KEYWORDASCOLUMNNAME,"KEYWORDASCOLUMNNAME"))->add(keywordascolumnname_AST)));
		currentAST.root = keywordascolumnname_AST;
		if ( keywordascolumnname_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			keywordascolumnname_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = keywordascolumnname_AST->getFirstChild();
		else
			currentAST.child = keywordascolumnname_AST;
		currentAST.advanceChildToEnd();
	}
	keywordascolumnname_AST = currentAST.root;
	returnAST = keywordascolumnname_AST;
}

void Sqlite3Parser::columnname() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST columnname_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	{
	switch ( LA(1)) {
	case ID:
	case QUOTEDID:
	case QUOTEDLITERAL:
	case STRINGLITERAL:
	{
		nonkeyword_columnname();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	case ABORT:
	case ACTION:
	case AUTOINCREMENT:
	case AS:
	case ASC:
	case AND:
	case OR:
	case CASCADE:
	case CASE_T:
	case CAST:
	case CREATE:
	case COLLATE:
	case CONFLICT:
	case CURRENT_TIME:
	case CURRENT_DATE:
	case CURRENT_TIMESTAMP:
	case DEFAULT:
	case DEFERRABLE:
	case DEFERRED:
	case DELETE:
	case DESC:
	case ELSE_T:
	case END:
	case ESCAPE:
	case FAIL:
	case GLOB:
	case KEY:
	case LIKE:
	case TABLE:
	case IF_T:
	case IGNORE:
	case INITIALLY:
	case IMMEDIATE:
	case IS:
	case NO:
	case NULL_T:
	case MATCH:
	case EXISTS:
	case ON:
	case RAISE:
	case REFERENCES:
	case REGEXP:
	case REPLACE:
	case RESTRICT:
	case ROLLBACK:
	case ROWID:
	case SET:
	case TEMPORARY:
	case TEMP:
	case THEN:
	case UPDATE:
	case WHEN:
	{
		keywordascolumnname();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
	}
	columnname_AST = currentAST.root;
	returnAST = columnname_AST;
}

void Sqlite3Parser::type_name() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST type_name_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	{ // ( ... )+
	int _cnt126=0;
	for (;;) {
		switch ( LA(1)) {
		case ID:
		case QUOTEDID:
		case QUOTEDLITERAL:
		case STRINGLITERAL:
		{
			name();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		case ABORT:
		case ACTION:
		case ASC:
		case CASCADE:
		case CAST:
		case CONFLICT:
		case CURRENT_TIME:
		case CURRENT_DATE:
		case CURRENT_TIMESTAMP:
		case DEFERRED:
		case DESC:
		case END:
		case FAIL:
		case GLOB:
		case KEY:
		case LIKE:
		case IGNORE:
		case INITIALLY:
		case IMMEDIATE:
		case NO:
		case MATCH:
		case RAISE:
		case REGEXP:
		case REPLACE:
		case RESTRICT:
		case ROLLBACK:
		case TEMPORARY:
		case TEMP:
		{
			keywordastablename();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		default:
		{
			if ( _cnt126>=1 ) { goto _loop126; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		}
		_cnt126++;
	}
	_loop126:;
	}  // ( ... )+
	{
	switch ( LA(1)) {
	case LPAREN:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp161_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp161_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp161_AST);
		}
		match(LPAREN);
		signednumber();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		{
		switch ( LA(1)) {
		case COMMA:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp162_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp162_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp162_AST);
			}
			match(COMMA);
			signednumber();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp163_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp163_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp163_AST);
		}
		match(RPAREN);
		break;
	}
	case CHECK:
	case COLLATE:
	case CONSTRAINT:
	case DEFAULT:
	case FOREIGN:
	case NOT:
	case NULL_T:
	case PRIMARY:
	case REFERENCES:
	case UNIQUE:
	case RPAREN:
	case COMMA:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		type_name_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		type_name_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(TYPE_NAME,"TYPE_NAME"))->add(type_name_AST)));
		currentAST.root = type_name_AST;
		if ( type_name_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			type_name_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = type_name_AST->getFirstChild();
		else
			currentAST.child = type_name_AST;
		currentAST.advanceChildToEnd();
	}
	type_name_AST = currentAST.root;
	returnAST = type_name_AST;
}

void Sqlite3Parser::columnconstraint() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST columnconstraint_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	{
	switch ( LA(1)) {
	case CONSTRAINT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp164_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp164_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp164_AST);
		}
		match(CONSTRAINT);
		name();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	case CHECK:
	case COLLATE:
	case DEFAULT:
	case NOT:
	case NULL_T:
	case PRIMARY:
	case REFERENCES:
	case UNIQUE:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{
	switch ( LA(1)) {
	case PRIMARY:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp165_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp165_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp165_AST);
		}
		match(PRIMARY);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp166_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp166_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp166_AST);
		}
		match(KEY);
		{
		switch ( LA(1)) {
		case ASC:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp167_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp167_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp167_AST);
			}
			match(ASC);
			break;
		}
		case DESC:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp168_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp168_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp168_AST);
			}
			match(DESC);
			break;
		}
		case AUTOINCREMENT:
		case CHECK:
		case COLLATE:
		case CONSTRAINT:
		case DEFAULT:
		case FOREIGN:
		case NOT:
		case NULL_T:
		case ON:
		case PRIMARY:
		case REFERENCES:
		case UNIQUE:
		case RPAREN:
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case ON:
		{
			conflictclause();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		case AUTOINCREMENT:
		case CHECK:
		case COLLATE:
		case CONSTRAINT:
		case DEFAULT:
		case FOREIGN:
		case NOT:
		case NULL_T:
		case PRIMARY:
		case REFERENCES:
		case UNIQUE:
		case RPAREN:
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case AUTOINCREMENT:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp169_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp169_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp169_AST);
			}
			match(AUTOINCREMENT);
			break;
		}
		case CHECK:
		case COLLATE:
		case CONSTRAINT:
		case DEFAULT:
		case FOREIGN:
		case NOT:
		case NULL_T:
		case PRIMARY:
		case REFERENCES:
		case UNIQUE:
		case RPAREN:
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	case NOT:
	case NULL_T:
	{
		{
		switch ( LA(1)) {
		case NOT:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp170_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp170_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp170_AST);
			}
			match(NOT);
			break;
		}
		case NULL_T:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp171_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp171_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp171_AST);
		}
		match(NULL_T);
		{
		switch ( LA(1)) {
		case ON:
		{
			conflictclause();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		case CHECK:
		case COLLATE:
		case CONSTRAINT:
		case DEFAULT:
		case FOREIGN:
		case NOT:
		case NULL_T:
		case PRIMARY:
		case REFERENCES:
		case UNIQUE:
		case RPAREN:
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	case UNIQUE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp172_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp172_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp172_AST);
		}
		match(UNIQUE);
		{
		switch ( LA(1)) {
		case ON:
		{
			conflictclause();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		case CHECK:
		case COLLATE:
		case CONSTRAINT:
		case DEFAULT:
		case FOREIGN:
		case NOT:
		case NULL_T:
		case PRIMARY:
		case REFERENCES:
		case UNIQUE:
		case RPAREN:
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	case CHECK:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp173_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp173_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp173_AST);
		}
		match(CHECK);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp174_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp174_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp174_AST);
		}
		match(LPAREN);
		expr();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp175_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp175_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp175_AST);
		}
		match(RPAREN);
		break;
	}
	case DEFAULT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp176_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp176_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp176_AST);
		}
		match(DEFAULT);
		{
		switch ( LA(1)) {
		case QUOTEDLITERAL:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp177_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp177_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp177_AST);
			}
			match(QUOTEDLITERAL);
			break;
		}
		case LPAREN:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp178_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp178_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp178_AST);
			}
			match(LPAREN);
			expr();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp179_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp179_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp179_AST);
			}
			match(RPAREN);
			break;
		}
		case ID:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp180_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp180_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp180_AST);
			}
			match(ID);
			break;
		}
		default:
			if ((_tokenSet_10.member(LA(1))) && (_tokenSet_11.member(LA(2)))) {
				literalvalue();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
			}
			else if ((_tokenSet_12.member(LA(1))) && (_tokenSet_11.member(LA(2)))) {
				keywordastablename();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
			}
			else if ((LA(1) == NUMERIC || LA(1) == PLUS || LA(1) == MINUS) && (_tokenSet_13.member(LA(2)))) {
				signednumber();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	case COLLATE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp181_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp181_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp181_AST);
		}
		match(COLLATE);
		collationname();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	case REFERENCES:
	{
		foreignkeyclause();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	if ( inputState->guessing==0 ) {
		columnconstraint_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		columnconstraint_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(COLUMNCONSTRAINT,"COLUMNCONSTRAINT"))->add(columnconstraint_AST)));
		currentAST.root = columnconstraint_AST;
		if ( columnconstraint_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			columnconstraint_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = columnconstraint_AST->getFirstChild();
		else
			currentAST.child = columnconstraint_AST;
		currentAST.advanceChildToEnd();
	}
	columnconstraint_AST = currentAST.root;
	returnAST = columnconstraint_AST;
}

void Sqlite3Parser::conflictclause() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST conflictclause_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp182_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp182_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp182_AST);
	}
	match(ON);
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp183_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp183_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp183_AST);
	}
	match(CONFLICT);
	{
	switch ( LA(1)) {
	case ROLLBACK:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp184_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp184_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp184_AST);
		}
		match(ROLLBACK);
		break;
	}
	case ABORT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp185_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp185_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp185_AST);
		}
		match(ABORT);
		break;
	}
	case FAIL:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp186_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp186_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp186_AST);
		}
		match(FAIL);
		break;
	}
	case IGNORE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp187_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp187_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp187_AST);
		}
		match(IGNORE);
		break;
	}
	case REPLACE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp188_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp188_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp188_AST);
		}
		match(REPLACE);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	conflictclause_AST = currentAST.root;
	returnAST = conflictclause_AST;
}

void Sqlite3Parser::literalvalue() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST literalvalue_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	switch ( LA(1)) {
	case NUMERIC:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp189_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp189_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp189_AST);
		}
		match(NUMERIC);
		literalvalue_AST = currentAST.root;
		break;
	}
	case STRINGLITERAL:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp190_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp190_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp190_AST);
		}
		match(STRINGLITERAL);
		literalvalue_AST = currentAST.root;
		break;
	}
	case NULL_T:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp191_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp191_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp191_AST);
		}
		match(NULL_T);
		literalvalue_AST = currentAST.root;
		break;
	}
	case CURRENT_TIME:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp192_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp192_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp192_AST);
		}
		match(CURRENT_TIME);
		literalvalue_AST = currentAST.root;
		break;
	}
	case CURRENT_DATE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp193_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp193_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp193_AST);
		}
		match(CURRENT_DATE);
		literalvalue_AST = currentAST.root;
		break;
	}
	case CURRENT_TIMESTAMP:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp194_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp194_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp194_AST);
		}
		match(CURRENT_TIMESTAMP);
		literalvalue_AST = currentAST.root;
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = literalvalue_AST;
}

void Sqlite3Parser::foreignkeyclause() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST foreignkeyclause_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp195_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp195_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp195_AST);
	}
	match(REFERENCES);
	tablename();
	if (inputState->guessing==0) {
		astFactory->addASTChild( currentAST, returnAST );
	}
	{
	switch ( LA(1)) {
	case LPAREN:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp196_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp196_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp196_AST);
		}
		match(LPAREN);
		columnname();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp197_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp197_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp197_AST);
				}
				match(COMMA);
				columnname();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
			}
			else {
				goto _loop159;
			}
			
		}
		_loop159:;
		} // ( ... )*
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp198_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp198_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp198_AST);
		}
		match(RPAREN);
		break;
	}
	case CHECK:
	case COLLATE:
	case CONSTRAINT:
	case DEFAULT:
	case DEFERRABLE:
	case FOREIGN:
	case NOT:
	case NULL_T:
	case MATCH:
	case ON:
	case PRIMARY:
	case REFERENCES:
	case UNIQUE:
	case RPAREN:
	case COMMA:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{ // ( ... )*
	for (;;) {
		switch ( LA(1)) {
		case ON:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp199_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp199_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp199_AST);
			}
			match(ON);
			{
			switch ( LA(1)) {
			case DELETE:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp200_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp200_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp200_AST);
				}
				match(DELETE);
				break;
			}
			case UPDATE:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp201_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp201_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp201_AST);
				}
				match(UPDATE);
				break;
			}
			case INSERT:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp202_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp202_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp202_AST);
				}
				match(INSERT);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{
			switch ( LA(1)) {
			case SET:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp203_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp203_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp203_AST);
				}
				match(SET);
				{
				switch ( LA(1)) {
				case NULL_T:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp204_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					if ( inputState->guessing == 0 ) {
						tmp204_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, tmp204_AST);
					}
					match(NULL_T);
					break;
				}
				case DEFAULT:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp205_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					if ( inputState->guessing == 0 ) {
						tmp205_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, tmp205_AST);
					}
					match(DEFAULT);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				break;
			}
			case CASCADE:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp206_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp206_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp206_AST);
				}
				match(CASCADE);
				break;
			}
			case RESTRICT:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp207_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp207_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp207_AST);
				}
				match(RESTRICT);
				break;
			}
			case NO:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp208_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp208_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp208_AST);
				}
				match(NO);
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp209_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp209_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp209_AST);
				}
				match(ACTION);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case MATCH:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp210_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp210_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp210_AST);
			}
			match(MATCH);
			name();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		default:
		{
			goto _loop164;
		}
		}
	}
	_loop164:;
	} // ( ... )*
	{
	bool synPredMatched167 = false;
	if (((LA(1) == NOT) && (LA(2) == DEFERRABLE))) {
		int _m167 = mark();
		synPredMatched167 = true;
		inputState->guessing++;
		try {
			{
			match(NOT);
			match(DEFERRABLE);
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
			synPredMatched167 = false;
		}
		rewind(_m167);
		inputState->guessing--;
	}
	if ( synPredMatched167 ) {
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp211_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp211_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp211_AST);
		}
		match(NOT);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp212_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp212_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp212_AST);
		}
		match(DEFERRABLE);
		{
		switch ( LA(1)) {
		case INITIALLY:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp213_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp213_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp213_AST);
			}
			match(INITIALLY);
			{
			switch ( LA(1)) {
			case DEFERRED:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp214_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp214_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp214_AST);
				}
				match(DEFERRED);
				break;
			}
			case IMMEDIATE:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp215_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp215_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp215_AST);
				}
				match(IMMEDIATE);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case CHECK:
		case COLLATE:
		case CONSTRAINT:
		case DEFAULT:
		case FOREIGN:
		case NOT:
		case NULL_T:
		case PRIMARY:
		case REFERENCES:
		case UNIQUE:
		case RPAREN:
		case COMMA:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
	}
	else if ((LA(1) == DEFERRABLE)) {
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp216_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp216_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp216_AST);
		}
		match(DEFERRABLE);
		{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp217_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp217_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp217_AST);
		}
		match(INITIALLY);
		{
		switch ( LA(1)) {
		case DEFERRED:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp218_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp218_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp218_AST);
			}
			match(DEFERRED);
			break;
		}
		case IMMEDIATE:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp219_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp219_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp219_AST);
			}
			match(IMMEDIATE);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		}
	}
	else if ((_tokenSet_11.member(LA(1))) && (_tokenSet_14.member(LA(2)))) {
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	}
	foreignkeyclause_AST = currentAST.root;
	returnAST = foreignkeyclause_AST;
}

void Sqlite3Parser::functionname() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST functionname_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	id();
	if (inputState->guessing==0) {
		astFactory->addASTChild( currentAST, returnAST );
	}
	functionname_AST = currentAST.root;
	returnAST = functionname_AST;
}

void Sqlite3Parser::subexpr() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST subexpr_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	{
	switch ( LA(1)) {
	case MINUS:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp220_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp220_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp220_AST);
		}
		match(MINUS);
		break;
	}
	case PLUS:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp221_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp221_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp221_AST);
		}
		match(PLUS);
		break;
	}
	case TILDE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp222_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp222_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp222_AST);
		}
		match(TILDE);
		break;
	}
	case NOT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp223_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp223_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp223_AST);
		}
		match(NOT);
		break;
	}
	case ABORT:
	case ACTION:
	case AUTOINCREMENT:
	case AS:
	case ASC:
	case AND:
	case OR:
	case CASCADE:
	case CASE_T:
	case CAST:
	case CREATE:
	case COLLATE:
	case CONFLICT:
	case CURRENT_TIME:
	case CURRENT_DATE:
	case CURRENT_TIMESTAMP:
	case DEFAULT:
	case DEFERRABLE:
	case DEFERRED:
	case DELETE:
	case DESC:
	case ELSE_T:
	case END:
	case ESCAPE:
	case FAIL:
	case GLOB:
	case KEY:
	case LIKE:
	case TABLE:
	case IF_T:
	case IGNORE:
	case INITIALLY:
	case IMMEDIATE:
	case IS:
	case NO:
	case NULL_T:
	case MATCH:
	case EXISTS:
	case ON:
	case RAISE:
	case REFERENCES:
	case REGEXP:
	case REPLACE:
	case RESTRICT:
	case ROLLBACK:
	case ROWID:
	case SET:
	case TEMPORARY:
	case TEMP:
	case THEN:
	case UPDATE:
	case WHEN:
	case ID:
	case QUOTEDID:
	case QUOTEDLITERAL:
	case NUMERIC:
	case STRINGLITERAL:
	case LPAREN:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{
	if ((_tokenSet_10.member(LA(1))) && (_tokenSet_15.member(LA(2)))) {
		literalvalue();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
	}
	else if ((_tokenSet_3.member(LA(1))) && (_tokenSet_16.member(LA(2)))) {
		{
		if ((_tokenSet_1.member(LA(1))) && (LA(2) == DOT)) {
			{
			if ((_tokenSet_1.member(LA(1))) && (LA(2) == DOT)) {
				databasename();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp224_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp224_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp224_AST);
				}
				match(DOT);
			}
			else if ((_tokenSet_1.member(LA(1))) && (LA(2) == DOT)) {
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			tablename();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp225_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp225_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp225_AST);
			}
			match(DOT);
		}
		else if ((_tokenSet_3.member(LA(1))) && (_tokenSet_15.member(LA(2)))) {
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		columnname();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
	}
	else if ((_tokenSet_1.member(LA(1))) && (LA(2) == LPAREN)) {
		functionname();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp226_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp226_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp226_AST);
		}
		match(LPAREN);
		{
		switch ( LA(1)) {
		case ABORT:
		case ACTION:
		case AUTOINCREMENT:
		case AS:
		case ASC:
		case AND:
		case OR:
		case CASCADE:
		case CASE_T:
		case CAST:
		case CREATE:
		case COLLATE:
		case CONFLICT:
		case CURRENT_TIME:
		case CURRENT_DATE:
		case CURRENT_TIMESTAMP:
		case DEFAULT:
		case DEFERRABLE:
		case DEFERRED:
		case DELETE:
		case DESC:
		case ELSE_T:
		case END:
		case ESCAPE:
		case FAIL:
		case GLOB:
		case KEY:
		case LIKE:
		case TABLE:
		case IF_T:
		case IGNORE:
		case INITIALLY:
		case IMMEDIATE:
		case IS:
		case NO:
		case NOT:
		case NULL_T:
		case MATCH:
		case EXISTS:
		case ON:
		case RAISE:
		case REFERENCES:
		case REGEXP:
		case REPLACE:
		case RESTRICT:
		case ROLLBACK:
		case ROWID:
		case SET:
		case TEMPORARY:
		case TEMP:
		case THEN:
		case UPDATE:
		case WHEN:
		case ID:
		case QUOTEDID:
		case QUOTEDLITERAL:
		case NUMERIC:
		case STRINGLITERAL:
		case LPAREN:
		case PLUS:
		case MINUS:
		case TILDE:
		{
			expr();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			{ // ( ... )*
			for (;;) {
				if ((LA(1) == COMMA)) {
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp227_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					if ( inputState->guessing == 0 ) {
						tmp227_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, tmp227_AST);
					}
					match(COMMA);
					expr();
					if (inputState->guessing==0) {
						astFactory->addASTChild( currentAST, returnAST );
					}
				}
				else {
					goto _loop191;
				}
				
			}
			_loop191:;
			} // ( ... )*
			break;
		}
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp228_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp228_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp228_AST);
		}
		match(RPAREN);
	}
	else if ((LA(1) == CAST) && (LA(2) == LPAREN)) {
		castexpr();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
	}
	else if ((LA(1) == EXISTS || LA(1) == LPAREN) && (_tokenSet_17.member(LA(2)))) {
		{
		switch ( LA(1)) {
		case EXISTS:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp229_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp229_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp229_AST);
			}
			match(EXISTS);
			break;
		}
		case LPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp230_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp230_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp230_AST);
		}
		match(LPAREN);
		{
		switch ( LA(1)) {
		case ABORT:
		case ACTION:
		case AUTOINCREMENT:
		case AS:
		case ASC:
		case AND:
		case OR:
		case CASCADE:
		case CASE_T:
		case CAST:
		case CREATE:
		case COLLATE:
		case CONFLICT:
		case CURRENT_TIME:
		case CURRENT_DATE:
		case CURRENT_TIMESTAMP:
		case DEFAULT:
		case DEFERRABLE:
		case DEFERRED:
		case DELETE:
		case DESC:
		case ELSE_T:
		case END:
		case ESCAPE:
		case FAIL:
		case GLOB:
		case KEY:
		case LIKE:
		case TABLE:
		case IF_T:
		case IGNORE:
		case INITIALLY:
		case IMMEDIATE:
		case IS:
		case NO:
		case NOT:
		case NULL_T:
		case MATCH:
		case EXISTS:
		case ON:
		case RAISE:
		case REFERENCES:
		case REGEXP:
		case REPLACE:
		case RESTRICT:
		case ROLLBACK:
		case ROWID:
		case SET:
		case TEMPORARY:
		case TEMP:
		case THEN:
		case UPDATE:
		case WHEN:
		case ID:
		case QUOTEDID:
		case QUOTEDLITERAL:
		case NUMERIC:
		case STRINGLITERAL:
		case LPAREN:
		case PLUS:
		case MINUS:
		case TILDE:
		{
			expr();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		case SELECT:
		{
			selectstmt();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp231_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp231_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp231_AST);
		}
		match(RPAREN);
	}
	else if ((LA(1) == CASE_T) && (_tokenSet_7.member(LA(2)))) {
		caseexpr();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
	}
	else if ((LA(1) == RAISE) && (LA(2) == LPAREN)) {
		raisefunction();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	}
	{
	if ((_tokenSet_18.member(LA(1))) && (_tokenSet_19.member(LA(2)))) {
		suffixexpr();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
	}
	else if ((_tokenSet_20.member(LA(1))) && (_tokenSet_21.member(LA(2)))) {
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	}
	subexpr_AST = currentAST.root;
	returnAST = subexpr_AST;
}

void Sqlite3Parser::binaryoperator() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST binaryoperator_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	switch ( LA(1)) {
	case OROP:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp232_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp232_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp232_AST);
		}
		match(OROP);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case STAR:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp233_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp233_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp233_AST);
		}
		match(STAR);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case SLASH:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp234_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp234_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp234_AST);
		}
		match(SLASH);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case PERCENT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp235_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp235_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp235_AST);
		}
		match(PERCENT);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case PLUS:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp236_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp236_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp236_AST);
		}
		match(PLUS);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case MINUS:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp237_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp237_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp237_AST);
		}
		match(MINUS);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case BITWISELEFT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp238_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp238_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp238_AST);
		}
		match(BITWISELEFT);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case BITWISERIGHT:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp239_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp239_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp239_AST);
		}
		match(BITWISERIGHT);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case AMPERSAND:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp240_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp240_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp240_AST);
		}
		match(AMPERSAND);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case BITOR:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp241_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp241_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp241_AST);
		}
		match(BITOR);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case LOWER:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp242_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp242_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp242_AST);
		}
		match(LOWER);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case LOWEREQUAL:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp243_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp243_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp243_AST);
		}
		match(LOWEREQUAL);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case GREATER:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp244_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp244_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp244_AST);
		}
		match(GREATER);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case GREATEREQUAL:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp245_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp245_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp245_AST);
		}
		match(GREATEREQUAL);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case EQUAL:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp246_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp246_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp246_AST);
		}
		match(EQUAL);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case EQUAL2:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp247_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp247_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp247_AST);
		}
		match(EQUAL2);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case UNEQUAL:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp248_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp248_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp248_AST);
		}
		match(UNEQUAL);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case UNEQUAL2:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp249_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp249_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp249_AST);
		}
		match(UNEQUAL2);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case IS:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp250_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp250_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp250_AST);
		}
		match(IS);
		binaryoperator_AST = currentAST.root;
		break;
	}
	case GLOB:
	case LIKE:
	case MATCH:
	case REGEXP:
	{
		like_operator();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		binaryoperator_AST = currentAST.root;
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = binaryoperator_AST;
}

void Sqlite3Parser::castexpr() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST castexpr_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp251_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp251_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp251_AST);
	}
	match(CAST);
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp252_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp252_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp252_AST);
	}
	match(LPAREN);
	expr();
	if (inputState->guessing==0) {
		astFactory->addASTChild( currentAST, returnAST );
	}
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp253_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp253_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp253_AST);
	}
	match(AS);
	type_name();
	if (inputState->guessing==0) {
		astFactory->addASTChild( currentAST, returnAST );
	}
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp254_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp254_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp254_AST);
	}
	match(RPAREN);
	castexpr_AST = currentAST.root;
	returnAST = castexpr_AST;
}

void Sqlite3Parser::caseexpr() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST caseexpr_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp255_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp255_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp255_AST);
	}
	match(CASE_T);
	{
	if ((_tokenSet_7.member(LA(1))) && (_tokenSet_22.member(LA(2)))) {
		expr();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
	}
	else if ((LA(1) == WHEN) && (_tokenSet_7.member(LA(2)))) {
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	}
	{ // ( ... )+
	int _cnt199=0;
	for (;;) {
		if ((LA(1) == WHEN)) {
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp256_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp256_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp256_AST);
			}
			match(WHEN);
			expr();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp257_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp257_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp257_AST);
			}
			match(THEN);
			expr();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
		}
		else {
			if ( _cnt199>=1 ) { goto _loop199; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt199++;
	}
	_loop199:;
	}  // ( ... )+
	{
	switch ( LA(1)) {
	case ELSE_T:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp258_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp258_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp258_AST);
		}
		match(ELSE_T);
		expr();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		break;
	}
	case END:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp259_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp259_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp259_AST);
	}
	match(END);
	caseexpr_AST = currentAST.root;
	returnAST = caseexpr_AST;
}

void Sqlite3Parser::raisefunction() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST raisefunction_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp260_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp260_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp260_AST);
	}
	match(RAISE);
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp261_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp261_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp261_AST);
	}
	match(LPAREN);
	{
	switch ( LA(1)) {
	case IGNORE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp262_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp262_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp262_AST);
		}
		match(IGNORE);
		break;
	}
	case ABORT:
	case FAIL:
	case ROLLBACK:
	{
		{
		switch ( LA(1)) {
		case ROLLBACK:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp263_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp263_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp263_AST);
			}
			match(ROLLBACK);
			break;
		}
		case ABORT:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp264_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp264_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp264_AST);
			}
			match(ABORT);
			break;
		}
		case FAIL:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp265_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp265_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp265_AST);
			}
			match(FAIL);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp266_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp266_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp266_AST);
		}
		match(COMMA);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp267_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp267_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp267_AST);
		}
		match(STRINGLITERAL);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	ANTLR_USE_NAMESPACE(antlr)RefAST tmp268_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	if ( inputState->guessing == 0 ) {
		tmp268_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp268_AST);
	}
	match(RPAREN);
	raisefunction_AST = currentAST.root;
	returnAST = raisefunction_AST;
}

void Sqlite3Parser::suffixexpr() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST suffixexpr_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	switch ( LA(1)) {
	case COLLATE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp269_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp269_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp269_AST);
		}
		match(COLLATE);
		collationname();
		if (inputState->guessing==0) {
			astFactory->addASTChild( currentAST, returnAST );
		}
		suffixexpr_AST = currentAST.root;
		break;
	}
	case BETWEEN:
	case GLOB:
	case LIKE:
	case IN:
	case NOT:
	case MATCH:
	case REGEXP:
	{
		{
		switch ( LA(1)) {
		case NOT:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp270_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp270_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp270_AST);
			}
			match(NOT);
			break;
		}
		case BETWEEN:
		case GLOB:
		case LIKE:
		case IN:
		case MATCH:
		case REGEXP:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		switch ( LA(1)) {
		case BETWEEN:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp271_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp271_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp271_AST);
			}
			match(BETWEEN);
			subexpr();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			{ // ( ... )*
			for (;;) {
				if ((_tokenSet_23.member(LA(1)))) {
					{
					switch ( LA(1)) {
					case GLOB:
					case LIKE:
					case IS:
					case MATCH:
					case REGEXP:
					case PLUS:
					case MINUS:
					case STAR:
					case AMPERSAND:
					case BITOR:
					case OROP:
					case EQUAL:
					case EQUAL2:
					case GREATER:
					case GREATEREQUAL:
					case LOWER:
					case LOWEREQUAL:
					case UNEQUAL:
					case UNEQUAL2:
					case BITWISELEFT:
					case BITWISERIGHT:
					case SLASH:
					case PERCENT:
					{
						binaryoperator();
						if (inputState->guessing==0) {
							astFactory->addASTChild( currentAST, returnAST );
						}
						break;
					}
					case OR:
					{
						ANTLR_USE_NAMESPACE(antlr)RefAST tmp272_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
						if ( inputState->guessing == 0 ) {
							tmp272_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, tmp272_AST);
						}
						match(OR);
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
					subexpr();
					if (inputState->guessing==0) {
						astFactory->addASTChild( currentAST, returnAST );
					}
				}
				else {
					goto _loop210;
				}
				
			}
			_loop210:;
			} // ( ... )*
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp273_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp273_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp273_AST);
			}
			match(AND);
			expr();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			break;
		}
		case IN:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp274_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp274_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp274_AST);
			}
			match(IN);
			{
			switch ( LA(1)) {
			case LPAREN:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp275_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp275_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp275_AST);
				}
				match(LPAREN);
				{
				switch ( LA(1)) {
				case SELECT:
				{
					selectstmt();
					if (inputState->guessing==0) {
						astFactory->addASTChild( currentAST, returnAST );
					}
					break;
				}
				case ABORT:
				case ACTION:
				case AUTOINCREMENT:
				case AS:
				case ASC:
				case AND:
				case OR:
				case CASCADE:
				case CASE_T:
				case CAST:
				case CREATE:
				case COLLATE:
				case CONFLICT:
				case CURRENT_TIME:
				case CURRENT_DATE:
				case CURRENT_TIMESTAMP:
				case DEFAULT:
				case DEFERRABLE:
				case DEFERRED:
				case DELETE:
				case DESC:
				case ELSE_T:
				case END:
				case ESCAPE:
				case FAIL:
				case GLOB:
				case KEY:
				case LIKE:
				case TABLE:
				case IF_T:
				case IGNORE:
				case INITIALLY:
				case IMMEDIATE:
				case IS:
				case NO:
				case NOT:
				case NULL_T:
				case MATCH:
				case EXISTS:
				case ON:
				case RAISE:
				case REFERENCES:
				case REGEXP:
				case REPLACE:
				case RESTRICT:
				case ROLLBACK:
				case ROWID:
				case SET:
				case TEMPORARY:
				case TEMP:
				case THEN:
				case UPDATE:
				case WHEN:
				case ID:
				case QUOTEDID:
				case QUOTEDLITERAL:
				case NUMERIC:
				case STRINGLITERAL:
				case LPAREN:
				case PLUS:
				case MINUS:
				case TILDE:
				{
					expr();
					if (inputState->guessing==0) {
						astFactory->addASTChild( currentAST, returnAST );
					}
					{ // ( ... )*
					for (;;) {
						if ((LA(1) == COMMA)) {
							ANTLR_USE_NAMESPACE(antlr)RefAST tmp276_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
							if ( inputState->guessing == 0 ) {
								tmp276_AST = astFactory->create(LT(1));
								astFactory->addASTChild(currentAST, tmp276_AST);
							}
							match(COMMA);
							expr();
							if (inputState->guessing==0) {
								astFactory->addASTChild( currentAST, returnAST );
							}
						}
						else {
							goto _loop214;
						}
						
					}
					_loop214:;
					} // ( ... )*
					break;
				}
				case RPAREN:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp277_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp277_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp277_AST);
				}
				match(RPAREN);
				break;
			}
			case ID:
			case QUOTEDID:
			case QUOTEDLITERAL:
			case STRINGLITERAL:
			{
				tablename();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		case GLOB:
		case LIKE:
		case MATCH:
		case REGEXP:
		{
			like_operator();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			subexpr();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
			{
			if ((LA(1) == ESCAPE) && (_tokenSet_7.member(LA(2)))) {
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp278_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				if ( inputState->guessing == 0 ) {
					tmp278_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp278_AST);
				}
				match(ESCAPE);
				subexpr();
				if (inputState->guessing==0) {
					astFactory->addASTChild( currentAST, returnAST );
				}
			}
			else if ((_tokenSet_20.member(LA(1))) && (_tokenSet_21.member(LA(2)))) {
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		suffixexpr_AST = currentAST.root;
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = suffixexpr_AST;
}

void Sqlite3Parser::like_operator() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST like_operator_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	switch ( LA(1)) {
	case LIKE:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp279_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp279_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp279_AST);
		}
		match(LIKE);
		like_operator_AST = currentAST.root;
		break;
	}
	case GLOB:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp280_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp280_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp280_AST);
		}
		match(GLOB);
		like_operator_AST = currentAST.root;
		break;
	}
	case REGEXP:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp281_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp281_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp281_AST);
		}
		match(REGEXP);
		like_operator_AST = currentAST.root;
		break;
	}
	case MATCH:
	{
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp282_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		if ( inputState->guessing == 0 ) {
			tmp282_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp282_AST);
		}
		match(MATCH);
		like_operator_AST = currentAST.root;
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	returnAST = like_operator_AST;
}

void Sqlite3Parser::between_subexpr() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST between_subexpr_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	subexpr();
	if (inputState->guessing==0) {
		astFactory->addASTChild( currentAST, returnAST );
	}
	{ // ( ... )+
	int _cnt204=0;
	for (;;) {
		if ((LA(1) == AND)) {
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp283_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			if ( inputState->guessing == 0 ) {
				tmp283_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp283_AST);
			}
			match(AND);
			subexpr();
			if (inputState->guessing==0) {
				astFactory->addASTChild( currentAST, returnAST );
			}
		}
		else {
			if ( _cnt204>=1 ) { goto _loop204; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt204++;
	}
	_loop204:;
	}  // ( ... )+
	between_subexpr_AST = currentAST.root;
	returnAST = between_subexpr_AST;
}

void Sqlite3Parser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory )
{
	factory.setMaxNodeType(112);
}
const char* Sqlite3Parser::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"\"ABORT\"",
	"\"ACTION\"",
	"\"AUTOINCREMENT\"",
	"\"AS\"",
	"\"ASC\"",
	"\"AND\"",
	"\"OR\"",
	"\"BETWEEN\"",
	"\"CASCADE\"",
	"\"CASE\"",
	"\"CAST\"",
	"\"CHECK\"",
	"\"CREATE\"",
	"\"COLLATE\"",
	"\"CONFLICT\"",
	"\"CONSTRAINT\"",
	"\"CURRENT_TIME\"",
	"\"CURRENT_DATE\"",
	"\"CURRENT_TIMESTAMP\"",
	"\"DEFAULT\"",
	"\"DEFERRABLE\"",
	"\"DEFERRED\"",
	"\"DELETE\"",
	"\"DESC\"",
	"\"ELSE\"",
	"\"END\"",
	"\"ESCAPE\"",
	"\"FAIL\"",
	"\"FOREIGN\"",
	"\"GLOB\"",
	"\"KEY\"",
	"\"LIKE\"",
	"\"TABLE\"",
	"\"IF\"",
	"\"IGNORE\"",
	"\"IN\"",
	"\"INDEX\"",
	"\"INITIALLY\"",
	"\"INSERT\"",
	"\"IMMEDIATE\"",
	"\"IS\"",
	"\"NO\"",
	"\"NOT\"",
	"\"NULL\"",
	"\"MATCH\"",
	"\"EXISTS\"",
	"\"ON\"",
	"\"PRIMARY\"",
	"\"RAISE\"",
	"\"REFERENCES\"",
	"\"REGEXP\"",
	"\"REPLACE\"",
	"\"RESTRICT\"",
	"\"ROLLBACK\"",
	"\"ROWID\"",
	"\"SET\"",
	"\"TEMPORARY\"",
	"\"TEMP\"",
	"\"THEN\"",
	"\"UNIQUE\"",
	"\"UPDATE\"",
	"\"USING\"",
	"\"VIRTUAL\"",
	"\"WHEN\"",
	"\"WHERE\"",
	"\"WITHOUT\"",
	"TYPE_NAME",
	"COLUMNDEF",
	"COLUMNCONSTRAINT",
	"TABLECONSTRAINT",
	"CREATETABLE",
	"CREATEINDEX",
	"INDEXEDCOLUMN",
	"KEYWORDASTABLENAME",
	"KEYWORDASCOLUMNNAME",
	"DIGIT",
	"DOT",
	"ID",
	"QUOTEDID",
	"QUOTEDLITERAL",
	"NUMERIC",
	"NL",
	"COMMENT",
	"WS",
	"STRINGLITERAL",
	"LPAREN",
	"RPAREN",
	"COMMA",
	"SEMI",
	"PLUS",
	"MINUS",
	"STAR",
	"TILDE",
	"AMPERSAND",
	"BITOR",
	"OROP",
	"EQUAL",
	"EQUAL2",
	"GREATER",
	"GREATEREQUAL",
	"LOWER",
	"LOWEREQUAL",
	"UNEQUAL",
	"UNEQUAL2",
	"BITWISELEFT",
	"BITWISERIGHT",
	"SELECT",
	"SLASH",
	"PERCENT",
	0
};

const unsigned long Sqlite3Parser::_tokenSet_0_data_[] = { 0UL, 0UL, 17760256UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// DOT ID QUOTEDID QUOTEDLITERAL STRINGLITERAL 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_0(_tokenSet_0_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_1_data_[] = { 0UL, 0UL, 17694720UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// ID QUOTEDID QUOTEDLITERAL STRINGLITERAL 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_1(_tokenSet_1_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_2_data_[] = { 0UL, 805306384UL, 4UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "TABLE" "TEMPORARY" "TEMP" "VIRTUAL" 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_2(_tokenSet_2_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_3_data_[] = { 4294408176UL, 2146941566UL, 17694729UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "ABORT" "ACTION" "AUTOINCREMENT" "AS" "ASC" "AND" "OR" "CASCADE" "CASE" 
// "CAST" "CREATE" "COLLATE" "CONFLICT" "CURRENT_TIME" "CURRENT_DATE" "CURRENT_TIMESTAMP" 
// "DEFAULT" "DEFERRABLE" "DEFERRED" "DELETE" "DESC" "ELSE" "END" "ESCAPE" 
// "FAIL" "GLOB" "KEY" "LIKE" "TABLE" "IF" "IGNORE" "INITIALLY" "IMMEDIATE" 
// "IS" "NO" "NULL" "MATCH" "EXISTS" "ON" "RAISE" "REFERENCES" "REGEXP" 
// "REPLACE" "RESTRICT" "ROLLBACK" "ROWID" "SET" "TEMPORARY" "TEMP" "THEN" 
// "UPDATE" "WHEN" ID QUOTEDID QUOTEDLITERAL STRINGLITERAL 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_3(_tokenSet_3_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_4_data_[] = { 557056UL, 2148007937UL, 134217728UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "CHECK" "CONSTRAINT" "FOREIGN" "PRIMARY" "UNIQUE" COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_4(_tokenSet_4_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_5_data_[] = { 9076736UL, 2150154240UL, 0UL, 0UL };
// "CHECK" "COLLATE" "CONSTRAINT" "DEFAULT" "NOT" "NULL" "PRIMARY" "REFERENCES" 
// "UNIQUE" 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_5(_tokenSet_5_data_,4);
const unsigned long Sqlite3Parser::_tokenSet_6_data_[] = { 2868826416UL, 3019762255UL, 1864237056UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "ABORT" "ACTION" "ASC" "CASCADE" "CAST" "CHECK" "COLLATE" "CONFLICT" 
// "CONSTRAINT" "CURRENT_TIME" "CURRENT_DATE" "CURRENT_TIMESTAMP" "DEFAULT" 
// "DEFERRED" "DESC" "END" "FAIL" "FOREIGN" "GLOB" "KEY" "LIKE" "IGNORE" 
// "INITIALLY" "IMMEDIATE" "NO" "NOT" "NULL" "MATCH" "ON" "PRIMARY" "RAISE" 
// "REFERENCES" "REGEXP" "REPLACE" "RESTRICT" "ROLLBACK" "TEMPORARY" "TEMP" 
// "UNIQUE" ID QUOTEDID QUOTEDLITERAL NUMERIC STRINGLITERAL LPAREN RPAREN 
// COMMA PLUS MINUS 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_6(_tokenSet_6_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_7_data_[] = { 4294408176UL, 2146957950UL, 1662910473UL, 1UL, 0UL, 0UL, 0UL, 0UL };
// "ABORT" "ACTION" "AUTOINCREMENT" "AS" "ASC" "AND" "OR" "CASCADE" "CASE" 
// "CAST" "CREATE" "COLLATE" "CONFLICT" "CURRENT_TIME" "CURRENT_DATE" "CURRENT_TIMESTAMP" 
// "DEFAULT" "DEFERRABLE" "DEFERRED" "DELETE" "DESC" "ELSE" "END" "ESCAPE" 
// "FAIL" "GLOB" "KEY" "LIKE" "TABLE" "IF" "IGNORE" "INITIALLY" "IMMEDIATE" 
// "IS" "NO" "NOT" "NULL" "MATCH" "EXISTS" "ON" "RAISE" "REFERENCES" "REGEXP" 
// "REPLACE" "RESTRICT" "ROLLBACK" "ROWID" "SET" "TEMPORARY" "TEMP" "THEN" 
// "UPDATE" "WHEN" ID QUOTEDID QUOTEDLITERAL NUMERIC STRINGLITERAL LPAREN 
// PLUS MINUS TILDE 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_7(_tokenSet_7_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_8_data_[] = { 4294410226UL, 2146958078UL, 4280221705UL, 131071UL, 0UL, 0UL, 0UL, 0UL };
// EOF "ABORT" "ACTION" "AUTOINCREMENT" "AS" "ASC" "AND" "OR" "BETWEEN" 
// "CASCADE" "CASE" "CAST" "CREATE" "COLLATE" "CONFLICT" "CURRENT_TIME" 
// "CURRENT_DATE" "CURRENT_TIMESTAMP" "DEFAULT" "DEFERRABLE" "DEFERRED" 
// "DELETE" "DESC" "ELSE" "END" "ESCAPE" "FAIL" "GLOB" "KEY" "LIKE" "TABLE" 
// "IF" "IGNORE" "IN" "INITIALLY" "IMMEDIATE" "IS" "NO" "NOT" "NULL" "MATCH" 
// "EXISTS" "ON" "RAISE" "REFERENCES" "REGEXP" "REPLACE" "RESTRICT" "ROLLBACK" 
// "ROWID" "SET" "TEMPORARY" "TEMP" "THEN" "UPDATE" "WHEN" DOT ID QUOTEDID 
// QUOTEDLITERAL NUMERIC STRINGLITERAL LPAREN RPAREN COMMA SEMI PLUS MINUS 
// STAR TILDE AMPERSAND BITOR OROP EQUAL EQUAL2 GREATER GREATEREQUAL LOWER 
// LOWEREQUAL UNEQUAL UNEQUAL2 BITWISELEFT BITWISERIGHT SELECT SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_8(_tokenSet_8_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_9_data_[] = { 1536UL, 4263946UL, 3758096384UL, 114686UL, 0UL, 0UL, 0UL, 0UL };
// "AND" "OR" "GLOB" "LIKE" "IS" "MATCH" "REGEXP" PLUS MINUS STAR AMPERSAND 
// BITOR OROP EQUAL EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL UNEQUAL 
// UNEQUAL2 BITWISELEFT BITWISERIGHT SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_9(_tokenSet_9_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_10_data_[] = { 7340032UL, 32768UL, 17825792UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "CURRENT_TIME" "CURRENT_DATE" "CURRENT_TIMESTAMP" "NULL" NUMERIC STRINGLITERAL 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_10(_tokenSet_10_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_11_data_[] = { 9076736UL, 2150154241UL, 201326592UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "CHECK" "COLLATE" "CONSTRAINT" "DEFAULT" "FOREIGN" "NOT" "NULL" "PRIMARY" 
// "REFERENCES" "UNIQUE" RPAREN COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_11(_tokenSet_11_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_12_data_[] = { 2859749680UL, 869345870UL, 0UL, 0UL };
// "ABORT" "ACTION" "ASC" "CASCADE" "CAST" "CONFLICT" "CURRENT_TIME" "CURRENT_DATE" 
// "CURRENT_TIMESTAMP" "DEFERRED" "DESC" "END" "FAIL" "GLOB" "KEY" "LIKE" 
// "IGNORE" "INITIALLY" "IMMEDIATE" "NO" "MATCH" "RAISE" "REGEXP" "REPLACE" 
// "RESTRICT" "ROLLBACK" "TEMPORARY" "TEMP" 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_12(_tokenSet_12_data_,4);
const unsigned long Sqlite3Parser::_tokenSet_13_data_[] = { 9076736UL, 2150154241UL, 202375168UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "CHECK" "COLLATE" "CONSTRAINT" "DEFAULT" "FOREIGN" "NOT" "NULL" "PRIMARY" 
// "REFERENCES" "UNIQUE" NUMERIC RPAREN COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_13(_tokenSet_13_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_14_data_[] = { 4294965234UL, 4294965887UL, 2132672553UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF "ABORT" "ACTION" "AUTOINCREMENT" "AS" "ASC" "AND" "OR" "CASCADE" 
// "CASE" "CAST" "CHECK" "CREATE" "COLLATE" "CONFLICT" "CONSTRAINT" "CURRENT_TIME" 
// "CURRENT_DATE" "CURRENT_TIMESTAMP" "DEFAULT" "DEFERRABLE" "DEFERRED" 
// "DELETE" "DESC" "ELSE" "END" "ESCAPE" "FAIL" "FOREIGN" "GLOB" "KEY" 
// "LIKE" "TABLE" "IF" "IGNORE" "INITIALLY" "IMMEDIATE" "IS" "NO" "NOT" 
// "NULL" "MATCH" "EXISTS" "ON" "PRIMARY" "RAISE" "REFERENCES" "REGEXP" 
// "REPLACE" "RESTRICT" "ROLLBACK" "ROWID" "SET" "TEMPORARY" "TEMP" "THEN" 
// "UNIQUE" "UPDATE" "WHEN" "WITHOUT" ID QUOTEDID QUOTEDLITERAL NUMERIC 
// STRINGLITERAL LPAREN RPAREN COMMA SEMI PLUS MINUS 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_14(_tokenSet_14_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_15_data_[] = { 2013401026UL, 1078022282UL, 4227858440UL, 114686UL, 0UL, 0UL, 0UL, 0UL };
// EOF "AUTOINCREMENT" "AS" "ASC" "AND" "OR" "BETWEEN" "COLLATE" "DESC" 
// "ELSE" "END" "ESCAPE" "GLOB" "LIKE" "IN" "IS" "NOT" "MATCH" "REGEXP" 
// "THEN" "WHEN" RPAREN COMMA SEMI PLUS MINUS STAR AMPERSAND BITOR OROP 
// EQUAL EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL UNEQUAL UNEQUAL2 
// BITWISELEFT BITWISERIGHT SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_15(_tokenSet_15_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_16_data_[] = { 2013401026UL, 1078022282UL, 4227923976UL, 114686UL, 0UL, 0UL, 0UL, 0UL };
// EOF "AUTOINCREMENT" "AS" "ASC" "AND" "OR" "BETWEEN" "COLLATE" "DESC" 
// "ELSE" "END" "ESCAPE" "GLOB" "LIKE" "IN" "IS" "NOT" "MATCH" "REGEXP" 
// "THEN" "WHEN" DOT RPAREN COMMA SEMI PLUS MINUS STAR AMPERSAND BITOR 
// OROP EQUAL EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL UNEQUAL UNEQUAL2 
// BITWISELEFT BITWISERIGHT SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_16(_tokenSet_16_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_17_data_[] = { 4294408176UL, 2146957950UL, 1662910473UL, 16385UL, 0UL, 0UL, 0UL, 0UL };
// "ABORT" "ACTION" "AUTOINCREMENT" "AS" "ASC" "AND" "OR" "CASCADE" "CASE" 
// "CAST" "CREATE" "COLLATE" "CONFLICT" "CURRENT_TIME" "CURRENT_DATE" "CURRENT_TIMESTAMP" 
// "DEFAULT" "DEFERRABLE" "DEFERRED" "DELETE" "DESC" "ELSE" "END" "ESCAPE" 
// "FAIL" "GLOB" "KEY" "LIKE" "TABLE" "IF" "IGNORE" "INITIALLY" "IMMEDIATE" 
// "IS" "NO" "NOT" "NULL" "MATCH" "EXISTS" "ON" "RAISE" "REFERENCES" "REGEXP" 
// "REPLACE" "RESTRICT" "ROLLBACK" "ROWID" "SET" "TEMPORARY" "TEMP" "THEN" 
// "UPDATE" "WHEN" ID QUOTEDID QUOTEDLITERAL NUMERIC STRINGLITERAL LPAREN 
// PLUS MINUS TILDE SELECT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_17(_tokenSet_17_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_18_data_[] = { 133120UL, 4276362UL, 0UL, 0UL };
// "BETWEEN" "COLLATE" "GLOB" "LIKE" "IN" "NOT" "MATCH" "REGEXP" 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_18(_tokenSet_18_data_,4);
const unsigned long Sqlite3Parser::_tokenSet_19_data_[] = { 4294410224UL, 2146958078UL, 1662910473UL, 1UL, 0UL, 0UL, 0UL, 0UL };
// "ABORT" "ACTION" "AUTOINCREMENT" "AS" "ASC" "AND" "OR" "BETWEEN" "CASCADE" 
// "CASE" "CAST" "CREATE" "COLLATE" "CONFLICT" "CURRENT_TIME" "CURRENT_DATE" 
// "CURRENT_TIMESTAMP" "DEFAULT" "DEFERRABLE" "DEFERRED" "DELETE" "DESC" 
// "ELSE" "END" "ESCAPE" "FAIL" "GLOB" "KEY" "LIKE" "TABLE" "IF" "IGNORE" 
// "IN" "INITIALLY" "IMMEDIATE" "IS" "NO" "NOT" "NULL" "MATCH" "EXISTS" 
// "ON" "RAISE" "REFERENCES" "REGEXP" "REPLACE" "RESTRICT" "ROLLBACK" "ROWID" 
// "SET" "TEMPORARY" "TEMP" "THEN" "UPDATE" "WHEN" ID QUOTEDID QUOTEDLITERAL 
// NUMERIC STRINGLITERAL LPAREN PLUS MINUS TILDE 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_19(_tokenSet_19_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_20_data_[] = { 2013398978UL, 1078005770UL, 4227858440UL, 114686UL, 0UL, 0UL, 0UL, 0UL };
// EOF "AUTOINCREMENT" "AS" "ASC" "AND" "OR" "COLLATE" "DESC" "ELSE" "END" 
// "ESCAPE" "GLOB" "LIKE" "IS" "MATCH" "REGEXP" "THEN" "WHEN" RPAREN COMMA 
// SEMI PLUS MINUS STAR AMPERSAND BITOR OROP EQUAL EQUAL2 GREATER GREATEREQUAL 
// LOWER LOWEREQUAL UNEQUAL UNEQUAL2 BITWISELEFT BITWISERIGHT SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_20(_tokenSet_20_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_21_data_[] = { 4294967282UL, 4294966015UL, 4280156185UL, 114687UL, 0UL, 0UL, 0UL, 0UL };
// EOF "ABORT" "ACTION" "AUTOINCREMENT" "AS" "ASC" "AND" "OR" "BETWEEN" 
// "CASCADE" "CASE" "CAST" "CHECK" "CREATE" "COLLATE" "CONFLICT" "CONSTRAINT" 
// "CURRENT_TIME" "CURRENT_DATE" "CURRENT_TIMESTAMP" "DEFAULT" "DEFERRABLE" 
// "DEFERRED" "DELETE" "DESC" "ELSE" "END" "ESCAPE" "FAIL" "FOREIGN" "GLOB" 
// "KEY" "LIKE" "TABLE" "IF" "IGNORE" "IN" "INITIALLY" "IMMEDIATE" "IS" 
// "NO" "NOT" "NULL" "MATCH" "EXISTS" "ON" "PRIMARY" "RAISE" "REFERENCES" 
// "REGEXP" "REPLACE" "RESTRICT" "ROLLBACK" "ROWID" "SET" "TEMPORARY" "TEMP" 
// "THEN" "UNIQUE" "UPDATE" "WHEN" "WHERE" ID QUOTEDID QUOTEDLITERAL NUMERIC 
// STRINGLITERAL LPAREN RPAREN COMMA SEMI PLUS MINUS STAR TILDE AMPERSAND 
// BITOR OROP EQUAL EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL UNEQUAL 
// UNEQUAL2 BITWISELEFT BITWISERIGHT SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_21(_tokenSet_21_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_22_data_[] = { 4294410224UL, 2146958078UL, 3810459657UL, 131071UL, 0UL, 0UL, 0UL, 0UL };
// "ABORT" "ACTION" "AUTOINCREMENT" "AS" "ASC" "AND" "OR" "BETWEEN" "CASCADE" 
// "CASE" "CAST" "CREATE" "COLLATE" "CONFLICT" "CURRENT_TIME" "CURRENT_DATE" 
// "CURRENT_TIMESTAMP" "DEFAULT" "DEFERRABLE" "DEFERRED" "DELETE" "DESC" 
// "ELSE" "END" "ESCAPE" "FAIL" "GLOB" "KEY" "LIKE" "TABLE" "IF" "IGNORE" 
// "IN" "INITIALLY" "IMMEDIATE" "IS" "NO" "NOT" "NULL" "MATCH" "EXISTS" 
// "ON" "RAISE" "REFERENCES" "REGEXP" "REPLACE" "RESTRICT" "ROLLBACK" "ROWID" 
// "SET" "TEMPORARY" "TEMP" "THEN" "UPDATE" "WHEN" DOT ID QUOTEDID QUOTEDLITERAL 
// NUMERIC STRINGLITERAL LPAREN PLUS MINUS STAR TILDE AMPERSAND BITOR OROP 
// EQUAL EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL UNEQUAL UNEQUAL2 
// BITWISELEFT BITWISERIGHT SELECT SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_22(_tokenSet_22_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_23_data_[] = { 1024UL, 4263946UL, 3758096384UL, 114686UL, 0UL, 0UL, 0UL, 0UL };
// "OR" "GLOB" "LIKE" "IS" "MATCH" "REGEXP" PLUS MINUS STAR AMPERSAND BITOR 
// OROP EQUAL EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL UNEQUAL UNEQUAL2 
// BITWISELEFT BITWISERIGHT SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_23(_tokenSet_23_data_,8);


