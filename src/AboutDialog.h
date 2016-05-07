#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class DBBrowserDB;

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(DBBrowserDB* _db, QWidget *parent = 0);
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
    DBBrowserDB* db;
};

#endif
