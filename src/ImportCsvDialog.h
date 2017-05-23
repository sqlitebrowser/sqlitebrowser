#ifndef IMPORTCSVDIALOG_H
#define IMPORTCSVDIALOG_H

#include "sqlitetypes.h"
#include <QDialog>

class DBBrowserDB;
class QCompleter;
class QListWidgetItem;

namespace Ui {
class ImportCsvDialog;
}

class ImportCsvDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportCsvDialog(const QStringList& filenames, DBBrowserDB* db, QWidget* parent = 0);
    ~ImportCsvDialog();

private slots:
    virtual void accept();
    void updatePreview();
    void checkInput();
    void selectFiles();
    void updateSelectedFilePreview(QListWidgetItem*);
    void updateSelection(bool);
    void matchSimilar();

private:
    Ui::ImportCsvDialog* ui;
    QStringList csvFilenames;
    QString selectedFile;
    DBBrowserDB* pdb;
    QCompleter* encodingCompleter;

    void importCsv(const QString& f);
    sqlb::FieldVector fetchCsvHeader(const QString& f);

    void setQuoteChar(const QChar& c);
    char currentQuoteChar() const;

    void setSeparatorChar(const QChar& c);
    char currentSeparatorChar() const;

    void setEncoding(const QString& sEnc);
    QString currentEncoding() const;
};

#endif
