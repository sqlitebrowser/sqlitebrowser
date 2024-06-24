#include "ImportCsvDialog.h"
#include "ui_ImportCsvDialog.h"
#include "sqlitedb.h"
#include "csvparser.h"
#include "sqlite.h"
#include "Settings.h"
#include "Data.h"

#include <QMessageBox>
#include <QProgressDialog>
#include <QPushButton>
#include <QDateTime>
#include <QTextCodec>
#include <QCompleter>
#include <QComboBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <memory>

// Enable this line to show basic performance stats after each imported CSV file. Please keep in mind that while these
// numbers might help to estimate the performance of the algorithm, this is not a proper benchmark.
//#define CSV_BENCHMARK

#ifdef CSV_BENCHMARK
#include <QElapsedTimer>
#endif

QChar ImportCsvDialog::getSettingsChar(const std::string& group, const std::string& name)
{
    QVariant value = Settings::getValue(group, name);
    // QVariant is not able to return the character as a QChar when QString is stored.
    // We do it manually, since it is versatile, when the option is passed from the command line,
    // for example.
    if(value.userType() == QMetaType::QString)
        return value.toString().at(0);
    else
        return value.toChar();
}

ImportCsvDialog::ImportCsvDialog(const std::vector<QString>& filenames, DBBrowserDB* db, QWidget* parent, const QString& table)
    : QDialog(parent),
      ui(new Ui::ImportCsvDialog),
      csvFilenames(filenames),
      pdb(db)
{
    ui->setupUi(this);

    // Hide "Advanced" section of the settings
    toggleAdvancedSection(false);

    if(!table.isEmpty()) {
        ui->editName->setText(table);
    } else {
        // Get the actual file name out of the provided path and use it as the default table name for import
        // For importing several files at once, the fields have to be the same so we can safely use the first
        QFileInfo file(filenames.front());
        ui->editName->setText(file.baseName());
    }

    // Create a list of all available encodings and create an auto completion list from them
    encodingCompleter = new QCompleter(toStringList(QTextCodec::availableCodecs()), this);
    encodingCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->editCustomEncoding->setCompleter(encodingCompleter);

    // Load last used settings and apply them
    ui->checkboxHeader->blockSignals(true);
    ui->checkBoxTrimFields->blockSignals(true);
    ui->checkBoxSeparateTables->blockSignals(true);
    ui->checkLocalConventions->blockSignals(true);
    ui->comboSeparator->blockSignals(true);
    ui->comboQuote->blockSignals(true);
    ui->comboEncoding->blockSignals(true);

    ui->checkboxHeader->setChecked(Settings::getValue("importcsv", "firstrowheader").toBool());
    ui->checkBoxTrimFields->setChecked(Settings::getValue("importcsv", "trimfields").toBool());
    ui->checkBoxSeparateTables->setChecked(Settings::getValue("importcsv", "separatetables").toBool());
    ui->checkLocalConventions->setChecked(Settings::getValue("importcsv", "localconventions").toBool());
    setSeparatorChar(getSettingsChar("importcsv", "separator"));
    setQuoteChar(getSettingsChar("importcsv", "quotecharacter"));
    setEncoding(Settings::getValue("importcsv", "encoding").toString());

    ui->checkboxHeader->blockSignals(false);
    ui->checkBoxTrimFields->blockSignals(false);
    ui->checkBoxSeparateTables->blockSignals(false);
    ui->checkLocalConventions->blockSignals(false);
    ui->comboSeparator->blockSignals(false);
    ui->comboQuote->blockSignals(false);
    ui->comboEncoding->blockSignals(false);

    // Prepare and show interface depending on how many files are selected
    if (csvFilenames.size() > 1)
    {
        ui->separateTables->setVisible(true);
        ui->checkBoxSeparateTables->setVisible(true);
        ui->filePickerBlock->setVisible(true);
        selectFiles();
    }
    else if (csvFilenames.size() == 1)
    {
        ui->separateTables->setVisible(false);
        ui->checkBoxSeparateTables->setVisible(false);
        ui->filePickerBlock->setVisible(false);
    }

    selectedFile = csvFilenames.front();
    updatePreview();
    checkInput();
}

ImportCsvDialog::~ImportCsvDialog()
{
    delete ui;
}

