#ifndef FOREIGNKEYDELEGATE_H
#define FOREIGNKEYDELEGATE_H

#include <QStyledItemDelegate>
#include <unordered_map>
#include <vector>
#include <string>

class DBBrowserDB;

namespace sqlb
{
class Table;
}

class ForeignKeyEditorDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ForeignKeyEditorDelegate(const DBBrowserDB& db, sqlb::Table& table, QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    const DBBrowserDB& m_db;
    sqlb::Table& m_table;
    mutable std::unordered_map<std::string, std::vector<std::string>> m_tablesIds;
};

#endif // FOREIGNKEYDELEGATE_H
