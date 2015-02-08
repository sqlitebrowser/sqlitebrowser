#include "SQLiteSyntaxHighlighter.h"
#include "PreferencesDialog.h"

SQLiteSyntaxHighlighter::SQLiteSyntaxHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    tableFormat = createFormat("table");
    keywordFormat = createFormat("keyword");
    singleLineCommentFormat = createFormat("comment");
    identifierFormat = createFormat("identifier");
    stringFormat = createFormat("string");
    functionFormat = createFormat("function");

    // Keywords
    QStringList keywordPatterns;
    keywordPatterns
            << "ABORT" << "ACTION" << "ADD" << "AFTER" << "ALL"
            << "ALTER" << "ANALYZE" << "AND" << "AS" << "ASC"
            << "ATTACH" << "AUTOINCREMENT" << "BEFORE" << "BEGIN" << "BETWEEN"
            << "BY" << "CASCADE" << "CASE" << "CAST" << "CHECK"
            << "COLLATE" << "COLUMN" << "COMMIT" << "CONFLICT" << "CONSTRAINT"
            << "CREATE" << "CROSS" << "CURRENT_DATE" << "CURRENT_TIME" << "CURRENT_TIMESTAMP"
            << "DATABASE" << "DEFAULT" << "DEFERRABLE" << "DEFERRED" << "DELETE"
            << "DESC" << "DETACH" << "DISTINCT" << "DROP" << "EACH"
            << "ELSE" << "END" << "ESCAPE" << "EXCEPT" << "EXCLUSIVE"
            << "EXISTS" << "EXPLAIN" << "FAIL" << "FOR" << "FOREIGN"
            << "FROM" << "FULL" << "GLOB" << "GROUP" << "HAVING"
            << "IF" << "IGNORE" << "IMMEDIATE" << "IN" << "INDEX"
            << "INDEXED" << "INITIALLY" << "INNER" << "INSERT" << "INSTEAD"
            << "INTERSECT" << "INTO" << "IS" << "ISNULL" << "JOIN"
            << "KEY" << "LEFT" << "LIKE" << "LIMIT" << "MATCH"
            << "NATURAL" << "NO" << "NOT" << "NOTNULL" << "NULL"
            << "OF" << "OFFSET" << "ON" << "OR" << "ORDER"
            << "OUTER" << "PLAN" << "PRAGMA" << "PRIMARY" << "QUERY"
            << "RAISE" << "REFERENCES" << "REGEXP" << "REINDEX" << "RELEASE"
            << "RENAME" << "REPLACE" << "RESTRICT" << "RIGHT" << "ROLLBACK"
            << "ROW" << "SAVEPOINT" << "SELECT" << "SET" << "TABLE"
            << "TEMP" << "TEMPORARY" << "THEN" << "TO" << "TRANSACTION"
            << "TRIGGER" << "UNION" << "UNIQUE" << "UPDATE" << "USING"
            << "VACUUM" << "VALUES" << "VIEW" << "VIRTUAL" << "WHEN"
            << "WHERE";

    rule.format = keywordFormat;
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(QString("\\b%1\\b").arg(pattern));
        rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        highlightingRules.append(rule);
    }

    // Functions
    QStringList functionPatterns;
    functionPatterns
            // Core functions
            << "ABS" << "CHANGES" << "CHAR" << "COALESCE" << "GLOB"
            << "IFNULL" << "INSTR" << "HEX" << "LAST_INSERT_ROW" << "LENGTH"
            << "LIKE" << "LOAD_EXTENSION" << "LOWER" << "LTRIM" << "MAX"
            << "MIN" << "NULLIF" << "QUOTE" << "RANDOM" << "RANDOMBLOB"
            << "REPLACE" << "ROUND" << "RTRIM" << "SOUNDEX" << "SQLITE_COMPILEOPTION_GET"
            << "SQLITE_COMPILEOPTION_USED" << "SQLITE_SOURCE_ID" << "SQLITE_VERSION" << "SUBSTR" << "TOTAL_CHANGES"
            << "TRIM" << "TYPEOF" << "UNICODE" << "UPPER" << "ZEROBLOB"
            // Date and time functions
            << "DATE" << "TIME" << "DATETIME" << "JULIANDAY" << "STRFTIME"
            // Aggregate functions
            << "AVG" << "COUNT" << "GROUP_CONCAT" << "MAX" << "MIN"
            << "SUM" << "TOTAL";

    rule.format = functionFormat;
    foreach(const QString& pattern, functionPatterns)
    {
        rule.pattern = QRegExp(QString("\\b%1\\b").arg(pattern));
        rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        highlightingRules.append(rule);
    }

    // single line comment
    rule.pattern = QRegExp("--[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // identifiers
    rule.pattern = QRegExp("\"[^\"]*\""); // ""
    rule.format = identifierFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("\\[[^\\]]*\\]"); // []
    highlightingRules.append(rule);

    rule.pattern = QRegExp("`[^`]*`"); // ``
    highlightingRules.append(rule);

    // string
    rule.pattern = QRegExp("'[^']*'"); // ''
    rule.format = stringFormat;
    highlightingRules.append(rule);
}

void SQLiteSyntaxHighlighter::setTableNames(const QStringList &tablenames)
{
    tableNameRules.clear();
    foreach(const QString& tblname, tablenames) {
        if(!tblname.isEmpty())
        {
            HighlightingRule rule;
            rule.pattern = QRegExp(QString("\\b%1\\b").arg(tblname));
            rule.format = tableFormat;
            tableNameRules.append(rule);
        }
    }
}

void SQLiteSyntaxHighlighter::highlightBlockVector(const QString& text, const QVector<HighlightingRule>& vec)
{
    foreach (const HighlightingRule &rule, vec) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}

void SQLiteSyntaxHighlighter::highlightBlock(const QString &text)
{
    highlightBlockVector(text, tableNameRules);
    highlightBlockVector(text, highlightingRules);
}

QTextCharFormat SQLiteSyntaxHighlighter::createFormat(const QString& settings_name)
{
    QTextCharFormat format;
    format.setForeground(QColor(PreferencesDialog::getSettingsValue("syntaxhighlighter", settings_name + "_colour").toString()));
    format.setFontWeight(PreferencesDialog::getSettingsValue("syntaxhighlighter", settings_name + "_bold").toBool() ? QFont::Bold : QFont::Normal);
    format.setFontItalic(PreferencesDialog::getSettingsValue("syntaxhighlighter", settings_name + "_italic").toBool());
    format.setFontUnderline(PreferencesDialog::getSettingsValue("syntaxhighlighter", settings_name + "_underline").toBool());
    return format;
}
