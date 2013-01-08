#ifndef __CREATEINDEXDIALOG_H__
#define __CREATEINDEXDIALOG_H__

#include <QDialog>
class DBBrowserDB;

namespace Ui {
class CreateIndexDialog;
}

class CreateIndexDialog : public QDialog
{
    Q_OBJECT

public:
    CreateIndexDialog(DBBrowserDB* db, QWidget* parent = 0);
    ~CreateIndexDialog();

private slots:
    void accept();
    void tableChanged(QString new_table);
    void checkInput();

private:
    DBBrowserDB* pdb;
    Ui::CreateIndexDialog* ui;
};

#endif
