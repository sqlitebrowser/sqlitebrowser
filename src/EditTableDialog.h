#ifndef EDITTABLEDIALOG_H
#define EDITTABLEDIALOG_H

#include "sqlitetypes.h"

#include <QDialog>

class DBBrowserDB;
class QTreeWidgetItem;
class ForeignKeyEditorDelegate;

namespace Ui {
class EditTableDialog;
}

class EditTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTableDialog(DBBrowserDB& pdb, const sqlb::ObjectIdentifier& tableName, bool createTable, QWidget* parent = nullptr);
    ~EditTableDialog() override;

protected:
    void keyPressEvent(QKeyEvent *evt) override;

private:
    enum Columns {
        kName = 0,
        kType = 1,
        kNotNull = 2,
        kPrimaryKey = 3,
        kAutoIncrement = 4,
        kUnique = 5,
        kDefault = 6,
        kCheck = 7,
        kForeignKey = 8
    };

    void updateColumnWidth();
    void updateSqlText();

    void moveCurrentField(bool down);

private slots:
    void populateFields();
    void addField();
    void removeField();
    void fieldSelectionChanged();
    void accept() override;
    void reject() override;
    void checkInput();
    void itemChanged(QTreeWidgetItem* item, int column);
    void updateTypes(QObject *object);
    bool eventFilter(QObject *object, QEvent *event) override;
    void updateTypes();
    void moveUp();
    void moveDown();
    void setWithoutRowid(bool without_rowid);
    void changeSchema(const QString& schema);

private:
    Ui::EditTableDialog* ui;
    DBBrowserDB& pdb;
    ForeignKeyEditorDelegate* m_fkEditorDelegate;
    sqlb::ObjectIdentifier curTable;
    sqlb::Table m_table;
    QStringList types;
    QStringList fields;
    bool m_bNewTable;
    QString m_sRestorePointName;
    bool m_bForeignKeysEnabled;
};

#endif
