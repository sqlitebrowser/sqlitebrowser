#ifndef IMPORTCSVDIALOG_H
#define IMPORTCSVDIALOG_H

#include "sqlitetypes.h"
#include <QDialog>

class DBBrowserDB;
class QCompleter;
class CSVParser;
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

    CSVParser parseCSV(const QString &f, qint64 count = -1);
    sqlb::FieldVector generateFieldList(const CSVParser& parser);

    void importCsv(const QString& f, const QString &n = QString());

    void setQuoteChar(const QChar& c);
    char currentQuoteChar() const;

    void setSeparatorChar(const QChar& c);
    char currentSeparatorChar() const;

    void setEncoding(const QString& sEnc);
    QString currentEncoding() const;
};

#endif
