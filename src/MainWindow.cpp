#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QtGui/QFileDialog>
#include <QSettings>
#include <QFile>
#include <QApplication>
#include <QTextStream>
#include <QWhatsThis>
#include <QMessageBox>
#include <QUrl>
#include <QStandardItemModel>
#include <QDragEnterEvent>
#include <QScrollBar>
#include "CreateIndexForm.h"
#include "DeleteIndexForm.h"
#include "DeleteTableForm.h"
#include "DialogAbout.h"
#include "ChooseTableForm.h"
#include "EditTableForm.h"
#include "EditFieldForm.h"
#include "ImportCSVForm.h"
#include "ExportTableCSVForm.h"
#include "PreferencesForm.h"
#include "EditForm.h"
#include "FindForm.h"
#include "SQLLogDock.h"
#include "SQLiteSyntaxHighlighter.h"

void MainWindow::setupUi()
{
    // Create the SQL log dock
    logWin = new SQLLogDock(this);
    this->addDockWidget(Qt::BottomDockWidgetArea, logWin);

    // Set up the db tree widget
    ui->dbTreeWidget->setColumnHidden(1, true);
    ui->dbTreeWidget->setColumnWidth(0, 300);

    // Create the SQL sytax highlighter
    sqliteHighlighter = new SQLiteSyntaxHighlighter(ui->sqlTextEdit->document());

    // Set up DB model
    queryResultListModel = new QStandardItemModel(this);
    ui->queryResultTableView->setModel(queryResultListModel);

    // Create the actions for the recently opened dbs list
    for(int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        this->connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
    for(int i = 0; i < MaxRecentFiles; ++i)
        ui->fileMenu->insertAction(ui->fileExitAction, recentFileActs[i]);
    recentSeparatorAct = ui->fileMenu->insertSeparator(ui->fileExitAction);

    // Create popup menus
    popupTableMenu = new QMenu(this);
    popupTableMenu->addAction(ui->editModifyTableActionPopup);
    popupTableMenu->addAction(ui->editAddFieldActionPopup);
    popupTableMenu->addSeparator();
    popupTableMenu->addAction(ui->editDeleteTableActionPopup);
    popupFieldMenu = new QMenu(this);
    popupFieldMenu->addAction(ui->editModifyFieldActionPopup);
    popupFieldMenu->addAction(ui->editDeleteFieldActionPopup);

    // Connect some more signals and slots
    connect(ui->dataTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(browseTableHeaderClicked(int)));
    connect(ui->sqlLogAction, SIGNAL(toggled(bool)), logWin, SLOT(setVisible(bool)));
    connect(ui->dataTable->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setRecordsetLabel()));
}

