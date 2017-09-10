#include "sqlitetablemodel.h"
#include "sqlitedb.h"
#include "sqlite.h"
#include "Settings.h"

#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QTextCodec>
#include <QMimeData>
#include <QFile>
#include <QUrl>

SqliteTableModel::SqliteTableModel(DBBrowserDB& db, QObject* parent, size_t chunkSize, const QString& encoding)
    : QAbstractTableModel(parent)
    , m_db(db)
    , m_rowCount(0)
    , m_chunkSize(chunkSize)
    , m_valid(false)
    , m_encoding(encoding)
{
    reset();
}

void SqliteTableModel::reset()
{
    m_sTable.clear();
    m_sRowidColumn.clear();
    m_iSortColumn = 0;
    m_sSortOrder = "ASC";
    m_headers.clear();
    m_mWhere.clear();
    m_vDataTypes.clear();
    m_vDisplayFormat.clear();
    m_pseudoPk.clear();
}

void SqliteTableModel::setChunkSize(size_t chunksize)
{
    m_chunkSize = chunksize;
}

void SqliteTableModel::setTable(const QString& table, int sortColumn, Qt::SortOrder sortOrder, const QVector<QString>& display_format)
{
    // Unset all previous settings. When setting a table all information on the previously browsed data set is removed first.
    reset();

    // Save the other parameters
    m_sTable = table;
    m_vDisplayFormat = display_format;

    // The first column is the rowid column and therefore is always of type integer
    m_vDataTypes.push_back(SQLITE_INTEGER);

    // Get the data types of all other columns as well as the column names
    bool allOk = false;
    if(m_db.getObjectByName(table)->type() == sqlb::Object::Types::Table)
    {
        sqlb::TablePtr t = m_db.getObjectByName(table).dynamicCast<sqlb::Table>();
        if(t && t->fields().size()) // parsing was OK
        {
            m_sRowidColumn = t->rowidColumn();
            m_headers.push_back(m_sRowidColumn);
            m_headers.append(t->fieldNames());

            // parse columns types
            static QStringList dataTypes = QStringList()
                    << "INTEGER"
                    << "REAL"
                    << "TEXT"
                    << "BLOB";
            foreach(const sqlb::FieldPtr fld,  t->fields())
            {
                QString name(fld->type().toUpper());
                int colType = dataTypes.indexOf(name);
                colType = (colType == -1) ? SQLITE_TEXT : colType + 1;
                m_vDataTypes.push_back(colType);
            }
            allOk = true;
        }
    }

    // If for one reason or another (either it's a view or we couldn't parse the table statement) we couldn't get the field
    // information we retrieve it from SQLite using an extra query.
    // NOTE: It would be nice to eventually get rid of this piece here. As soon as the grammar parser is good enough...
    if(!allOk)
    {
        QString sColumnQuery = QString::fromUtf8("SELECT * FROM %1;").arg(sqlb::escapeIdentifier(table));
        m_sRowidColumn = "rowid";
        m_headers.push_back("rowid");
        m_headers.append(getColumns(sColumnQuery, m_vDataTypes));
    }

    // Set sort parameters. We're setting the sort column to an invalid value before calling sort() because this way, in sort() the
    // current sort order is always changed and thus buildQuery() is always going to be called.
    // This is also why we don't need to call buildQuery() here again.
    m_iSortColumn = -1;
    sort(sortColumn, sortOrder);
}

namespace {
QString rtrimChar(const QString& s, QChar c) {
    QString r = s.trimmed();
    while(r.endsWith(c))
        r.chop(1);
    return r;
}
}

void SqliteTableModel::setQuery(const QString& sQuery, bool dontClearHeaders)
{
    // clear
    if(!dontClearHeaders)
        reset();

    if(!m_db.isOpen())
        return;

    m_sQuery = sQuery.trimmed();

    removeCommentsFromQuery(m_sQuery);

    // do a count query to get the full row count in a fast manner
    m_rowCount = getQueryRowCount();
    if(m_rowCount == -1)
    {
        m_valid = false;
        return;
    }

    // headers
    if(!dontClearHeaders)
    {
        m_headers.append(getColumns(sQuery, m_vDataTypes));
    }

    // now fetch the first entries
    clearCache();
    fetchData(0, m_chunkSize);
    m_valid = true;

    emit layoutChanged();
}

