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
        size_t num_columns = m_headers.size();
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

    emit finishedRowCount();
}

void SqliteTableModel::reset()
{
    beginResetModel();
    clearCache();

    m_sQuery.clear();
    m_query.clear();
    m_headers.clear();
    m_vDataTypes.clear();
    m_mCondFormats.clear();

    endResetModel();
}

void SqliteTableModel::setChunkSize(size_t chunksize)
{
    m_chunkSize = chunksize;
}

void SqliteTableModel::setQuery(const sqlb::Query& query)
{
    // Unset all previous settings. When setting a table all information on the previously browsed data set is removed first.
    reset();

    // Save the query
    m_query = query;

    // The first column is the rowid column and therefore is always of type integer
    m_vDataTypes.push_back(SQLITE_INTEGER);

    // Get the data types of all other columns as well as the column names
    bool allOk = false;
    if(m_db.getObjectByName(query.table()) && m_db.getObjectByName(query.table())->type() == sqlb::Object::Types::Table)
    {
        sqlb::TablePtr t = m_db.getObjectByName<sqlb::Table>(query.table());
        if(t && t->fields.size()) // parsing was OK
        {
            sqlb::StringVector rowids = t->rowidColumns();
            m_query.setRowIdColumns(rowids);
            m_headers.push_back(sqlb::joinStringVector(rowids, ","));
            for(const auto& n : t->fieldNames())
                m_headers.push_back(n);

            // parse columns types
            static QStringList dataTypes = QStringList()
                    << "INTEGER"
                    << "REAL"
                    << "TEXT"
                    << "BLOB";
            for(const sqlb::Field& fld :  t->fields)
            {
                QString name = QString::fromStdString(fld.type()).toUpper();
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
        QString sColumnQuery = QString::fromUtf8("SELECT * FROM %1;").arg(QString::fromStdString(query.table().toString()));
        if(m_query.rowIdColumns().empty())
            m_query.setRowIdColumn("_rowid_");
        m_headers.push_back("_rowid_");
        auto columns = getColumns(nullptr, sColumnQuery, m_vDataTypes);
        m_headers.insert(m_headers.end(), columns.begin(), columns.end());
    }

    // Tell the query object about the column names
    m_query.setColumNames(m_headers);

    // Apply new query and update view
    buildQuery();
}

void SqliteTableModel::setQuery(const QString& sQuery, const QString& sCountQuery, bool dontClearHeaders)
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

    worker->setQuery(m_sQuery, sCountQuery);
    worker->triggerRowCountDetermination(m_lifeCounter);

    if(!dontClearHeaders)
    {
        auto columns = getColumns(worker->getDb(), sQuery, m_vDataTypes);
        m_headers.insert(m_headers.end(), columns.begin(), columns.end());
    }

    // now fetch the first entries
    triggerCacheLoad(static_cast<int>(m_chunkSize / 2) - 1);

    emit layoutChanged();
}

int SqliteTableModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(m_currentRowCount);
}

int SqliteTableModel::columnCount(const QModelIndex&) const
{
    return static_cast<int>(m_headers.size());
}

size_t SqliteTableModel::filterCount() const
{
    return m_query.where().size();
}

// Convert a number to string using the Unicode superscript characters
template<class T>
static QString toSuperScript(T number)
{
    QString superScript = QString::number(number);
    superScript.replace("0", "⁰");
    superScript.replace("1", "¹");
    superScript.replace("2", "²");
    superScript.replace("3", "³");
    superScript.replace("4", "⁴");
    superScript.replace("5", "⁵");
    superScript.replace("6", "⁶");
    superScript.replace("7", "⁷");
    superScript.replace("8", "⁸");
    superScript.replace("9", "⁹");
    return superScript;
}

