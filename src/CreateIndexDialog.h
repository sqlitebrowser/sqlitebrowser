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
    explicit CreateIndexDialog(DBBrowserDB& db, const QString& indexName, bool createIndex, QWidget* parent = 0);
    ~CreateIndexDialog();

private slots:
    void accept();
    void tableChanged(const QString& new_table, bool initialLoad = false);
    void checkInput();

private:
    DBBrowserDB& pdb;
    QString curIndex;
    sqlb::Index index;
    bool newIndex;
    Ui::CreateIndexDialog* ui;

    void updateSqlText();
};

#endif
