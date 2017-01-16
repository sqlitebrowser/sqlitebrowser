#ifndef CREATEINDEXDIALOG_H
#define CREATEINDEXDIALOG_H

#include "sqlitetypes.h"

#include <QDialog>

class DBBrowserDB;

namespace Ui {
class CreateIndexDialog;
}

class CreateIndexDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateIndexDialog(DBBrowserDB& db, QWidget* parent = 0);
    ~CreateIndexDialog();

private slots:
    void accept();
    void tableChanged(const QString& new_table);
    void checkInput();

private:
    DBBrowserDB& pdb;
    Ui::CreateIndexDialog* ui;
    sqlb::Index index;

    void updateSqlText();
};

#endif
