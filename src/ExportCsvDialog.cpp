#include "ExportCsvDialog.h"
#include "ui_ExportCsvDialog.h"
#include "sqlitedb.h"
#include "PreferencesDialog.h"
#include "sqlitetablemodel.h"
#include "sqlite.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>

ExportCsvDialog::ExportCsvDialog(DBBrowserDB* db, QWidget* parent, const QString& query, const QString& selection)
    : QDialog(parent),
      ui(new Ui::ExportCsvDialog),
      pdb(db),
      m_sQuery(query)
{
    // Create UI
    ui->setupUi(this);

    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
    ui->checkHeader->setChecked(settings.value("exportcsv/firstrowheader", true).toBool());
    setSeparatorChar(QChar(settings.value("exportcsv/separator", ',').toInt()));
    setQuoteChar(QChar(settings.value("exportcsv/quotecharacter", '"').toInt()));

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
        // save settings
        QSettings settings(QApplication::organizationName(), QApplication::organizationName());
        settings.beginGroup("exportcsv");
        settings.setValue("firstrowheader", ui->checkHeader->isChecked());
        settings.setValue("separator", currentSeparatorChar());
        settings.setValue("quotecharacter", currentQuoteChar());
        settings.endGroup();

        // Create select statement when exporting an entire table
        if(m_sQuery.isEmpty())
        {
            m_sQuery = QString("SELECT * from `%1`;").arg(ui->comboTable->currentText());
        }

        // Prepare the quote and separating characters
        QChar quoteChar = currentQuoteChar();
        QString quotequoteChar = QString(quoteChar) + quoteChar;
        QChar sepChar = currentSeparatorChar();
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
                        if(content.contains(quoteChar) || content.contains(sepChar) || content.contains('\n'))
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

void ExportCsvDialog::setQuoteChar(const QChar& c)
{
    QComboBox* combo = ui->comboQuoteCharacter;
    int index = combo->findText(c);
    if(index == -1)
    {
        combo->setCurrentIndex(combo->count());
        ui->editCustomQuote->setText(c);
    }
    else
    {
        combo->setCurrentIndex(index);
    }
}

char ExportCsvDialog::currentQuoteChar() const
{
    // The last item in the combobox is the 'Other' item; if it is selected return the text of the line edit field instead
    if(ui->comboQuoteCharacter->currentIndex() == ui->comboQuoteCharacter->count()-1)
        return ui->editCustomQuote->text().length() ? ui->editCustomQuote->text().at(0).toLatin1() : 0;

    if(ui->comboQuoteCharacter->currentText().length())
        return ui->comboQuoteCharacter->currentText().at(0).toLatin1();
    else
        return 0;
}

void ExportCsvDialog::setSeparatorChar(const QChar& c)
{
    QComboBox* combo = ui->comboFieldSeparator;
    QString sText = c == '\t' ? QString("Tab") : QString(c);
    int index = combo->findText(sText);
    if(index == -1)
    {
        combo->setCurrentIndex(combo->count());
        ui->editCustomSeparator->setText(c);
    }
    else
    {
        combo->setCurrentIndex(index);
    }
}

char ExportCsvDialog::currentSeparatorChar() const
{
    // The last item in the combobox is the 'Other' item; if it is selected return the text of the line edit field instead
    if(ui->comboFieldSeparator->currentIndex() == ui->comboFieldSeparator->count()-1)
        return ui->editCustomSeparator->text().length() ? ui->editCustomSeparator->text().at(0).toLatin1() : 0;

    return ui->comboFieldSeparator->currentText() == tr("Tab") ? '\t' : ui->comboFieldSeparator->currentText().at(0).toLatin1();
}