int SqliteTableModel::getQueryRowCount()
{
    // Return -1 if there is an error
    int retval = -1;

    // Use a different approach of determining the row count when a EXPLAIN or a PRAGMA statement is used because a COUNT fails on these queries
    if(m_sQuery.startsWith("EXPLAIN", Qt::CaseInsensitive) || m_sQuery.startsWith("PRAGMA", Qt::CaseInsensitive))
    {
        // So just execute the statement as it is and fetch all results counting the rows
        sqlite3_stmt* stmt;
        QByteArray utf8Query = m_sQuery.toUtf8();
        if(sqlite3_prepare_v2(m_db._db, utf8Query, utf8Query.size(), &stmt, NULL) == SQLITE_OK)
        {
            retval = 0;
            while(sqlite3_step(stmt) == SQLITE_ROW)
                retval++;
            sqlite3_finalize(stmt);

            // Return the results but also set the chunk size the number of rows to prevent the lazy population mechanism to kick in as using LIMIT
            // fails on this kind of queries as well
            m_chunkSize = retval;
            return retval;
        }
    } else {
        // If it is a normal query - hopefully starting with SELECT - just do a COUNT on it and return the results
        QString sCountQuery = QString("SELECT COUNT(*) FROM (%1);").arg(rtrimChar(m_sQuery, ';'));
        m_db.logSQL(sCountQuery, kLogMsg_App);
        QByteArray utf8Query = sCountQuery.toUtf8();

        sqlite3_stmt* stmt;
        int status = sqlite3_prepare_v2(m_db._db, utf8Query, utf8Query.size(), &stmt, NULL);
        if(status == SQLITE_OK)
        {
            status = sqlite3_step(stmt);
            if(status == SQLITE_ROW)
            {
                QString sCount = QString::fromUtf8((const char*)sqlite3_column_text(stmt, 0));
                retval = sCount.toInt();
            }
            sqlite3_finalize(stmt);
        } else {
            qWarning() << "Count query failed: " << sCountQuery;
        }
    }

    return retval;
}

int SqliteTableModel::rowCount(const QModelIndex&) const
{
    return m_data.size(); // current fetched row count
}

int SqliteTableModel::totalRowCount() const
{
    return m_rowCount;
}

int SqliteTableModel::columnCount(const QModelIndex&) const
{
    return m_headers.size();
}

QVariant SqliteTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        // if we have a VIRTUAL table the model will not be valid, with no header data
        if(section < m_headers.size())
            return m_headers.at(section);

        return QString("%1").arg(section + 1);
    }
    else
        return QString("%1").arg(section + 1);
}

QVariant SqliteTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_rowCount)
        return QVariant();

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        // If this row is not in the cache yet get it first
        while(index.row() >= m_data.size() && canFetchMore())
            const_cast<SqliteTableModel*>(this)->fetchMore();   // Nothing evil to see here, move along

        if(role == Qt::DisplayRole && m_data.at(index.row()).at(index.column()).isNull())
        {
            return Settings::getValue("databrowser", "null_text").toString();
        } else if(role == Qt::DisplayRole && isBinary(index)) {
            return "BLOB";
        } else if(role == Qt::DisplayRole) {
            int limit = Settings::getValue("databrowser", "symbol_limit").toInt();
            QByteArray displayText = m_data.at(index.row()).at(index.column());
            if (displayText.length() > limit) {
                // Add "..." to the end of truncated strings
                return decode(displayText.left(limit).append(" ..."));
            } else {
                return decode(displayText);
            }
        } else {
            return decode(m_data.at(index.row()).at(index.column()));
        }
    } else if(role == Qt::FontRole) {
        QFont font;
        if(m_data.at(index.row()).at(index.column()).isNull() || isBinary(index))
            font.setItalic(true);
        return font;
    } else if(role == Qt::ForegroundRole) {
        if(m_data.at(index.row()).at(index.column()).isNull())
            return QColor(Settings::getValue("databrowser", "null_fg_colour").toString());
        else if (isBinary(index))
            return QColor(Settings::getValue("databrowser", "bin_fg_colour").toString());
        return QColor(Settings::getValue("databrowser", "reg_fg_colour").toString());
    } else if (role == Qt::BackgroundRole) {
        if(m_data.at(index.row()).at(index.column()).isNull())
            return QColor(Settings::getValue("databrowser", "null_bg_colour").toString());
        else if (isBinary(index))
            return QColor(Settings::getValue("databrowser", "bin_bg_colour").toString());
        return QColor(Settings::getValue("databrowser", "reg_bg_colour").toString());
    } else if(role == Qt::ToolTipRole) {
        sqlb::ForeignKeyClause fk = getForeignKeyClause(index.column()-1);
        if(fk.isSet())
            return tr("References %1(%2)\nHold Ctrl+Shift and click to jump there").arg(fk.table()).arg(fk.columns().join(","));
        else
            return QString();
    } else {
        return QVariant();
    }
}