/*
 *  Constructs a mainForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUi();
    setAcceptDrops(true);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
MainWindow::~MainWindow()
{
    delete ui;
    destroy();
}

void MainWindow::init()
{
    findWin = 0;
    editWin = 0;

    clipboard = QApplication::clipboard();

    gotoValidator = new QIntValidator(0, 0, this);
    ui->editGoto->setValidator(gotoValidator);
    resetBrowser();
    this->setWindowTitle(QApplication::applicationName());
    this->setWindowIcon( QPixmap( g_applicationIconName ) );
    updateRecentFileActions();


    if(!editWin){
        editWin = new editForm(this);
        connect( editWin, SIGNAL( goingAway() ),
                 this,    SLOT( editWinAway() ) );
        connect( editWin, SIGNAL( updateRecordText(int, int , QString) ),
                 this,      SLOT( updateRecordText(int, int , QString) ) );
    }

    connect( logWin, SIGNAL( goingAway() ),  this, SLOT( logWinAway() ) );
    connect( logWin, SIGNAL( dbState(bool) ),this, SLOT( dbState(bool)  ) );

    updatePreferences();

    //connect db and log
    db.logWin = logWin;

    //load last ui settings
    QSettings settings(QApplication::organizationName(), g_sApplicationNameShort);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());
    logWin->comboLogType()->setCurrentIndex(logWin->comboLogType()->findText(settings.value("SQLLogDock/Log", "Application").toString()));

    ui->sqlLogAction->setChecked(!logWin->isHidden());
    ui->viewDBToolbarAction->setChecked(!ui->toolbarDB->isHidden());
}

void MainWindow::destroy()
{
    if (gotoValidator){
        delete gotoValidator;
    }
}

//***********************************************************
//*** Open File
void MainWindow::fileOpen(const QString & fileName)
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
            this->setWindowTitle(QApplication::applicationName() +" - "+wFile);
            setCurrentFile(wFile);
        } else {
            QString err = "An error occurred:  ";
            err.append(db.lastErrorMessage);
            QMessageBox::information( this, QApplication::applicationName() ,err);
        }
        populateStructure();
        resetBrowser();
    }
}


void MainWindow::fileOpen()
{
    fileOpen(QString());
}



void MainWindow::fileNew()
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
        QMessageBox::information( this, QApplication::applicationName() ,err);
        return;
    }
    if (!fileName.isNull())
    {
        db.create(fileName);
        this->setWindowTitle( QApplication::applicationName() +" - "+fileName);
        populateStructure();
        resetBrowser();
        createTable();
        setCurrentFile(fileName);
    }
}

//** Populate DbTree Structure
void MainWindow::populateStructure()
{
    ui->dbTreeWidget->model()->removeRows(0, ui->dbTreeWidget->model()->rowCount());
    if (!db.isOpen()){
        return;
    }
    db.updateSchema();
    QStringList tblnames = db.getTableNames();
    sqliteHighlighter->setTableNames(tblnames);
    tableMap::ConstIterator it;

    for ( it = db.tbmap.begin(); it != db.tbmap.end(); ++it ) {

        //* Table node
        QTreeWidgetItem *tableItem = new QTreeWidgetItem();
        tableItem->setText(0, it.value().getname());
        tableItem->setText(1,  "table");
        tableItem->setText(3, it.value().getsql());
        tableItem->setIcon(0, QIcon(":/icons/table"));
        ui->dbTreeWidget->addTopLevelItem(tableItem);

        //* Field Nodes
        fieldMap::ConstIterator fit;
        for ( fit = it.value().fldmap.begin(); fit != it.value().fldmap.end(); ++fit ) {
            QTreeWidgetItem *fldItem = new QTreeWidgetItem(tableItem);
            fldItem->setText( 0, fit.value().getname() );
            fldItem->setText( 1, "field"  );
            fldItem->setText( 2, fit.value().gettype() );
            fldItem->setIcon(0, QIcon(":/icons/field"));
        }
        // TODO make an options/setting autoexpand
        ui->dbTreeWidget->setItemExpanded(tableItem, true);
    }
    objectMap::ConstIterator it2;
    for ( it2 = db.idxmap.begin(); it2 != db.idxmap.end(); ++it2 ) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText( 0, it2.value().getname() );
        item->setText( 1, "index"  );
        item->setText( 3, it2.value().getsql() );
        item->setIcon(0, QIcon(":/icons/index"));
        ui->dbTreeWidget->addTopLevelItem(item);
    }
    for ( it2 = db.viewmap.begin(); it2 != db.viewmap.end(); ++it2 ) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText( 0, it2.value().getname() );
        item->setText( 1, "view"  );
        item->setText( 3, it2.value().getsql() );
        item->setIcon(0, QIcon(":/icons/view"));
        ui->dbTreeWidget->addTopLevelItem(item);
    }
    for ( it2 = db.trgmap.begin(); it2 != db.trgmap.end(); ++it2 ) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText( 0, it2.value().getname() );
        item->setText( 1, "trigger"  );
        item->setText( 3, it2.value().getsql() );
        item->setIcon(0, QIcon(":/icons/trigger"));
        ui->dbTreeWidget->addTopLevelItem(item);
    }
}

void MainWindow::populateTable( const QString & tablename, bool keepColumnWidths)
{
    bool mustreset = false;
    QApplication::setOverrideCursor( Qt::WaitCursor );
    if (tablename.compare(db.curBrowseTableName)!=0)
        mustreset = true;

    QString orderby = QString::number(curBrowseOrderByIndex) + " " + (curBrowseOrderByMode == ORDERMODE_ASC ? "ASC" : "DESC");
    if (!db.browseTable(tablename, orderby)){
        ui->dataTable->setRowCount( 0 );
        ui->dataTable->setColumnCount( 0 );
        QApplication::restoreOverrideCursor();
        if (findWin){
            findWin->resetFields(db.getTableFields(""));
            findWin->resetResults();
        }
        return;
    }

    if (mustreset){
        updateTableView(0, keepColumnWidths);
        if (findWin) findWin->resetFields(db.getTableFields(db.curBrowseTableName));
    } else {
        updateTableView(-1, keepColumnWidths);
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

void MainWindow::resetBrowser()
{
    QString sCurrentTable = ui->comboBrowseTable->currentText();
    ui->comboBrowseTable->clear();
    QStringList tab = db.getTableNames();
    if(!tab.isEmpty()) {
        ui->comboBrowseTable->addItems(tab);
    }
    setRecordsetLabel();
    int pos = ui->comboBrowseTable->findText(sCurrentTable);
    pos = pos == -1 ? 0 : pos;
    ui->comboBrowseTable->setCurrentIndex(pos);
    curBrowseOrderByIndex = 1;
    curBrowseOrderByMode = ORDERMODE_ASC;
    populateTable(ui->comboBrowseTable->currentText());
}

void MainWindow::fileClose()
{
    db.close();
    this->setWindowTitle(QApplication::applicationName());
    resetBrowser();
    populateStructure();
    activateFields(false);
}


void MainWindow::fileExit()
{
    if (db.isOpen())
    {
        if (db.getDirty())
        {
            QString msg = "Do you want to save the changes made to the database file ";
            msg.append(db.curDBFilename);
            msg.append("?");
            if (QMessageBox::question( this, QApplication::applicationName() ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
            {
                db.save();
            } else {
                //not really necessary, I think... but will not hurt.
                db.revert();
            }
        }
        db.close();
    }
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    QSettings settings(QApplication::organizationName(), g_sApplicationNameShort);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
    settings.setValue("SQLLogDock/Log", logWin->comboLogType()->currentText());
    fileExit();
    QMainWindow::closeEvent(event);
}

void MainWindow::addRecord()
{
    if (db.addRecord()){
        populateTable(db.curBrowseTableName);
        //added record will be the last one in view
        updateTableView(db.getRecordCount()-1);
    }else{
        QMessageBox::information( this, QApplication::applicationName(),
                                  "Error adding record, make sure a table is selected.\n\n"
                                  "If the table contain fields declared as NOT NULL\n"
                                  "please select EDIT->PREFERENCES and adjust the\n"
                                  "default value for new records to insert an empty string." );
    }
}


void MainWindow::deleteRecord()
{
    if (ui->dataTable->currentRow()!=-1){
        int lastselected = ui->dataTable->currentRow();
        db.deleteRecord(ui->dataTable->currentRow());
        populateTable(db.curBrowseTableName);
        int nextselected = lastselected ;
        if (nextselected > db.getRecordCount()){
            nextselected = db.getRecordCount();
        }
        if (nextselected>0){
            selectTableLine(nextselected);
        }
    } else {
        QMessageBox::information( this, QApplication::applicationName(), "Please select a record first" );
    }
}

#define WRAP_SIZE 80
QString wrapText(const QString& text)
{
    QString wrap;
    int textSize = text.size();

    int cur = 0;
    while( wrap.size() < textSize)
    {
        wrap += text.mid(cur, WRAP_SIZE);
        cur += WRAP_SIZE;
        if( textSize - cur > WRAP_SIZE)
            wrap += '\n';
    }

    return wrap;
}

void MainWindow::updateTableView(int lineToSelect, bool keepColumnWidths)
{
    QApplication::setOverrideCursor( Qt::WaitCursor );

    ui->dataTable->setRowCount(db.getRecordCount());
    ui->dataTable->setColumnCount( db.browseFields.count() );
    ui->dataTable->setHorizontalHeaderLabels(db.browseFields);

    rowList tab = db.browseRecs;
    int maxRecs = db.getRecordCount();
    gotoValidator->setRange(0, maxRecs);

    if ( maxRecs > 0 ) {

        int rowNum = 0;
        int colNum = 0;
        QString rowLabel;
        for (int i = 0; i < tab.size(); ++i)
        {
            rowLabel.setNum(rowNum+1);
            ui->dataTable->setVerticalHeaderItem(rowNum, new QTableWidgetItem( rowLabel ));
            colNum = 0;
            QStringList& rt = tab[i];
            for (int e = 1; e < rt.size(); ++e)
            {
                QString& content = rt[e];

                QTableWidgetItem* item = new QTableWidgetItem(content);
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                item->setToolTip(wrapText(content));
                ui->dataTable->setItem( rowNum, colNum, item);
                colNum++;
            }
            rowNum++;
            if (rowNum==maxRecs) break;
        }
    }

    if(!keepColumnWidths) {
        for(int i = 0; i < ui->dataTable->columnCount(); ++i)
        {
            ui->dataTable->resizeColumnToContents(i);
            if( ui->dataTable->columnWidth(i) > 400 )
                ui->dataTable->setColumnWidth(i, 400);
        }
    }
    if (lineToSelect!=-1){
        selectTableLine(lineToSelect);
    }
    setRecordsetLabel();
    QApplication::restoreOverrideCursor();
}

void MainWindow::selectTableLine(int lineToSelect)
{
    ui->dataTable->clearSelection();
    ui->dataTable->selectRow(lineToSelect);
    ui->dataTable->setCurrentCell(lineToSelect, 0);
    ui->dataTable->scrollToItem(ui->dataTable->itemAt(lineToSelect, 0));
}

void MainWindow::navigatePrevious()
{
    int curRow = ui->dataTable->currentRow();
    curRow -= 100;
    if(curRow < 0) curRow = 0;
    updateTableView(curRow);
}


void MainWindow::navigateNext()
{
    int curRow = ui->dataTable->currentRow();
    curRow += 100;
    if(curRow >= ui->dataTable->rowCount()) curRow = ui->dataTable->rowCount()-1;
    updateTableView(curRow);
}


void MainWindow::navigateGoto()
{
    QString typed = ui->editGoto->text();
    bool ok;
    int dec = typed.toInt( &ok);
    if (dec==0) dec=1;
    if (dec>db.getRecordCount()) dec = db.getRecordCount();

    updateTableView(dec-1);
    ui->editGoto->setText(QString::number(dec,10));
}

void MainWindow::setRecordsetLabel()
{
    int from = ui->dataTable->verticalHeader()->visualIndexAt(0) + 1;
    int to = ui->dataTable->verticalHeader()->visualIndexAt(ui->dataTable->height()) - 1;
    int total = ui->dataTable->rowCount();
    if(to == -2)
        to = total;

    ui->labelRecordset->setText(QString("%1 - %2 of %3").arg(from).arg(to).arg(total));
}

void MainWindow::browseFind(bool open)
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

void MainWindow::browseFindAway()
{
    ui->buttonFind->toggle();
}

void MainWindow::browseRefresh()
{
    populateTable(ui->comboBrowseTable->currentText(), true);
}

void MainWindow::lookfor( const QString & wfield, const QString & woperator, const QString & wsearchterm )
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened. Please open or create a new database file." );
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

void MainWindow::showrecord( int dec )
{
    updateTableView(dec);
}

void MainWindow::createTable()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened. Please open or create a new database file." );
        return;
    }

    editTableForm * tableForm = new editTableForm(this);
    tableForm->setActiveTable(&db, "");
    tableForm->setModal(true);
    if(tableForm->exec()) {
        populateStructure();
        resetBrowser();
    }
}

void MainWindow::createIndex()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened. Please open or create a new database file." );
        return;
    }
    createIndexForm * indexForm = new createIndexForm( this );
    indexForm->setModal(true);
    indexForm->populateTable( db.tbmap );
    if ( indexForm->exec() ) {
        if (!db.executeSQL(indexForm->createStatement)){
            QString error = "Error: could not create the index. Message from database engine:  ";
            error.append(db.lastErrorMessage);
            QMessageBox::warning( this, QApplication::applicationName(), error );
        } else {
            populateStructure();
            resetBrowser();
        }
    }
}


void MainWindow::compact()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );
    if (db.isOpen()){
        if (!db.compact()){
            QString error = "Error: could not compact the database file. Message from database engine:  ";
            error.append(db.lastErrorMessage);
            QMessageBox::warning( this, QApplication::applicationName(), error );
        } else {
            QMessageBox::warning( this, QApplication::applicationName(), "Database compacted" );
        }
    }
    db.open(db.curDBFilename);
    populateStructure();
    resetBrowser();
    ui->fileCloseAction->setEnabled(true);
    ui->fileCompactAction->setEnabled(true);
    QApplication::restoreOverrideCursor( );
}


void MainWindow::deleteTable()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened." );
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
            QMessageBox::warning( this, QApplication::applicationName(), error );
        } else {
            populateStructure();
            resetBrowser();
        }
    }
}

//*****************************************
//** Edit Table
void MainWindow::editTable()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened." );
        return;
    }
    chooseTableForm * tableForm = new chooseTableForm( this );
    tableForm->setModal(true);
    QStringList tablelist = db.getTableNames();
    if (tablelist.empty()){
        QMessageBox::information( this, QApplication::applicationName(), "There are no tables to edit in this database." );
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

void MainWindow::deleteTablePopup()
{
    // Get name of table to delete
    QString table = ui->dbTreeWidget->currentItem()->text(0);

    // Ask user if he really wants to delete that table
    if(QMessageBox::warning(this, QApplication::applicationName(), tr("Are you sure you want to delete the table '%1'?\nAll data in the table will be lost.").arg(table),
                            QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape) == QMessageBox::Yes)
    {
        // Delete the table
        QString statement = QString("DROP TABLE %1;").arg(table);
        if (!db.executeSQL( statement)) {
            QString error = QString("Error: could not delete the table. Message from database engine:\n%1").arg(db.lastErrorMessage);
            QMessageBox::warning( this, QApplication::applicationName(), error );
        } else {
            populateStructure();
            resetBrowser();
        }
    }
}

void MainWindow::editTablePopup()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened." );
        return;
    }
    if(!ui->dbTreeWidget->selectionModel()->hasSelection()){
        return;
    }
    QString tableToEdit = ui->dbTreeWidget->currentItem()->text(0);

    editTableForm * edTableForm = new editTableForm( this );
    edTableForm->setModal(true);
    edTableForm->setActiveTable(&db, tableToEdit);
    edTableForm->exec();
    //check modified status
    if (edTableForm->modified)
    {
        populateStructure();
        resetBrowser();
    }
}

void MainWindow::deleteIndex()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened." );
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
            QMessageBox::warning( this, QApplication::applicationName(), error );
        } else {
            populateStructure();
            resetBrowser();
        }
    }

}


void MainWindow::copy()
{
    QWidget * t = ui->dataTable->cellWidget(ui->dataTable->currentRow(), ui->dataTable->currentColumn());
    if (t!=0){
        if (QString(t->metaObject()->className()) == "QLineEdit"){
            /*we are in edit mode*/
            if (t->hasFocus()){
                QLineEdit * le = (QLineEdit *) t;
                le->copy();
            }
        }
    }
    if (ui->editGoto->hasFocus())
        ui->editGoto->copy();
}


