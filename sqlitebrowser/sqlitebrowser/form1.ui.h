/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

void mainForm::init()
{
    clipboard = QApplication::clipboard();
    if ( clipboard->supportsSelection() )
 clipboard->setSelectionMode( TRUE );
    
    findWin = 0;
     editWin = 0;
     logWin = 0;
    recsPerView = 1000;
    recAtTop = 0;
    gotoValidator = new QIntValidator(0,0,this);
    editGoto->setValidator(gotoValidator);
    gotoValidator->setRange ( 0, 0);
    resetBrowser();
    this->setCaption(applicationName);
    this->setIcon( QPixmap::fromMimeSource( applicationIconName ) );
    buttonNext->setEnabled(FALSE);
    buttonPrevious->setEnabled(FALSE);
    
    
       if (!editWin)
    {
       editWin = new editForm(this);
       connect( editWin, SIGNAL( goingAway() ),this, SLOT( editWinAway() ) );
       connect( editWin, SIGNAL( updateRecordText(int, int , QString) ),
                     this, SLOT( updateRecordText(int, int , QString) ) );
   }
       
        if (!logWin)
    {
       logWin = new sqlLogForm(this);
       connect( logWin, SIGNAL( goingAway() ),this, SLOT( logWinAway() ) );
       connect( logWin, SIGNAL( dbState(bool) ),this, SLOT( dbState(bool)  ) );
   }
 
 //connect db and log
    db.logWin = logWin;
 
}

void mainForm::destroy()
{
    if (gotoValidator)
 delete gotoValidator;

}

void mainForm::fileOpen(const QString & fileName)
{
    QString wFile = fileName;
    if (!QFile::exists(wFile))
    {
     wFile = QFileDialog::getOpenFileName(
                    "",
                    "",
                    this,
                    "open file dialog"
                    "Choose a database file" );
    }
    if (QFile::exists(wFile) )
    {
 if (db.open(wFile))
 {
 this->setCaption(applicationName+" - "+wFile);
 fileCloseAction->setEnabled(true);
 fileCompactAction->setEnabled(true);
 editCreateTableAction->setEnabled(true);
 editDeleteTableAction->setEnabled(true);
 editModifyTableAction->setEnabled(true);
 editCreateIndexAction->setEnabled(true);
 editDeleteIndexAction->setEnabled(true);
                } else {
 QString err = "An error occurred:  ";
                err.append(db.lastErrorMessage);
                 QMessageBox::information( this, applicationName ,err);
                }
 populateStructure();
 resetBrowser();
    }
}


void mainForm::fileOpen()
{
 fileOpen(QString());
}



void mainForm::fileNew()
{
    QString fileName = QFileDialog::getSaveFileName(
                    "",
                    "",
                    this,
                    "create file dialog"
                    "Choose a filename to save under" );
     if (QFile::exists(fileName) )
    {
  QString err = "File ";
  err.append(fileName);
  err.append(" already exists. Please choose a different name");
 QMessageBox::information( this, applicationName ,err);
 return;
    }
    if (!fileName.isNull())
    {
 db.create(fileName);
 this->setCaption(applicationName+" - "+fileName);
 populateStructure();
 resetBrowser();
 createTable();
 fileCloseAction->setEnabled(true);
 fileCompactAction->setEnabled(true);
 editCreateTableAction->setEnabled(true);
 editDeleteTableAction->setEnabled(true);
 editModifyTableAction->setEnabled(true);
 editCreateIndexAction->setEnabled(true);
 editDeleteIndexAction->setEnabled(true);
    }
}

