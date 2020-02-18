#include "DbStructureModel.h"
#include "IconCache.h"
#include "sqlitedb.h"
#include "sqlitetablemodel.h"
#include "Settings.h"

#include <QTreeWidgetItem>
#include <QMimeData>
#include <QMessageBox>
#include <QApplication>
#include <unordered_map>
#include <map>

DbStructureModel::DbStructureModel(DBBrowserDB& db, QObject* parent)
    : QAbstractItemModel(parent),
      m_db(db),
      browsablesRootItem(nullptr),
      m_dropQualifiedNames(false),
      m_dropEnquotedNames(false)
{
    // Create root item and use its columns to store the header strings
    QStringList header;
    header << tr("Name") << tr("Object") << tr("Type") << tr("Schema") << tr("Database");
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
        // For the display role and the browsable branch of the tree we want to show the column name including the schema name if necessary (i.e.
        // for schemata != "main"). For the normal structure branch of the tree we don't want to add the schema name because it's already obvious from
        // the position of the item in the tree.
        if(index.column() == ColumnName && item->parent() == browsablesRootItem)
            return QString::fromStdString(sqlb::ObjectIdentifier(item->text(ColumnSchema).toStdString(), item->text(ColumnName).toStdString()).toDisplayString());
        else
            return Settings::getValue("db", "hideschemalinebreaks").toBool() ? item->text(index.column()).simplified() : item->text(index.column());
    case Qt::EditRole:
        return item->text(index.column());
    case Qt::ToolTipRole: {
        // Show the original text but limited, when it's supposed to be shown in a tooltip
        QString text = item->text(index.column());
        if (text.length() > 512) {
            text.truncate(509);
            text.append("...");
        }
        return text;
    }
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

    // Only enable dragging for SQLite objects and for fields (composition in SQL text editor).
    // Grouping nodes have no type.
    QString type = data(index.sibling(index.row(), ColumnObjectType), Qt::DisplayRole).toString();
    if(!type.isEmpty())
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
        emit structureUpdated();
        return;
    }

    // Create the nodes for browsables and for tables, indices, views and triggers. The idea here is to basically have two trees in one model:
    // In the root node there are two nodes: 'browsables' and 'all'. The first node contains a list of all browsable objects, i.e. views and tables.
    // The second node contains four sub-nodes (tables, indices, views and triggers), each containing a list of objects of that type.
    // This way we only have to have and only have to update one model and can use it in all sorts of places, just by setting a different root node.
    browsablesRootItem = new QTreeWidgetItem(rootItem);
    browsablesRootItem->setIcon(ColumnName, IconCache::get("view"));
    browsablesRootItem->setText(ColumnName, tr("Browsables"));

    // Make sure to always load the main schema first
    QTreeWidgetItem* itemAll = new QTreeWidgetItem(rootItem);
    itemAll->setIcon(ColumnName, IconCache::get("database"));
    itemAll->setText(ColumnName, tr("All"));
    itemAll->setText(ColumnObjectType, "database");
    buildTree(itemAll, "main");

    // Add the temporary database as a node if it isn't empty. Make sure it's always second if it exists.
    if(!m_db.schemata["temp"].empty())
    {
        QTreeWidgetItem* itemTemp = new QTreeWidgetItem(itemAll);
        itemTemp->setIcon(ColumnName, IconCache::get("database"));
        itemTemp->setText(ColumnName, tr("Temporary"));
        itemTemp->setText(ColumnObjectType, "database");
        buildTree(itemTemp, "temp");
    }

    // Now load all the other schemata last
    for(const auto& it : m_db.schemata)
    {
        // Don't load the main and temp schema again
        if(it.first != "main" && it.first != "temp")
        {
            QTreeWidgetItem* itemSchema = new QTreeWidgetItem(itemAll);
            itemSchema->setIcon(ColumnName, IconCache::get("database"));
            itemSchema->setText(ColumnName, QString::fromStdString(it.first));
            itemSchema->setText(ColumnObjectType, "database");
            buildTree(itemSchema, it.first);
        }
    }

    // Refresh the view
    endResetModel();
    emit structureUpdated();
}

QStringList DbStructureModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

