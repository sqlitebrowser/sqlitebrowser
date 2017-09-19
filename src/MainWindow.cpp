#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "EditIndexDialog.h"
#include "AboutDialog.h"
#include "EditTableDialog.h"
#include "ImportCsvDialog.h"
#include "ExportDataDialog.h"
#include "Settings.h"
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
#include "FilterTableHeader.h"
#include "RemoteDock.h"
#include "RemoteDatabase.h"

#include <QFile>
#include <QApplication>
#include <QTextStream>
#include <QWhatsThis>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QPersistentModelIndex>
#include <QDragEnterEvent>
#include <QScrollBar>
#include <QSortFilterProxyModel>
#include <QElapsedTimer>
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
      m_browseTableModel(new SqliteTableModel(db, this, Settings::getValue("db", "prefetchsize").toInt())),
      m_currentTabTableModel(m_browseTableModel),
      m_remoteDb(new RemoteDatabase),
      editDock(new EditDialog(this)),
      plotDock(new PlotDock(this)),
      remoteDock(new RemoteDock(this)),
      gotoValidator(new QIntValidator(0, 0, this))
{
    ui->setupUi(this);
    init();

    activateFields(false);
    updateRecentFileActions();
}

MainWindow::~MainWindow()
{
    delete m_remoteDb;
    delete gotoValidator;
    delete ui;
}

void MainWindow::init()
{
    // Load window settings
    tabifyDockWidget(ui->dockLog, ui->dockPlot);
    tabifyDockWidget(ui->dockLog, ui->dockSchema);
    tabifyDockWidget(ui->dockLog, ui->dockRemote);

    // Connect SQL logging and database state setting to main window
    connect(&db, SIGNAL(dbChanged(bool)), this, SLOT(dbState(bool)));
    connect(&db, SIGNAL(sqlExecuted(QString, int)), this, SLOT(logSql(QString,int)));
    connect(&db, SIGNAL(structureUpdated()), this, SLOT(populateStructure()));

    // Set the validator for the goto line edit
    ui->editGoto->setValidator(gotoValidator);

    // Set up filters
    connect(ui->dataTable->filterHeader(), SIGNAL(filterChanged(int,QString)), this, SLOT(updateFilter(int,QString)));
    connect(m_browseTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataTableSelectionChanged(QModelIndex)));

    // Set up DB structure tab
    dbStructureModel = new DbStructureModel(db, this);
    ui->dbTreeWidget->setModel(dbStructureModel);
    ui->dbTreeWidget->setColumnHidden(1, true);
    ui->dbTreeWidget->setColumnWidth(0, 300);

    // Set up DB schema dock
    ui->treeSchemaDock->setModel(dbStructureModel);
    ui->treeSchemaDock->setColumnHidden(1, true);
    ui->treeSchemaDock->setColumnWidth(0, 300);

    // Set up the table combo box in the Browse Data tab
    ui->comboBrowseTable->setModel(dbStructureModel);

    // Create docks
    ui->dockEdit->setWidget(editDock);
    ui->dockPlot->setWidget(plotDock);
    ui->dockRemote->setWidget(remoteDock);

    // Set up edit dock
    editDock->setReadOnly(true);

    // Restore window geometry
    restoreGeometry(Settings::getValue("MainWindow", "geometry").toByteArray());
    restoreState(Settings::getValue("MainWindow", "windowState").toByteArray());

    // Restore dock state settings
    ui->comboLogSubmittedBy->setCurrentIndex(ui->comboLogSubmittedBy->findText(Settings::getValue("SQLLogDock", "Log").toString()));

    // Add keyboard shortcuts
    QList<QKeySequence> shortcuts = ui->actionExecuteSql->shortcuts();
    shortcuts.push_back(QKeySequence(tr("Ctrl+Return")));
    ui->actionExecuteSql->setShortcuts(shortcuts);

    QShortcut* shortcutBrowseRefreshF5 = new QShortcut(QKeySequence("F5"), this);
    connect(shortcutBrowseRefreshF5, SIGNAL(activated()), this, SLOT(refresh()));
    QShortcut* shortcutBrowseRefreshCtrlR = new QShortcut(QKeySequence("Ctrl+R"), this);
    connect(shortcutBrowseRefreshCtrlR, SIGNAL(activated()), this, SLOT(refresh()));

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
    popupTableMenu->addAction(ui->editModifyObjectAction);
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
    popupBrowseDataHeaderMenu->addAction(ui->actionUnlockViewEditing);
    popupBrowseDataHeaderMenu->addAction(ui->actionBrowseTableEditDisplayFormat);
    popupBrowseDataHeaderMenu->addAction(ui->actionSetTableEncoding);
    popupBrowseDataHeaderMenu->addSeparator();
    popupBrowseDataHeaderMenu->addAction(ui->actionSetAllTablesEncoding);

    QShortcut* dittoRecordShortcut = new QShortcut(QKeySequence("Ctrl+\""), this);
    connect(dittoRecordShortcut, &QShortcut::activated, [this]() {
        int currentRow = ui->dataTable->currentIndex().row();
        auto row = m_browseTableModel->dittoRecord(currentRow);
        ui->dataTable->setCurrentIndex(row);
    });

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
    ui->viewMenu->insertAction(ui->viewDBToolbarAction, ui->dockEdit->toggleViewAction());
    ui->viewMenu->actions().at(3)->setShortcut(QKeySequence(tr("Ctrl+E")));
    ui->viewMenu->actions().at(3)->setIcon(QIcon(":/icons/log_dock"));

    // Add menu item for plot dock
    ui->viewMenu->insertAction(ui->viewDBToolbarAction, ui->dockRemote->toggleViewAction());
    ui->viewMenu->actions().at(4)->setIcon(QIcon(":/icons/log_dock"));

    // If we're not compiling in SQLCipher, hide its FAQ link in the help menu
#ifndef ENABLE_SQLCIPHER
    ui->actionSqlCipherFaq->setVisible(false);
