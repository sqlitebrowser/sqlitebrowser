#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QFile>
#include <QApplication>
#include <QTextStream>
#include <QWhatsThis>
#include <QMessageBox>
#include <QUrl>
#include <QStandardItemModel>
#include <QDragEnterEvent>
#include <QScrollBar>
#include <QSortFilterProxyModel>
#include <QElapsedTimer>

#include "CreateIndexDialog.h"
#include "AboutDialog.h"
#include "EditTableDialog.h"
#include "ImportCsvDialog.h"
#include "ExportCsvDialog.h"
#include "PreferencesDialog.h"
#include "EditDialog.h"
#include "SQLiteSyntaxHighlighter.h"
#include "sqltextedit.h"
#include "sqlitetablemodel.h"
#include "FilterTableHeader.h"
#include "SqlExecutionArea.h"
#include "VacuumDialog.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_browseTableModel(new SqliteTableModel(this, &db, PreferencesDialog::getSettingsValue("db", "prefetchsize").toInt())),
      sqliteHighlighterLogUser(0),
      sqliteHighlighterLogApp(0),
      editWin(new EditDialog(this)),
      gotoValidator(new QIntValidator(0, 0, this))
{
    ui->setupUi(this);
    init();

    activateFields(false);
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

    // Set up the db tree widget
    ui->dbTreeWidget->setColumnHidden(1, true);
    ui->dbTreeWidget->setColumnWidth(0, 300);

    // Set the validator for the goto line edit
    ui->editGoto->setValidator(gotoValidator);

    // Set up DB models
    ui->dataTable->setModel(m_browseTableModel);

    FilterTableHeader* tableHeader = new FilterTableHeader(ui->dataTable);
    connect(tableHeader, SIGNAL(filterChanged(int,QString)), m_browseTableModel, SLOT(updateFilter(int,QString)));
    connect(tableHeader, SIGNAL(filterChanged(int,QString)), this, SLOT(setRecordsetLabel()));
    ui->dataTable->setHorizontalHeader(tableHeader);

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
    popupTableMenu->addSeparator();
    popupTableMenu->addAction(ui->editDeleteObjectAction);

    // Set state of checkable menu actions
    ui->viewMenu->insertAction(ui->viewDBToolbarAction, ui->dockLog->toggleViewAction());
    ui->viewMenu->actions().at(0)->setShortcut(QKeySequence(tr("Ctrl+L")));
    ui->viewMenu->actions().at(0)->setIcon(QIcon(":/icons/log_dock"));
    ui->viewDBToolbarAction->setChecked(!ui->toolbarDB->isHidden());

    // Set statusbar fields
    statusEncodingLabel = new QLabel(ui->statusbar);
    statusEncodingLabel->setEnabled(false);
    statusEncodingLabel->setText("UTF-8");
    statusEncodingLabel->setToolTip(tr("Database encoding"));
    ui->statusbar->addPermanentWidget(statusEncodingLabel);

    // Connect some more signals and slots
    connect(tableHeader, SIGNAL(sectionClicked(int)), this, SLOT(browseTableHeaderClicked(int)));
    connect(ui->dataTable->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setRecordsetLabel()));
    connect(editWin, SIGNAL(goingAway()), this, SLOT(editWinAway()));
    connect(editWin, SIGNAL(updateRecordText(int, int, QByteArray)), this, SLOT(updateRecordText(int, int, QByteArray)));

    // Load window settings
    restoreGeometry(PreferencesDialog::getSettingsValue("MainWindow", "geometry").toByteArray());
    restoreState(PreferencesDialog::getSettingsValue("MainWindow", "windowState").toByteArray());
    ui->comboLogSubmittedBy->setCurrentIndex(ui->comboLogSubmittedBy->findText(PreferencesDialog::getSettingsValue("SQLLogDock", "Log").toString()));

    // Set other window settings
    setAcceptDrops(true);
    setWindowTitle(QApplication::applicationName());

    // Load all settings
    reloadSettings();
}

