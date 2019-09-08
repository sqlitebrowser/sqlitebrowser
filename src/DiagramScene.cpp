#include "sqlitedb.h"
#include "DiagramScene.h"
#include "DiagramObjects.h"

#include <algorithm>

#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>


DiagramScene::DiagramScene(const DBBrowserDB& db, QWidget *parent)
    : QGraphicsScene(parent)
    , m_db(db)
{

}

void DiagramScene::updateTables()
{
    for (TableProxy* proxy : m_tables) {
        proxy->tableWidget()->tableModel()->update();
    }
}

void DiagramScene::removeTable(const std::string& tableName)
{
    std::remove_if(m_tables.begin(), m_tables.end(), [&](TableProxy* proxy)
    {
        return (proxy->tableWidget()->tableModel()->tableName() == tableName);
    });
}

void DiagramScene::addTable(const std::string& tableName)
{
    sqlb::TablePtr obj = m_db.getObjectByName<sqlb::Table>(sqlb::ObjectIdentifier("main", tableName));
    TableModel* model = new TableModel(*obj);
    TableWidget* widget = new TableWidget(model);

    TableProxy* proxy = new TableProxy;
    proxy->setWidget(widget);
    addItem(proxy);
    m_tables.push_back(proxy);


    // Add relations to parent tables
    for (std::string field : obj->fieldNames()) {

        auto fk = std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(obj->constraint({field}, sqlb::Constraint::ForeignKeyConstraintType));

        if (fk != nullptr) {
            for (TableProxy* otherProxy : m_tables) {
                if (otherProxy->tableWidget()->tableModel()->tableName() == fk->table()) {
                    Relation* relation = new Relation(otherProxy, proxy);
                    addRelation(relation);
                }
            }
        }
    }

    // Add relations to child tables
    for (TableProxy* otherProxy : m_tables) {
        sqlb::TablePtr otherTable =
            m_db.getObjectByName<sqlb::Table>(sqlb::ObjectIdentifier("main", otherProxy->tableWidget()->tableModel()->tableName()));

        for (std::string field : otherTable->fieldNames()) {

            auto fk = std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(otherTable->constraint({field}, sqlb::Constraint::ForeignKeyConstraintType));

            if (fk != nullptr) {
                if (tableName == fk->table()) {
                    Relation* relation = new Relation(proxy, otherProxy);
                    addRelation(relation);
                }
            }
        }
    }
}

void DiagramScene::addRelation(Relation* rel)
{
    m_relations.push_back(rel);
    rel->setLineItem(addLine(rel->line()));
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseMoveEvent(event);

    // Move relation lines
    for (Relation* rel : m_relations) {
        for (TableProxy* proxy : m_tables) {
            QLineF line = rel->lineItem()->line();
            // Move start point, if from-table has moved.
            if (proxy == rel->parentTable() && proxy->scenePos() != line.p1()) {
                line.setP1(proxy->scenePos());
            }
            // Move end point, if to-table has moved.
            if (proxy == rel->childTable() && proxy->scenePos() != line.p2()) {
                line.setP2(proxy->scenePos());
            }
            rel->lineItem()->setLine(line);
        }
    }
}

