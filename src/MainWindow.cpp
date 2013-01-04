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
#include "DialogAbout.h"
#include "EditTableForm.h"
#include "EditFieldForm.h"
#include "ImportCSVForm.h"
#include "ExportTableCSVForm.h"
#include "PreferencesForm.h"
#include "EditForm.h"
#include "FindForm.h"
#include "SQLLogDock.h"
#include "SQLiteSyntaxHighlighter.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      editWin(new editForm(this)),
      clipboard(QApplication::clipboard()),
      findWin(0)
{
    ui->setupUi(this);
    init();

    activateFields(false);
    updatePreferences();
    resetBrowser();
    updateRecentFileActions();
}

/*
 *  Destroys the object and frees any allocated resources
 */
MainWindow::~MainWindow()
{
    delete gotoValidator;
    delete ui;
}

void MainWindow::init()
{
    // Create the SQL log dock
    logWin = new SQLLogDock(this);
    db.logWin = logWin;
    addDockWidget(Qt::BottomDockWidgetArea, logWin);

    // Set up the db tree widget
    ui->dbTreeWidget->setColumnHidden(1, true);
    ui->dbTreeWidget->setColumnWidth(0, 300);

    // Create the validator for the goto line edit
    gotoValidator = new QIntValidator(0, 0, this);
    ui->editGoto->setValidator(gotoValidator);

    // Create the SQL sytax highlighter
    sqliteHighlighter = new SQLiteSyntaxHighlighter(ui->sqlTextEdit->document());

    // Set up DB model
    queryResultListModel = new QStandardItemModel(this);
    ui->queryResultTableView->setModel(queryResultListModel);

    // Create the actions for the recently opened dbs list
    for(int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
    for(int i = 0; i < MaxRecentFiles; ++i)
        ui->fileMenu->insertAction(ui->fileExitAction, recentFileActs[i]);
    recentSeparatorAct = ui->fileMenu->insertSeparator(ui->fileExitAction);

    // Create popup menus
    popupTableMenu = new QMenu(this);
    popupTableMenu->addAction(ui->editModifyTableAction);
    popupTableMenu->addAction(ui->editAddFieldActionPopup);
    popupTableMenu->addSeparator();
    popupTableMenu->addAction(ui->editDeleteObjectAction);
    popupFieldMenu = new QMenu(this);
    popupFieldMenu->addAction(ui->editModifyFieldActionPopup);
    popupFieldMenu->addAction(ui->editDeleteFieldActionPopup);

    // Set state of checkable menu actions
    ui->sqlLogAction->setChecked(!logWin->isHidden());
    ui->viewDBToolbarAction->setChecked(!ui->toolbarDB->isHidden());

    // Connect some more signals and slots
    connect(ui->dataTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(browseTableHeaderClicked(int)));
    connect(ui->sqlLogAction, SIGNAL(toggled(bool)), logWin, SLOT(setVisible(bool)));
    connect(ui->dataTable->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setRecordsetLabel()));
    connect(editWin, SIGNAL(goingAway()), this, SLOT(editWinAway()));
    connect(editWin, SIGNAL(updateRecordText(int, int , QString)), this, SLOT(updateRecordText(int, int , QString)));
    connect(logWin, SIGNAL(goingAway()),  this, SLOT(logWinAway()));
    connect(logWin, SIGNAL(dbState(bool)),this, SLOT(dbState(bool)));

    // Load window settings
    QSettings settings(QApplication::organizationName(), g_sApplicationNameShort);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());
    logWin->comboLogType()->setCurrentIndex(logWin->comboLogType()->findText(settings.value("SQLLogDock/Log", "Application").toString()));

    // Set other window settings
    setAcceptDrops(true);
    setWindowTitle(QApplication::applicationName());
    setWindowIcon(QPixmap(g_applicationIconName));
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
            setCurrentFile(wFile);
        } else {
            QString err = "An error occurred:  ";
            err.append(db.lastErrorMessage);
            QMessageBox::warning(this, QApplication::applicationName(), err);
        }
        populateStructure();
        resetBrowser();
        if(ui->mainTab->currentIndex() == 2)
            loadPragmas();
    }
}

void MainWindow::fileOpen()
{
    fileOpen(QString());
}

