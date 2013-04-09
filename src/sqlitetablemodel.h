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
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);
    size_t queryMore(size_t offset);

    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

    void setQuery(const QString& sQuery);
    void setTable(const QString& table);
    void setChunkSize(size_t chunksize);
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    Qt::ItemFlags flags(const QModelIndex& index) const;

signals:
    
public slots:
    void updateFilter(int column, QString value);

private:
    void fetchData(unsigned int from, unsigned to);

    void buildQuery();

    DBBrowserDB* m_db;
    int m_rowCount;
    QStringList m_headers;
    QList<QStringList> m_data;

    QString m_sQuery;
    QString m_sTable;
    int m_iSortColumn;
    QString m_sSortOrder;
    QMap<int, QString> m_mWhere;

    size_t m_chunkSize;
};

#endif // SQLITETABLEMODEL_H
