#ifndef __IMPORTCSVDIALOG_H__
#define __IMPORTCSVDIALOG_H__

#include <QDialog>
class DBBrowserDB;

namespace Ui {
class ImportCsvDialog;
}

class ImportCsvDialog : public QDialog
{
    Q_OBJECT

public:
    ImportCsvDialog(QString filename, DBBrowserDB* db, QWidget* parent = 0);
    ~ImportCsvDialog();

protected slots:
    virtual void accept();
    virtual void updatePreview();
    virtual void checkInput();

private:
    Ui::ImportCsvDialog* ui;
    QString csvFilename;
    DBBrowserDB* pdb;

    char currentQuoteChar();
    char currentSeparatorChar();
};

#endif