void MainWindow::fileNew()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Choose a filename to save under", defaultlocation);
    if(!fileName.isEmpty())
    {
        if(QFile::exists(fileName))
            QFile::remove(fileName);
        db.create(fileName);
        setCurrentFile(fileName);
        populateStructure();
        resetBrowser();
        createTable();
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
    QStringList tblnames = db.getBrowsableObjectNames();
    sqliteHighlighter->setTableNames(tblnames);
    objectMap::ConstIterator it;

    for ( it = db.objMap.begin(); it != db.objMap.end(); ++it )
    {
        // Object node
        QTreeWidgetItem *tableItem = new QTreeWidgetItem();
        tableItem->setIcon(0, QIcon(QString(":/icons/%1").arg((*it).gettype())));
        tableItem->setText(0, (*it).getname());
        tableItem->setText(1, (*it).gettype());
        tableItem->setText(3, (*it).getsql());
        ui->dbTreeWidget->addTopLevelItem(tableItem);

        // If it is a table add the field Nodes
        if((*it).gettype() == "table" || (*it).gettype() == "view")
        {
            fieldMap::ConstIterator fit;
            for ( fit = (*it).fldmap.begin(); fit != (*it).fldmap.end(); ++fit ) {
                QTreeWidgetItem *fldItem = new QTreeWidgetItem(tableItem);
                fldItem->setText( 0, fit.value().getname() );
                fldItem->setText( 1, "field"  );
                fldItem->setText( 2, fit.value().gettype() );
                fldItem->setIcon(0, QIcon(":/icons/field"));
            }
            // TODO make an options/setting autoexpand
            ui->dbTreeWidget->setItemExpanded(tableItem, true);
        }
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

    // Activate the add and delete record buttons and editing only if a table has been selected
    bool is_table = db.getObjectByName(tablename).gettype() == "table";
    ui->buttonNewRecord->setEnabled(is_table);
    ui->buttonDeleteRecord->setEnabled(is_table);
    ui->dataTable->setEditTriggers(is_table ? QAbstractItemView::DoubleClicked | QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed : QAbstractItemView::NoEditTriggers);

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
    objectMap tab = db.getBrowsableObjects();
    for(objectMap::ConstIterator i=tab.begin();i!=tab.end();++i)
    {
        ui->comboBrowseTable->addItem(QIcon(QString(":icons/%1").arg(i.value().gettype())), i.value().getname());

        //ui->comboBrowseTable->addItems(tab);
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
    setWindowTitle(QApplication::applicationName());
    resetBrowser();
    populateStructure();
    loadPragmas();
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

    editTableForm dialog(this);
    dialog.setActiveTable(&db, "");
    if(dialog.exec())
    {
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
    createIndexForm dialog(this);
    dialog.populateTable(db.objMap.values("table"));
    if(dialog.exec())
    {
        if (!db.executeSQL(dialog.createStatement)){
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
    if (!db.compact()){
        QString error = "Error: could not compact the database file. Message from database engine:  ";
        error.append(db.lastErrorMessage);
        QApplication::restoreOverrideCursor( );
        QMessageBox::warning( this, QApplication::applicationName(), error );
    } else {
        QApplication::restoreOverrideCursor( );
        QMessageBox::information(this, QApplication::applicationName(), "Database successfully compacted.");
    }
    db.open(db.curDBFilename);
    populateStructure();
    resetBrowser();
}

void MainWindow::deleteObject()
{
    // Get name of table to delete
    QString table = ui->dbTreeWidget->currentItem()->text(0);
    QString type = ui->dbTreeWidget->currentItem()->text(1);

    // Ask user if he really wants to delete that table
    if(QMessageBox::warning(this, QApplication::applicationName(), tr("Are you sure you want to delete the %1 '%2'?\nAll data associated with the %1 will be lost.").arg(type).arg(table),
                            QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape) == QMessageBox::Yes)
    {
        // Delete the table
        QString statement = QString("DROP %1 %2;").arg(type.toUpper()).arg(table);
        if(!db.executeSQL( statement))
        {
            QString error = QString("Error: could not delete the %1. Message from database engine:\n%2").arg(type).arg(db.lastErrorMessage);
            QMessageBox::warning(this, QApplication::applicationName(), error);
        } else {
            populateStructure();
            resetBrowser();
        }
    }
}

void MainWindow::editTable()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), "There is no database opened." );
        return;
    }
    if(!ui->dbTreeWidget->selectionModel()->hasSelection()){
        return;
    }
    QString tableToEdit = ui->dbTreeWidget->currentItem()->text(0);

    editTableForm dialog(this);
    dialog.setActiveTable(&db, tableToEdit);
    dialog.exec();
    //check modified status
    if(dialog.modified)
    {
        populateStructure();
        resetBrowser();
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
    DialogAbout dialog(this);
    dialog.exec();
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

    // Don't allow editing of other objects than tables
    if(db.getObjectByName(ui->comboBrowseTable->currentText()).gettype() != "table")
        return;

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
    else if(tabindex == 2)
        loadPragmas();
}

void MainWindow::importTableFromCSV()
{
    QString wFile = QFileDialog::getOpenFileName(
                this,
                "Choose a text file",
                defaultlocation,
                "Text files(*.csv *.txt);;All files(*)");

    if (QFile::exists(wFile) )
    {
        importCSVForm dialog(this);
        dialog.initialize(wFile, &db);
        if(dialog.exec())
        {
            populateStructure();
            resetBrowser();
            QMessageBox::information( this, QApplication::applicationName(), "Import completed" );
        }
    }
}

void MainWindow::exportTableToCSV()
{
    exportTableCSVForm dialog(this);
    dialog.populateOptions(db.getBrowsableObjectNames());
    if(dialog.exec())
    {
        //load our table
        db.browseTable(dialog.option);

        QString fileName = QFileDialog::getSaveFileName(
                    this,
                    "Choose a filename to export data",
                    defaultlocation,
                    "Text files(*.csv *.txt)");

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
        QString msg = QString("Are you sure you want to undo all changes made to the database file '%1' since the last save?").arg(db.curDBFilename);
        if(QMessageBox::question(this, QApplication::applicationName(), msg, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape) == QMessageBox::Yes)
        {
            db.revert();
            populateStructure();
            resetBrowser();
        }
    }
}

void MainWindow::exportDatabaseToSQL()
{
    QString fileName = QFileDialog::getSaveFileName(
                this,
                "Choose a filename to export",
                defaultlocation,
                "Text files(*.sql *.txt)");

    if (fileName.size() > 0)
    {
        if (!db.dump(fileName))
        {
            QMessageBox::warning( this, QApplication::applicationName(), "Could not create export file." );
        } else {
            QMessageBox::information( this, QApplication::applicationName(), "Export completed." );
        }
    }
}

void MainWindow::importDatabaseFromSQL()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                "Choose a file to import",
                defaultlocation,
                "Text files(*.sql *.txt);;All files(*)");

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
    preferencesForm dialog(this);
    if(dialog.exec())
    {
        updatePreferences();
        resetBrowser();
    }
}

