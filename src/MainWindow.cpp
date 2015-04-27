#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "CreateIndexDialog.h"
#include "AboutDialog.h"
#include "EditTableDialog.h"
#include "ImportCsvDialog.h"
#include "ExportCsvDialog.h"
#include "PreferencesDialog.h"
#include "EditDialog.h"
#include "sqlitetablemodel.h"
#include "SqlExecutionArea.h"
#include "VacuumDialog.h"
#include "DbStructureModel.h"
#include "version.h"
#include "sqlite.h"
#include "CipherDialog.h"
#include "ExportSqlDialog.h"
#include "SqlUiLexer.h"
#include "FileDialog.h"
#include "ColumnDisplayFormatDialog.h"

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
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QMimeData>
#include <QColorDialog>
#include <QDesktopServices>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QInputDialog>
#include <QProgressDialog>
#include <QTextEdit>
#include <QClipboard>
#include <QShortcut>
#include <QTextCodec>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_browseTableModel(new SqliteTableModel(this, &db, PreferencesDialog::getSettingsValue("db", "prefetchsize").toInt())),
      editWin(new EditDialog(this)),
      editDock(new EditDialog(this, true)),
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
    // Connect SQL logging and database state setting to main window
    connect(&db, SIGNAL(dbChanged(bool)), this, SLOT(dbState(bool)));
    connect(&db, SIGNAL(sqlExecuted(QString, int)), this, SLOT(logSql(QString,int)));

    // Set the validator for the goto line edit
    ui->editGoto->setValidator(gotoValidator);

    // Set up DB models
    ui->dataTable->setModel(m_browseTableModel);

    // Set up DB structure tab
    dbStructureModel = new DbStructureModel(ui->dbTreeWidget);
    ui->dbTreeWidget->setModel(dbStructureModel);
    ui->dbTreeWidget->setColumnHidden(1, true);
    ui->dbTreeWidget->setColumnWidth(0, 300);

    // Set up DB schema dock
    ui->treeSchemaDock->setModel(dbStructureModel);
    ui->treeSchemaDock->setColumnHidden(1, true);
    ui->treeSchemaDock->setColumnWidth(0, 300);

    // Edit dock
    ui->dockEditWindow->setWidget(editDock);
    ui->dockEditWindow->hide();     // Hidden by default

    // Add keyboard shortcuts
    QList<QKeySequence> shortcuts = ui->actionExecuteSql->shortcuts();
    shortcuts.push_back(QKeySequence(tr("Ctrl+Return")));
    ui->actionExecuteSql->setShortcuts(shortcuts);

    QShortcut* shortcutBrowseRefresh = new QShortcut(QKeySequence("Ctrl+R"), this);
    connect(shortcutBrowseRefresh, SIGNAL(activated()), ui->buttonRefresh, SLOT(click()));

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
    popupTableMenu->addAction(ui->actionEditBrowseTable);
    popupTableMenu->addAction(ui->editModifyTableAction);
    popupTableMenu->addAction(ui->editDeleteObjectAction);
    popupTableMenu->addSeparator();
    popupTableMenu->addAction(ui->actionEditCopyCreateStatement);
    popupTableMenu->addAction(ui->actionExportCsvPopup);

    popupSaveSqlFileMenu = new QMenu(this);
    popupSaveSqlFileMenu->addAction(ui->actionSqlSaveFile);
    popupSaveSqlFileMenu->addAction(ui->actionSqlSaveFileAs);
    ui->actionSqlSaveFilePopup->setMenu(popupSaveSqlFileMenu);

    popupBrowseDataHeaderMenu = new QMenu(this);
    popupBrowseDataHeaderMenu->addAction(ui->actionShowRowidColumn);
    popupBrowseDataHeaderMenu->addAction(ui->actionBrowseTableEditDisplayFormat);
    popupBrowseDataHeaderMenu->addAction(ui->actionSetTableEncoding);
    popupBrowseDataHeaderMenu->addSeparator();
    popupBrowseDataHeaderMenu->addAction(ui->actionSetAllTablesEncoding);

    // Add menu item for log dock
    ui->viewMenu->insertAction(ui->viewDBToolbarAction, ui->dockLog->toggleViewAction());
    ui->viewMenu->actions().at(0)->setShortcut(QKeySequence(tr("Ctrl+L")));
    ui->viewMenu->actions().at(0)->setIcon(QIcon(":/icons/log_dock"));
    ui->viewDBToolbarAction->setChecked(!ui->toolbarDB->isHidden());

    // Add menu item for plot dock
    ui->viewMenu->insertAction(ui->viewDBToolbarAction, ui->dockPlot->toggleViewAction());
    QList<QKeySequence> plotkeyseqlist;
    plotkeyseqlist << QKeySequence(tr("Ctrl+P")) << QKeySequence(tr("Ctrl+D"));
    ui->viewMenu->actions().at(1)->setShortcuts(plotkeyseqlist);
    ui->viewMenu->actions().at(1)->setIcon(QIcon(":/icons/log_dock"));

    // Add menu item for schema dock
    ui->viewMenu->insertAction(ui->viewDBToolbarAction, ui->dockSchema->toggleViewAction());
    ui->viewMenu->actions().at(2)->setShortcut(QKeySequence(tr("Ctrl+I")));
    ui->viewMenu->actions().at(2)->setIcon(QIcon(":/icons/log_dock"));

    // Add menu item for edit dock
    ui->viewMenu->insertAction(ui->viewDBToolbarAction, ui->dockEditWindow->toggleViewAction());
    ui->viewMenu->actions().at(3)->setIcon(QIcon(":/icons/log_dock"));

    // Set statusbar fields
    statusEncryptionLabel = new QLabel(ui->statusbar);
    statusEncryptionLabel->setEnabled(false);
    statusEncryptionLabel->setVisible(false);
    statusEncryptionLabel->setText(tr("Encrypted"));
    statusEncryptionLabel->setToolTip(tr("Database is encrypted using SQLCipher"));
    ui->statusbar->addPermanentWidget(statusEncryptionLabel);

    statusReadOnlyLabel = new QLabel(ui->statusbar);
    statusReadOnlyLabel->setEnabled(false);
    statusReadOnlyLabel->setVisible(false);
    statusReadOnlyLabel->setText(tr("Read only"));
    statusReadOnlyLabel->setToolTip(tr("Database file is read only. Editing the database is disabled."));
    ui->statusbar->addPermanentWidget(statusReadOnlyLabel);

    statusEncodingLabel = new QLabel(ui->statusbar);
    statusEncodingLabel->setEnabled(false);
    statusEncodingLabel->setText("UTF-8");
    statusEncodingLabel->setToolTip(tr("Database encoding"));
    ui->statusbar->addPermanentWidget(statusEncodingLabel);

    // Connect some more signals and slots
    connect(ui->dataTable->filterHeader(), SIGNAL(sectionClicked(int)), this, SLOT(browseTableHeaderClicked(int)));
    connect(ui->dataTable->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setRecordsetLabel()));
    connect(ui->dataTable->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(updateBrowseDataColumnWidth(int,int,int)));
    connect(editWin, SIGNAL(goingAway()), this, SLOT(editWinAway()));
    connect(editWin, SIGNAL(updateRecordText(int, int, bool, QByteArray)), this, SLOT(updateRecordText(int, int, bool, QByteArray)));
    connect(editDock, SIGNAL(goingAway()), this, SLOT(editWinAway()));
    connect(editDock, SIGNAL(updateRecordText(int, int, QByteArray)), this, SLOT(updateRecordText(int, int, QByteArray)));
    connect(ui->dbTreeWidget->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(changeTreeSelection()));
    connect(ui->dataTable->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showDataColumnPopupMenu(QPoint)));

    // Load window settings
    tabifyDockWidget(ui->dockLog, ui->dockPlot);
    tabifyDockWidget(ui->dockLog, ui->dockSchema);
    restoreGeometry(PreferencesDialog::getSettingsValue("MainWindow", "geometry").toByteArray());
    restoreState(PreferencesDialog::getSettingsValue("MainWindow", "windowState").toByteArray());
    ui->comboLogSubmittedBy->setCurrentIndex(ui->comboLogSubmittedBy->findText(PreferencesDialog::getSettingsValue("SQLLogDock", "Log").toString()));
    ui->splitterForPlot->restoreState(PreferencesDialog::getSettingsValue("PlotDock", "splitterSize").toByteArray());
    ui->comboLineType->setCurrentIndex(PreferencesDialog::getSettingsValue("PlotDock", "lineType").toInt());
    ui->comboPointShape->setCurrentIndex(PreferencesDialog::getSettingsValue("PlotDock", "pointShape").toInt());

    // plot widgets
    ui->treePlotColumns->setSelectionMode(QAbstractItemView::NoSelection);

    // Set other window settings
    setAcceptDrops(true);
    setWindowTitle(QApplication::applicationName());

    // Load all settings
    reloadSettings();