namespace {
void rollback(
        ImportCsvDialog* dialog,
        DBBrowserDB* pdb,
        DBBrowserDB::db_pointer_type* db_ptr,
        const std::string& savepointName,
        size_t nRecord,
        const QString& message)
{
    // Release DB handle. This needs to be done before calling revertToSavepoint as that function needs to be able to acquire its own handle.
    if(db_ptr)
        *db_ptr = nullptr;

    QApplication::restoreOverrideCursor();  // restore original cursor
    if(!message.isEmpty())
    {
        QString sCSVInfo = QObject::tr("Error importing data");
        if(nRecord)
            sCSVInfo += QObject::tr(" from record number %1").arg(nRecord);
        QString error = sCSVInfo + QObject::tr(".\n%1").arg(message);
        QMessageBox::warning(dialog, QApplication::applicationName(), error);
    }
    pdb->revertToSavepoint(savepointName);
}
}

class CSVImportProgress : public CSVProgress
{
public:
    explicit CSVImportProgress(int64_t filesize)
        : m_pProgressDlg(new QProgressDialog(
                             QObject::tr("Importing CSV file..."),
                             QObject::tr("Cancel"),
                             0,
                             10000)),
          totalFileSize(filesize)
    {
        m_pProgressDlg->setWindowModality(Qt::ApplicationModal);
        // Disable context help button on Windows
        m_pProgressDlg->setWindowFlags(m_pProgressDlg->windowFlags()
                                      & ~Qt::WindowContextHelpButtonHint);
    }

    CSVImportProgress(const CSVImportProgress&) = delete;
    bool operator=(const CSVImportProgress&) = delete;

    void start() override
    {
        m_pProgressDlg->show();
    }

    bool update(int64_t pos) override
    {
        m_pProgressDlg->setValue(static_cast<int>((static_cast<float>(pos) / static_cast<float>(totalFileSize)) * 10000.0f));
        qApp->processEvents();

        return !m_pProgressDlg->wasCanceled();
    }

    void end() override
    {
        m_pProgressDlg->hide();
    }

private:
    std::unique_ptr<QProgressDialog> m_pProgressDlg;

    int64_t totalFileSize;
};

void ImportCsvDialog::accept()
{
    // Save settings
    Settings::setValue("importcsv", "firstrowheader", ui->checkboxHeader->isChecked());
    Settings::setValue("importcsv", "separator", currentSeparatorChar());
    Settings::setValue("importcsv", "quotecharacter", currentQuoteChar());
    Settings::setValue("importcsv", "trimfields", ui->checkBoxTrimFields->isChecked());
    Settings::setValue("importcsv", "separatetables", ui->checkBoxSeparateTables->isChecked());
    Settings::setValue("importcsv", "localconventions", ui->checkLocalConventions->isChecked());
    Settings::setValue("importcsv", "encoding", currentEncoding());

    // Get all the selected files and start the import
    if (ui->filePickerBlock->isVisible())
    {
        bool filesLeft = false;

        // Loop through all the rows in the file picker list
        for (int i = 0; i < ui->filePicker->count(); i++) {
            auto item = ui->filePicker->item(i);
            int row = ui->filePicker->row(item);

            // Check for files that aren't hidden (=imported) yet but that are checked and thus marked for import
            if (item->checkState() == Qt::Checked && !ui->filePicker->isRowHidden(row)) {
                if(!importCsv(item->data(Qt::DisplayRole).toString(), item->data(Qt::UserRole).toString()))
                {
                    // If we're supposed to cancel the import process, we always have at least one file left (the cancelled one). Also
                    // we stop looping through the rest of the CSV files.
                    filesLeft = true;
                    break;
                }

                // Hide each row after it's done
                ui->filePicker->setRowHidden(row, true);
            } else if(!ui->filePicker->isRowHidden(row)) {
                // Check for files that aren't hidden yet but that aren't checked either. These are files that are still left
                // to be imported
                filesLeft = true;
            }
        }

        // Don't close the window if there are still files left to be imported
        if(filesLeft)
        {
            QApplication::restoreOverrideCursor();  // restore original cursor
            return;
        }
    } else {
        importCsv(csvFilenames.front());
    }

    QApplication::restoreOverrideCursor();  // restore original cursor
    QDialog::accept();
}