#endif

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
    connect(editDock, SIGNAL(recordTextUpdated(QPersistentModelIndex, QByteArray, bool)), this, SLOT(updateRecordText(QPersistentModelIndex, QByteArray, bool)));
    connect(ui->dbTreeWidget->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(changeTreeSelection()));
    connect(ui->dataTable->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showDataColumnPopupMenu(QPoint)));
    connect(ui->dataTable->verticalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showRecordPopupMenu(QPoint)));
    connect(ui->dockEdit, SIGNAL(visibilityChanged(bool)), this, SLOT(toggleEditDock(bool)));
    connect(m_remoteDb, SIGNAL(openFile(QString)), this, SLOT(fileOpen(QString)));
    connect(m_remoteDb, &RemoteDatabase::gotCurrentVersion, this, &MainWindow::checkNewVersion);

    // Lambda function for keyboard shortcuts for selecting next/previous table in Browse Data tab
    connect(ui->dataTable, &ExtendedTableWidget::switchTable, [this](bool next) {
        int index = ui->comboBrowseTable->currentIndex();
        int num_items = ui->comboBrowseTable->count();
        if(next)
        {
            if(++index >= num_items)
                index = 0;
        } else {
            if(--index < 0)
                index = num_items - 1;
        }
        ui->comboBrowseTable->setCurrentIndex(index);
        populateTable();
    });

    // Set other window settings
    setAcceptDrops(true);
    setWindowTitle(QApplication::applicationName());

    // Load all settings
    reloadSettings();

#ifdef CHECKNEWVERSION
    // Check for a new version if automatic update check aren't disabled in the settings dialog
    if(Settings::getValue("checkversion", "enabled").toBool())
    {
        m_remoteDb->fetch("https://raw.githubusercontent.com/sqlitebrowser/sqlitebrowser/master/currentrelease",
                          RemoteDatabase::RequestTypeNewVersionCheck);
    }
#endif

#ifndef ENABLE_SQLCIPHER
    // Only show encryption menu action when SQLCipher support is enabled
    ui->actionEncryption->setVisible(false);
#endif

    /* Remove all the '&' signs from the dock titles. On at least Windows and
     * OSX, Qt doesn't seem to support them properly, so they end up being
     * visible instead of creating a keyboard shortcut
     */
    ui->dockEdit->setWindowTitle(ui->dockEdit->windowTitle().remove('&'));
    ui->dockLog->setWindowTitle(ui->dockLog->windowTitle().remove('&'));
    ui->dockPlot->setWindowTitle(ui->dockPlot->windowTitle().remove('&'));
    ui->dockSchema->setWindowTitle(ui->dockSchema->windowTitle().remove('&'));
    ui->dockRemote->setWindowTitle(ui->dockRemote->windowTitle().remove('&'));
}

bool MainWindow::fileOpen(const QString& fileName, bool dontAddToRecentFiles, bool readOnly)
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
        if (db.isOpen())
            if(!fileClose())
                return false;

        // Try opening it as a project file first
        if(loadProject(wFile, readOnly))
        {
            retval = true;
        } else {
            // No project file; so it should be a database file
            if(db.open(wFile, readOnly))
            {
                // Close all open but empty SQL tabs
                for(int i=ui->tabSqlAreas->count()-1;i>=0;i--)
                {
                    if(qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i))->getSql().trimmed().isEmpty())
                        closeSqlTab(i, true);
                }

                statusEncodingLabel->setText(db.getPragma("encoding"));
                statusEncryptionLabel->setVisible(db.encrypted());
                statusReadOnlyLabel->setVisible(db.readOnly());
                setCurrentFile(wFile);
                if(!dontAddToRecentFiles)
                    addToRecentFilesMenu(wFile);
                openSqlTab(true);
                loadExtensionsFromSettings();
                if(ui->mainTab->currentIndex() == BrowseTab)
                    populateTable();
                else if(ui->mainTab->currentIndex() == PragmaTab)
                    loadPragmas();
                retval = true;
            } else {
                QMessageBox::warning(this, qApp->applicationName(), tr("Could not open database file.\nReason: %1").arg(db.lastError()));
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
        populateTable();
        openSqlTab(true);
        createTable();
    }
}

void MainWindow::populateStructure()
{
    QString old_table = ui->comboBrowseTable->currentText();

    // Refresh the structure tab
    dbStructureModel->reloadData();
    ui->dbTreeWidget->setRootIndex(dbStructureModel->index(1, 0));      // Show the 'All' part of the db structure
    ui->dbTreeWidget->expandToDepth(0);
    ui->treeSchemaDock->setRootIndex(dbStructureModel->index(1, 0));    // Show the 'All' part of the db structure
    ui->treeSchemaDock->expandToDepth(0);

    // Refresh the browse data tab
    ui->comboBrowseTable->setRootModelIndex(dbStructureModel->index(0, 0)); // Show the 'browsable' section of the db structure tree
    int old_table_index = ui->comboBrowseTable->findText(old_table);
    if(old_table_index == -1 && ui->comboBrowseTable->count())      // If the old table couldn't be found anymore but there is another table, select that
        ui->comboBrowseTable->setCurrentIndex(0);
    else if(old_table_index == -1)                                  // If there aren't any tables to be selected anymore, clear the table view
        clearTableBrowser();
    else                                                            // Under normal circumstances just select the old table again
        ui->comboBrowseTable->setCurrentIndex(old_table_index);

    // Cancel here if no database is opened
    if(!db.isOpen())
        return;

    // Update table and column names for syntax highlighting
    objectMap tab = db.getBrowsableObjects();
    SqlUiLexer::TablesAndColumnsMap tablesToColumnsMap;
    for(auto it=tab.constBegin();it!=tab.constEnd();++it)
    {
        QString objectname = (*it)->name();

        sqlb::FieldInfoList fi = (*it)->fieldInformation();
        foreach(const sqlb::FieldInfo& f, fi)
            tablesToColumnsMap[objectname].append(f.name);
    }
    SqlTextEdit::sqlLexer->setTableNames(tablesToColumnsMap);
    ui->editLogApplication->reloadKeywords();
    ui->editLogUser->reloadKeywords();
    for(int i=0;i<ui->tabSqlAreas->count();i++)
        qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i))->getEditor()->reloadKeywords();

    // Resize SQL column to fit contents
    ui->dbTreeWidget->resizeColumnToContents(3);
}

void MainWindow::clearTableBrowser()
{
    if (!ui->dataTable->model())
        return;

    ui->dataTable->setModel(0);
    if(qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader()))
        qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader())->generateFilters(0);
}

