#include "DiagramObjects.h"
#include "IconCache.h"

#include <QLabel>
#include <QListView>
#include <QLineF>
#include <QVBoxLayout>

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
    return static_cast<int>(m_fields.size());
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const std::string& fieldName = m_fields.at(static_cast<size_t>(index.row()));

    switch (role) {
    case Qt::DisplayRole:
        return QString::fromStdString(fieldName);

    case Qt::DecorationRole:
        if (std::find(m_primaryKeys.begin(), m_primaryKeys.end(), fieldName) != m_primaryKeys.end())
            return IconCache::get("field_key");
        else if (m_table.constraint({fieldName}, sqlb::Constraint::ForeignKeyConstraintType) != nullptr)
            return IconCache::get("field_fk");
        else
            return IconCache::get("field");

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
        emit tableNameChanged(QString::fromStdString(m_tableName));
    }

    m_fields = m_table.fieldNames();
    m_primaryKeys = m_table.primaryKey();

    removeRows(0, rowCount());
    insertRows(0, static_cast<int>(m_fields.size()));

    emit dataChanged(index(0, 0), index(0, static_cast<int>(m_fields.size())));
}

std::string TableModel::tableName() const
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
    m_label = new QLabel(QString::fromStdString(m_tableModel->tableName()), this);

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
