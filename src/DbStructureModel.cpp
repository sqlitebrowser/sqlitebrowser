#include "DbStructureModel.h"
#include "sqlitedb.h"
#include "sqlitetablemodel.h"
#include "Settings.h"

#include <QTreeWidgetItem>
#include <QMimeData>
#include <QMessageBox>
#include <QApplication>

DbStructureModel::DbStructureModel(DBBrowserDB& db, QObject* parent)
    : QAbstractItemModel(parent),
      m_db(db)
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
    switch(role)
    {
    case Qt::DisplayRole:
        return Settings::getValue("db", "hideschemalinebreaks").toBool() ? item->text(index.column()).replace("\n", " ").simplified() : item->text(index.column());
    case Qt::EditRole:
    case Qt::ToolTipRole:   // Don't modify the text when it's supposed to be shown in a tooltip
        return item->text(index.column());
    case Qt::DecorationRole:
        return item->icon(index.column());
    default:
        return QVariant();
    }
}

Qt::ItemFlags DbStructureModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::ItemIsDropEnabled;

    // All items are enabled and selectable
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;

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

void DbStructureModel::reloadData()
{
    beginResetModel();

    // Remove all data except for the root item
    while(rootItem->childCount())
        delete rootItem->child(0);

    // Return here if no DB is opened
    if(!m_db.isOpen())
    {
        endResetModel();
        return;
    }

    // Create the nodes for browsables and for tables, indices, views and triggers. The idea here is to basically have two trees in one model:
    // In the root node there are two nodes: 'browsables' and 'all'. The first node contains a list of a all browsable objects, i.e. views and tables.
    // The seconds node contains four sub-nodes (tables, indices, views and triggers), each containing a list of objects of that type.
    // This way we only have to have and only have to update one model and can use it in all sorts of places, just by setting a different root node.
    QMap<QString, QTreeWidgetItem*> typeToParentItem;

    QTreeWidgetItem* itemBrowsables = new QTreeWidgetItem(rootItem);
    itemBrowsables->setIcon(0, QIcon(QString(":/icons/view")));
    itemBrowsables->setText(0, tr("Browsables (%1)").arg(m_db.objMap.values("table").count() + m_db.objMap.values("view").count()));
    typeToParentItem.insert("browsable", itemBrowsables);

    QTreeWidgetItem* itemAll = new QTreeWidgetItem(rootItem);
    itemAll->setIcon(0, QIcon(QString(":/icons/view")));
    itemAll->setText(0, tr("All"));

    QTreeWidgetItem* itemTables = new QTreeWidgetItem(itemAll);
    itemTables->setIcon(0, QIcon(QString(":/icons/table")));
    itemTables->setText(0, tr("Tables (%1)").arg(m_db.objMap.values("table").count()));
    typeToParentItem.insert("table", itemTables);

    QTreeWidgetItem* itemIndices = new QTreeWidgetItem(itemAll);
    itemIndices->setIcon(0, QIcon(QString(":/icons/index")));
    itemIndices->setText(0, tr("Indices (%1)").arg(m_db.objMap.values("index").count()));
    typeToParentItem.insert("index", itemIndices);

    QTreeWidgetItem* itemViews = new QTreeWidgetItem(itemAll);
    itemViews->setIcon(0, QIcon(QString(":/icons/view")));
    itemViews->setText(0, tr("Views (%1)").arg(m_db.objMap.values("view").count()));
    typeToParentItem.insert("view", itemViews);

    QTreeWidgetItem* itemTriggers = new QTreeWidgetItem(itemAll);
    itemTriggers->setIcon(0, QIcon(QString(":/icons/trigger")));
    itemTriggers->setText(0, tr("Triggers (%1)").arg(m_db.objMap.values("trigger").count()));
    typeToParentItem.insert("trigger", itemTriggers);

    // Get all database objects and sort them by their name
    QMultiMap<QString, sqlb::ObjectPtr> dbobjs;
    for(auto it=m_db.objMap.constBegin(); it != m_db.objMap.constEnd(); ++it)
        dbobjs.insert((*it)->name(), (*it));

    // Add the actual table objects
    for(auto it=dbobjs.constBegin();it!=dbobjs.constEnd();++it)
    {
        // Object node
        QTreeWidgetItem* item = addNode(typeToParentItem.value(sqlb::Object::typeToString((*it)->type())), *it);

        // If it is a table or view add the field nodes, add an extra node for the browsable section
        if((*it)->type() == sqlb::Object::Types::Table || (*it)->type() == sqlb::Object::Types::View)
            addNode(typeToParentItem.value("browsable"), *it);

        // Add field nodes if there are any
        sqlb::FieldInfoList fieldList = (*it)->fieldInformation();
        if(!fieldList.empty())
        {
            QStringList pk_columns;
            if((*it)->type() == sqlb::Object::Types::Table)
            {
                sqlb::FieldVector pk = (*it).dynamicCast<sqlb::Table>()->primaryKey();
                foreach(sqlb::FieldPtr pk_col, pk)
                    pk_columns.push_back(pk_col->name());

            }
            foreach(const sqlb::FieldInfo& field, fieldList)
            {
                QTreeWidgetItem *fldItem = new QTreeWidgetItem(item);
                fldItem->setText(0, field.name);
                fldItem->setText(1, "field");
                fldItem->setText(2, field.type);
                fldItem->setText(3, field.sql);
                if(pk_columns.contains(field.name))
                    fldItem->setIcon(0, QIcon(":/icons/field_key"));
                else
                    fldItem->setIcon(0, QIcon(":/icons/field"));
            }
        }
    }

    // Refresh the view
    endResetModel();
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
        {
            // Add the SQL code used to create the object
            d = d.append(data(index, Qt::DisplayRole).toString() + ";\n");

            // If it is a table also add the content
            if(data(index.sibling(index.row(), 1), Qt::DisplayRole).toString() == "table")
            {
                SqliteTableModel tableModel(m_db);
                tableModel.setTable(data(index.sibling(index.row(), 0), Qt::DisplayRole).toString());
                for(int i=0; i < tableModel.rowCount(); ++i)
                {
                    QString insertStatement = "INSERT INTO " + sqlb::escapeIdentifier(data(index.sibling(index.row(), 0), Qt::DisplayRole).toString()) + " VALUES(";
                    for(int j=1; j < tableModel.columnCount(); ++j)
                        insertStatement += QString("'%1',").arg(tableModel.data(tableModel.index(i, j)).toString());
                    insertStatement.chop(1);
                    insertStatement += ");\n";
                    d = d.append(insertStatement);
                }
            }
        }
    }

    // Create the MIME data object
    QMimeData* mime = new QMimeData();
    mime->setProperty("db_file", m_db.currentFile());      // Also save the file name to avoid dropping an object on the same database as it comes from
    mime->setData("text/plain", d);
    return mime;
}

bool DbStructureModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int, int, const QModelIndex&)
{
    if(action == Qt::IgnoreAction)
        return true;

    if(!data->hasFormat("text/plain"))
        return false;

    if(data->property("db_file") == m_db.currentFile())
        return false;

    // Get data
    QByteArray d = data->data("text/plain");

    // Try to execute the SQL statement
    if(m_db.executeMultiSQL(d, true, true))
    {
        m_db.updateSchema();
        reloadData();
        return true;
    } else {
        QMessageBox::warning(0, QApplication::applicationName(), m_db.lastError());
        return false;
    }
}

QTreeWidgetItem* DbStructureModel::addNode(QTreeWidgetItem* parent, const sqlb::ObjectPtr& object)
{
    QString type = sqlb::Object::typeToString(object->type());

    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setIcon(0, QIcon(QString(":/icons/%1").arg(type)));
    item->setText(0, object->name());
    item->setText(1, type);
    item->setText(3, object->originalSql());

    return item;
}
