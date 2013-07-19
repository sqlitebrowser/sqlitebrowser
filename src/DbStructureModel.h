#ifndef __DBSTRUCTUREMODEL_H__
#define __DBSTRUCTUREMODEL_H__

#include <QAbstractItemModel>
class DBBrowserDB;
class QTreeWidgetItem;

class DbStructureModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit DbStructureModel(QObject* parent = 0);
    ~DbStructureModel();

    void reloadData(DBBrowserDB* db);

    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& = QModelIndex()) const;

    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList& indices) const;

private:
    QTreeWidgetItem* rootItem;
};

#endif
