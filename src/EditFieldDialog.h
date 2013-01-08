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
    EditFieldDialog(QWidget* parent = 0);
    ~EditFieldDialog();

    QString table_name;
    QString field_name;
    QString field_type;
    QString original_field_name;
    bool is_new;

public slots:
    virtual void setInitialValues( DBBrowserDB *db, bool is_new, QString table, QString fld_name, QString fld_type );
    virtual void accept();
    virtual void checkInput();

private:
    DBBrowserDB *pdb;
    Ui::EditFieldDialog *ui;
};

#endif
