#include "ImportCsvDialog.h"
#include "ui_ImportCsvDialog.h"
#include "sqlitedb.h"

#include <QMessageBox>
#include <QProgressDialog>
#include <QPushButton>
#include <QDateTime>
#include <sqlite3.h>

ImportCsvDialog::ImportCsvDialog(const QString& filename, DBBrowserDB* db, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::ImportCsvDialog),
      csvFilename(filename),
      pdb(db)
{
    ui->setupUi(this);

    checkInput();
    updatePreview();
}

ImportCsvDialog::~ImportCsvDialog()
{
    delete ui;
}

namespace {
void rollback(ImportCsvDialog* dialog, DBBrowserDB* pdb, QProgressDialog& progress, const QString& savepointName)
{
    progress.hide();
    QApplication::restoreOverrideCursor();  // restore original cursor
    QString error = QObject::tr("Error importing data. Message from database engine: %1").arg(pdb->lastErrorMessage);
    QMessageBox::warning(dialog, QApplication::applicationName(), error);
    pdb->revert(savepointName);
}
}

void ImportCsvDialog::accept()
{
    QString sql;

    // Parse all csv data
    int numfields;
    QStringList curList = pdb->decodeCSV(csvFilename, currentSeparatorChar(), currentQuoteChar(), -1, &numfields);

    // Can not operate on an empty result
    if(numfields == 0)
        return;

    // Generate field names. These are either taken from the first CSV row or are generated in the format of "fieldXY" depending on the user input
    sqlb::FieldVector fieldList;
    if(ui->checkboxHeader->isChecked())
    {
        int cfieldnum = 0;
        while(!curList.empty() && cfieldnum != numfields)
        {
            // Remove invalid characters
            QString thisfield = curList.front();
            thisfield.replace("`", "");
            thisfield.replace(" ", "");
            thisfield.replace('"', "");
            thisfield.replace("'","");
            thisfield.replace(",","");
            thisfield.replace(";","");

            // Avoid empty field names
            if(thisfield.isEmpty())
                thisfield = QString("field%1").arg(cfieldnum+1);

            fieldList.push_back(sqlb::FieldPtr(new sqlb::Field(thisfield, "")));
            cfieldnum++;
            curList.pop_front();
        }
    } else {
        for(int i=0; i < numfields; ++i)
            fieldList.push_back(sqlb::FieldPtr(new sqlb::Field(QString("field%1").arg(i+1), "")));
    }

    // Show progress dialog
    QProgressDialog progress(tr("Inserting data..."), tr("Cancel"), 0, curList.size());
    progress.setWindowModality(Qt::ApplicationModal);

    // declare local variables we will need before the rollback jump
    int colNum = 0;

    // Are we importing into an existing table?
    bool importToExistingTable = false;
    objectMap objects = pdb->getBrowsableObjects();
    for(objectMap::ConstIterator i=objects.begin();i!=objects.end();++i)
    {
        if(i.value().gettype() == "table" && i.value().getname() == ui->editName->text())
        {
            if(i.value().table.fields().size() != numfields)
            {
                QMessageBox::warning(this, QApplication::applicationName(),
                                     tr("There is already a table of that name and an import into an existing table is only possible if the number of columns match."));
                return;
            } else {
                if(QMessageBox::question(this, QApplication::applicationName(), tr("There is already a table of that name. Do you want to import the data into it?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
                {
                    importToExistingTable = true;
                    break;
                } else {
                    return;
                }
            }
        }
    }

    // Create a savepoint, so we can rollback in case of any errors during importing
    // db needs to be saved or an error will occur
    QString restorepointName = QString("CSVIMPORT_%1").arg(QDateTime::currentMSecsSinceEpoch());
    if(!pdb->setRestorePoint(restorepointName))
        return rollback(this, pdb, progress, restorepointName);

    // Create table
    if(!importToExistingTable)
    {
        if(!pdb->createTable(ui->editName->text(), fieldList))
            return rollback(this, pdb, progress, restorepointName);
    }

    // now lets import all data, one row at a time
    for(int i=0;i<curList.size();++i)
    {
        if(colNum == 0)
            sql = QString("INSERT INTO `%1` VALUES(").arg(ui->editName->text());

        // need to mprintf here
        char* formSQL = sqlite3_mprintf("%Q", (const char*)curList[i].toUtf8());
        sql.append(formSQL);
        if(formSQL)
            sqlite3_free(formSQL);

        colNum++;
        if(colNum < numfields)
        {
            sql.append(",");
        } else {
            colNum = 0;
            sql.append(");");
            if(!pdb->executeSQL(sql, false, false))
                return rollback(this, pdb, progress, restorepointName);
        }
        progress.setValue(i);
        if(progress.wasCanceled())
            return rollback(this, pdb, progress, restorepointName);
    }

    QApplication::restoreOverrideCursor();  // restore original cursor
    QDialog::accept();
}

void ImportCsvDialog::updatePreview()
{
    // Show/hide custom quote/separator input fields
    ui->editCustomQuote->setVisible(ui->comboQuote->currentIndex() == ui->comboQuote->count()-1);
    ui->editCustomSeparator->setVisible(ui->comboSeparator->currentIndex() == ui->comboSeparator->count()-1);

    // Get preview data
    int numfields;
    int maxrecs = 20;
    QStringList curList = pdb->decodeCSV(csvFilename, currentSeparatorChar(), currentQuoteChar(), maxrecs, &numfields);

    // Reset preview widget
    ui->tablePreview->clear();
    ui->tablePreview->setColumnCount(numfields);

    // Exit if there are no lines to preview at all
    if(numfields == 0)
        return;

    // Use first row as header if necessary
    if(ui->checkboxHeader->isChecked())
    {
        ui->tablePreview->setHorizontalHeaderLabels(curList);

        // Remove this row to not show it in the data section
        for(int e=0;e < numfields; ++e)
            curList.pop_front();
    }

    // Fill data section
    ui->tablePreview->setRowCount(curList.count() / numfields);
    int rowNum = 0;
    int colNum = 0;
    for(QStringList::Iterator ct=curList.begin();ct!=curList.end();++ct)
    {
        if(colNum == 0)
            ui->tablePreview->setVerticalHeaderItem(rowNum, new QTableWidgetItem(QString::number(rowNum + 1)));
        ui->tablePreview->setItem(rowNum, colNum, new QTableWidgetItem(*ct));
        colNum++;
        if(colNum == numfields)
        {
            colNum = 0;
            rowNum++;
        }
    }
}

void ImportCsvDialog::checkInput()
{
    bool valid = true;
    if(ui->editName->text().isEmpty() || ui->editName->text().contains("`"))
        valid = false;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

char ImportCsvDialog::currentQuoteChar()
{
    // The last item in the combobox is the 'Other' item; if it is selected return the text of the line edit field instead
    if(ui->comboQuote->currentIndex() == ui->comboQuote->count()-1)
        return ui->editCustomQuote->text().length() ? ui->editCustomQuote->text().at(0).toLatin1() : 0;

    if(ui->comboQuote->currentText().length())
        return ui->comboQuote->currentText().at(0).toLatin1();
    else
        return 0;
}

char ImportCsvDialog::currentSeparatorChar()
{
    // The last item in the combobox is the 'Other' item; if it is selected return the text of the line edit field instead
    if(ui->comboSeparator->currentIndex() == ui->comboSeparator->count()-1)
        return ui->editCustomSeparator->text().length() ? ui->editCustomSeparator->text().at(0).toLatin1() : 0;

    return ui->comboSeparator->currentText() == tr("Tab") ? '\t' : ui->comboSeparator->currentText().at(0).toLatin1();
}