void ImportCsvDialog::updatePreview()
{
    // Show/hide custom quote/separator input fields
    ui->editCustomQuote->setVisible(ui->comboQuote->currentIndex() == ui->comboQuote->count() - OtherPrintable);
    ui->editCustomSeparator->setVisible(ui->comboSeparator->currentIndex() == ui->comboSeparator->count() - OtherPrintable);
    ui->spinBoxQuote->setVisible(ui->comboQuote->currentIndex() == ui->comboQuote->count() - OtherCode);
    ui->spinBoxSeparator->setVisible(ui->comboSeparator->currentIndex() == ui->comboSeparator->count() - OtherCode);

    ui->editCustomEncoding->setVisible(ui->comboEncoding->currentIndex() == ui->comboEncoding->count()-1);

    // Reset preview widget
    ui->tablePreview->clear();
    ui->tablePreview->setColumnCount(0);
    ui->tablePreview->setRowCount(0);

    // Analyse CSV file
    sqlb::FieldVector fieldList = generateFieldList(selectedFile);

    // Reset preview widget
    ui->tablePreview->clear();
    ui->tablePreview->setColumnCount(static_cast<int>(fieldList.size()));

    // Exit if there are no lines to preview at all
    if(fieldList.size() == 0)
        return;

    // Set horizontal header data
    QStringList horizontalHeader;
    std::transform(fieldList.begin(), fieldList.end(), std::back_inserter(horizontalHeader), [](const auto& field) { return QString::fromStdString(field.name()); });
    ui->tablePreview->setHorizontalHeaderLabels(horizontalHeader);

    // Parse file
    parseCSV(selectedFile, [this](size_t rowNum, const CSVRow& rowData) -> bool {
        // Skip first row if it is to be used as header
        if(rowNum == 0 && ui->checkboxHeader->isChecked())
            return true;

        // Decrease the row number by one if the header checkbox is checked to take into account that the first row was used for the table header labels
        // and therefore all data rows move one row up.
        if(ui->checkboxHeader->isChecked())
            rowNum--;

        // Fill data section
        ui->tablePreview->setRowCount(ui->tablePreview->rowCount() + 1);
        for(size_t i=0;i<rowData.num_fields;i++)
        {
            // Generate vertical header items
            if(i == 0)
                ui->tablePreview->setVerticalHeaderItem(static_cast<int>(rowNum), new QTableWidgetItem(QString::number(rowNum + 1)));

            // Add table item. Limit data length to a maximum character count to avoid a sluggish UI. And it's very unlikely that this
            // many characters are going to be needed anyway for a preview.
            uint64_t data_length = rowData.fields[i].data_length;
            if(data_length > 1024)
                data_length = 1024;
            ui->tablePreview->setItem(
                        static_cast<int>(rowNum),
                        static_cast<int>(i),
                        new QTableWidgetItem(QString::fromUtf8(rowData.fields[i].data, static_cast<int>(data_length))));
        }

        return true;
    }, 20);
}

void ImportCsvDialog::checkInput()
{
    bool allowImporting = false;
    if (ui->filePickerBlock->isVisible()) {
        bool checkedItem = false;
        for (int i = 0; i < ui->filePicker->count(); i++) {
            if (ui->filePicker->item(i)->checkState() == Qt::Checked) checkedItem = true;
        }
        allowImporting = !ui->editName->text().isEmpty() && checkedItem;
    } else {
        allowImporting = !ui->editName->text().isEmpty();
    }

    if (ui->filePicker->currentItem() && ui->checkBoxSeparateTables->isChecked()) {
        ui->filePicker->currentItem()->setData(Qt::UserRole, ui->editName->text());
    }

    ui->matchSimilar->setEnabled(ui->filePicker->currentItem() != nullptr);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(allowImporting);
}

void ImportCsvDialog::selectFiles()
{
    for (const auto& fileName : csvFilenames) {
        auto fInfo = QFileInfo(fileName);
        auto item = new QListWidgetItem();
        item->setText(fileName);
        item->setData(Qt::UserRole, fInfo.baseName());
        item->setCheckState(Qt::Checked);
        ui->filePicker->addItem(item);
    }

    connect(ui->filePicker, &QListWidget::itemSelectionChanged, this, &ImportCsvDialog::updateSelectedFilePreview);
}

