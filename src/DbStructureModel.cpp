#include "DbStructureModel.h"
#include "sqlitedb.h"
#include <QTreeWidgetItem>
#include <QMimeData>

DbStructureModel::DbStructureModel(QObject* parent)
    : QAbstractItemModel(parent)
{
    // Create root item and use its columns to store the header strings
    QStringList header;
    header << tr("Name") << tr("Object") << tr("Type") << tr("Schema");
    rootItem = new QTreeWidgetItem(header);
}

DbStructureModel::~DbStructureModel()
{
    delete rootItem;
}

int DbStructureModel::columnCount(const QModelIndex&) const
{
    return rootItem->columnCount();
}

QVariant DbStructureModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    // Get the item the index points at
    QTreeWidgetItem* item = static_cast<QTreeWidgetItem*>(index.internalPointer());

    // Depending on the role either return the text or the icon
    if(role == Qt::DisplayRole)
        return item->text(index.column());
    else if(role == Qt::DecorationRole)
        return item->icon(index.column());
    else
        return QVariant();
}

Qt::ItemFlags DbStructureModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    // All items are enabled and selectable
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    // Only enable dragging for entire table objects
    QString type = data(index.sibling(index.row(), 1), Qt::DisplayRole).toString();
    if(type == "table" || type == "view" || type == "index" || type == "trigger")
            flags |= Qt::ItemIsDragEnabled;

    return flags;
}

QVariant DbStructureModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // Get the header string from the root item
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section, role);

    return QVariant();
}

QModelIndex DbStructureModel::index(int row, int column, const QModelIndex& parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    QTreeWidgetItem *parentItem;
    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<QTreeWidgetItem*>(parent.internalPointer());

    QTreeWidgetItem* childItem = parentItem->child(row);
    if(childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DbStructureModel::parent(const QModelIndex& index) const
{
    if(!index.isValid())
        return QModelIndex();

    QTreeWidgetItem* childItem = static_cast<QTreeWidgetItem*>(index.internalPointer());
    QTreeWidgetItem* parentItem = childItem->parent();

    if(parentItem == rootItem)
        return QModelIndex();
    else
        return createIndex(0, 0, parentItem);
}

int DbStructureModel::rowCount(const QModelIndex& parent) const
{
    if(parent.column() > 0)
        return 0;

    if(!parent.isValid())
        return rootItem->childCount();
    else
        return static_cast<QTreeWidgetItem*>(parent.internalPointer())->childCount();
}

void DbStructureModel::reloadData(DBBrowserDB* db)
{
    // Remove all data except for the root item
    while(rootItem->childCount())
    {
        delete rootItem->child(0);
        rootItem->removeChild(rootItem->child(0));
    }

    // Create the nodes for tables, indices, views and triggers
    QMap<QString, QTreeWidgetItem*> typeToParentItem;
    QTreeWidgetItem* itemTables = new QTreeWidgetItem(rootItem);
    itemTables->setIcon(0, QIcon(QString(":/icons/table")));
    itemTables->setText(0, tr("Tables (%1)").arg(db->objMap.values("table").count()));
    typeToParentItem.insert("table", itemTables);
    QTreeWidgetItem* itemIndices = new QTreeWidgetItem(rootItem);
    itemIndices->setIcon(0, QIcon(QString(":/icons/index")));
    itemIndices->setText(0, tr("Indices (%1)").arg(db->objMap.values("index").count()));
    typeToParentItem.insert("index", itemIndices);
    QTreeWidgetItem* itemViews = new QTreeWidgetItem(rootItem);
    itemViews->setIcon(0, QIcon(QString(":/icons/view")));
    itemViews->setText(0, tr("Views (%1)").arg(db->objMap.values("view").count()));
    typeToParentItem.insert("view", itemViews);
    QTreeWidgetItem* itemTriggers = new QTreeWidgetItem(rootItem);
    itemTriggers->setIcon(0, QIcon(QString(":/icons/trigger")));
    itemTriggers->setText(0, tr("Triggers (%1)").arg(db->objMap.values("trigger").count()));
    typeToParentItem.insert("trigger", itemTriggers);

    // Add the actual table objects
    for(objectMap::ConstIterator it=db->objMap.begin();it!=db->objMap.end();++it)
    {
        // Object node
        QTreeWidgetItem *tableItem = new QTreeWidgetItem(typeToParentItem.value((*it).gettype()));
        tableItem->setIcon(0, QIcon(QString(":/icons/%1").arg((*it).gettype())));
        tableItem->setText(0, (*it).getname());
        tableItem->setText(1, (*it).gettype());
        tableItem->setText(3, (*it).getsql());

        // If it is a table or view add the field Nodes
        if((*it).gettype() == "table" || (*it).gettype() == "view")
        {
            for(int i=0;i<(*it).fldmap.size();i++)
            {
                QTreeWidgetItem *fldItem = new QTreeWidgetItem(tableItem);
                fldItem->setText(0, (*it).fldmap.at(i)->name());
                fldItem->setText(1, "field");
                fldItem->setText(2, (*it).fldmap.at(i)->type());
                fldItem->setIcon(0, QIcon(":/icons/field"));
            }
        }
    }

    // Refresh the view
    reset();
}

QStringList DbStructureModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

QMimeData* DbStructureModel::mimeData(const QModelIndexList& indices) const
{
    // Loop through selected indices
    QByteArray d;
    foreach(QModelIndex index, indices)
    {
        // Only export data for valid indices and only for the SQL column, i.e. only once per row
        if(index.isValid() && index.column() == 3)
            d = d.append(data(index, Qt::DisplayRole).toString() + ";\n");
    }

    // Create the MIME data object
    QMimeData* mime = new QMimeData();
    mime->setData("text/plain", d);
    return mime;
}
