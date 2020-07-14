#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QTreeWidgetItem>
#include <QUrl>

#include "Data.h"
#include "RemoteLocalFilesModel.h"
#include "RemoteDatabase.h"
#include "Settings.h"

using json = nlohmann::json;

RemoteLocalFilesModel::RemoteLocalFilesModel(QObject* parent, RemoteDatabase& remote) :
    QAbstractItemModel(parent),
    remoteDatabase(remote)
{
    QStringList header;
    header << tr("Name") << tr("Branch") << tr("Last modified") << tr("Size") << tr("Commit") << tr("File");
    rootItem = new QTreeWidgetItem(header);
}

RemoteLocalFilesModel::~RemoteLocalFilesModel()
{
    delete rootItem;
}

void RemoteLocalFilesModel::setIdentity(const QString& cert_filename)
{
    current_cert_filename = cert_filename;
    current_user_name = remoteDatabase.getInfoFromClientCert(cert_filename, RemoteDatabase::CertInfoUser);
    refresh();
}

void RemoteLocalFilesModel::refresh()
{
    beginResetModel();

    // Remove all data except for the root item
    while(rootItem->childCount())
        delete rootItem->child(0);

    // Get list of locally checked out databases
    auto files = remoteDatabase.localGetLocalFiles(current_cert_filename);

    // Loop through that list
    for(const auto& file : files)
    {
        QString user_name = file.user_name();

        // Check if there is already a node for this user
        QTreeWidgetItem* user_node = nullptr;
        for(int i=0;i<rootItem->childCount();i++)
        {
            if(rootItem->child(i)->text(ColumnName) == user_name)
            {
                    user_node = rootItem->child(i);
                    break;
            }
        }

        // If there is no node for this user yet create one
        if(user_node == nullptr)
        {
            user_node = new QTreeWidgetItem(rootItem);
            user_node->setText(ColumnName, user_name);
            user_node->setIcon(ColumnName, QIcon(user_name == current_user_name ? ":/icons/folder_user" : ":/icons/folder"));
        }

        // Get file information
        QFile file_info(Settings::getValue("remote", "clonedirectory").toString() + "/" + QString::fromStdString(file.file));

        // Add file to user node
        QTreeWidgetItem* file_node = new QTreeWidgetItem(user_node);
        file_node->setText(ColumnName, QString::fromStdString(file.name));
        file_node->setIcon(ColumnName, QIcon(":/icons/database"));
        file_node->setText(ColumnBranch, QString::fromStdString(file.branch));
        file_node->setText(ColumnLastModified, QLocale::system().toString(QFileInfo(file_info).lastModified().toLocalTime(), QLocale::ShortFormat));
        file_node->setText(ColumnSize, humanReadableSize(static_cast<unsigned long>(file_info.size())));
        file_node->setText(ColumnCommit, QString::fromStdString(file.commit_id));
        file_node->setText(ColumnFile, QString::fromStdString(file.file));
    }

    // Refresh the view
    endResetModel();
}

QModelIndex RemoteLocalFilesModel::index(int row, int column, const QModelIndex& parent) const
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

QModelIndex RemoteLocalFilesModel::parent(const QModelIndex& index) const
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

QVariant RemoteLocalFilesModel::data(const QModelIndex& index, int role) const
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
    case Qt::DecorationRole:
        return item->icon(index.column());
    default:
        return QVariant();
    }
}

QVariant RemoteLocalFilesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // Get the header string from the root item
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section, role);

    return QVariant();
}

int RemoteLocalFilesModel::rowCount(const QModelIndex& parent) const
{
    if(parent.column() > 0)
        return 0;

    if(!parent.isValid())
        return rootItem->childCount();
    else
        return static_cast<QTreeWidgetItem*>(parent.internalPointer())->childCount();
}

int RemoteLocalFilesModel::columnCount(const QModelIndex& /*parent*/) const
{
    return rootItem->columnCount();
}