bool MainWindow::fileOpen(const QString& fileName)
{
    bool retval = false;

    QString wFile = fileName;
    if (!QFile::exists(wFile))
    {
        wFile = QFileDialog::getOpenFileName(
                    this,
                    tr("Choose a database file"),
                    PreferencesDialog::getSettingsValue("db", "defaultlocation").toString());
    }
    if(QFile::exists(wFile) )
    {
        fileClose();
        if(db.open(wFile))
        {
            statusEncodingLabel->setText(db.getPragma("encoding"));
            setCurrentFile(wFile);
            retval = true;
        } else {
            QString err = tr("An error occurred: %1").arg(db.lastErrorMessage);
            QMessageBox::warning(this, QApplication::applicationName(), err);
        }
        loadExtensionsFromSettings();
        populateStructure();
        resetBrowser();
        if(ui->mainTab->currentIndex() == 2)
            loadPragmas();
        openSqlTab(true);
    }

    return retval;
}

void MainWindow::fileOpen()
{
    fileOpen(QString());
}

void MainWindow::fileNew()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Choose a filename to save under"), PreferencesDialog::getSettingsValue("db", "defaultlocation").toString());
    if(!fileName.isEmpty())
    {
        if(QFile::exists(fileName))
            QFile::remove(fileName);
        db.create(fileName);
        setCurrentFile(fileName);
        statusEncodingLabel->setText(db.getPragma("encoding"));
        loadExtensionsFromSettings();
        populateStructure();
        resetBrowser();
        openSqlTab(true);
        createTable();
    }
}

//** Populate DbTree Structure
void MainWindow::populateStructure()
{
    ui->dbTreeWidget->model()->removeRows(0, ui->dbTreeWidget->model()->rowCount());
    completerModelTables.clear();
    completerModelsFields.clear();
    if(!db.isOpen())
        return;

    db.updateSchema();
    QStringList tblnames = db.getBrowsableObjectNames();
    sqliteHighlighterLogUser->setTableNames(tblnames);
    sqliteHighlighterLogApp->setTableNames(tblnames);

    // setup models for sqltextedit autocomplete
    completerModelTables.setRowCount(tblnames.count());
    completerModelTables.setColumnCount(1);

    objectMap tab = db.getBrowsableObjects();
    int row = 0;
    for(objectMap::ConstIterator it=tab.begin(); it!=tab.end(); ++it, ++row)
    {
        QString sName = it.value().getname();
        QStandardItem* item = new QStandardItem(sName);
        item->setIcon(QIcon(QString(":icons/%1").arg(it.value().gettype())));
        completerModelTables.setItem(row, 0, item);

        // If it is a table add the field Nodes
        if((*it).gettype() == "table" || (*it).gettype() == "view")
        {
            QStandardItemModel* tablefieldmodel = new QStandardItemModel();
            tablefieldmodel->setRowCount((*it).fldmap.count());
            tablefieldmodel->setColumnCount(1);

            int fldrow = 0;
            for(int i=0;i<(*it).fldmap.size();i++,fldrow++)
            {
                QString fieldname = (*it).fldmap.at(i)->name();
                QStandardItem* fldItem = new QStandardItem(fieldname);
                fldItem->setIcon(QIcon(":/icons/field"));
                tablefieldmodel->setItem(fldrow, 0, fldItem);
            }
            completerModelsFields.insert(sName.toLower(), tablefieldmodel);
        }

    }
    for(int i=0;i<ui->tabSqlAreas->count();i++)
    {
        SqlExecutionArea* sqlarea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i));
        sqlarea->setTableNames(tblnames);
        sqlarea->getEditor()->setDefaultCompleterModel(&completerModelTables);
        sqlarea->getEditor()->insertFieldCompleterModels(completerModelsFields);
    }

    // fill the structure tab
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
            for(int i=0;i<(*it).fldmap.size();i++)
            {
                QTreeWidgetItem *fldItem = new QTreeWidgetItem(tableItem);
                fldItem->setText(0, (*it).fldmap.at(i)->name());
                fldItem->setText(1, "field");
                fldItem->setText(2, (*it).fldmap.at(i)->type());
                fldItem->setIcon(0, QIcon(":/icons/field"));
            }
        }
    }
}

