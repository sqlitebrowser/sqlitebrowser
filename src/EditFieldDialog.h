#ifndef __EDITFIELDDIALOG_H__
#define __EDITFIELDDIALOG_H__

#include <QDialog>

class DBBrowserDB;

namespace Ui {
class EditFieldDialog;
}

class EditFieldDialog : public QDialog
{
    Q_OBJECT

public:
    EditFieldDialog(DBBrowserDB* db, bool new_field, QString table, QString fld_name, QString fld_type, QWidget* parent = 0);
    ~EditFieldDialog();

public slots:
    virtual void accept();
    virtual void checkInput();

private:
    Ui::EditFieldDialog* ui;
    DBBrowserDB* pdb;
    QString original_field_name;
    QString table_name;
    bool is_new;

public:
    QString field_name;
    QString field_type;
};

#endif
