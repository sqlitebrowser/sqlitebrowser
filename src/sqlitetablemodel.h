#ifndef SQLITETABLEMODEL_H
#define SQLITETABLEMODEL_H

#include <QAbstractTableModel>
#include <QColor>
#include <QFont>

#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "RowCache.h"
#include "sql/Query.h"
#include "sql/sqlitetypes.h"

struct sqlite3;
class DBBrowserDB;
class CondFormat;

class SqliteTableModel : public QAbstractTableModel
{
    Q_OBJECT

#ifdef REGEX_UNIT_TEST
    friend class TestRegex;
#endif

public:
    explicit SqliteTableModel(DBBrowserDB& db, QObject *parent = nullptr, const QString& encoding = QString(), bool force_wait = false);
    ~SqliteTableModel() override;

    /// reset to state after construction
    void reset();

    /// returns logical amount of rows, whether currently cached or not
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    size_t filterCount() const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    bool setTypedData(const QModelIndex& index, bool isBlob, const QVariant& value, int role = Qt::EditRole);

    enum class RowCount
    {
        Unknown,  //< still finding out in background...
        Partial,  //< some chunk was read and at least a lower bound is thus known
        Complete  //< total row count of table known
    };

    /// what kind of information is available through rowCount()?
    RowCount rowCountAvailable () const;

    /// trigger asynchronous loading of (at least) the specified row
    /// into cache.
    void triggerCacheLoad (int single_row) const;

    /// trigger asynchronous loading of (at least) the specified rows
    /// into cache. \param row_end is exclusive.
    void triggerCacheLoad (int row_begin, int row_end) const;

    /// wait until not reading any data (that does not mean data is
    /// complete, just that the background reader is idle)
    void waitUntilIdle () const;

    /// load all rows into cache, return when done. Returns true if all data was loaded, false if the loading was cancelled.
    bool completeCache() const;

    /// returns true if all rows are currently available in cache
    /// [NOTE: potentially unsafe in case we have a limited-size
    /// cache, where entries can vanish again -- however we can't do
    /// this for the current implementation of the PlotDock]
    bool isCacheComplete () const;

    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

    QModelIndex dittoRecord(int old_row);

    /// configure for browsing results of specified query
    void setQuery(const QString& sQuery);

    std::string query() const { return m_sQuery.toStdString(); }
    std::string customQuery(bool withRowid) const { return m_query.buildQuery(withRowid); }

    /// configure for browsing specified table
    void setQuery(const sqlb::Query& query);

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    void sort(const std::vector<sqlb::OrderBy>& columns);
    sqlb::ObjectIdentifier currentTableName() const { return m_query.table(); }

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool isBinary(const QModelIndex& index) const;

    void setEncoding(const QString& encoding) { m_encoding = encoding; }
    QString encoding() const { return m_encoding; }

    // The pseudo-primary key is exclusively for editing views
    void setPseudoPk(std::vector<std::string> pseudoPk);
    bool hasPseudoPk() const;
    std::vector<std::string> pseudoPk() const { return m_query.rowIdColumns(); }

    std::shared_ptr<sqlb::ForeignKeyClause> getForeignKeyClause(size_t column) const;

    // This returns true if the model and, if set, the index can be edited. Not specifying the index parameter asks whether the model can
    // be edited in general (i.e. inserting and deleting rows as well as updating some cells). Specifying the index parameter asks whether
    // this specific index can be edited.
    // The model is able to operate in more or less two different modes, table browsing and query browsing. We only support editing data
    // in the table browsing mode but not for the query mode. This function returns true if the model is currently editable, i.e. it's
    // running in the table mode and isn't browsing a view, unless this view is set up for editing by specifying a pseudo PK.
    // When the index parameter is set, the same checks are performed but additionally the function checks whether this specific index
    // can be edited. This makes a difference for generated columns which are in (editable) tables but cannot be modified anyway.
    bool isEditable(const QModelIndex& index = QModelIndex()) const;

    // Conditional formats are of two kinds: regular conditional formats (including condition-free formats applying to any value in the
    // column) and formats applying to a particular row-id and which have always precedence over the first kind and whose filter apply
    // to the row-id column.
    void addCondFormat(const bool isRowIdFormat, size_t column, const CondFormat& condFormat);
    void setCondFormats(const bool isRowIdFormat, size_t column, const std::vector<CondFormat>& condFormats);

