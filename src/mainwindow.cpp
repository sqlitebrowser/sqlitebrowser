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
#include <QUrl>

#include "ui_mainwindow.h"
#include "createtableform.h"
#include "createindexform.h"
#include "deletetableform.h"
#include "deleteindexform.h"
#include "dialogabout.h"
#include "choosetableform.h"
#include "edittableform.h"
#include "editfieldform.h"
#include "importcsvform.h"
#include "exporttablecsvform.h"
#include "preferencesform.h"
#include "editform.h"
#include "findform.h"
#include "SQLLogDock.h"
#include "sqlitesyntaxhighlighter.h"

/*
 *  Constructs a mainForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUi();
    setAcceptDrops(true);

    m_ToolButtonWAL = new QToolButton(statusBar());
    m_ToolButtonWAL->setDisabled(true);
    m_ToolButtonWAL->setToolTip(QObject::tr("Journal mode"));
    m_ToolButtonWAL->setText("");
    statusBar()->addPermanentWidget(m_ToolButtonWAL);
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
MainWindow::~MainWindow()
{
    destroy();
    delete ui;
}

void MainWindow::setupUi()
{
    logWin = new SQLLogDock(this);
    sqlLogAction = logWin->toggleViewAction();
    sqlLogAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    this->addDockWidget(Qt::BottomDockWidgetArea, logWin);

    // Recent dbs
    for(int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        this->connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    sqlLogAction->setText(QObject::tr("SQL &Log"));
    sqlLogAction->setIcon(QIcon(":/oldimages/log"));
    sqlLogAction->setIconVisibleInMenu(true);

    // structure tab
    // this tab needs manual love, because toolbars are normaly not in child widgets

    QToolBar *dbToolbar = new QToolBar();
    ui->structureTab->layout()->addWidget(dbToolbar);
    dbToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    dbToolbar->addAction(ui->actionCreateTable);
    dbToolbar->addAction(ui->actionModify_Table);
    dbToolbar->addAction(ui->actionDelete_Table);
    dbToolbar->addSeparator();
    dbToolbar->addAction(ui->actionCreate_Field);
    dbToolbar->addAction(ui->actionModifyField);
    dbToolbar->addAction(ui->actionDelete_Field);

    dbTreeWidget = new QTreeWidget();
    ui->structureTab->layout()->addWidget(dbTreeWidget);
    QTreeWidgetItem *headerItem = dbTreeWidget->headerItem();
    headerItem->setText(0, QApplication::translate("mainForm", "Name", 0, QApplication::UnicodeUTF8));
    headerItem->setText(1, QApplication::translate("mainForm", "Object", 0, QApplication::UnicodeUTF8));
    headerItem->setText(2, QApplication::translate("mainForm", "Type", 0, QApplication::UnicodeUTF8));
    headerItem->setText(3, QApplication::translate("mainForm", "Schema", 0, QApplication::UnicodeUTF8));
    dbTreeWidget->setColumnHidden(1, true);
    dbTreeWidget->setColumnWidth(0, 300);
    dbTreeWidget->setAlternatingRowColors(true);
    dbTreeWidget->setRootIsDecorated(true);
    dbTreeWidget->setAnimated(true);
    dbTreeWidget->setContextMenuPolicy( Qt::CustomContextMenu );

    ui->dataTable->setAcceptDrops(true);
    ui->dataTable->setRowCount(0);
    ui->dataTable->setColumnCount(0);
    ui->dataTable->setSelectionMode(QTableWidget::SingleSelection);
    ui->dataTable->setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );

    sqliteHighlighter = new SQLiteSyntaxHighlighter(ui->textEditSQLexpression->document());

    queryResultListModel = new QStandardItemModel(ui->queryResultTableView);

    ui->queryResultTableView->setSelectionMode(QTreeView::NoSelection);
    ui->queryResultTableView->setModel(queryResultListModel);
    ui->queryResultTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->queryResultTableView->setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );

    recentSeparatorAct = ui->menu_File->insertSeparator(ui->action_Quit);
    for(int i = 0; i < MaxRecentFiles; ++i)
        ui->menu_File->insertAction(recentSeparatorAct, recentFileActs[i]);

    ui->menu_View->addAction(sqlLogAction);
    QAction* toggleDBToolbar = ui->toolBarDB->toggleViewAction();
    toggleDBToolbar->setText(QObject::tr("DB Toolbar"));
    ui->menu_View->addAction(toggleDBToolbar);

    //***********************************************8
    //** Db Tree Popup Menus
    popupTableMenu = new QMenu(this);
    popupTableMenu->addAction(ui->actionModify_Table);
    popupTableMenu->addAction(ui->actionCreateTable);
    popupTableMenu->addSeparator();
    popupTableMenu->addAction(ui->actionDelete_Table);

    popupFieldMenu = new QMenu(this);
    popupFieldMenu->addAction(ui->actionModifyField);
    popupFieldMenu->addAction(ui->actionDelete_Field);


    QObject::connect(ui->action_Quit, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(ui->action_Open_Database, SIGNAL(activated()), this, SLOT(fileOpen()));
    QObject::connect(ui->action_New_Database, SIGNAL(activated()), this, SLOT(fileNew()));
    QObject::connect(ui->action_Close_Database, SIGNAL(activated()), this, SLOT(fileClose()));
    QObject::connect(ui->comboBrowseTable, SIGNAL(activated(QString)), this, SLOT(populateTable(QString)));
    QObject::connect(ui->pushButtonNewRecord, SIGNAL(clicked()), this, SLOT(addRecord()));
    QObject::connect(ui->pushButtonDeleteRecord, SIGNAL(clicked()), this, SLOT(deleteRecord()));
    QObject::connect(ui->pushButtonPrevious, SIGNAL(clicked()), this, SLOT(navigatePrevious()));
    QObject::connect(ui->pushButtonNext, SIGNAL(clicked()), this, SLOT(navigateNext()));
    QObject::connect(ui->lineEditGoto, SIGNAL(returnPressed()), this, SLOT(navigateGoto()));
    QObject::connect(ui->pushButtonGoto, SIGNAL(clicked()), this, SLOT(navigateGoto()));
    QObject::connect(ui->pushButtonFind, SIGNAL(toggled(bool)), this, SLOT(browseFind(bool)));
    QObject::connect(ui->pushButtonRefresh, SIGNAL(clicked()), this, SLOT(browseRefresh()));
    QObject::connect(ui->actionCom_pact_Database, SIGNAL(activated()), this, SLOT(compact()));
//    QObject::connect(editCopyAction, SIGNAL(activated()), this, SLOT(copy()));
//    QObject::connect(editPasteAction, SIGNAL(activated()), this, SLOT(paste()));
    QObject::connect(ui->actionWhats_this, SIGNAL(activated()), this, SLOT(helpWhatsThis()));
    QObject::connect(ui->action_About, SIGNAL(activated()), this, SLOT(helpAbout()));
    QObject::connect(ui->dataTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(doubleClickTable(int,int)));
    QObject::connect(ui->dataTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(browseTableHeaderClicked(int)));
    QObject::connect(ui->mainTabContainer, SIGNAL(currentChanged(int)), this, SLOT(mainTabSelected(int)));
    QObject::connect(ui->executeQueryButton, SIGNAL(clicked()), this, SLOT(executeQuery()));
    QObject::connect(ui->actionFrom_CSV_File, SIGNAL(activated()), this, SLOT(importTableFromCSV()));
    QObject::connect(ui->actionTable_t_o_CSV_File, SIGNAL(activated()), this, SLOT(exportTableToCSV()));
    QObject::connect(ui->action_Revert_Changes, SIGNAL(activated()), this, SLOT(fileRevert()));
    QObject::connect(ui->action_Write_Changes, SIGNAL(activated()), this, SLOT(fileSave()));
    QObject::connect(ui->actionDelete_Index, SIGNAL(activated()), this, SLOT(deleteIndex()));
    QObject::connect(ui->actionCreate_Index, SIGNAL(activated()), this, SLOT(createIndex()));
    QObject::connect(ui->actionCreateTable, SIGNAL(activated()), this, SLOT(createTable()));
    QObject::connect(ui->actionDelete_Table, SIGNAL(activated()), this, SLOT(deleteTable()));
    QObject::connect(ui->actionModify_Table, SIGNAL(activated()), this, SLOT(editTable()));

    QObject::connect(ui->actionDelete_Table, SIGNAL(activated()), this, SLOT(deleteTablePopup()));
    QObject::connect(ui->actionModify_Table, SIGNAL(activated()), this, SLOT(editTablePopup()));
    QObject::connect(ui->actionCreate_Field, SIGNAL(activated()), this, SLOT(on_add_field()));
    QObject::connect(ui->actionModifyField, SIGNAL(activated()), this, SLOT(on_edit_field()));

    QObject::connect(ui->actionDatabase_to_SQL_File, SIGNAL(activated()), this, SLOT(exportDatabaseToSQL()));
    QObject::connect(ui->actionFrom_SQL_File, SIGNAL(activated()), this, SLOT(importDatabaseFromSQL()));
    QObject::connect(ui->action_Preference, SIGNAL(activated()), this, SLOT(openPreferences()));

    QObject::connect(dbTreeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_tree_context_menu(const QPoint &)));
    QObject::connect(dbTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(on_tree_selection_changed()));
} // setupUi

void MainWindow::init()
{
    findWin = 0;
    editWin = 0;

    clipboard = QApplication::clipboard();

    recsPerView = 1000;
    recAtTop = 0;
    gotoValidator = new QIntValidator(0, 0, this);
    ui->lineEditGoto->setValidator(gotoValidator);
    gotoValidator->setRange ( 0, 0);
    resetBrowser();
    this->setWindowTitle(QApplication::applicationName());
    this->setWindowIcon( QPixmap( g_applicationIconName ) );
    ui->pushButtonNext->setEnabled(false);
    ui->pushButtonPrevious->setEnabled(false);
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
        updateDBInfo();
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
    dbTreeWidget->model()->removeRows(0, dbTreeWidget->model()->rowCount());
    if (!db.isOpen()){
        return;
    }
    db.updateSchema();
    QStringList tblnames = db.getTableNames();
    sqliteHighlighter->setTableNames(tblnames);
//    logWin->userSqliteHighlighter()->setTableNames(tblnames);
//    logWin->appSqliteHighlighter()->setTableNames(tblnames);
    tableMap::ConstIterator it;

    for ( it = db.tbmap.begin(); it != db.tbmap.end(); ++it ) {

        //* Table node
        QTreeWidgetItem *tableItem = new QTreeWidgetItem();
        tableItem->setText(0, it.value().getname());
        tableItem->setText(1,  "table");
        tableItem->setText(3, it.value().getsql());
        tableItem->setIcon(0, QIcon(":/icons/table"));
        dbTreeWidget->addTopLevelItem(tableItem);

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
        dbTreeWidget->setItemExpanded(tableItem, true);
    }
    indexMap::ConstIterator it2;
    for ( it2 = db.idxmap.begin(); it2 != db.idxmap.end(); ++it2 ) {
        QTreeWidgetItem *idxItem = new QTreeWidgetItem();
        idxItem->setText( 0, it2.value().getname() );
        idxItem->setText( 1, "index"  );
        idxItem->setText( 3, it2.value().getsql() );
        idxItem->setIcon(0, QIcon(":/icons/index"));
        dbTreeWidget->addTopLevelItem(idxItem);
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
        recAtTop = 0;
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

void MainWindow::updateDBInfo()
{
    if(db.isOpen())
    {
        JournalMode::journalmode mode = db.journalMode();
        if( mode != JournalMode::journalmode_wal )
        {
            m_ToolButtonWAL->setEnabled(true);
        }
        m_ToolButtonWAL->setText(JournalMode::toString(db.journalMode()));
    }
    else
    {
        m_ToolButtonWAL->setEnabled(false);
    }
}

void MainWindow::resetBrowser()
{
    recAtTop = 0;
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
        recAtTop = ((db.getRecordCount()-1)/recsPerView)*recsPerView;
        updateTableView(db.getRecordCount()-recAtTop-1);
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
        db.deleteRecord(ui->dataTable->currentRow()+recAtTop);
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
    //  qDebug("line to select value is %d, rowAttop = %d",lineToSelect, recAtTop);
    QApplication::setOverrideCursor( Qt::WaitCursor );

    ui->dataTable->setRowCount(0);
    ui->dataTable->setColumnCount( db.browseFields.count() );
    ui->dataTable->setHorizontalHeaderLabels(db.browseFields);

    rowList tab = db.browseRecs;
    int maxRecs = db.getRecordCount();
    int recsThisView = maxRecs - recAtTop;

    if (recsThisView>recsPerView)
        recsThisView = recsPerView;

    // qDebug("recsthisview= %d\n",recsThisView);

    ui->dataTable->setRowCount(recsThisView);

    if ( recsThisView > 0 ) {

        int rowNum = 0;
        int colNum = 0;
        //int dcols =0;
        QString rowLabel;
        for (int i = recAtTop; i < tab.size(); ++i)
            //for ( int  = tab.at(recAtTop); rt !=tab.end(); ++rt )
        {
            rowLabel.setNum(recAtTop+rowNum+1);
            ui->dataTable->setVerticalHeaderItem(rowNum, new QTableWidgetItem( rowLabel ));
            colNum = 0;
            QStringList& rt = tab[i];
            for (int e = 1; e < rt.size(); ++e)
                //for ( QStringList::Iterator it = (*rt).begin(); it != (*rt).end(); ++it )
            {
                //skip first one (the rowid)
                //  if (it!=(*rt).begin()){
                QString& content = rt[e];

                QTableWidgetItem* item = new QTableWidgetItem(content);
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                item->setToolTip(wrapText(content));
                ui->dataTable->setItem( rowNum, colNum, item);
                colNum++;
                //}
            }
            rowNum++;
            if (rowNum==recsThisView) break;
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
    //ui->dataTable->clearSelection(true);
    if (lineToSelect!=-1){
        //qDebug("inside selection");
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
    int nextFirstRec = recAtTop - recsPerView;
    if (nextFirstRec  >= 0 ) {
        recAtTop = nextFirstRec;
        updateTableView(-1);
    }
}


void MainWindow::navigateNext()
{
    int nextFirstRec = recAtTop + recsPerView;
    //qDebug("called navigatenext, nextFirstRec=%d\n",nextFirstRec);
    if (nextFirstRec < db.getRecordCount() ) {
        recAtTop = nextFirstRec;
        updateTableView(-1);
    }
}


void MainWindow::navigateGoto()
{
    QString typed = ui->lineEditGoto->text();
    bool ok;
    int dec = typed.toInt( &ok);
    if (dec==0) dec=1;
    if (dec>db.getRecordCount()) dec = db.getRecordCount();

    recAtTop = ((dec-1)/recsPerView)*recsPerView;
    updateTableView(dec-recAtTop-1);
    ui->lineEditGoto->setText(QString::number(dec,10));
}

void MainWindow::setRecordsetLabel()
{
    if (db.getRecordCount()==0){
        ui->labelRecordset->setText("0 - 0 of 0");
    } else {
        QString label = QString::number(recAtTop+1,10);
        label.append(" - ");
        int lastrec = db.getRecordCount();
        int lastthisview = recAtTop+recsPerView;
        if (lastthisview > lastrec) lastthisview = lastrec;
        label.append(QString::number(lastthisview,10));
        label.append(" of ");
        label.append(QString::number(db.getRecordCount(),10));
        ui->labelRecordset->setText(label);
    }
    gotoValidator->setRange ( 0, db.getRecordCount());

    if (db.getRecordCount()>1000){
        if (recAtTop>=1000) {
            ui->pushButtonPrevious->setEnabled(TRUE);
        } else {
            ui->pushButtonPrevious->setEnabled(FALSE);
        }
        if (db.getRecordCount()>=(recAtTop+1000)) {
            ui->pushButtonNext->setEnabled(TRUE);
        } else {
            ui->pushButtonNext->setEnabled(FALSE);
        }
    } else {
        ui->pushButtonNext->setEnabled(FALSE);
        ui->pushButtonPrevious->setEnabled(FALSE);
    }
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
    ui->pushButtonFind->toggle();
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
    recAtTop = ((dec-1)/recsPerView)*recsPerView;
    updateTableView(dec-recAtTop-1);
}

void MainWindow::createTable()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened. Please open or create a new database file." );
        return;
    }
    createTableForm * tableForm = new createTableForm( this );
    tableForm->setModal(true);
    if ( tableForm->exec() ) {
        if (!db.executeSQL(tableForm->createStatement)){
            QString error = "Error: could not create the table. Message from database engine:  ";
            error.append(db.lastErrorMessage);
            QMessageBox::warning( this, QApplication::applicationName(), error );
        } else {
            populateStructure();
            resetBrowser();
        }
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
    ui->action_Close_Database->setEnabled(true);
    ui->actionCom_pact_Database->setEnabled(true);
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
    //TODO ask if user really wants to delte this table
}

void MainWindow::editTablePopup()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened." );
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
    QWidget * t =ui->dataTable->cellWidget(ui->dataTable->currentRow(), ui->dataTable->currentColumn());
    if (t!=0){
        if (QString(t->metaObject()->className()) == "QLineEdit"){
            /*we are in edit mode*/
            if (t->hasFocus()){
                QLineEdit * le = (QLineEdit *) t;
                le->copy();
            }
        }
    }
    if (ui->lineEditGoto->hasFocus())
        ui->lineEditGoto->copy();
}


