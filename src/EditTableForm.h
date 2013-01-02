#ifndef EDITTABLEFORM_H
#define EDITTABLEFORM_H

#include <QDialog>
class DBBrowserDB;

namespace Ui {
class editTableForm;
}

class editTableForm : public QDialog
{
    Q_OBJECT

public:
    editTableForm(QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
    ~editTableForm();

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
    Ui::editTableForm *ui;
};

#endif // EDITTABLEFORM_H