void ImportCsvDialog::updateSelectedFilePreview()
{
    auto selection = ui->filePicker->selectedItems();
    if(!selection.isEmpty())
    {
        auto item = selection.first();
        selectedFile = item->data(Qt::DisplayRole).toString();
        ui->editName->setText(item->data(Qt::UserRole).toString());
        updatePreview();
        checkInput();
    }
}

void ImportCsvDialog::updateSelection(bool selected)
{
    for (int i = 0; i < ui->filePicker->count(); i++)
        ui->filePicker->item(i)->setCheckState(selected ? Qt::Checked : Qt::Unchecked);
    ui->toggleSelected->setText(selected ? tr("Deselect All") : tr("Select All"));
    checkInput();
}

void ImportCsvDialog::matchSimilar()
{
    auto selectedHeader = generateFieldList(ui->filePicker->currentItem()->data(Qt::DisplayRole).toString());

    for (int i = 0; i < ui->filePicker->count(); i++)
    {
        auto item = ui->filePicker->item(i);
        auto header = generateFieldList(item->data(Qt::DisplayRole).toString());

        if (selectedHeader.size() == header.size())
        {
            bool matchingHeader = std::equal(selectedHeader.begin(), selectedHeader.end(), header.begin(),
                                             [](const sqlb::Field& item1, const sqlb::Field& item2) -> bool {
                                                return (item1.name() == item2.name());
                                             });
            if (matchingHeader) {
                item->setCheckState(Qt::Checked);
                item->setBackground(Qt::green);
            }
        }
        else
        {
            item->setCheckState(Qt::Unchecked);
            item->setBackground(Qt::white);
        }
    }

    checkInput();
}

CSVParser::ParserResult ImportCsvDialog::parseCSV(const QString &fileName, std::function<bool(size_t, CSVRow)> rowFunction, size_t count) const
{
    // Parse all csv data
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    CSVParser csv(ui->checkBoxTrimFields->isChecked(), toUtf8(currentSeparatorChar()), toUtf8(currentQuoteChar()));

    // Only show progress dialog if we parse all rows. The assumption here is that if a row count limit has been set, it won't be a very high one.
    if(count == 0)
        csv.setCSVProgress(new CSVImportProgress(file.size()));

    QTextStream tstream(&file);
    tstream.setCodec(currentEncoding().toUtf8());

    return csv.parse(rowFunction, tstream, count);
}

sqlb::FieldVector ImportCsvDialog::generateFieldList(const QString& filename) const
{
    sqlb::FieldVector fieldList;        // List of fields in the file

    // Parse the first couple of records of the CSV file and only analyse them
    parseCSV(filename, [this, &fieldList](size_t rowNum, const CSVRow& rowData) -> bool {
        // Has this row more columns than the previous one? Then add more fields to the field list as necessary.
        for(size_t i=fieldList.size();i<rowData.num_fields;i++)
        {
            std::string fieldname;

            // If the user wants to use the first row as table header and if this is the first row, extract a field name
            if(rowNum == 0 && ui->checkboxHeader->isChecked())
            {
                // Take field name from CSV
                fieldname = std::string(rowData.fields[i].data, rowData.fields[i].data_length);
            }

            // If we don't have a field name by now, generate one
            if(fieldname.empty())
                fieldname = "field" + std::to_string(i + 1);

            // Add field to the column list. For now we set the data type to nothing but this might be overwritten later in the automatic
            // type detection code.
            fieldList.emplace_back(fieldname, "");
        }

        // Try to find out a data type for each column. Skip the header row if there is one.
        if(!ui->checkNoTypeDetection->isChecked() && !(rowNum == 0 && ui->checkboxHeader->isChecked()))
        {
            for(size_t i=0;i<rowData.num_fields;i++)
            {
                // If the data type has been set to TEXT, there's no going back because it means we had at least one row with text-only
                // content and that means we don't want to set the data type to any number type.
                std::string old_type = fieldList.at(i).type();
                if(old_type != "TEXT")
                {
                    QString content = QString::fromUtf8(rowData.fields[i].data, static_cast<int>(rowData.fields[i].data_length));
                    const QLocale &locale = ui->checkLocalConventions->isChecked() ? QLocale::system() : QLocale::c();

                    // Check if the content can be converted to an integer or to real
                    bool convert_to_integer, convert_to_real;
                    locale.toLongLong(content, &convert_to_integer);
                    locale.toDouble(content, &convert_to_real);

                    // Set new data type. If we don't find any better data type, we fall back to the TEXT data type
                    std::string new_type = "TEXT";
                    if(old_type == "INTEGER" && !convert_to_integer && convert_to_real)  // So far it's integer, but now it's only convertible to float
                        new_type = "REAL";
                    else if(old_type.empty() && convert_to_integer)                      // No type yet, but this bit is convertible to integer
                        new_type = "INTEGER";
                    else if(old_type.empty() && convert_to_real)                         // No type yet and only convertible to float (less 'difficult' than integer)
                        new_type = "REAL";
                    else if(old_type == "REAL" && convert_to_real)                       // It was float so far and still is
                        new_type = "INTEGER";
                    else if(old_type == "INTEGER" && convert_to_integer)                 // It was integer so far and still is
                        new_type = "INTEGER";

                    fieldList.at(i).setType(new_type);
                }
            }
        }

        // All good
        return true;
    }, 20);

    return fieldList;
}

