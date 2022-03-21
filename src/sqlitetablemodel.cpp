#include "sqlitetablemodel.h"
#include "sqlitedb.h"
#include "sqlite.h"
#include "Settings.h"
#include "Data.h"
#include "CondFormat.h"
#include "RowLoader.h"

#include <QMessageBox>
#include <QApplication>
#include <QTextCodec>
#include <QMimeData>
#include <QFile>
#include <QUrl>
#include <QtConcurrent/QtConcurrentRun>
#include <QProgressDialog>
#include <QRegularExpression>
#include <QPushButton>

#include <cassert>

SqliteTableModel::SqliteTableModel(DBBrowserDB& db, QObject* parent, const QString& encoding, bool force_wait)
    : QAbstractTableModel(parent)
    , m_db(db)
    , m_lifeCounter(0)
    , m_currentRowCount(0)
    , m_encoding(encoding)
{
    // Load initial settings first
    reloadSettings();

    worker = new RowLoader(
        [this, force_wait](){ return m_db.get(tr("reading rows"), force_wait); },
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
        beginInsertRows(QModelIndex(), static_cast<int>(old_row_count), static_cast<int>(new_row_count - 1));
        m_currentRowCount = new_row_count;
        endInsertRows();
    }

    if(fetched_row_end != fetched_row_begin)
    {
        // TODO optimize
        size_t num_columns = m_headers.size();
        emit dataChanged(createIndex(static_cast<int>(fetched_row_begin), 0), createIndex(static_cast<int>(fetched_row_end) - 1, static_cast<int>(num_columns) - 1));
    }

    if(m_rowCountAvailable != RowCount::Complete)
        m_rowCountAvailable = RowCount::Partial;

    emit finishedFetch(static_cast<int>(fetched_row_begin), static_cast<int>(fetched_row_end));
}

void SqliteTableModel::handleRowCountComplete (int life_id, int num_rows)
{
    if(life_id < m_lifeCounter)
        return;

    m_rowCountAvailable = RowCount::Complete;
    handleFinishedFetch(life_id, static_cast<unsigned int>(num_rows), static_cast<unsigned int>(num_rows));

    emit finishedRowCount();
}

void SqliteTableModel::reset()
{
    beginResetModel();

    clearCache();
    m_sQuery.clear();
    m_query.clear();
    m_table_of_query.reset();
    m_headers.clear();
    m_vDataTypes.clear();
    m_mCondFormats.clear();
    m_mRowIdFormats.clear();

    endResetModel();
}

void SqliteTableModel::setQuery(const sqlb::Query& query)
{
    // Unset all previous settings. When setting a table all information on the previously browsed data set is removed first.
    reset();

    // Save the query
    m_query = query;

    // Set the row id columns
    m_table_of_query = m_db.getTableByName(query.table());
    if(!m_table_of_query->isView())
    {
        // It is a table
        m_query.setRowIdColumns(m_table_of_query->rowidColumns());
    } else {
        // It is a view
        if(m_query.rowIdColumns().empty())
            m_query.setRowIdColumn("_rowid_");
    }
    m_vDataTypes.emplace_back(SQLITE_INTEGER);  // TODO This is not necessarily true for tables without ROWID or with multiple PKs
    m_headers.push_back(sqlb::joinStringVector(m_query.rowIdColumns(), ","));

    // Store field names and affinity data types
    for(const auto& fld : m_table_of_query->fields)
    {
        m_headers.push_back(fld.name());
        m_vDataTypes.push_back(fld.affinity());
    }

    // Tell the query object about the column names
    m_query.setColumnNames(m_headers);

    // Apply new query and update view
    updateAndRunQuery();
}

