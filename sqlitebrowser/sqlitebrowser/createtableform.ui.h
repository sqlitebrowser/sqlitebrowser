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
    fieldsTable->setNumRows(0);
   // fieldsTable->setNumCols(0);
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
    
    if (fieldsTable->numRows()==0) {
	ok = false;
	QMessageBox::information( this, applicationName, "No fields defined" );
	return;
    }
    
    /*check field names for empty or illegal names*/
    
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
    }
    
    if (!ok){
	return;
    }
	
    if (ok){
	createStatement = "CREATE TABLE ";
	createStatement.append(tabname);
	createStatement.append(" (");
	for (int r=0; r<fieldsTable->numRows();r++){
	    createStatement.append(fieldsTable->text(r, 0));
	    createStatement.append(" ");
	    createStatement.append(fieldsTable->text(r, 1));
	    if (r<(fieldsTable->numRows() - 1))
		createStatement.append(", ");
	}
	createStatement.append(");");
	accept();
    }
}


void createTableForm::addField()
{
    fieldsTable->insertRows(fieldsTable->numRows());
}


void createTableForm::deleteField()
{
    if (fieldsTable->currentRow()!=-1){
	fieldsTable->removeRow(fieldsTable->currentRow());
    }
}
