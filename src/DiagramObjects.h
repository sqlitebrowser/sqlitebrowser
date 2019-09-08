#ifndef DIAGRAMOBJECTS_H
#define DIAGRAMOBJECTS_H

#include "sql/sqlitetypes.h"

#include <QAbstractListModel>
#include <QGraphicsProxyWidget>
#include <QMap>
#include <QWidget>
#include <QLineF>

#include <string>
#include <vector>

class QLabel;
class QListView;
class QLineF;
class QGraphicsLineItem;

class TableModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TableModel(sqlb::Table& table, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void update();
    std::string tableName() const;

signals:
    void tableNameChanged(QString);

private:
    std::vector<std::string> m_primaryKeys;
    std::vector<std::string> m_fields;
    std::string m_tableName;

    sqlb::Table& m_table;
};

class TableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TableWidget(TableModel* model, QWidget* parent = nullptr);
    TableModel *tableModel() const;
    QSize sizeHint() const override;

private:
    TableModel* m_tableModel;
    QListView* m_listView;
    QLabel* m_label;
};

class TableProxy : public QGraphicsProxyWidget
{
    Q_OBJECT

#define OVERRIDE_MOUSE_EVENT(event_type) \
    void event_type(QGraphicsSceneMouseEvent* event) override \
    { return QGraphicsItem::event_type(event); }

public:
    TableProxy(QGraphicsItem* parent = nullptr)
        : QGraphicsProxyWidget(parent)
    {
        setFlags(flags() | ItemIsMovable | ItemIsSelectable);
    }

    TableWidget* tableWidget() const
    {
        return static_cast<TableWidget*>(widget());
    }

    ~TableProxy() override {}

    OVERRIDE_MOUSE_EVENT(mouseDoubleClickEvent)
    OVERRIDE_MOUSE_EVENT(mouseMoveEvent)
    OVERRIDE_MOUSE_EVENT(mousePressEvent)
    OVERRIDE_MOUSE_EVENT(mouseReleaseEvent)
};

class Relation : public QObject
{
    Q_OBJECT

public:
    Relation(TableProxy* parentTable, TableProxy* childTable);
    ~Relation();

    TableProxy* parentTable() const { return m_parentTable; };
    TableProxy* childTable() const { return m_childTable; };

    void setLineItem(QGraphicsLineItem* item) { m_lineItem = item; };
    QGraphicsLineItem* lineItem() const { return m_lineItem; };
    QLineF line() const { return QLineF(m_parentTable->scenePos(),
                                        m_childTable->scenePos()); };

    QString tooltipText() const;

private:
    TableProxy *m_parentTable, *m_childTable;
    QGraphicsLineItem* m_lineItem;
};

#endif