void MainWindow::populateTable( const QString & tablename)
{
    // Remove the model-view link if the table name is empty in order to remove any data from the view
    if(tablename.isEmpty())
    {
        ui->dataTable->setModel(0);
        if(qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader()))
            qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader())->generateFilters(0);
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Set model
    ui->dataTable->setModel(m_browseTableModel);

    // Set new table
    m_browseTableModel->setTable(tablename);
    ui->dataTable->setColumnHidden(0, true);

    // Reset sorting
    curBrowseOrderByIndex = 0;
    curBrowseOrderByMode = Qt::AscendingOrder;
    m_browseTableModel->sort(curBrowseOrderByIndex, curBrowseOrderByMode);

    // Update the filter row
    qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader())->generateFilters(m_browseTableModel->columnCount());

    // Activate the add and delete record buttons and editing only if a table has been selected
    bool is_table = db.getObjectByName(tablename).gettype() == "table";
    ui->buttonNewRecord->setEnabled(is_table);
    ui->buttonDeleteRecord->setEnabled(is_table);
    ui->dataTable->setEditTriggers(is_table ? QAbstractItemView::SelectedClicked | QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed : QAbstractItemView::NoEditTriggers);

    // Set the recordset label
    setRecordsetLabel();

    // Reset the edit dialog
    if(editWin)
        editWin->reset();

    QApplication::restoreOverrideCursor();
}

void MainWindow::resetBrowser()
{
    const QString sCurrentTable = ui->comboBrowseTable->currentText();
    ui->comboBrowseTable->clear();
    const objectMap& tab = db.getBrowsableObjects();

    // fill a objmap which is sorted by table/view names
    QMap<QString, DBBrowserObject> objmap;
    for(objectMap::ConstIterator i=tab.begin();i!=tab.end();++i)
    {
        objmap[i.value().getname()] = i.value();;
    }

    // finaly fill the combobox in sorted order
    for(QMap<QString, DBBrowserObject>::ConstIterator it=objmap.constBegin();
        it!=objmap.constEnd();
        ++it)
    {
        ui->comboBrowseTable->addItem(
                    QIcon(QString(":icons/%1").arg((*it).gettype())),
                    (*it).getname());
    }

    setRecordsetLabel();
    int pos = ui->comboBrowseTable->findText(sCurrentTable);
    pos = pos == -1 ? 0 : pos;
    ui->comboBrowseTable->setCurrentIndex(pos);
    curBrowseOrderByIndex = 0;
    curBrowseOrderByMode = Qt::AscendingOrder;
    m_browseTableModel->sort(curBrowseOrderByIndex, curBrowseOrderByMode);
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
    for(int i=ui->tabSqlAreas->count()-1;i>=0;i--)
        closeSqlTab(i, true);
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    db.close();
    PreferencesDialog::setSettingsValue("MainWindow", "geometry", saveGeometry());
    PreferencesDialog::setSettingsValue("MainWindow", "windowState", saveState());
    PreferencesDialog::setSettingsValue("SQLLogDock", "Log", ui->comboLogSubmittedBy->currentText());
    QMainWindow::closeEvent(event);
}

void MainWindow::addRecord()
{
    int row = m_browseTableModel->rowCount();
    if(m_browseTableModel->insertRow(row))
    {
        selectTableLine(row);
    } else {
        QMessageBox::warning( this, QApplication::applicationName(), tr("Error adding record:\n") + db.lastErrorMessage);
    }
}

void MainWindow::deleteRecord()
{
    if(ui->dataTable->currentIndex().isValid())
    {
        int row = ui->dataTable->currentIndex().row();
        if(m_browseTableModel->removeRow(row))
        {
            populateTable(ui->comboBrowseTable->currentText());
            if(row > m_browseTableModel->totalRowCount())
                row = m_browseTableModel->totalRowCount();
            selectTableLine(row);
        } else {
            QMessageBox::warning( this, QApplication::applicationName(), tr("Error deleting record:\n") + db.lastErrorMessage);
        }
    } else {
        QMessageBox::information( this, QApplication::applicationName(), tr("Please select a record first"));
    }
}

