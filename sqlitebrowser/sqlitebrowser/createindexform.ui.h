/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/


void createIndexForm::tableSelected( const QString & entry )
{
        tableMap::Iterator it;
         for ( it = mtablemap.begin(); it != mtablemap.end(); ++it ) {
	     QString tname = it.data().getname() ;
	     
	     //populate the fields with first table name
	     if ((tname.compare(entry)==0)){
		 comboFields->clear();
		 fieldMap::Iterator fit;
		 fieldMap fmap = it.data().fldmap;
		 for ( fit = fmap.begin(); fit != fmap.end(); ++fit ) {
		     comboFields->insertItem( fit.data().getname(), -1 );
		 }
	     }
        }
}

void createIndexForm::confirmCreate()
{
    bool ok = true;
    QString idxname = indexLineEdit->text();
    if (idxname.isEmpty()) {
	ok = false;
	QMessageBox::information( this, applicationName, "Please select a name for the index" );
	return;
    }
    if (idxname.contains(" ")>0) {
	ok = false;
	QMessageBox::warning( this, applicationName, "Spaces are not allowed in the index name" );
	return;
    }
    if (ok){
	createStatement = "CREATE ";
	if (comboUnique->currentItem()==1){
	    createStatement.append("UNIQUE ");
	}
	createStatement.append("INDEX ");
	createStatement.append(indexLineEdit->text());
	createStatement.append(" ON ");
	createStatement.append(comboTables->currentText());
	createStatement.append("(");
	createStatement.append(comboFields->currentText());
	createStatement.append(" ");
	if (comboOrder->currentItem()==0){
	    createStatement.append("ASC");
	} else {
	    createStatement.append("DESC");
	}
	createStatement.append(");");
	accept();
    }
}

void createIndexForm::populateTable(tableMap rmap)
{
    mtablemap = rmap;
    tableMap::Iterator it;
         for ( it = mtablemap.begin(); it != mtablemap.end(); ++it ) {
	     comboTables->insertItem( it.data().getname() , -1);
	     
	     //populate the fields with first table name
	     if (it==mtablemap.begin()){
		 fieldMap::Iterator fit;
		 fieldMap fmap = it.data().fldmap;
		 for ( fit = fmap.begin(); fit != fmap.end(); ++fit ) {
		     comboFields->insertItem( fit.data().getname(), -1 );
		 }
	     }
        }
}
