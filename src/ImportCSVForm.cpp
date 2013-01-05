#include "ImportCSVForm.h"
#include <QMessageBox>
#include <QProgressDialog>
#include "sqlitedb.h"

/*
 *  Constructs a importCSVForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
importCSVForm::importCSVForm(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
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
        QMessageBox::information( this, QApplication::applicationName(), "Please choose a name for the new table that will hold the csv data" );
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
    
    sql = QString("CREATE TABLE `%1` (").arg(tabname);
    for (int r=0; r<numfields;r++){
        sql.append(QString("`%1`").arg(fieldList[r]));
        //createStatement.append(" text");
        if (r<(numfields - 1))
            sql.append(", ");
    }
    sql.append(");");

    //declare local variables we will need before the rollback jump
    int colNum = 0;

    //begin a savepoint, so we can rollback in case of any errors during importing
    //db needs to be saved or an error will occur
    if (!pdb->executeSQL(QString("SAVEPOINT CSVIMPORT;"), false, false)) goto rollback;

    //execute the create table statement
    if (!pdb->executeSQL(sql, false, false)) goto rollback;

        {//avoid error on MSVC due to rollback label
    //now lets import all data, one row at a time
    for ( int i=0; i < curList.size(); ++i ) {
        if (colNum==0)
            sql = QString("INSERT INTO `%1` VALUES(").arg(tabname);

        //need to mprintf here
        //sql.append(*ct);
        char * formSQL = sqlite3_mprintf("%Q",(const char *) curList[i].toUtf8());
        sql.append(formSQL);
        if (formSQL) sqlite3_free(formSQL);

        colNum++;
        if (colNum<numfields)
        {
            sql.append(",");
        } else {
            colNum = 0;
            sql.append(");");
            if (!pdb->executeSQL(sql, false, false)) goto rollback;
        }
        progress.setValue(i);
        if (progress.wasCanceled()) goto rollback;

    }
        }

    //everything ok, just return
    //Do not commit, it will be done automatically on save
    if (!pdb->executeSQL(QString("RELEASE CSVIMPORT;"))) goto rollback;
    pdb->setDirtyDirect(true);
   QApplication::restoreOverrideCursor();  // restore original cursor
   accept();
   return;

   rollback:
       progress.cancel();
       QApplication::restoreOverrideCursor();  // restore original cursor
       QString error = "Error importing data. Message from database engine:  ";
       error.append(pdb->lastErrorMessage);
       QMessageBox::warning( this, QApplication::applicationName(), error );
       //we will uncomment this when SQLite support nested transactions
       pdb->executeSQL(QString("ROLLBACK TO SAVEPOINT CSVIMPORT;"), false, false);
}

void importCSVForm::preview()
{
    //get only 20 lines, for preview
    int maxrecs = 20;
    curList = pdb->decodeCSV(csvfilename, sep, quote, maxrecs, &numfields);

    previewTable->clear();
    previewTable->setColumnCount(numfields);

    //can not operate on an empty result
    if (numfields==0) return;

    if (extractFieldNamesCheckbox->isChecked())
    {
        previewTable->setHorizontalHeaderLabels(curList);
        //pop the fieldnames
        for (int e=0; e<numfields; e++)
        {
            curList.pop_front();
        }
    }
    previewTable->setRowCount(curList.count()/numfields);
    int rowNum = 0;
    int colNum = 0;
    for ( QStringList::Iterator ct = curList .begin(); ct != curList .end(); ++ct ) {
        if (colNum==0) previewTable->setVerticalHeaderItem( rowNum, new QTableWidgetItem( QString::number(rowNum+1) ) );
        previewTable->setItem(rowNum, colNum, new QTableWidgetItem( *ct ) );
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
    sep = (char) qsep.toAscii();
}
    preview();
}


void importCSVForm::textQuoteChanged()
{
    QString curText = quoteBox->currentText();
    if(curText.length() > 0)
    {
        QChar qquote = curText.at(0);
        quote = (char) qquote.toAscii();
    }
    preview();
}


void importCSVForm::extractFieldNamesChanged( bool enabled )
{
    preview();
}


