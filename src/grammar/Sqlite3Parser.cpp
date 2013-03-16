/* $ANTLR 2.7.7 (20121001): "sqlite3.g" -> "Sqlite3Parser.cpp"$ */
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
	
	try {      // for error handling
		switch ( LA(1)) {
		case ID:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp1_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp1_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp1_AST);
			match(ID);
			id_AST = currentAST.root;
			break;
		}
		case QUOTEDID:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp2_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp2_AST);
			match(QUOTEDID);
			id_AST = currentAST.root;
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_0);
	}
	returnAST = id_AST;
}

void Sqlite3Parser::databasename() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST databasename_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		id();
		astFactory->addASTChild( currentAST, returnAST );
		databasename_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_1);
	}
	returnAST = databasename_AST;
}

void Sqlite3Parser::tablename() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST tablename_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		id();
		astFactory->addASTChild( currentAST, returnAST );
		tablename_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_2);
	}
	returnAST = tablename_AST;
}

void Sqlite3Parser::columnname() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST columnname_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		id();
		astFactory->addASTChild( currentAST, returnAST );
		columnname_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_3);
	}
	returnAST = columnname_AST;
}

void Sqlite3Parser::identifier() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST identifier_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		{
		if (((LA(1) >= DOT && LA(1) <= QUOTEDID)) && ((LA(2) >= DOT && LA(2) <= QUOTEDID))) {
			{
			switch ( LA(1)) {
			case ID:
			case QUOTEDID:
			{
				databasename();
				astFactory->addASTChild( currentAST, returnAST );
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
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp3_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp3_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp3_AST);
			match(DOT);
		}
		else if ((LA(1) == ID || LA(1) == QUOTEDID) && (LA(2) == ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE)) {
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		tablename();
		astFactory->addASTChild( currentAST, returnAST );
		identifier_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_4);
	}
	returnAST = identifier_AST;
}

void Sqlite3Parser::collationname() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST collationname_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp4_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp4_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp4_AST);
		match(ID);
		collationname_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_5);
	}
	returnAST = collationname_AST;
}

void Sqlite3Parser::signednumber() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST signednumber_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case PLUS:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp5_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp5_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp5_AST);
			match(PLUS);
			break;
		}
		case MINUS:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp6_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp6_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp6_AST);
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
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp7_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp7_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp7_AST);
		match(NUMERIC);
		signednumber_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_5);
	}
	returnAST = signednumber_AST;
}

void Sqlite3Parser::statementlist() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST statementlist_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case CREATE:
		{
			statement();
			astFactory->addASTChild( currentAST, returnAST );
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
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp8_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp8_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp8_AST);
				match(SEMI);
				statement();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop77;
			}
			
		}
		_loop77:;
		} // ( ... )*
		statementlist_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_4);
	}
	returnAST = statementlist_AST;
}

void Sqlite3Parser::statement() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST statement_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		createtable();
		astFactory->addASTChild( currentAST, returnAST );
		statement_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_6);
	}
	returnAST = statement_AST;
}

void Sqlite3Parser::createtable() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST createtable_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp9_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp9_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp9_AST);
		match(CREATE);
		{
		switch ( LA(1)) {
		case TEMP:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp10_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp10_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp10_AST);
			match(TEMP);
			break;
		}
		case TEMPORARY:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp11_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp11_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp11_AST);
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
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp12_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp12_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp12_AST);
		match(TABLE);
		{
		switch ( LA(1)) {
		case IF_T:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp13_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp13_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp13_AST);
			match(IF_T);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp14_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp14_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp14_AST);
			match(NOT);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp15_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp15_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp15_AST);
			match(EXISTS);
			break;
		}
		case ID:
		case QUOTEDID:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		tablename();
		astFactory->addASTChild( currentAST, returnAST );
		{
		switch ( LA(1)) {
		case LPAREN:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp16_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp16_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp16_AST);
			match(LPAREN);
			columndef();
			astFactory->addASTChild( currentAST, returnAST );
			{ // ( ... )*
			for (;;) {
				if ((LA(1) == COMMA) && (LA(2) == ID || LA(2) == QUOTEDID)) {
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp17_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp17_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp17_AST);
					match(COMMA);
					columndef();
					astFactory->addASTChild( currentAST, returnAST );
				}
				else {
					goto _loop85;
				}
				
			}
			_loop85:;
			} // ( ... )*
			{ // ( ... )*
			for (;;) {
				if ((LA(1) == COMMA)) {
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp18_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp18_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp18_AST);
					match(COMMA);
					tableconstraint();
					astFactory->addASTChild( currentAST, returnAST );
				}
				else {
					goto _loop87;
				}
				
			}
			_loop87:;
			} // ( ... )*
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp19_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp19_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp19_AST);
			match(RPAREN);
			break;
		}
		case AS:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp20_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp20_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp20_AST);
			match(AS);
			selectstmt();
			astFactory->addASTChild( currentAST, returnAST );
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		createtable_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		createtable_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(CREATETABLE,"CREATETABLE"))->add(createtable_AST)));
		currentAST.root = createtable_AST;
		if ( createtable_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			createtable_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = createtable_AST->getFirstChild();
		else
			currentAST.child = createtable_AST;
		currentAST.advanceChildToEnd();
		createtable_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_6);
	}
	returnAST = createtable_AST;
}

void Sqlite3Parser::create_statements() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST create_statements_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		createtable();
		astFactory->addASTChild( currentAST, returnAST );
		create_statements_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_4);
	}
	returnAST = create_statements_AST;
}

void Sqlite3Parser::columndef() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST columndef_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		columnname();
		astFactory->addASTChild( currentAST, returnAST );
		{
		switch ( LA(1)) {
		case ID:
		case QUOTEDID:
		{
			type_name();
			astFactory->addASTChild( currentAST, returnAST );
			break;
		}
		case CHECK:
		case COLLATE:
		case CONSTRAINT:
		case DEFAULT:
		case NOT:
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
			if ((_tokenSet_7.member(LA(1)))) {
				columnconstraint();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop91;
			}
			
		}
		_loop91:;
		} // ( ... )*
		columndef_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		columndef_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(COLUMNDEF,"COLUMNDEF"))->add(columndef_AST)));
		currentAST.root = columndef_AST;
		if ( columndef_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			columndef_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = columndef_AST->getFirstChild();
		else
			currentAST.child = columndef_AST;
		currentAST.advanceChildToEnd();
		columndef_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_8);
	}
	returnAST = columndef_AST;
}