sqlb::ForeignKeyClause SqliteTableModel::getForeignKeyClause(int column) const
{
    static const sqlb::ForeignKeyClause empty_foreign_key_clause;

    // No foreign keys when not browsing a table. This usually happens when executing custom SQL statements
    // and browsing the result set instead of browsing an entire table.
    if(m_sTable.isEmpty())
        return empty_foreign_key_clause;

    // Retrieve database object and check if it is a table. If it isn't stop here and don't return a foreign
    // key. This happens for views which don't have foreign keys (though we might want to think about how we
    // can check for foreign keys in the underlying tables for some purposes like tool tips).
    sqlb::ObjectPtr obj = m_db.getObjectByName(m_sTable);
    if(obj->type() != sqlb::Object::Table)
        return empty_foreign_key_clause;

    // Convert object to a table and check if the column number is in the valid range
    sqlb::TablePtr tbl = obj.dynamicCast<sqlb::Table>();
    if(tbl && tbl->name().size() && (column >= 0 && column < tbl->fields().count()))
    {
        // Note that the rowid column has number -1 here, it can safely be excluded since there will never be a
        // foreign key on that column.

        sqlb::ConstraintPtr ptr = tbl->constraint({tbl->fields().at(column)}, sqlb::Constraint::ForeignKeyConstraintType);
        if(ptr)
            return *(ptr.dynamicCast<sqlb::ForeignKeyClause>());
    }

    return empty_foreign_key_clause;
}

bool SqliteTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    // Don't even try setting any data if we're not browsing a table, i.e. the model data comes from a custom query
    if(!isEditable())
        return false;

    // This function is for in-place editing.
    // So, BLOB flag is false every times.
    return setTypedData(index, false, value, role);
}

bool SqliteTableModel::setTypedData(const QModelIndex& index, bool isBlob, const QVariant& value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        QByteArray newValue = encode(value.toByteArray());
        QByteArray oldValue = m_data.at(index.row()).at(index.column());

        // Special handling for integer columns: instead of setting an integer column to an empty string, set it to '0' when it is also
        // used in a primary key. Otherwise SQLite will always output an 'datatype mismatch' error.
        if(newValue == "" && !newValue.isNull())
        {
            sqlb::TablePtr table = m_db.getObjectByName(m_sTable).dynamicCast<sqlb::Table>();
            if(table)
            {
                sqlb::FieldPtr field = table->field(table->findField(m_headers.at(index.column())));
                if(table->primaryKey().contains(field) && field->isInteger())
                    newValue = "0";
            }
        }

        // Don't do anything if the data hasn't changed
        // To differentiate NULL and empty byte arrays, we also compare the NULL flag
        if(oldValue == newValue && oldValue.isNull() == newValue.isNull())
            return true;

        if(m_db.updateRecord(m_sTable, m_headers.at(index.column()), m_data[index.row()].at(0), newValue, isBlob, m_pseudoPk))
        {
            // Only update the cache if this row has already been read, if not there's no need to do any changes to the cache
            if(index.row() < m_data.size())
                m_data[index.row()].replace(index.column(), newValue);

            emit(dataChanged(index, index));
            return true;
        } else {
            QMessageBox::warning(0, qApp->applicationName(), tr("Error changing data:\n%1").arg(m_db.lastError()));
            return false;
        }
    }

    return false;
}

bool SqliteTableModel::canFetchMore(const QModelIndex&) const
{
    return m_data.size() < m_rowCount;
}

void SqliteTableModel::fetchMore(const QModelIndex&)
{
    int row = m_data.size();
    fetchData(row, row + m_chunkSize);
}

Qt::ItemFlags SqliteTableModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::ItemIsEnabled;

    Qt::ItemFlags ret = QAbstractTableModel::flags(index) | Qt::ItemIsDropEnabled;

    // Custom display format set?
    bool custom_display_format = false;
    if(m_vDisplayFormat.size())
    {
        // NOTE: This assumes that custom display formats never start and end with a backtick
        if(index.column() > 0)
            custom_display_format = !(m_vDisplayFormat.at(index.column()-1).startsWith("`") && m_vDisplayFormat.at(index.column()-1).endsWith("`"));
    }

    if(!isBinary(index) && !custom_display_format)
        ret |= Qt::ItemIsEditable;
    return ret;
}