void MainWindow::paste()
{
    QWidget * t = ui->dataTable->cellWidget(ui->dataTable->currentRow(), ui->dataTable->currentColumn());
    if (t!=0){
        if ( QString(t->metaObject()->className()) == "QLineEdit" ){
            /*we are in edit mode*/
            if (t->hasFocus()){
                QLineEdit * le = (QLineEdit *) t;
                le->paste();}
        }
    }
    if (ui->editGoto->hasFocus())
        ui->editGoto->paste();
}


void MainWindow::helpWhatsThis()
{
    QWhatsThis::enterWhatsThisMode ();
}


void MainWindow::helpAbout()
{
    DialogAbout * aForm = new DialogAbout( this );
    aForm ->exec() ;
}


void MainWindow::updateRecordText(int row, int col, QString newtext)
{
    if (!db.updateRecord(row, col, newtext)){
        QMessageBox::information( this, QApplication::applicationName(), "Data could not be updated" );
    }

    rowList tab = db.browseRecs;
    QStringList& rt = tab[row];
    QString& cv = rt[col+1];//must account for rowid

    QTableWidgetItem* item = new QTableWidgetItem(cv);
    item->setToolTip( wrapText(cv) );
    ui->dataTable->setItem(row, col, item);

}

void MainWindow::logWinAway()
{
    ui->sqlLogAction->toggle();
}

