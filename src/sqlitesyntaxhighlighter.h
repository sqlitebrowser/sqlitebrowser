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

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat tableFormat;
};

#endif // SQLITESYNTAXHIGHLIGHTER_H