void MainWindow::selectTableLine(int lineToSelect)
{
    // Are there even that many lines?
    if(lineToSelect >= m_browseTableModel->totalRowCount())
        return;

    QApplication::setOverrideCursor( Qt::WaitCursor );
    // Make sure this line has already been fetched
    while(lineToSelect >= m_browseTableModel->rowCount() && m_browseTableModel->canFetchMore())
          m_browseTableModel->fetchMore();

    // Select it
    ui->dataTable->clearSelection();
    ui->dataTable->selectRow(lineToSelect);
    ui->dataTable->scrollTo(ui->dataTable->currentIndex());
    QApplication::restoreOverrideCursor();
}

void MainWindow::navigatePrevious()
{
    int curRow = ui->dataTable->currentIndex().row();
    curRow -= 100;
    if(curRow < 0)
        curRow = 0;
    selectTableLine(curRow);
}


void MainWindow::navigateNext()
{
    int curRow = ui->dataTable->currentIndex().row();
    curRow += 100;
    if(curRow >= m_browseTableModel->totalRowCount())
        curRow = m_browseTableModel->totalRowCount() - 1;
    selectTableLine(curRow);
}


void MainWindow::navigateGoto()
{
    int row = ui->editGoto->text().toInt();
    if(row <= 0)
        row = 1;
    if(row > m_browseTableModel->totalRowCount())
        row = m_browseTableModel->totalRowCount();

    selectTableLine(row - 1);
    ui->editGoto->setText(QString::number(row));
}

void MainWindow::setRecordsetLabel()
{
    // Get all the numbers, i.e. the number of the first row and the last row as well as the total number of rows
    int from = ui->dataTable->verticalHeader()->visualIndexAt(0) + 1;
    int to = ui->dataTable->verticalHeader()->visualIndexAt(ui->dataTable->height()) - 1;
    int total = m_browseTableModel->totalRowCount();
    if(to == -2)
        to = total;

    // Update the validator of the goto row field
    gotoValidator->setRange(0, total);

    // Update the label showing the current position
    ui->labelRecordset->setText(tr("%1 - %2 of %3").arg(from).arg(to).arg(total));
}