void MainWindow::editWinAway()
{
    editWin->hide();
    activateWindow();
    ui->dataTable->setRangeSelected( QTableWidgetSelectionRange(editWin->curRow, editWin->curCol, editWin->curRow, editWin->curCol), true);
}



void MainWindow::editText(int row, int col)
{
    rowList tab = db.browseRecs;
    QStringList& rt = tab[row];
    QString cv = rt[col+1];//must account for rowid

    editWin->loadText(cv , row, col);
    editWin ->show();
}



void MainWindow::doubleClickTable( int row, int col )
{
    if ((row==-1) || (col==-1)){
        qDebug("no cell selected");
        return;
    }

    int realRow = row;

    editText(realRow , col);
}


void MainWindow::executeQuery()
{
    QString query = ui->sqlTextEdit->toPlainText().trimmed();
    if (query.isEmpty())
    {
        QMessageBox::information( this, QApplication::applicationName(), "Query string is empty" );
        return;
    }
    //log the query
    db.logSQL(query, kLogMsg_User);
    sqlite3_stmt *vm;
    QByteArray utf8Query = query.toUtf8();
    const char *tail = utf8Query.data();
    int ncol;
    int err=0;
    QString lastErrorMessage = QString("No error");
    //Accept multi-line queries, by looping until the tail is empty
    do
    {
        queryResultListModel->removeRows(0, queryResultListModel->rowCount());
        queryResultListModel->removeColumns(0, queryResultListModel->columnCount());
        queryResultListModel->setHorizontalHeaderLabels(QStringList());
        queryResultListModel->setVerticalHeaderLabels(QStringList());

        QString queryPart = tail;
        err=sqlite3_prepare(db._db,tail,utf8Query.length(),
                            &vm, &tail);
        if (err == SQLITE_OK){
            if( !queryPart.trimmed().startsWith("SELECT", Qt::CaseInsensitive))
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
                    queryResultListModel->setItem(rownum, e, new QStandardItem(QString(firstline)));
                }
                queryResultListModel->setVerticalHeaderItem(rownum, new QStandardItem(QString::number(rownum + 1)));
                rownum++;
            }
            sqlite3_finalize(vm);
        }else{
            lastErrorMessage = QString (sqlite3_errmsg(db._db));
        }
        ui->queryErrorLineEdit->setText(lastErrorMessage);
        ui->queryResultTableView->resizeColumnsToContents();

        if(err!=SQLITE_OK) break;
    } while( tail && *tail != 0 );
}


