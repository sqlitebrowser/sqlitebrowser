#ifndef ExportSqlDialog_H
#define ExportSqlDialog_H

#include <QDialog>

class DBBrowserDB;

namespace Ui {
class ExportSqlDialog;
}

class ExportSqlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportSqlDialog(DBBrowserDB* db, QWidget* parent = 0, const QString& selection = "");
    ~ExportSqlDialog();

private slots:
    virtual void accept();
    virtual void reject();

    void doSelectAll();
    void doDeselectAll();

private:
    Ui::ExportSqlDialog* ui;
    DBBrowserDB* pdb;
};

#endif
