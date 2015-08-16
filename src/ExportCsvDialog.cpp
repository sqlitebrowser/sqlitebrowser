#include "ExportCsvDialog.h"
#include "ui_ExportCsvDialog.h"
#include "sqlitedb.h"
#include "PreferencesDialog.h"
#include "sqlitetablemodel.h"
#include "sqlite.h"
#include "FileDialog.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
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
        {
            ui->listTables->addItem(new QListWidgetItem(QIcon(QString(":icons/%1").arg(i.value().gettype())), i.value().getname()));
        }

        // Sort list of tables and select the table specified in the selection parameter or alternatively the first one
        ui->listTables->model()->sort(0);
        if(selection.isEmpty())
        {
            ui->listTables->setCurrentItem(ui->listTables->item(0));
        }
        else
        {
            QList<QListWidgetItem*> items = ui->listTables->findItems(selection, Qt::MatchExactly);
            ui->listTables->setCurrentItem(items.at(0));
        }
    } else {
        // Hide table combo box
        ui->labelTable->setVisible(false);
        ui->listTables->setVisible(false);
        resize(minimumSize());
    }
}

ExportCsvDialog::~ExportCsvDialog()
{
    delete ui;
}

bool ExportCsvDialog::exportQuery(const QString& sQuery, const QString& sFilename)
{
    // Prepare the quote and separating characters
    QChar quoteChar = currentQuoteChar();
    QString quotequoteChar = QString(quoteChar) + quoteChar;
    QChar sepChar = currentSeparatorChar();
    QString newlineChar = "\r\n";

    // chars that require escaping
    std::string special_chars = newlineChar.toStdString() + sepChar.toLatin1() + quoteChar.toLatin1();

    // Open file
    QFile file(sFilename);
    if(file.open(QIODevice::WriteOnly))
    {
        // Open text stream to the file
        QTextStream stream(&file);

        QByteArray utf8Query = sQuery.toUtf8();
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
                    if(content.toStdString().find_first_of(special_chars) != std::string::npos)
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
                    if(content.toStdString().find_first_of(special_chars) != std::string::npos)
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
    } else {
        QMessageBox::warning(this, QApplication::applicationName(),
                             tr("Could not open output file: %1").arg(sFilename));
        return false;
    }

    return true;
}

void ExportCsvDialog::accept()
{
    if(!m_sQuery.isEmpty())
    {
        // called from sqlexecute query tab
        QString sFilename = FileDialog::getSaveFileName(
                this,
                tr("Choose a filename to export data"),
                tr("Text files(*.csv *.txt)"));
        if(sFilename.isEmpty())
        {
            close();
            return;
        }

        exportQuery(m_sQuery, sFilename);
    }
    else
    {
        // called from the File export menu
        QList<QListWidgetItem*> selectedItems = ui->listTables->selectedItems();

        if(selectedItems.isEmpty())
        {
            QMessageBox::warning(this, QApplication::applicationName(),
                                 tr("Please select at least 1 table."));
            return;
        }

        // Get filename
        QStringList filenames;
        if(selectedItems.size() == 1)
        {
            QString fileName = FileDialog::getSaveFileName(
                    this,
                    tr("Choose a filename to export data"),
                    tr("Text files(*.csv *.txt)"));
            if(fileName.isEmpty())
            {
                close();
                return;
            }

            filenames << fileName;
        }
        else
        {
            // ask for folder
            QString csvfolder = FileDialog::getExistingDirectory(
                        this,
                        tr("Choose a directory"),
                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

            if(csvfolder.isEmpty())
            {
                close();
                return;
            }

            for(QList<QListWidgetItem*>::iterator it = selectedItems.begin(); it != selectedItems.end(); ++it)
            {
                filenames << QDir(csvfolder).filePath((*it)->text() + ".csv");
            }
        }

        // Only if the user hasn't clicked the cancel button
        for(int i = 0; i < selectedItems.size(); ++i)
        {
            // if we are called from execute sql tab, query is already set
            // and we only export 1 select
            QString sQuery = QString("SELECT * FROM %1;").arg(sqlb::escapeIdentifier(selectedItems.at(i)->text()));

            exportQuery(sQuery, filenames.at(i));
        }
    }

    // save settings
    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
    settings.beginGroup("exportcsv");
    settings.setValue("firstrowheader", ui->checkHeader->isChecked());
    settings.setValue("separator", currentSeparatorChar());
    settings.setValue("quotecharacter", currentQuoteChar());
    settings.endGroup();


    QMessageBox::information(this, QApplication::applicationName(), tr("Export completed."));
    QDialog::accept();
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
