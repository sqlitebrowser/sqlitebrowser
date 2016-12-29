#ifndef DIAGRAMOBJECTS_H
#define DIAGRAMOBJECTS_H

#include "sqlitetypes.h"

#include <QAbstractListModel>
#include <QWidget>
#include <QMap>

class QLabel;
class QListView;
class QLineF;

class TableModel : public QAbstractListModel
{
    Q_OBJECT

public:
    TableModel(sqlb::Table& table, QObject* parent = Q_NULLPTR);

    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;

    void update();
    QString tableName() const;

signals:
    void tableNameChanged(QString);

private:
    QStringList m_primaryKeys;
    QStringList m_fields;
    QString m_tableName;

    sqlb::Table& m_table;
};

class TableWidget : public QWidget
{
    Q_OBJECT

public:
    TableWidget(TableModel* model, QWidget* parent = Q_NULLPTR);
    TableModel *tableModel() const;

private:
    TableModel* m_tableModel;
    QListView* m_listView;
    QLabel* m_label;
};

class Relation : public QObject
{
    Q_OBJECT

public:
    Relation(TableWidget* fromTable, TableWidget* toTable, QObject* parent = Q_NULLPTR);
    ~Relation();

    QString tooltipText() const;

private:
    TableWidget *m_fromTable, *m_toTable;
    QLineF* m_line;
};

#endif // DIAGRAMOBJECTS_H