QMimeData* DbStructureModel::mimeData(const QModelIndexList& indices) const
{
    // We store the SQL data and the names data separately
    QByteArray sqlData, namesData;

    // Loop through selected indices
    for(const QModelIndex& index : indices)
    {
        // Get the item the index points at
        QTreeWidgetItem* item = static_cast<QTreeWidgetItem*>(index.internalPointer());

        // Only export data for valid indices and only once per row (SQL column or Name column).
        if(index.isValid()) {
            QString objectType = data(index.sibling(index.row(), ColumnObjectType), Qt::DisplayRole).toString();

            // For names, export a (qualified) (escaped) identifier of the item for statement composition in SQL editor.
            if(objectType == "field")
                namesData.append(getNameForDropping(item->text(ColumnSchema), item->parent()->text(ColumnName), item->text(ColumnName)));
            else if(objectType == "database")
                namesData.append(getNameForDropping(item->text(ColumnName), "", ""));
            else if(!objectType.isEmpty())
                namesData.append(getNameForDropping(item->text(ColumnSchema), item->text(ColumnName), ""));

            if(objectType != "field" && index.column() == ColumnSQL)
            {
                // Add the SQL code used to create the object
                sqlData.append(data(index, Qt::DisplayRole).toString() + ";\n");

                // If it is a table also add the content
                if(objectType == "table")
                {
                    SqliteTableModel tableModel(m_db);
                    sqlb::ObjectIdentifier objid(data(index.sibling(index.row(), ColumnSchema), Qt::DisplayRole).toString().toStdString(),
                                                 data(index.sibling(index.row(), ColumnName), Qt::DisplayRole).toString().toStdString());
                    tableModel.setQuery(sqlb::Query(objid));
                    if(tableModel.completeCache())
                    {
                        // Only continue if all data was fetched

                        for(int i=0; i < tableModel.rowCount(); ++i)
                        {
                            QString insertStatement = "INSERT INTO " + QString::fromStdString(objid.toString()) + " VALUES(";
                            for(int j=1; j < tableModel.columnCount(); ++j)
                                insertStatement += QString("'%1',").arg(tableModel.data(tableModel.index(i, j), Qt::EditRole).toString());
                            insertStatement.chop(1);
                            insertStatement += ");\n";
                            sqlData.append(insertStatement);
                        }
                    }
                }
            }
        }
    }

    // Create the MIME data object
    QMimeData* mime = new QMimeData();
    mime->setProperty("db_file", m_db.currentFile());      // Also save the file name to avoid dropping an object on the same database as it comes from
    // When we have both SQL and Names data (probable row selection mode) we give precedence to the SQL data
    if (sqlData.length() == 0 && namesData.length() > 0) {
        // Remove last ", " or "."
        if (namesData.endsWith(", "))
            namesData.chop(2);
        else if (namesData.endsWith("."))
            namesData.chop(1);

        mime->setData("text/plain", namesData);
    } else
        mime->setData("text/plain", sqlData);
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
        return true;
    } else {
        QMessageBox::warning(nullptr, QApplication::applicationName(), m_db.lastError());
        return false;
    }
}

static long calc_number_of_objects_by_type(const objectMap& objmap, const std::string& type)
{
    auto objects = objmap.equal_range(type);
    if(objects.first == objmap.end())
        return 0;
    else
        return std::distance(objects.first, objects.second);
}