void MainWindow::browseRefresh()
{
    populateTable(ui->comboBrowseTable->currentText());
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
    VacuumDialog dialog(&db, this);
    if(dialog.exec())
    {
        populateStructure();
        resetBrowser();
    }
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

void MainWindow::updateRecordText(int row, int col, const QByteArray& newtext)
{
    m_browseTableModel->setData(m_browseTableModel->index(row, col), newtext);
}

void MainWindow::editWinAway()
{
    editWin->hide();
    activateWindow();
    ui->dataTable->setCurrentIndex(ui->dataTable->currentIndex().sibling(editWin->getCurrentRow(), editWin->getCurrentCol()));
}

void MainWindow::editText(const QModelIndex& index)
{
    editWin->loadText(index.data(Qt::EditRole).toByteArray(), index.row(), index.column());
    editWin->show();
}

void MainWindow::doubleClickTable(const QModelIndex& index)
{
    if(!index.isValid())
        return;

    // Don't allow editing of other objects than tables
    if(db.getObjectByName(ui->comboBrowseTable->currentText()).gettype() != "table")
        return;

    editText(index);
}

/*
 * I'm still not happy how the results are represented to the user
 * right now you only see the result of the last executed statement.
 * A better experiance would be tabs on the bottom with query results
 * for all the executed statements.
 * Or at least a some way the use could see results/status message
 * per executed statement.
 */
void MainWindow::executeQuery()
{
    SqlExecutionArea* sqlWidget = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());

    // Get SQL code to execute. This depends on the button that's been pressed
    QString query;
    bool singleStep = false;
    if(sender()->objectName() == "actionSqlExecuteLine")
    {
        query = sqlWidget->getEditor()->document()->toPlainText().mid(sqlWidget->getEditor()->textCursor().block().position());
        singleStep = true;
    } else {
        // if a part of the query is selected, we will only execute this part
        query = sqlWidget->getSelectedSql();
        if(query.isEmpty())
            query = sqlWidget->getSql();
    }
    if (query.isEmpty())
        return;

    //log the query
    db.logSQL(query, kLogMsg_User);
    sqlite3_stmt *vm;
    QByteArray utf8Query = query.toUtf8();
    const char *tail = utf8Query.data();
    int sql3status = 0;
    QString statusMessage;
    bool modified = false;
    bool wasdirty = db.getDirty();

    // there is no choice, we have to start a transaction before
    // we create the prepared statement, otherwise every executed
    // statement will get committed after the prepared statement
    // gets finalized, see http://www.sqlite.org/lang_transaction.html
    db.setRestorePoint();

    //Accept multi-line queries, by looping until the tail is empty
    QElapsedTimer timer;
    timer.start();
    do
    {
        const char* qbegin = tail;
        sql3status = sqlite3_prepare_v2(db._db,tail,utf8Query.length(),
                            &vm, &tail);
        QString queryPart = QString::fromUtf8(qbegin, tail - qbegin);
        if (sql3status == SQLITE_OK){
            sql3status = sqlite3_step(vm);
            sqlite3_finalize(vm);
            switch(sql3status)
            {
            case SQLITE_DONE:
            case SQLITE_ROW:
            {
                sqlWidget->getModel()->setQuery(queryPart);
                if(sqlWidget->getModel()->valid())
                {
                    statusMessage = tr("%1 Rows returned from: %2 (took %3ms)").arg(
                                sqlWidget->getModel()->totalRowCount()).arg(queryPart.trimmed()).arg(timer.elapsed());
                    sqlWidget->enableSaveButton(true);
                    sql3status = SQLITE_OK;
                }
                else
                {
                    statusMessage = tr("Error executing query: %1").arg(queryPart);
                    sql3status = SQLITE_ERROR;
                }
            }
            case SQLITE_OK:
            {
                if( !queryPart.trimmed().startsWith("SELECT", Qt::CaseInsensitive) )
                {
                    modified = true;
                    statusMessage = tr("Query executed successfully: %1 (took %2ms)").arg(queryPart.trimmed()).arg(timer.elapsed());
                }
                break;
            }
            default:
                statusMessage = QString::fromUtf8((const char*)sqlite3_errmsg(db._db)) +
                        ": " + queryPart;
                break;
            }
            timer.restart();

            // Stop after the first full statement if we're in single step mode
            if(singleStep)
                break;
        } else {
            statusMessage = QString::fromUtf8((const char*)sqlite3_errmsg(db._db)) +
                    ": " + queryPart;
        }
    } while( tail && *tail != 0 && (sql3status == SQLITE_OK || sql3status == SQLITE_DONE));
    sqlWidget->finishExecution(statusMessage);

    if(!modified && !wasdirty)
        db.revert(); // better rollback, if the logic is not enough we can tune it.
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
                PreferencesDialog::getSettingsValue("db", "defaultlocation").toString(),
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
    ExportCsvDialog dialog(&db, this);
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
        db.saveAll();
}

