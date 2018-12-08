#ifndef DBSTRUCTUREMODEL_H
#define DBSTRUCTUREMODEL_H

#include <QAbstractItemModel>
#include <memory>

class DBBrowserDB;
class QTreeWidgetItem;
namespace sqlb { class Object; using ObjectPtr = std::shared_ptr<Object>; }

class DbStructureModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit DbStructureModel(DBBrowserDB& db, QObject* parent = nullptr);
    ~DbStructureModel() override;

    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& = QModelIndex()) const override;

    QStringList mimeTypes() const override;
    QMimeData* mimeData(const QModelIndexList& indices) const override;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;

    enum Columns
    {
        ColumnName,
        ColumnObjectType,
        ColumnDataType,
        ColumnSQL,
        ColumnSchema,
    };

public slots:
    void reloadData();
    void setDropQualifiedNames(bool value) { m_dropQualifiedNames = value; }
    void setDropEnquotedNames(bool value) { m_dropEnquotedNames = value; }

signals:
    void structureUpdated();

private:
    DBBrowserDB& m_db;
    QTreeWidgetItem* rootItem;
    QTreeWidgetItem* browsablesRootItem;
    bool m_dropQualifiedNames;
    bool m_dropEnquotedNames;

    void buildTree(QTreeWidgetItem* parent, const QString& schema);
    QTreeWidgetItem* addNode(QTreeWidgetItem* parent, const sqlb::ObjectPtr& object, const QString& schema);
    QString getNameForDropping(const QString& domain, const QString& object, const QString& field) const;
};

#endif