bool ImportCsvDialog::importCsv(const QString& fileName, const QString& name)
{
    // This function returns a boolean to indicate whether to continue or abort the import process. It's worth keeping in mind that
    // this doesn't correlate 100% to this import being a success or not. The general rule is that if there was some internal error
    // when writing to the database, we stop the entire import process. Also, if the user has clicked Cancel during the import of one
    // file, we stop the entire import process, i.e. also the import of the remaining files. Furthermore, if the user clicks the Cancel
    // button in a message box we (obviously) cancel the import process, too. In all other cases we return true to indicate that the import
    // should continue.

#ifdef CSV_BENCHMARK
    // If benchmark mode is enabled start measuring the performance now
    qint64 timesRowFunction = 0;
    QElapsedTimer timer;
    timer.start();
#endif

    QString tableName;

    if (csvFilenames.size() > 1 && ui->checkBoxSeparateTables->isChecked()) {
        if (name.isEmpty()) {
            QFileInfo fileInfo(fileName);
            tableName = fileInfo.baseName();
        } else {
            tableName = name;
        }
    } else {
        tableName = ui->editName->text();
    }

    // Analyse CSV file
    sqlb::FieldVector fieldList = generateFieldList(fileName);
    if(fieldList.size() == 0)
        return true;

    // Are we importing into an existing table?
    bool importToExistingTable = false;
    const sqlb::TablePtr tbl = pdb->getTableByName(sqlb::ObjectIdentifier("main", tableName.toStdString()));
    if(tbl)
    {
        if(tbl->fields.size() != fieldList.size())
        {
            QMessageBox::warning(this, QApplication::applicationName(),
                                 tr("There is already a table named '%1' and an import into an existing table is only possible if the number of columns match.").arg(tableName));
            return true;
        } else {
            // Only ask whether to import into any table if the 'Yes to All' button has not been clicked (yet) (empty string is included).
            // Only ask whether to import into the existing table if the 'Yes' button has not been clicked (yet) for that table.
            if(!dontAskForExistingTableAgain.contains("") && !dontAskForExistingTableAgain.contains(tableName))
            {
                int answer = QMessageBox::question(this, QApplication::applicationName(),
                                                   tr("There is already a table named '%1'. Do you want to import the data into it?").arg(tableName),
                                                   QMessageBox::Yes | QMessageBox::No | QMessageBox::YesAll | QMessageBox::Cancel, QMessageBox::No);

                // Stop now if the No button has been clicked
                switch (answer) {
                case QMessageBox::No:
                    return true;

                // Stop now if the Cancel button has been clicked. But also indicate, that the entire import process should be stopped.
                case QMessageBox::Cancel:
                    return false;

                // If the 'Yes' button has been clicked, save the answer for that table for later
                case QMessageBox::Yes:
                    dontAskForExistingTableAgain.append(tableName);
                    break;
                // If the 'Yes to All' button has been clicked, save the answer for any future table name. An empty string is used in that case.
                case QMessageBox::YesAll:
                    dontAskForExistingTableAgain.append("");
                    break;
                }
            }

            // If we reached this point, this means that either the 'Yes' or the 'Yes all' button has been clicked or that no message box was shown at all
            // because the 'Yes all' button has been clicked for a previous file
            importToExistingTable = true;
        }
    }

    // Create a savepoint, so we can rollback in case of any errors during importing
    // db needs to be saved or an error will occur
    std::string restorepointName = pdb->generateSavepointName("csvimport");
    if(!pdb->setSavepoint(restorepointName))
    {
        rollback(this, pdb, nullptr, restorepointName, 0, tr("Creating restore point failed: %1").arg(pdb->lastError()));
        return false;
    }

    // Create table
    std::vector<QByteArray> nullValues;
    std::vector<bool> failOnMissingFieldList;
    bool ignoreDefaults = ui->checkIgnoreDefaults->isChecked();
    bool failOnMissing = ui->checkFailOnMissing->isChecked();
    if(!importToExistingTable)
    {
        if(!pdb->createTable(sqlb::ObjectIdentifier("main", tableName.toStdString()), fieldList))
        {
            rollback(this, pdb, nullptr, restorepointName, 0, tr("Creating the table failed: %1").arg(pdb->lastError()));
            return false;
        }

        // If we're creating the table in this import session, don't ask the user if it's okay to import more data into it. It seems
        // safe to just assume that's what they want.
        dontAskForExistingTableAgain.append(tableName);
    } else {
        // Importing into an existing table. So find out something about it's structure.

        // Prepare the values for each table column that are to be inserted if the field in the CSV file is empty. Depending on the data type
        // and the constraints of a field, we need to handle this case differently.
        if(tbl)
        {
            for(const sqlb::Field& f : tbl->fields)
            {
                // For determining the value for empty fields we follow a set of rules

                // Normally we don't have to fail the import when importing an empty field. This last value of the vector
                // is changed to true later if we actually do want to fail the import for this field.
                failOnMissingFieldList.push_back(false);

                // If a field has a default value, that gets priority over everything else.
                // Exception: if the user wants to ignore default values we never use them.
                if(!ignoreDefaults && !f.defaultValue().empty())
                {
                    nullValues.push_back(f.defaultValue().c_str());
                } else {
                    // If it has no default value, check if the field is NOT NULL
                    if(f.notnull())
                    {
                        // The field is NOT NULL

                        // If this is an integer column insert 0. Otherwise insert an empty string.
                        if(f.isInteger())
                            nullValues.push_back("0");
                        else
                            nullValues.push_back("");

                        // If the user wants to fail the import, remember this field
                        if(failOnMissing)
                            failOnMissingFieldList.back() = true;
                    } else {
                        // The field is not NOT NULL (stupid double negation here! NULL values are allowed in this case)

                        // Just insert a NULL value
                        nullValues.push_back(QByteArray());
                    }
                }
            }
        }
    }

    // Prepare the INSERT statement. The prepared statement can then be reused for each row to insert
    std::string sQuery = "INSERT " + currentOnConflictStrategy() + " INTO " + sqlb::escapeIdentifier(tableName.toStdString()) + " VALUES(";
    for(size_t i=1;i<=fieldList.size();i++)
        sQuery += "?" + std::to_string(i) + ",";
    sQuery.pop_back();  // Remove last comma
    sQuery.append(")");
    sqlite3_stmt* stmt;
    auto pDb = pdb->get(tr("importing CSV"));
    if(sqlite3_prepare_v2(pDb.get(), sQuery.c_str(), static_cast<int>(sQuery.size()), &stmt, nullptr) != SQLITE_OK)
    {
        rollback(this, pdb, nullptr, restorepointName, 0, tr("Could not prepare INSERT statement: %1").arg(pdb->lastError()));
        return false;
    }

    // Parse entire file
    size_t lastRowNum = 0;
    CSVParser::ParserResult result = parseCSV(fileName, [&](size_t rowNum, const CSVRow& rowData) -> bool {
        // Process the parser results row by row

#ifdef CSV_BENCHMARK
        qint64 timeAtStartOfRowFunction = timer.elapsed();
#endif

        // Save row num for later use. This is used in the case of an error to tell the user in which row the error occurred
        lastRowNum = rowNum;

        // If this is the first row and we want to use the first row as table header, skip it now because this is the data import, not the header parsing
        if(rowNum == 0 && ui->checkboxHeader->isChecked())
            return true;

        // Bind all values
        for(size_t i=0;i<rowData.num_fields;i++)
        {
            // Empty values need special treatment
            // When importing into an existing table where we could find out something about its table definition
            if(importToExistingTable && rowData.fields[i].data_length == 0 && nullValues.size() > i)
            {
                // Do we want to fail when trying to import an empty value into this field? Then exit with an error.
                if(failOnMissingFieldList.at(i))
                    return false;

                // This is an empty value. We'll need to look up how to handle it depending on the field to be inserted into.
                const QByteArray& val = nullValues.at(i);
                if(!val.isNull())       // No need to bind NULL values here as that is the default bound value in SQLite
                    sqlite3_bind_text(stmt, static_cast<int>(i)+1, val, val.size(), SQLITE_STATIC);
            // When importing into a new table, use the missing values setting directly
            } else if(!importToExistingTable && rowData.fields[i].data_length == 0) {
                // No need to bind NULL values here as that is the default bound value in SQLite
            } else {
                // This is a non-empty value, or we want to insert the empty string. Just add it to the statement.
                bool convert_ok = false;
                if(ui->checkLocalConventions->isChecked()) {
                    // Find the correct data type taking into account the locale.
                    QString content = QString::fromUtf8(rowData.fields[i].data, static_cast<int>(rowData.fields[i].data_length));
                    sqlite_int64 int64_value = QLocale::system().toLongLong(content, &convert_ok);
                    if(convert_ok) {
                        sqlite3_bind_int64(stmt, static_cast<int>(i)+1, int64_value);
                    } else {
                        double value = QLocale::system().toDouble(content, &convert_ok);
                        if(convert_ok)
                            sqlite3_bind_double(stmt, static_cast<int>(i)+1, value);
                    }
                }

                if(!convert_ok) {
                    // If we don't find any better data type or we want SQLite to apply the type affinity
                    // (impossible when using local conventions), we fall back to the TEXT data type.
                    sqlite3_bind_text(stmt, static_cast<int>(i)+1, rowData.fields[i].data, static_cast<int>(rowData.fields[i].data_length), SQLITE_STATIC);
                }
            }
        }

        // Insert row
        if(sqlite3_step(stmt) != SQLITE_DONE)
            return false;

        // Reset statement for next use. Also reset all bindings to NULL. This is important, so we don't need to bind missing columns or empty values in NULL
        // columns manually.
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);

#ifdef CSV_BENCHMARK
        timesRowFunction += timer.elapsed() - timeAtStartOfRowFunction;
#endif

        return true;
    });

    // Success?
    if(result != CSVParser::ParserResult::ParserResultSuccess)
    {
        // Some error occurred or the user cancelled the action

        // Rollback the entire import. If the action was cancelled, don't show an error message. If it errored, show an error message.
        QString message;
        if(result == CSVParser::ParserResult::ParserResultError)
        {
            QString error(sqlite3_errmsg(pDb.get()));
            message = tr("Inserting row failed: %1").arg(error);
        } else if(result == CSVParser::ParserResult::ParserResultUnexpectedEOF) {
            message = tr("Unexpected end of file. Please make sure that you have configured the correct quote characters and "
                         "the file is not malformed.");
        }

        sqlite3_finalize(stmt);
        rollback(this, pdb, &pDb, restorepointName, lastRowNum, message);
        return false;
    }

    // Clean up prepared statement
    sqlite3_finalize(stmt);

