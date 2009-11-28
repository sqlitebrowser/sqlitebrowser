#include "edittableform.h"

#include <qvariant.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qimage.h>
#include <qpixmap.h>

#include "renametableform.h"
#include "addfieldform.h"
#include "editfieldform.h"
/*
 *  Constructs a editTableForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
editTableForm::editTableForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
editTableForm::~editTableForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void editTableForm::languageChange()
{
    retranslateUi(this);
}

void editTableForm::init()
{
    pdb = NULL;
    modified = false;
}

void editTableForm::setActiveTable(DBBrowserDB * thedb, QString tableName)
{
    pdb = thedb;
    curTable = tableName;
    populateFields();
    tableLine->setText(curTable);
}

void editTableForm::populateFields()
{
    if (!pdb) return;

    //make sure we are not using cached information
    pdb->updateSchema();

    fields= pdb->getTableFields(curTable);
    types= pdb->getTableTypes(curTable);
    fieldListView->clear();
    fieldListView->setSorting (-1, FALSE);
    Q3ListViewItem * lasttbitem = 0;
    QStringList::Iterator tt = types.begin();
    for ( QStringList::Iterator ct = fields.begin(); ct != fields.end(); ++ct ) {
        Q3ListViewItem * tbitem = new Q3ListViewItem( fieldListView, lasttbitem);
        tbitem->setText( 0, *ct  );
        tbitem->setText( 1, *tt );
        lasttbitem = tbitem;
        ++tt;
    }
}

void editTableForm::renameTable()
{
    renameTableForm * renTableForm = new renameTableForm( this, "renametable", TRUE );
   renTableForm->setTableName(curTable);
   if (renTableForm->exec())
   {
       QApplication::setOverrideCursor( Qt::waitCursor ); // this might take time
       modified = true;
       QString newName = renTableForm->getTableName();
       qDebug(newName);
       QString sql;
       //do the sql rename here
       //if (!pdb->executeSQL(QString("BEGIN TRANSACTION;"))) goto rollback;

       sql = "CREATE TEMPORARY TABLE TEMP_TABLE(";
       Q3ListViewItemIterator it( fieldListView );
       Q3ListViewItem * item;
       while ( it.current() ) {
           item = it.current();
           sql.append(item->text(0));
           sql.append(" ");
           sql.append(item->text(1));
           if (item->nextSibling() != 0)
           {
               sql.append(", ");
            }
            ++it;
        }
       sql.append(");");
       if (!pdb->executeSQL(sql)) goto rollback;

       sql = "INSERT INTO TEMP_TABLE SELECT ";
       it = Q3ListViewItemIterator( fieldListView );
       while ( it.current() ) {
           item = it.current();
           sql.append(item->text(0));
           if (item->nextSibling() != 0)
           {
               sql.append(", ");
            }
            ++it;
        }
       sql.append(" FROM ");
       sql.append(curTable);
       sql.append(";");
       if (!pdb->executeSQL(sql)) goto rollback;

       sql = "DROP TABLE ";
       sql.append(curTable);
       sql.append(";");
       if (!pdb->executeSQL(sql)) goto rollback;

       sql = "CREATE TABLE ";
       sql.append(newName);
       sql.append(" (");
       it = Q3ListViewItemIterator( fieldListView );
       while ( it.current() ) {
           item = it.current();
           sql.append(item->text(0));
           sql.append(" ");
           sql.append(item->text(1));
           if (item->nextSibling() != 0)
           {
               sql.append(", ");
            }
            ++it;
        }
       sql.append(");");
       if (!pdb->executeSQL(sql)) goto rollback;

       sql = "INSERT INTO ";
       sql.append(newName);
       sql.append(" SELECT ");
       it = Q3ListViewItemIterator( fieldListView );
       while ( it.current() ) {
           item = it.current();
           sql.append(item->text(0));
           if (item->nextSibling() != 0)
           {
               sql.append(", ");
            }
            ++it;
        }
       sql.append(" FROM TEMP_TABLE;");
       if (!pdb->executeSQL(sql)) goto rollback;

       if (!pdb->executeSQL(QString("DROP TABLE TEMP_TABLE;"))) goto rollback;
       //if (!pdb->executeSQL(QString("COMMIT;"))) goto rollback;

       setActiveTable(pdb, newName);
   }

   //everything ok, just return
   QApplication::restoreOverrideCursor();  // restore original cursor
   return;

   rollback:
       QApplication::restoreOverrideCursor();  // restore original cursor
       QString error = "Error renaming table. Message from database engine:  ";
       error.append(pdb->lastErrorMessage);
       QMessageBox::warning( this, applicationName, error );
       pdb->executeSQL(QString("DROP TABLE TEMP_TABLE;"));
       //pdb->executeSQL(QString("ROLLBACK;"));
       setActiveTable(pdb, curTable);
}


void editTableForm::editField()
{
    Q3ListViewItem * item = fieldListView->selectedItem();
    if (item==0) {
        //should never happen, the button would not be active, but...
        return;
    } else {
        editFieldForm * fieldForm = new editFieldForm( this, "editfield", TRUE );
        fieldForm->setInitialValues(item->text(0),item->text(1));
        if (fieldForm->exec())
        {
            modified = true;
            //do the sql rename here
            //qDebug(fieldForm->name + fieldForm->type);
            item->setText(0,fieldForm->name);
            item->setText(1,fieldForm->type);

            //not until nested transaction are supported
           //if (!pdb->executeSQL(QString("BEGIN TRANSACTION;"))) goto rollback;

             QString sql = "CREATE TEMPORARY TABLE TEMP_TABLE(";
             Q3ListViewItemIterator it( fieldListView );
             Q3ListViewItem * item;
             while ( it.current() ) {
                 item = it.current();
                 sql.append(item->text(0));
                 sql.append(" ");
                 sql.append(item->text(1));
                 if (item->nextSibling() != 0)
                 {
                     sql.append(", ");
                 }
                 ++it;
             }
             sql.append(");");
             if (!pdb->executeSQL(sql)) goto rollback;

             sql = "INSERT INTO TEMP_TABLE SELECT ";
             for ( QStringList::Iterator ct = fields.begin(); ct != fields.end(); ++ct )		    {
                 sql.append( *ct );
                 if (*ct != fields.last())
                 {
                     sql.append(", ");
                 }
             }

             sql.append(" FROM ");
             sql.append(curTable);
             sql.append(";");
             if (!pdb->executeSQL(sql)) goto rollback;

             sql = "DROP TABLE ";
             sql.append(curTable);
             sql.append(";");
             if (!pdb->executeSQL(sql)) goto rollback;

             sql = "CREATE TABLE ";
             sql.append(curTable);
             sql.append(" (");
             it = Q3ListViewItemIterator( fieldListView );
             while ( it.current() ) {
                 item = it.current();
                 sql.append(item->text(0));
                 sql.append(" ");
                 sql.append(item->text(1));
                 if (item->nextSibling() != 0)
                 {
                     sql.append(", ");
                 }
                 ++it;
             }
             sql.append(");");
             if (!pdb->executeSQL(sql)) goto rollback;

             sql = "INSERT INTO ";
             sql.append(curTable);
             sql.append(" SELECT ");
             it = Q3ListViewItemIterator( fieldListView );
             while ( it.current() ) {
                 item = it.current();
                 sql.append(item->text(0));
                 if (item->nextSibling() != 0)
                 {
                     sql.append(", ");
                 }
                 ++it;
             }
             sql.append(" FROM TEMP_TABLE;");
             if (!pdb->executeSQL(sql)) goto rollback;

             if (!pdb->executeSQL(QString("DROP TABLE TEMP_TABLE;"))) goto rollback;
             //not until nested transaction are supported
             //if (!pdb->executeSQL(QString("COMMIT;"))) goto rollback;

             setActiveTable(pdb, curTable);
        }
        //everything ok, just return
        QApplication::restoreOverrideCursor();  // restore original cursor
        return;

        rollback:
            QApplication::restoreOverrideCursor();  // restore original cursor
            QString error = "Error editing field. Message from database engine:  ";
            error.append(pdb->lastErrorMessage);
            QMessageBox::warning( this, applicationName, error );
            //not until nested transaction are supported
            //pdb->executeSQL(QString("ROLLBACK;"));
            setActiveTable(pdb, curTable);
    }
}


void editTableForm::addField()
{
    addFieldForm * addForm = new addFieldForm( this, "addfield", TRUE );
   addForm->setInitialValues(QString(""),QString(""));
    if (addForm->exec())
   {
        modified = true;

        Q3ListViewItem * tbitem = new Q3ListViewItem( fieldListView);
        tbitem->setText( 0, addForm->fname);
        tbitem->setText( 1, addForm->ftype);
       //do the sql creation here
        modified = true;
            //do the sql rename here
            //qDebug(fieldForm->name + fieldForm->type);
        QString sql = "CREATE TEMPORARY TABLE TEMP_TABLE(";
        Q3ListViewItemIterator it( fieldListView );
        Q3ListViewItem * item;

        //not until nested transaction are supported
             //if (!pdb->executeSQL(QString("BEGIN TRANSACTION;"))) goto rollback;

        {//nest for MSVC support
             for ( QStringList::Iterator ct = fields.begin(); ct != fields.end(); ++ct )		    {
                 sql.append( *ct );
                 if (*ct != fields.last())
                 {
                     sql.append(", ");
                 }
             }
             sql.append(");");
             if (!pdb->executeSQL(sql)) goto rollback;

             sql = "INSERT INTO TEMP_TABLE SELECT ";
             for ( QStringList::Iterator ct1 = fields.begin(); ct1 != fields.end(); ++ct1 )		    {
                 sql.append( *ct1 );
                 if (*ct1 != fields.last())
                 {
                     sql.append(", ");
                 }
             }
        }

             sql.append(" FROM ");
             sql.append(curTable);
             sql.append(";");
             if (!pdb->executeSQL(sql)) goto rollback;

             sql = "DROP TABLE ";
             sql.append(curTable);
             sql.append(";");
             if (!pdb->executeSQL(sql)) goto rollback;

             sql = "CREATE TABLE ";
             sql.append(curTable);
             sql.append(" (");
             it = Q3ListViewItemIterator( fieldListView );
             while ( it.current() ) {
                 item = it.current();
                 sql.append(item->text(0));
                 sql.append(" ");
                 sql.append(item->text(1));
                 if (item->nextSibling() != 0)
                 {
                     sql.append(", ");
                 }
                 ++it;
             }
             sql.append(");");
             if (!pdb->executeSQL(sql)) goto rollback;
        {//nest for MSVC support

             sql = "INSERT INTO ";
             sql.append(curTable);
             sql.append("(");
             for ( QStringList::Iterator ct2 = fields.begin(); ct2 != fields.end(); ++ct2 )		    {
                 sql.append( *ct2 );
                 if (*ct2 != fields.last())
                 {
                     sql.append(", ");
                 }
             }
        }
        {//nest for MSVC support

             sql.append(") SELECT ");
             for ( QStringList::Iterator ct3 = fields.begin(); ct3 != fields.end(); ++ct3 )		    {
                 sql.append( *ct3 );
                 if (*ct3 != fields.last())
                 {
                     sql.append(", ");
                 }
             }
        }

             sql.append(" FROM TEMP_TABLE;");
             if (!pdb->executeSQL(sql)) goto rollback;

             if (!pdb->executeSQL(QString("DROP TABLE TEMP_TABLE;"))) goto rollback;
             //not until nested transaction are supported
             //if (!pdb->executeSQL(QString("COMMIT;"))) goto rollback;

             setActiveTable(pdb, curTable);

             QApplication::restoreOverrideCursor();  // restore original cursor
        return;

        rollback:
            QApplication::restoreOverrideCursor();  // restore original cursor
            QString error = "Error adding field. Message from database engine:  ";
            error.append(pdb->lastErrorMessage);
            QMessageBox::warning( this, applicationName, error );

            //not until nested transaction are supported
            //pdb->executeSQL(QString("ROLLBACK;"));
            setActiveTable(pdb, curTable);
   }
}


void editTableForm::removeField()
{
    Q3ListViewItem * remitem = fieldListView->selectedItem();
    if (remitem==0) {
        //should never happen, the button would not be active, but...
        return;
    } else {
        QString msg = "Are you sure you want to delete field ";
        msg.append(remitem->text(0));
        msg.append("? \n All data currently stored in this field will be lost");

        if (QMessageBox::warning( this, applicationName,
                                  msg,
                                  QMessageBox::Yes | QMessageBox::Default,
                                  QMessageBox::No | QMessageBox::Escape )
            == QMessageBox::Yes ){
            //delete field here
            /*fields= pdb->getTableFields(curTable);
    types= pdb->getTableTypes(curTable);*/
            modified = true;
            delete remitem;
        QString sql = "CREATE TEMPORARY TABLE TEMP_TABLE(";
        Q3ListViewItemIterator it( fieldListView );
        Q3ListViewItem * item;

        //not until nested transaction are supported
        //     if (!pdb->executeSQL(QString("BEGIN TRANSACTION;"))) goto rollback;

             it = Q3ListViewItemIterator( fieldListView );
             while ( it.current() ) {
                 item = it.current();
                 sql.append(item->text(0));
                 sql.append(" ");
                 sql.append(item->text(1));
                 if (item->nextSibling() != 0)
                 {
                     sql.append(", ");
                 }
                 ++it;
             }
             sql.append(");");
             if (!pdb->executeSQL(sql)) goto rollback;

             sql = "INSERT INTO TEMP_TABLE SELECT ";
            it = Q3ListViewItemIterator( fieldListView );
             while ( it.current() ) {
                 item = it.current();
                 sql.append(item->text(0));
                 if (item->nextSibling() != 0)
                 {
                     sql.append(", ");
                 }
                 ++it;
             }

             sql.append(" FROM ");
             sql.append(curTable);
             sql.append(";");
             if (!pdb->executeSQL(sql)) goto rollback;

             sql = "DROP TABLE ";
             sql.append(curTable);
             sql.append(";");
             if (!pdb->executeSQL(sql)) goto rollback;

             sql = "CREATE TABLE ";
             sql.append(curTable);
             sql.append(" (");
             it = Q3ListViewItemIterator( fieldListView );
             while ( it.current() ) {
                 item = it.current();
                 sql.append(item->text(0));
                 sql.append(" ");
                 sql.append(item->text(1));
                 if (item->nextSibling() != 0)
                 {
                     sql.append(", ");
                 }
                 ++it;
             }
             sql.append(");");
             if (!pdb->executeSQL(sql)) goto rollback;

             sql = "INSERT INTO ";
             sql.append(curTable);
             sql.append("(");
             it = Q3ListViewItemIterator( fieldListView );
             while ( it.current() ) {
                 item = it.current();
                 sql.append(item->text(0));
                 if (item->nextSibling() != 0)
                 {
                     sql.append(", ");
                 }
                 ++it;
             }
             sql.append(") SELECT ");
             it = Q3ListViewItemIterator( fieldListView );
             while ( it.current() ) {
                 item = it.current();
                 sql.append(item->text(0));
                 if (item->nextSibling() != 0)
                 {
                     sql.append(", ");
                 }
                 ++it;
             }
             sql.append(" FROM TEMP_TABLE;");
             if (!pdb->executeSQL(sql)) goto rollback;

             if (!pdb->executeSQL(QString("DROP TABLE TEMP_TABLE;"))) goto rollback;
             //not until nested transaction are supported
             //if (!pdb->executeSQL(QString("COMMIT;"))) goto rollback;

             setActiveTable(pdb, curTable);

             QApplication::restoreOverrideCursor();  // restore original cursor
        return;

        rollback:
            QApplication::restoreOverrideCursor();  // restore original cursor
            QString error = "Error removing field. Message from database engine:  ";
            error.append(pdb->lastErrorMessage);
            QMessageBox::warning( this, applicationName, error );

            //not until nested transaction are supported
            //pdb->executeSQL(QString("ROLLBACK;"));
            setActiveTable(pdb, curTable);
        }
    }
}

void editTableForm::fieldSelectionChanged()
{
    Q3ListViewItem * item = fieldListView->selectedItem();
    if (item==0) {
        renameFieldButton->setEnabled(false);
        removeFieldButton->setEnabled(false);
    } else {
        renameFieldButton->setEnabled(true);
        removeFieldButton->setEnabled(true);
    }
}