#ifdef CHECKNEWVERSION
    // Check for a new version if automatic update check aren't disabled in the settings dialog
    if(PreferencesDialog::getSettingsValue("checkversion", "enabled").toBool())
    {
        // Check for a new release version, usually only enabled on windows
        m_NetworkManager = new QNetworkAccessManager(this);
        connect(m_NetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpresponse(QNetworkReply*)));

        QUrl url("https://raw.githubusercontent.com/sqlitebrowser/sqlitebrowser/master/currentrelease");
        m_NetworkManager->get(QNetworkRequest(url));
    }
#endif

#ifndef ENABLE_SQLCIPHER
    // Only show encryption menu action when SQLCipher support is enabled
    ui->actionEncryption->setVisible(false);
#endif
}

bool MainWindow::fileOpen(const QString& fileName, bool dontAddToRecentFiles)
{
    bool retval = false;

    QString wFile = fileName;
    if (!QFile::exists(wFile))
    {
        wFile = FileDialog::getOpenFileName(
                    this,
                    tr("Choose a database file")
#ifndef Q_OS_MAC // Filters on OS X are buggy
                    , FileDialog::getSqlDatabaseFileFilter()
#endif
                    );
    }
    if(QFile::exists(wFile) )
    {
        // Close the database. If the user didn't want to close it, though, stop here
        if(!fileClose())
            return false;

        // Try opening it as a project file first
        if(loadProject(wFile))
        {
            retval = true;
        } else {
            // No project file; so it should be a database file
            if(db.open(wFile))
            {
                statusEncodingLabel->setText(db.getPragma("encoding"));
                statusEncryptionLabel->setVisible(db.encrypted());
                statusReadOnlyLabel->setVisible(db.readOnly());
                setCurrentFile(wFile);
                if(!dontAddToRecentFiles)
                    addToRecentFilesMenu(wFile);
                openSqlTab(true);
                loadExtensionsFromSettings();
                populateStructure();
                resetBrowser();
                if(ui->mainTab->currentIndex() == 2)
                    loadPragmas();
                retval = true;
            } else {
                QMessageBox::warning(this, qApp->applicationName(), tr("Invalid file format."));
                return false;
            }
        }
    }

    return retval;
}

void MainWindow::fileNew()
{
    QString fileName = FileDialog::getSaveFileName(this,
                                                   tr("Choose a filename to save under"),
                                                   FileDialog::getSqlDatabaseFileFilter());
    if(!fileName.isEmpty())
    {
        if(QFile::exists(fileName))
            QFile::remove(fileName);
        db.create(fileName);
        setCurrentFile(fileName);
        addToRecentFilesMenu(fileName);
        statusEncodingLabel->setText(db.getPragma("encoding"));
        statusEncryptionLabel->setVisible(false);
        statusReadOnlyLabel->setVisible(false);
        loadExtensionsFromSettings();
        populateStructure();
        resetBrowser();
        openSqlTab(true);
        createTable();
    }
}

void MainWindow::populateStructure()
{
    // Refresh the structure tab
    db.updateSchema();
    dbStructureModel->reloadData(&db);
    ui->dbTreeWidget->expandToDepth(0);
    ui->treeSchemaDock->expandToDepth(0);

    if(!db.isOpen())
        return;

    // Update table and column names for syntax highlighting
    objectMap tab = db.getBrowsableObjects();
    SqlUiLexer::TablesAndColumnsMap tablesToColumnsMap;
    for(objectMap::ConstIterator it=tab.begin(); it!=tab.end(); ++it)
    {
        // If it is a table or a view add the fields
        if((*it).gettype() == "table" || (*it).gettype() == "view")
        {
            QString objectname = it.value().getname();

            for(int i=0; i < (*it).table.fields().size(); ++i)
            {
                QString fieldname = (*it).table.fields().at(i)->name();
                tablesToColumnsMap[objectname].append(fieldname);
            }
        }
    }
    SqlTextEdit::sqlLexer->setTableNames(tablesToColumnsMap);
    ui->editLogApplication->reloadKeywords();
    ui->editLogUser->reloadKeywords();
    for(int i=0;i<ui->tabSqlAreas->count();i++)
        qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i))->getEditor()->reloadKeywords();

    // Resize SQL column to fit contents
    ui->dbTreeWidget->resizeColumnToContents(3);
}

void MainWindow::populateTable(const QString& tablename)
{
    // Remove the model-view link if the table name is empty in order to remove any data from the view
    if(ui->comboBrowseTable->model()->rowCount() == 0 && tablename.isEmpty())
    {
        ui->dataTable->setModel(0);
        if(qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader()))
            qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader())->generateFilters(0);
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Set model
    bool reconnectSelectionSignals = false;
    if(ui->dataTable->model() == 0)
        reconnectSelectionSignals = true;
    ui->dataTable->setModel(m_browseTableModel);
    if(reconnectSelectionSignals)
        connect(ui->dataTable->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(dataTableSelectionChanged(QModelIndex)));

    // Search stored table settings for this table
    QMap<QString, BrowseDataTableSettings>::ConstIterator tableIt;
    tableIt = browseTableSettings.constFind(tablename);
    bool storedDataFound = tableIt != browseTableSettings.constEnd();

    // Set new table
    if(!storedDataFound)
    {
        m_browseTableModel->setTable(tablename);
    } else {
        QVector<QString> v;
        bool only_defaults = true;
        for(int i=0;i<db.getObjectByName(tablename).table.fields().size();i++)
        {
            QString format = tableIt.value().displayFormats[i+1];
            if(format.size())
            {
                v.push_back(format);
                only_defaults = false;
            } else {
                v.push_back(sqlb::escapeIdentifier(db.getObjectByName(tablename).table.fields().at(i)->name()));
            }
        }
        if(only_defaults)
            m_browseTableModel->setTable(tablename);
        else
            m_browseTableModel->setTable(tablename, v);
    }

    // Restore table settings
    if(storedDataFound)
    {
        // There is information stored for this table, so extract it and apply it

        // Show rowid column. Needs to be done before the column widths setting because of the workaround in there and before the filter setting
        // because of the filter row generation.
        showRowidColumn(tableIt.value().showRowid);

        // Column widths
        for(QMap<int, int>::ConstIterator widthIt=tableIt.value().columnWidths.constBegin();widthIt!=tableIt.value().columnWidths.constEnd();++widthIt)
            ui->dataTable->setColumnWidth(widthIt.key(), widthIt.value());

        // Sorting
        m_browseTableModel->sort(tableIt.value().sortOrderIndex, tableIt.value().sortOrderMode);
        ui->dataTable->filterHeader()->setSortIndicator(tableIt.value().sortOrderIndex, tableIt.value().sortOrderMode);

        // Filters
        FilterTableHeader* filterHeader = qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader());
        for(QMap<int, QString>::ConstIterator filterIt=tableIt.value().filterValues.constBegin();filterIt!=tableIt.value().filterValues.constEnd();++filterIt)
            filterHeader->setFilter(filterIt.key(), filterIt.value());

        // Encoding
        m_browseTableModel->setEncoding(tableIt.value().encoding);
    } else {
        // There aren't any information stored for this table yet, so use some default values

        // Hide rowid column. Needs to be done before the column widths setting because of the workaround in there
        showRowidColumn(false);

        // Column widths
        for(int i=1;i<m_browseTableModel->columnCount();i++)
            ui->dataTable->setColumnWidth(i, ui->dataTable->horizontalHeader()->defaultSectionSize());

        // Sorting
        m_browseTableModel->sort(0, Qt::AscendingOrder);
        ui->dataTable->filterHeader()->setSortIndicator(0, Qt::AscendingOrder);

        // Encoding
        m_browseTableModel->setEncoding(defaultBrowseTableEncoding);

        // The filters can be left empty as they are
    }

    // Activate the add and delete record buttons and editing only if a table has been selected
    bool editable = db.getObjectByName(tablename).gettype() == "table" && !db.readOnly();
    ui->buttonNewRecord->setEnabled(editable);
    ui->buttonDeleteRecord->setEnabled(editable);
    ui->dataTable->setEditTriggers(editable ? QAbstractItemView::SelectedClicked | QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed : QAbstractItemView::NoEditTriggers);

    // Set the recordset label
    setRecordsetLabel();

    // Reset the edit dialog
    editWin->reset();
    editDock->reset();

    // update plot
    updatePlot(m_browseTableModel);

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

    // Finally fill the combobox in sorted order
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
    populateTable(ui->comboBrowseTable->currentText());
}

