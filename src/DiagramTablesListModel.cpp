#include "DiagramTablesListModel.h"

DiagramTablesListModel::DiagramTablesListModel(const DBBrowserDB &db, QObject *parent)
    : QAbstractListModel(parent)
    , m_db(db)
{
}

int DiagramTablesListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_tablesList.count();
}

QVariant DiagramTablesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Table& table = m_tablesList.at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        return table.name;

    default:
        return QVariant();
    }
}

Qt::ItemFlags DiagramTablesListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags;

    if (!m_tablesList.at(index.row()).isDisplayed)
        flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;

    return flags;
}

void DiagramTablesListModel::update()
{
    removeRows(0, rowCount());
    m_tablesList.clear();

    objectMap objects = m_db.getBrowsableObjects("main");

    for(const auto& obj : objects)
    {
        if(obj.second->type() == sqlb::Object::Table)
            m_tablesList.push_back(Table(QString::fromStdString(obj.second->name())));
    }

    insertRows(0, m_tablesList.size());

    emit dataChanged(index(0, 0), index(0, m_tablesList.size()));
}
