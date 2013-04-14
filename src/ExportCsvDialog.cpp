#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include "ExportCsvDialog.h"
#include "ui_ExportCsvDialog.h"
#include "sqlitedb.h"
#include "PreferencesDialog.h"
#include "sqlitetablemodel.h"

ExportCsvDialog::ExportCsvDialog(DBBrowserDB* db, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::ExportCsvDialog),
      pdb(db)
{
    // Create UI
    ui->setupUi(this);

    // Get list of tables to export
    objectMap objects = pdb->getBrowsableObjects();
    for(objectMap::ConstIterator i=objects.begin();i!=objects.end();++i)
        ui->comboTable->addItem(QIcon(QString(":icons/%1").arg(i.value().gettype())), i.value().getname());
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
        // Get data from selected table
        SqliteTableModel tableModel(this, pdb);
        tableModel.setTable(ui->comboTable->currentText());
        while(tableModel.canFetchMore())
            tableModel.fetchMore();

        // Prepare the quote and separating characters
        QString quoteChar = ui->comboQuoteCharacter->currentText();
        QString quotequoteChar = quoteChar + quoteChar;
        QString sepChar = ui->comboFieldSeparator->currentText();
        if(sepChar == tr("Tab")) sepChar = "\t";
        QString newlineChar = "\n";

        // Open file
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly))
        {
            // Open text stream to the file
            QTextStream stream(&file);

            // Put field names in first row if user wants to have them
            if(ui->checkHeader->isChecked())
            {
                for(int i=1;i<tableModel.columnCount();i++)
                {
                    stream << quoteChar << tableModel.headerData(i, Qt::Horizontal).toString() << quoteChar;
                    if(i < tableModel.columnCount() - 1)
                        stream << sepChar;
                    else
                        stream << newlineChar;
                }
            }

            // Get and write actual data
            for(int i=0;i<tableModel.totalRowCount();i++)
            {
                for(int j=1;j<tableModel.columnCount();j++)
                {
                    QString content = tableModel.data(tableModel.index(i, j)).toString();
                    stream << quoteChar << content.replace(quoteChar, quotequoteChar) << quoteChar;
                    if(j < tableModel.columnCount() - 1)
                        stream << sepChar;
                    else
                        stream << newlineChar;
                }
            }

            // Done writing the file
            file.close();
            QMessageBox::information(this, QApplication::applicationName(), tr("Export completed."));
            QDialog::accept();
        } else {
            QMessageBox::warning(this, QApplication::applicationName(), tr("Could not open output file."));
        }
    }
}
