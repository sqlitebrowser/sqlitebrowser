#ifndef SQLUILEXER_H
#define SQLUILEXER_H

#include "Qsci/qscilexersql.h"

#include <map>

class QsciAPIs;

class SqlUiLexer : public QsciLexerSQL
{
    Q_OBJECT

public:
    explicit SqlUiLexer(QObject *parent = nullptr);

    enum ApiCompleterIconId
    {
        ApiCompleterIconIdKeyword = 1,
        ApiCompleterIconIdFunction,
        ApiCompleterIconIdTable,
        ApiCompleterIconIdColumn,
        ApiCompleterIconIdSchema,
    };

    using TablesAndColumnsMap = std::map<QString, std::vector<QString>>;
    using QualifiedTablesMap = std::map<QString, TablesAndColumnsMap>;

    void setTableNames(const QualifiedTablesMap& tables);

    const char* keywords(int set) const override;

    QStringList autoCompletionWordSeparators() const override;

    bool caseSensitive() const override;

private:
    QsciAPIs* autocompleteApi;

    void setupAutoCompletion();

    QStringList listTables;
    QStringList listFunctions;
    QStringList keywordPatterns;
};

#endif