void MainWindow::populateTable()
{
    // Early exit if the Browse Data tab isn't visible as there is no need to update it in this case
    if(ui->mainTab->currentIndex() != BrowseTab)
        return;

    // Remove the model-view link if the table name is empty in order to remove any data from the view
    if(ui->comboBrowseTable->model()->rowCount(ui->comboBrowseTable->rootModelIndex()) == 0)
    {
        clearTableBrowser();
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Get current table name
    QString tablename = ui->comboBrowseTable->currentText();

    // Set model
    bool reconnectSelectionSignals = false;
    if(ui->dataTable->model() == 0)
        reconnectSelectionSignals = true;
    ui->dataTable->setModel(m_browseTableModel);
    if(reconnectSelectionSignals)
        connect(ui->dataTable->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(dataTableSelectionChanged(QModelIndex)));

    // Search stored table settings for this table
    auto settingsIt = browseTableSettings.constFind(tablename);
    bool storedDataFound = settingsIt != browseTableSettings.constEnd();

    // Set new table
    if(!storedDataFound)
    {
        // No stored settings found.

        // Set table name and apply default display format settings
        m_browseTableModel->setTable(tablename, 0, Qt::AscendingOrder);

        // There aren't any information stored for this table yet, so use some default values

        // Hide rowid column. Needs to be done before the column widths setting because of the workaround in there
        showRowidColumn(false);

        // Enable editing in general, but lock view editing
        unlockViewEditing(false);

        // Column widths
        for(int i=1;i<m_browseTableModel->columnCount();i++)
            ui->dataTable->setColumnWidth(i, ui->dataTable->horizontalHeader()->defaultSectionSize());

        // Sorting
        ui->dataTable->filterHeader()->setSortIndicator(0, Qt::AscendingOrder);

        // Encoding
        m_browseTableModel->setEncoding(defaultBrowseTableEncoding);

        // Plot
        plotDock->updatePlot(m_browseTableModel, &browseTableSettings[ui->comboBrowseTable->currentText()]);

        // The filters can be left empty as they are
    } else {
        // Stored settings found. Retrieve them.
        BrowseDataTableSettings storedData = settingsIt.value();

        // Load display formats and set them along with the table name
        QVector<QString> v;
        bool only_defaults = true;
        const sqlb::FieldInfoList& tablefields = db.getObjectByName(tablename)->fieldInformation();
        for(int i=0; i<tablefields.size(); ++i)
        {
            QString format = storedData.displayFormats[i+1];
            if(format.size())
            {
                v.push_back(format);
                only_defaults = false;
            } else {
                v.push_back(sqlb::escapeIdentifier(tablefields.at(i).name));
            }
        }
        if(only_defaults)
            m_browseTableModel->setTable(tablename, storedData.sortOrderIndex, storedData.sortOrderMode);
        else
            m_browseTableModel->setTable(tablename, storedData.sortOrderIndex, storedData.sortOrderMode, v);

        // There is information stored for this table, so extract it and apply it

        // Show rowid column. Needs to be done before the column widths setting because of the workaround in there and before the filter setting
        // because of the filter row generation.
        showRowidColumn(storedData.showRowid);

        // Enable editing in general and (un)lock view editing depending on the settings
        unlockViewEditing(!storedData.unlockViewPk.isEmpty(), storedData.unlockViewPk);

        // Column widths
        for(auto widthIt=storedData.columnWidths.constBegin();widthIt!=storedData.columnWidths.constEnd();++widthIt)
            ui->dataTable->setColumnWidth(widthIt.key(), widthIt.value());

        // Sorting
        ui->dataTable->filterHeader()->setSortIndicator(storedData.sortOrderIndex, storedData.sortOrderMode);

        // Filters
        FilterTableHeader* filterHeader = qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader());
        for(auto filterIt=storedData.filterValues.constBegin();filterIt!=storedData.filterValues.constEnd();++filterIt)
            filterHeader->setFilter(filterIt.key(), filterIt.value());

        // Encoding
        m_browseTableModel->setEncoding(storedData.encoding);

        // Plot
        plotDock->updatePlot(m_browseTableModel, &browseTableSettings[ui->comboBrowseTable->currentText()], true, false);
    }

    // Show/hide menu options depending on whether this is a table or a view
    if(db.getObjectByName(ui->comboBrowseTable->currentText())->type() == sqlb::Object::Table)
    {
        // Table
        ui->actionUnlockViewEditing->setVisible(false);
        ui->actionShowRowidColumn->setVisible(true);
    } else {
        // View
        ui->actionUnlockViewEditing->setVisible(true);
        ui->actionShowRowidColumn->setVisible(false);
    }

    // Set the recordset label
    setRecordsetLabel();

    QApplication::restoreOverrideCursor();
}

bool MainWindow::fileClose()
{
    // Close the database but stop the closing process here if the user pressed the cancel button in there
    if(!db.close())
        return false;

    setWindowTitle(QApplication::applicationName());
    loadPragmas();
    statusEncryptionLabel->setVisible(false);
    statusReadOnlyLabel->setVisible(false);

    // Reset the model for the Browse tab
    m_browseTableModel->reset();

    // Remove all stored table information browse data tab
    browseTableSettings.clear();
    defaultBrowseTableEncoding = QString();

    // Clear edit dock
    editDock->setCurrentIndex(QModelIndex());

    // Reset the recordset label inside the Browse tab now
    setRecordsetLabel();

    // Reset the plot dock model
    plotDock->updatePlot(0);

    activateFields(false);

    // Clear the SQL Log
    ui->editLogApplication->clear();
    ui->editLogUser->clear();

    return true;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    if(db.close())
    {
        Settings::setValue("MainWindow", "geometry", saveGeometry());
        Settings::setValue("MainWindow", "windowState", saveState());
        Settings::setValue("SQLLogDock", "Log", ui->comboLogSubmittedBy->currentText());
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
        QMessageBox::warning(this, QApplication::applicationName(), tr("Error adding record:\n") + db.lastError());
    }
}

