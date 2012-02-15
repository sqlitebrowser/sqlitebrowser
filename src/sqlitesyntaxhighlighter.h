#ifndef SQLITESYNTAXHIGHLIGHTER_H
#define SQLITESYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SQLiteSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SQLiteSyntaxHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    
};

#endif // SQLITESYNTAXHIGHLIGHTER_H
