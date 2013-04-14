#ifndef SQLITETABLEMODEL_H
#define SQLITETABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

class DBBrowserDB;

class SqliteTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SqliteTableModel(QObject *parent = 0, DBBrowserDB* db = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int totalRowCount() const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
    void fetchMore(const QModelIndex &parent = QModelIndex());
    size_t queryMore(size_t offset);

    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

    void setQuery(const QString& sQuery);
    void setTable(const QString& table);
    void setChunkSize(size_t chunksize);
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    Qt::ItemFlags flags(const QModelIndex& index) const;

    typedef QList<QByteArray> QByteArrayList;

signals:
    
public slots:
    void updateFilter(int column, const QString& value);

private:
    void fetchData(unsigned int from, unsigned to);
    void clearCache();

    void buildQuery();

    DBBrowserDB* m_db;
    int m_rowCount;
    QStringList m_headers;
    QList<QByteArrayList> m_data;

    QString m_sQuery;
    QString m_sTable;
    int m_iSortColumn;
    QString m_sSortOrder;
    QMap<int, QString> m_mWhere;

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

#endif // SQLITETABLEMODEL_H
