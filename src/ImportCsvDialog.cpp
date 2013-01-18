#include "ImportCsvDialog.h"
#include "ui_ImportCsvDialog.h"
#include <QMessageBox>
#include <QProgressDialog>
#include <QPushButton>
#include "sqlitedb.h"

ImportCsvDialog::ImportCsvDialog(QString filename, DBBrowserDB* db, QWidget* parent)
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
    QList<DBBrowserField> fieldList;
    if(ui->checkboxHeader->isChecked())
    {
        int cfieldnum = 0;
        while(!curList.empty() && cfieldnum != numfields)
        {
            // Remove invalid characters
            QString thisfield = curList.front();
            thisfield.replace(" ", "");
            thisfield.replace('"', "");
            thisfield.replace("'","");
            thisfield.replace(",","");
            thisfield.replace(";","");

            // Avoid empty field names
            if(thisfield.isEmpty())
                thisfield = QString("field%1").arg(cfieldnum+1);

            fieldList.push_back(DBBrowserField(thisfield, ""));
            cfieldnum++;
            curList.pop_front();
        }
    } else {
        for(int i=0;i<numfields;i++)
            fieldList.push_back(DBBrowserField(QString("field%1").arg(i+1), ""));
    }

    // Show progress dialog
    QProgressDialog progress("Inserting data...", "Cancel", 0, curList.size());
    progress.setWindowModality(Qt::ApplicationModal);

    // declare local variables we will need before the rollback jump
    int colNum = 0;

    // Create a savepoint, so we can rollback in case of any errors during importing
    // db needs to be saved or an error will occur
    if(!pdb->executeSQL("SAVEPOINT CSVIMPORT;", false))
        goto rollback;

    // Create table
    if(!pdb->createTable(ui->editName->text(), fieldList))
        goto rollback;

    { // avoid error on MSVC due to rollback label
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
                    goto rollback;
            }
            progress.setValue(i);
            if(progress.wasCanceled())
                goto rollback;
        }
    }

    // Everything ok, release the savepoint
    if(!pdb->executeSQL("RELEASE SAVEPOINT CSVIMPORT;"))
        goto rollback;
    pdb->setDirtyDirect(true);
    QApplication::restoreOverrideCursor();  // restore original cursor
    QDialog::accept();
    return;

rollback:
    progress.hide();
    QApplication::restoreOverrideCursor();  // restore original cursor
    QString error = QString("Error importing data. Message from database engine: %1").arg(pdb->lastErrorMessage);
    QMessageBox::warning(this, QApplication::applicationName(), error);
    pdb->executeSQL("ROLLBACK TO SAVEPOINT CSVIMPORT;", false);
}

void ImportCsvDialog::updatePreview()
{
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
        for(int e=0;e<numfields;e++)
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
    ui->editName->setText(ui->editName->text().trimmed());

    bool valid = true;
    if(ui->editName->text().isEmpty() || ui->editName->text().contains(" "))
        valid = false;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

char ImportCsvDialog::currentQuoteChar()
{
    if(ui->comboQuote->currentText().length())
        return ui->comboQuote->currentText().at(0).toAscii();
    else
        return 0;
}

char ImportCsvDialog::currentSeparatorChar()
{
    return ui->comboSeparator->currentText() == "Tab" ? '\t' : ui->comboSeparator->currentText().at(0).toAscii();
}
