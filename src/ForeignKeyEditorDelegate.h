#ifndef FOREIGNKEYDELEGATE_H
#define FOREIGNKEYDELEGATE_H

#include <QStyledItemDelegate>

class DBBrowserDB;
class QPushButton;
class QComboBox;
class QLineEdit;

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

    void updateTablesList(const QString& oldTableName);

private:
    const DBBrowserDB& m_db;
    sqlb::Table& m_table;
    mutable QMap<QString, QStringList> m_tablesIds;
};

#endif // FOREIGNKEYDELEGATE_H