    // Search for the specified expression in the given cells. This intended as a replacement for QAbstractItemModel::match() even though
    // it does not override it, which - because of the different parameters - is not possible.
    // start contains the index to start with, column_list contains the ordered list of the columns to look in, value is the value to search for,
    // flags allows to modify  the search process (Qt::MatchContains, Qt::MatchRegExp, Qt::MatchCaseSensitive, and Qt::MatchWrap are understood),
    // reverse can be set to true to progress through the cells in backwards direction, and dont_skip_to_next_field can be set to true if the current
    // cell can be matched as well.
    QModelIndex nextMatch(const QModelIndex& start,
                          const std::vector<int>& column_list,
                          const QString& value,
                          Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchContains),
                          bool reverse = false,
                          bool dont_skip_to_next_field = false) const;

    DBBrowserDB& db() { return m_db; }

    void reloadSettings();

public slots:
    void updateFilter(const std::string& column, const QString& value);
    void updateGlobalFilter(const std::vector<QString>& values);

signals:
    void finishedFetch(int fetched_row_begin, int fetched_row_end);
    void finishedRowCount();

protected:
    Qt::DropActions supportedDropActions() const override;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;

private:
    friend class RowLoader;
    class RowLoader * worker;

    /// clears the cache, resets row-count to unknown (but keeps table
    /// & query info), increase life_counter
    void clearCache();

    void handleFinishedFetch(int life_id, unsigned int fetched_row_begin, unsigned int fetched_row_end);
    void handleRowCountComplete(int life_id, int num_rows);

    void updateAndRunQuery();

    void getColumnNames(const std::string& sQuery);

    QByteArray encode(const QByteArray& str) const;
    QByteArray decode(const QByteArray& str) const;

    // Return matching conditional format color/font or invalid value, otherwise.
    // Only format roles are expected in role (Qt::ItemDataRole)
    QVariant getMatchingCondFormat(size_t row, size_t column, const QString& value, int role) const;
    QVariant getMatchingCondFormat(const std::map<size_t, std::vector<CondFormat>>& mCondFormats, size_t column, const QString& value, int role) const;

    DBBrowserDB& m_db;

    /// counts numbers of clearCache() since instantiation; using this
    /// to avoid processing of queued signals originating in an era
    /// before the most recent reset().
    int m_lifeCounter;

    /// note: the row count can be determined by the row-count query
    /// (which yields the "final" row count"), or, if it is faster, by
    /// the first chunk reading actual data (in which case the row
    /// count will be set to that chunk's size and later updated to
    /// the full row count, when the row-count query returns)
    RowCount m_rowCountAvailable;
    unsigned int m_currentRowCount;

    std::vector<std::string> m_headers;

    /// reading something in background right now? (either counting
    /// rows or actually loading data, doesn't matter)
    bool readingData() const;

    using Row = std::vector<QByteArray>;
    mutable RowCache<Row> m_cache;

    Row makeDefaultCacheEntry () const;

    bool isBinary(const QByteArray& index) const;

    QString m_sQuery;
    std::vector<int> m_vDataTypes;
    std::map<size_t, std::vector<CondFormat>> m_mCondFormats;
    std::map<size_t, std::vector<CondFormat>> m_mRowIdFormats;

    sqlb::Query m_query;
    std::shared_ptr<sqlb::Table> m_table_of_query;  // This holds a pointer to the table object which is queried in the m_query object

    QString m_encoding;

    /**
     * These are used for multi-threaded population of the table
     */
    mutable std::mutex m_mutexDataCache;

private:
    /**
     * Settings. These are stored here to avoid fetching and converting them every time we need them. Because this class
     * uses a lot of settings and because some of its functions are called very often, this should speed things up noticeable.
     * Call reloadSettings() to update these.
     */

    QString m_nullText;
    QString m_blobText;
    QColor m_regFgColour;
    QColor m_regBgColour;
    QColor m_nullFgColour;
    QColor m_nullBgColour;
    QColor m_binFgColour;
    QColor m_binBgColour;
    QFont m_font;
    int m_symbolLimit;
    bool m_imagePreviewEnabled;

    /**
     * @brief m_chunkSize Size of the next chunk fetch more will try to fetch.
     * This value should be rather high, because our query
     * uses LIMIT and sqlite3 will still execute the whole query and
     * just skip the not wanted rows, but the execution will
     * still take nearly the same time as doing the query at all up
     * to that row count.
     */
    size_t m_chunkSize;
};

#endif
