#include "sqlitedb.h"
#include "DiagramScene.h"
#include "DiagramObjects.h"

#include <algorithm>

#include <QGraphicsProxyWidget>
#include <QDebug>

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

void DiagramScene::removeTable(const QString& tableName)
{
    std::remove_if(m_tables.begin(), m_tables.end(), [&](TableWidget* wgt)
    {
        return (wgt->tableModel()->tableName() == tableName);
    });
}

void DiagramScene::addTable(const QString& tableName)
{
    DBBrowserObject obj = m_db.getObjectByName(tableName);
    TableModel* model = new TableModel(obj.table);
    TableWidget* widget = new TableWidget(model);
    m_tables.push_back(widget);

    addWidget(widget);
}