void MainWindow::fileRevert()
{
    if (db.isOpen()){
        QString msg = tr("Are you sure you want to undo all changes made to the database file '%1' since the last save?").arg(db.curDBFilename);
        if(QMessageBox::question(this, QApplication::applicationName(), msg, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape) == QMessageBox::Yes)
        {
            db.revertAll();
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
                PreferencesDialog::getSettingsValue("db", "defaultlocation").toString(),
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
    // Get file name to import
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Choose a file to import"),
                PreferencesDialog::getSettingsValue("db", "defaultlocation").toString(),
                tr("Text files(*.sql *.txt);;All files(*)"));

    // Cancel when file doesn't exist
    if(!QFile::exists(fileName))
        return;

    // If there is already a database file opened ask the user wether to import into this one or a new one. If no DB is opened just ask for a DB name directly
    QString newDbFile;
    if((db.isOpen() && QMessageBox::question(this,
                                            QApplication::applicationName(),
                                            tr("Do you want to create a new database file to hold the imported data?\n"
                                               "If you answer no we will attempt to import the data in the SQL file to the current database."),
                                            QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) || !db.isOpen())
    {
        newDbFile = QFileDialog::getSaveFileName(
                    this,
                    tr("Choose a filename to save under"),
                    PreferencesDialog::getSettingsValue("db", "defaultlocation").toString());
        if(QFile::exists(newDbFile))
        {
            QMessageBox::information(this, QApplication::applicationName(), tr("File %1 already exists. Please choose a different name.").arg(newDbFile));
            return;
        } else if(newDbFile.size() == 0) {
            return;
        }

        db.create(newDbFile);
        loadExtensionsFromSettings();
    }

    // Open, read, execute and close file
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    if(!db.executeMultiSQL(f.readAll(), newDbFile.size() == 0))
        QMessageBox::warning(this, QApplication::applicationName(), tr("Error importing data: %1").arg(db.lastErrorMessage));
    else
        QMessageBox::information(this, QApplication::applicationName(), tr("Import completed."));
    f.close();
    QApplication::restoreOverrideCursor();

    // Resfresh window when importing into an existing DB or - when creating a new file - just open it correctly
    if(newDbFile.size())
    {
        fileOpen(newDbFile);
    } else {
        populateStructure();
        resetBrowser();
    }
}

void MainWindow::openPreferences()
{
    PreferencesDialog dialog(this);
    if(dialog.exec())
        reloadSettings();
}

void MainWindow::createSyntaxHighlighters()
{
    delete sqliteHighlighterLogApp;
    delete sqliteHighlighterLogUser;
    sqliteHighlighterLogApp = new SQLiteSyntaxHighlighter(ui->editLogApplication->document());
    sqliteHighlighterLogUser = new SQLiteSyntaxHighlighter(ui->editLogUser->document());
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
}
//** Tree selection changed
void MainWindow::changeTreeSelection()
{
    // Just assume first that something's selected that can not be edited at all
    ui->editDeleteObjectAction->setEnabled(false);
    ui->editModifyTableAction->setEnabled(false);

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
    } else if(ui->dbTreeWidget->currentItem()->text(1) == "view" || ui->dbTreeWidget->currentItem()->text(1) == "trigger" || ui->dbTreeWidget->currentItem()->text(1) == "index") {
        ui->editDeleteObjectAction->setEnabled(true);
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
    QStringList files = PreferencesDialog::getSettingsValue("General", "recentFileList").toStringList();
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

    QStringList files = PreferencesDialog::getSettingsValue("General", "recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    PreferencesDialog::setSettingsValue("General", "recentFileList", files);

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
    ui->scrollAreaWidgetContents->setEnabled(enable);
    ui->buttonBoxPragmas->setEnabled(enable);
    ui->buttonGoto->setEnabled(enable);
    ui->editGoto->setEnabled(enable);
    ui->buttonRefresh->setEnabled(enable);
    ui->buttonDeleteRecord->setEnabled(enable);
    ui->buttonNewRecord->setEnabled(enable);
    ui->actionExecuteSql->setEnabled(enable);
    ui->actionLoadExtension->setEnabled(enable);
    ui->actionSqlExecuteLine->setEnabled(enable);
    ui->actionSqlOpenFile->setEnabled(enable);
    ui->actionSqlOpenTab->setEnabled(enable);
    ui->actionSqlSaveFile->setEnabled(enable);
}

void MainWindow::browseTableHeaderClicked(int logicalindex)
{
    // Abort if there is more than one column selected because this tells us that the user pretty sure wants to do a range selection instead of sorting data
    if(ui->dataTable->selectionModel()->selectedColumns().count() > 1)
        return;

    // instead of the column name we just use the column index, +2 because 'rowid, *' is the projection
    curBrowseOrderByIndex = logicalindex;
    curBrowseOrderByMode = curBrowseOrderByMode == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder;
    m_browseTableModel->sort(curBrowseOrderByIndex, curBrowseOrderByMode);

    // select the first item in the column so the header is bold
    // we might try to select the last selected item
    ui->dataTable->setCurrentIndex(ui->dataTable->currentIndex().sibling(0, logicalindex));
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

void MainWindow::closeSqlTab(int index, bool force)
{
    // Don't close last tab
    if(ui->tabSqlAreas->count() == 1 && !force)
        return;

    // Remove the tab and delete the widget
    QWidget* w = ui->tabSqlAreas->widget(index);
    ui->tabSqlAreas->removeTab(index);
    delete w;
}

unsigned int MainWindow::openSqlTab(bool resetCounter)
{
    static unsigned int tabNumber = 0;

    if(resetCounter)
        tabNumber = 0;

    // Create new tab, add it to the tab widget and select it
    SqlExecutionArea* w = new SqlExecutionArea(this, &db);
    w->setTableNames(db.getBrowsableObjectNames());
    w->getEditor()->setDefaultCompleterModel(&completerModelTables);
    w->getEditor()->insertFieldCompleterModels(completerModelsFields);
    int index = ui->tabSqlAreas->addTab(w, QString("SQL %1").arg(++tabNumber));
    ui->tabSqlAreas->setCurrentIndex(index);

    return index;
}

void MainWindow::openSqlFile()
{
    QString file = QFileDialog::getOpenFileName(
                this,
                tr("Select SQL file to open"),
                PreferencesDialog::getSettingsValue("db", "defaultlocation").toString(),
                tr("Text files(*.sql *.txt);;All files(*)"));

    if(QFile::exists(file))
    {
        QFile f(file);
        f.open(QIODevice::ReadOnly);

        // Decide wether to open a new tab or take the current one
        unsigned int index;
        SqlExecutionArea* current_tab = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());
        if(current_tab->getSql().isEmpty() && current_tab->getModel()->rowCount() == 0)
            index = ui->tabSqlAreas->currentIndex();
        else
            index = openSqlTab();

        qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(index))->getEditor()->setPlainText(f.readAll());
        QFileInfo fileinfo(file);
        ui->tabSqlAreas->setTabText(index, fileinfo.fileName());
    }
}

void MainWindow::saveSqlFile()
{
    QString file = QFileDialog::getSaveFileName(
                this,
                tr("Select file name"),
                PreferencesDialog::getSettingsValue("db", "defaultlocation").toString(),
                tr("Text files(*.sql *.txt);;All files(*)"));

    QFile f(file);
    f.open(QIODevice::WriteOnly);
    f.write(qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget())->getSql().toUtf8());
    QFileInfo fileinfo(file);
    ui->tabSqlAreas->setTabText(ui->tabSqlAreas->currentIndex(), fileinfo.fileName());
}

void MainWindow::loadExtension()
{
    QString file = QFileDialog::getOpenFileName(
                this,
                tr("Select extension file"),
                PreferencesDialog::getSettingsValue("db", "defaultlocation").toString(),
                tr("Extensions(*.so *.dll);;All files(*)"));

    if(file.isEmpty())
        return;

    if(db.loadExtension(file))
        QMessageBox::information(this, QApplication::applicationName(), tr("Extension successfully loaded."));
    else
        QMessageBox::warning(this, QApplication::applicationName(), tr("Error loading extension: %1").arg(db.lastErrorMessage));
}

void MainWindow::loadExtensionsFromSettings()
{
    if(!db.isOpen())
        return;

    QStringList list = PreferencesDialog::getSettingsValue("extensions", "list").toStringList();
    foreach(QString ext, list)
    {
        if(db.loadExtension(ext) == false)
            QMessageBox::warning(this, QApplication::applicationName(), tr("Error loading extension: %1").arg(db.lastErrorMessage));
    }
}

void MainWindow::reloadSettings()
{
    // Set prefetch sizes for lazy population of table models
    m_browseTableModel->setChunkSize(PreferencesDialog::getSettingsValue("db", "prefetchsize").toInt());
    for(int i=0;i<ui->tabSqlAreas->count();i++)
    {
        SqlExecutionArea* sqlArea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i));
        sqlArea->getModel()->setChunkSize(PreferencesDialog::getSettingsValue("db", "prefetchsize").toInt());

        QFont font = sqlArea->getEditor()->font();
        font.setPointSize(PreferencesDialog::getSettingsValue("editor", "fontsize").toInt());
        sqlArea->getEditor()->setFont(font);
    }

    // Create the syntax highlighters
    createSyntaxHighlighters();

    // Set font for SQL logs
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(PreferencesDialog::getSettingsValue("log", "fontsize").toInt());
    ui->editLogApplication->setFont(font);
    ui->editLogUser->setFont(font);

    // Load extensions
    loadExtensionsFromSettings();

    // Refresh view
    populateStructure();
    resetBrowser();
}
