#include "edittableform.h"
#include "ui_edittableform.h"
#include "renametableform.h"
#include "editfieldform.h"
#include <QMessageBox>

/*
 *  Constructs a editTableForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
editTableForm::editTableForm(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl),
      ui(new Ui::editTableForm)
{
    ui->setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
editTableForm::~editTableForm()
{
    delete ui;
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
    ui->editTableName->setText(curTable);
}

void editTableForm::populateFields()
{
    if (!pdb) return;

    //make sure we are not using cached information
    pdb->updateSchema();

    fields= pdb->getTableFields(curTable);
    types= pdb->getTableTypes(curTable);
    ui->treeWidget->model()->removeRows(0, ui->treeWidget->model()->rowCount());
    QStringList::Iterator tt = types.begin();
    for ( QStringList::Iterator ct = fields.begin(); ct != fields.end(); ++ct ) {
        QTreeWidgetItem *fldItem = new QTreeWidgetItem();
        fldItem->setText( 0, *ct  );
        fldItem->setText( 1, *tt );
        ui->treeWidget->addTopLevelItem(fldItem);
        ++tt;
    }
}

void editTableForm::accept()
{
    // Rename table
    QApplication::setOverrideCursor( Qt::WaitCursor ); // this might take time
    modified = true;
    QString newName = ui->editTableName->text();
    QString sql = QString("ALTER TABLE `%1` RENAME TO `%2`").arg(curTable, newName);
    if (!pdb->executeSQL(sql)){
        QApplication::restoreOverrideCursor();
        QString error("Error renaming table. Message from database engine:\n");
        error.append(pdb->lastErrorMessage).append("\n\n").append(sql);
        QMessageBox::warning( this, QApplication::applicationName(), error );
    } else {
        QApplication::restoreOverrideCursor();
        QDialog::accept();
    }
}

void editTableForm::checkInput()
{
    ui->editTableName->setText(ui->editTableName->text().trimmed());
    bool valid = true;
    if(ui->editTableName->text().isEmpty() || ui->editTableName->text().contains(" "))
        valid = false;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

void editTableForm::editField()
{
    if( !ui->treeWidget->currentItem()){
        return;
    }
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    editFieldForm * fieldForm = new editFieldForm( this );
    fieldForm->setModal(true);
    fieldForm->setInitialValues(false, curTable, item->text(0), item->text(1));
    if (fieldForm->exec())
    {
        modified = true;
        //do the sql rename here
        //qDebug(fieldForm->name + fieldForm->type);
        item->setText(0,fieldForm->field_name);
        item->setText(1,fieldForm->field_name);
    }
    //not until nested transaction are supported
    //if (!pdb->executeSQL(QString("BEGIN TRANSACTION;"))) goto rollback;

    //             QString sql = "CREATE TEMPORARY TABLE TEMP_TABLE(";
    //             Q3ListViewItemIterator it( fieldListView );
    //             Q3ListViewItem * item;
    //             while ( it.current() ) {
    //                 item = it.current();
    //                 sql.append(item->text(0));
    //                 sql.append(" ");
    //                 sql.append(item->text(1));
    //                 if (item->nextSibling() != 0)
    //                 {
    //                     sql.append(", ");
    //                 }
    //                 ++it;
    //             }
    //             sql.append(");");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             sql = "INSERT INTO TEMP_TABLE SELECT ";
    //             for ( QStringList::Iterator ct = fields.begin(); ct != fields.end(); ++ct )		    {
    //                 sql.append( *ct );
    //                 if (*ct != fields.last())
    //                 {
    //                     sql.append(", ");
    //                 }
    //             }
    //
    //             sql.append(" FROM ");
    //             sql.append(curTable);
    //             sql.append(";");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             sql = "DROP TABLE ";
    //             sql.append(curTable);
    //             sql.append(";");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             sql = "CREATE TABLE ";
    //             sql.append(curTable);
    //             sql.append(" (");
    //             it = Q3ListViewItemIterator( fieldListView );
    //             while ( it.current() ) {
    //                 item = it.current();
    //                 sql.append(item->text(0));
    //                 sql.append(" ");
    //                 sql.append(item->text(1));
    //                 if (item->nextSibling() != 0)
    //                 {
    //                     sql.append(", ");
    //                 }
    //                 ++it;
    //             }
    //             sql.append(");");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             sql = "INSERT INTO ";
    //             sql.append(curTable);
    //             sql.append(" SELECT ");
    //             it = Q3ListViewItemIterator( fieldListView );
    //             while ( it.current() ) {
    //                 item = it.current();
    //                 sql.append(item->text(0));
    //                 if (item->nextSibling() != 0)
    //                 {
    //                     sql.append(", ");
    //                 }
    //                 ++it;
    //             }
    //             sql.append(" FROM TEMP_TABLE;");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             if (!pdb->executeSQL(QString("DROP TABLE TEMP_TABLE;"))) goto rollback;
    //             //not until nested transaction are supported
    //             //if (!pdb->executeSQL(QString("COMMIT;"))) goto rollback;
    //
    //             setActiveTable(pdb, curTable);
    //        }
    //        //everything ok, just return
    //        QApplication::restoreOverrideCursor();  // restore original cursor
    //        return;
    //
    //        rollback:
    //            QApplication::restoreOverrideCursor();  // restore original cursor
    //            QString error = "Error editing field. Message from database engine:  ";
    //            error.append(pdb->lastErrorMessage);
    //            QMessageBox::warning( this, applicationName, error );
    //            //not until nested transaction are supported
    //            //pdb->executeSQL(QString("ROLLBACK;"));
    //            setActiveTable(pdb, curTable);
    //}
}


void editTableForm::addField()
{
    editFieldForm * addForm = new editFieldForm( this );
    addForm->setModal(true);
    addForm->setInitialValues(true, curTable, QString(""),QString(""));
    if (addForm->exec())
    {
        modified = true;

        QTreeWidgetItem *tbitem = new QTreeWidgetItem();
        tbitem->setText( 0, addForm->field_name);
        tbitem->setText( 1, addForm->field_name);
        //do the sql creation here
        modified = true;
        //do the sql rename here
        //qDebug(fieldForm->name + fieldForm->type);
        QString sql = "CREATE TEMPORARY TABLE TEMP_TABLE(";

    }
    // Q3ListViewItemIterator it( fieldListView );
    // Q3ListViewItem * item;

    //not until nested transaction are supported
    //if (!pdb->executeSQL(QString("BEGIN TRANSACTION;"))) goto rollback;

    // {//nest for MSVC support
    //             for ( QStringList::Iterator ct = fields.begin(); ct != fields.end(); ++ct )		    {
    //                 sql.append( *ct );
    //                 if (*ct != fields.last())
    //                 {
    //                     sql.append(", ");
    //                 }
    //             }
    //             sql.append(");");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             sql = "INSERT INTO TEMP_TABLE SELECT ";
    //             for ( QStringList::Iterator ct1 = fields.begin(); ct1 != fields.end(); ++ct1 )		    {
    //                 sql.append( *ct1 );
    //                 if (*ct1 != fields.last())
    //                 {
    //                     sql.append(", ");
    //                 }
    //             }
    //        }
    //
    //             sql.append(" FROM ");
    //             sql.append(curTable);
    //             sql.append(";");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             sql = "DROP TABLE ";
    //             sql.append(curTable);
    //             sql.append(";");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             sql = "CREATE TABLE ";
    //             sql.append(curTable);
    //             sql.append(" (");
    //             it = Q3ListViewItemIterator( fieldListView );
    //             while ( it.current() ) {
    //                 item = it.current();
    //                 sql.append(item->text(0));
    //                 sql.append(" ");
    //                 sql.append(item->text(1));
    //                 if (item->nextSibling() != 0)
    //                 {
    //                     sql.append(", ");
    //                 }
    //                 ++it;
    //             }
    //             sql.append(");");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //        {//nest for MSVC support
    //
    //             sql = "INSERT INTO ";
    //             sql.append(curT             for ( QStringList::Iterator ct = fields.begin(); ct != fields.end(); ++ct )		    {
    //sql.append( *ct );
    //if (*ct != fields.last())
    // {
    //    sql.append(", ");
    // }
    //}
    //sql.append(");");
    // if (!pdb->executeSQL(sql)) goto rollback;

    // sql = "INSERT INTO TEMP_TABLE SELECT ";
    // for ( QStringList::Iterator ct1 = fields.begin(); ct1 != fields.end(); ++ct1 )		    {
    //     sql.append( *ct1 );
    // if (*ct1 != fields.last())
    //  {
    //    sql.append(", ");
    // }
    // }
    //}

    // sql.append(" FROM ");
    // sql.append(";");
    //if (!pdb->executeSQL(sql)) goto rollback;

    //// sql = "DROP TABLE ";
    // sql.append(curTable);
    // sql.append(";");
    // if (!pdb->executeSQL(sql)) goto rollback;

    // sql = "CREATE TABLE ";
    // sql.append(curTable);
    //  sql.append(" (");
    //  it = Q3ListViewItemIterator( fieldListView );
    //  while ( it.current() ) {
    //   item = it.current();
    //   sql.append(item->text(0));
    ///    sql.append(" ");
    //   sql.append(item->text(1));
    //    if (item->nextSibling() != 0)
    //    {
    //   sql.append(", ");
    //    }
    //   ++it;
    // }
    //sql.append(");");
    //    if (!pdb->executeSQL(sql)) goto rollback;
    //{//nest for MSVC support
    //
    //    sql = "INSERT INTO ";
    //    sql.append(curTable);
    //    sql.append("(");
    //    for ( QStringList::Iterator ct2 = fields.begin(); ct2 != fields.end(); ++ct2 )		    {
    //        sql.append( *ct2 );
    //        if (*ct2 != fields.last())
    //        {
    //            sql.append(", ");
    //        }
    //    }
    //}
    //{//nest for MSVC support
    //
    //    sql.append(") SELECT ");
    //    for ( QStringList::Iterator ct3 = fields.begin(); ct3 != fields.end(); ++ct3 )		    {
    //        sql.append( *ct3 );
    //        if (*ct3 != fields.last())
    //        {
    //            sql.append(", ");
    //        }
    //    }
    //}
    //
    //    sql.append(" FROM TEMP_TABLE;");
    //    if (!pdb->executeSQL(sql)) goto rollback;
    //
    //    if (!pdb->executeSQL(QString("DROP TABLE TEMP_TABLE;"))) goto rollback;
    //    //not until nested transaction are supported
    //    //if (!pdb->executeSQL(QString("COMMIT;"))) goto rollback;
    //
    //    setActiveTable(pdb, curTable);
    //
    //    QApplication::restoreOverrideCursor();  // restore original cursor
    //return;
    //
    //rollback:
    //   QApplication::restoreOverrideCursor();  // restore original cursor
    //   QString error = "Error adding field. Message from database engine:  ";
    //   error.append(pdb->lastErrorMessage);
    //   QMessageBox::warning( this, applicationName, error );
    //
    //   //not until nested transaction are supported
    //   //pdb->executeSQL(QString("ROLLBACK;"));
    //   setActiveTable(pdb, curTable);able);
    //             sql.append("(");
    //             for ( QStringList::Iterator ct2 = fields.begin(); ct2 != fields.end(); ++ct2 )		    {
    //                 sql.append( *ct2 );
    //                 if (*ct2 != fields.last())
    //                 {
    //                     sql.append(", ");
    //                 }
    //             }
    //        }
    //        {//nest for MSVC support
    //
    //             sql.append(") SELECT ");
    //             for ( QStringList::Iterator ct3 = fields.begin(); ct3 != fields.end(); ++ct3 )		    {
    //                 sql.append( *ct3 );
    //                 if (*ct3 != fields.last())
    //                 {
    //                     sql.append(", ");
    //                 }
    //             }
    //        }
    //
    //             sql.append(" FROM TEMP_TABLE;");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             if (!pdb->executeSQL(QString("DROP TABLE TEMP_TABLE;"))) goto rollback;
    //             //not until nested transaction are supported
    //             //if (!pdb->executeSQL(QString("COMMIT;"))) goto rollback;
    //
    //             setActiveTable(pdb, curTable);
    //
    //             QApplication::restoreOverrideCursor();  // restore original cursor
    //        return;
    //
    //        rollback:
    //            QApplication::restoreOverrideCursor();  // restore original cursor
    //            QString error = "Error adding field. Message from database engine:  ";
    //            error.append(pdb->lastErrorMessage);
    //            QMessageBox::warning( this, applicationName, error );
    //
    //            //not until nested transaction are supported
    //            //pdb->executeSQL(QString("ROLLBACK;"));
    //            setActiveTable(pdb, curTable);
    //s}
}


void editTableForm::removeField()
{
    if(!ui->treeWidget->currentItem()){
        return;
    }

    QString msg = "Are you sure you want to delete field ";
    msg.append(ui->treeWidget->currentItem()->text(0));
    msg.append("? \n All data currently stored in this field will be lost");

    if (QMessageBox::warning( this, QApplication::applicationName(),
                              msg,
                              QMessageBox::Yes | QMessageBox::Default,
                              QMessageBox::No | QMessageBox::Escape )
            == QMessageBox::Yes ){
        //delete field here

    }
    /*fields= pdb->getTableFields(curTable);
    types= pdb->getTableTypes(curTable);*/
    //modified = true;
    // delete remitem;
    //        QString sql = "CREATE TEMPORARY TABLE TEMP_TABLE(";
    //        Q3ListViewItemIterator it( fieldListView );
    //        Q3ListViewItem * item;
    //
    //        //not until nested transaction are supported
    //        //     if (!pdb->executeSQL(QString("BEGIN TRANSACTION;"))) goto rollback;
    //
    //             it = Q3ListViewItemIterator( fieldListView );
    //             while ( it.current() ) {
    //                 item = it.current();
    //                 sql.append(item->text(0));
    //                 sql.append(" ");
    //                 sql.append(item->text(1));
    //                 if (item->nextSibling() != 0)
    //                 {
    //                     sql.append(", ");
    //                 }
    //                 ++it;
    //             }
    //             sql.append(");");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             sql = "INSERT INTO TEMP_TABLE SELECT ";
    //            it = Q3ListViewItemIterator( fieldListView );
    //             while ( it.current() ) {
    //                 item = it.current();
    //                 sql.append(item->text(0));
    //                 if (item->nextSibling() != 0)
    //                 {
    //                     sql.append(", ");
    //                 }
    //                 ++it;
    //             }
    //
    //             sql.append(" FROM ");
    //             sql.append(curTable);
    //             sql.append(";");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             sql = "DROP TABLE ";
    //             sql.append(curTable);
    //             sql.append(";");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             sql = "CREATE TABLE ";
    //             sql.append(curTable);
    //             sql.append(" (");
    //             it = Q3ListViewItemIterator( fieldListView );
    //             while ( it.current() ) {
    //                 item = it.current();
    //                 sql.append(item->text(0));
    //                 sql.append(" ");
    //                 sql.append(item->text(1));
    //                 if (item->nextSibling() != 0)
    //                 {
    //                     sql.append(", ");
    //                 }
    //                 ++it;
    //             }
    //             sql.append(");");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             sql = "INSERT INTO ";
    //             sql.append(curTable);
    //             sql.append("(");
    //             it = Q3ListViewItemIterator( fieldListView );
    //             while ( it.current() ) {
    //                 item = it.current();
    //                 sql.append(item->text(0));
    //                 if (item->nextSibling() != 0)
    //                 {
    //                     sql.append(", ");
    //                 }
    //                 ++it;
    //             }
    //             sql.append(") SELECT ");
    //             it = Q3ListViewItemIterator( fieldListView );
    //             while ( it.current() ) {
    //                 item = it.current();
    //                 sql.append(item->text(0));
    //                 if (item->nextSibling() != 0)
    //                 {
    //                     sql.append(", ");
    //                 }
    //                 ++it;
    //             }
    //             sql.append(" FROM TEMP_TABLE;");
    //             if (!pdb->executeSQL(sql)) goto rollback;
    //
    //             if (!pdb->executeSQL(QString("DROP TABLE TEMP_TABLE;"))) goto rollback;
    //             //not until nested transaction are supported
    //             //if (!pdb->executeSQL(QString("COMMIT;"))) goto rollback;
    //
    //             setActiveTable(pdb, curTable);
    //
    //             QApplication::restoreOverrideCursor();  // restore original cursor
    //        return;
    //
    //        rollback:
    //            QApplication::restoreOverrideCursor();  // restore original cursor
    //            QString error = "Error removing field. Message from database engine:  ";
    //            error.append(pdb->lastErrorMessage);
    //            QMessageBox::warning( this, applicationName, error );
    //
    //            //not until nested transaction are supported
    //            //pdb->executeSQL(QString("ROLLBACK;"));
    //            setActiveTable(pdb, curTable);
    //        }
    //    //}
}

void editTableForm::fieldSelectionChanged()
{
    ui->renameFieldButton->setEnabled(ui->treeWidget->selectionModel()->hasSelection());
    ui->removeFieldButton->setEnabled(ui->treeWidget->selectionModel()->hasSelection());
}