void MainWindow::paste()
{
    QWidget * t =ui->dataTable->cellWidget(ui->dataTable->currentRow(), ui->dataTable->currentColumn());
    if (t!=0){
        if ( QString(t->metaObject()->className()) == "QLineEdit" ){
            /*we are in edit mode*/
            if (t->hasFocus()){
                QLineEdit * le = (QLineEdit *) t;
                le->paste();}
        }
    }
    if (ui->lineEditGoto->hasFocus())
        ui->lineEditGoto->paste();
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
    ui->dataTable->setItem( row - recAtTop, col, item);

}

void MainWindow::logWinAway()
{
    sqlLogAction->toggle();
}

void MainWindow::editWinAway()
{
    editWin->hide();
    activateWindow();
    ui->dataTable->setRangeSelected( QTableWidgetSelectionRange(editWin->curRow - recAtTop, editWin->curCol, editWin->curRow- recAtTop, editWin->curCol), true);
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

    int realRow = row + recAtTop;

    editText(realRow , col);
}


void MainWindow::executeQuery()
{
    QString query = ui->textEditSQLexpression->toPlainText().trimmed();
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
    if ((tabindex == 0)||(tabindex == 1))
    {
        populateStructure();
        resetBrowser();
    }
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
    ui->action_Write_Changes->setEnabled(dirty);
    ui->action_Revert_Changes->setEnabled(dirty);
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
    if( !dbTreeWidget->selectionModel()->hasSelection() ){
        return;
    }
    QTreeWidgetItem *cItem = dbTreeWidget->currentItem();

    if(cItem->text(1) == "table"){
        ui->actionDelete_Table->setDisabled(false);
        ui->actionModify_Table->setDisabled(false);
        popupTableMenu->exec( dbTreeWidget->mapToGlobal(qPoint) );
    }else if(cItem->text(1) == "field"){
        popupFieldMenu->exec( dbTreeWidget->mapToGlobal(qPoint) );
    }
}
//** Tree selection changed
void MainWindow::on_tree_selection_changed(){
    if (!dbTreeWidget->selectionModel()->hasSelection()){
        ui->actionDelete_Table->setEnabled(false);
        ui->actionModify_Table->setEnabled(false);
        ui->actionCreate_Field->setEnabled(false);
        ui->actionModifyField->setEnabled(false);
        ui->actionDelete_Field->setEnabled(false);
        return;
    }

    if(dbTreeWidget->currentItem()->text(1) == "table"){
        ui->actionDelete_Table->setEnabled(true);
        ui->actionModify_Table->setEnabled(true);
        ui->actionCreate_Field->setEnabled(true);
        ui->actionModifyField->setEnabled(false);
        ui->actionDelete_Field->setEnabled(false);
    }else if(dbTreeWidget->currentItem()->text(1) == "field"){
        ui->actionDelete_Table->setEnabled(false);
        ui->actionModify_Table->setEnabled(false);
        ui->actionCreate_Field->setEnabled(false);
        ui->actionModifyField->setEnabled(true);
        ui->actionDelete_Field->setEnabled(true);
    }
}


void MainWindow::on_add_field(){
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
void MainWindow::on_edit_field(){
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

    if( !fileName.isEmpty() && fileName.endsWith("db") )
            fileOpen(fileName);
}

void MainWindow::activateFields(bool enable)
{
    ui->action_Close_Database->setEnabled(enable);
    ui->actionCom_pact_Database->setEnabled(enable);
    ui->actionCreateTable->setEnabled(enable);
    ui->actionDelete_Table->setEnabled(enable);
    ui->actionModify_Table->setEnabled(enable);
    ui->actionCreate_Index->setEnabled(enable);
    ui->actionDelete_Index->setEnabled(enable);

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
