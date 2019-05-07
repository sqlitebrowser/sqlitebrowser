#ifndef IMPORTCSVDIALOG_H
#define IMPORTCSVDIALOG_H

#include "csvparser.h"
#include "sql/sqlitetypes.h"

#include <QDialog>
#include <functional>

class DBBrowserDB;
class QCompleter;

namespace Ui {
class ImportCsvDialog;
}

class ImportCsvDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportCsvDialog(const QStringList& filenames, DBBrowserDB* db, QWidget* parent = nullptr);
    ~ImportCsvDialog() override;

private slots:
    void accept() override;
    void updatePreview();
    void checkInput();
    void selectFiles();
    void updateSelectedFilePreview();
    void updateSelection(bool);
    void matchSimilar();
    void toggleAdvancedSection(bool show);

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

    void setQuoteChar(char32_t c);
    char32_t currentQuoteChar() const;

    void setSeparatorChar(char32_t c);
    char32_t currentSeparatorChar() const;

    void setEncoding(const QString& sEnc);
    QString currentEncoding() const;

    QString currentOnConflictStrategy() const;

    char32_t toUtf8(const QString& s) const;
};

#endif