void MainWindow::updatePreferences()
{
    preferencesForm prefs(this);

    if(prefs.defaultencoding == "Latin1")
        db.setEncoding(kEncodingLatin1);
    else
        db.setEncoding(kEncodingUTF8);

    db.setDefaultNewData(prefs.defaultnewdata);
    defaultlocation= prefs.defaultlocation;
    editWin->defaultlocation = defaultlocation;
    editWin->setTextFormat(prefs.defaulttext);
}

//******************************************************************
//** Tree Events
//******************************************************************

//** Db Tree Context Menu
void MainWindow::on_tree_context_menu(const QPoint &qPoint)
{
    if(!ui->dbTreeWidget->selectionModel()->hasSelection())
        return;

    QTreeWidgetItem *cItem = ui->dbTreeWidget->currentItem();

    if(cItem->text(1) == "table" || cItem->text(1) == "view" || cItem->text(1) == "trigger" || cItem->text(1) == "index")
        popupTableMenu->exec(ui->dbTreeWidget->mapToGlobal(qPoint));
    else if(cItem->text(1) == "field")
        popupFieldMenu->exec(ui->dbTreeWidget->mapToGlobal(qPoint));
}
//** Tree selection changed
void MainWindow::on_tree_selection_changed()
{
    // Just assume first that something's selected that can not be edited at all
    ui->editDeleteObjectAction->setEnabled(false);
    ui->editModifyTableAction->setEnabled(false);
    ui->editAddFieldActionPopup->setEnabled(false);
    ui->editModifyFieldActionPopup->setEnabled(false);
    ui->editDeleteFieldActionPopup->setEnabled(false);

    if(ui->dbTreeWidget->currentItem() == 0)
        return;

    // Change the text of the actions
    ui->editDeleteObjectAction->setIcon(QIcon(QString(":icons/%1_delete").arg(ui->dbTreeWidget->currentItem()->text(1))));
    if(ui->dbTreeWidget->currentItem()->text(1) == "view")
        ui->editDeleteObjectAction->setText(tr("Delete View"));
    else if(ui->dbTreeWidget->currentItem()->text(1) == "trigger")
        ui->editDeleteObjectAction->setText(tr("Delete Trigger"));
    else if(ui->dbTreeWidget->currentItem()->text(1) == "index")
        ui->editDeleteObjectAction->setText(tr("Delete Index"));
    else
        ui->editDeleteObjectAction->setText(tr("Delete Table"));

    // Activate actions
    if(ui->dbTreeWidget->currentItem()->text(1) == "table")
    {
        ui->editDeleteObjectAction->setEnabled(true);
        ui->editModifyTableAction->setEnabled(true);
        ui->editAddFieldActionPopup->setEnabled(true);
    } else if(ui->dbTreeWidget->currentItem()->text(1) == "field" && ui->dbTreeWidget->currentItem()->parent()->text(1) == "table") {
        ui->editModifyFieldActionPopup->setEnabled(true);
        ui->editDeleteFieldActionPopup->setEnabled(true);
    } else if(ui->dbTreeWidget->currentItem()->text(1) == "view" || ui->dbTreeWidget->currentItem()->text(1) == "trigger" || ui->dbTreeWidget->currentItem()->text(1) == "index") {
        ui->editDeleteObjectAction->setEnabled(true);
    }
}

