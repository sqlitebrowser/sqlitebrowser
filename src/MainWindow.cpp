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
#include "CreateIndexDialog.h"
#include "AboutDialog.h"
#include "EditTableDialog.h"
#include "EditFieldDialog.h"
#include "ImportCsvDialog.h"
#include "ExportCsvDialog.h"
#include "PreferencesDialog.h"
#include "EditDialog.h"
#include "FindDialog.h"
#include "SQLiteSyntaxHighlighter.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      editWin(new EditDialog(this)),
      findWin(0)
{
    ui->setupUi(this);
    init();

    activateFields(false);
    updatePreferences();
    resetBrowser();
    updateRecentFileActions();
}

MainWindow::~MainWindow()
{
    delete gotoValidator;
    delete ui;
}

void MainWindow::init()
{
    // Init the SQL log dock
    db.mainWindow = this;
    sqliteHighlighterLogUser = new SQLiteSyntaxHighlighter(ui->editLogUser->document());
    sqliteHighlighterLogApp = new SQLiteSyntaxHighlighter(ui->editLogApplication->document());

    // Set up the db tree widget
    ui->dbTreeWidget->setColumnHidden(1, true);
    ui->dbTreeWidget->setColumnWidth(0, 300);

    // Create the validator for the goto line edit
    gotoValidator = new QIntValidator(0, 0, this);
    ui->editGoto->setValidator(gotoValidator);

    // Create the SQL sytax highlighter
    sqliteHighlighterTabSql = new SQLiteSyntaxHighlighter(ui->sqlTextEdit->document());

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
    ui->sqlLogAction->setChecked(!ui->dockLog->isHidden());
    ui->viewDBToolbarAction->setChecked(!ui->toolbarDB->isHidden());

    // Connect some more signals and slots
    connect(ui->dataTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(browseTableHeaderClicked(int)));
    connect(ui->dataTable->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setRecordsetLabel()));
    connect(editWin, SIGNAL(goingAway()), this, SLOT(editWinAway()));
    connect(editWin, SIGNAL(updateRecordText(int, int , QString)), this, SLOT(updateRecordText(int, int , QString)));

    // Load window settings
    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());
    ui->comboLogSubmittedBy->setCurrentIndex(ui->comboLogSubmittedBy->findText(settings.value("SQLLogDock/Log", "Application").toString()));

    // Set other window settings
    setAcceptDrops(true);
    setWindowTitle(QApplication::applicationName());
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
                    tr("Choose a database file"),
                    defaultlocation);
    }
    if(QFile::exists(wFile) )
    {
        fileClose();
        if(db.open(wFile))
        {
            setCurrentFile(wFile);
        } else {
            QString err = tr("An error occurred: %1").arg(db.lastErrorMessage);
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Choose a filename to save under"), defaultlocation);
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
    sqliteHighlighterTabSql->setTableNames(tblnames);
    sqliteHighlighterLogUser->setTableNames(tblnames);
    sqliteHighlighterLogApp->setTableNames(tblnames);

    QMap<QString, QTreeWidgetItem*> typeToParentItem;
    QTreeWidgetItem* itemTables = new QTreeWidgetItem(ui->dbTreeWidget);
    itemTables->setIcon(0, QIcon(QString(":/icons/table")));
    itemTables->setText(0, tr("Tables (%1)").arg(db.objMap.values("table").count()));
    typeToParentItem.insert("table", itemTables);
    QTreeWidgetItem* itemIndices = new QTreeWidgetItem(ui->dbTreeWidget);
    itemIndices->setIcon(0, QIcon(QString(":/icons/index")));
    itemIndices->setText(0, tr("Indices (%1)").arg(db.objMap.values("index").count()));
    typeToParentItem.insert("index", itemIndices);
    QTreeWidgetItem* itemViews = new QTreeWidgetItem(ui->dbTreeWidget);
    itemViews->setIcon(0, QIcon(QString(":/icons/view")));
    itemViews->setText(0, tr("Views (%1)").arg(db.objMap.values("view").count()));
    typeToParentItem.insert("view", itemViews);
    QTreeWidgetItem* itemTriggers = new QTreeWidgetItem(ui->dbTreeWidget);
    itemTriggers->setIcon(0, QIcon(QString(":/icons/trigger")));
    itemTriggers->setText(0, tr("Triggers (%1)").arg(db.objMap.values("trigger").count()));
    typeToParentItem.insert("trigger", itemTriggers);
    ui->dbTreeWidget->setItemExpanded(itemTables, true);
    ui->dbTreeWidget->setItemExpanded(itemIndices, true);
    ui->dbTreeWidget->setItemExpanded(itemViews, true);
    ui->dbTreeWidget->setItemExpanded(itemTriggers, true);

    for(objectMap::ConstIterator it=db.objMap.begin();it!=db.objMap.end();++it)
    {
        // Object node
        QTreeWidgetItem *tableItem = new QTreeWidgetItem(typeToParentItem.value((*it).gettype()));
        tableItem->setIcon(0, QIcon(QString(":/icons/%1").arg((*it).gettype())));
        tableItem->setText(0, (*it).getname());
        tableItem->setText(1, (*it).gettype());
        tableItem->setText(3, (*it).getsql());

        // If it is a table add the field Nodes
        if((*it).gettype() == "table" || (*it).gettype() == "view")
        {
            fieldMap::ConstIterator fit;
            for ( fit = (*it).fldmap.begin(); fit != (*it).fldmap.end(); ++fit ) {
                QTreeWidgetItem *fldItem = new QTreeWidgetItem(tableItem);
                fldItem->setText(0, fit.value().getname());
                fldItem->setText(1, "field");
                fldItem->setText(2, fit.value().gettype());
                fldItem->setIcon(0, QIcon(":/icons/field"));
            }
        }
    }
}

void MainWindow::populateTable( const QString & tablename, bool keepColumnWidths)
{
    bool mustreset = false;
    QApplication::setOverrideCursor( Qt::WaitCursor );
    if (tablename.compare(db.curBrowseTableName)!=0)
    {
        mustreset = true;
        curBrowseOrderByIndex = 1;
        curBrowseOrderByMode = ORDERMODE_ASC;
    }

    QString orderby = QString::number(curBrowseOrderByIndex) + " " + (curBrowseOrderByMode == ORDERMODE_ASC ? "ASC" : "DESC");
    if (!db.browseTable(tablename, orderby)){
        ui->dataTable->setRowCount( 0 );
        ui->dataTable->setColumnCount( 0 );
        QApplication::restoreOverrideCursor();
        if(findWin)
            findWin->resetFields(db.getTableFields(""));
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
    if(findWin)
        findWin->resetFields();
    if(editWin)
        editWin->reset();
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
    ui->buttonLogClear->click();
}

void MainWindow::fileExit()
{
    if (db.isOpen())
    {
        if (db.getDirty())
        {
            QString msg = tr("Do you want to save the changes made to the database file %1?").arg(db.curDBFilename);
            if(QMessageBox::question( this, QApplication::applicationName() ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
                db.save();
            else
                db.revert(); //not really necessary, I think... but will not hurt.
        }
        db.close();
    }
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
    settings.setValue("SQLLogDock/Log", ui->comboLogSubmittedBy->currentText());
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
                                  tr("Error adding record, make sure a table is selected.\n\n"
                                  "If the table contain fields declared as NOT NULL\n"
                                  "please select EDIT->PREFERENCES and adjust the\n"
                                  "default value for new records to insert an empty string."));
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
        QMessageBox::information( this, QApplication::applicationName(), tr("Please select a record first"));
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

    ui->labelRecordset->setText(tr("%1 - %2 of %3").arg(from).arg(to).arg(total));
}

void MainWindow::browseFind(bool open)
{
    if(open)
    {
        if(!findWin)
        {
            findWin = new FindDialog(this);
            connect(findWin, SIGNAL(lookfor(const QString&, const QString&, const QString&)), this, SLOT(lookfor(const QString&, const QString&, const QString&)));
            connect(findWin, SIGNAL(showrecord(int)),this, SLOT(updateTableView(int)));
            connect(findWin, SIGNAL(goingAway()),this, SLOT(browseFindAway()));
        }
        findWin->resetFields(db.getTableFields(db.curBrowseTableName));
        findWin->show();
    } else {
        if(findWin)
            findWin->hide();
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
        QMessageBox::information( this, QApplication::applicationName(), tr("There is no database opened. Please open or create a new database file."));
        return;
    }
    
    //we may need to modify woperator and wsearchterm, so use copies
    QString finaloperator = woperator;
    QString finalsearchterm = wsearchterm;
    
    //special case for CONTAINS operator: use LIKE and surround the search word with % characters
    if(woperator.compare(tr("contains")) == 0)
    {
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
    finalsearchterm.toDouble(&ok);
    if (!ok) finalsearchterm.toInt(&ok, 10);
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

void MainWindow::createTable()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), tr("There is no database opened. Please open or create a new database file."));
        return;
    }

    EditTableDialog dialog(&db, "", this);
    if(dialog.exec())
    {
        populateStructure();
        resetBrowser();
    }
}

void MainWindow::createIndex()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), tr("There is no database opened. Please open or create a new database file."));
        return;
    }
    CreateIndexDialog dialog(&db, this);
    if(dialog.exec())
    {
        populateStructure();
        resetBrowser();
    }
}

