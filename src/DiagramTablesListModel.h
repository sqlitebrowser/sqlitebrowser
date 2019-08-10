#ifndef DIAGRAMTABLESLISTMODEL_H
#define DIAGRAMTABLESLISTMODEL_H

#include "sqlitedb.h"

#include <QAbstractListModel>

class DiagramTablesListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit DiagramTablesListModel(const DBBrowserDB& db, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

public slots:
    void update();

private:
    struct Table
    {
        explicit Table(const QString& table_name)
            : name(table_name)
        { }

        QString name;
        bool isDisplayed = false;
        // ToDo: tables, displayed on diagram, are disabled & can't be dragged
    };

    QList<Table> m_tablesList;
    const DBBrowserDB& m_db;
};

#endif
