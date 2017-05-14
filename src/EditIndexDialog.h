#ifndef EDITINDEXDIALOG_H
#define EDITINDEXDIALOG_H

#include "sqlitetypes.h"

#include <QDialog>
#include <QModelIndex>

class DBBrowserDB;

namespace Ui {
class EditIndexDialog;
}

class EditIndexDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditIndexDialog(DBBrowserDB& db, const QString& indexName, bool createIndex, QWidget* parent = 0);
    ~EditIndexDialog();

private slots:
    void accept();
    void reject();

    void tableChanged(const QString& new_table, bool initialLoad = false);
    void checkInput();
    void addToIndex(const QModelIndex& idx = QModelIndex());
    void removeFromIndex(const QModelIndex& idx = QModelIndex());
    void moveColumnUp();
    void moveColumnDown();
    void addExpressionColumn();

private:
    DBBrowserDB& pdb;
    QString curIndex;
    sqlb::Index index;
    bool newIndex;
    Ui::EditIndexDialog* ui;
    QString m_sRestorePointName;

    void updateColumnLists();
    void updateSqlText();
    void moveCurrentColumn(bool down);
};

#endif