#ifdef CSV_BENCHMARK
    QMessageBox::information(this, qApp->applicationName(),
                             tr("Importing the file '%1' took %2ms. Of this %3ms were spent in the row function.")
                             .arg(fileName)
                             .arg(timer.elapsed())
                             .arg(timesRowFunction));
#endif

    return true;
}

void ImportCsvDialog::setQuoteChar(QChar c)
{
    QComboBox* combo = ui->comboQuote;
    int index = combo->findText(QString(c));
    ui->spinBoxQuote->setValue(c.unicode());
    if(index == -1)
    {
        if(c.isPrint())
        {
            combo->setCurrentIndex(combo->count() - OtherPrintable);
            ui->editCustomQuote->setText(QString(c));
        }
        else
        {
            combo->setCurrentIndex(combo->count() - OtherCode);
        }
    }
    else
    {
        combo->setCurrentIndex(index);
    }
}

QChar ImportCsvDialog::currentQuoteChar() const
{
    QString value;

    // The last item in the combobox is the 'Other' item; if it is selected return the text of the line edit field instead
    if(ui->comboQuote->currentIndex() == ui->comboQuote->count() - OtherPrintable)
        value = ui->editCustomQuote->text().length() ? ui->editCustomQuote->text() : "";
    else if(ui->comboQuote->currentIndex() == ui->comboQuote->count() - OtherCode)
        value = QString(QChar(ui->spinBoxQuote->value()));
    else if(ui->comboQuote->currentText().length())
        value = ui->comboQuote->currentText();

    return value.size() ? value.at(0) : QChar();
}