void SqliteTableModel::sort(int column, Qt::SortOrder order)
{
    // Don't do anything when the sort order hasn't changed
    if(m_iSortColumn == column && m_sSortOrder == (order == Qt::AscendingOrder ? "ASC" : "DESC"))
        return;

    // Save sort order
	if (column >= 0 && column < m_headers.size())
		m_iSortColumn = column;
    m_sSortOrder = (order == Qt::AscendingOrder ? "ASC" : "DESC");

    // Set the new query (but only if a table has already been set
    if(m_sTable != "")
        buildQuery();
}

bool SqliteTableModel::insertRows(int row, int count, const QModelIndex& parent)
{
    if(!isEditable())
        return false;

    QByteArrayList blank_data;
    for(int i=0; i < m_headers.size(); ++i)
        blank_data.push_back("");

    DataType tempList;
    for(int i=row; i < row + count; ++i)
    {
        QString rowid = m_db.addRecord(m_sTable);
        if(rowid.isNull())
        {
            return false;
        }
        m_rowCount++;
        tempList.append(blank_data);
        tempList[i - row].replace(0, rowid.toUtf8());

        // update column with default values
        QByteArrayList rowdata;
        if( m_db.getRow(m_sTable, rowid, rowdata) )
        {
            for(int j=1; j < m_headers.size(); ++j)
            {
                tempList[i - row].replace(j, rowdata[j - 1]);
            }
        }
    }

    beginInsertRows(parent, row, row + count - 1);
    for(int i = 0; i < tempList.size(); ++i)
    {
        m_data.insert(i + row, tempList.at(i));
    }
    endInsertRows();
    return true;
}

bool SqliteTableModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if(!isEditable())
        return false;

    beginRemoveRows(parent, row, row + count - 1);

    bool ok = true;

    QStringList rowids;
    for(int i=count-1;i>=0;i--)
    {
        rowids.append(m_data.at(row + i).at(0));
        m_data.removeAt(row + i);
        --m_rowCount;
    }
    if(!m_db.deleteRecords(m_sTable, rowids))
    {
        ok = false;
    }

    endRemoveRows();
    return ok;
}

QModelIndex SqliteTableModel::dittoRecord(int old_row)
{
    if(!isEditable())
        return QModelIndex();

    insertRow(rowCount());
    int firstEditedColumn = 0;
    int new_row = rowCount() - 1;

    sqlb::TablePtr t = m_db.getObjectByName(m_sTable).dynamicCast<sqlb::Table>();

    sqlb::FieldVector pk = t->primaryKey();
    for (int col = 0; col < t->fields().size(); ++col) {
        if(!pk.contains(t->fields().at(col))) {
            if (!firstEditedColumn)
                firstEditedColumn = col + 1;

            QVariant value = data(index(old_row, col + 1), Qt::EditRole);
            setData(index(new_row, col + 1), value);
        }
    }

    return index(new_row, firstEditedColumn);
}

void SqliteTableModel::fetchData(unsigned int from, unsigned to)
{
    int currentsize = m_data.size();

    QString sLimitQuery;
    if(m_sQuery.startsWith("PRAGMA", Qt::CaseInsensitive) || m_sQuery.startsWith("EXPLAIN", Qt::CaseInsensitive))
    {
        sLimitQuery = m_sQuery;
    } else {
        // Remove trailing trailing semicolon
        QString queryTemp = rtrimChar(m_sQuery, ';');

        // If the query ends with a LIMIT statement take it as it is, if not append our own LIMIT part for lazy population
        if(queryTemp.contains(QRegExp("LIMIT\\s+.+\\s*((,|\\b(OFFSET)\\b)\\s*.+\\s*)?$", Qt::CaseInsensitive)))
            sLimitQuery = queryTemp;
        else
            sLimitQuery = queryTemp + QString(" LIMIT %1, %2;").arg(from).arg(to-from);
    }
    m_db.logSQL(sLimitQuery, kLogMsg_App);
    QByteArray utf8Query = sLimitQuery.toUtf8();
    sqlite3_stmt *stmt;
    int status = sqlite3_prepare_v2(m_db._db, utf8Query, utf8Query.size(), &stmt, NULL);

    if(SQLITE_OK == status)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            QByteArrayList rowdata;
            for (int i = 0; i < m_headers.size(); ++i)
            {
                if(sqlite3_column_type(stmt, i) == SQLITE_NULL)
                {
                    rowdata.append(QByteArray());
                } else {
                    int bytes = sqlite3_column_bytes(stmt, i);
                    if(bytes)
                        rowdata.append(QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, i)), bytes));
                    else
                        rowdata.append(QByteArray(""));
                }
            }
            m_data.push_back(rowdata);
        }
    }
    sqlite3_finalize(stmt);

    // Check if there was any new data
    if(m_data.size() > currentsize)
    {
        beginInsertRows(QModelIndex(), currentsize, m_data.size()-1);
        endInsertRows();
    }
}