void mainForm::populateStructure()
{
    dblistView->clear();
    dblistView->setSorting(-1);
    if (!db.isOpen()){
 return;
    }
    db.updateSchema();
    tableMap::Iterator it;
    tableMap tmap = db.tbmap;
    QListViewItem * lasttbitem = 0;
        for ( it = tmap.begin(); it != tmap.end(); ++it ) {
     QListViewItem * tbitem = new QListViewItem( dblistView, lasttbitem );
     //tbitem->setOpen( TRUE );
      tbitem->setText( 0, it.data().getname() );
      tbitem->setText( 1,  "table" );
      tbitem->setText( 3, it.data().getsql() );
     fieldMap::Iterator fit;
     fieldMap fmap = it.data().fldmap;
     QListViewItem * lastflditem = 0;
     for ( fit = fmap.begin(); fit != fmap.end(); ++fit ) {
   QListViewItem * fielditem = new QListViewItem(tbitem, lastflditem);
   fielditem->setText( 0, fit.data().getname() );
   fielditem->setText( 1, "field"  );
   fielditem->setText( 2, fit.data().gettype() );
   lastflditem = fielditem;
     }
     lasttbitem = tbitem;
        }
  indexMap::Iterator it2;
   indexMap imap = db.idxmap;
        for ( it2 = imap.begin(); it2 != imap.end(); ++it2 ) {
     QListViewItem * item = new QListViewItem( dblistView, lasttbitem );
     item->setText( 0, it2.data().getname());
     item->setText( 1,  "index"  );
     item->setText( 3, it2.data().getsql() );
     lasttbitem = item ;
        }
} 

void mainForm::populateTable( const QString & tablename)
{
    bool mustreset = false;
    QApplication::setOverrideCursor( waitCursor, TRUE );
    if (tablename.compare(db.curBrowseTableName)!=0)
 mustreset = true;
    
    if (!db.browseTable(tablename)){
 dataTable->setNumRows( 0 );
 dataTable->setNumCols( 0 );
 QApplication::restoreOverrideCursor();
 if (findWin){
     findWin->resetFields(db.getTableFields(""));
     findWin->resetResults();
 }
 return;
    }

    if (mustreset){
 recAtTop = 0;
 updateTableView(0);
 if (findWin) findWin->resetFields(db.getTableFields(db.curBrowseTableName));
    } else {
 updateTableView(-1);
    }
  //got to keep findWin in synch  
    if (findWin){
 findWin->resetResults();
    }
    if (editWin)
    {
 editWin->reset();
    }
}

void mainForm::resetBrowser()
{
    recAtTop = 0;
    comboBrowseTable->clear();
    QStringList tab = db.getTableNames();
    if  (tab.isEmpty()){
 comboBrowseTable->insertItem("",-1);
    } else {
 for ( QStringList::Iterator ct = tab.begin(); ct != tab.end(); ++ct ) {
     comboBrowseTable->insertItem(*ct,-1);
 }
    }
    setRecordsetLabel();
    comboBrowseTable->setCurrentItem(0);
    populateTable(comboBrowseTable->currentText());
}

void mainForm::fileClose()
{
    db.close();
    this->setCaption(applicationName);
    resetBrowser();
    populateStructure();
    fileCloseAction->setEnabled(false);
    fileCompactAction->setEnabled(false);
    editCreateTableAction->setEnabled(false);
    editDeleteTableAction->setEnabled(false);
    editModifyTableAction->setEnabled(false);
    editCreateIndexAction->setEnabled(false);
    editDeleteIndexAction->setEnabled(false);
}