void MainWindow::deleteRecord()
{
    if(ui->dataTable->selectionModel()->hasSelection())
    {
        // If only filter header is selected
        if(ui->dataTable->selectionModel()->selectedIndexes().isEmpty())
            return;

        int old_row = ui->dataTable->currentIndex().row();
        while(ui->dataTable->selectionModel()->hasSelection())
        {
            int first_selected_row = ui->dataTable->selectionModel()->selectedIndexes().first().row();
            int last_selected_row = ui->dataTable->selectionModel()->selectedIndexes().last().row();
            int selected_rows_count = last_selected_row - first_selected_row + 1;
            if(!m_browseTableModel->removeRows(first_selected_row, selected_rows_count))
            {
                QMessageBox::warning(this, QApplication::applicationName(), tr("Error deleting record:\n%1").arg(db.lastError()));
                break;
            }
        }

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
    ui->dataTable->scrollTo(ui->dataTable->currentIndex(), QAbstractItemView::PositionAtTop);
    QApplication::restoreOverrideCursor();
}

void MainWindow::navigatePrevious()
{
    int curRow = ui->dataTable->currentIndex().row();
    curRow -= ui->dataTable->numVisibleRows() - 1;
    if(curRow < 0)
        curRow = 0;
    selectTableLine(curRow);
}


void MainWindow::navigateNext()
{
    int curRow = ui->dataTable->currentIndex().row();
    curRow += ui->dataTable->numVisibleRows() - 1;
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
    int total = m_browseTableModel->totalRowCount();
    int to = ui->dataTable->verticalHeader()->visualIndexAt(ui->dataTable->height()) - 1;
    if (to == -2)
        to = total;

    // Update the validator of the goto row field
    gotoValidator->setRange(0, total);

    // Update the label showing the current position
    ui->labelRecordset->setText(tr("%1 - %2 of %3").arg(from).arg(to).arg(total));
}

void MainWindow::refresh()
{
    // What the Refresh function does depends on the currently active tab. This way the keyboard shortcuts (F5 and Ctrl+R)
    // always perform some meaningful task; they just happen to be context dependent in the function they trigger.
    switch(ui->mainTab->currentIndex())
    {
    case StructureTab:
        // Refresh the schema
        db.updateSchema();
        break;
    case BrowseTab:
        // Refresh the schema and reload the current table
        db.updateSchema();
        populateTable();
        break;
    case PragmaTab:
        // Reload pragma values
        loadPragmas();
        break;
    case ExecuteTab:
        // (Re-)Run the current SQL query
        executeQuery();
        break;
    }
}

void MainWindow::createTable()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), tr("There is no database opened. Please open or create a new database file."));
        return;
    }

    EditTableDialog dialog(db, "", true, this);
    if(dialog.exec())
    {
        populateTable();
    }
}

void MainWindow::createIndex()
{
    if (!db.isOpen()){
        QMessageBox::information( this, QApplication::applicationName(), tr("There is no database opened. Please open or create a new database file."));
        return;
    }

    EditIndexDialog dialog(db, "", true, this);
    if(dialog.exec())
        populateTable();
}

void MainWindow::compact()
{
    VacuumDialog dialog(&db, this);
    dialog.exec();
}

void MainWindow::deleteObject()
{
    // Get name and type of object to delete
    QString table = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 0), Qt::EditRole).toString();
    QString type = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 1), Qt::EditRole).toString();

    // Ask user if he really wants to delete that table
    if(QMessageBox::warning(this, QApplication::applicationName(), tr("Are you sure you want to delete the %1 '%2'?\nAll data associated with the %1 will be lost.").arg(type).arg(table),
                            QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        // Delete the table
        QString statement = QString("DROP %1 %2;").arg(type.toUpper()).arg(sqlb::escapeIdentifier(table));
        if(!db.executeSQL( statement))
        {
            QString error = tr("Error: could not delete the %1. Message from database engine:\n%2").arg(type).arg(db.lastError());
            QMessageBox::warning(this, QApplication::applicationName(), error);
        } else {
            populateTable();
            changeTreeSelection();
        }
    }
}

