#ifndef DIAGRAMOBJECTS_H
#define DIAGRAMOBJECTS_H

#include "sql/sqlitetypes.h"

#include <QAbstractListModel>
#include <QWidget>
#include <QMap>

#include <string>
#include <vector>

class QLabel;
class QListView;
class QLineF;

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

class Relation : public QObject
{
    Q_OBJECT

public:
    Relation(TableWidget* fromTable, TableWidget* toTable, QObject* parent = nullptr);
    ~Relation();

    QString tooltipText() const;

private:
    TableWidget *m_fromTable, *m_toTable;
    QLineF* m_line;
};

#endif
