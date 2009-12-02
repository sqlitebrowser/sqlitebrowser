#include "importcsvform.h"

#include <qvariant.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qimage.h>
#include <qpixmap.h>
#include <QProgressDialog>

/*
 *  Constructs a importCSVForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
importCSVForm::importCSVForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
importCSVForm::~importCSVForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void importCSVForm::languageChange()
{
    retranslateUi(this);
}

void importCSVForm::init()
{
    pdb = 0;
    sep=',';
    quote='"';
}
void importCSVForm::initialize(QString & csvfile, DBBrowserDB * db)
{
    pdb = db;
    csvfilename = csvfile;
    preview();
}

void importCSVForm::createButtonPressed()
{
    QString tabname;
    QStringList fieldList;
    QString sql;

    //minimun validation for tabname
    tabname = tableNameEdit->text();
    if (tabname.isEmpty()) {
        QMessageBox::information( this, applicationName, "Please choose a name for the new table that will hold the csv data" );
        return;
    }
    tabname.replace(" ", "");
    tabname.replace('"', "");
    tabname.replace("'","");
    tabname.replace(",","");
    tabname.replace(";","");

    if (tabname.isEmpty()) {
        tabname = "tempTable";
    }

    //parse all csv data
    curList = pdb->decodeCSV(csvfilename, sep, quote, -1, &numfields);

     //can not operate on an empty result
    if (numfields==0) return;

    if (extractFieldNamesCheckbox->isChecked())
    {
        int cfieldnum = 0;
        for ( QStringList::Iterator ct = curList.begin(); ct != curList.end(); ++ct ) 		{
            QString thisfield = *ct;
            //basic conforming
            thisfield.replace(" ", "");
            thisfield.replace('"', "");
            thisfield.replace("'","");
            thisfield.replace(",","");
            thisfield.replace(";","");

            if (thisfield.isEmpty()) thisfield.append("field");

            fieldList << thisfield;
            cfieldnum++;
            if (cfieldnum==numfields) break;
        }
        //pop the fieldnames
        for (int e=0; e<numfields; e++)
        {
            curList.pop_front();
        }
    } else {
        //generate temp fieldnames
        for (int e=0; e<numfields; e++)
        {
            fieldList << QString("field").append(QString::number(e+1,10));
        }
    }

    QProgressDialog progress("Inserting data...", "Cancel", 0, curList.size());
    progress.setWindowModality(Qt::ApplicationModal);
    
    sql = "CREATE TABLE ";
    sql.append(tabname);
    sql.append(" (");
    for (int r=0; r<numfields;r++){
        sql.append(fieldList[r]);
        //createStatement.append(" text");
        if (r<(numfields - 1))
            sql.append(", ");
    }
    sql.append(");");

    //declare local variables we will need before the rollback jump
    int colNum = 0;

    //begin a transaction, so we can rollback in case of any errors during importing
    //db needs to be saved or an error will occur
    if (!pdb->executeSQLDirect(QString("BEGIN TRANSACTION RESTOREPOINT;"))) goto rollback;

    //execute the create table statement
    if (!pdb->executeSQLDirect(sql)) goto rollback;

        {//avoid error on MSVC due to rollback label
    //now lets import all data, one row at a time
    for ( int i=0; i < curList.size(); ++i ) {
        if (colNum==0)
        {
            sql = "INSERT INTO ";
            sql.append(tabname);
            sql.append(" VALUES(");
        }
        //need to mprintf here
        //sql.append(*ct);
        char * formSQL = sqlite3_mprintf("%Q",(const char *) curList[i]);
        sql.append(formSQL);
        if (formSQL) sqlite3_free(formSQL);

        colNum++;
        if (colNum<numfields)
        {
            sql.append(",");
        } else {
            colNum = 0;
            sql.append(");");
            if (!pdb->executeSQLDirect(sql)) goto rollback;
        }
        progress.setValue(i);
        if (progress.wasCanceled()) goto rollback;

    }
        }

    //everything ok, just return
    //if (!pdb->executeSQL(QString("COMMIT TRANSACTION RESTOREPOINT;"))) goto rollback;
    pdb->setDirtyDirect(true);
   QApplication::restoreOverrideCursor();  // restore original cursor
   accept();
   return;

   rollback:
       progress.cancel();
       QApplication::restoreOverrideCursor();  // restore original cursor
       QString error = "Error importing data. Message from database engine:  ";
       error.append(pdb->lastErrorMessage);
       QMessageBox::warning( this, applicationName, error );
       //we will uncomment this when SQLite support nested transactions
       pdb->executeSQLDirect(QString("ROLLBACK TRANSACTION  RESTOREPOINT;"));
}

void importCSVForm::preview()
{
    //get only 20 lines, for preview
    int maxrecs = 20;
    curList = pdb->decodeCSV(csvfilename, sep, quote, maxrecs, &numfields);

    //qDebug("count = %d, numfields = %d", curList .count(), numfields);

    previewTable->setNumRows(0);
    previewTable->setNumCols(0);
    previewTable->setNumCols(numfields);
    int cheadnum = 0;

    //can not operate on an empty result
    if (numfields==0) return;

    if (extractFieldNamesCheckbox->isChecked())
    {
        for ( QStringList::Iterator ct = curList.begin(); ct != curList.end(); ++ct ) {
            previewTable->horizontalHeader()->setLabel( cheadnum, *ct  );
            cheadnum++;
            if (cheadnum==numfields) break;
        }
        //pop the fieldnames
        for (int e=0; e<numfields; e++)
        {
            curList.pop_front();
        }
    }
    previewTable->setNumRows(curList.count()/numfields);
    int rowNum = 0;
    int colNum = 0;
    for ( QStringList::Iterator ct = curList .begin(); ct != curList .end(); ++ct ) {
        if (colNum==0) previewTable->verticalHeader()->setLabel( rowNum, QString::number(rowNum,10) );
        previewTable->setText( rowNum, colNum,*ct);
        colNum++;
        if (colNum==numfields)
        {
            colNum = 0;
            rowNum ++;
        }
    }
}


void importCSVForm::fieldSeparatorChanged()
{
    QString curText =fieldBox->currentText();
    if (curText.compare(QString("TAB"))==0)
    {
        sep = 9;
    } else {
    QChar qsep = curText.at(0);
    sep = (char) qsep.ascii();
}
    preview();
}


void importCSVForm::textQuoteChanged()
{
    QString curText = quoteBox->currentText();
    QChar qquote = curText.at(0);
        quote = (char) qquote.ascii();
    preview();
}


void importCSVForm::extractFieldNamesChanged( bool enabled )
{
    preview();
}


