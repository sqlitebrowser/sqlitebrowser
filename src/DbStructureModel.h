#ifndef DBSTRUCTUREMODEL_H
#define DBSTRUCTUREMODEL_H

#include <QAbstractItemModel>
#include <memory>

class DBBrowserDB;
class QTreeWidgetItem;

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
    void setDropSelectQuery(bool value) { m_dropSelectQuery = value; }

private:
    DBBrowserDB& m_db;
    QTreeWidgetItem* rootItem;
    QTreeWidgetItem* browsablesRootItem;
    bool m_dropQualifiedNames;
    bool m_dropEnquotedNames;
    bool m_dropSelectQuery;

    void buildTree(QTreeWidgetItem* parent, const std::string& schema);
    QTreeWidgetItem* addNode(const std::string& schema, const std::string& name, const std::string& object_type, const std::string& sql, QTreeWidgetItem* parent_item, const std::string& data_type = {}, const std::string& icon_suffix = {});
    QString getNameForDropping(const QString& domain, const QString& object, const QString& field) const;
};

#endif