bool MainWindow::fileClose()
{
    // Close the database but stop the closing process here if the user pressed the cancel button in there
    if(!db.close())
        return false;

    setWindowTitle(QApplication::applicationName());
    resetBrowser();
    populateStructure();
    loadPragmas();
    statusEncryptionLabel->setVisible(false);
    statusReadOnlyLabel->setVisible(false);

    // Delete the model for the Browse tab and create a new one
    delete m_browseTableModel;
    m_browseTableModel = new SqliteTableModel(this, &db, PreferencesDialog::getSettingsValue("db", "prefetchsize").toInt());
    connect(ui->dataTable->filterHeader(), SIGNAL(filterChanged(int,QString)), this, SLOT(updateFilter(int,QString)));
    connect(m_browseTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(dataTableSelectionChanged(QModelIndex)));

    // Reset the edit dialog/dock
    editDock->reset();
    editWin->reset();

    // Remove all stored table information browse data tab
    browseTableSettings.clear();
    defaultBrowseTableEncoding = QString();

    // Manually update the recordset label inside the Browse tab now
    setRecordsetLabel();

    // Reset the plot dock model
    updatePlot(0);

    activateFields(false);
    ui->buttonLogClear->click();
    for(int i=ui->tabSqlAreas->count()-1;i>=0;i--)
        closeSqlTab(i, true);

    return true;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    if(db.close())
    {
        PreferencesDialog::setSettingsValue("MainWindow", "geometry", saveGeometry());
        PreferencesDialog::setSettingsValue("MainWindow", "windowState", saveState());
        PreferencesDialog::setSettingsValue("SQLLogDock", "Log", ui->comboLogSubmittedBy->currentText());
        PreferencesDialog::setSettingsValue("PlotDock", "splitterSize", ui->splitterForPlot->saveState());
        PreferencesDialog::setSettingsValue("PlotDock", "lineType", ui->comboLineType->currentIndex());
        PreferencesDialog::setSettingsValue("PlotDock", "pointShape", ui->comboPointShape->currentIndex());
        QMainWindow::closeEvent(event);
    } else {
        event->ignore();
    }
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
    if(ui->dataTable->selectionModel()->hasSelection())
    {
        int old_row = ui->dataTable->currentIndex().row();
        while(ui->dataTable->selectionModel()->hasSelection())
        {
            if(!m_browseTableModel->removeRow(ui->dataTable->selectionModel()->selectedIndexes().first().row()))
            {
                QMessageBox::warning(this, QApplication::applicationName(), tr("Error deleting record:\n%1").arg(db.lastErrorMessage));
                break;
            }
        }
        populateTable(ui->comboBrowseTable->currentText());
        if(old_row > m_browseTableModel->totalRowCount())
            old_row = m_browseTableModel->totalRowCount();
        selectTableLine(old_row);
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

void MainWindow::navigateBegin()
{
    selectTableLine(0);
}

void MainWindow::navigateEnd()
{
    selectTableLine(m_browseTableModel->totalRowCount()-1);
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

    EditTableDialog dialog(&db, "", true, this);
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
    // Get name and type of object to delete
    QString table = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 0)).toString();
    QString type = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 1)).toString();

    // Ask user if he really wants to delete that table
    if(QMessageBox::warning(this, QApplication::applicationName(), tr("Are you sure you want to delete the %1 '%2'?\nAll data associated with the %1 will be lost.").arg(type).arg(table),
                            QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape) == QMessageBox::Yes)
    {
        // Delete the table
        QString statement = QString("DROP %1 %2;").arg(type.toUpper()).arg(sqlb::escapeIdentifier(table));
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
    QString tableToEdit = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 0)).toString();

    EditTableDialog dialog(&db, tableToEdit, false, this);
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

void MainWindow::updateRecordText(int row, int col, bool isBlob, const QByteArray& newtext)
{
    m_browseTableModel->setTypedData(m_browseTableModel->index(row, col), isBlob, newtext);
}

void MainWindow::editWinAway()
{
    // Get the sender
    EditDialog* sendingEditDialog = qobject_cast<EditDialog*>(sender());

    // Only hide the edit window, not the edit dock
    editWin->hide();

    // Update main window
    activateWindow();
    ui->dataTable->setFocus();
    ui->dataTable->setCurrentIndex(ui->dataTable->currentIndex().sibling(sendingEditDialog->getCurrentRow(), sendingEditDialog->getCurrentCol()));
}

void MainWindow::doubleClickTable(const QModelIndex& index)
{
    // Cancel on invalid index
    if(!index.isValid())
        return;

    // Don't allow editing of other objects than tables
    bool allowEditing = db.getObjectByName(ui->comboBrowseTable->currentText()).gettype() == "table";
    editDock->allowEditing(allowEditing);
    editWin->allowEditing(allowEditing);

    // Load the current value into both, edit window and edit dock
    editWin->loadText(index.data(Qt::EditRole).toByteArray(), index.row(), index.column());
    editDock->loadText(index.data(Qt::EditRole).toByteArray(), index.row(), index.column());

    // If the edit dock is visible don't open the edit window. If it's invisible open the edit window.
    // The edit dock obviously doesn't need to be opened when it's already visible but setting focus to it makes sense.
    if(!ui->dockEditWindow->isVisible())
        editWin->show();
    else
        editDock->setFocus();
}

void MainWindow::dataTableSelectionChanged(const QModelIndex& index)
{
    // Cancel on invalid index
    if(!index.isValid())
        return;

    // Don't allow editing of other objects than tables
    editDock->allowEditing(db.getObjectByName(ui->comboBrowseTable->currentText()).gettype() == "table");

    // Load the current value into the edit dock only
    editDock->loadText(index.data(Qt::EditRole).toByteArray(), index.row(), index.column());
}

