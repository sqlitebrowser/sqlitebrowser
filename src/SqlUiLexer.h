#ifndef SQLUILEXER_H
#define SQLUILEXER_H

#include "Qsci/qscilexersql.h"

#include <QMap>

class QsciAPIs;

class SqlUiLexer : public QsciLexerSQL
{
    Q_OBJECT

public:
    explicit SqlUiLexer(QObject *parent = 0);

    enum ApiCompleterIconId
    {
        ApiCompleterIconIdKeyword = 1,
        ApiCompleterIconIdFunction,
        ApiCompleterIconIdTable,
        ApiCompleterIconIdColumn,
    };

    typedef QMap<QString, QList<QString> > TablesAndColumnsMap;
    void setTableNames(const TablesAndColumnsMap& tables);

    virtual const char* keywords(int set) const;

    virtual QStringList autoCompletionWordSeparators() const;

private:
    QsciAPIs* autocompleteApi;

    void setupAutoCompletion();

    QStringList listTables;
    QStringList listFunctions;
};

#endif