void SqliteTableModel::buildQuery()
{
    QString where;

    if(m_mWhere.size())
    {
        where = "WHERE ";

        for(QMap<int, QString>::const_iterator i=m_mWhere.constBegin();i!=m_mWhere.constEnd();++i)
        {
            QString column;
            if(m_vDisplayFormat.size())
                column = QString("col%1").arg(i.key());
            else
                column = m_headers.at(i.key());
            where.append(QString("%1 %2 AND ").arg(sqlb::escapeIdentifier(column)).arg(i.value()));
        }

        // Remove last 'AND '
        where.chop(4);
    }

    QString selector;
    if(m_vDisplayFormat.empty())
    {
        selector = "*";
    } else {
        for(int i=0;i<m_vDisplayFormat.size();i++)
            selector += m_vDisplayFormat.at(i) + " AS " + QString("col%1").arg(i+1) + ",";
        selector.chop(1);
    }

    // Note: Building the SQL string is intentionally split into several parts here instead of arg()'ing it all together as one.
    // The reason is that we're adding '%' characters automatically around search terms (and even if we didn't the user could add
    // them manually) which means that e.g. searching for '1' results in another '%1' in the string which then totally confuses
    // the QString::arg() function, resulting in an invalid SQL.
    QString sql = QString("SELECT %1,%2 FROM %3 ")
            .arg(sqlb::escapeIdentifier(m_headers.at(0)))
            .arg(selector)
            .arg(sqlb::escapeIdentifier(m_sTable))
            + where
            + QString("ORDER BY %1 %2")
            .arg(sqlb::escapeIdentifier(m_headers.at(m_iSortColumn)))
            .arg(m_sSortOrder);
    setQuery(sql, true);
}

void SqliteTableModel::removeCommentsFromQuery(QString& query) {
    // first remove block comments
    {
        QRegExp rxSQL("^((?:(?:[^'/]|/(?![*]))*|'[^']*')*)(/[*](?:[^*]|[*](?!/))*[*]/)(.*)$");	// set up regex to find block comment
        QString result;

        while(query.size() != 0)
        {
            int pos = rxSQL.indexIn(query);
            if(pos > -1)
            {
                result += rxSQL.cap(1) + " ";
                query = rxSQL.cap(3);
            } else {
                result += query;
                query = "";
            }
        }
        query = result;
    }

    // deal with end-of-line comments
    {
        /* The regular expression for removing end of line comments works like this:
         * ^((?:(?:[^'-]|-(?!-))*|(?:'[^']*'))*)(--.*)$
         * ^                                          $ # anchor beginning and end of string so we use it all
         *  (                                  )(    )  # two separate capture groups for code and comment
         *                                       --.*   # comment starts with -- and consumes everything afterwards
         *   (?:                 |           )*         # code is none or many strings alternating with non-strings
         *                        (?:'[^']*')           # a string is a quote, followed by none or more non-quotes, followed by a quote
         *      (?:[^'-]|-(?!-))*                       # non-string is a sequence of characters which aren't quotes or hyphens,
         *                                                OR if they are hyphens then they can't be followed immediately by another hyphen
         */
        QRegExp rxSQL("^((?:(?:[^'-]|-(?!-))*|(?:'[^']*'))*)(--[^\\r\\n]*)([\\r\\n]*)(.*)$");	// set up regex to find end-of-line comment
        QString result;

        while(query.size() != 0)
        {
            int pos = rxSQL.indexIn(query);
            if(pos > -1)
            {
                result += rxSQL.cap(1) + rxSQL.cap(3);
                query = rxSQL.cap(4);
            } else {
                result += query;
                query = "";
            }
        }

        query = result.trimmed();
    }

    // Remove multiple line breaks that might have been created by deleting comments till the end of the line but not including the line break
    query.replace(QRegExp("\\n+"), "\n");

    // Also remove any remaining whitespace at the end of each line
    query.replace(QRegExp("[ \t]+\n"), "\n");
}

