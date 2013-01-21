#ifndef __EXPORTCSVDIALOG_H__
#define __EXPORTCSVDIALOG_H__

#include <QDialog>
class DBBrowserDB;

namespace Ui {
class ExportCsvDialog;
}

class ExportCsvDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportCsvDialog(DBBrowserDB* db, QString deflocation, QWidget* parent = 0);
    ~ExportCsvDialog();

private slots:
    virtual void accept();

private:
    Ui::ExportCsvDialog* ui;
    DBBrowserDB* pdb;
    QString defaultLocation;
};

#endif
