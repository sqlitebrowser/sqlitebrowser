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
    header << tr("Name") << tr("Object") << tr("Type") << tr("Schema") << tr("Database");
    rootItem = new QTreeWidgetItem(header);
    browsablesRootItem = nullptr;
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
        // For the display role and the browsabled branch of the tree we want to show the column name including the schema name if necessary (i.e.
        // for schemata != "main"). For the normal structure branch of the tree we don't want to add the schema name because it's already obvious from
        // the position of the item in the tree.
        if(index.column() == ColumnName && item->parent() == browsablesRootItem)
            return sqlb::ObjectIdentifier(item->text(ColumnSchema), item->text(ColumnName)).toDisplayString();
        else
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
    QString type = data(index.sibling(index.row(), ColumnObjectType), Qt::DisplayRole).toString();
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
    browsablesRootItem = new QTreeWidgetItem(rootItem);
    browsablesRootItem->setIcon(ColumnName, QIcon(QString(":/icons/view")));
    browsablesRootItem->setText(ColumnName, tr("Browsables"));

    // Make sure to always load the main schema first
    QTreeWidgetItem* itemAll = new QTreeWidgetItem(rootItem);
    itemAll->setIcon(ColumnName, QIcon(QString(":/icons/database")));
    itemAll->setText(ColumnName, tr("All"));
    buildTree(itemAll, "main");

    // Add the temporary database as a node if it isn't empty. Make sure it's always second if it exists.
    if(!m_db.schemata["temp"].isEmpty())
    {
        QTreeWidgetItem* itemTemp = new QTreeWidgetItem(itemAll);
        itemTemp->setIcon(ColumnName, QIcon(QString(":/icons/database")));
        itemTemp->setText(ColumnName, tr("Temporary"));
        buildTree(itemTemp, "temp");
    }

    // Now load all the other schemata last
    for(auto it=m_db.schemata.constBegin();it!=m_db.schemata.constEnd();++it)
    {
        // Don't load the main and temp schema again
        if(it.key() != "main" && it.key() != "temp")
        {
            QTreeWidgetItem* itemSchema = new QTreeWidgetItem(itemAll);
            itemSchema->setIcon(ColumnName, QIcon(QString(":/icons/database")));
            itemSchema->setText(ColumnName, it.key());
            buildTree(itemSchema, it.key());
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
    for(const QModelIndex& index : indices)
    {
        // Only export data for valid indices and only for the SQL column, i.e. only once per row
        if(index.isValid() && index.column() == ColumnSQL)
        {
            // Add the SQL code used to create the object
            d = d.append(data(index, Qt::DisplayRole).toString() + ";\n");

            // If it is a table also add the content
            if(data(index.sibling(index.row(), ColumnObjectType), Qt::DisplayRole).toString() == "table")
            {
                SqliteTableModel tableModel(m_db);
                sqlb::ObjectIdentifier objid(data(index.sibling(index.row(), ColumnSchema), Qt::DisplayRole).toString(),
                                             data(index.sibling(index.row(), ColumnName), Qt::DisplayRole).toString());
                tableModel.setTable(objid);
                tableModel.waitForFetchingFinished();
                for(int i=0; i < tableModel.rowCount(); ++i)
                {
                    QString insertStatement = "INSERT INTO " + objid.toString() + " VALUES(";
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

void DbStructureModel::buildTree(QTreeWidgetItem* parent, const QString& schema)
{
    // Build a map from object type to tree node to simplify finding the correct tree node later
    QMap<QString, QTreeWidgetItem*> typeToParentItem;

    // Get object map for the given schema
    objectMap objmap = m_db.schemata[schema];

    // Prepare tree
    QTreeWidgetItem* itemTables = new QTreeWidgetItem(parent);
    itemTables->setIcon(ColumnName, QIcon(QString(":/icons/table")));
    itemTables->setText(ColumnName, tr("Tables (%1)").arg(objmap.values("table").count()));
    typeToParentItem.insert("table", itemTables);

    QTreeWidgetItem* itemIndices = new QTreeWidgetItem(parent);
    itemIndices->setIcon(ColumnName, QIcon(QString(":/icons/index")));
    itemIndices->setText(ColumnName, tr("Indices (%1)").arg(objmap.values("index").count()));
    typeToParentItem.insert("index", itemIndices);

    QTreeWidgetItem* itemViews = new QTreeWidgetItem(parent);
    itemViews->setIcon(ColumnName, QIcon(QString(":/icons/view")));
    itemViews->setText(ColumnName, tr("Views (%1)").arg(objmap.values("view").count()));
    typeToParentItem.insert("view", itemViews);

    QTreeWidgetItem* itemTriggers = new QTreeWidgetItem(parent);
    itemTriggers->setIcon(ColumnName, QIcon(QString(":/icons/trigger")));
    itemTriggers->setText(ColumnName, tr("Triggers (%1)").arg(objmap.values("trigger").count()));
    typeToParentItem.insert("trigger", itemTriggers);

    // Get all database objects and sort them by their name
    QMultiMap<QString, sqlb::ObjectPtr> dbobjs;
    for(auto it=objmap.constBegin(); it != objmap.constEnd(); ++it)
        dbobjs.insert((*it)->name(), (*it));

    // Add the database objects to the tree nodes
    for(auto it=dbobjs.constBegin();it!=dbobjs.constEnd();++it)
    {
        // Object node
        QTreeWidgetItem* item = addNode(typeToParentItem.value(sqlb::Object::typeToString((*it)->type())), *it, schema);

        // If it is a table or view add the field nodes, add an extra node for the browsable section
        if((*it)->type() == sqlb::Object::Types::Table || (*it)->type() == sqlb::Object::Types::View)
            addNode(browsablesRootItem, *it, schema);

        // Add field nodes if there are any
        sqlb::FieldInfoList fieldList = (*it)->fieldInformation();
        if(!fieldList.empty())
        {
            QStringList pk_columns;
            if((*it)->type() == sqlb::Object::Types::Table)
            {
                sqlb::FieldVector pk = (*it).dynamicCast<sqlb::Table>()->primaryKey();
                for(const sqlb::FieldPtr& pk_col : pk)
                    pk_columns.push_back(pk_col->name());

            }
            for(const sqlb::FieldInfo& field : fieldList)
            {
                QTreeWidgetItem *fldItem = new QTreeWidgetItem(item);
                fldItem->setText(ColumnName, field.name);
                fldItem->setText(ColumnObjectType, "field");
                fldItem->setText(ColumnDataType, field.type);
                fldItem->setText(ColumnSQL, field.sql);
                fldItem->setText(ColumnSchema, schema);
                if(pk_columns.contains(field.name))
                    fldItem->setIcon(ColumnName, QIcon(":/icons/field_key"));
                else
                    fldItem->setIcon(ColumnName, QIcon(":/icons/field"));
            }
        }
    }
}

QTreeWidgetItem* DbStructureModel::addNode(QTreeWidgetItem* parent, const sqlb::ObjectPtr& object, const QString& schema)
{
    QString type = sqlb::Object::typeToString(object->type());

    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setIcon(ColumnName, QIcon(QString(":/icons/%1").arg(type)));
    item->setText(ColumnName, object->name());
    item->setText(ColumnObjectType, type);
    item->setText(ColumnSQL, object->originalSql());
    item->setText(ColumnSchema, schema);

    return item;
}
