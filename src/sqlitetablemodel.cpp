#include "sqlitetablemodel.h"
#include "sqlitedb.h"
#include "sqlite.h"
#include "Settings.h"
#include "Data.h"

#include <QMessageBox>
#include <QApplication>
#include <QTextCodec>
#include <QMimeData>
#include <QFile>
#include <QUrl>
#include <QtConcurrent/QtConcurrentRun>
#include <QProgressDialog>

#include "RowLoader.h"

SqliteTableModel::SqliteTableModel(DBBrowserDB& db, QObject* parent, size_t chunkSize, const QString& encoding)
    : QAbstractTableModel(parent)
    , m_db(db)
    , m_lifeCounter(0)
    , m_currentRowCount(0)
    , m_chunkSize(chunkSize)
    , m_encoding(encoding)
{
    worker = new RowLoader(
        [this](){ return m_db.get(tr("reading rows")); },
        [this](QString stmt){ return m_db.logSQL(stmt, kLogMsg_App); },
        m_headers, m_mutexDataCache, m_cache
        );

    worker->start();

    // any UI updates must be performed in the UI thread, not in the worker thread:
    connect(worker, &RowLoader::fetched, this, &SqliteTableModel::handleFinishedFetch, Qt::QueuedConnection);
    connect(worker, &RowLoader::rowCountComplete, this, &SqliteTableModel::handleRowCountComplete, Qt::QueuedConnection);

    reset();
}

SqliteTableModel::~SqliteTableModel()
{
    worker->stop();
    worker->wait();
    worker->disconnect();
    delete worker;
}

SqliteTableModel::RowCount SqliteTableModel::rowCountAvailable () const
{
    return m_rowCountAvailable;
}

void SqliteTableModel::handleFinishedFetch (int life_id, unsigned int fetched_row_begin, unsigned int fetched_row_end)
{
    if(life_id < m_lifeCounter)
        return;

    Q_ASSERT(fetched_row_end >= fetched_row_begin);

    auto old_row_count = m_currentRowCount;

    auto new_row_count = std::max(old_row_count, fetched_row_begin);
    new_row_count = std::max(new_row_count, fetched_row_end);
    Q_ASSERT(new_row_count >= old_row_count);

    if(new_row_count != old_row_count)
    {
        beginInsertRows(QModelIndex(), old_row_count, new_row_count - 1);
        m_currentRowCount = new_row_count;
        endInsertRows();
    }

    if(fetched_row_end != fetched_row_begin)
    {
        // TODO optimize
        int num_columns = m_headers.size();
        emit dataChanged(createIndex(fetched_row_begin, 0), createIndex(fetched_row_end - 1, num_columns - 1));
    }

    if(m_rowCountAvailable != RowCount::Complete)
        m_rowCountAvailable = RowCount::Partial;

    emit finishedFetch(fetched_row_begin, fetched_row_end);
}

void SqliteTableModel::handleRowCountComplete (int life_id, int num_rows)
{
    if(life_id < m_lifeCounter)
        return;

    m_rowCountAvailable = RowCount::Complete;
    handleFinishedFetch(life_id, num_rows, num_rows);
}

void SqliteTableModel::reset()
{
    beginResetModel();
    clearCache();

    m_sTable.clear();
    m_sRowidColumn.clear();
    m_iSortColumn = 0;
    m_sSortOrder = "ASC";
    m_headers.clear();
    m_mWhere.clear();
    m_vDataTypes.clear();
    m_vDisplayFormat.clear();
    m_pseudoPk.clear();

    endResetModel();
}

void SqliteTableModel::setChunkSize(size_t chunksize)
{
    m_chunkSize = chunksize;
}

void SqliteTableModel::setTable(const sqlb::ObjectIdentifier& table, int sortColumn, Qt::SortOrder sortOrder, const QVector<QString>& display_format)
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
            for(const sqlb::FieldPtr& fld :  t->fields())
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
        QString sColumnQuery = QString::fromUtf8("SELECT * FROM %1;").arg(table.toString());
        m_sRowidColumn = "rowid";
        m_headers.push_back("rowid");
        m_headers.append(getColumns(nullptr, sColumnQuery, m_vDataTypes));
    }

    // Set sort parameters. We're setting the sort column to an invalid value before calling sort() because this way, in sort() the
    // current sort order is always changed and thus buildQuery() is always going to be called.
    // This is also why we don't need to call buildQuery() here again.
    m_iSortColumn = -1;
    sort(sortColumn, sortOrder);
}

