#include <QImage>

#include "Data.h"
#include "RemoteModel.h"
#include "RemoteNetwork.h"

using json = nlohmann::json;

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
    m_children.push_back(item);
}

RemoteModelItem* RemoteModelItem::child(int row) const
{
    return m_children[static_cast<size_t>(row)];
}

RemoteModelItem* RemoteModelItem::parent() const
{
    return m_parent;
}

int RemoteModelItem::childCount() const
{
    return static_cast<int>(m_children.size());
}

int RemoteModelItem::row() const
{
    if(m_parent)
    {
        auto f = std::find( m_parent->m_children.begin(), m_parent->m_children.end(), const_cast<RemoteModelItem*>(this));
        if(f == m_parent->m_children.end())
            return -1;
        else
            return static_cast<int>(std::distance(m_parent->m_children.begin(), f));
    }

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

std::vector<RemoteModelItem*> RemoteModelItem::loadArray(const json& array, RemoteModelItem* parent)
{
    std::vector<RemoteModelItem*> items;

    // Loop through all directory items
    for(const auto& elem : array)
    {
        // Create a new model item with the specified parent
        RemoteModelItem* item = new RemoteModelItem(parent);

        // Save all relevant values. If one of the values isn't set in the JSON document, an empty string
        // will be stored
        item->setValue(RemoteModelColumnName, QString::fromStdString(elem["name"]));
        item->setValue(RemoteModelColumnType, QString::fromStdString(elem["type"]));
        item->setValue(RemoteModelColumnUrl, QString::fromStdString(elem["url"]));
        item->setValue(RemoteModelColumnLastModified, isoDateTimeStringToLocalDateTimeString(QString::fromStdString(elem["last_modified"])));
        if(elem.contains("commit_id"))
            item->setValue(RemoteModelColumnCommitId, QString::fromStdString(elem["commit_id"]));
        if(elem.contains("size"))
            item->setValue(RemoteModelColumnSize, QString::number(static_cast<unsigned long>(elem["size"])));

        items.push_back(item);
    }

    return items;
}

RemoteModel::RemoteModel(QObject* parent) :
    QAbstractItemModel(parent),
    headerList({tr("Name"), tr("Last modified"), tr("Size"), tr("Commit")}),
    rootItem(new RemoteModelItem())
{
}

RemoteModel::~RemoteModel()
{
    delete rootItem;
}

void RemoteModel::setNewRootDir(const QString& url, const QString& cert)
{
    // Get user name from client cert
    currentUserName = RemoteNetwork::get().getInfoFromClientCert(cert, RemoteNetwork::CertInfoUser);

    // Save settings
    currentRootDirectory = url;
    currentClientCert = cert;

    // Fetch root directory
    refresh();
}

void RemoteModel::refresh()
{
    // Fetch root directory and put the reply data under the root item
    RemoteNetwork::get().fetch(currentRootDirectory, RemoteNetwork::RequestTypeCustom, currentClientCert, [this](const QByteArray& reply) {
        parseDirectoryListing(reply, QModelIndex());
    });
}

void RemoteModel::parseDirectoryListing(const QString& text, QModelIndex parent)
{
    // Load new JSON root document assuming it's an array
    json array = json::parse(text.toStdString(), nullptr, false);
    if(array.is_discarded() || !array.is_array())
        return;

    // Get model index to store the new data under
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
    std::vector<RemoteModelItem*> items = RemoteModelItem::loadArray(array, parentItem);
    beginInsertRows(parent, 0, static_cast<int>(items.size() - 1));
    for(RemoteModelItem* item : items)
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
                return item->value(RemoteModelColumnLastModified);
            }
        case 2:
            {
                // Folders don't have a size
                if(type == "folder")
                    return QVariant();

                // Convert size to human readable format
                unsigned int size = item->value(RemoteModelColumnSize).toUInt();
                return humanReadableSize(size);
            }
        case 3:
            {
                if(type == "folder")
                    return QVariant();
                return item->value(RemoteModelColumnCommitId);
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
    return headerList.at(static_cast<size_t>(section));
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
    return static_cast<int>(headerList.size());
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
    RemoteNetwork::get().fetch(item->value(RemoteModelColumnUrl).toUrl(), RemoteNetwork::RequestTypeCustom, currentClientCert, [this, parent](const QByteArray& reply) {
        parseDirectoryListing(reply, parent);
    });
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
