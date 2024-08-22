#include "CondFormat.h"
#include "Settings.h"
#include "Data.h"
#include "sqlitedb.h"

#include <QAbstractTableModel>

CondFormat::Alignment CondFormat::fromCombinedAlignment(Qt::Alignment align)
{
    if (align.testFlag(Qt::AlignLeft))
        return AlignLeft;
    if (align.testFlag(Qt::AlignRight))
        return AlignRight;
    if (align.testFlag(Qt::AlignCenter))
        return AlignCenter;
    if (align.testFlag(Qt::AlignJustify))
        return AlignJustify;

    return AlignLeft;
}

CondFormat::CondFormat(const QString& filter,
                       const QColor& foreground,
                       const QColor& background,
                       const QFont& font,
                       const Alignment alignment,
                       const QString& encoding)
    : m_filter(filter),
      m_bgColor(background),
      m_fgColor(foreground),
      m_font(font),
      m_align(alignment)
{
    if (!filter.isEmpty())
        m_sqlCondition = filterToSqlCondition(filter, encoding);
}

CondFormat::CondFormat(const QString& filter,
                       const QAbstractTableModel* model,
                       const QModelIndex index,
                       const QString& encoding)
    : m_filter(filter)
{

    if (!filter.isEmpty())
        m_sqlCondition = filterToSqlCondition(filter, encoding);

    m_bgColor = QColor(model->data(index, Qt::BackgroundRole).toString());
    m_fgColor = QColor(model->data(index, Qt::ForegroundRole).toString());
    m_align = fromCombinedAlignment(Qt::Alignment(model->data(index, Qt::TextAlignmentRole).toInt()));
    m_font.fromString(model->data(index, Qt::FontRole).toString());
}

std::string CondFormat::filterToSqlCondition(const QString& value, const QString& encoding)
{
    if(value.isEmpty())
        return {};

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
        float valf = val.toFloat(&ok);
        if (ok) {
            float val2f = val2.toFloat(&ok);
            ok = ok && (valf < val2f);
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
                // TODO: check midRef
                value.mid(2).toFloat(&numeric);
                op = value.left(2);
                val = value.mid(2);
            }
        } else if(value.at(0) == '>' || value.at(0) == '<') {
            // TODO: check midRef
            value.mid(1).toFloat(&numeric);
            op = value.at(0);
            val = value.mid(1);
        } else if(value.at(0) == '=') {
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
        } else if(value.at(0) == '/' && value.at(value.size()-1) == '/' && value.size() > 2) {
            val = value.mid(1, value.length() - 2);
            op = "REGEXP";
            numeric = false;
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

    if(val.isEmpty() || val == "%" || val == "%%")
        return {};
    else {
        // Quote and escape value, but only if it's not numeric and not the empty string sequence
        if(!numeric && val != "''")
            val = sqlb::escapeString(val);

        QString whereClause(op + " " + QString(encodeString(val.toUtf8(), encoding)));
        if (!val2.isEmpty())
            whereClause += " AND " + QString(encodeString(val2.toUtf8(), encoding));
        whereClause += " " + escape;
        return whereClause.toStdString();
    }
}

Qt::AlignmentFlag CondFormat::alignmentFlag() const
{
    switch (m_align) {
    case AlignLeft:
        return Qt::AlignLeft;
    case AlignCenter:
        return Qt::AlignCenter;
    case AlignRight:
        return Qt::AlignRight;
    case AlignJustify:
        return Qt::AlignJustify;
    }

    return Qt::AlignLeft;
}
