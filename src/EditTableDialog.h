#ifndef EDITTABLEDIALOG_H
#define EDITTABLEDIALOG_H

#include "sqlitetypes.h"

#include <QDialog>

class DBBrowserDB;
class QTreeWidgetItem;

namespace Ui {
class EditTableDialog;
}

class EditTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTableDialog(DBBrowserDB* pdb, const QString& tableName, bool createTable, QWidget* parent = 0);
    ~EditTableDialog();

protected:
    void keyPressEvent(QKeyEvent *evt);

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
    virtual void populateFields();
    virtual void addField();
    virtual void removeField();
    virtual void fieldSelectionChanged();
    virtual void accept();
    virtual void reject();
    virtual void checkInput();
    virtual void itemChanged(QTreeWidgetItem* item, int column);
    virtual void updateTypes();
    virtual void moveUp();
    virtual void moveDown();
    virtual void setWithoutRowid(bool without_rowid);

private:
    Ui::EditTableDialog* ui;
    DBBrowserDB* pdb;
    QString curTable;
    sqlb::Table m_table;
    QStringList types;
    QStringList fields;
    bool m_bNewTable;
    QString m_sRestorePointName;
};

#endif
