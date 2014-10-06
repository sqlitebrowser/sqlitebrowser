#include "ExportCsvDialog.h"
#include "ui_ExportCsvDialog.h"
#include "sqlitedb.h"
#include "PreferencesDialog.h"
#include "sqlitetablemodel.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <sqlite3.h>

ExportCsvDialog::ExportCsvDialog(DBBrowserDB* db, QWidget* parent, const QString& query, const QString& selection)
    : QDialog(parent),
      ui(new Ui::ExportCsvDialog),
      pdb(db),
      m_sQuery(query)
{
    // Create UI
    ui->setupUi(this);
    showCustomCharEdits();

    // If a SQL query was specified hide the table combo box. If not fill it with tables to export
    if(query.isEmpty())
    {
        // Get list of tables to export
        objectMap objects = pdb->getBrowsableObjects();
        for(objectMap::ConstIterator i=objects.begin();i!=objects.end();++i)
            ui->comboTable->addItem(QIcon(QString(":icons/%1").arg(i.value().gettype())), i.value().getname());

        // Sort list of tables and select the table specified in the selection parameter or alternatively the first one
        ui->comboTable->model()->sort(0);
        if(selection.isEmpty())
            ui->comboTable->setCurrentIndex(0);
        else
            ui->comboTable->setCurrentIndex(ui->comboTable->findText(selection));
    } else {
        // Hide table combo box
        ui->labelTable->setVisible(false);
        ui->comboTable->setVisible(false);
    }
}

ExportCsvDialog::~ExportCsvDialog()
{
    delete ui;
}

void ExportCsvDialog::accept()
{
    // Get filename
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Choose a filename to export data"),
                PreferencesDialog::getSettingsValue("db", "defaultlocation").toString(),
                tr("Text files(*.csv *.txt)"));

    // Only if the user hasn't clicked the cancel button
    if(fileName.size() > 0)
    {
        if(m_sQuery.isEmpty())
        {
            m_sQuery = QString("SELECT * from `%1`;").arg(ui->comboTable->currentText());
        }

        // Prepare the quote and separating characters
        QString quoteChar = ui->comboQuoteCharacter->currentIndex() == ui->comboQuoteCharacter->count()-1
                    ? ui->editCustomQuote->text() : ui->comboQuoteCharacter->currentText();
        QString quotequoteChar = quoteChar + quoteChar;
        QString sepChar;
        if(ui->comboFieldSeparator->currentIndex() == ui->comboFieldSeparator->count()-1)
        {
            sepChar = ui->editCustomSeparator->text();
        } else {
            sepChar = ui->comboFieldSeparator->currentText();
            if(sepChar == tr("Tab")) sepChar = "\t";
        }
        QString newlineChar = "\r\n";

        // Open file
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly))
        {
            // Open text stream to the file
            QTextStream stream(&file);

            QByteArray utf8Query = m_sQuery.toUtf8();
            sqlite3_stmt *stmt;

            int status = sqlite3_prepare_v2(pdb->_db, utf8Query.data(), utf8Query.size(), &stmt, NULL);
            if(SQLITE_OK == status)
            {
                if(ui->checkHeader->isChecked())
                {
                    int columns = sqlite3_column_count(stmt);
                    for (int i = 0; i < columns; ++i)
                    {
                        QString content = QString::fromUtf8(sqlite3_column_name(stmt, i));
                        if(content.contains(quoteChar) || content.contains(newlineChar))
                            stream << quoteChar << content.replace(quoteChar, quotequoteChar) << quoteChar;
                        else
                            stream << content;
                        if(i != columns - 1)
                            stream << sepChar;
                    }
                    stream << newlineChar;
                }

                QApplication::setOverrideCursor(Qt::WaitCursor);
                int columns = sqlite3_column_count(stmt);
                size_t counter = 0;
                while(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    for (int i = 0; i < columns; ++i)
                    {
                        QString content = QString::fromUtf8(
                                    (const char*)sqlite3_column_blob(stmt, i),
                                    sqlite3_column_bytes(stmt, i));
                        if(content.contains(quoteChar) || content.contains("\r\n") || content.contains('\n'))
                            stream << quoteChar << content.replace(quoteChar, quotequoteChar) << quoteChar;
                        else
                            stream << content;
                        if(i != columns - 1)
                            stream << sepChar;
                    }
                    stream << newlineChar;
                    if(counter % 1000 == 0)
                        qApp->processEvents();
                    counter++;
                }
            }
            sqlite3_finalize(stmt);

            QApplication::restoreOverrideCursor();
            qApp->processEvents();

            // Done writing the file
            file.close();
            QMessageBox::information(this, QApplication::applicationName(), tr("Export completed."));
            QDialog::accept();
        } else {
            QMessageBox::warning(this, QApplication::applicationName(), tr("Could not open output file."));
        }
    }
}

void ExportCsvDialog::showCustomCharEdits()
{
    // Show/hide custom quote/separator input fields
    ui->editCustomQuote->setVisible(ui->comboQuoteCharacter->currentIndex() == ui->comboQuoteCharacter->count()-1);
    ui->editCustomSeparator->setVisible(ui->comboFieldSeparator->currentIndex() == ui->comboFieldSeparator->count()-1);
}
