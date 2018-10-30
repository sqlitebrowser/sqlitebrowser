#include "CondFormat.h"
#include "Settings.h"
#include "Data.h"

CondFormat::CondFormat(const QString& filter, const QColor& color, const QString& encoding)
    : m_filter(filter),
      m_color(color)
{
    m_sqlCondition = filterToSqlCondition(filter, encoding);
}

QString CondFormat::filterToSqlCondition(const QString& value, const QString& encoding)
{
    // Check for any special comparison operators at the beginning of the value string. If there are none default to LIKE.
    QString op = "LIKE";
    QString val, val2;
    QString escape;
    bool numeric = false, ok = false;

    // range/BETWEEN operator
    if (value.contains("~")) {
        int sepIdx = value.indexOf('~');
        val  = value.mid(0, sepIdx);
        val2 = value.mid(sepIdx+1);
        val.toFloat(&ok);
        if (ok) {
            val2.toFloat(&ok);
            ok = ok && (val.toFloat() < val2.toFloat());
        }
    }
    if (ok) {
        op = "BETWEEN";
        numeric = true;
    } else {
        val.clear();
        val2.clear();
        if(value.left(2) == ">=" || value.left(2) == "<=" || value.left(2) == "<>")
        {
            // Check if we're filtering for '<> NULL'. In this case we need a special comparison operator.
            if(value.left(2) == "<>" && value.mid(2) == "NULL")
            {
                // We are filtering for '<>NULL'. Override the comparison operator to search for NULL values in this column. Also treat search value (NULL) as number,
                // in order to avoid putting quotes around it.
                op = "IS NOT";
                numeric = true;
                val = "NULL";
            } else if(value.left(2) == "<>" && value.mid(2) == "''") {
                // We are filtering for "<>''", i.e. for everything which is not an empty string
                op = "<>";
                numeric = true;
                val = "''";
            } else {
                value.mid(2).toFloat(&numeric);
                op = value.left(2);
                val = value.mid(2);
            }
        } else if(value.left(1) == ">" || value.left(1) == "<") {
            value.mid(1).toFloat(&numeric);
            op = value.left(1);
            val = value.mid(1);
        } else if(value.left(1) == "=") {
            val = value.mid(1);

            // Check if value to compare with is 'NULL'
            if(val != "NULL")
            {
                // It's not, so just compare normally to the value, whatever it is.
                op = "=";
            } else {
                // It is NULL. Override the comparison operator to search for NULL values in this column. Also treat search value (NULL) as number,
                // in order to avoid putting quotes around it.
                op = "IS";
                numeric = true;
            }
        } else {
            // Keep the default LIKE operator

            // Set the escape character if one has been specified in the settings dialog
            QString escape_character = Settings::getValue("databrowser", "filter_escape").toString();
            if(escape_character == "'") escape_character = "''";
            if(escape_character.length())
                escape = QString("ESCAPE '%1'").arg(escape_character);

            // Add % wildcards at the start and at the beginning of the filter query, but only if there weren't set any
            // wildcards manually. The idea is to assume that a user who's just typing characters expects the wildcards to
            // be added but a user who adds them herself knows what she's doing and doesn't want us to mess up her query.
            if(!value.contains("%"))
            {
                val = value;
                val.prepend('%');
                val.append('%');
            }
        }
    }
    if(val.isEmpty())
        val = value;

    if(val == "" || val == "%" || val == "%%")
        return QString();
    else {
        // Quote and escape value, but only if it's not numeric and not the empty string sequence
        if(!numeric && val != "''")
            val = QString("'%1'").arg(val.replace("'", "''"));

        QString whereClause(op + " " + QString(encodeString(val.toUtf8(), encoding)));
        if (!val2.isEmpty())
            whereClause += " AND " + QString(encodeString(val2.toUtf8(), encoding));
        whereClause += " " + escape;
        return whereClause;
    }
}