void ImportCsvDialog::setSeparatorChar(QChar c)
{
    QComboBox* combo = ui->comboSeparator;
    QString sText = c == '\t' ? QString("Tab") : QString(c);
    int index = combo->findText(sText);
    ui->spinBoxSeparator->setValue(c.unicode());
    if(index == -1)
    {
        if(c.isPrint())
        {
            combo->setCurrentIndex(combo->count() - OtherPrintable);
            ui->editCustomSeparator->setText(QString(c));
        }
        else
        {
            combo->setCurrentIndex(combo->count() - OtherCode);
        }
    }
    else
    {
        combo->setCurrentIndex(index);
    }
}

QChar ImportCsvDialog::currentSeparatorChar() const
{
    QString value;

    // The last options in the combobox are the 'Other (*)' items;
    // if one of them is selected return the text or code of the corresponding field instead
    if(ui->comboSeparator->currentIndex() == ui->comboSeparator->count() - OtherPrintable)
        value = ui->editCustomSeparator->text().length() ? ui->editCustomSeparator->text() : "";
    else if(ui->comboSeparator->currentIndex() == ui->comboSeparator->count() - OtherCode)
        value = QString(QChar(ui->spinBoxSeparator->value()));
    else
        value = ui->comboSeparator->currentText() == tr("Tab") ? "\t" : ui->comboSeparator->currentText();

    return value.size() ? value.at(0) : QChar();
}