QVariant SqliteTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        // if we have a VIRTUAL table the model will not be valid, with no header data
        if(static_cast<size_t>(section) < m_headers.size()) {
            QString sortIndicator;
            for(size_t i = 0; i < m_query.orderBy().size(); i++) {
                const sqlb::SortedColumn sortedColumn = m_query.orderBy()[i];
                // Append sort indicator with direction and ordinal number in superscript style
                if (sortedColumn.column == static_cast<size_t>(section)) {
                    sortIndicator = sortedColumn.direction == sqlb::Ascending ? " ▾" : " ▴";
                    sortIndicator.append(toSuperScript(i+1));
                    break;
                }
            }
            return QString::fromStdString(m_headers.at(static_cast<size_t>(section))) + sortIndicator;
        }
        return QString("%1").arg(section + 1);
    }
    else
        return QString("%1").arg(section + 1);
}

QColor SqliteTableModel::getMatchingCondFormatColor(int column, const QString& value, int role) const
{
    bool isNumber;
    value.toFloat(&isNumber);
    QString sql;
    // For each conditional format for this column,
    // if the condition matches the current data, return the associated colour.
    for (const CondFormat& eachCondFormat : m_mCondFormats.value(column)) {
        if (isNumber && !eachCondFormat.sqlCondition().contains("'"))
            sql = QString("SELECT %1 %2").arg(value, eachCondFormat.sqlCondition());
        else
            sql = QString("SELECT '%1' %2").arg(value, eachCondFormat.sqlCondition());

        // Empty filter means: apply format to any row.
        // Query the DB for the condition, waiting in case there is a loading in progress.
        if (eachCondFormat.filter().isEmpty() || m_db.querySingleValueFromDb(sql, false, DBBrowserDB::Wait) == "1")
            return role == Qt::ForegroundRole ? eachCondFormat.foregroundColor() : eachCondFormat.backgroundColor();
    }
    return QColor();
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
        else if (m_mCondFormats.contains(index.column())) {
            QString value = cached_row->at(index.column());
            // Unlock before querying from DB
            lock.unlock();
            QColor condFormatColor = getMatchingCondFormatColor(index.column(), value, role);
            if (condFormatColor.isValid())
                return condFormatColor;
            }
        // Regular case (not null, not binary and no matching conditional format)
        return QColor(Settings::getValue("databrowser", "reg_fg_colour").toString());
    } else if (role == Qt::BackgroundRole) {
        if(!row_available)
            return QColor(255, 200, 200);
        if(cached_row->at(index.column()).isNull())
            return QColor(Settings::getValue("databrowser", "null_bg_colour").toString());
        else if (nosync_isBinary(index))
            return QColor(Settings::getValue("databrowser", "bin_bg_colour").toString());
        else if (m_mCondFormats.contains(index.column())) {
            QString value = cached_row->at(index.column());
            // Unlock before querying from DB
            lock.unlock();
            QColor condFormatColor = getMatchingCondFormatColor(index.column(), value, role);
            if (condFormatColor.isValid())
                return condFormatColor;
        }
        // Regular case (not null, not binary and no matching conditional format)
        return QColor(Settings::getValue("databrowser", "reg_bg_colour").toString());
    } else if(role == Qt::ToolTipRole) {
        sqlb::ForeignKeyClause fk = getForeignKeyClause(index.column()-1);
        if(fk.isSet())
          return tr("References %1(%2)\nHold %3Shift and click to jump there")
                  .arg(QString::fromStdString(fk.table()))
                  .arg(QString::fromStdString(sqlb::joinStringVector(fk.columns(), ",")))
                  .arg(QKeySequence(Qt::CTRL).toString(QKeySequence::NativeText));
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
    if(m_query.table().isEmpty())
        return empty_foreign_key_clause;

    // Retrieve database object and check if it is a table. If it isn't stop here and don't return a foreign
    // key. This happens for views which don't have foreign keys (though we might want to think about how we
    // can check for foreign keys in the underlying tables for some purposes like tool tips).
    sqlb::ObjectPtr obj = m_db.getObjectByName(m_query.table());
    if(obj->type() != sqlb::Object::Table)
        return empty_foreign_key_clause;

    // Convert object to a table and check if the column number is in the valid range
    sqlb::TablePtr tbl = std::dynamic_pointer_cast<sqlb::Table>(obj);
    if(tbl && tbl->name().size() && (column >= 0 && column < static_cast<int>(tbl->fields.size())))
    {
        // Note that the rowid column has number -1 here, it can safely be excluded since there will never be a
        // foreign key on that column.

        sqlb::ConstraintPtr ptr = tbl->constraint({tbl->fields.at(column).name()}, sqlb::Constraint::ForeignKeyConstraintType);
        if(ptr)
            return *(std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(ptr));
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
            sqlb::TablePtr table = m_db.getObjectByName<sqlb::Table>(m_query.table());
            if(table)
            {
                auto field = sqlb::findField(table, m_headers.at(index.column()));
                if(contains(table->primaryKey(), field->name()) && field->isInteger())
                    newValue = "0";
            }
        }

        // Don't do anything if the data hasn't changed
        // To differentiate NULL and empty byte arrays, we also compare the NULL flag
        if(oldValue == newValue && oldValue.isNull() == newValue.isNull())
            return true;

        if(m_db.updateRecord(m_query.table(), m_headers.at(index.column()), cached_row.at(0), newValue, isBlob, m_query.rowIdColumns()))
        {
            cached_row[index.column()] = newValue;
            if(m_headers.at(index.column()) == sqlb::joinStringVector(m_query.rowIdColumns(), ",")) {
                cached_row[0] =  newValue;
                const QModelIndex& rowidIndex = index.sibling(index.row(), 0);
                lock.unlock();
                emit dataChanged(rowidIndex, rowidIndex);
            } else {
                lock.unlock();
            }
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
    if(m_query.selectedColumns().size())
    {
        if(index.column() > 0)
            custom_display_format = QString::fromStdString(m_query.selectedColumns().at(index.column()-1).selector) != sqlb::escapeIdentifier(headerData(index.column(), Qt::Horizontal).toString());
    }

    if(!isBinary(index) && !custom_display_format)
        ret |= Qt::ItemIsEditable;
    return ret;
}

void SqliteTableModel::sort(int column, Qt::SortOrder order)
{
    // Construct a sort order list from this item and forward it to the function to sort by lists
    std::vector<sqlb::SortedColumn> list;
    list.emplace_back(column, order == Qt::AscendingOrder ? sqlb::Ascending : sqlb::Descending);
    sort(list);
}

void SqliteTableModel::sort(const std::vector<sqlb::SortedColumn>& columns)
{
    // Don't do anything when the sort order hasn't changed
    if(m_query.orderBy() == columns)
        return;

    // Save sort order
    m_query.orderBy() = columns;

    // Set the new query (but only if a table has already been set
    if(!m_query.table().isEmpty())
        buildQuery();
}

SqliteTableModel::Row SqliteTableModel::makeDefaultCacheEntry () const
{
    Row blank_data;

    for(size_t i=0; i < m_headers.size(); ++i)
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
        QString rowid = m_db.addRecord(m_query.table());
        if(rowid.isNull())
        {
            return false;
        }
        tempList.push_back(blank_data);
        tempList.back()[0] = rowid.toUtf8();

        // update column with default values
        Row rowdata;
        if(m_db.getRow(m_query.table(), rowid, rowdata))
        {
            for(size_t j=1; j < m_headers.size(); ++j)
            {
                tempList.back()[j] = rowdata[j - 1];
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

    QStringList rowids;
    for(int i=count-1;i>=0;i--)
    {
        if(m_cache.count(row+i)) {
            rowids.append(m_cache.at(row + i).at(0));
        }
    }

    bool ok = m_db.deleteRecords(m_query.table(), rowids, m_query.rowIdColumns());

    if (ok) {
        beginRemoveRows(parent, row, row + count - 1);

        for(int i=count-1;i>=0;i--)
        {
            m_cache.erase(row + i);
            m_currentRowCount--;
        }

        endRemoveRows();
    }
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

    sqlb::TablePtr t = m_db.getObjectByName<sqlb::Table>(m_query.table());

    sqlb::StringVector pk = t->primaryKey();
    for (size_t col = 0; col < t->fields.size(); ++col) {
        if(!contains(pk, t->fields.at(col).name())) {
            if (!firstEditedColumn)
                firstEditedColumn = col + 1;

            QVariant value = data(index(old_row, col + 1), Qt::EditRole);
            setData(index(new_row, col + 1), value);
        }
    }

    return index(new_row, firstEditedColumn);
}

void SqliteTableModel::buildQuery()
{
    setQuery(QString::fromStdString(m_query.buildQuery(true)), QString::fromStdString(m_query.buildCountQuery()), true);
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

std::vector<std::string> SqliteTableModel::getColumns(std::shared_ptr<sqlite3> pDb, const QString& sQuery, std::vector<int>& fieldsTypes)
{
    if(!pDb)
        pDb = m_db.get(tr("retrieving list of columns"));

    sqlite3_stmt* stmt;
    QByteArray utf8Query = sQuery.toUtf8();
    int status = sqlite3_prepare_v2(pDb.get(), utf8Query, utf8Query.size(), &stmt, nullptr);
    std::vector<std::string> listColumns;
    if(SQLITE_OK == status)
    {
        sqlite3_step(stmt);
        int columns = sqlite3_data_count(stmt);
        for(int i = 0; i < columns; ++i)
        {
            listColumns.push_back(sqlite3_column_name(stmt, i));
            fieldsTypes.push_back(sqlite3_column_type(stmt, i));
        }
    }
    sqlite3_finalize(stmt);

    return listColumns;
}

void SqliteTableModel::addCondFormat(int column, const CondFormat& condFormat)
{
    m_mCondFormats[column].push_back(condFormat);
    emit layoutChanged();
}

void SqliteTableModel::setCondFormats(int column, const std::vector<CondFormat>& condFormats)
{
    m_mCondFormats[column] = condFormats;
    emit layoutChanged();
}

void SqliteTableModel::updateFilter(int column, const QString& value)
{
    QString whereClause = CondFormat::filterToSqlCondition(value, m_encoding);

    // If the value was set to an empty string remove any filter for this column. Otherwise insert a new filter rule or replace the old one if there is already one
    if(whereClause.isEmpty())
        m_query.where().erase(column);
    else
        m_query.where()[column] = whereClause.toStdString();

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
    return encodeString(str, m_encoding);
}

QByteArray SqliteTableModel::decode(const QByteArray& str) const
{
    return decodeString(str, m_encoding);
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

void SqliteTableModel::setPseudoPk(std::vector<std::string> pseudoPk)
{
    if(pseudoPk.empty())
        pseudoPk.emplace_back("_rowid_");

    // Do nothing if the value didn't change
    if(m_query.rowIdColumns() == pseudoPk)
        return;

    m_query.setRowIdColumns(pseudoPk);
    if(m_headers.size())
        m_headers[0] = sqlb::joinStringVector(pseudoPk, ",");

    buildQuery();
}

bool SqliteTableModel::hasPseudoPk() const
{
    return m_query.hasCustomRowIdColumn();
}

bool SqliteTableModel::isEditable() const
{
    return !m_query.table().isEmpty() &&
            m_db.isOpen() &&
            ((m_db.getObjectByName(m_query.table()) && m_db.getObjectByName(m_query.table())->type() == sqlb::Object::Types::Table) || m_query.hasCustomRowIdColumn());
}

void SqliteTableModel::triggerCacheLoad (int row) const
{
    int halfChunk = static_cast<int>( m_chunkSize / 2);
    size_t row_begin = static_cast<std::size_t>(std::max(0, row - halfChunk));
    size_t row_end = static_cast<std::size_t>(row + halfChunk);

    if(rowCountAvailable() == RowCount::Complete)
    {
        row_end = std::min(row_end, static_cast<std::size_t>(rowCount()));
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
    for(int i = 0; i < (rowCount() + static_cast<int>( m_chunkSize / 2)); i += static_cast<int>(m_chunkSize))
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