void SqliteTableModel::setQuery(const QString& sQuery)
{
    // Reset
    reset();

    m_sQuery = sQuery.trimmed();
    removeCommentsFromQuery(m_sQuery);

    getColumnNames(sQuery.toStdString());

    worker->setQuery(m_sQuery, QString());

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
    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::FontRole)
        return QVariant();

    if (orientation == Qt::Vertical && role == Qt::FontRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        size_t column = static_cast<size_t>(section);
        // if we have a VIRTUAL table the model will not be valid, with no header data
        if(column < m_headers.size()) {
            const std::string plainHeader = m_headers.at(static_cast<size_t>(section));
            // In the edit role, return a plain column name, but in the display role, add the sort indicator.
            switch (role) {
            case Qt::EditRole:
                return QString::fromStdString(plainHeader);
            case Qt::FontRole: {
                bool is_pk = false;
                bool is_fk = getForeignKeyClause(column-1) != nullptr;

                if (contains(m_query.rowIdColumns(), m_headers.at(column))) {
                    is_pk = true;
                } else if (m_table_of_query) {
                    auto field = sqlb::findField(m_table_of_query, m_headers.at(column));
                    const auto pk = m_table_of_query->primaryKeyColumns();
                    is_pk = field != m_table_of_query->fields.end() && contains(pk, field->name());
                }

                QFont font;
                font.setUnderline(is_pk);
                font.setItalic(is_fk);
                return font;
            }
            default:
                QString sortIndicator;
                for(size_t i = 0; i < m_query.orderBy().size(); i++) {
                    const sqlb::OrderBy sortedColumn = m_query.orderBy()[i];
                    // Append sort indicator with direction and ordinal number in superscript style
                    if (sortedColumn.expr == plainHeader) {
                        sortIndicator = sortedColumn.direction == sqlb::OrderBy::Ascending ? " ▴" : " ▾";
                        if(m_query.orderBy().size() > 1)
                            sortIndicator.append(toSuperScript(i+1));
                        break;
                    }
                }
                return QString::fromStdString(plainHeader) + sortIndicator;
            }
        }
        return QString::number(section + 1);
    }
    else
        return QString::number(section + 1);
}

QVariant SqliteTableModel::getMatchingCondFormat(const std::map<size_t, std::vector<CondFormat>>& mCondFormats, size_t column, const QString& value, int role) const
{
    if (!mCondFormats.count(column))
        return QVariant();

    bool isNumber;
    value.toDouble(&isNumber);
    std::string sql;

    // For each conditional format for this column,
    // if the condition matches the current data, return the associated format.
    for (const CondFormat& eachCondFormat : mCondFormats.at(column)) {
        if (isNumber && !contains(eachCondFormat.sqlCondition(), '\''))
            sql = "SELECT " + value.toStdString() + " " + eachCondFormat.sqlCondition();
        else
            sql = "SELECT " + sqlb::escapeString(value.toStdString()) + " " + eachCondFormat.sqlCondition();

        // Empty filter means: apply format to any row.
        // Query the DB for the condition, waiting in case there is a loading in progress.
        if (eachCondFormat.filter().isEmpty() || m_db.querySingleValueFromDb(sql, false, DBBrowserDB::Wait) == "1")
            switch (role) {
              case Qt::ForegroundRole:
                return eachCondFormat.foregroundColor();
            case Qt::BackgroundRole:
                return eachCondFormat.backgroundColor();
            case Qt::FontRole:
                return eachCondFormat.font();
            case Qt::TextAlignmentRole:
                return static_cast<int>(eachCondFormat.alignmentFlag() | Qt::AlignVCenter);
            }
    }
    return QVariant();
}

QVariant SqliteTableModel::getMatchingCondFormat(size_t row, size_t column, const QString& value, int role) const
{
    QVariant format;
    // Check first for a row-id format and when there is none, for a conditional format.
    if (m_mRowIdFormats.count(column))
    {
        std::unique_lock<std::mutex> lock(m_mutexDataCache);
        const bool row_available = m_cache.count(row);
        const QByteArray blank_data("");
        const QByteArray& row_id_data = row_available ? m_cache.at(row).at(0) : blank_data;
        lock.unlock();

        format = getMatchingCondFormat(m_mRowIdFormats, column, row_id_data, role);
        if (format.isValid())
            return format;
    }
    if (m_mCondFormats.count(column))
        return getMatchingCondFormat(m_mCondFormats, column, value, role);
    else
        return QVariant();
}