/*
 * I'm still not happy how the results are represented to the user
 * right now you only see the result of the last executed statement.
 * A better experience would be tabs on the bottom with query results
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
    int execution_start_line = 0;
    int execution_start_index = 0;
    if(sender()->objectName() == "actionSqlExecuteLine")
    {
        int cursor_line, cursor_index;
        sqlWidget->getEditor()->getCursorPosition(&cursor_line, &cursor_index);
        execution_start_line = cursor_line;
        while(cursor_line < sqlWidget->getEditor()->lines())
            query += sqlWidget->getEditor()->text(cursor_line++);
        singleStep = true;
    } else {
        // if a part of the query is selected, we will only execute this part
        query = sqlWidget->getSelectedSql();
        int dummy;
        if(query.isEmpty())
            query = sqlWidget->getSql();
        else
            sqlWidget->getEditor()->getSelection(&execution_start_line, &execution_start_index, &dummy, &dummy);
    }
    if (query.isEmpty())
        return;

    //log the query
    db.logSQL(query, kLogMsg_User);
    sqlite3_stmt *vm;
    QByteArray utf8Query = query.toUtf8();
    const char *tail = utf8Query.data();
    int sql3status = 0;
    int tail_length = utf8Query.length();
    QString statusMessage;
    bool modified = false;
    bool wasdirty = db.getDirty();

    // there is no choice, we have to start a transaction before
    // we create the prepared statement, otherwise every executed
    // statement will get committed after the prepared statement
    // gets finalized, see http://www.sqlite.org/lang_transaction.html
    db.setSavepoint();

    // Remove any error indicators
    sqlWidget->getEditor()->clearErrorIndicators();

    //Accept multi-line queries, by looping until the tail is empty
    QElapsedTimer timer;
    timer.start();
    do
    {
        int tail_length_before = tail_length;
        const char* qbegin = tail;
        sql3status = sqlite3_prepare_v2(db._db,tail, tail_length, &vm, &tail);
        QString queryPart = QString::fromUtf8(qbegin, tail - qbegin);
        tail_length -= (tail - qbegin);
        int execution_end_index = execution_start_index + tail_length_before - tail_length;

        if (sql3status == SQLITE_OK)
        {
            sql3status = sqlite3_step(vm);
            sqlite3_finalize(vm);

            // SQLite returns SQLITE_DONE when a valid SELECT statement was executed but returned no results. To run into the branch that updates
            // the status message and the table view anyway manipulate the status value here. This is also done for PRAGMA statements as they (sometimes)
            // return rows just like SELECT statements, too.
            if((queryPart.trimmed().startsWith("SELECT", Qt::CaseInsensitive) ||
               queryPart.trimmed().startsWith("PRAGMA", Qt::CaseInsensitive)) && sql3status == SQLITE_DONE)
                sql3status = SQLITE_ROW;

            switch(sql3status)
            {
            case SQLITE_ROW:
            {
                sqlWidget->getModel()->setQuery(queryPart);
                if(sqlWidget->getModel()->valid())
                {
                    // The query takes the last placeholder as it may itself contain the sequence '%' + number
                    statusMessage = tr("%1 rows returned in %2ms from: %3").arg(
                                sqlWidget->getModel()->totalRowCount()).arg(timer.elapsed()).arg(queryPart.trimmed());
                    sqlWidget->enableSaveButton(true);
                    sql3status = SQLITE_OK;
                }
                else
                {
                    statusMessage = tr("Error executing query: %1").arg(queryPart);
                    sql3status = SQLITE_ERROR;
                }
            }
            case SQLITE_DONE:
            case SQLITE_OK:
            {
                if( !queryPart.trimmed().startsWith("SELECT", Qt::CaseInsensitive) )
                {
                    modified = true;

                    QString stmtHasChangedDatabase;
                    if(queryPart.trimmed().startsWith("INSERT", Qt::CaseInsensitive) ||
                            queryPart.trimmed().startsWith("UPDATE", Qt::CaseInsensitive) ||
                            queryPart.trimmed().startsWith("DELETE", Qt::CaseInsensitive))
                    {
                        stmtHasChangedDatabase = tr(", %1 rows affected").arg(sqlite3_changes(db._db));
                    }

                    statusMessage = tr("Query executed successfully: %1 (took %2ms%3)").arg(queryPart.trimmed()).arg(timer.elapsed()).arg(stmtHasChangedDatabase);
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
            sqlWidget->getEditor()->setErrorIndicator(execution_start_line, execution_start_index, execution_start_line, execution_end_index);
        }

        execution_start_index = execution_end_index;
    } while( tail && *tail != 0 && (sql3status == SQLITE_OK || sql3status == SQLITE_DONE));
    sqlWidget->finishExecution(statusMessage);
    updatePlot(sqlWidget->getModel());

    if(!modified && !wasdirty)
        db.revertToSavepoint(); // better rollback, if the logic is not enough we can tune it.
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
    QString wFile = FileDialog::getOpenFileName(
                this,
                tr("Choose a text file"),
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
    // Get the current table name if we are in the Browse Data tab
    QString current_table;
    if(ui->mainTab->currentIndex() == 0)
        current_table = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 0)).toString();
    else if(ui->mainTab->currentIndex() == 1)
        current_table = ui->comboBrowseTable->currentText();

    // Open dialog
    ExportCsvDialog dialog(&db, this, "", current_table);
    dialog.exec();
}

void MainWindow::dbState( bool dirty )
{
    ui->fileSaveAction->setEnabled(dirty);
    ui->fileRevertAction->setEnabled(dirty);
    ui->fileAttachAction->setEnabled(!dirty);
    //ui->actionEncryption->setEnabled(!dirty);
}

void MainWindow::fileSave()
{
    if(db.isOpen())
        db.releaseAllSavepoints();
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
    QString current_table;
    if(ui->mainTab->currentIndex() == 1)
        current_table = ui->comboBrowseTable->currentText();

    ExportSqlDialog dialog(&db, this, current_table);
    dialog.exec();
}

void MainWindow::importDatabaseFromSQL()
{
    // Get file name to import
    QString fileName = FileDialog::getOpenFileName(
                this,
                tr("Choose a file to import"),
                tr("Text files(*.sql *.txt);;All files(*)"));

    // Cancel when file doesn't exist
    if(!QFile::exists(fileName))
        return;

    // If there is already a database file opened ask the user whether to import into this one or a new one. If no DB is opened just ask for a DB name directly
    QString newDbFile;
    if((db.isOpen() && QMessageBox::question(this,
                                            QApplication::applicationName(),
                                            tr("Do you want to create a new database file to hold the imported data?\n"
                                               "If you answer no we will attempt to import the data in the SQL file to the current database."),
                                            QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) || !db.isOpen())
    {
        newDbFile = FileDialog::getSaveFileName(
                    this,
                    tr("Choose a filename to save under"),
                    FileDialog::getSqlDatabaseFileFilter());
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

    // Refresh window when importing into an existing DB or - when creating a new file - just open it correctly
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

//** Db Tree Context Menu
void MainWindow::createTreeContextMenu(const QPoint &qPoint)
{
    if(!ui->dbTreeWidget->selectionModel()->hasSelection())
        return;

    QString type = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 1)).toString();

    if(type == "table" || type == "view" || type == "trigger" || type == "index")
        popupTableMenu->exec(ui->dbTreeWidget->mapToGlobal(qPoint));
}
//** Tree selection changed
void MainWindow::changeTreeSelection()
{
    // Just assume first that something's selected that can not be edited at all
    ui->editDeleteObjectAction->setEnabled(false);
    ui->editModifyTableAction->setEnabled(false);
    ui->actionEditBrowseTable->setEnabled(false);

    if(!ui->dbTreeWidget->currentIndex().isValid())
        return;

    // Change the text of the actions
    QString type = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 1)).toString();
    ui->editDeleteObjectAction->setIcon(QIcon(QString(":icons/%1_delete").arg(type)));
    if(type == "view")
        ui->editDeleteObjectAction->setText(tr("Delete View"));
    else if(type == "trigger")
        ui->editDeleteObjectAction->setText(tr("Delete Trigger"));
    else if(type == "index")
        ui->editDeleteObjectAction->setText(tr("Delete Index"));
    else
        ui->editDeleteObjectAction->setText(tr("Delete Table"));

    // Activate actions
    if(type == "table")
    {
        ui->editDeleteObjectAction->setEnabled(!db.readOnly());
        ui->editModifyTableAction->setEnabled(!db.readOnly());
    } else if(type == "view" || type == "trigger" || type == "index") {
        ui->editDeleteObjectAction->setEnabled(!db.readOnly());
    }
    if(type == "table" || type == "view")
    {
        ui->actionEditBrowseTable->setEnabled(!db.readOnly());
        ui->actionExportCsvPopup->setEnabled(true);
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
    // Get recent files list from settings
    QStringList files = PreferencesDialog::getSettingsValue("General", "recentFileList").toStringList();

    // Check if files still exist and remove any non-existant file
    for(int i=0;i<files.size();i++)
    {
        QFileInfo fi(files.at(i));
        if(!fi.exists())
        {
            files.removeAt(i);
            i--;
        }
    }

    // Store updated list
    PreferencesDialog::setSettingsValue("General", "recentFileList", files);

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(files[i]);
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);

        // Add shortcut for opening the file using the keyboard. However, if the application is configured to store
        // more than nine recently opened files don't set shortcuts for the later ones which wouldn't be single digit anymore.
        if(i < 9)
            recentFileActs[i]->setShortcut(QKeySequence(Qt::CTRL + (Qt::Key_1+i)));
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
}

void MainWindow::addToRecentFilesMenu(const QString& filename)
{
    QStringList files = PreferencesDialog::getSettingsValue("General", "recentFileList").toStringList();
    files.removeAll(filename);
    files.prepend(filename);
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
    bool write = !db.readOnly();

    ui->fileCloseAction->setEnabled(enable);
    ui->fileCompactAction->setEnabled(enable && write);
    ui->fileExportCSVAction->setEnabled(enable);
    ui->fileExportSQLAction->setEnabled(enable);
    ui->fileImportCSVAction->setEnabled(enable && write);
    ui->editCreateTableAction->setEnabled(enable && write);
    ui->editCreateIndexAction->setEnabled(enable && write);
    ui->buttonNext->setEnabled(enable);
    ui->buttonPrevious->setEnabled(enable);
    ui->buttonBegin->setEnabled(enable);
    ui->buttonEnd->setEnabled(enable);
    ui->scrollAreaWidgetContents->setEnabled(enable);
    ui->buttonBoxPragmas->setEnabled(enable && write);
    ui->buttonGoto->setEnabled(enable);
    ui->editGoto->setEnabled(enable);
    ui->buttonRefresh->setEnabled(enable);
    ui->buttonDeleteRecord->setEnabled(enable && write);
    ui->buttonNewRecord->setEnabled(enable && write);
    ui->actionExecuteSql->setEnabled(enable);
    ui->actionLoadExtension->setEnabled(enable);
    ui->actionSqlExecuteLine->setEnabled(enable);
    ui->actionSqlOpenFile->setEnabled(enable);
    ui->actionSqlOpenTab->setEnabled(enable);
    ui->actionSqlSaveFilePopup->setEnabled(enable);
    ui->actionSqlSaveFileAs->setEnabled(enable);
    ui->actionSqlSaveFile->setEnabled(enable);
    ui->actionSaveProject->setEnabled(enable);
    ui->actionEncryption->setEnabled(enable && write);
    ui->buttonClearFilters->setEnabled(enable);
    ui->dockEditWindow->setEnabled(enable && write);
}

void MainWindow::browseTableHeaderClicked(int logicalindex)
{
    // Abort if there is more than one column selected because this tells us that the user pretty sure wants to do a range selection instead of sorting data
    if(ui->dataTable->selectionModel()->selectedColumns().count() > 1)
        return;

    // instead of the column name we just use the column index, +2 because 'rowid, *' is the projection
    BrowseDataTableSettings& settings = browseTableSettings[ui->comboBrowseTable->currentText()];
    settings.sortOrderIndex = logicalindex;
    settings.sortOrderMode = settings.sortOrderMode == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder;
    ui->dataTable->sortByColumn(settings.sortOrderIndex, settings.sortOrderMode);

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
        ui->editLogUser->append(sql + "\n");
        ui->editLogUser->verticalScrollBar()->setValue(ui->editLogUser->verticalScrollBar()->maximum());
    } else {
        ui->editLogApplication->append(sql + "\n");
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
    int index = ui->tabSqlAreas->addTab(w, QString("SQL %1").arg(++tabNumber));
    ui->tabSqlAreas->setCurrentIndex(index);
    w->getEditor()->setFocus();

    return index;
}

void MainWindow::openSqlFile()
{
    QString file = FileDialog::getOpenFileName(
                this,
                tr("Select SQL file to open"),
                tr("Text files(*.sql *.txt);;All files(*)"));

    if(QFile::exists(file))
    {
        QFile f(file);
        f.open(QIODevice::ReadOnly);

        // Decide whether to open a new tab or take the current one
        unsigned int index;
        SqlExecutionArea* current_tab = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());
        if(current_tab->getSql().isEmpty() && current_tab->getModel()->rowCount() == 0)
            index = ui->tabSqlAreas->currentIndex();
        else
            index = openSqlTab();

        SqlExecutionArea* sqlarea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(index));
        sqlarea->getEditor()->setText(f.readAll());
        sqlarea->setFileName(file);
        QFileInfo fileinfo(file);
        ui->tabSqlAreas->setTabText(index, fileinfo.fileName());
    }
}

void MainWindow::saveSqlFile()
{
    SqlExecutionArea* sqlarea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());

    // If this SQL file hasn't been saved before open the Save As dialog. Otherwise just use the old file name for saving
    if(sqlarea->fileName().isEmpty())
    {
        saveSqlFileAs();
    } else {
        QFile f(sqlarea->fileName());
        f.open(QIODevice::WriteOnly);
        f.write(sqlarea->getSql().toUtf8());

        QFileInfo fileinfo(sqlarea->fileName());
        ui->tabSqlAreas->setTabText(ui->tabSqlAreas->currentIndex(), fileinfo.fileName());
    }
}

void MainWindow::saveSqlFileAs()
{
    QString file = FileDialog::getSaveFileName(
                this,
                tr("Select file name"),
                tr("Text files(*.sql *.txt);;All files(*)"));

    if(!file.isEmpty())
    {
        // Just set the selected file name and call the standard save action which is going to use it
        qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget())->setFileName(file);
        saveSqlFile();
    }
}

void MainWindow::loadExtension()
{
    QString file = FileDialog::getOpenFileName(
                this,
                tr("Select extension file"),
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
    // Read settings
    int prefetch_size = PreferencesDialog::getSettingsValue("db", "prefetchsize").toInt();
    int log_fontsize = PreferencesDialog::getSettingsValue("log", "fontsize").toInt();

    QFont logfont("Monospace");
    logfont.setStyleHint(QFont::TypeWriter);
    logfont.setPointSize(log_fontsize);

    // Set data browser font
    QFont dataBrowserFont(PreferencesDialog::getSettingsValue("databrowser", "font").toString());
    dataBrowserFont.setPointSize(PreferencesDialog::getSettingsValue("databrowser", "fontsize").toInt());
    ui->dataTable->setFont(dataBrowserFont);

    // Set prefetch sizes for lazy population of table models
    m_browseTableModel->setChunkSize(prefetch_size);
    for(int i=0; i < ui->tabSqlAreas->count(); ++i)
    {
        SqlExecutionArea* sqlArea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i));
        sqlArea->reloadSettings();
        sqlArea->getModel()->setChunkSize(prefetch_size);
        sqlArea->getResultView()->setFont(logfont);
    }

    // Set font for SQL logs
    ui->editLogApplication->reloadSettings();
    ui->editLogUser->reloadSettings();
    ui->editLogApplication->setFont(logfont);
    ui->editLogUser->setFont(logfont);

    // Load extensions
    loadExtensionsFromSettings();

    // Refresh view
    populateStructure();
    resetBrowser();
}

void MainWindow::httpresponse(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        // Check for redirect
        QVariant possibleRedirectUrl =
                     reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

        if(!possibleRedirectUrl.toUrl().isEmpty())
        {
            if(reply->url() == possibleRedirectUrl.toUrl())
                return; // escape possible redirect loop
            m_NetworkManager->get(QNetworkRequest(possibleRedirectUrl.toUrl()));
            return;
        }

        // first line of the currentrelease file contains a major.minor.patch version string
        QString sversion(reply->readLine());

        QStringList versiontokens = sversion.split(".");
        if(versiontokens.size() < 3)
            return;

        int major = versiontokens[0].toInt();
        int minor = versiontokens[1].toInt();
        int patch = versiontokens[2].toInt();

        bool newversion = false;
        if(major > MAJOR_VERSION)
            newversion = true;
        else if(major == MAJOR_VERSION)
        {
            if(minor > MINOR_VERSION)
                newversion = true;
            else if(minor == MINOR_VERSION)
            {
                if(patch > PATCH_VERSION)
                    newversion = true;
            }
        }

        if(newversion)
        {
            QSettings settings(QApplication::organizationName(), QApplication::organizationName());
            int ignmajor = settings.value("checkversion/ignmajor", 999).toInt();
            int ignminor = settings.value("checkversion/ignminor", 0).toInt();
            int ignpatch = settings.value("checkversion/ignpatch", 0).toInt();

            // check if the user doesn't care about the current update
            if(!(ignmajor == major && ignminor == minor && ignpatch == patch))
            {
                QMessageBox msgBox;
                QPushButton *idontcarebutton = msgBox.addButton(tr("Don't show again"), QMessageBox::ActionRole);
                msgBox.addButton(QMessageBox::Ok);
                msgBox.setTextFormat(Qt::RichText);
                msgBox.setWindowTitle(tr("New version available."));
                msgBox.setText(tr("A new DB Browser for SQLite version is available (%1.%2.%3).<br/><br/>"
                                  "Please download at <a href='%4'>%4</a>.").arg(major).arg(minor).arg(patch).
                                    arg(QString(reply->readLine()).trimmed()));
                msgBox.exec();

                if(msgBox.clickedButton() == idontcarebutton)
                {
                    // save that the user don't want to get bothered about this update
                    settings.beginGroup("checkversion");
                    settings.setValue("ignmajor", major);
                    settings.setValue("ignminor", minor);
                    settings.setValue("ignpatch", patch);
                    settings.endGroup();
                }
            }
        }
    }

    reply->deleteLater();
}

namespace {
/*!
 * \brief guessdatatype try to parse the first 10 rows and decide the datatype
 * \param model model to check the data
 * \param column index of the column to check
 * \return the guessed datatype
 */