void MainWindow::on_add_field(){
    //if( !dbTreeWidget->currentItem() ){
    //   return;
    //}

    editFieldForm dialog(this);
    dialog.setInitialValues(&db, true, ui->dbTreeWidget->currentItem()->text(0), "", "TEXT");
    if(dialog.exec())
        populateStructure();
}

void MainWindow::on_edit_field(){
    if(!ui->dbTreeWidget->currentItem())
        return;

    // TODO
    QMessageBox::information(this, QApplication::applicationName(), tr("Sorry! This function is currently not implemented as SQLite does not support editing columns yet."));
    /*QTreeWidgetItem *item = ui->dbTreeWidget->currentItem();
    editFieldForm dialog(this);
    dialog.setInitialValues(&db, false, item->parent()->text(0), item->text(0), item->text(2));
    if(dialog.exec())
    {
        item->setText(0, dialog.field_name);
        item->setText(2, dialog.field_type);
    }*/
}

void MainWindow::on_delete_field(){
    if(!ui->dbTreeWidget->currentItem())
        return;

    // TODO
    QMessageBox::information(this, QApplication::applicationName(), tr("Sorry! This function is currently not implemented as SQLite does not support the deletion of columns yet."));
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
    setWindowTitle( QApplication::applicationName() +" - "+fileName);
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
    ui->fileExportCSVAction->setEnabled(enable);
    ui->fileExportSQLAction->setEnabled(enable);
    ui->fileImportCSVAction->setEnabled(enable);
    ui->editCreateTableAction->setEnabled(enable);
    ui->editCreateIndexAction->setEnabled(enable);
    ui->buttonNext->setEnabled(enable);
    ui->buttonPrevious->setEnabled(enable);
    ui->executeQueryButton->setEnabled(enable);
    ui->scrollAreaWidgetContents->setEnabled(enable);
    ui->buttonGoto->setEnabled(enable);
    ui->editGoto->setEnabled(enable);
    ui->buttonRefresh->setEnabled(enable);
    ui->buttonDeleteRecord->setEnabled(enable);
    ui->buttonNewRecord->setEnabled(enable);
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

void MainWindow::loadPragmas()
{
    ui->comboboxPragmaAutoVacuum->setCurrentIndex(db.getPragma("auto_vacuum").toInt());
    ui->checkboxPragmaAutomaticIndex->setChecked(db.getPragma("automatic_index").toInt());
    ui->spinPragmaBusyTimeout->setValue(db.getPragma("busy_timeout").toInt());
    ui->spinPragmaCacheSize->setValue(db.getPragma("cache_size").toInt());
    ui->checkboxPragmaCheckpointFullFsync->setChecked(db.getPragma("checkpoint_fullfsync").toInt());
    ui->comboboxPragmaEncoding->setCurrentIndex(db.getPragma("encoding").toInt());
    ui->checkboxPragmaForeignKeys->setChecked(db.getPragma("foreign_keys").toInt());
    ui->checkboxPragmaFullFsync->setChecked(db.getPragma("fullfsync").toInt());
    ui->checkboxPragmaIgnoreCheckConstraints->setChecked(db.getPragma("ignore_check_constraints").toInt());
    ui->comboboxPragmaJournalMode->setCurrentIndex(db.getPragma("journal_mode").toInt());
    ui->spinPragmaJournalSizeLimit->setValue(db.getPragma("journal_size_limit").toInt());
    ui->checkboxPragmaLegacyFileFormat->setChecked(db.getPragma("legacy_file_format").toInt());
    ui->comboboxPragmaLockingMode->setCurrentIndex(db.getPragma("locking_mode").toInt());
    ui->spinPragmaMaxPageCount->setValue(db.getPragma("max_page_count").toInt());
    ui->spinPragmaPageSize->setValue(db.getPragma("page_size").toInt());
    ui->checkboxPragmaReadUncommitted->setChecked(db.getPragma("read_uncommitted").toInt());
    ui->checkboxPragmaRecursiveTriggers->setChecked(db.getPragma("recursive_triggers").toInt());
    ui->checkboxPragmaReverseUnorderedSelects->setChecked(db.getPragma("reverse_unordered_selects").toInt());
    ui->spinPragmaSchemaVersion->setValue(db.getPragma("schema_version").toInt());
    ui->checkboxPragmaSecureDelete->setChecked(db.getPragma("secure_delete").toInt());
    ui->comboboxPragmaSynchronous->setCurrentIndex(db.getPragma("synchronous").toInt());
    ui->comboboxPragmaTempStore->setCurrentIndex(db.getPragma("temp_store").toInt());
    ui->spinPragmaUserVersion->setValue(db.getPragma("user_version").toInt());
    ui->spinPragmaWalAutoCheckpoint->setValue(db.getPragma("wal_autocheckpoint").toInt());
}

void MainWindow::savePragmas()
{
    db.setPragma("auto_vacuum", QString::number(ui->comboboxPragmaAutoVacuum->currentIndex()));
    db.setPragma("automatic_index", QString::number(ui->checkboxPragmaAutomaticIndex->isChecked()));
    db.setPragma("busy_timeout", QString::number(ui->spinPragmaBusyTimeout->value()));
    db.setPragma("cache_size", QString::number(ui->spinPragmaCacheSize->value()));
    db.setPragma("checkpoint_fullfsync", QString::number(ui->checkboxPragmaCheckpointFullFsync->isChecked()));
    db.setPragma("encoding", QString::number(ui->comboboxPragmaEncoding->currentIndex()));
    db.setPragma("foreign_keys", QString::number(ui->checkboxPragmaForeignKeys->isChecked()));
    db.setPragma("fullfsync", QString::number(ui->checkboxPragmaFullFsync->isChecked()));
    db.setPragma("ignore_check_constraints", QString::number(ui->checkboxPragmaIgnoreCheckConstraints->isChecked()));
    db.setPragma("journal_mode", QString::number(ui->comboboxPragmaJournalMode->currentIndex()));
    db.setPragma("journal_size_limit", QString::number(ui->spinPragmaJournalSizeLimit->value()));
    db.setPragma("legacy_file_format", QString::number(ui->checkboxPragmaLegacyFileFormat->isChecked()));
    db.setPragma("locking_mode", QString::number(ui->comboboxPragmaLockingMode->currentIndex()));
    db.setPragma("max_page_count", QString::number(ui->spinPragmaMaxPageCount->value()));
    db.setPragma("page_size", QString::number(ui->spinPragmaPageSize->value()));
    db.setPragma("read_uncommitted", QString::number(ui->checkboxPragmaReadUncommitted->isChecked()));
    db.setPragma("recursive_triggers", QString::number(ui->checkboxPragmaRecursiveTriggers->isChecked()));
    db.setPragma("reverse_unordered_selects", QString::number(ui->checkboxPragmaReverseUnorderedSelects->isChecked()));
    db.setPragma("schema_version", QString::number(ui->spinPragmaSchemaVersion->value()));
    db.setPragma("secure_delete", QString::number(ui->checkboxPragmaSecureDelete->isChecked()));
    db.setPragma("synchronous", QString::number(ui->comboboxPragmaSynchronous->currentIndex()));
    db.setPragma("temp_store", QString::number(ui->comboboxPragmaTempStore->currentIndex()));
    db.setPragma("user_version", QString::number(ui->spinPragmaUserVersion->value()));
    db.setPragma("wal_autocheckpoint", QString::number(ui->spinPragmaWalAutoCheckpoint->value()));
}
