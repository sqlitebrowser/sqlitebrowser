/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
void createTableForm::init()
{
   fieldListView->clear();
}

void createTableForm::confirmCreate()
{
    bool ok = true;
    QString tabname = tablenameLineEdit->text();
    if (tabname.isEmpty()) {
	ok = false;
	QMessageBox::information( this, applicationName, "Please select a name for the table" );
	return;
    }
    if (tabname.contains(" ")>0) {
	ok = false;
	QMessageBox::warning( this, applicationName, "Spaces are not allowed in the table name" );
	return;
    }
    
    if (fieldListView->firstChild()==0) {
	ok = false;
	QMessageBox::information( this, applicationName, "No fields defined" );
	return;
    }
    
    /*check field names for empty or illegal names
   TODO: move to add
      
    for (int r=0; r<fieldsTable->numRows();r++){
	QString rowname = fieldsTable->text(r, 0);
	if (rowname.isEmpty()) {
	    ok = false;
	    QMessageBox::warning( this, applicationName, "Empty field names are not allowed" );
	    break;
	}
	if (rowname.contains(" ")>0) {
	    ok = false;
	    QMessageBox::warning( this, applicationName, "Spaces are not allowed in the field names" );
	    break;
	}
    }*/
    
    if (!ok){
	return;
    }
	
    if (ok){
	createStatement = "CREATE TABLE ";
	createStatement.append(tabname);
	createStatement.append(" (");
	Q3ListViewItemIterator it( fieldListView );
	Q3ListViewItem * item;
	while ( it.current() ) {
	    item = it.current();
	  createStatement.append(item->text(0));
	  createStatement.append(" ");
	  createStatement.append(item->text(1));
	    if (item->nextSibling() != 0)
	   {
		createStatement.append(", ");
	    }
	    ++it;
	}
	/*for (int r=0; r<fieldsTable->numRows();r++){
	    createStatement.append(fieldsTable->text(r, 0));
	    createStatement.append(" ");
	    createStatement.append(fieldsTable->text(r, 1));
	    if (r<(fieldsTable->numRows() - 1))
		createStatement.append(", ");
	}*/
	createStatement.append(");");
	accept();
    }
}


void createTableForm::addField()
{
   addFieldForm * addForm = new addFieldForm( this, "addfield", TRUE );
   addForm->setInitialValues(QString(""),QString(""));
    if (addForm->exec())
   {
       //qDebug(addForm->fname + addForm->ftype);
       Q3ListViewItem * tbitem = new Q3ListViewItem( fieldListView);
       tbitem->setText( 0, addForm->fname  );
       tbitem->setText( 1, addForm->ftype );
   }
}


void createTableForm::deleteField()
{
    Q3ListViewItem * item = fieldListView->selectedItem();
    if (item==0) {
	//should never happen, the button would not be active, but...
	return;
    } else {
	delete item;
	}
}



void createTableForm::fieldSelectionChanged()
{
     Q3ListViewItem * item = fieldListView->selectedItem();
    if (item==0) {
	buttonDeleteField->setEnabled(false);
    } else {
	buttonDeleteField->setEnabled(true);
    }

}
