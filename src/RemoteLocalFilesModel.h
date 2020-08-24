#ifndef REMOTELOCALFILESMODEL_H
#define REMOTELOCALFILESMODEL_H

#include <QAbstractItemModel>

#include <json.hpp>

class RemoteDatabase;

class QTreeWidgetItem;

class RemoteLocalFilesModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit RemoteLocalFilesModel(QObject* parent, RemoteDatabase& remote);
    ~RemoteLocalFilesModel() override;

    void setIdentity(const QString& cert_filename);
    void refresh();

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;

    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

    enum Columns
    {
        ColumnName,
        ColumnBranch,
        ColumnLastModified,
        ColumnSize,
        ColumnCommit,
        ColumnFile,
    };

private:
    // Pointer to the root item. This contains all the actual item data.
    QTreeWidgetItem* rootItem;

    // Reference to the remote database object which is stored somewhere in the main window.
    RemoteDatabase& remoteDatabase;

    // This stores the currently used network identity for further requests
    QString current_cert_filename;
    QString current_user_name;
};

#endif