void MainWindow::editObject()
{
    if(!ui->dbTreeWidget->selectionModel()->hasSelection())
        return;

    // Get name and type of the object to edit
    QString name = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 0), Qt::EditRole).toString();
    QString type = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 1), Qt::EditRole).toString();

    if(type == "table")
    {
        EditTableDialog dialog(db, name, false, this);
        if(dialog.exec())
            populateTable();
    } else if(type == "index") {
        EditIndexDialog dialog(db, name, false, this);
        if(dialog.exec())
            populateTable();
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

void MainWindow::updateRecordText(const QPersistentModelIndex& idx, const QByteArray& text, bool isBlob)
{
    m_currentTabTableModel->setTypedData(idx, isBlob, text);
}

void MainWindow::toggleEditDock(bool visible)
{
    if (!visible) {
        // Update main window
        activateWindow();
        ui->dataTable->setFocus();
    } else {
        // fill edit dock with actual data
        editDock->setCurrentIndex(ui->dataTable->currentIndex());
    }
}

void MainWindow::doubleClickTable(const QModelIndex& index)
{
    // Cancel on invalid index
    if (!index.isValid()) {
        return;
    }

    // * Don't allow editing of other objects than tables (on the browse table) *
    bool isEditingAllowed = !db.readOnly() && m_currentTabTableModel == m_browseTableModel &&
            (db.getObjectByName(ui->comboBrowseTable->currentText())->type() == sqlb::Object::Types::Table);

    // Enable or disable the Apply, Null, & Import buttons in the Edit Cell
    // dock depending on the value of the "isEditingAllowed" bool above
    editDock->setReadOnly(!isEditingAllowed);

    editDock->setCurrentIndex(index);

    // Show the edit dock
    ui->dockEdit->setVisible(true);

    // Set focus on the edit dock
    editDock->setFocus();
}

void MainWindow::dataTableSelectionChanged(const QModelIndex& index)
{
    // Cancel on invalid index
    if(!index.isValid()) {
        editDock->setCurrentIndex(QModelIndex());
        return;
    }

    bool editingAllowed = !db.readOnly() && (m_currentTabTableModel == m_browseTableModel) &&
            (db.getObjectByName(ui->comboBrowseTable->currentText())->type() == sqlb::Object::Types::Table);

    // Don't allow editing of other objects than tables
    editDock->setReadOnly(!editingAllowed);

    // If the Edit Cell dock is visible, load the new value into it
    if (editDock->isVisible()) {
        editDock->setCurrentIndex(index);
    }
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
    // Make sure a database is opened. This is necessary because we allow opened SQL editor tabs even if no database is loaded. Hitting F5 or similar
    // then might call this function.
    if(!db.isOpen())
        return;

    SqlExecutionArea* sqlWidget = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());

    // Get SQL code to execute. This depends on the button that's been pressed
    QString query;
    int execution_start_line = 0;
    int execution_start_index = 0;
    if(sender()->objectName() == "actionSqlExecuteLine")
    {
        int cursor_line, cursor_index;
        SqlTextEdit *editor = sqlWidget->getEditor();

        editor->getCursorPosition(&cursor_line, &cursor_index);

        execution_start_line = cursor_line;

        int lineStartCursorPosition = editor->positionFromLineIndex(cursor_line, 0);

        QString entireSQL = editor->text();
        QString firstPartEntireSQL = entireSQL.left(lineStartCursorPosition);
        QString secondPartEntireSQL = entireSQL.right(entireSQL.length() - lineStartCursorPosition);

        QString firstPartSQL = firstPartEntireSQL.split(";").last();
        QString lastPartSQL = secondPartEntireSQL.split(";").first();

        query = firstPartSQL + lastPartSQL;
    } else {
        // if a part of the query is selected, we will only execute this part
        query = sqlWidget->getSelectedSql();
        int dummy;
        if(query.isEmpty())
            query = sqlWidget->getSql();
        else
            sqlWidget->getEditor()->getSelection(&execution_start_line, &execution_start_index, &dummy, &dummy);
    }

    SqliteTableModel::removeCommentsFromQuery(query);

    if (query.trimmed().isEmpty() || query.trimmed() == ";")
        return;

    query = query.remove(QRegExp("^\\s*BEGIN TRANSACTION;|COMMIT;\\s*$")).trimmed();

    //log the query
    db.logSQL(query, kLogMsg_User);
    sqlite3_stmt *vm;
    QByteArray utf8Query = query.toUtf8();
    const char *tail = utf8Query.data();
    int sql3status = SQLITE_OK;
    int tail_length = utf8Query.length();
    QString statusMessage;
    bool modified = false;
    bool wasdirty = db.getDirty();
    bool structure_updated = false;
    bool savepoint_created = false;

    // Remove any error indicators
    sqlWidget->getEditor()->clearErrorIndicators();

    // Accept multi-line queries, by looping until the tail is empty
    QElapsedTimer timer;
    timer.start();
    while( tail && *tail != 0 && (sql3status == SQLITE_OK || sql3status == SQLITE_DONE))
    {
        // Check whether the DB structure is changed by this statement
        QString qtail = QString(tail).trimmed();
        if(!structure_updated && (qtail.startsWith("ALTER", Qt::CaseInsensitive) ||
                qtail.startsWith("CREATE", Qt::CaseInsensitive) ||
                qtail.startsWith("DROP", Qt::CaseInsensitive) ||
                qtail.startsWith("ROLLBACK", Qt::CaseInsensitive)))
            structure_updated = true;

        // Check whether this is trying to set a pragma or to vacuum the database
        if((qtail.startsWith("PRAGMA", Qt::CaseInsensitive) && qtail.contains('=')) || qtail.startsWith("VACUUM", Qt::CaseInsensitive))
        {
            // We're trying to set a pragma. If the database has been modified it needs to be committed first. We'll need to ask the
            // user about that
            if(db.getDirty())
            {
                if(QMessageBox::question(this,
                                         QApplication::applicationName(),
                                         tr("Setting PRAGMA values or vacuuming will commit your current transaction.\nAre you sure?"),
                                         QMessageBox::Yes | QMessageBox::Default,
                                         QMessageBox::No | QMessageBox::Escape) == QMessageBox::Yes)
                {
                    // Commit all changes
                    db.releaseAllSavepoints();
                } else {
                    // Abort
                    statusMessage = tr("Execution aborted by user");
                    break;
                }
            }
        } else {
            // We're not trying to set a pragma or to vacuum the database. In this case make sure a savepoint has been created in order to avoid committing
            // all changes to the database immediately. Don't set more than one savepoint.

            if(!savepoint_created)
            {
                // there is no choice, we have to start a transaction before we create the prepared statement,
                // otherwise every executed statement will get committed after the prepared statement gets finalized,
                // see http://www.sqlite.org/lang_transaction.html
                db.setSavepoint();
                savepoint_created = true;
            }
        }

        // Execute next statement
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
            case SQLITE_MISUSE:
                continue;
            default:
                statusMessage = QString::fromUtf8((const char*)sqlite3_errmsg(db._db)) +
                        ": " + queryPart;
                break;
            }
            timer.restart();
        } else {
            statusMessage = QString::fromUtf8((const char*)sqlite3_errmsg(db._db)) +
                    ": " + queryPart;
            sqlWidget->getEditor()->setErrorIndicator(execution_start_line, execution_start_index, execution_start_line, execution_end_index);
        }

        execution_start_index = execution_end_index;

        // Process events to keep the UI responsive
        qApp->processEvents();
    }
    sqlWidget->finishExecution(statusMessage);
    plotDock->updatePlot(sqlWidget->getModel());

    connect(sqlWidget->getTableResult(), &ExtendedTableWidget::activated, this, &MainWindow::dataTableSelectionChanged);
    connect(sqlWidget->getTableResult(), SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickTable(QModelIndex)));

    if(!modified && !wasdirty && savepoint_created)
        db.revertToSavepoint(); // better rollback, if the logic is not enough we can tune it.

    // If the DB structure was changed by some command in this SQL script, update our schema representations
    if(structure_updated)
        db.updateSchema();
}

void MainWindow::mainTabSelected(int tabindex)
{
    editDock->setReadOnly(true);

    switch (tabindex)
    {
    case StructureTab:
        break;

    case BrowseTab:
        m_currentTabTableModel = m_browseTableModel;
        populateTable();
        break;

    case PragmaTab:
        loadPragmas();
        break;

    case ExecuteTab:
    {
        SqlExecutionArea* sqlWidget = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());

        if (sqlWidget) {
            m_currentTabTableModel = sqlWidget->getModel();

            dataTableSelectionChanged(sqlWidget->getTableResult()->currentIndex());
        }
        break;
    }

    default: break;
    }
}

void MainWindow::importTableFromCSV()
{
    QStringList wFiles = FileDialog::getOpenFileNames(
                            this,
                            tr("Choose text files"),
                            tr("Text files(*.csv *.txt);;All files(*)"));

    QStringList validFiles;
    foreach(auto file, wFiles) {
        if (QFile::exists(file))
            validFiles.append(file);
    }

    if (!validFiles.isEmpty()) {
        ImportCsvDialog dialog(validFiles, &db, this);
        if (dialog.exec()) {
            populateTable();
            QMessageBox::information(this, QApplication::applicationName(), tr("Import completed"));
        }
    }
}

void MainWindow::exportTableToCSV()
{
    // Get the current table name if we are in the Browse Data tab
    QString current_table;
    if(ui->mainTab->currentIndex() == StructureTab) {
        QString type = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 1)).toString();
        if(type == "table" || type == "view")
            current_table = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 0)).toString();
    }
    else if(ui->mainTab->currentIndex() == BrowseTab)
        current_table = ui->comboBrowseTable->currentText();

    // Open dialog
    ExportDataDialog dialog(db, ExportDataDialog::ExportFormatCsv, this, "", current_table);
    dialog.exec();
}

