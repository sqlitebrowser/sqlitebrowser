#ifndef SQLITETABLEMODEL_H
#define SQLITETABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QVector>
#include <QMutex>
#include <QColor>
#include <memory>

#include "RowCache.h"
#include "CondFormat.h"
#include "sql/Query.h"

struct sqlite3;
class DBBrowserDB;

class SqliteTableModel : public QAbstractTableModel
{
    Q_OBJECT

#ifdef REGEX_UNIT_TEST
    friend class TestRegex;
#endif

public:
    explicit SqliteTableModel(DBBrowserDB& db, QObject *parent = nullptr, size_t chunkSize = 50000, const QString& encoding = QString());
    ~SqliteTableModel() override;

    /// reset to state after construction
    void reset();

    /// returns logical amount of rows, whether currently cached or not
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int filterCount() const;
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
    void setQuery(const QString& sQuery, bool dontClearHeaders = false);

    QString query() const { return m_sQuery; }
    QString customQuery(bool withRowid) const { return QString::fromStdString(m_query.buildQuery(withRowid)); }

    /// configure for browsing specified table
    void setQuery(const sqlb::Query& query);

    void setChunkSize(size_t chunksize);
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    sqlb::ObjectIdentifier currentTableName() const { return m_query.table(); }

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool isBinary(const QModelIndex& index) const;

    void setEncoding(const QString& encoding) { m_encoding = encoding; }
    QString encoding() const { return m_encoding; }

    // The pseudo-primary key is exclusively for editing views
    void setPseudoPk(const QString& pseudoPk);
    bool hasPseudoPk() const;
    QString pseudoPk() const { return QString::fromStdString(m_query.rowIdColumn()); }

    sqlb::ForeignKeyClause getForeignKeyClause(int column) const;

    // This returns true if the model is set up for editing. The model is able to operate in more or less two different modes, table browsing
    // and query browsing. We only support editing data for the table browsing mode and not for the query mode. This function returns true if
    // the model is currently editable, i.e. it's running in table mode and it isn't a view.
    bool isEditable() const;

    // Helper function for removing all comments from a SQL query
    static void removeCommentsFromQuery(QString& query);

    void addCondFormat(int column, const CondFormat& condFormat);
    void setCondFormats(int column, const QVector<CondFormat>& condFormats);

public slots:
    void updateFilter(int column, const QString& value);

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

    void buildQuery();

    /// \param pDb connection to query; if null, obtains it from 'm_db'.
    QStringList getColumns(std::shared_ptr<sqlite3> pDb, const QString& sQuery, QVector<int>& fieldsTypes);

    QByteArray encode(const QByteArray& str) const;
    QByteArray decode(const QByteArray& str) const;

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

    QStringList m_headers;

    /// reading something in background right now? (either counting
    /// rows or actually loading data, doesn't matter)
    bool readingData() const;

    using Row = QVector<QByteArray>;
    mutable RowCache<Row> m_cache;

    Row makeDefaultCacheEntry () const;

    bool nosync_isBinary(const QModelIndex& index) const;

    QString m_sQuery;
    QVector<int> m_vDataTypes;
    QMap<int, QVector<CondFormat>> m_mCondFormats;
    sqlb::Query m_query;

    /**
     * @brief m_chunkSize Size of the next chunk fetch more will try to fetch.
     * This value should be rather high, because our query
     * uses LIMIT and sqlite3 will still execute the whole query and
     * just skip the not wanted rows, but the execution will
     * still take nearly the same time as doing the query at all up
     * to that row count.
     */
    size_t m_chunkSize;

    QString m_encoding;

    /**
     * These are used for multi-threaded population of the table
     */
    mutable QMutex m_mutexDataCache;
};

#endif
