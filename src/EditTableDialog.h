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
    explicit EditTableDialog(DBBrowserDB* pdb, const QString& tableName, QWidget* parent = 0);
    ~EditTableDialog();

private slots:
    virtual void populateFields();
    virtual void editField();
    virtual void addField();
    virtual void removeField();
    virtual void fieldSelectionChanged();
    virtual void accept();
    virtual void reject();
    virtual void checkInput();

private:
    Ui::EditTableDialog* ui;
    DBBrowserDB* pdb;
    QString curTable;
    QStringList types;
    QStringList fields;
};

#endif