void SqliteTableModel::setQuery(const QString& sQuery, bool dontClearHeaders)
{
    // clear
    if(!dontClearHeaders)
        reset();
    else
        clearCache();

    if(!m_db.isOpen())
        return;

    m_sQuery = sQuery.trimmed();
    removeCommentsFromQuery(m_sQuery);

    worker->setQuery(m_sQuery);
    worker->triggerRowCountDetermination(m_lifeCounter);

    if(!dontClearHeaders)
        m_headers.append(getColumns(worker->getDb(), sQuery, m_vDataTypes));

    // now fetch the first entries
    triggerCacheLoad(m_chunkSize / 2 - 1);

    emit layoutChanged();
}

int SqliteTableModel::rowCount(const QModelIndex&) const
{
    return m_currentRowCount;
}

int SqliteTableModel::columnCount(const QModelIndex&) const
{
    return m_headers.size();
}

int SqliteTableModel::filterCount() const
{
    return m_mWhere.size();
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

    if (index.row() >= rowCount())
        return QVariant();

    QMutexLocker lock(&m_mutexDataCache);

    Row blank_data;
    bool row_available;

    const Row * cached_row;
    if(m_cache.count(index.row()))
    {
        cached_row = &m_cache.at(index.row());
        row_available = true;
    } else {
        blank_data = makeDefaultCacheEntry();
        cached_row = &blank_data;
        row_available = false;
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(!row_available)
            return tr("loading...");
        if(role == Qt::DisplayRole && cached_row->at(index.column()).isNull())
        {
            return Settings::getValue("databrowser", "null_text").toString();
        } else if(role == Qt::DisplayRole && nosync_isBinary(index)) {
            return Settings::getValue("databrowser", "blob_text").toString();
        } else if(role == Qt::DisplayRole) {
            int limit = Settings::getValue("databrowser", "symbol_limit").toInt();
            QByteArray displayText = cached_row->at(index.column());
            if (displayText.length() > limit) {
                // Add "..." to the end of truncated strings
                return decode(displayText.left(limit).append(" ..."));
            } else {
                return decode(displayText);
            }
        } else {
            return decode(cached_row->at(index.column()));
        }
    } else if(role == Qt::FontRole) {
        QFont font;
        if(!row_available || cached_row->at(index.column()).isNull() || nosync_isBinary(index))
            font.setItalic(true);
        return font;
    } else if(role == Qt::ForegroundRole) {
        if(!row_available)
            return QColor(100, 100, 100);
        if(cached_row->at(index.column()).isNull())
            return QColor(Settings::getValue("databrowser", "null_fg_colour").toString());
        else if (nosync_isBinary(index))
            return QColor(Settings::getValue("databrowser", "bin_fg_colour").toString());
        return QColor(Settings::getValue("databrowser", "reg_fg_colour").toString());
    } else if (role == Qt::BackgroundRole) {
        if(!row_available)
            return QColor(255, 200, 200);
        if(cached_row->at(index.column()).isNull())
            return QColor(Settings::getValue("databrowser", "null_bg_colour").toString());
        else if (nosync_isBinary(index))
            return QColor(Settings::getValue("databrowser", "bin_bg_colour").toString());
        return QColor(Settings::getValue("databrowser", "reg_bg_colour").toString());
    } else if(role == Qt::ToolTipRole) {
        sqlb::ForeignKeyClause fk = getForeignKeyClause(index.column()-1);
        if(fk.isSet())
            return tr("References %1(%2)\nHold Ctrl+Shift and click to jump there").arg(fk.table()).arg(fk.columns().join(","));
        else
            return QString();
    }

    return QVariant();
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
    if(readingData()) {
        // can't insert rows while reading data in background
        return false;
    }

    if(index.isValid() && role == Qt::EditRole)
    {
        QMutexLocker lock(&m_mutexDataCache);

        auto & cached_row = m_cache.at(index.row());

        QByteArray newValue = encode(value.toByteArray());
        QByteArray oldValue = cached_row.at(index.column());

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

        if(m_db.updateRecord(m_sTable, m_headers.at(index.column()), cached_row.at(0), newValue, isBlob, m_pseudoPk))
        {
            cached_row.replace(index.column(), newValue);
            lock.unlock();
            emit dataChanged(index, index);
            return true;
        } else {
            lock.unlock();
            QMessageBox::warning(nullptr, qApp->applicationName(), tr("Error changing data:\n%1").arg(m_db.lastError()));
            return false;
        }
    }

    return false;
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
    if(!m_sTable.isEmpty())
        buildQuery();
}

