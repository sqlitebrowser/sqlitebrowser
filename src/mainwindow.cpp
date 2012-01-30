#include "mainwindow.h"

#include <QtGui/QFileDialog>
#include <QSettings>
#include <QFile>
#include <QApplication>
#include <QVariant>
#include <QImage>
#include <QPixmap>
#include <QTextStream>
#include <QWhatsThis>
#include <QMessageBox>

#include "createtableform.h"
#include "createindexform.h"
#include "deletetableform.h"
#include "deleteindexform.h"
#include "aboutform.h"
#include "choosetableform.h"
#include "edittableform.h"
#include "editfieldform.h"
#include "importcsvform.h"
#include "exporttablecsvform.h"
#include "preferencesform.h"
#include "editform.h"
#include "findform.h"
#include "sqllogform.h"

/*
 *  Constructs a mainForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
mainForm::mainForm(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
mainForm::~mainForm()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void mainForm::languageChange()
{
    retranslateUi(this);
}

void mainForm::init()
{
    
    findWin = 0;
    editWin = 0;
    logWin = 0;

    clipboard = QApplication::clipboard();

    recsPerView = 1000;
    recAtTop = 0;
    gotoValidator = new QIntValidator(0, 0, this);
    editGoto->setValidator(gotoValidator);
    gotoValidator->setRange ( 0, 0);
    resetBrowser();
    this->setWindowTitle(applicationName);
    this->setWindowIcon( QPixmap( applicationIconName ) );
    buttonNext->setEnabled(false);
    buttonPrevious->setEnabled(false);
    updateRecentFileActions();


    if(!editWin){
        editWin = new editForm(this);
        connect( editWin, SIGNAL( goingAway() ),
                 this,    SLOT( editWinAway() ) );
        connect( editWin, SIGNAL( updateRecordText(int, int , QString) ),
                 this,      SLOT( updateRecordText(int, int , QString) ) );
    }

    if(!logWin){
        logWin = new sqlLogForm(this);
        connect( logWin, SIGNAL( goingAway() ),  this, SLOT( logWinAway() ) );
        connect( logWin, SIGNAL( dbState(bool) ),this, SLOT( dbState(bool)  ) );
    }

    updatePreferences();

    //connect db and log
    db.logWin = logWin;

}

void mainForm::destroy()
{
    if (gotoValidator){
        delete gotoValidator;
    }
}

//***********************************************************
//*** Open File
void mainForm::fileOpen(const QString & fileName)
{
    QString wFile = fileName;
    if (!QFile::exists(wFile))
    {
        wFile = QFileDialog::getOpenFileName(
                    this,
                    "Choose a database file",
                    defaultlocation);
    }
    if (QFile::exists(wFile) )
    {
        if (db.open(wFile))
        {
            this->setWindowTitle(applicationName+" - "+wFile);
            fileCloseAction->setEnabled(true);
            fileCompactAction->setEnabled(true);
            editCreateTableAction->setEnabled(true);
            editDeleteTableAction->setEnabled(true);
            editModifyTableAction->setEnabled(true);
            editCreateIndexAction->setEnabled(true);
            editDeleteIndexAction->setEnabled(true);
            setCurrentFile(wFile);
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
                this,
                "Choose a filename to save under",
                defaultlocation);
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
        this->setWindowTitle(applicationName+" - "+fileName);
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
        setCurrentFile(fileName);
    }
}

//** Populate DbTree Structure
void mainForm::populateStructure()
{
    dbTreeWidget->model()->removeRows(0, dbTreeWidget->model()->rowCount());
    if (!db.isOpen()){
        return;
    }
    db.updateSchema();
    tableMap::Iterator it;
    tableMap tmap = db.tbmap;

    for ( it = tmap.begin(); it != tmap.end(); ++it ) {

        //* Table node
        QTreeWidgetItem *tableItem = new QTreeWidgetItem();
        tableItem->setText(0, it.value().getname());
        tableItem->setText(1,  "table");
        tableItem->setText(3, it.value().getsql());
        tableItem->setIcon(0, QIcon(":/icons/table"));
        dbTreeWidget->addTopLevelItem(tableItem);

        //* Field Nodes
        fieldMap::Iterator fit;
        fieldMap fmap = it.value().fldmap;
        for ( fit = fmap.begin(); fit != fmap.end(); ++fit ) {
            QTreeWidgetItem *fldItem = new QTreeWidgetItem(tableItem);
            fldItem->setText( 0, fit.value().getname() );
            fldItem->setText( 1, "field"  );
            fldItem->setText( 2, fit.value().gettype() );
            fldItem->setIcon(0, QIcon(":/icons/field"));
        }
        // TODO make an options/setting autoexpand
        dbTreeWidget->setItemExpanded(tableItem, true);
    }
    indexMap::Iterator it2;
    indexMap imap = db.idxmap;
    for ( it2 = imap.begin(); it2 != imap.end(); ++it2 ) {
        QTreeWidgetItem *idxItem = new QTreeWidgetItem();
        idxItem->setText( 0, it2.value().getname() );
        idxItem->setText( 1, "index"  );
        idxItem->setText( 3, it2.value().getsql() );
        idxItem->setIcon(0, QIcon(":/icons/index"));
        dbTreeWidget->addTopLevelItem(idxItem);
    }
}

void mainForm::populateTable( const QString & tablename)
{
    bool mustreset = false;
    QApplication::setOverrideCursor( Qt::WaitCursor );
    if (tablename.compare(db.curBrowseTableName)!=0)
        mustreset = true;

    if (!db.browseTable(tablename)){
        dataTable->setRowCount( 0 );
        dataTable->setColumnCount( 0 );
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
    QApplication::restoreOverrideCursor();
}

void mainForm::resetBrowser()
{
    recAtTop = 0;
    comboBrowseTable->clear();
    QStringList tab = db.getTableNames();
    if  (tab.isEmpty()){
        comboBrowseTable->addItem("");
    } else {
        comboBrowseTable->addItems(tab);
    }
    setRecordsetLabel();
    comboBrowseTable->setCurrentIndex(0);
    populateTable(comboBrowseTable->currentText());
}

void mainForm::fileClose()
{
    db.close();
    this->setWindowTitle(applicationName);
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
                db.save();
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
        populateTable(db.curBrowseTableName);
        //added record will be the last one in view
        recAtTop = ((db.getRecordCount()-1)/recsPerView)*recsPerView;
        updateTableView(db.getRecordCount()-recAtTop-1);
    }else{
        QMessageBox::information( this, applicationName,
                                  "Error adding record, make sure a table is selected.\n\n"
                                  "If the table contain fields declared as NOT NULL\n"
                                  "please select EDIT->PREFERENCES and adjust the\n"
                                  "default value for new records to insert an empty string." );
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
        QMessageBox::information( this, applicationName, "Please select a record first" );
    }
}

void mainForm::updateTableView(int lineToSelect)
{
    //  qDebug("line to select value is %d, rowAttop = %d",lineToSelect, recAtTop);
    QApplication::setOverrideCursor( Qt::WaitCursor );

    dataTable->setRowCount(0);
    dataTable->setColumnCount( db.browseFields.count() );
    dataTable->setHorizontalHeaderLabels(db.browseFields);

    rowList tab = db.browseRecs;
    int maxRecs = db.getRecordCount();
    int recsThisView = maxRecs - recAtTop;

    if (recsThisView>recsPerView)
        recsThisView = recsPerView;

    // qDebug("recsthisview= %d\n",recsThisView);

    dataTable->setRowCount(recsThisView);

    if ( recsThisView > 0 ) {

        int rowNum = 0;
        int colNum = 0;
        //int dcols =0;
        QString rowLabel;
        for (int i = recAtTop; i < tab.size(); ++i)
            //for ( int  = tab.at(recAtTop); rt !=tab.end(); ++rt )
        {
            rowLabel.setNum(recAtTop+rowNum+1);
            dataTable->setVerticalHeaderItem(rowNum, new QTableWidgetItem( rowLabel ));
            colNum = 0;
            QStringList& rt = tab[i];
            for (int e = 1; e < rt.size(); ++e)
                //for ( QStringList::Iterator it = (*rt).begin(); it != (*rt).end(); ++it )
            {
                //skip first one (the rowid)
                //  if (it!=(*rt).begin()){
                QString& content = rt[e];
                QString firstline = content.section( '\n', 0,0 );
                if (content.length()>MAX_DISPLAY_LENGTH)
                {
                    firstline.truncate(MAX_DISPLAY_LENGTH);
                    firstline.append("...");
                }
                dataTable->setItem( rowNum, colNum, new QTableWidgetItem(firstline));
                colNum++;
                //}
            }
            rowNum++;
            if (rowNum==recsThisView) break;
        }

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
    dataTable->clearSelection();
    dataTable->selectRow(lineToSelect);
    dataTable->setCurrentCell(lineToSelect, 0);
    dataTable->scrollToItem(dataTable->itemAt(lineToSelect, 0));
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
    QString finaloperator = woperator;
    QString finalsearchterm = wsearchterm;
    
    //special case for CONTAINS operator: use LIKE and surround the search word with % characters
    if (woperator.compare("contains")==0){
        finaloperator = QString("LIKE");
        QString newsearchterm = "%";
        newsearchterm.append(wsearchterm);
        newsearchterm.append("%");
        finalsearchterm = QString(newsearchterm);
    }
    QApplication::setOverrideCursor( Qt::WaitCursor );
    QString statement = "SELECT rowid, ";
    statement.append(wfield);
    statement.append("  FROM ");
    statement.append(db.curBrowseTableName);
    statement.append(" WHERE ");
    statement.append(wfield);
    statement.append(" ");
    statement.append(finaloperator);
    statement.append(" ");
    //searchterm needs to be quoted if it is not a number
    bool ok = false;
    wsearchterm.toDouble(&ok);
    if (!ok) wsearchterm.toInt(&ok, 10);
    if (!ok) {//not a number, quote it
        char * formSQL = sqlite3_mprintf("%Q",(const char *) finalsearchterm.toUtf8());
        statement.append(formSQL);
        if (formSQL) sqlite3_free(formSQL);
    } else {//append the number, unquoted
        statement.append(finalsearchterm);
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
    createTableForm * tableForm = new createTableForm( this );
    tableForm->setModal(true);
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
    createIndexForm * indexForm = new createIndexForm( this );
    indexForm->setModal(true);
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
    QApplication::setOverrideCursor( Qt::WaitCursor );
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
    deleteTableForm * tableForm = new deleteTableForm( this );
    tableForm->setModal(true);
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

//*****************************************
//** Edit Table
void mainForm::editTable()
{
    if (!db.isOpen()){
        QMessageBox::information( this, applicationName, "There is no database opened." );
        return;
    }
    chooseTableForm * tableForm = new chooseTableForm( this );
    tableForm->setModal(true);
    QStringList tablelist = db.getTableNames();
    if (tablelist.empty()){
        QMessageBox::information( this, applicationName, "There are no tables to edit in this database." );
        return;
    }
    tableForm->populateOptions( tablelist );
    if ( tableForm->exec() ) {
        //statement.append(tableForm->option);
        editTableForm * edTableForm = new editTableForm( this );
        edTableForm->setModal(true);
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
void mainForm::editTablePopup()
{
    if (!db.isOpen()){
        QMessageBox::information( this, applicationName, "There is no database opened." );
        return;
    }
    if(!dbTreeWidget->selectionModel()->hasSelection()){
        return;
    }
    QString tableToEdit =dbTreeWidget->currentItem()->text(0);
    qDebug(tableToEdit.toUtf8());
    //chooseTableForm * tableForm = new chooseTableForm( this, "choosetable", TRUE );
    //QStringList tablelist = db.getTableNames();
    //if (tablelist.empty()){
    //    QMessageBox::information( this, applicationName, "There are no tables to edit in this database." );
    //    return;
    //}
    //tableForm->populateOptions( tablelist );
    //if ( tableForm->exec() ) {
    //statement.append(tableForm->option);
    editTableForm * edTableForm = new editTableForm( this );
    edTableForm->setModal(true);
    //send table name ? or handle it all from here?
    edTableForm->setActiveTable(&db, tableToEdit);
    edTableForm->exec();
    //check modified status
    if (edTableForm->modified)
    {
        populateStructure();
        resetBrowser();
    }
    // }
}

void mainForm::deleteIndex()
{
    if (!db.isOpen()){
        QMessageBox::information( this, applicationName, "There is no database opened." );
        return;
    }
    deleteIndexForm * indexForm = new deleteIndexForm( this );
    indexForm->setModal(true);
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
        if (QString(t->metaObject()->className()) == "QLineEdit"){
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
        if ( QString(t->metaObject()->className()) == "QLineEdit" ){
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
    aboutForm * aForm = new aboutForm( this );
    aForm ->exec() ;
}


void mainForm::updateRecordText(int row, int col, QString newtext)
{
    if (!db.updateRecord(row, col, newtext)){
        QMessageBox::information( this, applicationName, "Data could not be updated" );
    }

    rowList tab = db.browseRecs;
    QStringList& rt = tab[row];
    QString& cv = rt[col+1];//must account for rowid

    QString content = cv ;
    QString firstline = content.section( '\n', 0,0 );
    if (content.length()>14)
    {
        firstline.truncate(14);
        firstline.append("...");
    }
    dataTable->setItem( row - recAtTop, col, new QTableWidgetItem(firstline));
}

void mainForm::logWinAway()
{
    sqlLogAction->toggle();
}

void mainForm::editWinAway()
{
    editWin->hide();
    activateWindow();
    dataTable->setRangeSelected( QTableWidgetSelectionRange(editWin->curRow - recAtTop, editWin->curCol, editWin->curRow- recAtTop, editWin->curCol), true);
}



void mainForm::editText(int row, int col)
{
    rowList tab = db.browseRecs;
    QStringList& rt = tab[row];
    QString cv = rt[col+1];//must account for rowid

    editWin->loadText(cv , row, col);
    editWin ->show();
}



void mainForm::doubleClickTable( int row, int col )
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
    QString query = db.GetEncodedQString(sqlTextEdit->toPlainText());
    if (query.isEmpty())
    {
        QMessageBox::information( this, applicationName, "Query string is empty" );
        return;
    }
    //log the query
    db.logSQL(query, kLogMsg_User);
    sqlite3_stmt *vm;
    const char *tail=NULL;
    int ncol;
    int err=0;
    QString lastErrorMessage = QString("No error");
    //Accept multi-line queries, by looping until the tail is empty
    while (1) {
        if (tail!=NULL) {
            query = QString(tail);
        }
        queryResultListModel->removeRows(0, queryResultListModel->rowCount());
        queryResultListModel->setHorizontalHeaderLabels(QStringList());
        queryResultListModel->setVerticalHeaderLabels(QStringList());

        err=sqlite3_prepare(db._db,query.toUtf8(),query.length(),
                            &vm, &tail);
        if (err == SQLITE_OK){
            db.setDirty(true);
            int rownum = 0;
            bool mustCreateColumns = true;
            while ( sqlite3_step(vm) == SQLITE_ROW ){
                ncol = sqlite3_data_count(vm);
                //setup num of cols here for display grid
                if (mustCreateColumns)
                {
                    QStringList headerList;
                    for (int e=0; e<ncol; e++)
                        headerList = headerList << sqlite3_column_name(vm, e);
                    queryResultListModel->setHorizontalHeaderLabels(headerList);
                    mustCreateColumns = false;
                }
                for (int e=0; e<ncol; e++){
                    char * strresult = 0;
                    QString rv;
                    strresult = (char *) sqlite3_column_text(vm, e);
                    rv = QString(strresult);
                    //show it here
                    QString decoded = db.GetDecodedQString(rv);
                    QString firstline = decoded.section( '\n', 0,0 );
                    if (firstline.length()>MAX_DISPLAY_LENGTH)
                    {
                        firstline.truncate(MAX_DISPLAY_LENGTH);
                        firstline.append("...");
                    }
                    queryResultListModel->setItem(rownum, e, new QStandardItem(QString(firstline)));
                }
                queryResultListModel->setVerticalHeaderItem(rownum, new QStandardItem(QString::number(rownum + 1)));
                rownum++;
            }
            sqlite3_finalize(vm);
        }else{
            lastErrorMessage = QString (sqlite3_errmsg(db._db));
        }
        queryErrorLineEdit->setText(lastErrorMessage);
        queryResultTableView->resizeColumnsToContents();

        if ((!tail)||(*tail==0)) break;
        if(err!=SQLITE_OK) break;
    }
}


void mainForm::mainTabSelected(const QString & tabname)
{
    if ((mainTab->currentIndex() == 0)||(mainTab->currentIndex() == 1))
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

    // Not needed anymore due to nested savepoints
    /*     if (db.getDirty())
     {
  QString msg = "Database needs to be saved before the import operation.\nSave current changes and continue?";
  if (QMessageBox::question( this, applicationName ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
  {
     db.save();
  } else {
   return;
  }
    }*/

    QString wFile = QFileDialog::getOpenFileName(
                this,
                "Choose a text file",
                defaultlocation,
                "Text files (*.csv *.txt)");

    if (QFile::exists(wFile) )
    {
        importCSVForm * csvForm = new importCSVForm( this );
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
    exportTableCSVForm * exportForm = new exportTableCSVForm( this );
    exportForm->populateOptions( db.getTableNames());
    if ( exportForm->exec() ) {
        //qDebug(exportForm->option);
        //load our table
        db.browseTable(exportForm->option);

        QString fileName = QFileDialog::getSaveFileName(
                    this,
                    "Choose a filename to export data",
                    defaultlocation,
                    "Text files (*.csv *txt)");

        if (fileName.size() > 0)
        {
            QFile file(fileName);
            if ( file.open( QIODevice::WriteOnly ) )
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

                //int dcols =0;
                QString rowLabel;
                for ( int i=0; i<tab.size(); ++i )
                {
                    QStringList& rt = tab[i];
                    for ( int e=1; e<rt.size(); ++e ) {
                        QString content = rt[e];
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
        db.save();
        //dbStatusBar->showMessage("Date written to file", 4000)
    }
}


void mainForm::fileRevert()
{
    if (db.isOpen()){
        QString msg = "Are you sure you want to undo all changes made to the database file \n\n<b> ";
        msg.append(db.curDBFilename);
        msg.append("</b>\n since the last save?");
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
                this,
                "Choose a filename to export",
                defaultlocation,
                "Text files (*.sql *txt)");

    if (fileName.size() > 0)
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
                this,
                "Choose a file to import",
                defaultlocation,
                "Text files (*.sql *txt)");

    if (fileName.size() > 0)
    {
        QString msg = "Do you want to create a new database file to hold the imported data?\nIf you answer NO we will attempt to import data in the .sql file to the current database.";
        if (QMessageBox::question( this, applicationName ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
        {
            QString newDBfile = QFileDialog::getSaveFileName(
                        this,
                        "Choose a filename to save under",
                        defaultlocation);
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


void mainForm::openPreferences()
{
    preferencesForm * prefForm = new preferencesForm( this );
    if ( prefForm->exec() ) {
        updatePreferences();
        resetBrowser();
    }
}

void mainForm::updatePreferences()
{
    preferencesForm * prefForm = new preferencesForm( this );
    prefForm->loadSettings();

    if (prefForm->defaultencoding=="Latin1")
    {
        db.setEncoding(kEncodingLatin1);
    } else {
        db.setEncoding(kEncodingUTF8);
    }
    db.setDefaultNewData(prefForm->defaultnewdata);
    defaultlocation= prefForm->defaultlocation;
    editWin->defaultlocation = defaultlocation;
    editWin->setTextFormat(prefForm->defaulttext);
}

//******************************************************************
//** Tree Events
//******************************************************************

//** Db Tree Context Menu
void mainForm::on_tree_context_menu(const QPoint &qPoint){
    if( !dbTreeWidget->selectionModel()->hasSelection() ){
        return;
    }
    QTreeWidgetItem *cItem = dbTreeWidget->currentItem();

    if(cItem->text(1) == "table"){
        editDeleteTableActionPopup->setDisabled(false);
        editModifyTableActionPopup->setDisabled(false);
        popupTableMenu->exec( dbTreeWidget->mapToGlobal(qPoint) );

    }else if(cItem->text(1) == "field"){
        popupFieldMenu->exec( dbTreeWidget->mapToGlobal(qPoint) );
    }
}
//** Tree selection changed
void mainForm::on_tree_selection_changed(){
    if (!dbTreeWidget->selectionModel()->hasSelection()){
        editDeleteTableActionPopup->setEnabled(false);
        editModifyTableActionPopup->setEnabled(false);
        editAddFieldActionPopup->setEnabled(false);
        editModifyFieldActionPopup->setEnabled(false);
        editDeleteFieldActionPopup->setEnabled(false);
        return;
    }

    if(dbTreeWidget->currentItem()->text(1) == "table"){
        editDeleteTableActionPopup->setEnabled(true);
        editModifyTableActionPopup->setEnabled(true);
        editAddFieldActionPopup->setEnabled(true);
        editModifyFieldActionPopup->setEnabled(false);
        editDeleteFieldActionPopup->setEnabled(false);

    }else if(dbTreeWidget->currentItem()->text(1) == "field"){
        editAddFieldActionPopup->setEnabled(false);
        editDeleteTableActionPopup->setEnabled(false);
        editModifyTableActionPopup->setEnabled(false);
        editModifyFieldActionPopup->setEnabled(true);
        editDeleteFieldActionPopup->setEnabled(true);
    }
}


void mainForm::on_add_field(){
    qDebug("YES");
    //if( !dbTreeWidget->currentItem() ){
    //   return;
    //}
    //QTreeWidgetItem *item = dbTreeWidget->currentItem();
    editFieldForm *fieldForm = new editFieldForm( this );
    //qDebug(item->text(2));
    fieldForm->setInitialValues(true, dbTreeWidget->currentItem()->text(0), "", "TEXT");
    fieldForm->setDB(this->db);
    if (fieldForm->exec())
    {
        //modified = true;
        //do the sql rename here
        //populateStructure();
        qDebug((fieldForm->field_name + fieldForm->field_type).toUtf8());
        //* find parent item
        populateStructure();
        //       QTreeWidgetItem *parentItem = dbTreeWidget->findItems()
        return;
        QTreeWidgetItem *newItem = new QTreeWidgetItem(dbTreeWidget->currentItem());
        newItem->setText(0, fieldForm->field_name);
        newItem->setText(1, "field");
        newItem->setText(2, fieldForm->field_type);
    }
}
void mainForm::on_edit_field(){
    qDebug("YES");
    if( !dbTreeWidget->currentItem() ){
        return;
    }
    QTreeWidgetItem *item = dbTreeWidget->currentItem();
    editFieldForm *fieldForm = new editFieldForm( this );
    qDebug(item->text(2).toUtf8());
    fieldForm->setInitialValues(false, "TABLE_NAME", item->text(0), item->text(2));
    fieldForm->setDB(this->db);
    if (fieldForm->exec())
    {
        //modified = true;
        //do the sql rename here
        //qDebug(fieldForm->name + fieldForm->type);
        item->setText(0,fieldForm->field_name);
        item->setText(2,fieldForm->field_type);
    }
}

void mainForm::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        fileOpen(action->data().toString());
}

void mainForm::updateRecentFileActions()
{
    QSettings settings(sOrganisation, sApplicationNameShort);
    QStringList files = settings.value("recentFileList").toStringList();


    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(files[i]);
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    recentSeparatorAct->setVisible(numRecentFiles > 0);
}

void mainForm::setCurrentFile(const QString &fileName)
{
    setWindowFilePath(fileName);

    QSettings settings(sOrganisation, sApplicationNameShort);
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        mainForm *mainWin = qobject_cast<mainForm *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}
