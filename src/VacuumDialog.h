#ifndef __VACUUMDIALOG_H__
#define __VACUUMDIALOG_H__

#include <QDialog>

namespace Ui {
class VacuumDialog;
}

class DBBrowserDB;

class VacuumDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VacuumDialog(DBBrowserDB* _db, QWidget* parent = 0);
    ~VacuumDialog();

private:
    Ui::VacuumDialog* ui;
    DBBrowserDB* db;

protected slots:
    virtual void accept();
};

#endif