QStringList SqliteTableModel::getColumns(const QString& sQuery, QVector<int>& fieldsTypes)
{
    sqlite3_stmt* stmt;
    QByteArray utf8Query = sQuery.toUtf8();
    int status = sqlite3_prepare_v2(m_db._db, utf8Query, utf8Query.size(), &stmt, NULL);
    QStringList listColumns;
    if(SQLITE_OK == status)
    {
        sqlite3_step(stmt);
        int columns = sqlite3_data_count(stmt);
        for(int i = 0; i < columns; ++i)
        {
            listColumns.append(QString::fromUtf8((const char*)sqlite3_column_name(stmt, i)));
            fieldsTypes.push_back(sqlite3_column_type(stmt, i));
        }
    }
    sqlite3_finalize(stmt);

    return listColumns;
}

void SqliteTableModel::updateFilter(int column, const QString& value)
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
                // We are filtering for '<> NULL'. Override the comparison operator to search for NULL values in this column. Also treat search value (NULL) as number,
                // in order to avoid putting quotes around it.
                op = "IS NOT";
                numeric = true;
                val = "NULL";
            } else {
                bool ok;
                value.mid(2).toFloat(&ok);
                if(ok)
                {
                    op = value.left(2);
                    val = value.mid(2);
                    numeric = true;
                }
            }
        } else if(value.left(1) == ">" || value.left(1) == "<") {
            bool ok;
            value.mid(1).toFloat(&ok);
            if(ok)
            {
                op = value.left(1);
                val = value.mid(1);
                numeric = true;
            }
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
    if(!numeric)
        val = QString("'%1'").arg(val.replace("'", "''"));

    // If the value was set to an empty string remove any filter for this column. Otherwise insert a new filter rule or replace the old one if there is already one
    if(val == "''" || val == "'%'" || val == "'%%'")
        m_mWhere.remove(column);
    else {
        QString whereClause(op + " " + QString(encode(val.toUtf8())));
        if (!val2.isEmpty())
            whereClause += " AND " + QString(encode(val2.toUtf8()));
        whereClause += " " + escape;
        m_mWhere.insert(column, whereClause);
    }

    // Build the new query
    buildQuery();
}

void SqliteTableModel::clearCache()
{
	if(!m_data.empty())
	{
		beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
		m_data.clear();
		endRemoveRows();
	}
}

bool SqliteTableModel::isBinary(const QModelIndex& index) const
{
    return m_data.at(index.row()).at(index.column()).left(1024).contains('\0');
}

QByteArray SqliteTableModel::encode(const QByteArray& str) const
{
    if(m_encoding.isEmpty())
        return str;
    else
        return QTextCodec::codecForName(m_encoding.toUtf8())->fromUnicode(str);
}

QByteArray SqliteTableModel::decode(const QByteArray& str) const
{
    if(m_encoding.isEmpty())
        return str;
    else
        return QTextCodec::codecForName(m_encoding.toUtf8())->toUnicode(str).toUtf8();
}

Qt::DropActions SqliteTableModel::supportedDropActions() const
{
    return Qt::CopyAction;
}

bool SqliteTableModel::dropMimeData(const QMimeData* data, Qt::DropAction, int row, int column, const QModelIndex& parent)
{
    // What has been dropped on the widget?
    if(data->hasUrls())
    {
        // If it's a URL, open the file and paste the content in the current cell
        QList<QUrl> urls = data->urls();
        QFile file(urls.first().toLocalFile());
        if(file.exists() && file.open(QFile::ReadOnly))
        {
            setData(index(row, column, parent), file.readAll());
            return true;
        }
    } else if(data->hasText()) {
        // If it's just text we can set the cell data directly
        setData(index(row, column, parent), data->text());
    }

    return false;
}

void SqliteTableModel::setPseudoPk(const QString& pseudoPk)
{
    if(pseudoPk.isEmpty())
    {
        m_pseudoPk.clear();
        if(m_headers.size())
            m_headers[0] = m_sRowidColumn;
    } else {
        m_pseudoPk = pseudoPk;
        if(m_headers.size())
            m_headers[0] = pseudoPk;
    }

    buildQuery();
}

bool SqliteTableModel::isEditable() const
{
    return !m_sTable.isEmpty();
}
