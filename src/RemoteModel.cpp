#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QImage>

#include "RemoteModel.h"
#include "RemoteDatabase.h"

RemoteModelItem::RemoteModelItem(RemoteModelItem* parent) :
    m_parent(parent),
    m_fetchedDirectoryList(false)
{
}

RemoteModelItem::~RemoteModelItem()
{
    qDeleteAll(m_children);
}

QVariant RemoteModelItem::value(RemoteModelColumns column) const
{
    return m_values[column];
}

void RemoteModelItem::setValue(RemoteModelColumns column, QVariant value)
{
    m_values[column] = value;
}

void RemoteModelItem::appendChild(RemoteModelItem *item)
{
    m_children.append(item);
}

RemoteModelItem* RemoteModelItem::child(int row) const
{
    return m_children.value(row);
}

RemoteModelItem* RemoteModelItem::parent() const
{
    return m_parent;
}

int RemoteModelItem::childCount() const
{
    return m_children.count();
}

int RemoteModelItem::row() const
{
    if(m_parent)
        return m_parent->m_children.indexOf(const_cast<RemoteModelItem*>(this));

    return 0;
}

bool RemoteModelItem::fetchedDirectoryList() const
{
    return m_fetchedDirectoryList;
}

void RemoteModelItem::setFetchedDirectoryList(bool fetched)
{
    m_fetchedDirectoryList = fetched;
}

QList<RemoteModelItem*> RemoteModelItem::loadArray(const QJsonValue& value, RemoteModelItem* parent)
{
    QList<RemoteModelItem*> items;

    // Loop through all directory items
    QJsonArray array = value.toArray();
    for(int i=0;i<array.size();i++)
    {
        // Create a new model item with the specified parent
        RemoteModelItem* item = new RemoteModelItem(parent);

        // Save all relevant values. If one of the values isn't set in the JSON document, an empty string
        // will be stored
        item->setValue(RemoteModelColumnName, array.at(i).toObject().value("name"));
        item->setValue(RemoteModelColumnType, array.at(i).toObject().value("type"));
        item->setValue(RemoteModelColumnUrl, array.at(i).toObject().value("url"));
        item->setValue(RemoteModelColumnCommitId, array.at(i).toObject().value("commit_id"));
        item->setValue(RemoteModelColumnSize, array.at(i).toObject().value("size"));
        item->setValue(RemoteModelColumnLastModified, array.at(i).toObject().value("last_modified"));

        items.push_back(item);
    }

    return items;
}

RemoteModel::RemoteModel(QObject* parent, RemoteDatabase& remote) :
    QAbstractItemModel(parent),
    rootItem(new RemoteModelItem()),
    remoteDatabase(remote)
{
    // Initialise list of column names
    headerList << tr("Name") << tr("Commit") << tr("Last modified") << tr("Size");

    // Set up signals
    connect(&remoteDatabase, &RemoteDatabase::gotDirList, this, &RemoteModel::parseDirectoryListing);
}

RemoteModel::~RemoteModel()
{
    delete rootItem;
}

void RemoteModel::setNewRootDir(const QString& url, const QString& cert)
{
    // Get user name from client cert
    currentUserName = remoteDatabase.getInfoFromClientCert(cert, RemoteDatabase::CertInfoUser);

    // Save settings
    currentRootDirectory = url;
    currentClientCert = cert;

    // Fetch root directory and put the reply data under the root item
    remoteDatabase.fetch(currentRootDirectory, RemoteDatabase::RequestTypeDirectory, currentClientCert, QModelIndex());
}

void RemoteModel::parseDirectoryListing(const QString& json, const QVariant& userdata)
{
    // Load new JSON root document assuming it's an array
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if(doc.isNull() || !doc.isArray())
        return;
    QJsonArray array = doc.array();

    // Get model index to store the new data under
    QModelIndex parent = userdata.toModelIndex();
    RemoteModelItem* parentItem = const_cast<RemoteModelItem*>(modelIndexToItem(parent));

    // An invalid model index indicates that this is a new root item. This means the old one needs to be entirely deleted first.
    if(!parent.isValid())
    {
        // Clear root item
        beginResetModel();
        delete rootItem;
        rootItem = new RemoteModelItem();
        endResetModel();

        // Set parent model index and parent item to the new values
        parent = QModelIndex();
        parentItem = rootItem;
    }

    // Insert data
    beginInsertRows(parent, 0, array.size());
    QList<RemoteModelItem*> items = RemoteModelItem::loadArray(QJsonValue(array), parentItem);
    foreach(RemoteModelItem* item, items)
        parentItem->appendChild(item);
    endInsertRows();

    // Emit directory listing parsed signal
    emit directoryListingParsed(parent);
}