void Sqlite3Parser::tableconstraint() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST tableconstraint_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case CONSTRAINT:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp21_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp21_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp21_AST);
			match(CONSTRAINT);
			name();
			astFactory->addASTChild( currentAST, returnAST );
			break;
		}
		case CHECK:
		case PRIMARY:
		case UNIQUE:
		case FOREIGN:
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
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp22_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp22_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp22_AST);
			match(PRIMARY);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp23_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp23_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp23_AST);
			match(KEY);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp24_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp24_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp24_AST);
			match(LPAREN);
			indexedcolumn();
			astFactory->addASTChild( currentAST, returnAST );
			{ // ( ... )*
			for (;;) {
				if ((LA(1) == COMMA)) {
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp25_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp25_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp25_AST);
					match(COMMA);
					indexedcolumn();
					astFactory->addASTChild( currentAST, returnAST );
				}
				else {
					goto _loop110;
				}
				
			}
			_loop110:;
			} // ( ... )*
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp26_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp26_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp26_AST);
			match(RPAREN);
			{
			switch ( LA(1)) {
			case ON:
			{
				conflictclause();
				astFactory->addASTChild( currentAST, returnAST );
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
			break;
		}
		case UNIQUE:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp27_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp27_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp27_AST);
			match(UNIQUE);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp28_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp28_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp28_AST);
			match(LPAREN);
			indexedcolumn();
			astFactory->addASTChild( currentAST, returnAST );
			{ // ( ... )*
			for (;;) {
				if ((LA(1) == COMMA)) {
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp29_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp29_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp29_AST);
					match(COMMA);
					indexedcolumn();
					astFactory->addASTChild( currentAST, returnAST );
				}
				else {
					goto _loop113;
				}
				
			}
			_loop113:;
			} // ( ... )*
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp30_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp30_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp30_AST);
			match(RPAREN);
			{
			switch ( LA(1)) {
			case ON:
			{
				conflictclause();
				astFactory->addASTChild( currentAST, returnAST );
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
			break;
		}
		case CHECK:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp31_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp31_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp31_AST);
			match(CHECK);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp32_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp32_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp32_AST);
			match(LPAREN);
			expr();
			astFactory->addASTChild( currentAST, returnAST );
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp33_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp33_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp33_AST);
			match(RPAREN);
			break;
		}
		case FOREIGN:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp34_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp34_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp34_AST);
			match(FOREIGN);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp35_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp35_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp35_AST);
			match(KEY);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp36_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp36_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp36_AST);
			match(LPAREN);
			columnname();
			astFactory->addASTChild( currentAST, returnAST );
			{ // ( ... )*
			for (;;) {
				if ((LA(1) == COMMA)) {
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp37_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp37_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp37_AST);
					match(COMMA);
					columnname();
					astFactory->addASTChild( currentAST, returnAST );
				}
				else {
					goto _loop116;
				}
				
			}
			_loop116:;
			} // ( ... )*
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp38_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp38_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp38_AST);
			match(RPAREN);
			foreignkeyclause();
			astFactory->addASTChild( currentAST, returnAST );
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		tableconstraint_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		tableconstraint_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(TABLECONSTRAINT,"TABLECONSTRAINT"))->add(tableconstraint_AST)));
		currentAST.root = tableconstraint_AST;
		if ( tableconstraint_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			tableconstraint_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = tableconstraint_AST->getFirstChild();
		else
			currentAST.child = tableconstraint_AST;
		currentAST.advanceChildToEnd();
		tableconstraint_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_8);
	}
	returnAST = tableconstraint_AST;
}

void Sqlite3Parser::selectstmt() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST selectstmt_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp39_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp39_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp39_AST);
		match(SELECT);
		selectstmt_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_9);
	}
	returnAST = selectstmt_AST;
}

void Sqlite3Parser::type_name() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST type_name_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		{ // ( ... )+
		int _cnt95=0;
		for (;;) {
			if ((LA(1) == ID || LA(1) == QUOTEDID)) {
				name();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				if ( _cnt95>=1 ) { goto _loop95; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt95++;
		}
		_loop95:;
		}  // ( ... )+
		{
		switch ( LA(1)) {
		case LPAREN:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp40_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp40_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp40_AST);
			match(LPAREN);
			signednumber();
			astFactory->addASTChild( currentAST, returnAST );
			{
			switch ( LA(1)) {
			case COMMA:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp41_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp41_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp41_AST);
				match(COMMA);
				signednumber();
				astFactory->addASTChild( currentAST, returnAST );
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
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp42_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp42_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp42_AST);
			match(RPAREN);
			break;
		}
		case CHECK:
		case COLLATE:
		case CONSTRAINT:
		case DEFAULT:
		case NOT:
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
		type_name_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		type_name_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(TYPE_NAME,"TYPE_NAME"))->add(type_name_AST)));
		currentAST.root = type_name_AST;
		if ( type_name_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			type_name_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = type_name_AST->getFirstChild();
		else
			currentAST.child = type_name_AST;
		currentAST.advanceChildToEnd();
		type_name_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_10);
	}
	returnAST = type_name_AST;
}

