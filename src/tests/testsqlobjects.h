#ifndef TESTSQLOBJECTS_H
#define TESTSQLOBJECTS_H

#include <QObject>

class TestTable: public QObject
{
    Q_OBJECT
private slots:
    void sqlOutput();
    void sqlGraveAccentOutput();
    void sqlSquareBracketsOutput();
    void autoincrement();
    void notnull();
    void withoutRowid();
    void strict();
    void strictAndWithoutRowid();
    void foreignKeys();
    void uniqueConstraint();

    void parseSQL();
    void parseSQLSquareBrackets();
    void parseSQLdefaultexpr();
    void parseSQLMultiPk();
    void parseSQLForeignKey();
    void parseSQLSingleQuotes();
    void parseSQLKeywordInIdentifier();
    void parseSQLSomeKeywordsInIdentifier();
    void parseSQLWithoutRowid();
    void parseSQLStrictTable();
    void parseSQLStrictAndWithoutRowidTable();
    void parseNonASCIIChars();
    void parseNonASCIICharsEs();
    void parseSQLEscapedQuotes();
    void parseSQLForeignKeys();
    void parseSQLCheckConstraint();
    void parseDefaultValues();
    void createTableWithIn();
    void createTableWithNotLikeConstraint();
    void rowValues();
    void complexExpressions();
    void datetimeExpression();
    void extraParentheses();
    void moduloOperator();
    void complexExpression();
    void parseIdentifierWithDollar();

    void parseTest();
    void parseTest_data();
};

#endif
