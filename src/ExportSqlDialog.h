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
    explicit ExportSqlDialog(DBBrowserDB* db, QWidget* parent = nullptr, const QString& selection = QString());
    ~ExportSqlDialog() override;

private slots:
    void accept() override;
    void doSelectAll();
    void doDeselectAll();
    void whatChanged(int index);

private:
    Ui::ExportSqlDialog* ui;
    DBBrowserDB* pdb;
};

#endif