QVariant::Type guessdatatype(SqliteTableModel* model, int column)
{
    QVariant::Type type = QVariant::Invalid;
    for(int i = 0; i < std::min(10, model->rowCount()) && type != QVariant::String; ++i)
    {
        QVariant data = model->data(model->index(i, column));
        if(data.convert(QVariant::Double))
        {
            type = QVariant::Double;
        }
        else
        {
            QString s = model->data(model->index(i, column)).toString();
            QDate d = QDate::fromString(s, Qt::ISODate);
            if(d.isValid())
                type = QVariant::DateTime;
            else
                type = QVariant::String;
        }

    }
    return type;
}
}

void MainWindow::updatePlot(SqliteTableModel *model, bool update)
{
    // add columns to x/y selection tree widget
    if(update)
    {
        // disconnect treeplotcolumns item changed updates
        disconnect(ui->treePlotColumns, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                   this,SLOT(on_treePlotColumns_itemChanged(QTreeWidgetItem*,int)));

        m_currentPlotModel = model;

        // save current selected columns, so we can restore them after the update
        QString sItemX; // selected X column
        QMap<QString, QColor> mapItemsY; // selected Y columns with color
        for(int i = 0; i < ui->treePlotColumns->topLevelItemCount(); ++i)
        {
            QTreeWidgetItem* item = ui->treePlotColumns->topLevelItem(i);
            if(item->checkState(PlotColumnX) == Qt::Checked)
                sItemX = item->text(PlotColumnField);

            if(item->checkState(PlotColumnY) == Qt::Checked)
            {
                mapItemsY[item->text(PlotColumnField)] = item->backgroundColor(PlotColumnY);
            }
        }

        ui->treePlotColumns->clear();

        if(model)
        {
            for(int i = 0; i < model->columnCount(); ++i)
            {
                QVariant::Type columntype = guessdatatype(model, i);
                if(columntype != QVariant::String && columntype != QVariant::Invalid)
                {
                    QTreeWidgetItem* columnitem = new QTreeWidgetItem(ui->treePlotColumns);
                    // maybe i make this more complicated than i should
                    // but store the model column index in the first 16 bit and the type
                    // in the other 16 bits
                    uint itemdata = 0;
                    itemdata = i << 16;
                    itemdata |= columntype;
                    columnitem->setData(PlotColumnField, Qt::UserRole, itemdata);
                    columnitem->setText(PlotColumnField, model->headerData(i, Qt::Horizontal).toString());

                    // restore previous check state
                    if(mapItemsY.contains(columnitem->text(PlotColumnField)))
                    {
                        columnitem->setCheckState(PlotColumnY, Qt::Checked);
                        columnitem->setBackgroundColor(PlotColumnY, mapItemsY[columnitem->text(PlotColumnField)]);
                    }
                    else
                        columnitem->setCheckState(PlotColumnY, Qt::Unchecked);
                    if(sItemX == columnitem->text(PlotColumnField))
                        columnitem->setCheckState(PlotColumnX, Qt::Checked);
                    else
                        columnitem->setCheckState(PlotColumnX, Qt::Unchecked);

                    ui->treePlotColumns->addTopLevelItem(columnitem);
                }
            }
        }

        ui->plotWidget->yAxis->setLabel("Y");
        ui->plotWidget->xAxis->setLabel("X");
        connect(ui->treePlotColumns, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                this,SLOT(on_treePlotColumns_itemChanged(QTreeWidgetItem*,int)));
    }

    // search for the x axis select
    QTreeWidgetItem* xitem = 0;
    for(int i = 0; i < ui->treePlotColumns->topLevelItemCount(); ++i)
    {
        xitem = ui->treePlotColumns->topLevelItem(i);
        if(xitem->checkState(PlotColumnX) == Qt::Checked)
            break;

        xitem = 0;
    }

    QStringList yAxisLabels;

    ui->plotWidget->clearGraphs();
    if(xitem)
    {
        // regain the model column index and the datatype
        // leading 16 bit are column index, the other 16 bit are the datatype
        // right now datatype is only important for X axis (date, non date)
        uint xitemdata = xitem->data(PlotColumnField, Qt::UserRole).toUInt();
        int x = xitemdata >> 16;
        int xtype = xitemdata & (uint)0xFF;


        // check if we have a x axis with datetime data
        if(xtype == QVariant::DateTime)
        {
            ui->plotWidget->xAxis->setTickLabelType(QCPAxis::ltDateTime);
            ui->plotWidget->xAxis->setDateTimeFormat("yyyy-MM-dd");
        }
        else
        {
            ui->plotWidget->xAxis->setTickLabelType(QCPAxis::ltNumber);
        }

        // add graph for each selected y axis
        for(int i = 0; i < ui->treePlotColumns->topLevelItemCount(); ++i)
        {
            QTreeWidgetItem* item = ui->treePlotColumns->topLevelItem(i);
            if(item->checkState((PlotColumnY)) == Qt::Checked)
            {
                // regain the model column index and the datatype
                // leading 16 bit are column index
                uint itemdata = item->data(0, Qt::UserRole).toUInt();
                int column = itemdata >> 16;
                QCPGraph* graph = ui->plotWidget->addGraph();

                int y = column;

                graph->setPen(QPen(item->backgroundColor(PlotColumnY)));

                // prepare the data vectors for qcustomplot
                // possible improvement might be a QVector subclass that directly
                // access the model data, to save memory, we are copying here
                QVector<double> xdata(model->rowCount()), ydata(model->rowCount());
                for(int i = 0; i < model->rowCount(); ++i)
                {
                    // convert x type axis if it's datetime
                    if(xtype == QVariant::DateTime)
                    {
                        QString s = model->data(model->index(i, x)).toString();
                        QDateTime d = QDateTime::fromString(s, Qt::ISODate);
                        xdata[i] = d.toTime_t();
                    }
                    else
                    {
                        xdata[i] = model->data(model->index(i, x)).toDouble();
                    }

                    ydata[i] = model->data(model->index(i, y)).toDouble();
                }

                // set some graph styles, this could also be improved to let the user choose
                // some styling
                graph->setData(xdata, ydata);
                graph->setLineStyle((QCPGraph::LineStyle) ui->comboLineType->currentIndex());
                // WARN: ssDot is removed
                int shapeIdx = ui->comboPointShape->currentIndex();
                if (shapeIdx > 0) shapeIdx += 1;
                graph->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)shapeIdx, 5));

                // gather Y label column names
                yAxisLabels << model->headerData(y, Qt::Horizontal).toString();
            }
        }

        ui->plotWidget->rescaleAxes(true);

        // set axis labels
        ui->plotWidget->xAxis->setLabel(model->headerData(x, Qt::Horizontal).toString());
        ui->plotWidget->yAxis->setLabel(yAxisLabels.join("|"));
    }
    ui->plotWidget->replot();
}