void MainWindow::mainTabSelected(int tabindex)
{
    if(tabindex == 0)
        populateStructure();
    else if(tabindex == 1)
        resetBrowser();
}

void MainWindow::importTableFromCSV()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened. Please open or create a new database file first." );
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
            QMessageBox::information( this, QApplication::applicationName(), "Import completed" );
        }
    }
}

void MainWindow::exportTableToCSV()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened to export" );
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
                QMessageBox::information( this, QApplication::applicationName(), "Export completed" );
            }
        }
        populateStructure();
        resetBrowser();
    }
}


void MainWindow::dbState( bool dirty )
{
    ui->fileSaveAction->setEnabled(dirty);
    ui->fileRevertAction->setEnabled(dirty);
}


void MainWindow::fileSave()
{
    if (db.isOpen()){
        db.save();
        //dbStatusBar->showMessage("Date written to file", 4000)
    }
}


void MainWindow::fileRevert()
{
    if (db.isOpen()){
        QString msg = "Are you sure you want to undo all changes made to the database file \n\n<b> ";
        msg.append(db.curDBFilename);
        msg.append("</b>\n since the last save?");
        if (QMessageBox::question( this, QApplication::applicationName() ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
        {
            db.revert();
            populateStructure();
            resetBrowser();
        }
    }
}


void MainWindow::exportDatabaseToSQL()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened to export" );
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
            QMessageBox::information( this, QApplication::applicationName(), "Could not create export file" );
        } else {
            QMessageBox::information( this, QApplication::applicationName(), "Export completed" );
        }
    }
}