void MainWindow::compact()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );
    if (!db.compact()){
        QString error = tr("Error: could not compact the database file. Message from database engine: %1").arg(db.lastErrorMessage);
        QApplication::restoreOverrideCursor( );
        QMessageBox::warning( this, QApplication::applicationName(), error );
    } else {
        QApplication::restoreOverrideCursor( );
        QMessageBox::information(this, QApplication::applicationName(), tr("Database successfully compacted."));
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
        QString statement = QString("DROP %1 `%2`;").arg(type.toUpper()).arg(table);
        if(!db.executeSQL( statement))
        {
            QString error = tr("Error: could not delete the %1. Message from database engine:\n%2").arg(type).arg(db.lastErrorMessage);
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
        QMessageBox::information( this, QApplication::applicationName(), tr("There is no database opened."));
        return;
    }
    if(!ui->dbTreeWidget->selectionModel()->hasSelection()){
        return;
    }
    QString tableToEdit = ui->dbTreeWidget->currentItem()->text(0);

    EditTableDialog dialog(&db, tableToEdit, this);
    if(dialog.exec())
    {
        populateStructure();
        resetBrowser();
    }
}

void MainWindow::helpWhatsThis()
{
    QWhatsThis::enterWhatsThisMode ();
}

void MainWindow::helpAbout()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::updateRecordText(int row, int col, const QString& newtext)
{
    if (!db.updateRecord(row, col, newtext)){
        QMessageBox::information( this, QApplication::applicationName(), tr("Data could not be updated"));
    }

    rowList tab = db.browseRecs;
    QStringList& rt = tab[row];
    QString& cv = rt[col+1];//must account for rowid

    QTableWidgetItem* item = new QTableWidgetItem(cv);
    item->setToolTip( wrapText(cv) );
    ui->dataTable->setItem(row, col, item);

}