void Sqlite3Parser::columnconstraint() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST columnconstraint_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case CONSTRAINT:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp43_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp43_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp43_AST);
			match(CONSTRAINT);
			name();
			astFactory->addASTChild( currentAST, returnAST );
			break;
		}
		case CHECK:
		case COLLATE:
		case DEFAULT:
		case NOT:
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
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp44_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp44_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp44_AST);
			match(PRIMARY);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp45_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp45_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp45_AST);
			match(KEY);
			{
			switch ( LA(1)) {
			case ASC:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp46_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp46_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp46_AST);
				match(ASC);
				break;
			}
			case DESC:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp47_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp47_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp47_AST);
				match(DESC);
				break;
			}
			case AUTOINCREMENT:
			case CHECK:
			case COLLATE:
			case CONSTRAINT:
			case DEFAULT:
			case NOT:
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
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp48_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp48_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp48_AST);
				match(AUTOINCREMENT);
				break;
			}
			case CHECK:
			case COLLATE:
			case CONSTRAINT:
			case DEFAULT:
			case NOT:
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
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp49_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp49_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp49_AST);
			match(NOT);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp50_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp50_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp50_AST);
			match(NULL_T);
			{
			switch ( LA(1)) {
			case ON:
			{
				conflictclause();
				astFactory->addASTChild( currentAST, returnAST );
				break;
			}
			case CHECK:
			case COLLATE:
			case CONSTRAINT:
			case DEFAULT:
			case NOT:
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
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp51_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp51_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp51_AST);
			match(UNIQUE);
			{
			switch ( LA(1)) {
			case ON:
			{
				conflictclause();
				astFactory->addASTChild( currentAST, returnAST );
				break;
			}
			case CHECK:
			case COLLATE:
			case CONSTRAINT:
			case DEFAULT:
			case NOT:
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
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp52_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp52_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp52_AST);
			match(CHECK);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp53_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp53_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp53_AST);
			match(LPAREN);
			expr();
			astFactory->addASTChild( currentAST, returnAST );
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp54_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp54_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp54_AST);
			match(RPAREN);
			break;
		}
		case DEFAULT:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp55_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp55_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp55_AST);
			match(DEFAULT);
			{
			switch ( LA(1)) {
			case NUMERIC:
			case PLUS:
			case MINUS:
			{
				signednumber();
				astFactory->addASTChild( currentAST, returnAST );
				break;
			}
			case STRING:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp56_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp56_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp56_AST);
				match(STRING);
				break;
			}
			case LPAREN:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp57_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp57_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp57_AST);
				match(LPAREN);
				expr();
				astFactory->addASTChild( currentAST, returnAST );
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp58_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp58_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp58_AST);
				match(RPAREN);
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
		case COLLATE:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp59_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp59_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp59_AST);
			match(COLLATE);
			collationname();
			astFactory->addASTChild( currentAST, returnAST );
			break;
		}
		case REFERENCES:
		{
			foreignkeyclause();
			astFactory->addASTChild( currentAST, returnAST );
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		columnconstraint_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
		columnconstraint_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(2))->add(astFactory->create(COLUMNCONSTRAINT,"COLUMNCONSTRAINT"))->add(columnconstraint_AST)));
		currentAST.root = columnconstraint_AST;
		if ( columnconstraint_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
			columnconstraint_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
			  currentAST.child = columnconstraint_AST->getFirstChild();
		else
			currentAST.child = columnconstraint_AST;
		currentAST.advanceChildToEnd();
		columnconstraint_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_10);
	}
	returnAST = columnconstraint_AST;
}

void Sqlite3Parser::name() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST name_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case ID:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp60_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp60_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp60_AST);
			match(ID);
			name_AST = currentAST.root;
			break;
		}
		case QUOTEDID:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp61_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp61_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp61_AST);
			match(QUOTEDID);
			name_AST = currentAST.root;
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_11);
	}
	returnAST = name_AST;
}

void Sqlite3Parser::conflictclause() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST conflictclause_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp62_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp62_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp62_AST);
		match(ON);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp63_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp63_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp63_AST);
		match(CONFLICT);
		{
		switch ( LA(1)) {
		case ROLLBACK:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp64_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp64_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp64_AST);
			match(ROLLBACK);
			break;
		}
		case ABORT:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp65_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp65_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp65_AST);
			match(ABORT);
			break;
		}
		case FAIL:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp66_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp66_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp66_AST);
			match(FAIL);
			break;
		}
		case IGNORE:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp67_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp67_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp67_AST);
			match(IGNORE);
			break;
		}
		case REPLACE:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp68_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp68_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp68_AST);
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
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_10);
	}
	returnAST = conflictclause_AST;
}

void Sqlite3Parser::expr() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST expr_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		subexpr();
		astFactory->addASTChild( currentAST, returnAST );
		{ // ( ... )*
		for (;;) {
			if ((_tokenSet_12.member(LA(1))) && (_tokenSet_13.member(LA(2)))) {
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
				case IN:
				case SLASH:
				case PERCENT:
				{
					binaryoperator();
					astFactory->addASTChild( currentAST, returnAST );
					break;
				}
				case AND:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp69_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp69_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp69_AST);
					match(AND);
					break;
				}
				case OR:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp70_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp70_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp70_AST);
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
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop138;
			}
			
		}
		_loop138:;
		} // ( ... )*
		expr_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_14);
	}
	returnAST = expr_AST;
}

void Sqlite3Parser::foreignkeyclause() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST foreignkeyclause_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp71_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp71_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp71_AST);
		match(REFERENCES);
		tablename();
		astFactory->addASTChild( currentAST, returnAST );
		{
		switch ( LA(1)) {
		case LPAREN:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp72_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp72_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp72_AST);
			match(LPAREN);
			columnname();
			astFactory->addASTChild( currentAST, returnAST );
			{ // ( ... )*
			for (;;) {
				if ((LA(1) == COMMA)) {
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp73_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp73_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp73_AST);
					match(COMMA);
					columnname();
					astFactory->addASTChild( currentAST, returnAST );
				}
				else {
					goto _loop123;
				}
				
			}
			_loop123:;
			} // ( ... )*
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp74_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp74_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp74_AST);
			match(RPAREN);
			break;
		}
		case CHECK:
		case COLLATE:
		case CONSTRAINT:
		case DEFAULT:
		case NOT:
		case MATCH:
		case ON:
		case PRIMARY:
		case REFERENCES:
		case UNIQUE:
		case RPAREN:
		case COMMA:
		case DEFERRABLE:
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
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp75_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp75_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp75_AST);
				match(ON);
				{
				switch ( LA(1)) {
				case DELETE:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp76_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp76_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp76_AST);
					match(DELETE);
					break;
				}
				case UPDATE:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp77_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp77_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp77_AST);
					match(UPDATE);
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
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp78_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp78_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp78_AST);
					match(SET);
					{
					switch ( LA(1)) {
					case NULL_T:
					{
						ANTLR_USE_NAMESPACE(antlr)RefAST tmp79_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
						tmp79_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, tmp79_AST);
						match(NULL_T);
						break;
					}
					case DEFAULT:
					{
						ANTLR_USE_NAMESPACE(antlr)RefAST tmp80_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
						tmp80_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, tmp80_AST);
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
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp81_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp81_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp81_AST);
					match(CASCADE);
					break;
				}
				case RESTRICT:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp82_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp82_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp82_AST);
					match(RESTRICT);
					break;
				}
				case NO:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp83_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp83_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp83_AST);
					match(NO);
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp84_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp84_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp84_AST);
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
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp85_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp85_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp85_AST);
				match(MATCH);
				name();
				astFactory->addASTChild( currentAST, returnAST );
				break;
			}
			default:
			{
				goto _loop128;
			}
			}
		}
		_loop128:;
		} // ( ... )*
		{
		if ((LA(1) == NOT || LA(1) == DEFERRABLE) && (_tokenSet_15.member(LA(2)))) {
			{
			switch ( LA(1)) {
			case NOT:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp86_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp86_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp86_AST);
				match(NOT);
				break;
			}
			case DEFERRABLE:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp87_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp87_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp87_AST);
			match(DEFERRABLE);
			{
			switch ( LA(1)) {
			case INITIALLY:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp88_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp88_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp88_AST);
				match(INITIALLY);
				{
				switch ( LA(1)) {
				case DEFERRED:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp89_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp89_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp89_AST);
					match(DEFERRED);
					break;
				}
				case IMMEDIATE:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp90_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp90_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp90_AST);
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
			case NOT:
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
		else if ((_tokenSet_10.member(LA(1))) && (_tokenSet_16.member(LA(2)))) {
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		foreignkeyclause_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_10);
	}
	returnAST = foreignkeyclause_AST;
}