void MainWindow::exportTableToJson()
{
    // Get the current table name if we are in the Browse Data tab
    QString current_table;
    if(ui->mainTab->currentIndex() == StructureTab) {
        QString type = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 1)).toString();
        if(type == "table" || type == "view")
            current_table = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 0)).toString();
    }
    else if(ui->mainTab->currentIndex() == BrowseTab)
        current_table = ui->comboBrowseTable->currentText();

    // Open dialog
    ExportDataDialog dialog(db, ExportDataDialog::ExportFormatJson, this, "", current_table);
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
    {
        if(!db.releaseAllSavepoints())
        {
            QMessageBox::warning(this, QApplication::applicationName(), tr("Error while saving the database file. This means that not all changes to the database were "
                                                                           "saved. You need to resolve the following error first.\n\n%1").arg(db.lastError()));
        }
    }
}

void MainWindow::fileRevert()
{
    if (db.isOpen()){
        QString msg = tr("Are you sure you want to undo all changes made to the database file '%1' since the last save?").arg(db.currentFile());
        if(QMessageBox::question(this, QApplication::applicationName(), msg, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape) == QMessageBox::Yes)
        {
            db.revertAll();
            populateTable();
        }
    }
}

void MainWindow::exportDatabaseToSQL()
{
    QString current_table;
    if(ui->mainTab->currentIndex() == BrowseTab)
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
        QMessageBox::warning(this, QApplication::applicationName(), tr("Error importing data: %1").arg(db.lastError()));
    else
        QMessageBox::information(this, QApplication::applicationName(), tr("Import completed."));
    f.close();
    QApplication::restoreOverrideCursor();

    // Refresh window when importing into an existing DB or - when creating a new file - just open it correctly
    if(newDbFile.size())
    {
        fileOpen(newDbFile);
    } else {
        db.updateSchema();
        populateTable();
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
    ui->editModifyObjectAction->setEnabled(false);
    ui->actionEditBrowseTable->setEnabled(false);

    if(!ui->dbTreeWidget->currentIndex().isValid())
        return;

    // Change the text and tooltips of the actions
    QString type = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 1)).toString();

    if (type.isEmpty())
    {
        ui->editDeleteObjectAction->setIcon(QIcon(":icons/table_delete"));
        ui->editModifyObjectAction->setIcon(QIcon(":icons/table_modify"));
    } else {
        ui->editDeleteObjectAction->setIcon(QIcon(QString(":icons/%1_delete").arg(type)));
        ui->editModifyObjectAction->setIcon(QIcon(QString(":icons/%1_modify").arg(type)));
    }

    if (type == "view") {
        ui->editDeleteObjectAction->setText(tr("Delete View"));
        ui->editDeleteObjectAction->setToolTip(tr("Delete View"));
        ui->editModifyObjectAction->setText(tr("Modify View"));
        ui->editModifyObjectAction->setToolTip(tr("Modify View"));
    } else if(type == "trigger") {
        ui->editDeleteObjectAction->setText(tr("Delete Trigger"));
        ui->editDeleteObjectAction->setToolTip(tr("Delete Trigger"));
        ui->editModifyObjectAction->setText(tr("Modify Trigger"));
        ui->editModifyObjectAction->setToolTip(tr("Modify Trigger"));
    } else if(type == "index") {
        ui->editDeleteObjectAction->setText(tr("Delete Index"));
        ui->editDeleteObjectAction->setToolTip(tr("Delete Index"));
        ui->editModifyObjectAction->setText(tr("Modify Index"));
        ui->editModifyObjectAction->setToolTip(tr("Modify Index"));
    } else {
        ui->editDeleteObjectAction->setText(tr("Delete Table"));
        ui->editDeleteObjectAction->setToolTip(tr("Delete Table"));
        ui->editModifyObjectAction->setText(tr("Modify Table"));
        ui->editModifyObjectAction->setToolTip(tr("Modify Table"));
    }

    // Activate actions
    if(type == "table" || type == "index")
    {
        ui->editDeleteObjectAction->setEnabled(!db.readOnly());
        ui->editModifyObjectAction->setEnabled(!db.readOnly());
    } else if(type == "view" || type == "trigger") {
        ui->editDeleteObjectAction->setEnabled(!db.readOnly());
    }
    if(type == "table" || type == "view")
    {
        ui->actionEditBrowseTable->setEnabled(true);
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
    QStringList files = Settings::getValue("General", "recentFileList").toStringList();

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
    Settings::setValue("General", "recentFileList", files);

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(QDir::toNativeSeparators(files[i]));
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
    setWindowTitle(QApplication::applicationName() + " - " + QDir::toNativeSeparators(fileName));
    activateFields(true);
    dbState(db.getDirty());
}

void MainWindow::addToRecentFilesMenu(const QString& filename)
{
    QStringList files = Settings::getValue("General", "recentFileList").toStringList();
    QFileInfo info(filename);

    files.removeAll(info.absoluteFilePath());
    files.prepend(info.absoluteFilePath());
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    Settings::setValue("General", "recentFileList", files);

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
    ui->fileExportJsonAction->setEnabled(enable);
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
    ui->actionSaveProject->setEnabled(enable);
    ui->actionEncryption->setEnabled(enable && write);
    ui->buttonClearFilters->setEnabled(enable);
    ui->dockEdit->setEnabled(enable);
    ui->dockPlot->setEnabled(enable);

    remoteDock->enableButtons();
}

void MainWindow::enableEditing(bool enable_edit, bool enable_insertdelete)
{
    // Don't enable anything if this is a read only database
    bool edit = enable_edit && !db.readOnly();
    bool insertdelete = enable_insertdelete && !db.readOnly();

    // Apply settings
    ui->buttonNewRecord->setEnabled(insertdelete);
    ui->buttonDeleteRecord->setEnabled(insertdelete);
    ui->dataTable->setEditTriggers(edit ? QAbstractItemView::SelectedClicked | QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed : QAbstractItemView::NoEditTriggers);

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

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    int tab = -1;

    switch (event->key())
    {
    case Qt::Key_1:
        tab = Tabs::StructureTab;
        break;
    case Qt::Key_2:
        tab = Tabs::BrowseTab;
        break;
    case Qt::Key_3:
        tab = Tabs::PragmaTab;
        break;
    case Qt::Key_4:
        tab = Tabs::ExecuteTab;
        break;
    default:
        break;
    }

    if (event->modifiers() & Qt::AltModifier && tab != -1)
        ui->mainTab->setCurrentIndex(tab);

    QMainWindow::keyPressEvent(event);
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
    SqlExecutionArea* w = new SqlExecutionArea(db, this);
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
        if(!f.isOpen())
        {
            QMessageBox::warning(this, qApp->applicationName(), tr("Couldn't read file: %1.").arg(f.errorString()));
            return;
        }

        // Decide whether to open a new tab or take the current one
        unsigned int index;
        SqlExecutionArea* current_tab = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());
        if(current_tab && current_tab->getSql().isEmpty() && current_tab->getModel()->rowCount() == 0)
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
    if(!sqlarea)
        return;

    // If this SQL file hasn't been saved before open the Save As dialog. Otherwise just use the old file name for saving
    if(sqlarea->fileName().isEmpty())
    {
        saveSqlFileAs();
    } else {
        QFile f(sqlarea->fileName());
        f.open(QIODevice::WriteOnly);
        if(f.isOpen() && f.write(sqlarea->getSql().toUtf8()) != -1)
        {
            QFileInfo fileinfo(sqlarea->fileName());
            ui->tabSqlAreas->setTabText(ui->tabSqlAreas->currentIndex(), fileinfo.fileName());
        } else {
            QMessageBox::warning(this, qApp->applicationName(), tr("Couldn't save file: %1.").arg(f.errorString()));
        }
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
        QMessageBox::warning(this, QApplication::applicationName(), tr("Error loading extension: %1").arg(db.lastError()));
}

