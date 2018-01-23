#ifndef SQLITETABLEMODEL_H
#define SQLITETABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QVector>
#include <QFuture>

#include "sqlitetypes.h"

class DBBrowserDB;

class SqliteTableModel : public QAbstractTableModel
{
    Q_OBJECT

#ifdef REGEX_UNIT_TEST
    friend class TestRegex;
#endif

public:
    explicit SqliteTableModel(DBBrowserDB& db, QObject *parent = nullptr, size_t chunkSize = 50000, const QString& encoding = QString());
    ~SqliteTableModel();
    void reset();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int totalRowCount() const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int filterCount() const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    bool setTypedData(const QModelIndex& index, bool isBlob, const QVariant& value, int role = Qt::EditRole);
    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
    void fetchMore(const QModelIndex &parent = QModelIndex());

    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

    QModelIndex dittoRecord(int old_row);

    void setQuery(const QString& sQuery, bool dontClearHeaders = false);
    QString query() const { return m_sQuery; }
    QString customQuery(bool withRowid);
    void setTable(const sqlb::ObjectIdentifier& table, int sortColumn = 0, Qt::SortOrder sortOrder = Qt::AscendingOrder, const QMap<int, QString> filterValues = QMap<int, QString>(), const QVector<QString> &display_format = QVector<QString>());
    void setChunkSize(size_t chunksize);
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    const sqlb::ObjectIdentifier& currentTableName() const { return m_sTable; }

    Qt::ItemFlags flags(const QModelIndex& index) const;

    bool isBinary(const QModelIndex& index) const;

    void setEncoding(const QString& encoding) { m_encoding = encoding; }
    QString encoding() const { return m_encoding; }

    // The pseudo-primary key is exclusively for editing views
    void setPseudoPk(const QString& pseudoPk);
    QString pseudoPk() const { return m_pseudoPk; }

    typedef QVector<QByteArray> QByteArrayList;

    sqlb::ForeignKeyClause getForeignKeyClause(int column) const;

    // This returns true if the model is set up for editing. The model is able to operate in more or less two different modes, table browsing
    // and query browsing. We only support editing data for the table browsing mode and not for the query mode. This function returns true if
    // the model is currently editable, i.e. it's running in table mode and it isn't a view.
    bool isEditable() const;

    // Helper function for removing all comments from a SQL query
    static void removeCommentsFromQuery(QString& query);

    // Call this if you want to wait until the thread which is currently fetching more data is finished
    void waitForFetchingFinished();

public slots:
    void updateFilter(int column, const QString& value, bool applyQuery = true);

    // This cancels the execution of the current query. It can't guarantee that the query is stopped immediately or after returning but it should
    // stop soon afterwards. If some data has already been loaded into the model, that data is not deleted
    void cancelQuery();

signals:
    void finishedFetch();

protected:
    virtual Qt::DropActions supportedDropActions() const;
    virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);

private:
    void fetchData(unsigned int from, unsigned to);
    void clearCache();

    void buildQuery();
    QStringList getColumns(const QString& sQuery, QVector<int>& fieldsTypes);
    int getQueryRowCount();

    QByteArray encode(const QByteArray& str) const;
    QByteArray decode(const QByteArray& str) const;

    DBBrowserDB& m_db;
    QFuture<int> m_rowCount;
    int m_rowCountAdjustment;   // This needs to be added to the results of the m_rowCount future object to get the actual row count
    QStringList m_headers;
    typedef QList<QByteArrayList> DataType;
    DataType m_data;

    QString m_sQuery;
    sqlb::ObjectIdentifier m_sTable;
    QString m_sRowidColumn;
    QString m_pseudoPk;
    int m_iSortColumn;
    QString m_sSortOrder;
    QMap<int, QString> m_mWhere;
    QVector<QString> m_vDisplayFormat;
    QVector<int> m_vDataTypes;

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
    mutable QFuture<void> m_futureFetch;
    mutable QMutex m_mutexDataCache;
};

#endif
