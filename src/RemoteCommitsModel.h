#ifndef REMOTECOMMITSMODEL_H
#define REMOTECOMMITSMODEL_H

#include <QAbstractItemModel>

#include <json.hpp>

class QTreeWidgetItem;

class RemoteCommitsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit RemoteCommitsModel(QObject* parent);
    ~RemoteCommitsModel() override;

    void clear();
    void refresh(const std::string& json_data, const std::string& last_commit_id);

    QModelIndex index(int row, int column,const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;

    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    enum Columns
    {
        ColumnCommitId,
        ColumnMessage,
        ColumnDate,
        ColumnAuthor,
    };

private:
    // Pointer to the root item. This contains all the actual item data.
    QTreeWidgetItem* rootItem;
};

#endif