SqliteTableModel::Row SqliteTableModel::makeDefaultCacheEntry () const
{
    Row blank_data;

    for(int i=0; i < m_headers.size(); ++i)
        blank_data.push_back("");

    return blank_data;
}

bool SqliteTableModel::readingData() const
{
    return worker->readingData();
}

bool SqliteTableModel::insertRows(int row, int count, const QModelIndex& parent)
{
    if(!isEditable())
        return false;

    if(readingData()) {
        // can't insert rows while reading data in background
        return false;
    }

    const auto blank_data = makeDefaultCacheEntry();

    std::vector<Row> tempList;
    for(int i=row; i < row + count; ++i)
    {
        QString rowid = m_db.addRecord(m_sTable);
        if(rowid.isNull())
        {
            return false;
        }
        tempList.push_back(blank_data);
        tempList.back().replace(0, rowid.toUtf8());

        // update column with default values
        Row rowdata;
        if(m_db.getRow(m_sTable, rowid, rowdata))
        {
            for(int j=1; j < m_headers.size(); ++j)
            {
                tempList.back().replace(j, rowdata[j - 1]);
            }
        }
    }

    beginInsertRows(parent, row, row + count - 1);
    for(unsigned int i = 0; i < tempList.size(); ++i)
    {
        m_cache.insert(i + row, std::move(tempList.at(i)));
        m_currentRowCount++;
    }
    endInsertRows();

    return true;
}

bool SqliteTableModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if(!isEditable())
        return false;

    if(readingData()) {
        // can't delete rows while reading data in background
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);

    QStringList rowids;
    for(int i=count-1;i>=0;i--)
    {
        if(m_cache.count(row+i)) {
            rowids.append(m_cache.at(row + i).at(0));
        }
        m_cache.erase(row + i);
        m_currentRowCount--;
    }

    bool ok = m_db.deleteRecords(m_sTable, rowids, m_pseudoPk);

    endRemoveRows();
    return ok;
}