void MainWindow::loadExtensionsFromSettings()
{
    if(!db.isOpen())
        return;

    QStringList list = Settings::getValue("extensions", "list").toStringList();
    foreach(QString ext, list)
    {
        if(db.loadExtension(ext) == false)
            QMessageBox::warning(this, QApplication::applicationName(), tr("Error loading extension: %1").arg(db.lastError()));
    }
}

void MainWindow::reloadSettings()
{
    // Set data browser font
    ui->dataTable->reloadSettings();

    // Set prefetch sizes for lazy population of table models
    m_browseTableModel->setChunkSize(Settings::getValue("db", "prefetchsize").toInt());
    for(int i=0;i<ui->tabSqlAreas->count();++i)
        qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i))->reloadSettings();

    // Prepare log font
    QFont logfont("Monospace");
    logfont.setStyleHint(QFont::TypeWriter);
    logfont.setPointSize(Settings::getValue("log", "fontsize").toInt());

    // Set font for SQL logs and edit dialog
    ui->editLogApplication->reloadSettings();
    ui->editLogUser->reloadSettings();
    ui->editLogApplication->setFont(logfont);
    ui->editLogUser->setFont(logfont);
    editDock->reloadSettings();

    // Load extensions
    loadExtensionsFromSettings();

    // Refresh view
    populateStructure();
    populateTable();

    // Hide or show the remote dock as needed
    bool showRemoteActions = Settings::getValue("remote", "active").toBool();
    ui->viewMenu->actions().at(4)->setVisible(showRemoteActions);
    if(!showRemoteActions)
        ui->dockRemote->setHidden(true);

    // Update the remote database connection settings
    m_remoteDb->reloadSettings();

    // Reload remote dock settings
    remoteDock->reloadSettings();
}

void MainWindow::checkNewVersion(const QString& versionstring, const QString& url)
{
    // versionstring contains a major.minor.patch version string
    QStringList versiontokens = versionstring.split(".");
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
                                arg(url));
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

void MainWindow::on_actionWiki_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/sqlitebrowser/sqlitebrowser/wiki"));
}

void MainWindow::on_actionBug_report_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/sqlitebrowser/sqlitebrowser/issues/new"));
}

void MainWindow::on_actionSqlCipherFaq_triggered()
{
    QDesktopServices::openUrl(QUrl("https://discuss.zetetic.net/c/sqlcipher/sqlcipher-faq"));
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

bool MainWindow::loadProject(QString filename, bool readOnly)
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
                    fileOpen(dbfilename, true, readOnly);
                    ui->dbTreeWidget->collapseAll();

                    // PRAGMAs
                    if(xml.attributes().hasAttribute("foreign_keys"))
                        db.setPragma("foreign_keys", xml.attributes().value("foreign_keys").toString());
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
                            if(ui->mainTab->currentIndex() == BrowseTab)
                            {
                                populateTable();     // Refresh view
                                ui->dataTable->sortByColumn(browseTableSettings[ui->comboBrowseTable->currentText()].sortOrderIndex,
                                                            browseTableSettings[ui->comboBrowseTable->currentText()].sortOrderMode);
                                showRowidColumn(browseTableSettings[ui->comboBrowseTable->currentText()].showRowid);
                                unlockViewEditing(!browseTableSettings[ui->comboBrowseTable->currentText()].unlockViewPk.isEmpty(), browseTableSettings[ui->comboBrowseTable->currentText()].unlockViewPk);
                            }
                            xml.skipCurrentElement();
                        }
                    }
                } else if(xml.name() == "tab_sql") {
                    // Close all open tabs first
                    for(int i=ui->tabSqlAreas->count()-1;i>=0;i--)
                        closeSqlTab(i, true);

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
                                                    tr("DB Browser for SQLite project file (*.sqbpro)"),
                                                    db.currentFile());
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
        xml.writeAttribute("path", db.currentFile());
        xml.writeAttribute("foreign_keys", db.getPragma("foreign_keys"));
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
            QFile file(db.currentFile() + ".enctemp");
            file.open(QFile::WriteOnly);
            file.close();
        }

        // Attach a new database using the new settings
        qApp->processEvents();
        if(ok)
            ok = db.executeSQL(QString("ATTACH DATABASE '%1' AS sqlitebrowser_edit_encryption KEY %2;").arg(db.currentFile() + ".enctemp").arg(dialog.password()),
                               false, false);
        qApp->processEvents();
        if(ok)
            ok = db.executeSQL(QString("PRAGMA sqlitebrowser_edit_encryption.cipher_page_size = %1").arg(dialog.pageSize()), false, false);

        // Export the current database to the new one
        qApp->processEvents();
        if(ok)
            ok = db.executeSQL("SELECT sqlcipher_export('sqlitebrowser_edit_encryption');", false, false);

        // Set user version of the new database
        qApp->processEvents();
        if (ok)
            ok = db.executeSQL(QString("PRAGMA sqlitebrowser_edit_encryption.user_version = %1;").arg(db.getPragma("user_version").toInt()), false, false);

        // Check for errors
        qApp->processEvents();
        if(ok)
        {
            // No errors: Then close the current database, switch names, open the new one and if that succeeded delete the old one

            fileClose();
            QFile::rename(db.currentFile(), db.currentFile() + ".enctempold");
            QFile::rename(db.currentFile() + ".enctemp", db.currentFile());
            if(fileOpen(db.currentFile()))
                QFile::remove(db.currentFile() + ".enctempold");
        } else {
            QMessageBox::warning(this, qApp->applicationName(), db.lastError());
        }
    }
#endif
}