void Sqlite3Parser::indexedcolumn() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST indexedcolumn_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		id();
		astFactory->addASTChild( currentAST, returnAST );
		indexedcolumn_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_8);
	}
	returnAST = indexedcolumn_AST;
}

void Sqlite3Parser::functionname() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST functionname_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		id();
		astFactory->addASTChild( currentAST, returnAST );
		functionname_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_17);
	}
	returnAST = functionname_AST;
}

void Sqlite3Parser::subexpr() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST subexpr_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		{
		if ((_tokenSet_18.member(LA(1))) && (_tokenSet_19.member(LA(2)))) {
			unaryoperator();
			astFactory->addASTChild( currentAST, returnAST );
		}
		else if ((_tokenSet_19.member(LA(1))) && (_tokenSet_20.member(LA(2)))) {
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		{
		switch ( LA(1)) {
		case CURRENT_TIME:
		case CURRENT_DATE:
		case CURRENT_TIMESTAMP:
		case NULL_T:
		case NUMERIC:
		case STRING:
		case PLUS:
		case MINUS:
		{
			literalvalue();
			astFactory->addASTChild( currentAST, returnAST );
			break;
		}
		case CAST:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp91_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp91_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp91_AST);
			match(CAST);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp92_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp92_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp92_AST);
			match(LPAREN);
			expr();
			astFactory->addASTChild( currentAST, returnAST );
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp93_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp93_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp93_AST);
			match(AS);
			type_name();
			astFactory->addASTChild( currentAST, returnAST );
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp94_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp94_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp94_AST);
			match(RPAREN);
			break;
		}
		case CASE_T:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp95_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp95_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp95_AST);
			match(CASE_T);
			{
			switch ( LA(1)) {
			case CASE_T:
			case CURRENT_TIME:
			case CURRENT_DATE:
			case CURRENT_TIMESTAMP:
			case NOT:
			case NULL_T:
			case EXISTS:
			case ID:
			case QUOTEDID:
			case NUMERIC:
			case STRING:
			case LPAREN:
			case PLUS:
			case MINUS:
			case TILDE:
			case CAST:
			case RAISE:
			{
				expr();
				astFactory->addASTChild( currentAST, returnAST );
				break;
			}
			case WHEN:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			{ // ( ... )+
			int _cnt151=0;
			for (;;) {
				if ((LA(1) == WHEN)) {
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp96_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp96_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp96_AST);
					match(WHEN);
					expr();
					astFactory->addASTChild( currentAST, returnAST );
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp97_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp97_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp97_AST);
					match(THEN);
					expr();
					astFactory->addASTChild( currentAST, returnAST );
				}
				else {
					if ( _cnt151>=1 ) { goto _loop151; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
				}
				
				_cnt151++;
			}
			_loop151:;
			}  // ( ... )+
			{
			switch ( LA(1)) {
			case ELSE:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp98_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp98_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp98_AST);
				match(ELSE);
				expr();
				astFactory->addASTChild( currentAST, returnAST );
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
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp99_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp99_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp99_AST);
			match(END);
			break;
		}
		case RAISE:
		{
			raisefunction();
			astFactory->addASTChild( currentAST, returnAST );
			break;
		}
		default:
			if ((LA(1) == ID || LA(1) == QUOTEDID) && (_tokenSet_21.member(LA(2)))) {
				{
				if ((LA(1) == ID || LA(1) == QUOTEDID) && ((LA(2) >= DOT && LA(2) <= QUOTEDID))) {
					{
					if ((LA(1) == ID || LA(1) == QUOTEDID) && (LA(2) == DOT)) {
						databasename();
						astFactory->addASTChild( currentAST, returnAST );
						ANTLR_USE_NAMESPACE(antlr)RefAST tmp100_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
						tmp100_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, tmp100_AST);
						match(DOT);
					}
					else if ((LA(1) == ID || LA(1) == QUOTEDID) && (LA(2) == ID || LA(2) == QUOTEDID)) {
					}
					else {
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					
					}
					tablename();
					astFactory->addASTChild( currentAST, returnAST );
				}
				else if ((LA(1) == ID || LA(1) == QUOTEDID) && (_tokenSet_22.member(LA(2)))) {
				}
				else {
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				
				}
				columnname();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else if ((LA(1) == ID || LA(1) == QUOTEDID) && (LA(2) == LPAREN)) {
				functionname();
				astFactory->addASTChild( currentAST, returnAST );
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp101_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp101_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp101_AST);
				match(LPAREN);
				{
				switch ( LA(1)) {
				case CASE_T:
				case CURRENT_TIME:
				case CURRENT_DATE:
				case CURRENT_TIMESTAMP:
				case NOT:
				case NULL_T:
				case EXISTS:
				case ID:
				case QUOTEDID:
				case NUMERIC:
				case STRING:
				case LPAREN:
				case PLUS:
				case MINUS:
				case TILDE:
				case CAST:
				case RAISE:
				{
					expr();
					astFactory->addASTChild( currentAST, returnAST );
					{ // ( ... )*
					for (;;) {
						if ((LA(1) == COMMA)) {
							ANTLR_USE_NAMESPACE(antlr)RefAST tmp102_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
							tmp102_AST = astFactory->create(LT(1));
							astFactory->addASTChild(currentAST, tmp102_AST);
							match(COMMA);
							expr();
							astFactory->addASTChild( currentAST, returnAST );
						}
						else {
							goto _loop146;
						}
						
					}
					_loop146:;
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
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp103_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp103_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp103_AST);
				match(RPAREN);
			}
			else if ((LA(1) == LPAREN) && (_tokenSet_13.member(LA(2)))) {
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp104_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp104_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp104_AST);
				match(LPAREN);
				expr();
				astFactory->addASTChild( currentAST, returnAST );
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp105_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp105_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp105_AST);
				match(RPAREN);
			}
			else if ((LA(1) == NOT || LA(1) == EXISTS || LA(1) == LPAREN) && (LA(2) == EXISTS || LA(2) == LPAREN || LA(2) == SELECT)) {
				{
				switch ( LA(1)) {
				case NOT:
				case EXISTS:
				{
					{
					switch ( LA(1)) {
					case NOT:
					{
						ANTLR_USE_NAMESPACE(antlr)RefAST tmp106_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
						tmp106_AST = astFactory->create(LT(1));
						astFactory->addASTChild(currentAST, tmp106_AST);
						match(NOT);
						break;
					}
					case EXISTS:
					{
						break;
					}
					default:
					{
						throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
					}
					}
					}
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp107_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp107_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp107_AST);
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
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp108_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp108_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp108_AST);
				match(LPAREN);
				selectstmt();
				astFactory->addASTChild( currentAST, returnAST );
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp109_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp109_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp109_AST);
				match(RPAREN);
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{
		if ((_tokenSet_23.member(LA(1))) && (_tokenSet_24.member(LA(2)))) {
			suffixexpr();
			astFactory->addASTChild( currentAST, returnAST );
		}
		else if ((_tokenSet_14.member(LA(1))) && (_tokenSet_25.member(LA(2)))) {
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		subexpr_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_14);
	}
	returnAST = subexpr_AST;
}

void Sqlite3Parser::binaryoperator() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST binaryoperator_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case OROP:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp110_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp110_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp110_AST);
			match(OROP);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case STAR:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp111_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp111_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp111_AST);
			match(STAR);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case SLASH:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp112_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp112_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp112_AST);
			match(SLASH);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case PERCENT:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp113_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp113_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp113_AST);
			match(PERCENT);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case PLUS:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp114_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp114_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp114_AST);
			match(PLUS);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case MINUS:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp115_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp115_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp115_AST);
			match(MINUS);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case BITWISELEFT:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp116_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp116_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp116_AST);
			match(BITWISELEFT);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case BITWISERIGHT:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp117_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp117_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp117_AST);
			match(BITWISERIGHT);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case AMPERSAND:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp118_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp118_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp118_AST);
			match(AMPERSAND);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case BITOR:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp119_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp119_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp119_AST);
			match(BITOR);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case LOWER:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp120_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp120_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp120_AST);
			match(LOWER);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case LOWEREQUAL:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp121_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp121_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp121_AST);
			match(LOWEREQUAL);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case GREATER:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp122_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp122_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp122_AST);
			match(GREATER);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case GREATEREQUAL:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp123_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp123_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp123_AST);
			match(GREATEREQUAL);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case EQUAL:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp124_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp124_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp124_AST);
			match(EQUAL);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case EQUAL2:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp125_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp125_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp125_AST);
			match(EQUAL2);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case UNEQUAL:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp126_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp126_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp126_AST);
			match(UNEQUAL);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case UNEQUAL2:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp127_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp127_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp127_AST);
			match(UNEQUAL2);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case IS:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp128_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp128_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp128_AST);
			match(IS);
			{
			if ((LA(1) == NOT) && (_tokenSet_13.member(LA(2)))) {
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp129_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp129_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp129_AST);
				match(NOT);
			}
			else if ((_tokenSet_13.member(LA(1))) && (_tokenSet_20.member(LA(2)))) {
			}
			else {
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			
			}
			binaryoperator_AST = currentAST.root;
			break;
		}
		case IN:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp130_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp130_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp130_AST);
			match(IN);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case LIKE:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp131_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp131_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp131_AST);
			match(LIKE);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case GLOB:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp132_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp132_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp132_AST);
			match(GLOB);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case MATCH:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp133_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp133_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp133_AST);
			match(MATCH);
			binaryoperator_AST = currentAST.root;
			break;
		}
		case REGEXP:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp134_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp134_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp134_AST);
			match(REGEXP);
			binaryoperator_AST = currentAST.root;
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_13);
	}
	returnAST = binaryoperator_AST;
}

