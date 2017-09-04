#ifndef DBSTRUCTUREMODEL_H
#define DBSTRUCTUREMODEL_H

#include <QAbstractItemModel>

class DBBrowserDB;
class QTreeWidgetItem;
namespace sqlb { class Object; typedef QSharedPointer<Object> ObjectPtr; }

class DbStructureModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit DbStructureModel(DBBrowserDB& db, QObject* parent = 0);
    ~DbStructureModel();

    void reloadData();

    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& = QModelIndex()) const;

    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList& indices) const;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);

    enum Columns
    {
        ColumnName,
        ColumnObjectType,
        ColumnDataType,
        ColumnSQL,
        ColumnSchema,
    };

private:
    DBBrowserDB& m_db;
    QTreeWidgetItem* rootItem;
    QTreeWidgetItem* browsablesRootItem;

    void buildTree(QTreeWidgetItem* parent, const QString& schema);
    QTreeWidgetItem* addNode(QTreeWidgetItem* parent, const sqlb::ObjectPtr& object, const QString& schema);
};

#endif
