#ifndef IMPORTCSVDIALOG_H
#define IMPORTCSVDIALOG_H

#include "csvparser.h"
#include "sqlitetypes.h"

#include <QDialog>
#include <functional>

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
    void updateSelectedFilePreview();
    void updateSelection(bool);
    void matchSimilar();

private:
    Ui::ImportCsvDialog* ui;
    QStringList csvFilenames;
    QString selectedFile;
    DBBrowserDB* pdb;
    QCompleter* encodingCompleter;
    QStringList dontAskForExistingTableAgain;

    CSVParser::ParserResult parseCSV(const QString& fileName, std::function<bool(size_t, CSVRow)> rowFunction, size_t count = 0);
    sqlb::FieldVector generateFieldList(const QString& filename);

    bool importCsv(const QString& f, const QString& n = QString());

    void setQuoteChar(const QChar& c);
    char currentQuoteChar() const;

    void setSeparatorChar(const QChar& c);
    char currentSeparatorChar() const;

    void setEncoding(const QString& sEnc);
    QString currentEncoding() const;
};

#endif
