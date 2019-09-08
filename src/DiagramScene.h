#ifndef DBDIAGRAM_H
#define DBDIAGRAM_H

#include <QGraphicsScene>

class DBBrowserDB;
class TableWidget;
class Relation;
class TableProxy;

class DiagramScene : public QGraphicsScene
{
public:
    explicit DiagramScene(const DBBrowserDB& db, QWidget* parent = nullptr);
    void addTable(const std::string& tableName);

public slots:
    void updateTables();
    void removeTable(const std::string& tableName);

private:

    void addRelation(Relation* rel);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    QList<TableProxy*> m_tables;
    QList<Relation*> m_relations;

    // it won't be const when diagrams would allow to edit DB
    // in the (near) future :)
    const DBBrowserDB& m_db;
};

#endif
