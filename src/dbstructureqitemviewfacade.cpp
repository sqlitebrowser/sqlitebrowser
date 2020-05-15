#include "DbStructureModel.h"
#include "dbstructureqitemviewfacade.h"

DbStructureQItemViewFacade::DbStructureQItemViewFacade(QAbstractItemView& aitemView)
    : QObject(&aitemView),
      m_itemView(aitemView)
{

}

const QAbstractItemView& DbStructureQItemViewFacade::itemView() const
{
    return m_itemView;
}

bool DbStructureQItemViewFacade::hasSelection() const
{
    return m_itemView.selectionModel()->hasSelection();
}

QString DbStructureQItemViewFacade::displayName() const
{
    QAbstractItemModel *model = m_itemView.model();
    QModelIndex index = m_itemView.currentIndex();
    return model->data(index.sibling(index.row(), DbStructureModel::ColumnName), Qt::DisplayRole).toString();
}

QString DbStructureQItemViewFacade::name() const
{
    QAbstractItemModel *model = m_itemView.model();
    QModelIndex index = m_itemView.currentIndex();
    return model->data(index.sibling(index.row(), DbStructureModel::ColumnName), Qt::EditRole).toString();
}

QString DbStructureQItemViewFacade::objectType() const
{
    QAbstractItemModel *model = m_itemView.model();
    QModelIndex index = m_itemView.currentIndex();
    return model->data(index.sibling(index.row(), DbStructureModel::ColumnObjectType), Qt::EditRole).toString();
}

QString DbStructureQItemViewFacade::schema() const
{
    QAbstractItemModel *model = m_itemView.model();
    QModelIndex index = m_itemView.currentIndex();
    return model->data(index.sibling(index.row(), DbStructureModel::ColumnSchema), Qt::EditRole).toString();
}

QString DbStructureQItemViewFacade::sql() const
{
    QAbstractItemModel *model = m_itemView.model();
    QModelIndex index = m_itemView.currentIndex();
    return model->data(index.sibling(index.row(), DbStructureModel::ColumnSQL), Qt::EditRole).toString();
}

sqlb::ObjectIdentifier DbStructureQItemViewFacade::object() const
{
    return sqlb::ObjectIdentifier(schema().toStdString(), name().toStdString());
}