void MainWindow::editWinAway()
{
    editWin->hide();
    activateWindow();
    ui->dataTable->setRangeSelected(QTableWidgetSelectionRange(editWin->getCurrentRow(), editWin->getCurrentCol(), editWin->getCurrentRow(), editWin->getCurrentCol()), true);
}

void MainWindow::editText(int row, int col)
{
    rowList tab = db.browseRecs;
    QStringList& rt = tab[row];
    QString cv = rt[col+1];//must account for rowid

    editWin->loadText(cv , row, col);
    editWin->show();
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
        QMessageBox::information( this, QApplication::applicationName(), tr("Query string is empty"));
        return;
    }
    //log the query
    db.logSQL(query, kLogMsg_User);
    sqlite3_stmt *vm;
    QByteArray utf8Query = query.toUtf8();
    const char *tail = utf8Query.data();
    int ncol;
    int err=0;
    QString lastErrorMessage = tr("No error");
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
    {
        populateStructure();
    } else if(tabindex == 1) {
        populateStructure();
        resetBrowser();
    } else if(tabindex == 2) {
        loadPragmas();
    }
}

void MainWindow::importTableFromCSV()
{
    QString wFile = QFileDialog::getOpenFileName(
                this,
                tr("Choose a text file"),
                defaultlocation,
                tr("Text files(*.csv *.txt);;All files(*)"));

    if (QFile::exists(wFile) )
    {
        ImportCsvDialog dialog(wFile, &db, this);
        if(dialog.exec())
        {
            populateStructure();
            resetBrowser();
            QMessageBox::information(this, QApplication::applicationName(), tr("Import completed"));
        }
    }
}

