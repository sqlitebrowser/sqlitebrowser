#ifndef DIAGRAMTABLESLISTMODEL_H
#define DIAGRAMTABLESLISTMODEL_H

#include "sqlitedb.h"

#include <QAbstractListModel>

class DiagramTablesListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    DiagramTablesListModel(const DBBrowserDB& db, QObject* parent = Q_NULLPTR);

    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;

public slots:
    void update();

private:
    struct Table
    {
        Table(const QString& table_name)
            : name(table_name)
        { }

        QString name;
        bool isDisplayed = false;
        // ToDo: tables, displayed on diagram, are disabled & can't be dragged
    };

    QList<Table> m_tablesList;
    const DBBrowserDB& m_db;
};


#endif // DIAGRAMTABLESLISTMODEL_H
