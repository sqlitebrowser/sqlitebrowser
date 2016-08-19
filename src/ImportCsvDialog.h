#ifndef IMPORTCSVDIALOG_H
#define IMPORTCSVDIALOG_H

#include <QDialog>

class DBBrowserDB;
class QCompleter;

namespace Ui {
class ImportCsvDialog;
}

class ImportCsvDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportCsvDialog(const QString& filename, DBBrowserDB* db, QWidget* parent = 0);
    ~ImportCsvDialog();

private slots:
    virtual void accept();
    void updatePreview();
    void checkInput();

private:
    Ui::ImportCsvDialog* ui;
    QString csvFilename;
    DBBrowserDB* pdb;
    QCompleter* encodingCompleter;

    void setQuoteChar(const QChar& c);
    char currentQuoteChar() const;

    void setSeparatorChar(const QChar& c);
    char currentSeparatorChar() const;

    void setEncoding(const QString& sEnc);
    QString currentEncoding() const;
};

#endif