void DbStructureModel::buildTree(QTreeWidgetItem* parent, const std::string& schema)
{
    // Build a map from object type to tree node to simplify finding the correct tree node later
    std::unordered_map<std::string, QTreeWidgetItem*> typeToParentItem;

    // Get object map for the given schema
    objectMap objmap = m_db.schemata[schema];

    // Prepare tree
    QTreeWidgetItem* itemTables = new QTreeWidgetItem(parent);
    itemTables->setIcon(ColumnName, IconCache::get("table"));
    itemTables->setText(ColumnName, tr("Tables (%1)").arg(calc_number_of_objects_by_type(objmap, "table")));
    typeToParentItem.insert({"table", itemTables});

    QTreeWidgetItem* itemIndices = new QTreeWidgetItem(parent);
    itemIndices->setIcon(ColumnName, IconCache::get("index"));
    itemIndices->setText(ColumnName, tr("Indices (%1)").arg(calc_number_of_objects_by_type(objmap, "index")));
    typeToParentItem.insert({"index", itemIndices});

    QTreeWidgetItem* itemViews = new QTreeWidgetItem(parent);
    itemViews->setIcon(ColumnName, IconCache::get("view"));
    itemViews->setText(ColumnName, tr("Views (%1)").arg(calc_number_of_objects_by_type(objmap, "view")));
    typeToParentItem.insert({"view", itemViews});

    QTreeWidgetItem* itemTriggers = new QTreeWidgetItem(parent);
    itemTriggers->setIcon(ColumnName, IconCache::get("trigger"));
    itemTriggers->setText(ColumnName, tr("Triggers (%1)").arg(calc_number_of_objects_by_type(objmap, "trigger")));
    typeToParentItem.insert({"trigger", itemTriggers});

    // Get all database objects and sort them by their name.
    // This needs to be a multimap because SQLite allows views and triggers with the same name which means that names can appear twice.
    std::multimap<std::string, sqlb::ObjectPtr> dbobjs;
    for(const auto& it : objmap)
        dbobjs.insert({it.second->name(), it.second});

    // Add the database objects to the tree nodes
    for(const auto& obj : dbobjs)
    {
        sqlb::ObjectPtr it = obj.second;

        // Object node
        QTreeWidgetItem* item = addNode(typeToParentItem.at(sqlb::Object::typeToString(it->type())), it, schema);

        // If it is a table or view add the field nodes, add an extra node for the browsable section
        if(it->type() == sqlb::Object::Types::Table || it->type() == sqlb::Object::Types::View)
            addNode(browsablesRootItem, it, schema);

        // Add field nodes if there are any
        sqlb::FieldInfoList fieldList = it->fieldInformation();
        if(!fieldList.empty())
        {
            sqlb::StringVector pk_columns;
            if(it->type() == sqlb::Object::Types::Table)
            {
                const auto pk = std::dynamic_pointer_cast<sqlb::Table>(it)->primaryKey();
                if(pk)
                    pk_columns = pk->columnList();
            }

            for(const sqlb::FieldInfo& field : fieldList)
            {
                QTreeWidgetItem *fldItem = new QTreeWidgetItem(item);
                bool isFK = false;
                if(it->type() == sqlb::Object::Types::Table)
                    isFK = std::dynamic_pointer_cast<sqlb::Table>(it)->constraint({field.name}, sqlb::Constraint::ForeignKeyConstraintType) != nullptr;

                fldItem->setText(ColumnName, QString::fromStdString(field.name));
                fldItem->setText(ColumnObjectType, "field");
                fldItem->setText(ColumnDataType, QString::fromStdString(field.type));
                fldItem->setText(ColumnSQL, QString::fromStdString(field.sql));
                fldItem->setText(ColumnSchema, QString::fromStdString(schema));
                if(contains(pk_columns, field.name))
                    fldItem->setIcon(ColumnName, IconCache::get("field_key"));
                else if(isFK)
                    fldItem->setIcon(ColumnName, IconCache::get("field_fk"));
                else
                    fldItem->setIcon(ColumnName, IconCache::get("field"));
            }
        }
    }
}

QTreeWidgetItem* DbStructureModel::addNode(QTreeWidgetItem* parent, const sqlb::ObjectPtr& object, const std::string& schema)
{
    std::string type = sqlb::Object::typeToString(object->type());

    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setIcon(ColumnName, IconCache::get(type));
    item->setText(ColumnName, QString::fromStdString(object->name()));
    item->setText(ColumnObjectType, QString::fromStdString(type));
    item->setText(ColumnSQL, QString::fromStdString(object->originalSql()));
    item->setText(ColumnSchema, QString::fromStdString(schema));

    return item;
}

QString DbStructureModel::getNameForDropping(const QString& domain, const QString& object, const QString& field) const
{
    // Take into account the drag&drop options for composing a name.  Commas are included for composing a
    // list of fields. A dot is added after other items, in order to allow composing a fully qualified name
    // by selecting together and dropping a parent item and a child (e.g. select with control an attached
    // database and a table, and drag and drop them to get "schema1"."table1".)  Note that this only makes
    // sense when the "Drop Qualified Names" option is not set.
    QString name;
    if ((domain != "main" && m_dropQualifiedNames) || object.isEmpty())
        name = m_dropEnquotedNames ? sqlb::escapeIdentifier(domain) + "." : domain + ".";

    if (!object.isEmpty() && (m_dropQualifiedNames || field.isEmpty()))
        name += m_dropEnquotedNames ? sqlb::escapeIdentifier(object) + "." : object + ".";

    if (!field.isEmpty())
        name += m_dropEnquotedNames ? sqlb::escapeIdentifier(field) + ", " : field + ", ";

    return name;
}
