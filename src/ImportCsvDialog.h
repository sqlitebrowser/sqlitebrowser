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
    explicit ImportCsvDialog(const std::vector<QString>& filenames, DBBrowserDB* db, QWidget* parent = nullptr);
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

    // Positions for combos starting at the bottom
    enum {OtherCode = 1,
          OtherPrintable = 2};

    Ui::ImportCsvDialog* ui;
    std::vector<QString> csvFilenames;
    QString selectedFile;
    DBBrowserDB* pdb;
    QCompleter* encodingCompleter;
    QStringList dontAskForExistingTableAgain;

    CSVParser::ParserResult parseCSV(const QString& fileName, std::function<bool(size_t, CSVRow)> rowFunction, size_t count = 0) const;
    sqlb::FieldVector generateFieldList(const QString& filename) const;

    bool importCsv(const QString& f, const QString& n = QString());

    void setQuoteChar(QChar c);
    QChar currentQuoteChar() const;

    void setSeparatorChar(QChar c);
    QChar currentSeparatorChar() const;

    void setEncoding(const QString& sEnc);
    QString currentEncoding() const;

    std::string currentOnConflictStrategy() const;

    char32_t toUtf8(const QString& s) const;

    static QChar getSettingsChar(const std::string& group, const std::string& name);
};

#endif
