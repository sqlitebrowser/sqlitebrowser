#include "DiagramObjects.h"

#include <QLabel>
#include <QListView>
#include <QLineF>
#include <QVBoxLayout>
#include <QDebug>

// TableModel

TableModel::TableModel(sqlb::Table& table, QObject *parent)
    : QAbstractListModel(parent)
    , m_table(table)
{
    update();
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_fields.size();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const QString& fieldName = m_fields.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return fieldName;

    case Qt::DecorationRole:
        if (m_primaryKeys.contains(fieldName))
            return QIcon(":/icons/field_key");
        else
            return QIcon(":/icons/field");

    default:
        return QVariant();
    }
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled;
}

void TableModel::update()
{
    if (m_table.name() != m_tableName) {
        m_tableName = m_table.name();
        emit tableNameChanged(m_tableName);
    }

    m_fields = m_table.fieldNames();
    m_primaryKeys.clear();
    auto primaryKeyFields = m_table.primaryKey();
    for (auto pkf : primaryKeyFields) {
        m_primaryKeys.append(pkf->name());
    }

    removeRows(0, rowCount());
    insertRows(0, m_fields.size());

    emit dataChanged(index(0, 0), index(0, m_fields.size()));
}

QString TableModel::tableName() const
{
    return m_tableName;
}

// TableWidget

TableWidget::TableWidget(TableModel* model, QWidget *parent)
    : QWidget(parent)
    , m_tableModel(model)
{
    m_tableModel->setParent(this);

    m_listView = new QListView(this);
    m_listView->setModel(m_tableModel);
    m_label = new QLabel(m_tableModel->tableName(), this);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_label);
    layout->addWidget(m_listView);
    setLayout(layout);

    connect(m_tableModel, &TableModel::tableNameChanged, m_label, &QLabel::setText);
}

TableModel *TableWidget::tableModel() const
{
    return m_tableModel;
}

// Relation

Relation::Relation(TableWidget* fromTable, TableWidget* toTable, QObject *parent)
    : QObject(parent)
    , m_fromTable(fromTable)
    , m_toTable(toTable)
{
    m_line = new QLineF();
}

Relation::~Relation()
{
    delete m_line;
}

QString Relation::tooltipText() const
{
    // ToDo: show informative tooltips, e.g:
    // foo(bar_id) references bar(id)
    return QString();
}
