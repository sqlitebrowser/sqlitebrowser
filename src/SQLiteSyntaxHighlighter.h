#ifndef SQLITESYNTAXHIGHLIGHTER_H
#define SQLITESYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SQLiteSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SQLiteSyntaxHighlighter(QTextDocument *parent = 0);

    void setTableNames(const QStringList& tablenames);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    void highlightBlockVector(const QString& text, const QVector<HighlightingRule>& vec);

    QVector<HighlightingRule> highlightingRules;
    QVector<HighlightingRule> tableNameRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat identifierFormat;
    QTextCharFormat tableFormat;
    QTextCharFormat functionFormat;

    QTextCharFormat createFormat(const QString& settings_name);
};

#endif // SQLITESYNTAXHIGHLIGHTER_H