void MainWindow::on_treePlotColumns_itemChanged(QTreeWidgetItem *changeitem, int column)
{
    // disable change updates, or we get unwanted redrawing and weird behavior
    disconnect(ui->treePlotColumns, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this,SLOT(on_treePlotColumns_itemChanged(QTreeWidgetItem*,int)));

    // make sure only 1 X axis is selected
    if(column == PlotColumnX)
    {
        for(int i = 0; i < ui->treePlotColumns->topLevelItemCount(); ++i)
        {
            QTreeWidgetItem* item = ui->treePlotColumns->topLevelItem(i);
            if(item->checkState(column) == Qt::Checked && item != changeitem)
            {
                item->setCheckState(column, Qt::Unchecked);
            }
        }
    }
    else if(column == PlotColumnY)
    {
        if(changeitem->checkState(column) == Qt::Checked)
        {
            // get a default color
            QColor curbkcolor = changeitem->backgroundColor(column);
            QColor color = !curbkcolor.isValid() ? (Qt::GlobalColor)(qrand() % 13 + 5) : curbkcolor;
            if(color.isValid())
            {
                changeitem->setBackgroundColor(column, color);
            }
            else
            {
                changeitem->setCheckState(column, Qt::Unchecked);
            }
        }
    }

    connect(ui->treePlotColumns, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this,SLOT(on_treePlotColumns_itemChanged(QTreeWidgetItem*,int)));

    updatePlot(m_currentPlotModel, false);
}

void MainWindow::on_treePlotColumns_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // disable change updates, or we get unwanted redrawing and weird behavior
    disconnect(ui->treePlotColumns, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this,SLOT(on_treePlotColumns_itemChanged(QTreeWidgetItem*,int)));

    if(column == PlotColumnY)
    {
        // On double click open the colordialog
        QColorDialog colordialog(this);
        QColor curbkcolor = item->backgroundColor(column);
        QColor precolor = !curbkcolor.isValid() ? (Qt::GlobalColor)(qrand() % 13 + 5) : curbkcolor;
        QColor color = colordialog.getColor(precolor, this, tr("Choose a axis color"));
        if(color.isValid())
        {
            item->setCheckState(column, Qt::Checked);
            item->setBackgroundColor(column, color);
        }
        else
        {
            item->setCheckState(column, Qt::Unchecked);
        }
    }

    connect(ui->treePlotColumns, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this,SLOT(on_treePlotColumns_itemChanged(QTreeWidgetItem*,int)));

    updatePlot(m_currentPlotModel, false);
}

void MainWindow::on_butSavePlot_clicked()
{
    QString fileName = FileDialog::getSaveFileName(this,
                                                    tr("Choose a filename to save under"),
                                                    tr("PNG(*.png);;JPG(*.jpg);;PDF(*.pdf);;BMP(*.bmp);;All Files(*)")
                                                    );
    if(!fileName.isEmpty())
    {
        if(fileName.endsWith(".png", Qt::CaseInsensitive))
        {
            ui->plotWidget->savePng(fileName);
        }
        else if(fileName.endsWith(".jpg", Qt::CaseInsensitive))
        {
            ui->plotWidget->saveJpg(fileName);
        }
        else if(fileName.endsWith(".pdf", Qt::CaseInsensitive))
        {
            ui->plotWidget->savePdf(fileName);
        }
        else if(fileName.endsWith(".bmp", Qt::CaseInsensitive))
        {
            ui->plotWidget->saveBmp(fileName);
        }
        else
        {
            fileName += ".png";
            ui->plotWidget->savePng(fileName);
        }
    }
}

void MainWindow::on_actionWiki_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/sqlitebrowser/sqlitebrowser/wiki"));
}

void MainWindow::on_actionBug_report_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/sqlitebrowser/sqlitebrowser/issues/new"));
}

void MainWindow::on_actionWebsite_triggered()
{
    QDesktopServices::openUrl(QUrl("http://sqlitebrowser.org"));
}

void MainWindow::updateBrowseDataColumnWidth(int section, int /*old_size*/, int new_size)
{
    QSet<int> selectedCols(ui->dataTable->selectedCols());
    QString tableName(ui->comboBrowseTable->currentText());
    if (!selectedCols.contains(section))
    {
        browseTableSettings[tableName].columnWidths[section] = new_size;
    }
    else
    {
        ui->dataTable->blockSignals(true);
        foreach (int col, selectedCols)
        {
            ui->dataTable->setColumnWidth(col, new_size);
            browseTableSettings[tableName].columnWidths[col] = new_size;
        }
        ui->dataTable->blockSignals(false);
    }
}

