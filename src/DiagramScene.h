#ifndef DBDIAGRAM_H
#define DBDIAGRAM_H

#include <QGraphicsScene>

class DBBrowserDB;
class TableWidget;
class Relation;

class DiagramScene : public QGraphicsScene
{
public:
    explicit DiagramScene(const DBBrowserDB& db, QWidget* parent = nullptr);

public slots:
    void updateTables();
    void removeTable(const std::string& tableName);

    // ToDo: implement connections
    // void addConnection(const QString& fromTable, const QString& toTable);
    // void removeConnection(const QString& fromTable, const QString& toTable);

private:
    void addTable(const std::string& tableName);

    QList<TableWidget*> m_tables;
    QList<Relation> m_relations;

    // it won't be const when diagrams would allow to edit DB
    // in the (near) future :)
    const DBBrowserDB& m_db;
};

#endif
