#ifndef EDITTABLEDIALOG_H
#define EDITTABLEDIALOG_H

#include "sql/ObjectIdentifier.h"
#include "sql/sqlitetypes.h"

#include <map>

#include <QDialog>

class DBBrowserDB;
class ForeignKeyEditorDelegate;

class QTableWidgetItem;
class QTreeWidgetItem;

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
        kCollation = 8,
        kForeignKey = 9
    };

    enum ConstraintColumns {
        kConstraintColumns = 0,
        kConstraintType = 1,
        kConstraintName = 2,
        kConstraintSql = 3
    };

    enum MoveFieldDirection
    {
        MoveUp,
        MoveDown,
        MoveTop,
        MoveBottom
    };

    void updateColumnWidth();
    void updateSqlText();

    void moveCurrentField(MoveFieldDirection dir);

private slots:
    void populateFields();
    void populateConstraints();
    void addField();
    void removeField();
    void fieldSelectionChanged();
    void accept() override;
    void reject() override;
    void checkInput();
    void fieldItemChanged(QTreeWidgetItem* item, int column);
    void constraintItemChanged(QTableWidgetItem* item);
    void updateTypeAndCollation(QObject *object);
    bool eventFilter(QObject *object, QEvent *event) override;
    void updateTypeAndCollation();
    void moveUp();
    void moveDown();
    void moveTop();
    void moveBottom();
    void setWithoutRowid(bool without_rowid);
    void changeSchema(const QString& schema);
    void removeConstraint();
    void addConstraint(sqlb::Constraint::ConstraintTypes type);

private:
    Ui::EditTableDialog* ui;
    DBBrowserDB& pdb;
    ForeignKeyEditorDelegate* m_fkEditorDelegate;
    sqlb::ObjectIdentifier curTable;
    std::map<QString, QString> trackColumns;
    sqlb::Table m_table;
    bool m_bNewTable;
    std::string m_sRestorePointName;
    QStringList m_collationList;
};

#endif
