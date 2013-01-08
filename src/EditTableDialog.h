#ifndef __EDITTABLEDIALOG_H__
#define __EDITTABLEDIALOG_H__

#include <QDialog>
class DBBrowserDB;

namespace Ui {
class EditTableDialog;
}

class EditTableDialog : public QDialog
{
    Q_OBJECT

public:
    EditTableDialog(QWidget* parent = 0);
    ~EditTableDialog();

    bool modified;
    QString curTable;

public slots:
    virtual void setActiveTable( DBBrowserDB * thedb, QString tableName );
    virtual void populateFields();
    virtual void editField();
    virtual void addField();
    virtual void removeField();
    virtual void fieldSelectionChanged();
    virtual void accept();
    virtual void reject();
    virtual void checkInput();

protected:
    QStringList types;
    QStringList fields;
    DBBrowserDB *pdb;

private:
    Ui::EditTableDialog *ui;
};

#endif