QModelIndex SqliteTableModel::dittoRecord(int old_row)
{
    if(!isEditable())
        return QModelIndex();

    if (!insertRow(rowCount()))
        return QModelIndex();

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

QString SqliteTableModel::customQuery(bool withRowid)
{
    QString where;

    if(m_mWhere.size())
    {
        where = "WHERE ";

        for(QMap<int, QString>::const_iterator i=m_mWhere.constBegin();i!=m_mWhere.constEnd();++i)
        {
            QString columnId = sqlb::escapeIdentifier(m_headers.at(i.key()));
            if(m_vDisplayFormat.size() && m_vDisplayFormat.at(i.key()-1) != columnId)
                columnId = m_vDisplayFormat.at(i.key()-1);
            where.append(QString("%1 %2 AND ").arg(columnId).arg(i.value()));
        }

        // Remove last 'AND '
        where.chop(4);
    }

    QString selector;
    if (withRowid)
        selector = sqlb::escapeIdentifier(m_headers.at(0)) + ",";

    if(m_vDisplayFormat.empty())
    {
        selector += "*";
    } else {
        QString columnId;
        for(int i=0;i<m_vDisplayFormat.size();i++) {
            columnId = sqlb::escapeIdentifier(m_headers.at(i+1));
            if (columnId != m_vDisplayFormat.at(i))
                selector += m_vDisplayFormat.at(i) + " AS " + columnId + ",";
            else
                selector += columnId + ",";
        }
        selector.chop(1);
    }

    // Note: Building the SQL string is intentionally split into several parts here instead of arg()'ing it all together as one.
    // The reason is that we're adding '%' characters automatically around search terms (and even if we didn't the user could add
    // them manually) which means that e.g. searching for '1' results in another '%1' in the string which then totally confuses
    // the QString::arg() function, resulting in an invalid SQL.
    return QString("SELECT %1 FROM %2 ")
            .arg(selector)
            .arg(m_sTable.toString())
            + where
            + QString("ORDER BY %1 %2")
            .arg(sqlb::escapeIdentifier(m_headers.at(m_iSortColumn)))
            .arg(m_sSortOrder);
}

void SqliteTableModel::buildQuery()
{
    setQuery(customQuery(true), true);
}

void SqliteTableModel::removeCommentsFromQuery(QString& query)
{
    int oldSize = query.size();

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

    if (oldSize != query.size()) {
        // Remove multiple line breaks that might have been created by deleting comments till the end of the line but not including the line break
        query.replace(QRegExp("\\n+"), "\n");

        // Also remove any remaining whitespace at the end of each line
        query.replace(QRegExp("[ \t]+\n"), "\n");
    }
}

QStringList SqliteTableModel::getColumns(std::shared_ptr<sqlite3> pDb, const QString& sQuery, QVector<int>& fieldsTypes)
{
    if(!pDb)
        pDb = m_db.get(tr("retrieving list of columns"));

    sqlite3_stmt* stmt;
    QByteArray utf8Query = sQuery.toUtf8();
    int status = sqlite3_prepare_v2(pDb.get(), utf8Query, utf8Query.size(), &stmt, nullptr);
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

    // If the value was set to an empty string remove any filter for this column. Otherwise insert a new filter rule or replace the old one if there is already one
    if(val == "" || val == "%" || val == "%%")
        m_mWhere.remove(column);
    else {
        // Quote and escape value, but only if it's not numeric and not the empty string sequence
        if(!numeric && val != "''")
            val = QString("'%1'").arg(val.replace("'", "''"));

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
    m_lifeCounter++;

    if(m_db.isOpen()) {
        worker->cancel();
        worker->waitUntilIdle();
    }

    if(m_currentRowCount > 0)
    {
        beginRemoveRows(QModelIndex(), 0, m_currentRowCount - 1);
        endRemoveRows();
    }

    m_cache.clear();
    m_currentRowCount = 0;
    m_rowCountAvailable = RowCount::Unknown;
}

bool SqliteTableModel::isBinary(const QModelIndex& index) const
{
    QMutexLocker lock(&m_mutexDataCache);
    return nosync_isBinary(index);
}

bool SqliteTableModel::nosync_isBinary(const QModelIndex& index) const
{
    if(!m_cache.count(index.row()))
        return false;

    const auto & cached_row = m_cache.at(index.row());

    return !isTextOnly(cached_row.at(index.column()), m_encoding, true);
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
    // Do nothing if the value didn't change
    if(m_pseudoPk == pseudoPk)
        return;

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
    return !m_sTable.isEmpty() &&
            m_db.isOpen() &&
            ((m_db.getObjectByName(m_sTable) && m_db.getObjectByName(m_sTable)->type() == sqlb::Object::Types::Table) || !m_pseudoPk.isEmpty());
}

void SqliteTableModel::triggerCacheLoad (int row) const
{
    size_t row_begin = std::max(0, row - int(m_chunkSize) / 2);
    size_t row_end = row + m_chunkSize / 2;

    if(rowCountAvailable() == RowCount::Complete) {
        row_end = std::min(row_end, size_t(rowCount()));
    } else {
        // will be truncated by reader
    }

    // avoid re-fetching data
    QMutexLocker lk(&m_mutexDataCache);
    m_cache.smallestNonAvailableRange(row_begin, row_end);

    if(row_end != row_begin)
        worker->triggerFetch(m_lifeCounter, row_begin, row_end);
}

void SqliteTableModel::triggerCacheLoad (int row_begin, int row_end) const
{
    if(row_end == row_begin)
        return;

    triggerCacheLoad((row_begin + row_end) / 2);
}

bool SqliteTableModel::completeCache () const
{
    // Show progress dialog because fetching all data might take some time but only show
    // cancel button if we allow cancellation here. This isn't
    QProgressDialog progress(tr("Fetching data..."),
                             tr("Cancel"), 0, rowCount());
    progress.setWindowModality(Qt::ApplicationModal);
    progress.show();

    waitUntilIdle();

    // This loop fetches all data by loading it block by block into the cache
    for(int i=0;i<rowCount()+static_cast<int>(m_chunkSize)/2;i+=m_chunkSize)
    {
        progress.setValue(i);
        qApp->processEvents();
        if(progress.wasCanceled())
            return false;

        triggerCacheLoad(i);
        worker->waitUntilIdle();
    }

    return true;
}

bool SqliteTableModel::isCacheComplete () const
{
    if(readingData())
        return false;
    QMutexLocker lock(&m_mutexDataCache);
    return m_cache.numSet() == m_currentRowCount;
}

void SqliteTableModel::waitUntilIdle () const
{
    worker->waitUntilIdle();
}