QVariant SqliteTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= rowCount())
        return QVariant();

    std::unique_lock<std::mutex> lock(m_mutexDataCache);

    const size_t row = static_cast<size_t>(index.row());
    const size_t column = static_cast<size_t>(index.column());

    const bool row_available = m_cache.count(row);
    const QByteArray blank_data("");
    const QByteArray& data = row_available ? m_cache.at(row).at(column) : blank_data;

    if(role == Qt::DisplayRole)
    {
        if(!row_available)
            return tr("loading...");
        if(data.isNull())
        {
            return m_nullText;
        } else if(isBinary(data)) {
            return m_blobText;
        } else {
            if (data.length() > m_symbolLimit) {
                // Add "..." to the end of truncated strings
                return decode(data.left(m_symbolLimit).append(" ..."));
            } else {
                return decode(data);
            }
        }
    } else if(role == Qt::EditRole) {
        if(!row_available)
            return QVariant();
        return decode(data);
    } else if(role == Qt::FontRole) {
        QFont font = m_font;
        if(!row_available || data.isNull() || isBinary(data))
            font.setItalic(true);
        else {
            // Unlock before querying from DB
            lock.unlock();
            QVariant condFormatFont = getMatchingCondFormat(row, column, data, role);
            if (condFormatFont.isValid())
                return condFormatFont;
        }
        return font;
    } else if(role == Qt::ForegroundRole) {
        if(!row_available)
            return QColor(100, 100, 100);
        if(data.isNull())
            return m_nullFgColour;
        else if (isBinary(data))
            return m_binFgColour;
        else {
            // Unlock before querying from DB
            lock.unlock();
            QVariant condFormatColor = getMatchingCondFormat(row, column, data, role);
            if (condFormatColor.isValid())
                return condFormatColor;
            }
        // Regular case (not null, not binary and no matching conditional format)
        return m_regFgColour;
    } else if (role == Qt::BackgroundRole) {
        if(!row_available)
            return QColor(255, 200, 200);
        if(data.isNull())
            return m_nullBgColour;
        else if (isBinary(data))
            return m_binBgColour;
        else {
            // Unlock before querying from DB
            lock.unlock();
            QVariant condFormatColor = getMatchingCondFormat(row, column, data, role);
            if (condFormatColor.isValid())
                return condFormatColor;
        }
        // Regular case (not null, not binary and no matching conditional format)
        return m_regBgColour;
    } else if(role == Qt::ToolTipRole) {
        auto fk = getForeignKeyClause(column-1);
        if(fk)
            return tr("References %1(%2)\nHold %3Shift and click to jump there").arg(
                    QString::fromStdString(fk->table()),
                    QString::fromStdString(sqlb::joinStringVector(fk->columns(), ",")),
                    QKeySequence(Qt::CTRL).toString(QKeySequence::NativeText));
    } else if (role == Qt::TextAlignmentRole) {
        // Align horizontally according to conditional format or default (left for text and right for numbers)
        // Align vertically to the center, which displays better.
        lock.unlock();
        QVariant condFormat = getMatchingCondFormat(row, column, data, role);
        if (condFormat.isValid())
            return condFormat;
        bool isNumber = m_vDataTypes.at(column) == SQLITE_INTEGER || m_vDataTypes.at(column) == SQLITE_FLOAT;
        return static_cast<int>((isNumber ? Qt::AlignRight : Qt::AlignLeft) | Qt::AlignVCenter);
    } else if(role == Qt::DecorationRole) {
        if(!row_available)
            return QVariant();

        if(m_imagePreviewEnabled && !isImageData(data).isNull())
        {
            QImage img;
            if(img.loadFromData(data))
                return QPixmap::fromImage(img);
        }
    }


    return QVariant();
}

std::shared_ptr<sqlb::ForeignKeyClause> SqliteTableModel::getForeignKeyClause(size_t column) const
{
    // No foreign keys when not browsing a table. This usually happens when executing custom SQL statements
    // and browsing the result set instead of browsing an entire table.
    if(m_query.table().isEmpty())
        return nullptr;

    // Check if database object is a table. If it isn't stop here and don't return a foreign key.
    // This happens for views which don't have foreign keys (though we might want to think about
    // how we can check for foreign keys in the underlying tables for some purposes like tool tips).
    // If it is a table, heck if the column number is in the valid range.
    if(m_table_of_query && m_table_of_query->name().size() && column < m_table_of_query->fields.size())
    {
        // Note that the rowid column has number -1 here, it can safely be excluded since there will never be a
        // foreign key on that column.

        return m_table_of_query->foreignKey({m_table_of_query->fields.at(column).name()});
    }

    return nullptr;
}