void Sqlite3Parser::unaryoperator() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST unaryoperator_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case MINUS:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp135_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp135_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp135_AST);
			match(MINUS);
			unaryoperator_AST = currentAST.root;
			break;
		}
		case PLUS:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp136_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp136_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp136_AST);
			match(PLUS);
			unaryoperator_AST = currentAST.root;
			break;
		}
		case TILDE:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp137_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp137_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp137_AST);
			match(TILDE);
			unaryoperator_AST = currentAST.root;
			break;
		}
		case NOT:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp138_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp138_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp138_AST);
			match(NOT);
			unaryoperator_AST = currentAST.root;
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_19);
	}
	returnAST = unaryoperator_AST;
}

void Sqlite3Parser::literalvalue() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST literalvalue_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case NUMERIC:
		case PLUS:
		case MINUS:
		{
			signednumber();
			astFactory->addASTChild( currentAST, returnAST );
			literalvalue_AST = currentAST.root;
			break;
		}
		case STRING:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp139_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp139_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp139_AST);
			match(STRING);
			literalvalue_AST = currentAST.root;
			break;
		}
		case NULL_T:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp140_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp140_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp140_AST);
			match(NULL_T);
			literalvalue_AST = currentAST.root;
			break;
		}
		case CURRENT_TIME:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp141_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp141_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp141_AST);
			match(CURRENT_TIME);
			literalvalue_AST = currentAST.root;
			break;
		}
		case CURRENT_DATE:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp142_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp142_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp142_AST);
			match(CURRENT_DATE);
			literalvalue_AST = currentAST.root;
			break;
		}
		case CURRENT_TIMESTAMP:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp143_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp143_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp143_AST);
			match(CURRENT_TIMESTAMP);
			literalvalue_AST = currentAST.root;
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_22);
	}
	returnAST = literalvalue_AST;
}

