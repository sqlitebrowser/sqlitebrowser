#ifndef JSONPATHCHECKER_H
#define JSONPATHCHECKER_H

#include <QString>
#include <QRegularExpression>
#include <QCoreApplication>

/**
 * Detects invalid SQLite JSON path syntax in a SQL statement and returns a
 * warning message if found, or an empty string if the statement looks fine.
 *
 * The specific pattern detected here is a bare negative array index inside a
 * JSON path expression, e.g. '$[-1]'. This is valid syntax in MySQL/MariaDB
 * but is NOT valid in SQLite. SQLite uses the '#' symbol to refer to the array
 * length, so the correct SQLite equivalent for "last element" is '$[#-1]', not
 * '$[-1]'.
 *
 * The danger of this mistake is that SQLite silently returns NULL for the
 * invalid path instead of raising an error. When such an expression appears in
 * a WHERE clause, it causes rows to be silently dropped from the result set —
 * a data-loss bug that is very hard to diagnose.
 *
 * See: https://github.com/sqlitebrowser/sqlitebrowser/issues/4113
 * See: https://sqlite.org/json1.html (path syntax section)
 */
inline QString checkForInvalidJsonPaths(const QString& query)
{
    // Match any JSON path step that uses a bare negative array index, e.g.:
    //   $[-1]          — negative index at the root
    //   $.array[-1]    — negative index after a key step
    //   $[0][-1]       — negative index after another array step
    //
    // The three alternates in the leading group cover these three cases:
    //   \$              — root anchor  (e.g. $[-1])
    //   \.[\w*@]+       — key step     (e.g. .array[-1])
    //   \]              — array step   (e.g. [0][-1])
    //
    // The negative look-ahead (?!#) ensures we do NOT match the valid SQLite
    // form $[#-N], which uses '#' as the array-length placeholder.
    // Capture group 1 isolates the bare negative index step (e.g. '[-1]') so
    // the warning message shows the problematic part without the leading anchor.
    static const QRegularExpression invalidJsonPathRx(
        QStringLiteral(R"((?:\$|\.[\w*@]+|\])(\[(?!#)-\d+\]))"),
        QRegularExpression::CaseInsensitiveOption
    );

    const QRegularExpressionMatch match = invalidJsonPathRx.match(query);
    if(match.hasMatch())
    {
        return QCoreApplication::translate("RunSql",
            "Warning: the JSON path step '%1' uses a bare negative array index which is not "
            "supported by SQLite. SQLite silently returns NULL for such paths, which may "
            "cause rows to be silently missing from your results. "
            "Use '[#-N]' syntax instead (e.g. '$[#-1]' for the last element). "
            "See: https://sqlite.org/json1.html")
            .arg(match.captured(1));
    }

    return {};
}

#endif // JSONPATHCHECKER_H
