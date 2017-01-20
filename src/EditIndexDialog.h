#ifndef EDITINDEXDIALOG_H
#define EDITINDEXDIALOG_H

#include "sqlitetypes.h"

#include <QDialog>

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
    void tableChanged(const QString& new_table, bool initialLoad = false);
    void checkInput();

private:
    DBBrowserDB& pdb;
    QString curIndex;
    sqlb::Index index;
    bool newIndex;
    Ui::EditIndexDialog* ui;

    void updateSqlText();
};

#endif