bool MainWindow::loadProject(QString filename)
{
    // Show the open file dialog when no filename was passed as parameter
    if(filename.isEmpty())
    {
        filename = FileDialog::getOpenFileName(this,
                                                tr("Choose a file to open"),
                                                tr("DB Browser for SQLite project file (*.sqbpro)"));
    }

    if(!filename.isEmpty())
    {
        QFile file(filename);
        file.open(QFile::ReadOnly | QFile::Text);

        QXmlStreamReader xml(&file);
        xml.readNext();     // token == QXmlStreamReader::StartDocument
        xml.readNext();     // name == sqlb_project
        if(xml.name() != "sqlb_project")
            return false;

        addToRecentFilesMenu(filename);

        while(!xml.atEnd() && !xml.hasError())
        {
            // Read next token
            QXmlStreamReader::TokenType token = xml.readNext();

            // Handle element start
            if(token == QXmlStreamReader::StartElement)
            {
                if(xml.name() == "db")
                {
                    // DB file
                    QString dbfilename = xml.attributes().value("path").toString();
                    if(!QFile::exists(dbfilename))
                        dbfilename = QFileInfo(filename).absolutePath() + QDir::separator() + dbfilename;
                    fileOpen(dbfilename, true);
                    ui->dbTreeWidget->collapseAll();
                } else if(xml.name() == "window") {
                    // Window settings
                    while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "window")
                    {
                        // Currently selected tab
                        if(xml.name() == "current_tab")
                            ui->mainTab->setCurrentIndex(xml.attributes().value("id").toString().toInt());
                    }
                } else if(xml.name() == "tab_structure") {
                    // Database Structure tab settings
                    while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "tab_structure")
                    {
                        if(xml.name() == "column_width")
                        {
                            // Tree view column widths
                            ui->dbTreeWidget->setColumnWidth(xml.attributes().value("id").toString().toInt(),
                                                             xml.attributes().value("width").toString().toInt());
                            xml.skipCurrentElement();
                        } else if(xml.name() == "expanded_item") {
                            // Tree view expanded items
                            int parent = xml.attributes().value("parent").toString().toInt();
                            QModelIndex idx;
                            if(parent == -1)
                                idx = ui->dbTreeWidget->model()->index(xml.attributes().value("id").toString().toInt(), 0);
                            else
                                idx = ui->dbTreeWidget->model()->index(xml.attributes().value("id").toString().toInt(), 0, ui->dbTreeWidget->model()->index(parent, 0));
                            ui->dbTreeWidget->expand(idx);
                            xml.skipCurrentElement();
                        }
                    }
                } else if(xml.name() == "tab_browse") {
                    // Browse Data tab settings
                    while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "tab_browse")
                    {
                        if(xml.name() == "current_table")
                        {
                            // Currently selected table
                            ui->comboBrowseTable->setCurrentIndex(ui->comboBrowseTable->findText(xml.attributes().value("name").toString()));
                            xml.skipCurrentElement();
                        } else if(xml.name() == "default_encoding") {
                            // Default text encoding
                            defaultBrowseTableEncoding = xml.attributes().value("codec").toString();
                            xml.skipCurrentElement();
                        } else if(xml.name() == "browsetable_info") {
                            QString attrData = xml.attributes().value("data").toString();
                            QByteArray temp = QByteArray::fromBase64(attrData.toUtf8());
                            QDataStream stream(temp);
                            stream >> browseTableSettings;
                            populateTable(ui->comboBrowseTable->currentText());     // Refresh view
                            ui->dataTable->sortByColumn(browseTableSettings[ui->comboBrowseTable->currentText()].sortOrderIndex,
                                                        browseTableSettings[ui->comboBrowseTable->currentText()].sortOrderMode);
                            showRowidColumn(browseTableSettings[ui->comboBrowseTable->currentText()].showRowid);
                            xml.skipCurrentElement();
                        }
                    }
                } else if(xml.name() == "tab_sql") {
                    // Close existing tab
                    QWidget* w = ui->tabSqlAreas->widget(0);
                    ui->tabSqlAreas->removeTab(0);
                    delete w;

                    // Execute SQL tab data
                    while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "tab_sql")
                    {
                        if(xml.name() == "sql")
                        {
                            // SQL editor tab
                            unsigned int index = openSqlTab();
                            ui->tabSqlAreas->setTabText(index, xml.attributes().value("name").toString());
                            qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(index))->getEditor()->setText(xml.readElementText());
                        } else if(xml.name() == "current_tab") {
                            // Currently selected tab
                            ui->tabSqlAreas->setCurrentIndex(xml.attributes().value("id").toString().toInt());
                            xml.skipCurrentElement();
                        }
                    }
                }
            }
        }

        file.close();
        return !xml.hasError();
    } else {
        // No project was opened
        return false;
    }
}

static void saveDbTreeState(const QTreeView* tree, QXmlStreamWriter& xml, QModelIndex index = QModelIndex(), int parent_row = -1)
{
    for(int i=0;i<tree->model()->rowCount(index);i++)
    {
        if(tree->isExpanded(tree->model()->index(i, 0, index)))
        {
            xml.writeStartElement("expanded_item");
            xml.writeAttribute("id", QString::number(i));
            xml.writeAttribute("parent", QString::number(parent_row));
            xml.writeEndElement();
        }

        saveDbTreeState(tree, xml, tree->model()->index(i, 0, index), i);
    }
}

void MainWindow::saveProject()
{
    QString filename = FileDialog::getSaveFileName(this,
                                                    tr("Choose a filename to save under"),
                                                    tr("DB Browser for SQLite project file (*.sqbpro)")
                                                    );
    if(!filename.isEmpty())
    {
        // Make sure the file has got a .sqbpro ending
        if(!filename.endsWith(".sqbpro", Qt::CaseInsensitive))
            filename.append(".sqbpro");

        QFile file(filename);
        file.open(QFile::WriteOnly | QFile::Text);
        QXmlStreamWriter xml(&file);
        xml.writeStartDocument();
        xml.writeStartElement("sqlb_project");

        // Database file name
        xml.writeStartElement("db");
        xml.writeAttribute("path", db.curDBFilename);
        xml.writeEndElement();

        // Window settings
        xml.writeStartElement("window");
        xml.writeStartElement("current_tab");   // Currently selected tab
        xml.writeAttribute("id", QString::number(ui->mainTab->currentIndex()));
        xml.writeEndElement();
        xml.writeEndElement();

        // Database Structure tab settings
        xml.writeStartElement("tab_structure");
        for(int i=0;i<ui->dbTreeWidget->model()->columnCount();i++) // Widths of tree view columns
        {
            xml.writeStartElement("column_width");
            xml.writeAttribute("id", QString::number(i));
            xml.writeAttribute("width", QString::number(ui->dbTreeWidget->columnWidth(i)));
            xml.writeEndElement();
        }
        saveDbTreeState(ui->dbTreeWidget, xml);                     // Expanded tree items
        xml.writeEndElement();

        // Browse Data tab settings
        xml.writeStartElement("tab_browse");
        xml.writeStartElement("current_table");     // Currently selected table
        xml.writeAttribute("name", ui->comboBrowseTable->currentText());
        xml.writeEndElement();
        xml.writeStartElement("default_encoding");  // Default encoding for text stored in tables
        xml.writeAttribute("codec", defaultBrowseTableEncoding);
        xml.writeEndElement();
        {                                           // Table browser information
            QByteArray temp;
            QDataStream stream(&temp, QIODevice::WriteOnly);
            stream << browseTableSettings;
            xml.writeStartElement("browsetable_info");
            xml.writeAttribute("data", temp.toBase64());
            xml.writeEndElement();
        }
        xml.writeEndElement();

        // Execute SQL tab data
        xml.writeStartElement("tab_sql");
        for(int i=0;i<ui->tabSqlAreas->count();i++)                                     // All SQL tabs content
        {
            xml.writeStartElement("sql");
            xml.writeAttribute("name", ui->tabSqlAreas->tabText(i));
            xml.writeCharacters(qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i))->getSql());
            xml.writeEndElement();
        }
        xml.writeStartElement("current_tab");                                           // Currently selected tab
        xml.writeAttribute("id", QString::number(ui->tabSqlAreas->currentIndex()));
        xml.writeEndElement();
        xml.writeEndElement();

        xml.writeEndElement();
        xml.writeEndDocument();
        file.close();
        addToRecentFilesMenu(filename);
    }
}

void MainWindow::fileAttach()
{
    // Get file name of database to attach
    QString file = FileDialog::getOpenFileName(
                this,
                tr("Choose a database file"),
                FileDialog::getSqlDatabaseFileFilter());
    if(!QFile::exists(file))
        return;

    // Attach it
    db.attach(file);
}