void MainWindow::importDatabaseFromSQL()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                "Choose a file to import",
                defaultlocation,
                "Text files (*.sql *txt)");

    if (fileName.size() > 0)
    {
        QString msg = "Do you want to create a new database file to hold the imported data?\nIf you answer NO we will attempt to import data in the .sql file to the current database.";
        if (QMessageBox::question( this, QApplication::applicationName() ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
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
                QMessageBox::information( this, QApplication::applicationName() ,err);
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
            QMessageBox::information( this, QApplication::applicationName(), QString("Error importing data at line %1").arg(lineErr) );
        }
        else
        {
            QMessageBox::information( this, QApplication::applicationName(), "Import completed" );
        }
        populateStructure();
        resetBrowser();
    }
}


void MainWindow::openPreferences()
{
    preferencesForm * prefForm = new preferencesForm( this );
    if ( prefForm->exec() ) {
        updatePreferences();
        resetBrowser();
    }
}

void MainWindow::updatePreferences()
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
void MainWindow::on_tree_context_menu(const QPoint &qPoint){
    if( !ui->dbTreeWidget->selectionModel()->hasSelection() ){
        return;
    }
    QTreeWidgetItem *cItem = ui->dbTreeWidget->currentItem();

    if(cItem->text(1) == "table"){
        ui->editDeleteTableActionPopup->setDisabled(false);
        ui->editModifyTableActionPopup->setDisabled(false);
        popupTableMenu->exec( ui->dbTreeWidget->mapToGlobal(qPoint) );

    }else if(cItem->text(1) == "field"){
        popupFieldMenu->exec( ui->dbTreeWidget->mapToGlobal(qPoint) );
    }
}
//** Tree selection changed
void MainWindow::on_tree_selection_changed(){
    if (!ui->dbTreeWidget->selectionModel()->hasSelection()){
        ui->editDeleteTableActionPopup->setEnabled(false);
        ui->editModifyTableActionPopup->setEnabled(false);
        ui->editAddFieldActionPopup->setEnabled(false);
        ui->editModifyFieldActionPopup->setEnabled(false);
        ui->editDeleteFieldActionPopup->setEnabled(false);
        return;
    }

    if(ui->dbTreeWidget->currentItem()->text(1) == "table"){
        ui->editDeleteTableActionPopup->setEnabled(true);
        ui->editModifyTableActionPopup->setEnabled(true);
        ui->editAddFieldActionPopup->setEnabled(true);
        ui->editModifyFieldActionPopup->setEnabled(false);
        ui->editDeleteFieldActionPopup->setEnabled(false);

    }else if(ui->dbTreeWidget->currentItem()->text(1) == "field"){
        ui->editAddFieldActionPopup->setEnabled(false);
        ui->editDeleteTableActionPopup->setEnabled(false);
        ui->editModifyTableActionPopup->setEnabled(false);
        ui->editModifyFieldActionPopup->setEnabled(true);
        ui->editDeleteFieldActionPopup->setEnabled(true);
    }
}


