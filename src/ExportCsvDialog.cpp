#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include "ExportCsvDialog.h"
#include "ui_ExportCsvDialog.h"
#include "sqlitedb.h"

ExportCsvDialog::ExportCsvDialog(DBBrowserDB* db, const QString& deflocation, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::ExportCsvDialog),
      pdb(db),
      defaultLocation(deflocation)
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
                defaultLocation,
                tr("Text files(*.csv *.txt)"));

    // Only if the user hasn't clicked the cancel button
    if(fileName.size() > 0)
    {
        // Get data from selected table
        pdb->browseTable(ui->comboTable->currentText());

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
                QStringList fields = pdb->browseFields;
                for(int i=0;i<fields.count();i++)
                {
                    stream << quoteChar << fields.at(i) << quoteChar;
                    if(i < fields.count() - 1)
                        stream << sepChar;
                    else
                        stream << newlineChar;
                }
            }

            // Get and write actual data
            rowList data = pdb->browseRecs;
            for(int i=0;i<data.size();i++)
            {
                QList<QByteArray> row = data[i];
                for(int j=1;j<row.size();j++)
                {
                    QString content = row[j];
                    stream << quoteChar << content.replace(quoteChar, quotequoteChar) << quoteChar;
                    if(j < row.count() - 1)
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
