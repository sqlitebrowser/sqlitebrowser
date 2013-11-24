#include <QObject>

class TestTable: public QObject
{
    Q_OBJECT
private slots:
    void sqlOutput();
    void autoincrement();
    void notnull();

    void parseSQL();
    void parseSQLdefaultexpr();
    void parseSQLMultiPk();
    void parseSQLForeignKey();
    void parseSQLSingleQuotes();
    void parseSQLKeywordInIdentifier();
};