bool SqliteTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    // Don't even try setting any data if we're not browsing a table, i.e. the model data comes from a custom query
    if(!isEditable(index))
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
        std::unique_lock<std::mutex> lock(m_mutexDataCache);

        auto & cached_row = m_cache.at(static_cast<size_t>(index.row()));
        const size_t column = static_cast<size_t>(index.column());

        QByteArray newValue = encode(value.toByteArray());
        QByteArray oldValue = cached_row.at(column);

        // Special handling for integer columns: instead of setting an integer column to an empty string, set it to '0' when it is also
        // used in a primary key. Otherwise SQLite will always output an 'datatype mismatch' error.
        if(newValue == "" && !newValue.isNull())
        {
            if(m_table_of_query)
            {
                auto field = sqlb::findField(m_table_of_query, m_headers.at(column));
                const auto pk = m_table_of_query->primaryKeyColumns();
                if(contains(pk, field->name()) && field->isInteger())
                    newValue = "0";
            }
        }

        // Don't do anything if the data hasn't changed
        // To differentiate NULL and empty byte arrays, we also compare the NULL flag
        if(oldValue == newValue && oldValue.isNull() == newValue.isNull())
            return true;

        // Determine type. If the BLOB flag is set, it's always BLOB. If the affinity data type of the modified column is something numeric,
        // we check if the new value is also numeric. In that case we can safely set the data type to INTEGER or FLOAT. In all other cases we
        // default to TEXT.
        int type = SQLITE_TEXT;
        if(isBlob)
        {
            type = SQLITE_BLOB;
        } else if(m_vDataTypes.at(column) == SQLITE_INTEGER) {
            bool ok;
            newValue.toLongLong(&ok);
            if(ok)
                type = SQLITE_INTEGER;
        } else if(m_vDataTypes.at(column) == SQLITE_FLOAT) {
            bool ok;
            newValue.toDouble(&ok);
            if(ok)
                type = SQLITE_FLOAT;
        }

        if(m_db.updateRecord(m_query.table(), m_headers.at(column), cached_row.at(0), newValue, type, m_query.rowIdColumns()))
        {
            cached_row[column] = newValue;

            // After updating the value itself in the cache, we need to check if we need to update the rowid too.
            if(contains(m_query.rowIdColumns(), m_headers.at(column)))
            {
                // When the cached rowid column needs to be updated as well, we need to distinguish between single-column and multi-column primary keys.
                // For the former ones, we can just overwrite the existing value with the new value.
                // For the latter ones, we need to make a new JSON object of the values of all primary key columns, not just the updated one.
                if(m_query.rowIdColumns().size() == 1)
                {
                    cached_row[0] = newValue;
                } else {
                    assert(m_headers.size() == cached_row.size());
                    QByteArray output;
                    for(size_t i=0;i<m_query.rowIdColumns().size();i++)
                    {
                        auto it = std::find(m_headers.begin()+1, m_headers.end(), m_query.rowIdColumns().at(i));    // +1 in order to omit the rowid column itself
                        auto v = cached_row[static_cast<size_t>(std::distance(m_headers.begin(), it))];
                        output += QByteArray::number(v.size()) + ":" + v;
                    }
                    cached_row[0] = output;
                }
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
            custom_display_format = m_query.selectedColumns().at(static_cast<size_t>(index.column())-1).selector != m_query.selectedColumns().at(static_cast<size_t>(index.column())-1).original_column;
    }

    if(!isBinary(index) && !custom_display_format && isEditable(index))
        ret |= Qt::ItemIsEditable;
    return ret;
}

void SqliteTableModel::sort(int column, Qt::SortOrder order)
{
    // Construct a sort order list from this item and forward it to the function to sort by lists
    std::vector<sqlb::OrderBy> list;
    list.emplace_back(m_headers.at(static_cast<size_t>(column)), order == Qt::AscendingOrder ? sqlb::OrderBy::Ascending : sqlb::OrderBy::Descending);
    sort(list);
}

void SqliteTableModel::sort(const std::vector<sqlb::OrderBy>& columns)
{
    // Don't do anything when the sort order hasn't changed
    if(m_query.orderBy() == columns)
        return;

    // Save sort order
    m_query.orderBy() = columns;

    // Set the new query (but only if a table has already been set
    if(!m_query.table().isEmpty())
        updateAndRunQuery();
}