void Sqlite3Parser::raisefunction() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST raisefunction_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp144_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp144_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp144_AST);
		match(RAISE);
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp145_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp145_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp145_AST);
		match(LPAREN);
		{
		switch ( LA(1)) {
		case IGNORE:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp146_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp146_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp146_AST);
			match(IGNORE);
			break;
		}
		case ROLLBACK:
		case ABORT:
		case FAIL:
		{
			{
			switch ( LA(1)) {
			case ROLLBACK:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp147_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp147_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp147_AST);
				match(ROLLBACK);
				break;
			}
			case ABORT:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp148_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp148_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp148_AST);
				match(ABORT);
				break;
			}
			case FAIL:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp149_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp149_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp149_AST);
				match(FAIL);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp150_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp150_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp150_AST);
			match(COMMA);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp151_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp151_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp151_AST);
			match(STRING);
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp152_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp152_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp152_AST);
		match(RPAREN);
		raisefunction_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_22);
	}
	returnAST = raisefunction_AST;
}

void Sqlite3Parser::suffixexpr() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST suffixexpr_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case COLLATE:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp153_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp153_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp153_AST);
			match(COLLATE);
			collationname();
			astFactory->addASTChild( currentAST, returnAST );
			break;
		}
		case GLOB:
		case LIKE:
		case NOT:
		case MATCH:
		case REGEXP:
		case IN:
		{
			{
			switch ( LA(1)) {
			case NOT:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp154_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp154_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp154_AST);
				match(NOT);
				break;
			}
			case GLOB:
			case LIKE:
			case MATCH:
			case REGEXP:
			case IN:
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
			case GLOB:
			case LIKE:
			case MATCH:
			case REGEXP:
			{
				{
				switch ( LA(1)) {
				case LIKE:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp155_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp155_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp155_AST);
					match(LIKE);
					break;
				}
				case GLOB:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp156_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp156_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp156_AST);
					match(GLOB);
					break;
				}
				case REGEXP:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp157_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp157_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp157_AST);
					match(REGEXP);
					break;
				}
				case MATCH:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp158_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp158_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp158_AST);
					match(MATCH);
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
				expr();
				astFactory->addASTChild( currentAST, returnAST );
				{
				if ((LA(1) == ESCAPE) && (_tokenSet_13.member(LA(2)))) {
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp159_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp159_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp159_AST);
					match(ESCAPE);
					expr();
					astFactory->addASTChild( currentAST, returnAST );
				}
				else if ((_tokenSet_14.member(LA(1))) && (_tokenSet_25.member(LA(2)))) {
				}
				else {
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				
				}
				break;
			}
			case IN:
			{
				ANTLR_USE_NAMESPACE(antlr)RefAST tmp160_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
				tmp160_AST = astFactory->create(LT(1));
				astFactory->addASTChild(currentAST, tmp160_AST);
				match(IN);
				{
				switch ( LA(1)) {
				case LPAREN:
				{
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp161_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp161_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp161_AST);
					match(LPAREN);
					{
					switch ( LA(1)) {
					case SELECT:
					{
						selectstmt();
						astFactory->addASTChild( currentAST, returnAST );
						break;
					}
					case CASE_T:
					case CURRENT_TIME:
					case CURRENT_DATE:
					case CURRENT_TIMESTAMP:
					case NOT:
					case NULL_T:
					case EXISTS:
					case ID:
					case QUOTEDID:
					case NUMERIC:
					case STRING:
					case LPAREN:
					case PLUS:
					case MINUS:
					case TILDE:
					case CAST:
					case RAISE:
					{
						expr();
						astFactory->addASTChild( currentAST, returnAST );
						{ // ( ... )*
						for (;;) {
							if ((LA(1) == COMMA)) {
								ANTLR_USE_NAMESPACE(antlr)RefAST tmp162_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
								tmp162_AST = astFactory->create(LT(1));
								astFactory->addASTChild(currentAST, tmp162_AST);
								match(COMMA);
								expr();
								astFactory->addASTChild( currentAST, returnAST );
							}
							else {
								goto _loop163;
							}
							
						}
						_loop163:;
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
					ANTLR_USE_NAMESPACE(antlr)RefAST tmp163_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
					tmp163_AST = astFactory->create(LT(1));
					astFactory->addASTChild(currentAST, tmp163_AST);
					match(RPAREN);
					break;
				}
				case ID:
				case QUOTEDID:
				{
					tablename();
					astFactory->addASTChild( currentAST, returnAST );
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
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
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
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_14);
	}
	returnAST = suffixexpr_AST;
}

void Sqlite3Parser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory )
{
	factory.setMaxNodeType(100);
}
const char* Sqlite3Parser::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"\"AUTOINCREMENT\"",
	"\"AS\"",
	"\"ASC\"",
	"\"CASE\"",
	"\"CHECK\"",
	"\"CREATE\"",
	"\"COLLATE\"",
	"\"CONFLICT\"",
	"\"CONSTRAINT\"",
	"\"CURRENT_TIME\"",
	"\"CURRENT_DATE\"",
	"\"CURRENT_TIMESTAMP\"",
	"\"DEFAULT\"",
	"\"DESC\"",
	"\"ELSE\"",
	"\"END\"",
	"\"ESCAPE\"",
	"\"GLOB\"",
	"\"KEY\"",
	"\"LIKE\"",
	"\"TABLE\"",
	"\"IF\"",
	"\"IS\"",
	"\"NOT\"",
	"\"NULL\"",
	"\"MATCH\"",
	"\"EXISTS\"",
	"\"ON\"",
	"\"PRIMARY\"",
	"\"REFERENCES\"",
	"\"REGEXP\"",
	"\"TEMPORARY\"",
	"\"TEMP\"",
	"\"THEN\"",
	"\"UNIQUE\"",
	"\"WHEN\"",
	"TYPE_NAME",
	"COLUMNDEF",
	"COLUMNCONSTRAINT",
	"TABLECONSTRAINT",
	"CREATETABLE",
	"DIGIT",
	"DOT",
	"ID",
	"QUOTEDID",
	"NUMERIC",
	"SL_COMMENT",
	"ML_COMMENT",
	"WS",
	"STRING",
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
	"FOREIGN",
	"ROLLBACK",
	"ABORT",
	"FAIL",
	"IGNORE",
	"REPLACE",
	"DELETE",
	"UPDATE",
	"SET",
	"CASCADE",
	"RESTRICT",
	"NO",
	"ACTION",
	"DEFERRABLE",
	"INITIALLY",
	"DEFERRED",
	"IMMEDIATE",
	"SELECT",
	"AND",
	"OR",
	"CAST",
	"ELSE",
	"IN",
	"RAISE",
	"SLASH",
	"PERCENT",
	0
};

const unsigned long Sqlite3Parser::_tokenSet_0_data_[] = { 2897810722UL, 3720462567UL, 1627391999UL, 27UL, 0UL, 0UL, 0UL, 0UL };
// EOF "AS" "CHECK" "COLLATE" "CONSTRAINT" "DEFAULT" "END" "ESCAPE" "GLOB" 
// "LIKE" "IS" "NOT" "MATCH" "ON" "PRIMARY" "REFERENCES" "REGEXP" "THEN" 
// "UNIQUE" "WHEN" DOT ID QUOTEDID LPAREN RPAREN COMMA PLUS MINUS STAR 
// AMPERSAND BITOR OROP EQUAL EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL 
// UNEQUAL UNEQUAL2 BITWISELEFT BITWISERIGHT DEFERRABLE AND OR ELSE IN 
// SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_0(_tokenSet_0_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_1_data_[] = { 0UL, 16384UL, 0UL, 0UL };
// DOT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_1(_tokenSet_1_data_,4);
const unsigned long Sqlite3Parser::_tokenSet_2_data_[] = { 2897810722UL, 3720446183UL, 1627391999UL, 27UL, 0UL, 0UL, 0UL, 0UL };
// EOF "AS" "CHECK" "COLLATE" "CONSTRAINT" "DEFAULT" "END" "ESCAPE" "GLOB" 
// "LIKE" "IS" "NOT" "MATCH" "ON" "PRIMARY" "REFERENCES" "REGEXP" "THEN" 
// "UNIQUE" "WHEN" ID QUOTEDID LPAREN RPAREN COMMA PLUS MINUS STAR AMPERSAND 
// BITOR OROP EQUAL EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL UNEQUAL 
// UNEQUAL2 BITWISELEFT BITWISERIGHT DEFERRABLE AND OR ELSE IN SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_2(_tokenSet_2_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_3_data_[] = { 750327072UL, 3716251879UL, 1610614783UL, 27UL, 0UL, 0UL, 0UL, 0UL };
// "AS" "CHECK" "COLLATE" "CONSTRAINT" "DEFAULT" "END" "ESCAPE" "GLOB" 
// "LIKE" "IS" "NOT" "MATCH" "PRIMARY" "REFERENCES" "REGEXP" "THEN" "UNIQUE" 
// "WHEN" ID QUOTEDID RPAREN COMMA PLUS MINUS STAR AMPERSAND BITOR OROP 
// EQUAL EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL UNEQUAL UNEQUAL2 
// BITWISELEFT BITWISERIGHT AND OR ELSE IN SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_3(_tokenSet_3_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_4_data_[] = { 2UL, 0UL, 0UL, 0UL };
// EOF 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_4(_tokenSet_4_data_,4);
const unsigned long Sqlite3Parser::_tokenSet_5_data_[] = { 750327072UL, 3716153575UL, 1610614783UL, 27UL, 0UL, 0UL, 0UL, 0UL };
// "AS" "CHECK" "COLLATE" "CONSTRAINT" "DEFAULT" "END" "ESCAPE" "GLOB" 
// "LIKE" "IS" "NOT" "MATCH" "PRIMARY" "REFERENCES" "REGEXP" "THEN" "UNIQUE" 
// "WHEN" RPAREN COMMA PLUS MINUS STAR AMPERSAND BITOR OROP EQUAL EQUAL2 
// GREATER GREATEREQUAL LOWER LOWEREQUAL UNEQUAL UNEQUAL2 BITWISELEFT BITWISERIGHT 
// AND OR ELSE IN SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_5(_tokenSet_5_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_6_data_[] = { 2UL, 33554432UL, 0UL, 0UL };
// EOF SEMI 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_6(_tokenSet_6_data_,4);
const unsigned long Sqlite3Parser::_tokenSet_7_data_[] = { 134288640UL, 67UL, 0UL, 0UL };
// "CHECK" "COLLATE" "CONSTRAINT" "DEFAULT" "NOT" "PRIMARY" "REFERENCES" 
// "UNIQUE" 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_7(_tokenSet_7_data_,4);
const unsigned long Sqlite3Parser::_tokenSet_8_data_[] = { 0UL, 25165824UL, 0UL, 0UL };
// RPAREN COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_8(_tokenSet_8_data_,4);
const unsigned long Sqlite3Parser::_tokenSet_9_data_[] = { 2UL, 41943040UL, 0UL, 0UL };
// EOF RPAREN SEMI 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_9(_tokenSet_9_data_,4);
const unsigned long Sqlite3Parser::_tokenSet_10_data_[] = { 134288640UL, 25165891UL, 0UL, 0UL };
// "CHECK" "COLLATE" "CONSTRAINT" "DEFAULT" "NOT" "PRIMARY" "REFERENCES" 
// "UNIQUE" RPAREN COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_10(_tokenSet_10_data_,4);
const unsigned long Sqlite3Parser::_tokenSet_11_data_[] = { 2818643200UL, 29458499UL, 16779264UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "CHECK" "COLLATE" "CONSTRAINT" "DEFAULT" "NOT" "MATCH" "ON" "PRIMARY" 
// "REFERENCES" "UNIQUE" ID QUOTEDID LPAREN RPAREN COMMA FOREIGN DEFERRABLE 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_11(_tokenSet_11_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_12_data_[] = { 614465536UL, 3690987524UL, 1610614783UL, 26UL, 0UL, 0UL, 0UL, 0UL };
// "GLOB" "LIKE" "IS" "MATCH" "REGEXP" PLUS MINUS STAR AMPERSAND BITOR 
// OROP EQUAL EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL UNEQUAL UNEQUAL2 
// BITWISELEFT BITWISERIGHT AND OR IN SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_12(_tokenSet_12_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_13_data_[] = { 1476452480UL, 744718336UL, 2147483648UL, 4UL, 0UL, 0UL, 0UL, 0UL };
// "CASE" "CURRENT_TIME" "CURRENT_DATE" "CURRENT_TIMESTAMP" "NOT" "NULL" 
// "EXISTS" ID QUOTEDID NUMERIC STRING LPAREN PLUS MINUS TILDE CAST RAISE 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_13(_tokenSet_13_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_14_data_[] = { 616038432UL, 3716153508UL, 1610614783UL, 27UL, 0UL, 0UL, 0UL, 0UL };
// "AS" "END" "ESCAPE" "GLOB" "LIKE" "IS" "MATCH" "REGEXP" "THEN" "WHEN" 
// RPAREN COMMA PLUS MINUS STAR AMPERSAND BITOR OROP EQUAL EQUAL2 GREATER 
// GREATEREQUAL LOWER LOWEREQUAL UNEQUAL UNEQUAL2 BITWISELEFT BITWISERIGHT 
// AND OR ELSE IN SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_14(_tokenSet_14_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_15_data_[] = { 134288640UL, 25165891UL, 50331648UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "CHECK" "COLLATE" "CONSTRAINT" "DEFAULT" "NOT" "PRIMARY" "REFERENCES" 
// "UNIQUE" RPAREN COMMA DEFERRABLE INITIALLY 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_15(_tokenSet_15_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_16_data_[] = { 2554402050UL, 266567747UL, 2048UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// EOF "CHECK" "COLLATE" "CONSTRAINT" "DEFAULT" "KEY" "NOT" "NULL" "ON" 
// "PRIMARY" "REFERENCES" "UNIQUE" ID QUOTEDID NUMERIC STRING LPAREN RPAREN 
// COMMA SEMI PLUS MINUS FOREIGN 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_16(_tokenSet_16_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_17_data_[] = { 0UL, 4194304UL, 0UL, 0UL };
// LPAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_17(_tokenSet_17_data_,4);
const unsigned long Sqlite3Parser::_tokenSet_18_data_[] = { 134217728UL, 738197504UL, 0UL, 0UL };
// "NOT" PLUS MINUS TILDE 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_18(_tokenSet_18_data_,4);
const unsigned long Sqlite3Parser::_tokenSet_19_data_[] = { 1476452480UL, 207847424UL, 2147483648UL, 4UL, 0UL, 0UL, 0UL, 0UL };
// "CASE" "CURRENT_TIME" "CURRENT_DATE" "CURRENT_TIMESTAMP" "NOT" "NULL" 
// "EXISTS" ID QUOTEDID NUMERIC STRING LPAREN PLUS MINUS CAST RAISE 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_19(_tokenSet_19_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_20_data_[] = { 2092491936UL, 4259561636UL, 4026533887UL, 31UL, 0UL, 0UL, 0UL, 0UL };
// "AS" "CASE" "COLLATE" "CURRENT_TIME" "CURRENT_DATE" "CURRENT_TIMESTAMP" 
// "END" "ESCAPE" "GLOB" "LIKE" "IS" "NOT" "NULL" "MATCH" "EXISTS" "REGEXP" 
// "THEN" "WHEN" DOT ID QUOTEDID NUMERIC STRING LPAREN RPAREN COMMA PLUS 
// MINUS STAR TILDE AMPERSAND BITOR OROP EQUAL EQUAL2 GREATER GREATEREQUAL 
// LOWER LOWEREQUAL UNEQUAL UNEQUAL2 BITWISELEFT BITWISERIGHT SELECT AND 
// OR CAST ELSE IN RAISE SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_20(_tokenSet_20_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_21_data_[] = { 750257184UL, 3716268196UL, 1610614783UL, 27UL, 0UL, 0UL, 0UL, 0UL };
// "AS" "COLLATE" "END" "ESCAPE" "GLOB" "LIKE" "IS" "NOT" "MATCH" "REGEXP" 
// "THEN" "WHEN" DOT ID QUOTEDID RPAREN COMMA PLUS MINUS STAR AMPERSAND 
// BITOR OROP EQUAL EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL UNEQUAL 
// UNEQUAL2 BITWISELEFT BITWISERIGHT AND OR ELSE IN SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_21(_tokenSet_21_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_22_data_[] = { 750257184UL, 3716153508UL, 1610614783UL, 27UL, 0UL, 0UL, 0UL, 0UL };
// "AS" "COLLATE" "END" "ESCAPE" "GLOB" "LIKE" "IS" "NOT" "MATCH" "REGEXP" 
// "THEN" "WHEN" RPAREN COMMA PLUS MINUS STAR AMPERSAND BITOR OROP EQUAL 
// EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL UNEQUAL UNEQUAL2 BITWISELEFT 
// BITWISERIGHT AND OR ELSE IN SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_22(_tokenSet_22_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_23_data_[] = { 681575424UL, 4UL, 0UL, 2UL, 0UL, 0UL, 0UL, 0UL };
// "COLLATE" "GLOB" "LIKE" "NOT" "MATCH" "REGEXP" IN 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_23(_tokenSet_23_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_24_data_[] = { 2023809152UL, 744718340UL, 2147483648UL, 6UL, 0UL, 0UL, 0UL, 0UL };
// "CASE" "CURRENT_TIME" "CURRENT_DATE" "CURRENT_TIMESTAMP" "GLOB" "LIKE" 
// "NOT" "NULL" "MATCH" "EXISTS" "REGEXP" ID QUOTEDID NUMERIC STRING LPAREN 
// PLUS MINUS TILDE CAST IN RAISE 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_24(_tokenSet_24_data_,8);
const unsigned long Sqlite3Parser::_tokenSet_25_data_[] = { 2092561824UL, 4259545319UL, 3758098431UL, 31UL, 0UL, 0UL, 0UL, 0UL };
// "AS" "CASE" "CHECK" "COLLATE" "CONSTRAINT" "CURRENT_TIME" "CURRENT_DATE" 
// "CURRENT_TIMESTAMP" "DEFAULT" "END" "ESCAPE" "GLOB" "LIKE" "IS" "NOT" 
// "NULL" "MATCH" "EXISTS" "PRIMARY" "REFERENCES" "REGEXP" "THEN" "UNIQUE" 
// "WHEN" ID QUOTEDID NUMERIC STRING LPAREN RPAREN COMMA PLUS MINUS STAR 
// TILDE AMPERSAND BITOR OROP EQUAL EQUAL2 GREATER GREATEREQUAL LOWER LOWEREQUAL 
// UNEQUAL UNEQUAL2 BITWISELEFT BITWISERIGHT AND OR CAST ELSE IN RAISE 
// SLASH PERCENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet Sqlite3Parser::_tokenSet_25(_tokenSet_25_data_,8);