void ImportCsvDialog::setEncoding(const QString& sEnc)
{
    QComboBox* combo = ui->comboEncoding;
    int index = combo->findText(sEnc);
    if(index == -1)
    {
        combo->setCurrentIndex(combo->count() - 1);
        ui->editCustomEncoding->setText(sEnc);
    }
    else
    {
        combo->setCurrentIndex(index);
    }
}

QString ImportCsvDialog::currentEncoding() const
{
    // The last item in the combobox is the 'Other' item; if it is selected return the text of the line edit field instead
    if(ui->comboEncoding->currentIndex() == ui->comboEncoding->count()-1)
        return ui->editCustomEncoding->text().length() ? ui->editCustomEncoding->text() : "UTF-8";
    else
        return ui->comboEncoding->currentText();
}

std::string ImportCsvDialog::currentOnConflictStrategy() const
{
    switch(ui->comboOnConflictStrategy->currentIndex())
    {
    case 1:
        return "OR IGNORE";
    case 2:
        return "OR REPLACE";
    default:
        return {};
    }
}

void ImportCsvDialog::toggleAdvancedSection(bool show)
{
    ui->labelNoTypeDetection->setVisible(show);
    ui->checkNoTypeDetection->setVisible(show);
    ui->labelLocalConventions->setVisible(show);
    ui->checkLocalConventions->setVisible(show);
    ui->labelFailOnMissing->setVisible(show);
    ui->checkFailOnMissing->setVisible(show);
    ui->labelIgnoreDefaults->setVisible(show);
    ui->checkIgnoreDefaults->setVisible(show);
    ui->labelOnConflictStrategy->setVisible(show);
    ui->comboOnConflictStrategy->setVisible(show);
}

char32_t ImportCsvDialog::toUtf8(const QString& s) const
{
    if(s.isEmpty())
        return 0;

    QByteArray ba = s.toUtf8();

    char32_t result = 0;
    for(int i=std::min(ba.size()-1,3);i>=0;i--)
        result = (result << 8) + static_cast<unsigned char>(ba.at(i));

    return result;
}