void mainForm::fileExit()
{
    if (db.isOpen())
    {
  if (db.getDirty())
  {
   QString msg = "Do you want to save the changes made to the database file ";
   msg.append(db.curDBFilename);
   msg.append("?");
   if (QMessageBox::question( this, applicationName ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
   {
    bool done(false);
    do {
     if ( db.save() ) 
      done = true;
     else {
      QString error = "Error: could not save the database.\nMessage from database engine:  ";
      error.append(db.lastErrorMessage);
      switch ( QMessageBox::warning( this, applicationName, error, QMessageBox::Retry|QMessageBox::Default, QMessageBox::Cancel|QMessageBox::Escape, QMessageBox::Ignore) ) {
       case QMessageBox::Retry:
        break;
       case QMessageBox::Ignore:
        db.revert();
        done = true;
        break;
       case QMessageBox::Cancel:
        return;
      }
     }
    } while ( !done );
   } else {
    //not really necessary, I think... but will not hurt.
    db.revert();
   }
  }
  db.close();
    }
 QApplication::exit( 0 );
}

void mainForm::closeEvent( QCloseEvent * )
{
        fileExit();
}

void mainForm::addRecord()
{
    if (db.addRecord()){
 //added record will be the last one in view 
 recAtTop = ((db.getRecordCount()-1)/recsPerView)*recsPerView;
 updateTableView(db.getRecordCount()-recAtTop-1);
    }else{
 QMessageBox::information( this, applicationName,
    "Please select a table first" );
    }
}


void mainForm::deleteRecord()
{
    if (dataTable->currentRow()!=-1){
 int lastselected = dataTable->currentRow();
 db.deleteRecord(dataTable->currentRow()+recAtTop);
 populateTable(db.curBrowseTableName);
 int nextselected = lastselected ;
 if (nextselected > db.getRecordCount()){
     nextselected = db.getRecordCount();
 }
 if (nextselected>0){
     selectTableLine(nextselected);
 }
    } else {
 QMessageBox::information( this, applicationName, "Please select a record first" );    }
}

void mainForm::updateTableView(int lineToSelect)
{
  //  qDebug("line to select value is %d, rowAttop = %d",lineToSelect, recAtTop);
    QApplication::setOverrideCursor( waitCursor, TRUE );
    QStringList fields = db.browseFields;
    
     dataTable->setNumRows(0); 
    dataTable->setNumCols( fields.count() );
    int cheadnum = 0;
    for ( QStringList::Iterator ct = fields.begin(); ct != fields.end(); ++ct ) {
 dataTable->horizontalHeader()->setLabel( cheadnum, *ct  );
 cheadnum++;
    }
    
    rowList tab = db.browseRecs;
    int maxRecs = db.getRecordCount();
    int recsThisView = maxRecs - recAtTop;
    
    if (recsThisView>recsPerView)
 recsThisView = recsPerView;
    
   // qDebug("recsthisview= %d\n",recsThisView);
    
    dataTable->setNumRows( recsThisView);
    rowList::iterator rt;
    int rowNum = 0;
    int colNum = 0;
    //int dcols =0;
    QString rowLabel;
    for ( rt = tab.at(recAtTop); rt !=tab.end(); ++rt )
    {
 rowLabel.setNum(recAtTop+rowNum+1);
 dataTable->verticalHeader()->setLabel( rowNum, rowLabel  );
 colNum = 0;
 for ( QStringList::Iterator it = (*rt).begin(); it != (*rt).end(); ++it ) {
     //skip first one (the rowid)
     if (it!=(*rt).begin()){
  QString content = *it;
  QString firstline = content.section( '\n', 0,0 );
  if (content.length()>MAX_DISPLAY_LENGTH)
		{
      firstline.truncate(MAX_DISPLAY_LENGTH);
      firstline.append("...");
  }
  dataTable->setText( rowNum, colNum, firstline);
  colNum++;
     }
 }
 rowNum++;
 if (rowNum==recsThisView) break;
    }
    
    //dataTable->clearSelection(true);
    if (lineToSelect!=-1){
 //qDebug("inside selection");
 selectTableLine(lineToSelect);
    } 
    setRecordsetLabel();
    QApplication::restoreOverrideCursor();
}

void mainForm::selectTableLine(int lineToSelect)
{
    dataTable->clearSelection(true);
   dataTable->selectRow(lineToSelect);
   dataTable->setCurrentCell(lineToSelect, 0);
   dataTable->ensureCellVisible (lineToSelect, 0 ) ; 
}

void mainForm::navigatePrevious()
{
    int nextFirstRec = recAtTop - recsPerView;
    if (nextFirstRec  >= 0 ) {
 recAtTop = nextFirstRec;
 updateTableView(-1);
    }
}


void mainForm::navigateNext()
{
    int nextFirstRec = recAtTop + recsPerView; 
    //qDebug("called navigatenext, nextFirstRec=%d\n",nextFirstRec);
    if (nextFirstRec < db.getRecordCount() ) {
 recAtTop = nextFirstRec;
 updateTableView(-1);
    }
}


void mainForm::navigateGoto()
{
    QString typed = editGoto->text();
    bool ok;
    int dec = typed.toInt( &ok);
    if (dec==0) dec=1;
    if (dec>db.getRecordCount()) dec = db.getRecordCount();
    
    recAtTop = ((dec-1)/recsPerView)*recsPerView;
    updateTableView(dec-recAtTop-1);
    editGoto->setText(QString::number(dec,10));
}

void mainForm::setRecordsetLabel()
{
    if (db.getRecordCount()==0){
 labelRecordset->setText("0 - 0 of 0");
    } else { 
 QString label = QString::number(recAtTop+1,10);
 label.append(" - ");
 int lastrec = db.getRecordCount();
 int lastthisview = recAtTop+recsPerView;
 if (lastthisview > lastrec) lastthisview = lastrec;
 label.append(QString::number(lastthisview,10));
 label.append(" of ");
 label.append(QString::number(db.getRecordCount(),10));
 labelRecordset->setText(label);
    }
    gotoValidator->setRange ( 0, db.getRecordCount());
    
    if (db.getRecordCount()>1000){
 if (recAtTop>=1000) {
     buttonPrevious->setEnabled(TRUE);
 } else {
      buttonPrevious->setEnabled(FALSE);
 }
 if (db.getRecordCount()>=(recAtTop+1000)) {
     buttonNext->setEnabled(TRUE);
 } else {
     buttonNext->setEnabled(FALSE);
 }
    } else {
 buttonNext->setEnabled(FALSE);
 buttonPrevious->setEnabled(FALSE);
    }
}

void mainForm::browseFind(bool open)
{
    if (open){
 if ( ! findWin ) {
     findWin= new findForm( this );
     connect( findWin, SIGNAL( lookfor(const QString&, const QString&, const QString&) ),
                     this, SLOT( lookfor(const QString&, const QString&, const QString&) ) );
     connect( findWin, SIGNAL( showrecord(int) ),this, SLOT( showrecord(int) ) );
     connect( findWin, SIGNAL( goingAway() ),this, SLOT( browseFindAway() ) );
	}
	findWin->resetFields(db.getTableFields(db.curBrowseTableName));
	findWin->show();
    } else {
	if (findWin){
	    findWin->hide();
	}
    }
}

void mainForm::browseFindAway()
{
	buttonFind->toggle();
}

void mainForm::lookfor( const QString & wfield, const QString & woperator, const QString & wsearchterm )
{
    if (!db.isOpen()){
	QMessageBox::information( this, applicationName, "There is no database opened. Please open or create a new database file." );
	return;
    }
    
    //we may need to modify woperator and wsearchterm, so use copies
    QString * finaloperator = new QString(woperator);
    QString * finalsearchterm = new QString(wsearchterm);
    
    //special case for CONTAINS operator: use LIKE and surround the search word with % characters
    if (woperator.compare("contains")==0){
	finaloperator = new QString("LIKE");
	QString newsearchterm = "%";
	newsearchterm.append(wsearchterm);
	newsearchterm.append("%");
	finalsearchterm = new QString( newsearchterm);
    }
    QApplication::setOverrideCursor( waitCursor, TRUE );
    QString statement = "SELECT rowid, ";
    statement.append(wfield);
    statement.append("  FROM ");
    statement.append( db.curBrowseTableName);
    statement.append(" WHERE ");
    statement.append(wfield);
    statement.append(" ");
    statement.append(*finaloperator);
    statement.append(" ");
    //searchterm needs to be quoted if it is not a number
    bool ok = false;
    wsearchterm.toDouble(&ok);
    if (!ok) wsearchterm.toInt(&ok, 10);
    if (!ok) {//not a number, quote it
	char * formSQL = sqlite3_mprintf("%Q",static_cast<const char*>((*finalsearchterm).utf8()));
	statement.append(formSQL);
	 if (formSQL) sqlite3_free(formSQL);
     } else {//append the number, unquoted
	 statement.append(*finalsearchterm);
     }
    statement.append(" ORDER BY rowid; ");
    resultMap res = db.getFindResults(statement);
    findWin->showResults(res);
    QApplication::restoreOverrideCursor();
}


void mainForm::showrecord( int dec )
{
    recAtTop = ((dec-1)/recsPerView)*recsPerView;
    updateTableView(dec-recAtTop-1);
}

void mainForm::createTable()
{
    if (!db.isOpen()){
 QMessageBox::information( this, applicationName, "There is no database opened. Please open or create a new database file." );
 return;
    }
    createTableForm * tableForm = new createTableForm( this, "createtable", TRUE );
    if ( tableForm->exec() ) {
 if (!db.executeSQL(tableForm->createStatement)){
     QString error = "Error: could not create the table. Message from database engine:  ";
	    error.append(db.lastErrorMessage);
	    QMessageBox::warning( this, applicationName, error );
 } else {
     populateStructure();
     resetBrowser();
 }
    }
}


void mainForm::createIndex()
{
    if (!db.isOpen()){
 QMessageBox::information( this, applicationName, "There is no database opened. Please open or create a new database file." );
 return;
    }
    createIndexForm * indexForm = new createIndexForm( this, "createindex", TRUE );
    tableMap tmap = db.tbmap;
    indexForm->populateTable( tmap );
    if ( indexForm->exec() ) {
 if (!db.executeSQL(indexForm->createStatement)){
     QString error = "Error: could not create the index. Message from database engine:  ";
	    error.append(db.lastErrorMessage);
	    QMessageBox::warning( this, applicationName, error );
	} else {
	    populateStructure();
	    resetBrowser();
	}
    }
}


void mainForm::compact()
{
    QApplication::setOverrideCursor( waitCursor, TRUE );
    if (db.isOpen()){
	if (!db.compact()){
	    QString error = "Error: could not compact the database file. Message from database engine:  ";
	    error.append(db.lastErrorMessage);
	    QMessageBox::warning( this, applicationName, error );
	} else {
	    QMessageBox::warning( this, applicationName, "Database compacted" );
	}
    }
    db.open(db.curDBFilename);
    populateStructure();
    resetBrowser();
    fileCloseAction->setEnabled(true);
    fileCompactAction->setEnabled(true);
    QApplication::restoreOverrideCursor( );
}


void mainForm::deleteTable()
{
    if (!db.isOpen()){
 QMessageBox::information( this, applicationName, "There is no database opened." );
 return;
    }
    deleteTableForm * tableForm = new deleteTableForm( this, "deletetable", TRUE );
    tableForm->populateOptions( db.getTableNames());
    if ( tableForm->exec() ) {
 QString statement = "DROP TABLE ";
 statement.append(tableForm->option);
 statement.append(";");
 if (!db.executeSQL( statement)){
     QString error = "Error: could not delete the table. Message from database engine:  ";
     error.append(db.lastErrorMessage);
     QMessageBox::warning( this, applicationName, error );
 } else {
     populateStructure();
     resetBrowser();
 }
    }
}

void mainForm::editTable()
{
    if (!db.isOpen()){
 QMessageBox::information( this, applicationName, "There is no database opened." );
 return;
    }
    chooseTableForm * tableForm = new chooseTableForm( this, "choosetable", TRUE );
    QStringList tablelist = db.getTableNames();
    if (tablelist.empty()){
 QMessageBox::information( this, applicationName, "There are no tables to edit in this database." );
 return;
    }
    tableForm->populateOptions( tablelist );
    if ( tableForm->exec() ) {
 //statement.append(tableForm->option);
 editTableForm * edTableForm = new editTableForm( this, "edittable", TRUE );
 //send table name ? or handle it all from here?
 edTableForm->setActiveTable(&db, tableForm->option);
 edTableForm->exec();
 //check modified status
 if (edTableForm->modified)
 {
     populateStructure();
     resetBrowser();
 }
    }
}


void mainForm::deleteIndex()
{
        if (!db.isOpen()){
 QMessageBox::information( this, applicationName, "There is no database opened." );
 return;
    }
    deleteIndexForm * indexForm = new deleteIndexForm( this, "deleteindex", TRUE );
    indexForm->populateOptions( db.getIndexNames());
    if ( indexForm->exec() ) {
 QString statement = "DROP INDEX ";
 statement.append(indexForm->option);
 statement.append(";");
 if (!db.executeSQL( statement)){
     QString error = "Error: could not delete the index. Message from database engine:  ";
     error.append(db.lastErrorMessage);
     QMessageBox::warning( this, applicationName, error );
 } else {
     populateStructure();
     resetBrowser();
 }
    }

}


void mainForm::copy()
{
    QWidget * t =dataTable->cellWidget(dataTable->currentRow(), dataTable->currentColumn());
 if (t!=0){
     if (t->isA( "QLineEdit" )){
  /*we are in edit mode*/
  if (t->hasFocus()){
  QLineEdit * le = (QLineEdit *) t;
  le->copy();
     }
     }
 }
    if (editGoto->hasFocus())
 editGoto->copy();
}


void mainForm::paste()
{
    QWidget * t =dataTable->cellWidget(dataTable->currentRow(), dataTable->currentColumn());
 if (t!=0){
     if (t->isA( "QLineEdit" )){
  /*we are in edit mode*/
  if (t->hasFocus()){
  QLineEdit * le = (QLineEdit *) t;
  le->paste();}
     }
 }
    if (editGoto->hasFocus())
 editGoto->paste();
}


void mainForm::helpWhatsThis()
{
    QWhatsThis::enterWhatsThisMode ();
}


void mainForm::helpAbout()
{
    /*QString wcaption = "About ";
    wcaption.append(applicationName);
    QString wtext = "This text, etc...";
    QMessageBox::about ( this, wcaption, wtext);*/
    aboutForm * aForm = new aboutForm( this, "about", TRUE );
    aForm ->exec() ;
}


void mainForm::updateRecordText(int row, int col, QString newtext)
{
    if (!db.updateRecord(row, col, newtext)){
 QMessageBox::information( this, applicationName, "Data could not be updated" );
    }
    /*dataTable->setVScrollBarMode(QScrollView::AlwaysOff);
    dataTable->setVScrollBarMode(QScrollView::Auto);
    dataTable->setReadOnly(true);*/
     rowList tab = db.browseRecs;
 rowList::iterator rt = tab.at(row);
 QString rowid = (*rt).first();
 QStringList::Iterator cv = (*rt).at(col+1);//must account for rowid
 
 QString content = *cv ;
 QString firstline = content.section( '\n', 0,0 );
 if (content.length()>14)
 {
    firstline.truncate(14);
    firstline.append("...");
 }
 dataTable->setText( row - recAtTop, col, firstline);
}

void mainForm::logWinAway()
{
    sqlLogAction->toggle();
}

void mainForm::editWinAway()
{
    editWin->hide();
    setActiveWindow();
    dataTable->selectCells ( editWin->curRow - recAtTop, editWin->curCol, editWin->curRow- recAtTop, editWin->curCol);
}



void mainForm::editText(int row, int col)
{
    rowList tab = db.browseRecs;
    rowList::iterator rt = tab.at(row);
    QString rowid = (*rt).first();
    QStringList::Iterator cv = (*rt).at(col+1);//must account for rowid
     //dataTable->setText( row - recAtTop, col, *cv  );
 
    editWin->loadText(*cv , row, col);
    editWin ->show();
}



void mainForm::doubleClickTable( int row, int col, int button, const QPoint & mousepoint )
{
    if ((row==-1) || (col==-1)){
 qDebug("no cell selected");
 return;
    }
    
    int realRow = row + recAtTop;
   
    editText(realRow , col);
}


void mainForm::executeQuery()
{
    if (!db.isOpen()){
 QMessageBox::information( this, applicationName, "There is no database opened." );
 return;
    }
    QString query = sqlTextEdit->text();
    if (query.isEmpty())
    {
 QMessageBox::information( this, applicationName, "Query string is empty" );
 return;
    }
    //log the query
    db.logSQL(query, kLogMsg_User);
    sqlite3_stmt *vm;
    const void *tail;
 int ncol;

   int err=0;
   QString lastErrorMessage = QString("No error");
   queryResultListView->clear();
   queryResultListView->setSorting (-1, FALSE);
   while (queryResultListView->columns()>0)
   {
       queryResultListView->removeColumn(0);
   }
   
	err=sqlite3_prepare16(db._db,query.ucs2(),query.length(),&vm,&tail);
	if (err == SQLITE_OK){
	    db.setDirty(true);
	    int rownum = 0;
	  QListViewItem * lasttbitem = 0;
	  bool mustCreateColumns = true;
	  while ( sqlite3_step(vm) == SQLITE_ROW ){
	      //r.clear()
	          QListViewItem * tbitem = new QListViewItem( queryResultListView, lasttbitem);
	      //setup num of cols here for display grid
	      if (mustCreateColumns)
		  {
			ncol = sqlite3_data_count(vm);
			for (int e=0; e<ncol; e++)
				queryResultListView->addColumn("");
		
			mustCreateColumns = false;
	      }
	      for (int e=0; e<ncol; e++){
      QString rv(StringFromUTF16(sqlite3_column_text16(vm, e)));
    		//show it here
		  QString firstline = rv.section( '\n', 0,0 );
		  if (firstline.length()>MAX_DISPLAY_LENGTH)
		{
		    firstline.truncate(MAX_DISPLAY_LENGTH);
		   firstline.append("...");
		}
		  tbitem->setText( e, firstline);
		  lasttbitem = tbitem;
		rownum++;
	      }
	  }

          sqlite3_finalize(vm);
        }else{
          lastErrorMessage = QString (sqlite3_errmsg(db._db));
        }
       queryErrorLineEdit->setText(lastErrorMessage);
}


void mainForm::mainTabSelected(const QString & tabname)
{
    if ((mainTab->currentPageIndex ()==0)||(mainTab->currentPageIndex ()==1))
    {
	populateStructure();
	resetBrowser();
    }
}


void mainForm::toggleLogWindow( bool enable )
{
    if (enable){
	logWin->show();
    } else {
	logWin->hide();
    }
}


void mainForm::importTableFromCSV()
{
    if (!db.isOpen()){
		QMessageBox::information( this, applicationName, "There is no database opened. Please open or create a new database file first." );
		return;
    }
     
     if (db.getDirty())
     {
		QString msg = "Database needs to be saved before the import operation.\nSave current changes and continue?";
		if (QMessageBox::question( this, applicationName ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
		{
			bool	done(false);
			do {
				if ( db.save() ) 
					done = true;
				else {
					QString error = "Error: could not save the database.\nMessage from database engine:  ";
					error.append(db.lastErrorMessage);
					switch ( QMessageBox::warning( this, applicationName, error, QMessageBox::Retry|QMessageBox::Default, QMessageBox::Cancel|QMessageBox::Escape, QMessageBox::Ignore) ) {
						case QMessageBox::Retry:
							break;
						case QMessageBox::Ignore:
							db.revert();
							done = true;
							break;
						case QMessageBox::Cancel:
							return;
					}
				}
			} while ( !done );
		} else {
			return;
		}
    }
    
    QString wFile = QFileDialog::getOpenFileName(
                    "",
                    "Text files (*.csv *.txt)",
                    this,
                    "import csv data"
                    "Choose a text file" );
    
    if (QFile::exists(wFile) )
    {
		importCSVForm * csvForm = new importCSVForm( this, "importcsv", TRUE );
		csvForm->initialize(wFile, &db);
		if ( csvForm->exec() ) {
			populateStructure();
			resetBrowser();
			QMessageBox::information( this, applicationName, "Import completed" );
		}
    }
}

void mainForm::exportTableToCSV()
{
     if (!db.isOpen()){
	QMessageBox::information( this, applicationName, "There is no database opened to export" );
	return;
    }
     exportTableCSVForm * exportForm = new exportTableCSVForm( this, "export table", TRUE );
    exportForm->populateOptions( db.getTableNames());
    if ( exportForm->exec() ) {
	//qDebug(exportForm->option);
	//load our table
	db.browseTable(exportForm->option);
	
	QString fileName = QFileDialog::getSaveFileName(
                    "",
                    "Text files (*.csv *txt)",
                    this,
                    "save file dialog"
                    "Choose a filename to export data" );
    
	if (fileName)
	{
	    QFile file(fileName);
	    if ( file.open( IO_WriteOnly ) ) 
	    {
		char quote = '"';
		char sep = ',';
		char feed = 10;
		int colNum = 0;
		int colCount = 0;
		QTextStream stream( &file );
		//fieldnames on first row
		 QStringList fields = db.browseFields;
		 colCount = fields.count();
		 for ( QStringList::Iterator ct = fields.begin(); ct != fields.end(); ++ct ) {
		     stream << quote;
		     stream << *ct;
		     stream << quote;
		     colNum++;
		     if (colNum<colCount)
		{
			 stream << sep;
		} else {
			stream << feed;
			colNum = 0;
		}
		 }
		
		//now export data
		rowList tab = db.browseRecs;
		rowList::iterator rt;
		
		//int dcols =0;
		QString rowLabel;
		for ( rt = tab.at(0); rt !=tab.end(); ++rt )
		{
		    for ( QStringList::Iterator it = (*rt).begin(); it != (*rt).end(); ++it ) {
			//skip first one (the rowid)
		if (it!=(*rt).begin()){
			    QString content = *it;
			    stream<< quote;
			    QChar qquote = quote;
			    content.replace(quote, QString(qquote).append(qquote));
			    stream<< content;
			    stream<< quote;
			    colNum++;
			    if (colNum<colCount)
			    {
				stream << sep;
			    } else {
				stream << feed;
				colNum = 0;
			    }
			}
	    }
		}
    
		file.close();
		QMessageBox::information( this, applicationName, "Export completed" );
	    }	
	}
	populateStructure();
	resetBrowser();
    }
}


void mainForm::dbState( bool dirty )
{
    fileSaveAction->setEnabled(dirty);
    fileRevertAction->setEnabled(dirty);
}


void mainForm::fileSave()
{
    if (db.isOpen()){
		do {
			if ( !db.save() ) {
				QString error = "Error: could not save the database.\nMessage from database engine:  ";
				error.append(db.lastErrorMessage);
				if ( QMessageBox::warning( this, applicationName, error, QMessageBox::Ok, QMessageBox::Retry|QMessageBox::Default) != QMessageBox::Retry )
					break;
			}
		} while ( db.getDirty() );
    }
}


void mainForm::fileRevert()
{
    if (db.isOpen()){
	QString msg = "Are you sure you want to undo all changes made to the database file \n ";
	msg.append(db.curDBFilename);
	msg.append(" since the last save?");
	if (QMessageBox::question( this, applicationName ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
	    {
		db.revert();
		populateStructure();
		resetBrowser();
	    }
    }
}


void mainForm::exportDatabaseToSQL()
{
        if (!db.isOpen()){
	QMessageBox::information( this, applicationName, "There is no database opened to export" );
	return;
    }

    QString fileName = QFileDialog::getSaveFileName(
                    "",
                    "Text files (*.sql *txt)",
                    0,
                    "save file dialog"
                    "Choose a filename to export" );
    
	if (fileName)
	{
	    if (!db.dump(fileName))
	    {
		QMessageBox::information( this, applicationName, "Could not create export file" );
	    } else {
		QMessageBox::information( this, applicationName, "Export completed" );
	    }
	}
}


void mainForm::importDatabaseFromSQL()
{
    QString fileName = QFileDialog::getOpenFileName(
                    "",
                    "Text files (*.sql *txt)",
                    0,
                    "import file dialog"
                    "Choose a file to import" );
    
    if (fileName)
	{
	QString msg = "Do you want to create a new database file to hold the imported data?\nIf you answer NO we will attempt to import data in the .sql file to the current database.";
	if (QMessageBox::question( this, applicationName ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
	{
	QString newDBfile = QFileDialog::getSaveFileName(
		"",
		"",
		this,
		"create file dialog"
		"Choose a filename to save under" );
	if (QFile::exists(newDBfile) )
	{
	    QString err = "File ";
	    err.append(newDBfile);
	    err.append(" already exists. Please choose a different name");
	    QMessageBox::information( this, applicationName ,err);
	    return;
	}
	if (!fileName.isNull())
	{
	    db.create(newDBfile);
	}
    }
	    int lineErr;
	    if (!db.reload(fileName, &lineErr))
	    {
		QMessageBox::information( this, applicationName, QString("Error importing data at line %1").arg(lineErr) );
	    }
	    else
	    {
		QMessageBox::information( this, applicationName, "Import completed" );
	    }
	    populateStructure();
	    resetBrowser();
	}	
}
