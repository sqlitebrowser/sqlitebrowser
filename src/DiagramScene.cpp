#include "sqlitedb.h"
#include "DiagramScene.h"
#include "DiagramObjects.h"

#include <algorithm>

#include <QGraphicsProxyWidget>

class MovableProxyWidget : public QGraphicsProxyWidget
{
    Q_OBJECT

#define OVERRIDE_MOUSE_EVENT(event_type) \
    void event_type(QGraphicsSceneMouseEvent* event) override \
    { return QGraphicsItem::event_type(event); }

public:
    MovableProxyWidget(QGraphicsItem* parent = nullptr)
        : QGraphicsProxyWidget(parent)
    {
        setFlags(flags() | ItemIsMovable | ItemIsSelectable);
    }

    ~MovableProxyWidget() override {}

    OVERRIDE_MOUSE_EVENT(mouseDoubleClickEvent)
    OVERRIDE_MOUSE_EVENT(mouseMoveEvent)
    OVERRIDE_MOUSE_EVENT(mousePressEvent)
    OVERRIDE_MOUSE_EVENT(mouseReleaseEvent)
};

DiagramScene::DiagramScene(const DBBrowserDB& db, QWidget *parent)
    : QGraphicsScene(parent)
    , m_db(db)
{

}

void DiagramScene::updateTables()
{
    for (TableWidget* tw : m_tables) {
        tw->tableModel()->update();
    }
}

void DiagramScene::removeTable(const std::string& tableName)
{
    std::remove_if(m_tables.begin(), m_tables.end(), [&](TableWidget* wgt)
    {
        return (wgt->tableModel()->tableName() == tableName);
    });
}

void DiagramScene::addTable(const std::string& tableName)
{
    sqlb::TablePtr obj = m_db.getObjectByName<sqlb::Table>(sqlb::ObjectIdentifier("main", tableName));
    TableModel* model = new TableModel(*obj);
    TableWidget* widget = new TableWidget(model);
    m_tables.push_back(widget);

    MovableProxyWidget* proxy = new MovableProxyWidget;
    proxy->setWidget(widget);
    addItem(proxy);
}

#include "DiagramScene.moc"
