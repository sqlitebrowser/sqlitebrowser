#ifndef TESTSQLOBJECTS_H
#define TESTSQLOBJECTS_H

#include <QObject>

class TestTable: public QObject
{
    Q_OBJECT
private slots:
    void sqlOutput();
    void autoincrement();
    void notnull();
    void withoutRowid();
    void foreignKeys();
    void uniqueConstraint();

    void parseSQL();
    void parseSQLdefaultexpr();
    void parseSQLMultiPk();
    void parseSQLForeignKey();
    void parseSQLSingleQuotes();
    void parseSQLKeywordInIdentifier();
    void parseSQLWithoutRowid();
    void parseNonASCIIChars();
    void parseSQLEscapedQuotes();
    void parseSQLForeignKeys();
    void parseSQLCheckConstraint();
    void parseDefaultValues();
    void createTableWithIn();
    void createTableWithNotLikeConstraint();
};

#endif