void MainWindow::updateFilter(int column, const QString& value)
{
    m_browseTableModel->updateFilter(column, value);
    browseTableSettings[ui->comboBrowseTable->currentText()].filterValues[column] = value;
    setRecordsetLabel();
}

void MainWindow::editEncryption()
{
#ifdef ENABLE_SQLCIPHER
    CipherDialog dialog(this, true);
    if(dialog.exec())
    {
        // Show progress dialog even though we can't provide any detailed progress information but this
        // process might take some time.
        QProgressDialog progress(this);
        progress.setCancelButton(0);
        progress.setWindowModality(Qt::ApplicationModal);
        progress.show();
        qApp->processEvents();

        // Apply all unsaved changes
        bool ok = db.releaseAllSavepoints();
        qApp->processEvents();

        // Create the new file first or it won't work
        if(ok)
        {
            QFile file(db.curDBFilename + ".enctemp");
            file.open(QFile::WriteOnly);
            file.close();
        }

        // Attach a new database using the new settings
        qApp->processEvents();
        if(ok)
            ok = db.executeSQL(QString("ATTACH DATABASE '%1' AS sqlitebrowser_edit_encryption KEY '%2';").arg(db.curDBFilename + ".enctemp").arg(dialog.password()),
                               false, false);
        qApp->processEvents();
        if(ok)
            ok = db.executeSQL(QString("PRAGMA sqlitebrowser_edit_encryption.cipher_page_size = %1").arg(dialog.pageSize()), false, false);

        // Export the current database to the new one
        qApp->processEvents();
        if(ok)
            ok = db.executeSQL("SELECT sqlcipher_export('sqlitebrowser_edit_encryption');", false, false);

        // Check for errors
        qApp->processEvents();
        if(ok)
        {
            // No errors: Then close the current database, switch names, open the new one and if that succeeded delete the old one

            fileClose();
            QFile::rename(db.curDBFilename, db.curDBFilename + ".enctempold");
            QFile::rename(db.curDBFilename + ".enctemp", db.curDBFilename);
            if(fileOpen(db.curDBFilename))
                QFile::remove(db.curDBFilename + ".enctempold");
        } else {
            QMessageBox::warning(this, qApp->applicationName(), db.lastErrorMessage);
        }
    }
#endif
}

void MainWindow::switchToBrowseDataTab()
{
    if(!ui->dbTreeWidget->selectionModel()->hasSelection())
        return;

    QString tableToBrowse = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 0)).toString();

    ui->mainTab->setCurrentIndex(1);
    ui->comboBrowseTable->setCurrentIndex(ui->comboBrowseTable->findText(tableToBrowse));
    populateTable(tableToBrowse);
}

void MainWindow::on_buttonClearFilters_clicked()
{
    ui->dataTable->filterHeader()->clearFilters();
}

void MainWindow::copyCurrentCreateStatement()
{
    // Cancel if no field is currently selected
    if(!ui->dbTreeWidget->selectionModel()->hasSelection())
        return;

    // Get the CREATE statement from the Schema column
    QString stmt = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 3)).toString();

    // Copy the statement to the global application clipboard
    QApplication::clipboard()->setText(stmt);
}

void MainWindow::on_comboLineType_currentIndexChanged(int index)
{
    Q_ASSERT(index >= QCPGraph::lsNone &&
             index <= QCPGraph::lsImpulse);
    QCPGraph::LineStyle lineStyle = (QCPGraph::LineStyle) index;
    for (int i = 0, ie = ui->plotWidget->graphCount(); i < ie; ++i)
    {
        QCPGraph * graph = ui->plotWidget->graph(i);
        if (graph)
            graph->setLineStyle(lineStyle);
    }
    ui->plotWidget->replot();
}

void MainWindow::on_comboPointShape_currentIndexChanged(int index)
{
    // WARN: because ssDot point shape is removed
    if (index > 0) index += 1;
    Q_ASSERT(index >= QCPScatterStyle::ssNone &&
             index <  QCPScatterStyle::ssPixmap);
    QCPScatterStyle::ScatterShape shape = (QCPScatterStyle::ScatterShape) index;
    for (int i = 0, ie = ui->plotWidget->graphCount(); i < ie; ++i)
    {
        QCPGraph * graph = ui->plotWidget->graph(i);
        if (graph)
            graph->setScatterStyle(QCPScatterStyle(shape, 5));
    }
    ui->plotWidget->replot();
}

void MainWindow::jumpToRow(const QString& table, QString column, const QByteArray& value)
{
    // First check if table exists
    DBBrowserObject obj = db.getObjectByName(table);
    if(obj.getname().size() == 0)
        return;

    // If no column name is set, assume the primary key is meant
    if(!column.size())
        column = obj.table.fields().at(obj.table.findPk())->name();

    // If column doesn't exist don't do anything
    int column_index = obj.table.findField(column);
    if(column_index == -1)
        return;

    // Jump to table
    ui->comboBrowseTable->setCurrentIndex(ui->comboBrowseTable->findText(table));
    populateTable(table);

    // Set filter
    ui->dataTable->filterHeader()->setFilter(column_index+1, value);
}

void MainWindow::showDataColumnPopupMenu(const QPoint& pos)
{
    // Get the index of the column which the user has clicked on and store it in the action. This is sort of hack-ish and it might be the heat in my room
    // but I haven't come up with a better solution so far
    ui->actionBrowseTableEditDisplayFormat->setProperty("clicked_column", ui->dataTable->horizontalHeader()->logicalIndexAt(pos));

    // Calculate the proper position for the context menu and display it
    popupBrowseDataHeaderMenu->exec(ui->dataTable->horizontalHeader()->mapToGlobal(pos));
}

void MainWindow::editDataColumnDisplayFormat()
{
    // Get the current table name and fetch its table object, then retrieve the fields of that table and look up the index of the clicked table header
    // section using it as the table field array index. Subtract one from the header index to get the column index because in the the first (though hidden)
    // column is always the rowid column. Ultimately, get the column name from the column object
    QString current_table = ui->comboBrowseTable->currentText();
    int field_number = sender()->property("clicked_column").toInt();
    QString field_name = db.getObjectByName(current_table).table.fields().at(field_number-1)->name();

    // Get the current display format of the field
    QString current_displayformat = browseTableSettings[current_table].displayFormats[field_number];

    // Open the dialog
    ColumnDisplayFormatDialog dialog(field_name, current_displayformat, this);
    if(dialog.exec())
    {
        // Set the newly selected display format
        QString new_format = dialog.selectedDisplayFormat();
        if(new_format.size())
            browseTableSettings[current_table].displayFormats[field_number] = new_format;
        else
            browseTableSettings[current_table].displayFormats.remove(field_number);

        // Refresh view
        populateTable(current_table);
    }
}

void MainWindow::showRowidColumn(bool show)
{
    // Block all signals from the horizontal header. Otherwise the QHeaderView::sectionResized signal causes us trouble
    ui->dataTable->horizontalHeader()->blockSignals(true);

    // Show/hide rowid column
    ui->dataTable->setColumnHidden(0, !show);

    // Update checked status of the popup menu action
    ui->actionShowRowidColumn->setChecked(show);

    // Save settings for this table
    QString current_table = ui->comboBrowseTable->currentText();
    browseTableSettings[current_table].showRowid = show;

    // Update the filter row
    qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader())->generateFilters(m_browseTableModel->columnCount(), show);

    // Re-enable signals
    ui->dataTable->horizontalHeader()->blockSignals(false);
}

void MainWindow::browseDataSetTableEncoding(bool forAllTables)
{
    // Get the old encoding
    QString encoding = m_browseTableModel->encoding();

    // Ask the user for a new encoding
    bool ok;
    QString question;
    if(forAllTables)
        question = tr("Please choose a new encoding for this table.");
    else
        question = tr("Please choose a new encoding for all tables.");
    encoding = QInputDialog::getText(this,
                                     tr("Set encoding"),
                                     tr("%1\nLeave the field empty for using the database encoding.").arg(question),
                                     QLineEdit::Normal,
                                     encoding,
                                     &ok);

    // Only set the new encoding if the user clicked the OK button
    if(ok)
    {
        // Check if encoding is valid
        if(!QTextCodec::codecForName(encoding.toUtf8()))
        {
            QMessageBox::warning(this, qApp->applicationName(), tr("This encoding is either not valid or not supported."));
            return;
        }

        // Set encoding for current table
        m_browseTableModel->setEncoding(encoding);

        // Save encoding for this table
        browseTableSettings[ui->comboBrowseTable->currentText()].encoding = encoding;

        // Set default encoding if requested to and change all stored table encodings
        if(forAllTables)
        {
            defaultBrowseTableEncoding = encoding;

            for(QMap<QString, BrowseDataTableSettings>::Iterator it=browseTableSettings.begin();it!=browseTableSettings.end();++it)
                it.value().encoding = encoding;
        }
    }
}

void MainWindow::browseDataSetDefaultTableEncoding()
{
    browseDataSetTableEncoding(true);
}