void MainWindow::exportTableToCSV()
{
    ExportCsvDialog dialog(&db, defaultlocation, this);
    dialog.exec();
}

void MainWindow::dbState( bool dirty )
{
    ui->fileSaveAction->setEnabled(dirty);
    ui->fileRevertAction->setEnabled(dirty);
}

void MainWindow::fileSave()
{
    if(db.isOpen())
        db.save();
}

void MainWindow::fileRevert()
{
    if (db.isOpen()){
        QString msg = tr("Are you sure you want to undo all changes made to the database file '%1' since the last save?").arg(db.curDBFilename);
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
                tr("Choose a filename to export"),
                defaultlocation,
                tr("Text files(*.sql *.txt)"));

    if(fileName.size())
    {
        if(!db.dump(fileName))
            QMessageBox::warning(this, QApplication::applicationName(), tr("Export cancelled or failed."));
        else
            QMessageBox::information(this, QApplication::applicationName(), tr("Export completed."));
    }
}

void MainWindow::importDatabaseFromSQL()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Choose a file to import"),
                defaultlocation,
                tr("Text files(*.sql *.txt);;All files(*)"));

    if (fileName.size() > 0)
    {
        QString msg = tr("Do you want to create a new database file to hold the imported data?\nIf you answer NO we will attempt to import data in the .sql file to the current database.");
        if (QMessageBox::question( this, QApplication::applicationName() ,msg, QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
        {
            QString newDBfile = QFileDialog::getSaveFileName(
                        this,
                        tr("Choose a filename to save under"),
                        defaultlocation);
            if (QFile::exists(newDBfile) )
            {
                QString err = tr("File %1 already exists. Please choose a different name.").arg(newDBfile);
                QMessageBox::information( this, QApplication::applicationName() ,err);
                return;
            }
            if(!fileName.isNull())
                db.create(newDBfile);
        }
        int lineErr;
        if (!db.reload(fileName, &lineErr))
            QMessageBox::information(this, QApplication::applicationName(), tr("Error importing data at line %1").arg(lineErr) );
        else
            QMessageBox::information(this, QApplication::applicationName(), tr("Import completed"));
        populateStructure();
        resetBrowser();
    }
}

void MainWindow::openPreferences()
{
    PreferencesDialog dialog(this);
    if(dialog.exec())
    {
        updatePreferences();
        resetBrowser();
    }
}

void MainWindow::updatePreferences()
{
    PreferencesDialog prefs(this);

    if(prefs.defaultencoding == "Latin1")
        db.setEncoding(kEncodingLatin1);
    else
        db.setEncoding(kEncodingUTF8);

    db.setDefaultNewData(prefs.defaultnewdata);
    defaultlocation= prefs.defaultlocation;
    editWin->defaultlocation = defaultlocation;
}

//******************************************************************
//** Tree Events
//******************************************************************

//** Db Tree Context Menu
void MainWindow::createTreeContextMenu(const QPoint &qPoint)
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
void MainWindow::changeTreeSelection()
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

void MainWindow::addField()
{
    EditFieldDialog dialog(&db, true, ui->dbTreeWidget->currentItem()->text(0), "", "TEXT", this);
    if(dialog.exec())
        populateStructure();
}

void MainWindow::editField()
{
    QTreeWidgetItem *item = ui->dbTreeWidget->currentItem();
    EditFieldDialog dialog(&db, false, item->parent()->text(0), item->text(0), item->text(2), this);
    if(dialog.exec())
    {
        item->setText(0, dialog.getFieldName());
        item->setText(2, dialog.getFieldType());
    }
}

void MainWindow::deleteField()
{
    if(!ui->dbTreeWidget->currentItem())
        return;

    // Ask user wether he really wants to delete that column first
    QString msg = tr("Are you sure you want to delete the field '%1'?\nAll data currently stored in this field will be lost.").arg(ui->dbTreeWidget->currentItem()->text(0));
    if(QMessageBox::warning(this, QApplication::applicationName(), msg, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape) == QMessageBox::Yes)
    {
        db.dropColumn(ui->dbTreeWidget->currentItem()->parent()->text(0), ui->dbTreeWidget->currentItem()->text(0));
        delete ui->dbTreeWidget->currentItem();
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
    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
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

    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
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
    ui->buttonBoxPragmas->setEnabled(enable);
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
    pragmaValues.autovacuum = db.getPragma("auto_vacuum").toInt();
    pragmaValues.automatic_index = db.getPragma("automatic_index").toInt();
    pragmaValues.checkpoint_fullsync = db.getPragma("checkpoint_fullfsync").toInt();
    pragmaValues.foreign_keys = db.getPragma("foreign_keys").toInt();
    pragmaValues.fullfsync = db.getPragma("fullfsync").toInt();
    pragmaValues.ignore_check_constraints = db.getPragma("ignore_check_constraints").toInt();
    pragmaValues.journal_mode = db.getPragma("journal_mode").toUpper();
    pragmaValues.journal_size_limit = db.getPragma("journal_size_limit").toInt();
    pragmaValues.locking_mode = db.getPragma("locking_mode").toUpper();
    pragmaValues.max_page_count = db.getPragma("max_page_count").toInt();
    pragmaValues.page_size = db.getPragma("page_size").toInt();
    pragmaValues.recursive_triggers = db.getPragma("recursive_triggers").toInt();
    pragmaValues.secure_delete = db.getPragma("secure_delete").toInt();
    pragmaValues.synchronous = db.getPragma("synchronous").toInt();
    pragmaValues.temp_store = db.getPragma("temp_store").toInt();
    pragmaValues.user_version = db.getPragma("user_version").toInt();
    pragmaValues.wal_autocheckpoint = db.getPragma("wal_autocheckpoint").toInt();

    updatePragmaUi();
}

void MainWindow::updatePragmaUi()
{
    ui->comboboxPragmaAutoVacuum->setCurrentIndex(pragmaValues.autovacuum);
    ui->checkboxPragmaAutomaticIndex->setChecked(pragmaValues.automatic_index);
    ui->checkboxPragmaCheckpointFullFsync->setChecked(pragmaValues.checkpoint_fullsync);
    ui->checkboxPragmaForeignKeys->setChecked(pragmaValues.foreign_keys);
    ui->checkboxPragmaFullFsync->setChecked(pragmaValues.fullfsync);
    ui->checkboxPragmaIgnoreCheckConstraints->setChecked(pragmaValues.ignore_check_constraints);
    ui->comboboxPragmaJournalMode->setCurrentIndex(ui->comboboxPragmaJournalMode->findText(pragmaValues.journal_mode, Qt::MatchFixedString));
    ui->spinPragmaJournalSizeLimit->setValue(pragmaValues.journal_size_limit);
    ui->comboboxPragmaLockingMode->setCurrentIndex(ui->comboboxPragmaLockingMode->findText(pragmaValues.locking_mode, Qt::MatchFixedString));
    ui->spinPragmaMaxPageCount->setValue(pragmaValues.max_page_count);
    ui->spinPragmaPageSize->setValue(pragmaValues.page_size);
    ui->checkboxPragmaRecursiveTriggers->setChecked(pragmaValues.recursive_triggers);
    ui->checkboxPragmaSecureDelete->setChecked(pragmaValues.secure_delete);
    ui->comboboxPragmaSynchronous->setCurrentIndex(pragmaValues.synchronous);
    ui->comboboxPragmaTempStore->setCurrentIndex(pragmaValues.temp_store);
    ui->spinPragmaUserVersion->setValue(pragmaValues.user_version);
    ui->spinPragmaWalAutoCheckpoint->setValue(pragmaValues.wal_autocheckpoint);
}

void MainWindow::savePragmas()
{
    if( db.getDirty() )
    {
        QString msg = tr("Setting PRAGMA values will commit your current transaction.\nAre you sure?");
        if(QMessageBox::question(this, QApplication::applicationName(), msg, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape) == QMessageBox::No)
        {
            return; // abort
        }
    }
    db.setPragma("auto_vacuum", ui->comboboxPragmaAutoVacuum->currentIndex(), pragmaValues.autovacuum);
    db.setPragma("automatic_index", ui->checkboxPragmaAutomaticIndex->isChecked(), pragmaValues.automatic_index);
    db.setPragma("checkpoint_fullfsync", ui->checkboxPragmaCheckpointFullFsync->isChecked(), pragmaValues.checkpoint_fullsync);
    db.setPragma("foreign_keys", ui->checkboxPragmaForeignKeys->isChecked(), pragmaValues.foreign_keys);
    db.setPragma("fullfsync", ui->checkboxPragmaFullFsync->isChecked(), pragmaValues.fullfsync);
    db.setPragma("ignore_check_constraints", ui->checkboxPragmaIgnoreCheckConstraints->isChecked(), pragmaValues.ignore_check_constraints);
    db.setPragma("journal_mode", ui->comboboxPragmaJournalMode->currentText().toUpper(), pragmaValues.journal_mode);
    db.setPragma("journal_size_limit", ui->spinPragmaJournalSizeLimit->value(), pragmaValues.journal_size_limit);
    db.setPragma("locking_mode", ui->comboboxPragmaLockingMode->currentText().toUpper(), pragmaValues.locking_mode);
    db.setPragma("max_page_count", ui->spinPragmaMaxPageCount->value(), pragmaValues.max_page_count);
    db.setPragma("page_size", ui->spinPragmaPageSize->value(), pragmaValues.page_size);
    db.setPragma("recursive_triggers", ui->checkboxPragmaRecursiveTriggers->isChecked(), pragmaValues.recursive_triggers);
    db.setPragma("secure_delete", ui->checkboxPragmaSecureDelete->isChecked(), pragmaValues.secure_delete);
    db.setPragma("synchronous", ui->comboboxPragmaSynchronous->currentIndex(), pragmaValues.synchronous);
    db.setPragma("temp_store", ui->comboboxPragmaTempStore->currentIndex(), pragmaValues.temp_store);
    db.setPragma("user_version", ui->spinPragmaUserVersion->value(), pragmaValues.user_version);
    db.setPragma("wal_autocheckpoint", ui->spinPragmaWalAutoCheckpoint->value(), pragmaValues.wal_autocheckpoint);

    updatePragmaUi();
}

void MainWindow::logSql(const QString& sql, int msgtype)
{
    if(msgtype == kLogMsg_User)
    {
        ui->editLogUser->append(sql);
        ui->editLogUser->verticalScrollBar()->setValue(ui->editLogUser->verticalScrollBar()->maximum());
    } else {
        ui->editLogApplication->append(sql);
        ui->editLogApplication->verticalScrollBar()->setValue(ui->editLogApplication->verticalScrollBar()->maximum());
    }
}
