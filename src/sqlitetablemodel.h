#ifndef SQLITETABLEMODEL_H
#define SQLITETABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QVector>

class DBBrowserDB;
namespace sqlb { class ForeignKeyClause; }

class SqliteTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SqliteTableModel(QObject *parent = 0, DBBrowserDB* db = 0, size_t chunkSize = 50000, const QString& encoding = QString());
    void reset();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int totalRowCount() const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    bool setTypedData(const QModelIndex& index, bool isBlob, const QVariant& value, int role = Qt::EditRole);
    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
    void fetchMore(const QModelIndex &parent = QModelIndex());
    size_t queryMore(size_t offset);

    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

    void setQuery(const QString& sQuery, bool dontClearHeaders = false);
    QString query() const { return m_sQuery; }
    void setTable(const QString& table, const QVector<QString> &display_format = QVector<QString>());
    void setChunkSize(size_t chunksize);
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    Qt::ItemFlags flags(const QModelIndex& index) const;

    bool valid() const { return m_valid; }

    bool isBinary(const QModelIndex& index) const;

    void setEncoding(QString encoding) { m_encoding = encoding; }
    QString encoding() const { return m_encoding; }

    typedef QList<QByteArray> QByteArrayList;

    sqlb::ForeignKeyClause getForeignKeyClause(int column) const;

public slots:
    void updateFilter(int column, const QString& value);

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

    DBBrowserDB* m_db;
    int m_rowCount;
    QStringList m_headers;
    typedef QList<QByteArrayList> DataType;
    DataType m_data;

    QString m_sQuery;
    QString m_sTable;
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

    bool m_valid; //! tells if the current query is valid.

    QString m_encoding;
};

#endif // SQLITETABLEMODEL_H