SqliteTableModel::Row SqliteTableModel::makeDefaultCacheEntry () const
{
    Row blank_data;

    for(size_t i=0; i < m_headers.size(); ++i)
        blank_data.emplace_back("");

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
        tempList.emplace_back(blank_data);
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
    for(size_t i = 0; i < tempList.size(); ++i)
    {
        m_cache.insert(i + static_cast<size_t>(row), std::move(tempList.at(i)));
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

    std::vector<QByteArray> rowids;
    for(int i=count-1;i>=0;i--)
    {
        if(m_cache.count(static_cast<size_t>(row+i))) {
            rowids.push_back(m_cache.at(static_cast<size_t>(row + i)).at(0));
        }
    }

    bool ok = m_db.deleteRecords(m_query.table(), rowids, m_query.rowIdColumns());

    if (ok) {
        beginRemoveRows(parent, row, row + count - 1);

        for(int i=count-1;i>=0;i--)
        {
            m_cache.erase(static_cast<size_t>(row + i));
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

    size_t firstEditedColumn = 0;
    int new_row = rowCount() - 1;

    const auto pk = m_table_of_query->primaryKeyColumns();
    for (size_t col = 0; col < m_table_of_query->fields.size(); ++col) {
        if(pk.empty() || !contains(pk, m_table_of_query->fields.at(col).name())) {
            if (!firstEditedColumn)
                firstEditedColumn = col + 1;

            QVariant value = data(index(old_row, static_cast<int>(col + 1)), Qt::EditRole);
            setData(index(new_row, static_cast<int>(col + 1)), value);
        }
    }

    return index(new_row, static_cast<int>(firstEditedColumn));
}

void SqliteTableModel::updateAndRunQuery()
{
    clearCache();

    // Update the query
    m_sQuery = QString::fromStdString(m_query.buildQuery(true));
    QString sCountQuery = QString::fromStdString(m_query.buildCountQuery());
    worker->setQuery(m_sQuery, sCountQuery);

    // now fetch the first entries
    triggerCacheLoad(static_cast<int>(m_chunkSize / 2) - 1);

    emit layoutChanged();
}

void SqliteTableModel::getColumnNames(const std::string& sQuery)
{
    auto pDb = m_db.get(tr("retrieving list of columns"));

    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(pDb.get(), sQuery.c_str(), static_cast<int>(sQuery.size()), &stmt, nullptr) == SQLITE_OK)
    {
        int columns = sqlite3_column_count(stmt);
        for(int i = 0; i < columns; ++i)
        {
            m_headers.push_back(sqlite3_column_name(stmt, i));
            m_vDataTypes.push_back(sqlite3_column_type(stmt, i));
        }
    }
    sqlite3_finalize(stmt);
}

static void addCondFormatToMap(std::map<size_t, std::vector<CondFormat>>& mCondFormats, size_t column, const CondFormat& condFormat)
{
    // If the condition is already present in the vector, update that entry and respect the order, since two entries with the same
    // condition do not make sense.
    auto it = std::find_if(mCondFormats[column].begin(), mCondFormats[column].end(), [condFormat](const CondFormat& format) {
            return format.sqlCondition() == condFormat.sqlCondition();
        });
    // Replace cond-format if present. push it back if it's a conditionless format (apply to every cell in column) or insert
    // as first element otherwise.
    if(it != mCondFormats[column].end()) {
        *it = condFormat;
    } else if (condFormat.filter().isEmpty())
        mCondFormats[column].push_back(condFormat);
    else
        mCondFormats[column].insert(mCondFormats[column].begin(), condFormat);
}

void SqliteTableModel::addCondFormat(const bool isRowIdFormat, size_t column, const CondFormat& condFormat)
{
    if(isRowIdFormat)
        addCondFormatToMap(m_mRowIdFormats, column, condFormat);
    else
        addCondFormatToMap(m_mCondFormats, column, condFormat);
    emit layoutChanged();
}

void SqliteTableModel::setCondFormats(const bool isRowIdFormat, size_t column, const std::vector<CondFormat>& condFormats)
{
    if(isRowIdFormat)
        m_mRowIdFormats[column] = condFormats;
    else
        m_mCondFormats[column] = condFormats;
    emit layoutChanged();
}

void SqliteTableModel::updateFilter(const std::string& column, const QString& value)
{
    std::string whereClause = CondFormat::filterToSqlCondition(value, m_encoding);

    // If the value was set to an empty string remove any filter for this column. Otherwise insert a new filter rule or replace the old one if there is already one
       if(whereClause.empty())
        m_query.where().erase(column);
    else
        m_query.where()[column] = whereClause;

    // Build the new query
    updateAndRunQuery();
}

void SqliteTableModel::updateGlobalFilter(const std::vector<QString>& values)
{
    std::vector<std::string> filters;
    for(auto& v : values)
        filters.push_back(CondFormat::filterToSqlCondition(v, m_encoding));
    m_query.setGlobalWhere(filters);

    // Build the new query
    updateAndRunQuery();
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
        beginRemoveRows(QModelIndex(), 0, static_cast<int>(m_currentRowCount - 1));
        endRemoveRows();
    }

    m_cache.clear();

    m_currentRowCount = 0;
    m_rowCountAvailable = RowCount::Unknown;
}

bool SqliteTableModel::isBinary(const QModelIndex& index) const
{
    std::lock_guard<std::mutex> lock(m_mutexDataCache);

    const size_t row = static_cast<size_t>(index.row());
    if(!m_cache.count(row))
        return false;

    const auto & cached_row = m_cache.at(row);
    return isBinary(cached_row.at(static_cast<size_t>(index.column())));
}

bool SqliteTableModel::isBinary(const QByteArray& data) const
{
    return !isTextOnly(data, m_encoding, true);
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
    if(!m_table_of_query->isView())
        return;

    if(pseudoPk.empty())
        pseudoPk.emplace_back("_rowid_");

    // Do nothing if the value didn't change
    if(m_query.rowIdColumns() == pseudoPk)
        return;

    m_query.setRowIdColumns(pseudoPk);
    if(m_headers.size())
        m_headers[0] = sqlb::joinStringVector(pseudoPk, ",");

    updateAndRunQuery();
}

bool SqliteTableModel::hasPseudoPk() const
{
    return m_query.hasCustomRowIdColumn();
}

bool SqliteTableModel::isEditable(const QModelIndex& index) const
{
    if(m_query.table().isEmpty())
        return false;
    if(!m_db.isOpen())
        return false;
    if((!m_table_of_query || m_table_of_query->isView()) && !m_query.hasCustomRowIdColumn())
        return false;

    // Extra check when the index parameter is set and pointing to a generated column in a table
    if(index.isValid() && m_table_of_query)
    {
        const auto field = sqlb::findField(m_table_of_query, m_headers.at(static_cast<size_t>(index.column())));
        if(field != m_table_of_query->fields.cend() && field->generated())
            return false;
    }

    return true;
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
    std::lock_guard<std::mutex> lk(m_mutexDataCache);
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

    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    // This is to prevent distracted cancellation of the fetching and avoid the
    // Snap-To Windows optional feature.
    cancelButton->setDefault(false);
    cancelButton->setAutoDefault(false);
    progress.setCancelButton(cancelButton);

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
    std::lock_guard<std::mutex> lock(m_mutexDataCache);
    return m_cache.numSet() == m_currentRowCount;
}

void SqliteTableModel::waitUntilIdle () const
{
    worker->waitUntilIdle();
}

QModelIndex SqliteTableModel::nextMatch(const QModelIndex& start, const std::vector<int>& column_list, const QString& value, Qt::MatchFlags flags, bool reverse, bool dont_skip_to_next_field) const
{
    // Extract flags
    bool whole_cell = !(flags & Qt::MatchContains);
    bool regex = flags & Qt::MatchRegExp;
    Qt::CaseSensitivity case_sensitive = ((flags & Qt::MatchCaseSensitive) ? Qt::CaseSensitive : Qt::CaseInsensitive);
    bool wrap = flags & Qt::MatchWrap;
    int increment = (reverse ? -1 : 1);

    // Prepare the regular expression for regex mode
    QRegularExpression reg_exp;
    if(regex)
    {
        reg_exp = QRegularExpression(value, (case_sensitive ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption));
        if(!reg_exp.isValid())
            return QModelIndex();

        if(whole_cell)
        {
#if QT_VERSION < QT_VERSION_CHECK(5, 12, 0)
        reg_exp.setPattern("\\A(" + reg_exp.pattern() + ")\\Z");
#else
        reg_exp.setPattern(QRegularExpression::anchoredPattern(reg_exp.pattern()));
#endif
        }
    }

    // Wait until the row count is there
    waitUntilIdle();

    // Stop right away if there is no data in the table
    if(rowCount() == 0)
        return QModelIndex();

    // Make sure the start position starts in a column from the list of columns to search in
    QModelIndex pos = start;
    if(std::find(column_list.begin(), column_list.end(), pos.column()) == column_list.end())
    {
        // If for some weird reason the start index is not in the column list, we simply use the first column of the column list instead
        pos = pos.sibling(pos.row(), reverse ? column_list.back() : column_list.front());
    }

    // Get the last cell to search in. If wrapping is enabled, we search until we hit the start cell again. If wrapping is not enabled, we start at the last
    // cell of the table.
    QModelIndex end = (wrap ? pos : index(rowCount(), column_list.back()));

    // Loop through all cells for the search
    while(true)
    {
        // Go to the next cell and skip all columns in between which we do not care about. This is done as the first step in order
        // to skip the start index when matching the first cell is disabled.
        if(dont_skip_to_next_field == false)
        {
            while(true)
            {
                // Next cell position
                int next_row = pos.row();
                int next_column = pos.column() + increment;

                // Have we reached the end of the row? Then go to the next one
                if(next_column < 0 || next_column >= static_cast<int>(m_headers.size()))
                {
                    next_row += increment;
                    next_column = (reverse ? column_list.back() : column_list.front());
                }

                // Have we reached the last row? Then wrap around to the first one
                if(wrap && (next_row < 0 || next_row >= rowCount()))
                    next_row = (reverse ? rowCount()-1 : 0);

                // Set next index for search
                pos = pos.sibling(next_row, next_column);

                // Have we hit the last column? We have not found anything then
                if(pos == end)
                    return QModelIndex();

                // Is this a column which we are supposed to search in? If so, stop looking for the next cell and start comparing
                if(std::find(column_list.begin(), column_list.end(), next_column) != column_list.end())
                    break;
            }
        }

        // Make sure the next time we hit the above check, we actuall move on to the next cell and do not skip the loop again.
        dont_skip_to_next_field = false;

        // Get row from cache. If it is not in the cache, load the next chunk from the database
        const size_t row = static_cast<size_t>(pos.row());
        if(!m_cache.count(row))
        {
            triggerCacheLoad(static_cast<int>(row));
            waitUntilIdle();
        }
        const Row* row_data = &m_cache.at(row);

        // Get cell data
        const size_t column = static_cast<size_t>(pos.column());
        QString data = row_data->at(column);

        // Perform comparison
        if(whole_cell && !regex && data.compare(value, case_sensitive) == 0)
            return pos;
        else if(!whole_cell && !regex && data.contains(value, case_sensitive))
            return pos;
        else if(regex && reg_exp.match(data).hasMatch())
            return pos;
    }
}

void SqliteTableModel::reloadSettings()
{
    m_nullText = Settings::getValue("databrowser", "null_text").toString();
    m_blobText = Settings::getValue("databrowser", "blob_text").toString();
    m_regFgColour = QColor(Settings::getValue("databrowser", "reg_fg_colour").toString());
    m_regBgColour = QColor(Settings::getValue("databrowser", "reg_bg_colour").toString());
    m_nullFgColour = QColor(Settings::getValue("databrowser", "null_fg_colour").toString());
    m_nullBgColour = QColor(Settings::getValue("databrowser", "null_bg_colour").toString());
    m_binFgColour = QColor(Settings::getValue("databrowser", "bin_fg_colour").toString());
    m_binBgColour = QColor(Settings::getValue("databrowser", "bin_bg_colour").toString());
    m_font = QFont(Settings::getValue("databrowser", "font").toString());
    m_font.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
    m_symbolLimit = Settings::getValue("databrowser", "symbol_limit").toInt();
    m_imagePreviewEnabled = Settings::getValue("databrowser", "image_preview").toBool();
    m_chunkSize = static_cast<std::size_t>(Settings::getValue("db", "prefetchsize").toUInt());
}
