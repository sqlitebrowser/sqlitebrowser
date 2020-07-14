#include <QTreeWidgetItem>

#include "Data.h"
#include "RemoteCommitsModel.h"
#include "Settings.h"

using json = nlohmann::json;

RemoteCommitsModel::RemoteCommitsModel(QObject* parent) :
    QAbstractItemModel(parent)
{
    QStringList header;
    header << tr("Commit ID") << tr("Message") << tr("Date") << tr("Author");
    rootItem = new QTreeWidgetItem(header);
}

RemoteCommitsModel::~RemoteCommitsModel()
{
    delete rootItem;
}

void RemoteCommitsModel::clear()
{
    beginResetModel();

    // Remove all data except for the root item
    while(rootItem->childCount())
        delete rootItem->child(0);

    endResetModel();
}

void RemoteCommitsModel::refresh(const std::string& json_data, const std::string& last_commit_id)
{
    // Clear previous items
    clear();
    beginResetModel();

    // Read in all commits
    json j = json::parse(json_data, nullptr, false);
    std::unordered_map<std::string, json::iterator> commit_to_iterator;
    for(auto it=j.begin();it!=j.end();++it)
        commit_to_iterator.insert({it.value()["id"], it});

    // Starting from the last commit add follow the chain up to the first commit
    std::string parent_id = last_commit_id;
    while(true)
    {
        if(commit_to_iterator.find(parent_id) == commit_to_iterator.end())
            break;

        json commit = commit_to_iterator[parent_id].value();

        QTreeWidgetItem* item = new QTreeWidgetItem(rootItem);
        item->setText(ColumnCommitId, QString::fromStdString(commit["id"]));
        item->setText(ColumnMessage, QString::fromStdString(commit["message"]));
        item->setText(ColumnDate, isoDateTimeStringToLocalDateTimeString(QString::fromStdString(commit["timestamp"])));
        item->setText(ColumnAuthor, QString::fromStdString(commit["author_name"]) + " <" + QString::fromStdString(commit["author_email"]) + ">");

        parent_id = commit["parent"];
    }

    // Refresh the view
    endResetModel();
}

QModelIndex RemoteCommitsModel::index(int row, int column, const QModelIndex& parent) const
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

QModelIndex RemoteCommitsModel::parent(const QModelIndex& index) const
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

QVariant RemoteCommitsModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    // Get the item the index points at
    QTreeWidgetItem* item = static_cast<QTreeWidgetItem*>(index.internalPointer());

    // Return data depending on the role
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return item->text(index.column());
    default:
        return QVariant();
    }
}

QVariant RemoteCommitsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // Get the header string from the root item
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section, role);

    return QVariant();
}

int RemoteCommitsModel::rowCount(const QModelIndex& parent) const
{
    if(parent.column() > 0)
        return 0;

    if(!parent.isValid())
        return rootItem->childCount();
    else
        return static_cast<QTreeWidgetItem*>(parent.internalPointer())->childCount();
}

int RemoteCommitsModel::columnCount(const QModelIndex& /*parent*/) const
{
    return rootItem->columnCount();
}