void MainWindow::switchToBrowseDataTab(QString tableToBrowse)
{
    // If no table name was provided get the currently selected table fromt he structure tab
    if(tableToBrowse.isEmpty())
    {
        // Cancel here if there is no selection
        if(!ui->dbTreeWidget->selectionModel()->hasSelection())
            return;

        tableToBrowse = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 0)).toString();
    }

    ui->comboBrowseTable->setCurrentIndex(ui->comboBrowseTable->findText(tableToBrowse));
    ui->mainTab->setCurrentIndex(BrowseTab);
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

void MainWindow::jumpToRow(const QString& table, QString column, const QByteArray& value)
{
    // First check if table exists
    sqlb::TablePtr obj = db.getObjectByName(table).dynamicCast<sqlb::Table>();
    if(!obj)
        return;

    // If no column name is set, assume the primary key is meant
    if(!column.size())
        column = obj->fields().at(obj->findPk())->name();

    // If column doesn't exist don't do anything
    int column_index = obj->findField(column);
    if(column_index == -1)
        return;

    // Jump to table
    ui->comboBrowseTable->setCurrentIndex(ui->comboBrowseTable->findText(table));
    populateTable();

    // Set filter
    ui->dataTable->filterHeader()->setFilter(column_index+1, value);
}

void MainWindow::showDataColumnPopupMenu(const QPoint& pos)
{
    // Get the index of the column which the user has clicked on and store it in the action. This is sort of hack-ish and it might be the heat in my room
    // but I haven't come up with a better solution so far
    int logical_index = ui->dataTable->horizontalHeader()->logicalIndexAt(pos);
    if(logical_index == -1)     // Don't open the popup menu if the user hasn't clicked on a column header
            return;
    ui->actionBrowseTableEditDisplayFormat->setProperty("clicked_column", logical_index);

    // Calculate the proper position for the context menu and display it
    popupBrowseDataHeaderMenu->exec(ui->dataTable->horizontalHeader()->mapToGlobal(pos));
}

void MainWindow::showRecordPopupMenu(const QPoint& pos)
{
    const QString sCurrentTable = ui->comboBrowseTable->currentText();
    if(!(db.getObjectByName(sCurrentTable)->type() == sqlb::Object::Types::Table && !db.readOnly()))
        return;

    int row = ui->dataTable->verticalHeader()->logicalIndexAt(pos);
    if (row == -1)
        return;

    QMenu popupRecordMenu(this);
    QAction* action = new QAction("Duplicate record", &popupRecordMenu);
    popupRecordMenu.addAction(action);

    connect(action, &QAction::triggered, [&]() {
       m_browseTableModel->dittoRecord(row);
    });

    popupRecordMenu.exec(ui->dataTable->verticalHeader()->mapToGlobal(pos));
}

void MainWindow::editDataColumnDisplayFormat()
{
    // Get the current table name and fetch its table object, then retrieve the fields of that table and look up the index of the clicked table header
    // section using it as the table field array index. Subtract one from the header index to get the column index because in the the first (though hidden)
    // column is always the rowid column. Ultimately, get the column name from the column object
    QString current_table = ui->comboBrowseTable->currentText();
    int field_number = sender()->property("clicked_column").toInt();
    QString field_name = db.getObjectByName(current_table).dynamicCast<sqlb::Table>()->fields().at(field_number-1)->name();

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
        populateTable();
    }
}

void MainWindow::showRowidColumn(bool show)
{
    // Block all signals from the horizontal header. Otherwise the QHeaderView::sectionResized signal causes us trouble
    ui->dataTable->horizontalHeader()->blockSignals(true);

    // WORKAROUND
    // Set the opposite hidden/visible status of what we actually want for the rowid column. This is to work around a Qt bug which
    // is present in at least version 5.7.1. The problem is this: when you browse a table/view with n colums, then switch to a table/view
    // with less than n columns, you'll be able to resize the first (hidden!) column by resizing the section to the left of the first visible
    // column. By doing so the table view gets messed up. But even when not resizing the first hidden column, tab-ing through the fields
    // will stop at the not-so-much-hidden rowid column, too. All this can be fixed by this line. I haven't managed to find another workaround
    // or way to fix this yet.
    ui->dataTable->setColumnHidden(0, show);

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

    ui->dataTable->update();
}

void MainWindow::browseDataSetTableEncoding(bool forAllTables)
{
    // Get the old encoding
    QString encoding = m_browseTableModel->encoding();

    // Ask the user for a new encoding
    bool ok;
    QString question;
    if(forAllTables)
        question = tr("Please choose a new encoding for all tables.");
    else
        question = tr("Please choose a new encoding for this table.");
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

            for(auto it=browseTableSettings.begin();it!=browseTableSettings.end();++it)
                it.value().encoding = encoding;
        }
    }
}

void MainWindow::browseDataSetDefaultTableEncoding()
{
    browseDataSetTableEncoding(true);
}

void MainWindow::fileOpenReadOnly()
{
    // Redirect to 'standard' fileOpen(), with the read only flag set
    fileOpen(QString(), false, true);
}

void MainWindow::unlockViewEditing(bool unlock, QString pk)
{
    QString currentTable = ui->comboBrowseTable->currentText();

    // If this isn't a view just unlock editing and return
    if(db.getObjectByName(currentTable)->type() != sqlb::Object::View)
    {
        m_browseTableModel->setPseudoPk(QString());
        enableEditing(true, true);
        return;
    }

    // If the view gets unlocked for editing and we don't have a 'primary key' for this view yet, then ask for one
    if(unlock && pk.isEmpty())
    {
        while(true)
        {
            // Ask for a PK
            pk = QInputDialog::getText(this, qApp->applicationName(), tr("Please enter a pseudo-primary key in order to enable editing on this view. "
                                                                         "This should be the name of a unique column in the view."));

            // Cancelled?
            if(pk.isEmpty())
                return;

            // Do some basic testing of the input and if the input appears to be good, go on
            if(db.executeSQL(QString("SELECT %1 FROM %2 LIMIT 1;").arg(sqlb::escapeIdentifier(pk)).arg(sqlb::escapeIdentifier(currentTable)), false, true))
                break;
        }
    } else if(!unlock) {
        // Locking the view is done by unsetting the pseudo-primary key
        pk.clear();
    }

    // (De)activate editing
    enableEditing(unlock, false);
    m_browseTableModel->setPseudoPk(pk);

    // Update checked status of the popup menu action
    ui->actionUnlockViewEditing->setChecked(unlock);

    // Save settings for this table
    browseTableSettings[currentTable].unlockViewPk = pk;
}