QModelIndex RemoteModel::index(int row, int column, const QModelIndex& parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    const RemoteModelItem* parentItem = modelIndexToItem(parent);
    RemoteModelItem* childItem = parentItem->child(row);

    if(childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex RemoteModel::parent(const QModelIndex& index) const
{
    if(!index.isValid())
        return QModelIndex();

    const RemoteModelItem* childItem = modelIndexToItem(index);
    RemoteModelItem* parentItem = childItem->parent();

    if(parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

QVariant RemoteModel::data(const QModelIndex& index, int role) const
{
    // Don't return data for invalid indices
    if(!index.isValid())
        return QVariant();

    // Type of item
    const RemoteModelItem* item = modelIndexToItem(index);
    QString type = item->value(RemoteModelColumnType).toString();

    // Decoration role? Only for first column!
    if(role == Qt::DecorationRole && index.column() == 0)
    {
        // Use different icons depending on item type
        if(type == "folder" && index.parent() == QModelIndex() && item->value(RemoteModelColumnName) == currentUserName)
            return QImage(":/icons/folder_user");
        else if(type == "folder")
            return QImage(":/icons/folder");
        else if(type == "database")
            return QImage(":/icons/database");
    } else if(role == Qt::DisplayRole) {
        // Display role?

        // Return different value depending on column
        switch(index.column())
        {
        case 0:
            {
                return item->value(RemoteModelColumnName);
            }
        case 1:
            {
                if(type == "folder")
                    return QVariant();
                return item->value(RemoteModelColumnCommitId);
            }
        case 2:
            {
                return item->value(RemoteModelColumnLastModified);
            }
        case 3:
            {
                // Folders don't have a size
                if(type == "folder")
                    return QVariant();

                // Convert size to human readable format
                float size = item->value(RemoteModelColumnSize).toLongLong();
                QStringList list;
                list << "KiB" << "MiB" << "GiB" << "TiB";
                QStringListIterator it(list);
                QString unit(tr("bytes"));
                while(size >= 1024.0f && it.hasNext())
                {
                    unit = it.next();
                    size /= 1024.0;
                }
                return QString().setNum(size, 'f', 2).remove(".00") + " " + unit;
            }
        }
    }

    return QVariant();
}

QVariant RemoteModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // Call default implementation for vertical headers and for non-display roles
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QAbstractItemModel::headerData(section, orientation, role);

    // Return header string depending on column
    return headerList.at(section);
}

int RemoteModel::rowCount(const QModelIndex& parent) const
{
    if(parent.column() > 0)
        return 0;

    const RemoteModelItem* parentItem = modelIndexToItem(parent);
    return parentItem->childCount();
}

int RemoteModel::columnCount(const QModelIndex& /*parent*/) const
{
    return headerList.size();
}

bool RemoteModel::hasChildren(const QModelIndex& parent) const
{
    if(!parent.isValid())
        return true;

    // If the item actually has children or is of type "folder" (and may have no children yet), we say that it actually has children
    const RemoteModelItem* item = modelIndexToItem(parent);
    return item->childCount() || item->value(RemoteModelColumnType) == "folder";
}

bool RemoteModel::canFetchMore(const QModelIndex& parent) const
{
    if(!parent.isValid())
        return false;

    // If the item is of type "folder" and we haven't tried fetching a directory listing yet, we indicate that there might be more data to load
    const RemoteModelItem* item = modelIndexToItem(parent);
    return item->value(RemoteModelColumnType) == "folder" && !item->fetchedDirectoryList();
}

void RemoteModel::fetchMore(const QModelIndex& parent)
{
    // Can we even fetch more data?
    if(!canFetchMore(parent))
       return;

    // Get parent item
    RemoteModelItem* item = static_cast<RemoteModelItem*>(parent.internalPointer());

    // Fetch item URL
    item->setFetchedDirectoryList(true);
    remoteDatabase.fetch(item->value(RemoteModelColumnUrl).toString(), RemoteDatabase::RequestTypeDirectory, currentClientCert, parent);
}

const QString& RemoteModel::currentClientCertificate() const
{
    return currentClientCert;
}

const RemoteModelItem* RemoteModel::modelIndexToItem(const QModelIndex& idx) const
{
    if(!idx.isValid())
        return rootItem;
    else
        return static_cast<const RemoteModelItem*>(idx.internalPointer());
}