void MainWindow::on_add_field(){
    //if( !dbTreeWidget->currentItem() ){
    //   return;
    //}
    //QTreeWidgetItem *item = dbTreeWidget->currentItem();
    editFieldForm *fieldForm = new editFieldForm( this );
    fieldForm->setInitialValues(&db, true, ui->dbTreeWidget->currentItem()->text(0), "", "TEXT");
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
        QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->dbTreeWidget->currentItem());
        newItem->setText(0, fieldForm->field_name);
        newItem->setText(1, "field");
        newItem->setText(2, fieldForm->field_type);
    }
}
void MainWindow::on_edit_field(){
    if( !ui->dbTreeWidget->currentItem() ){
        return;
    }
    QTreeWidgetItem *item = ui->dbTreeWidget->currentItem();
    editFieldForm *fieldForm = new editFieldForm( this );
    qDebug(item->text(2).toUtf8());
    fieldForm->setInitialValues(&db, false, "TABLE_NAME", item->text(0), item->text(2));
    if (fieldForm->exec())
    {
        //modified = true;
        //do the sql rename here
        //qDebug(fieldForm->name + fieldForm->type);
        item->setText(0,fieldForm->field_name);
        item->setText(2,fieldForm->field_type);
    }
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        fileOpen(action->data().toString());
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings(QApplication::organizationName(), g_sApplicationNameShort);
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

void MainWindow::setCurrentFile(const QString &fileName)
{
    setWindowFilePath(fileName);
    activateFields(true);

    QSettings settings(QApplication::organizationName(), g_sApplicationNameShort);
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if( event->mimeData()->hasFormat("text/uri-list") )
            event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    if( urls.isEmpty() )
        return;

    QString fileName = urls.first().toLocalFile();

    if(!fileName.isEmpty())
            fileOpen(fileName);
}

void MainWindow::activateFields(bool enable)
{
    ui->fileCloseAction->setEnabled(enable);
    ui->fileCompactAction->setEnabled(enable);
    ui->editCreateTableAction->setEnabled(enable);
    ui->editDeleteTableAction->setEnabled(enable);
    ui->editModifyTableAction->setEnabled(enable);
    ui->editCreateIndexAction->setEnabled(enable);
    ui->editDeleteIndexAction->setEnabled(enable);
    ui->buttonNext->setEnabled(enable);
    ui->buttonPrevious->setEnabled(enable);
    ui->executeQueryButton->setEnabled(enable);
}

void MainWindow::browseTableHeaderClicked(int logicalindex)
{
    // instead of the column name we just use the column index, +2 because 'rowid, *' is the projection
    curBrowseOrderByIndex = logicalindex + 2;
    curBrowseOrderByMode = curBrowseOrderByMode == ORDERMODE_ASC ? ORDERMODE_DESC : ORDERMODE_ASC;
    populateTable(ui->comboBrowseTable->currentText(), true);

    // select the first item in the column so the header is bold
    // we might try to select the last selected item
    ui->dataTable->setCurrentCell(0, logicalindex);
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    setRecordsetLabel();
}
