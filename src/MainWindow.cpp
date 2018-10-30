#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Application.h"
#include "EditIndexDialog.h"
#include "AboutDialog.h"
#include "EditTableDialog.h"
#include "AddRecordDialog.h"
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
#include "FindReplaceDialog.h"
#include "Data.h"

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
#include <QUrlQuery>
#include <QDataStream>      // This include seems to only be necessary for the Windows build
#include <QPrinter>
#include <QPrintPreviewDialog>

#ifdef Q_OS_MACX //Needed only on macOS
    #include <QOpenGLWidget>
#endif

// These are needed for reading and writing object files
QDataStream& operator>>(QDataStream& ds, sqlb::ObjectIdentifier& objid)
{
    // Read in the item
    QVariant v;
    ds >> v;

    // If it is a string list, we can treat it as an object identifier. If it isn't, we assume it's just a
    // single string and use interpret it as the table name in the main schema. This is done for backwards
    // compatability with old project file formats.
    if(v.toStringList().isEmpty())
        objid = sqlb::ObjectIdentifier("main", v.toString());
    else
        objid = sqlb::ObjectIdentifier(v);
    return ds;
}

// These are temporary helper functions to turn a vector of sorted columns into a single column to sort and vice verse. This is done by just taking the
// first sort column there is and ignoring all the others or creating a single item vector respectively. These functions can be removed once all parts
// of the application have been converted to deal with vectors of sorted columns.
void fromSortOrderVector(const QVector<BrowseDataTableSettings::SortedColumn>& vector, int& index, Qt::SortOrder& mode)
{
    if(vector.size())
    {
        index = vector.at(0).index;
        mode = vector.at(0).mode;
    } else {
        index = 0;
        mode = Qt::AscendingOrder;
    }
}
QVector<BrowseDataTableSettings::SortedColumn> toSortOrderVector(int index, Qt::SortOrder mode)
{
    QVector<BrowseDataTableSettings::SortedColumn> vector;
    vector.push_back(BrowseDataTableSettings::SortedColumn(index, mode));
    return vector;
}


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      db(),
      m_browseTableModel(new SqliteTableModel(db, this, Settings::getValue("db", "prefetchsize").toInt())),
      m_currentTabTableModel(m_browseTableModel),
      m_remoteDb(new RemoteDatabase),
      editDock(new EditDialog(this)),
      plotDock(new PlotDock(this)),
      remoteDock(new RemoteDock(this)),
      findReplaceDialog(new FindReplaceDialog(this)),
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

#ifdef Q_OS_MACX
    // Add OpenGL Context for macOS
    QOpenGLWidget *ogl = new QOpenGLWidget(this);
    ui->horizontalLayout->addWidget(ogl);
    ogl->setHidden(true);
#endif

    // Connect SQL logging and database state setting to main window
    connect(&db, SIGNAL(dbChanged(bool)), this, SLOT(dbState(bool)));
    connect(&db, SIGNAL(sqlExecuted(QString, int)), this, SLOT(logSql(QString,int)));
    connect(&db, &DBBrowserDB::requestCollation, this, &MainWindow::requestCollation);

    // Set the validator for the goto line edit
    ui->editGoto->setValidator(gotoValidator);

    // Set up filters
    connect(ui->dataTable->filterHeader(), SIGNAL(filterChanged(int,QString)), this, SLOT(updateFilter(int,QString)));
    connect(m_browseTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataTableSelectionChanged(QModelIndex)));

    // Select in table the rows correspoding to the selected points in plot
    connect(plotDock, SIGNAL(pointsSelected(int,int)), ui->dataTable, SLOT(selectTableLines(int,int)));

    // Set up DB structure tab
    dbStructureModel = new DbStructureModel(db, this);
    connect(&db, &DBBrowserDB::structureUpdated, [this]() {
        QString old_table = ui->comboBrowseTable->currentText();
        dbStructureModel->reloadData();
        populateStructure(old_table);
    });
    ui->dbTreeWidget->setModel(dbStructureModel);
    ui->dbTreeWidget->setColumnWidth(DbStructureModel::ColumnName, 300);
    ui->dbTreeWidget->setColumnHidden(DbStructureModel::ColumnObjectType, true);
    ui->dbTreeWidget->setColumnHidden(DbStructureModel::ColumnSchema, true);

    // Set up DB schema dock
    ui->treeSchemaDock->setModel(dbStructureModel);
    ui->treeSchemaDock->setColumnHidden(DbStructureModel::ColumnObjectType, true);
    ui->treeSchemaDock->setColumnHidden(DbStructureModel::ColumnSchema, true);

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

    // Add print shortcut for the DB Structure tab (dbTreeWidget) with context to the widget, so other print shortcuts aren't eclipsed.
    QShortcut* shortcutPrint = new QShortcut(QKeySequence(QKeySequence::Print), ui->dbTreeWidget, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutPrint, &QShortcut::activated, this, &MainWindow::printDbStructure);

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

    popupSchemaDockMenu = new QMenu(this);
    popupSchemaDockMenu->addAction(ui->actionDropQualifiedCheck);
    popupSchemaDockMenu->addAction(ui->actionEnquoteNamesCheck);

    popupOpenDbMenu = new QMenu(this);
    popupOpenDbMenu->addAction(ui->fileOpenAction);
    popupOpenDbMenu->addAction(ui->fileOpenReadOnlyAction);
    ui->fileOpenActionPopup->setMenu(popupOpenDbMenu);

    popupNewRecordMenu = new QMenu(this);
    popupNewRecordMenu->addAction(ui->newRecordAction);
    popupNewRecordMenu->addAction(ui->insertValuesAction);
    ui->buttonNewRecord->setMenu(popupNewRecordMenu);

    popupSaveSqlFileMenu = new QMenu(this);
    popupSaveSqlFileMenu->addAction(ui->actionSqlSaveFile);
    popupSaveSqlFileMenu->addAction(ui->actionSqlSaveFileAs);
    ui->actionSqlSaveFilePopup->setMenu(popupSaveSqlFileMenu);

    popupSaveSqlResultsMenu = new QMenu(this);
    popupSaveSqlResultsMenu->addAction(ui->actionSqlResultsExportCsv);
    popupSaveSqlResultsMenu->addAction(ui->actionSqlResultsSaveAsView);
    ui->actionSqlResultsSave->setMenu(popupSaveSqlResultsMenu);
    qobject_cast<QToolButton*>(ui->toolbarSql->widgetForAction(ui->actionSqlResultsSave))->setPopupMode(QToolButton::InstantPopup);

    popupSaveFilterAsMenu = new QMenu(this);
    popupSaveFilterAsMenu->addAction(ui->actionFilteredTableExportCsv);
    popupSaveFilterAsMenu->addAction(ui->actionFilterSaveAsView);
    ui->buttonSaveFilterAsPopup->setMenu(popupSaveFilterAsMenu);
    ui->buttonSaveFilterAsPopup->setPopupMode(QToolButton::InstantPopup);

    popupBrowseDataHeaderMenu = new QMenu(this);
    popupBrowseDataHeaderMenu->addAction(ui->actionShowRowidColumn);
    popupBrowseDataHeaderMenu->addAction(ui->actionHideColumns);
    popupBrowseDataHeaderMenu->addAction(ui->actionShowAllColumns);
    popupBrowseDataHeaderMenu->addSeparator();
    popupBrowseDataHeaderMenu->addAction(ui->actionUnlockViewEditing);
    popupBrowseDataHeaderMenu->addAction(ui->actionBrowseTableEditDisplayFormat);
    popupBrowseDataHeaderMenu->addSeparator();
    popupBrowseDataHeaderMenu->addAction(ui->actionSetTableEncoding);
    popupBrowseDataHeaderMenu->addAction(ui->actionSetAllTablesEncoding);

    QShortcut* dittoRecordShortcut = new QShortcut(QKeySequence("Ctrl+\""), this);
    connect(dittoRecordShortcut, &QShortcut::activated, [this]() {
        int currentRow = ui->dataTable->currentIndex().row();
        duplicateRecord(currentRow);
    });

    // Add menu item for log dock
    ui->viewMenu->insertAction(ui->viewDBToolbarAction, ui->dockLog->toggleViewAction());
    ui->viewMenu->actions().at(0)->setShortcut(QKeySequence(tr("Ctrl+L")));
    ui->viewMenu->actions().at(0)->setIcon(QIcon(":/icons/log_dock"));

    // Add menu item for plot dock
    ui->viewMenu->insertAction(ui->viewDBToolbarAction, ui->dockPlot->toggleViewAction());
    ui->viewMenu->actions().at(1)->setShortcut(QKeySequence(tr("Ctrl+D")));
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

    // Set checked state if toolbar is visible
    ui->viewDBToolbarAction->setChecked(!ui->toolbarDB->isHidden());
    ui->viewExtraDBToolbarAction->setChecked(!ui->toolbarExtraDB->isHidden());
    ui->viewProjectToolbarAction->setChecked(!ui->toolbarProject->isHidden());

    // Add separator between docks and toolbars
    ui->viewMenu->insertSeparator(ui->viewDBToolbarAction);

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

    // When changing the text of the toolbar actions, also automatically change their icon text and their tooltip text
    connect(ui->editModifyObjectAction, &QAction::changed, [=]() {
        ui->editModifyObjectAction->setIconText(ui->editModifyObjectAction->text());
        ui->editModifyObjectAction->setToolTip(ui->editModifyObjectAction->text());
    });
    connect(ui->editDeleteObjectAction, &QAction::changed, [=]() {
        ui->editDeleteObjectAction->setIconText(ui->editDeleteObjectAction->text());
        ui->editDeleteObjectAction->setToolTip(ui->editDeleteObjectAction->text());
    });

    // Connect some more signals and slots
    connect(ui->dataTable->filterHeader(), SIGNAL(sectionClicked(int)), this, SLOT(browseTableHeaderClicked(int)));
    connect(ui->dataTable->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setRecordsetLabel()));
    connect(ui->dataTable->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(updateBrowseDataColumnWidth(int,int,int)));
    connect(editDock, SIGNAL(recordTextUpdated(QPersistentModelIndex, QByteArray, bool)), this, SLOT(updateRecordText(QPersistentModelIndex, QByteArray, bool)));
    connect(ui->dbTreeWidget->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(changeTreeSelection()));
    connect(ui->dataTable->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showDataColumnPopupMenu(QPoint)));
    connect(ui->dataTable->verticalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showRecordPopupMenu(QPoint)));
    connect(ui->dataTable, SIGNAL(openFileFromDropEvent(QString)), this, SLOT(fileOpen(QString)));
    connect(ui->dockEdit, SIGNAL(visibilityChanged(bool)), this, SLOT(toggleEditDock(bool)));
    connect(m_remoteDb, SIGNAL(openFile(QString)), this, SLOT(fileOpen(QString)));
    connect(m_remoteDb, &RemoteDatabase::gotCurrentVersion, this, &MainWindow::checkNewVersion);
    connect(m_browseTableModel, &SqliteTableModel::finishedFetch, this, &MainWindow::setRecordsetLabel);
    connect(ui->dataTable, &ExtendedTableWidget::selectedRowsToBeDeleted, this, &MainWindow::deleteRecord);
    connect(ui->actionDropQualifiedCheck, &QAction::toggled, dbStructureModel, &DbStructureModel::setDropQualifiedNames);
    connect(ui->actionEnquoteNamesCheck, &QAction::toggled, dbStructureModel, &DbStructureModel::setDropEnquotedNames);
    ui->actionDropQualifiedCheck->setChecked(Settings::getValue("SchemaDock", "dropQualifiedNames").toBool());
    ui->actionEnquoteNamesCheck->setChecked(Settings::getValue("SchemaDock", "dropEnquotedNames").toBool());

    connect(m_browseTableModel, &SqliteTableModel::finishedFetch, [this](){
        auto & settings = browseTableSettings[currentlyBrowsedTableName()];
        plotDock->updatePlot(m_browseTableModel, &settings, true, false);
    });

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

    // Connect tool pragmas
    connect(ui->actionIntegrityCheck, &QAction::triggered, [this]() {
            runSqlNewTab("PRAGMA integrity_check;", ui->actionIntegrityCheck->text());
    });
    connect(ui->actionQuickCheck, &QAction::triggered, [this]() {
            runSqlNewTab("PRAGMA quick_check;", ui->actionQuickCheck->text());
    });
    connect(ui->actionForeignKeyCheck, &QAction::triggered, [this]() {
            runSqlNewTab("PRAGMA foreign_key_check;", ui->actionForeignKeyCheck->text());
    });
    connect(ui->actionOptimize, &QAction::triggered, [this]() {
            runSqlNewTab("PRAGMA optimize;", ui->actionOptimize->text());
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
        m_remoteDb->fetch("https://download.sqlitebrowser.org/currentrelease",
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
        populateTable();
        openSqlTab(true);
        createTable();
    }
}

void MainWindow::fileNewInMemoryDatabase()
{
    db.create(":memory:");
    setCurrentFile(tr("In-Memory database"));
    statusEncodingLabel->setText(db.getPragma("encoding"));
    statusEncryptionLabel->setVisible(false);
    statusReadOnlyLabel->setVisible(false);
    populateTable();
    openSqlTab(true);
    createTable();
}

void MainWindow::populateStructure(const QString& old_table)
{
    // Refresh the structure tab
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
    SqlUiLexer::QualifiedTablesMap qualifiedTablesMap;
    for(auto it=db.schemata.constBegin();it!=db.schemata.constEnd();++it)
    {
        SqlUiLexer::TablesAndColumnsMap tablesToColumnsMap;
        objectMap tab = db.getBrowsableObjects(it.key());
        for(auto it : tab)
        {
            QString objectname = it->name();

            sqlb::FieldInfoList fi = it->fieldInformation();
            for(const sqlb::FieldInfo& f : fi)
                tablesToColumnsMap[objectname].append(f.name);
        }
        qualifiedTablesMap[it.key()] = tablesToColumnsMap;
    }
    SqlTextEdit::sqlLexer->setTableNames(qualifiedTablesMap);
    ui->editLogApplication->reloadKeywords();
    ui->editLogUser->reloadKeywords();
    for(int i=0;i<ui->tabSqlAreas->count();i++)
        qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i))->getEditor()->reloadKeywords();

    // Resize SQL column to fit contents
    ui->dbTreeWidget->resizeColumnToContents(DbStructureModel::ColumnSQL);
    ui->treeSchemaDock->resizeColumnToContents(DbStructureModel::ColumnSQL);
    // Resize also the Name column in the Dock since it has usually
    // short content and there is little space there.
    ui->treeSchemaDock->resizeColumnToContents(DbStructureModel::ColumnName);


}

void MainWindow::clearTableBrowser()
{
    if (!ui->dataTable->model())
        return;

    ui->dataTable->setModel(nullptr);
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
    sqlb::ObjectIdentifier tablename = currentlyBrowsedTableName();

    // Set model
    bool reconnectSelectionSignals = false;
    if(ui->dataTable->model() == nullptr)
        reconnectSelectionSignals = true;
    ui->dataTable->setModel(m_browseTableModel);
    if(reconnectSelectionSignals)
    {
        connect(ui->dataTable->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(dataTableSelectionChanged(QModelIndex)));
        connect(ui->dataTable->selectionModel(), &QItemSelectionModel::selectionChanged, [this](const QItemSelection&, const QItemSelection&) {
            updateInsertDeleteRecordButton();
        });
    }
    // Search stored table settings for this table
    bool storedDataFound = browseTableSettings.contains(tablename);

    // Set new table
    if(!storedDataFound)
    {
        // No stored settings found.

        // Set table name and apply default display format settings
        m_browseTableModel->setTable(tablename, 0, Qt::AscendingOrder);

        // There aren't any information stored for this table yet, so use some default values

        // Hide rowid column. Needs to be done before the column widths setting because of the workaround in there
        showRowidColumn(false);

        // Unhide all columns by default
        on_actionShowAllColumns_triggered();

        // Enable editing in general, but lock view editing
        unlockViewEditing(false);

        // Column widths
        for(int i=1;i<m_browseTableModel->columnCount();i++)
            ui->dataTable->setColumnWidth(i, ui->dataTable->horizontalHeader()->defaultSectionSize());

        // Sorting
        ui->dataTable->filterHeader()->setSortIndicator(0, Qt::AscendingOrder);

        // Encoding
        m_browseTableModel->setEncoding(defaultBrowseTableEncoding);

        setRecordsetLabel();

        // Plot
        attachPlot(ui->dataTable, m_browseTableModel, &browseTableSettings[tablename]);

        // The filters can be left empty as they are
    } else {
        // Stored settings found. Retrieve them.
        BrowseDataTableSettings storedData = browseTableSettings[tablename];

        // Load display formats and set them along with the table name
        QVector<QString> v;
        bool only_defaults = true;
        if(db.getObjectByName(tablename))
        {
            const sqlb::FieldInfoList& tablefields = db.getObjectByName(tablename)->fieldInformation();
            for(size_t i=0; i<tablefields.size(); ++i)
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
        }

        int sortOrderIndex;
        Qt::SortOrder sortOrderMode;
        fromSortOrderVector(storedData.sortOrder, sortOrderIndex, sortOrderMode);
        if(only_defaults)
            m_browseTableModel->setTable(tablename, sortOrderIndex, sortOrderMode, storedData.filterValues);
        else
            m_browseTableModel->setTable(tablename, sortOrderIndex, sortOrderMode, storedData.filterValues, v);

        // There is information stored for this table, so extract it and apply it
        applyBrowseTableSettings(storedData);

        setRecordsetLabel();

        // Plot
        attachPlot(ui->dataTable, m_browseTableModel, &browseTableSettings[tablename], false);
    }

    // Show/hide menu options depending on whether this is a table or a view
    if(db.getObjectByName(currentlyBrowsedTableName()) && db.getObjectByName(currentlyBrowsedTableName())->type() == sqlb::Object::Table)
    {
        // Table
        sqlb::TablePtr table = db.getObjectByName<sqlb::Table>(currentlyBrowsedTableName());
        ui->actionUnlockViewEditing->setVisible(false);
        ui->actionShowRowidColumn->setVisible(!table->isWithoutRowidTable());
    } else {
        // View
        ui->actionUnlockViewEditing->setVisible(true);
        ui->actionShowRowidColumn->setVisible(false);
    }

    updateInsertDeleteRecordButton();

    QApplication::restoreOverrideCursor();
}

void MainWindow::applyBrowseTableSettings(BrowseDataTableSettings storedData, bool skipFilters)
{
    // We don't want to pass storedData by reference because the functions below would change the referenced data in their original
    // place, thus modifiying the data this function can use. To have a static description of what the view should look like we want
    // a copy here.

    // Show rowid column. Needs to be done before the column widths setting because of the workaround in there and before the filter setting
    // because of the filter row generation.
    showRowidColumn(storedData.showRowid, skipFilters);

    // Enable editing in general and (un)lock view editing depending on the settings
    unlockViewEditing(!storedData.unlockViewPk.isEmpty(), storedData.unlockViewPk);

    // Column hidden status
    on_actionShowAllColumns_triggered();
    for(auto hiddenIt=storedData.hiddenColumns.constBegin();hiddenIt!=storedData.hiddenColumns.constEnd();++hiddenIt)
        hideColumns(hiddenIt.key(), hiddenIt.value());

    // Column widths
    for(auto widthIt=storedData.columnWidths.constBegin();widthIt!=storedData.columnWidths.constEnd();++widthIt)
        ui->dataTable->setColumnWidth(widthIt.key(), widthIt.value());

    // Sorting
    int sortOrderIndex;
    Qt::SortOrder sortOrderMode;
    fromSortOrderVector(storedData.sortOrder, sortOrderIndex, sortOrderMode);
    ui->dataTable->filterHeader()->setSortIndicator(sortOrderIndex, sortOrderMode);

    // Filters
    if(!skipFilters)
    {
        // Set filters blocking signals, since the filter is already applied to the browse table model
        FilterTableHeader* filterHeader = qobject_cast<FilterTableHeader*>(ui->dataTable->horizontalHeader());
        bool oldState = filterHeader->blockSignals(true);
        for(auto filterIt=storedData.filterValues.constBegin();filterIt!=storedData.filterValues.constEnd();++filterIt)
            filterHeader->setFilter(filterIt.key(), filterIt.value());
        filterHeader->blockSignals(oldState);
    }

    // Encoding
    m_browseTableModel->setEncoding(storedData.encoding);
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

    // Reset the plot dock model and connection
    attachPlot(nullptr, nullptr);

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
        Settings::setValue("SchemaDock", "dropQualifiedNames", ui->actionDropQualifiedCheck->isChecked());
        Settings::setValue("SchemaDock", "dropEnquotedNames", ui->actionEnquoteNamesCheck->isChecked());

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
        // Error inserting empty row.
        // User has to provide values acomplishing the constraints. Open Add Record Dialog.
        insertValues();
    }
}

void MainWindow::insertValues()
{
    AddRecordDialog dialog(db, currentlyBrowsedTableName(), this);
    if (dialog.exec())
        populateTable();
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

        if(old_row > m_browseTableModel->rowCount())
            old_row = m_browseTableModel->rowCount();
        selectTableLine(old_row);
    } else {
        QMessageBox::information( this, QApplication::applicationName(), tr("Please select a record first"));
    }
}

void MainWindow::attachPlot(ExtendedTableWidget* tableWidget, SqliteTableModel* model, BrowseDataTableSettings* settings, bool keepOrResetSelection)
{
    plotDock->updatePlot(model, settings, true, keepOrResetSelection);
    // Disconnect previous connection
    disconnect(plotDock, SIGNAL(pointsSelected(int,int)), nullptr, nullptr);
    if(tableWidget) {
        // Connect plot selection to the current table results widget.
        connect(plotDock, SIGNAL(pointsSelected(int,int)), tableWidget, SLOT(selectTableLines(int,int)));
        connect(tableWidget, SIGNAL(destroyed()), plotDock, SLOT(resetPlot()));

    }
}

void MainWindow::selectTableLine(int lineToSelect)
{
    ui->dataTable->selectTableLine(lineToSelect);
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
    if(curRow >= m_browseTableModel->rowCount())
        curRow = m_browseTableModel->rowCount() - 1;
    selectTableLine(curRow);
}

void MainWindow::navigateBegin()
{
    selectTableLine(0);
}

void MainWindow::navigateEnd()
{
    selectTableLine(m_browseTableModel->rowCount()-1);
}


void MainWindow::navigateGoto()
{
    int row = ui->editGoto->text().toInt();
    if(row <= 0)
        row = 1;
    if(row > m_browseTableModel->rowCount())
        row = m_browseTableModel->rowCount();

    selectTableLine(row - 1);
    ui->editGoto->setText(QString::number(row));
}

void MainWindow::setRecordsetLabel()
{
    // Get all the numbers, i.e. the number of the first row and the last row as well as the total number of rows
    int from = ui->dataTable->verticalHeader()->visualIndexAt(0) + 1;
    int total = m_browseTableModel->rowCount();
    int to = ui->dataTable->verticalHeader()->visualIndexAt(ui->dataTable->height()) - 1;
    if (to == -2)
        to = total;

    // Update the validator of the goto row field
    gotoValidator->setRange(0, total);

    // Update the label showing the current position
    QString txt;
    switch(m_browseTableModel->rowCountAvailable())
    {
    case SqliteTableModel::RowCount::Unknown:
        txt = tr("determining row count...");
        break;
    case SqliteTableModel::RowCount::Partial:
        txt = tr("%1 - %2 of >= %3").arg(from).arg(to).arg(total);
        break;
    case SqliteTableModel::RowCount::Complete:
    default:
        txt = tr("%1 - %2 of %3").arg(from).arg(to).arg(total);
        break;
    }
    ui->labelRecordset->setText(txt);

    enableEditing(m_browseTableModel->rowCountAvailable() != SqliteTableModel::RowCount::Unknown);
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

    EditTableDialog dialog(db, sqlb::ObjectIdentifier(), true, this);
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

    EditIndexDialog dialog(db, sqlb::ObjectIdentifier(), true, this);
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
    sqlb::ObjectIdentifier name(ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnSchema), Qt::EditRole).toString(),
                                ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnName), Qt::EditRole).toString());
    QString type = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnObjectType), Qt::EditRole).toString();

    // Due to different grammar in languages (e.g. gender or declension), each message must be given separately to translation.
    QString message;
    if (type == "table")
        message = tr("Are you sure you want to delete the table '%1'?\nAll data associated with the table will be lost.");
    else if (type == "view")
        message = tr("Are you sure you want to delete the view '%1'?");
    else if (type == "trigger")
        message = tr("Are you sure you want to delete the trigger '%1'?");
    else if (type == "index")
        message = tr("Are you sure you want to delete the index '%1'?");

    // Ask user if he really wants to delete that table
    if(QMessageBox::warning(this, QApplication::applicationName(), message.arg(name.name()),
                            QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        // Delete the table
        QString statement = QString("DROP %1 %2;").arg(type.toUpper()).arg(name.toString());
        if(!db.executeSQL(statement))
        {
            if (type == "table")
                message = tr("Error: could not delete the table.");
            else if (type == "view")
                message = tr("Error: could not delete the view.");
            else if (type == "trigger")
                message = tr("Error: could not delete the trigger.");
            else if (type == "index")
                message = tr("Error: could not delete the index.");

            QString error = tr("Message from database engine:\n%1").arg(db.lastError());
            QMessageBox::warning(this, QApplication::applicationName(), message + " " + error);
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
    sqlb::ObjectIdentifier name(ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnSchema), Qt::EditRole).toString(),
                                ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnName), Qt::EditRole).toString());
    QString type = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnObjectType), Qt::EditRole).toString();

    if(type == "table")
    {
        // For a safe and possibly complex table modification we must follow the steps documented in
        // https://www.sqlite.org/lang_altertable.html
        // Paragraph (first procedure): Making Other Kinds Of Table Schema Changes

        QString foreign_keys = db.getPragma("foreign_keys");
        if (foreign_keys == "1") {
            if(db.getDirty() && QMessageBox::question(this,
                                     QApplication::applicationName(),
                                     tr("Editing the table requires to save all pending changes now.\nAre you sure you want to save the database?"),
                                     QMessageBox::Save | QMessageBox::Default,
                                     QMessageBox::Cancel | QMessageBox::Escape) != QMessageBox::Save)
                return;
            // Commit all changes so the foreign_keys can be effective.
            fileSave();
            db.setPragma("foreign_keys", "0");
        }

        EditTableDialog dialog(db, name, false, this);
        bool ok = dialog.exec();

        // If foreign_keys were enabled, we must commit or rollback the transaction so the foreign_keys pragma can be restored.
        if (foreign_keys == "1") {
            if (!db.querySingleValueFromDb(QString("PRAGMA %1.foreign_key_check").arg(sqlb::escapeIdentifier(name.schema()))).isNull()) {
                // Raise warning for accepted modification. When rejected, warn user also since we know now that the table has problems,
                // but it wasn't our fault.
                if (ok)
                    QMessageBox::warning(this, QApplication::applicationName(),
                                         tr("Error checking foreign keys after table modification. The changes will be reverted."));
                else
                    QMessageBox::warning(this, QApplication::applicationName(),
                                         tr("This table did not pass a foreign-key check.<br/>"
                                            "You should run 'Tools | Foreign-Key Check' and fix the reported issues."));
                db.revertAll();
            } else {
                // Commit all changes so the foreign_keys can be effective.
                fileSave();
            }
            db.setPragma("foreign_keys", foreign_keys);
        }
        if(ok) {
            ui->dataTable->filterHeader()->clearFilters();
            populateTable();
        }
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
            (db.getObjectByName(currentlyBrowsedTableName())->type() == sqlb::Object::Types::Table);

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
            (db.getObjectByName(currentlyBrowsedTableName())->type() == sqlb::Object::Types::Table);

    // Don't allow editing of other objects than tables
    editDock->setReadOnly(!editingAllowed);

    // If the Edit Cell dock is visible, load the new value into it
    if (editDock->isVisible()) {
        editDock->setCurrentIndex(index);
    }
}

MainWindow::StatementType MainWindow::getQueryType(const QString& query) const
{
    // Helper function for getting the type of a given query

    if(query.startsWith("SELECT", Qt::CaseInsensitive)) return SelectStatement;
    if(query.startsWith("ALTER", Qt::CaseInsensitive)) return AlterStatement;
    if(query.startsWith("DROP", Qt::CaseInsensitive)) return DropStatement;
    if(query.startsWith("ROLLBACK", Qt::CaseInsensitive)) return RollbackStatement;
    if(query.startsWith("PRAGMA", Qt::CaseInsensitive)) return PragmaStatement;
    if(query.startsWith("VACUUM", Qt::CaseInsensitive)) return VacuumStatement;
    if(query.startsWith("INSERT", Qt::CaseInsensitive)) return InsertStatement;
    if(query.startsWith("UPDATE", Qt::CaseInsensitive)) return UpdateStatement;
    if(query.startsWith("DELETE", Qt::CaseInsensitive)) return DeleteStatement;
    if(query.startsWith("CREATE", Qt::CaseInsensitive)) return CreateStatement;
    if(query.startsWith("ATTACH", Qt::CaseInsensitive)) return AttachStatement;
    if(query.startsWith("DETACH", Qt::CaseInsensitive)) return DetachStatement;

    return OtherStatement;
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

    // Get current SQL tab and editor
    SqlExecutionArea* sqlWidget = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());
    SqlTextEdit* editor = sqlWidget->getEditor();
    const QString tabName = ui->tabSqlAreas->tabText(ui->tabSqlAreas->currentIndex()).remove('&');

    // Determine execution mode: execute all, execute selection or execute current line
    enum executionMode
    {
        All,
        Selection,
        Line
    };
    executionMode mode;
    if(sender() && sender()->objectName() == "actionSqlExecuteLine")
        mode = Line;
    else if(!sqlWidget->getSelectedSql().isEmpty())
        mode = Selection;
    else
        mode = All;

    // Get SQL code to execute. This depends on the execution mode.
    QString query = sqlWidget->getSql();
    int execute_from_line = 0;           // These three variables hold the start position
    int execute_from_index = 0;          // of the executed statements in the entire
    int execute_from_position = 0;       // SQL code of the current SQL tab.
    int execute_to_line = 0;             // These three variables hold the end position
    int execute_to_index = 0;            // of the executed statements in the entire
    int execute_to_position = 0;         // SQL code of the current SQL tab.

    switch(mode)
    {
    case Selection:
        {
            // Start and end positions are start and end positions from the selection
            editor->getSelection(&execute_from_line, &execute_from_index, &execute_to_line, &execute_to_index);
            execute_from_position = editor->positionFromLineIndex(execute_from_line, execute_from_index);
            execute_to_position = editor->positionFromLineIndex(execute_to_line, execute_to_index);
            db.logSQL(tr("-- EXECUTING SELECTION IN '%1'\n--").arg(tabName), kLogMsg_User);
        } break;
    case Line:
        {
            // Start position is the first character of the current line, except for those cases where we're in the middle of a
            // statement which started on one the previous line. In that case the start position is actually a bit earlier. For
            // the end position we set the last character of the current line. If the statement(s) continue(s) into the next line,
            // SQLite will execute it/them anyway and we'll stop afterwards.
            int dummy;
            editor->getCursorPosition(&execute_from_line, &dummy);
            execute_from_position = editor->positionFromLineIndex(execute_from_line, 0);

            // Need to set the end position here before adjusting the start line
            execute_to_line = execute_from_line;
            execute_to_index = editor->text(execute_to_line).length() - 1;     // The -1 compensates for the line break at the end of the line
            execute_to_position = editor->positionFromLineIndex(execute_to_line, execute_to_index);

            QByteArray firstPartEntireSQL = query.toUtf8().left(execute_from_position);
            if(firstPartEntireSQL.lastIndexOf(';') != -1)
            {
                execute_from_position -= firstPartEntireSQL.length() - firstPartEntireSQL.lastIndexOf(';') - 1;
                editor->lineIndexFromPosition(execute_from_position, &execute_from_line, &execute_from_index);
            }
            db.logSQL(tr("-- EXECUTING LINE IN '%1'\n--").arg(tabName), kLogMsg_User);
        } break;
    case All:
        {
            // Start position is the first byte, end position the last.
            // Note that we use byte positions that might differ from character positions.
            execute_to_position = editor->length();
            editor->lineIndexFromPosition(execute_to_position, &execute_to_line, &execute_to_index);
            db.logSQL(tr("-- EXECUTING ALL IN '%1'\n--").arg(tabName), kLogMsg_User);
        } break;
    }

    // Cancel if there is nothing to execute
    if(query.trimmed().isEmpty() || query.trimmed() == ";" || execute_from_position == execute_to_position ||
            query.mid(execute_from_position, execute_to_position-execute_from_position).trimmed().isEmpty() ||
            query.mid(execute_from_position, execute_to_position-execute_from_position).trimmed() == ";")
        return;

    // All replacements in the query should be made by the same amount of characters, so the positions in the file
    // for error indicators and line and column logs are not displaced.
    // Whitespace and comments are discarded by SQLite, so it is better to just let it ignore them.
    query = query.replace(QRegExp("^(\\s*)BEGIN TRANSACTION;", Qt::CaseInsensitive), "\\1                  ");
    query = query.replace(QRegExp("COMMIT;(\\s*)$", Qt::CaseInsensitive), "       \\1");

    // Convert query to C string which we will use from now on, starting from the determined start position and
    // until the end of the SQL code. By doing so we go further than the determined end position because in Line
    // mode the last statement might go beyond that point.
    QByteArray utf8Query = query.toUtf8().mid(execute_from_position);

    // Remove any error indicators
    editor->clearErrorIndicators();

    // Set cursor and start execution timer
    QApplication::setOverrideCursor(Qt::BusyCursor);
    QElapsedTimer timer;
    timer.start();

    // Prepare execution
    sqlite3_stmt* vm;
    const char* tail = utf8Query.data();
    int sql3status = SQLITE_OK;
    int tail_length = utf8Query.length();
    QString statusMessage;
    bool ok = false;
    bool modified = false;
    const bool wasdirty = db.getDirty();
    bool structure_updated = false;
    bool savepoint_created = false;

    // Accept multi-line queries, by looping until the tail is empty
    while(tail && *tail != 0 && (sql3status == SQLITE_OK || sql3status == SQLITE_DONE))
    {
        // What type of query is this?
        QString qtail = QString(tail).trimmed();
        StatementType query_type = getQueryType(qtail);

        // Check whether the DB structure is changed by this statement
        if(!structure_updated && (query_type == AlterStatement ||
                query_type == CreateStatement ||
                query_type == DropStatement ||
                query_type == RollbackStatement))
            structure_updated = true;

        // Check whether this is trying to set a pragma or to vacuum the database
        if((query_type == PragmaStatement && qtail.contains('=') && !qtail.contains("defer_foreign_keys", Qt::CaseInsensitive)) || query_type == VacuumStatement)
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
                // We have to start a transaction before we create the prepared statement otherwise every executed
                // statement will get committed after the prepared statement gets finalized
                db.setSavepoint();
                savepoint_created = true;
            }
        }

        // Execute next statement
        int tail_length_before = tail_length;
        const char* qbegin = tail;
        auto pDb = db.get(tr("executing query"));
        sql3status = sqlite3_prepare_v2(pDb.get(), tail, tail_length, &vm, &tail);
        QString queryPart = QString::fromUtf8(qbegin, tail - qbegin);
        tail_length -= (tail - qbegin);
        int end_of_current_statement_position = execute_from_position + tail_length_before - tail_length;

        if (sql3status == SQLITE_OK)
        {
            sql3status = sqlite3_step(vm);
            sqlite3_finalize(vm);

            // Get type
            StatementType query_part_type = getQueryType(queryPart.trimmed());

            // SQLite returns SQLITE_DONE when a valid SELECT statement was executed but returned no results. To run into the branch that updates
            // the status message and the table view anyway manipulate the status value here. This is also done for PRAGMA statements as they (sometimes)
            // return rows just like SELECT statements, too.
            if((query_part_type == SelectStatement || query_part_type == PragmaStatement) && sql3status == SQLITE_DONE)
                sql3status = SQLITE_ROW;

            switch(sql3status)
            {
            case SQLITE_ROW:
            {
                // If we get here, the SQL statement returns some sort of data. So hand it over to the model for display. Don't set the modified flag
                // because statements that display data don't change data as well.
                pDb = nullptr;

                auto * model = sqlWidget->getModel();
                model->setQuery(queryPart);

                // Wait until the initial loading of data (= first chunk and row count) has been performed. I have the
                // feeling that a lot of stuff would need rewriting if we wanted to become more asynchronous here:
                // essentially the entire loop over the commands would need to be signal-driven.
                model->waitUntilIdle();
                qApp->processEvents(); // to make row count available

                statusMessage = tr("%1 rows returned in %2ms").arg(model->rowCount()).arg(timer.elapsed());
                ok = true;
                ui->actionSqlResultsSave->setEnabled(true);
                ui->actionSqlResultsSaveAsView->setEnabled(!db.readOnly());

                sql3status = SQLITE_OK;
                break;
            }
            case SQLITE_DONE:
            case SQLITE_OK:
            {
                // If we get here, the SQL statement doesn't return data and just executes. Don't run it again because it has already been executed.
                // But do set the modified flag because statements that don't return data, often modify the database.

                sqlWidget->getModel()->reset();

                QString stmtHasChangedDatabase;
                if(query_part_type == InsertStatement || query_part_type == UpdateStatement || query_part_type == DeleteStatement)
                    stmtHasChangedDatabase = tr(", %1 rows affected").arg(sqlite3_changes(pDb.get()));

                // Attach/Detach statements don't modify the original database
                if(query_part_type != StatementType::AttachStatement && query_part_type != StatementType::DetachStatement)
                    modified = true;

                statusMessage = tr("query executed successfully. Took %1ms%2").arg(timer.elapsed()).arg(stmtHasChangedDatabase);
                ok = true;
                break;
            }
            case SQLITE_MISUSE:
                continue;
            default:
                ok = false;
                break;
            }
            timer.restart();
        } else {
            ok = false;

        }
        editor->lineIndexFromPosition(execute_from_position, &execute_from_line, &execute_from_index);

        // Special case: if the start position is at the end of a line, then move to the beggining of next line.
        // Otherwise for the typical case, the line reference is one less than expected.
        // Note that execute_from_index uses character positions and not byte positions, so text().length() must be used.
        if (editor->text(execute_from_line).length() == execute_from_index+1) {
            execute_from_line++;
            execute_from_index = 0;
        }

        // If there was an error, save the error message for later and highlight the erroneous SQL statement
        if (!ok)
        {
            statusMessage = QString::fromUtf8(sqlite3_errmsg(pDb.get()));

            int end_of_current_statement_line, end_of_current_statement_index;
            editor->lineIndexFromPosition(end_of_current_statement_position, &end_of_current_statement_line, &end_of_current_statement_index);
            editor->setErrorIndicator(execute_from_line, execute_from_index, end_of_current_statement_line, end_of_current_statement_index);

            editor->setCursorPosition(execute_from_line, execute_from_index);
        }

        // Log the query and the result message.
        // The query takes the last placeholder as it may itself contain the sequence '%' + number.
        statusMessage = QString(tr("-- At line %1:\n%4\n-- Result: %3")).arg(execute_from_line+1).arg(statusMessage).arg(queryPart.trimmed());
        db.logSQL(statusMessage, kLogMsg_User);

        // Release the database
        pDb = nullptr;

        // Revert to save point now if it wasn't needed. We need to do this here because there are some rare cases where the next statement might
        // be affected by what is only a temporary and unnecessary savepoint. For example in this case:
        // ATTACH 'xxx' AS 'db2'
        // SELECT * FROM db2.xy;    -- Savepoint created here
        // DETACH db2;              -- Savepoint makes this statement fail
        if(!modified && !wasdirty && savepoint_created)
        {
            db.revertToSavepoint(); // better rollback, if the logic is not enough we can tune it.
            savepoint_created = false;
        }

        // Update the start position for the next statement and check if we are at
        // the end of the part we want to execute. If so, stop the execution now.
        execute_from_position = end_of_current_statement_position;
        if(execute_from_position >= execute_to_position)
            break;

        // Process events to keep the UI responsive
        qApp->processEvents();
    }

    sqlWidget->finishExecution(statusMessage, ok);
    attachPlot(sqlWidget->getTableResult(), sqlWidget->getModel());

    connect(sqlWidget->getTableResult(), &ExtendedTableWidget::activated, this, &MainWindow::dataTableSelectionChanged);
    connect(sqlWidget->getTableResult(), SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickTable(QModelIndex)));

    // If the DB structure was changed by some command in this SQL script, update our schema representations
    if(structure_updated)
        db.updateSchema();

    QApplication::restoreOverrideCursor();
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
    for(const auto& file : wFiles) {
        if (QFile::exists(file))
            validFiles.append(file);
    }

    if (!validFiles.isEmpty())
    {
        ImportCsvDialog dialog(validFiles, &db, this);
        if (dialog.exec())
            populateTable();
    }
}

void MainWindow::exportTableToCSV()
{
    // Get the current table name if we are in the Browse Data tab
    sqlb::ObjectIdentifier current_table;
    if(ui->mainTab->currentIndex() == StructureTab)
    {
        QString type = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnObjectType)).toString();
        if(type == "table" || type == "view")
        {
            QString schema = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnSchema)).toString();
            QString name = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnName)).toString();
            current_table = sqlb::ObjectIdentifier(schema, name);
        }
    } else if(ui->mainTab->currentIndex() == BrowseTab) {
        current_table = currentlyBrowsedTableName();
    }

    // Open dialog
    ExportDataDialog dialog(db, ExportDataDialog::ExportFormatCsv, this, "", current_table);
    dialog.exec();
}

void MainWindow::exportTableToJson()
{
    // Get the current table name if we are in the Browse Data tab
    sqlb::ObjectIdentifier current_table;
    if(ui->mainTab->currentIndex() == StructureTab)
    {
        QString type = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnObjectType)).toString();
        if(type == "table" || type == "view")
        {
            QString schema = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnSchema)).toString();
            QString name = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnName)).toString();
            current_table = sqlb::ObjectIdentifier(schema, name);
        }
    } else if(ui->mainTab->currentIndex() == BrowseTab) {
        current_table = currentlyBrowsedTableName();
    }

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
    }

    // Defer foreign keys. Just deferring them instead of disabling them should work fine because in the import we only expect CREATE and INSERT
    // statements which unlike in the Edit Table dialog shouldn't trigger any problems.
    QString foreignKeysOldSettings = db.getPragma("defer_foreign_keys");
    db.setPragma("defer_foreign_keys", "1");

    // Open, read, execute and close file
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    bool ok = db.executeMultiSQL(f.readAll(), newDbFile.size() == 0);
    // Restore cursor before asking the user to accept the message
    QApplication::restoreOverrideCursor();
    if(!ok)
        QMessageBox::warning(this, QApplication::applicationName(), tr("Error importing data: %1").arg(db.lastError()));
    else if(db.getPragma("foreign_keys") == "1" && !db.querySingleValueFromDb(QString("PRAGMA foreign_key_check")).isNull())
        QMessageBox::warning(this, QApplication::applicationName(), tr("Import completed. Some foreign key constraints are violated. Please fix them before saving."));
    else
        QMessageBox::information(this, QApplication::applicationName(), tr("Import completed."));
    f.close();

    // Restore the former foreign key settings
    db.setPragma("defer_foreign_keys", foreignKeysOldSettings);

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

//** DB Schema Dock Context Menu
void MainWindow::createSchemaDockContextMenu(const QPoint &qPoint)
{
    popupSchemaDockMenu->exec(ui->treeSchemaDock->mapToGlobal(qPoint));
}

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
        ui->editModifyObjectAction->setText(tr("Modify View"));
    } else if(type == "trigger") {
        ui->editDeleteObjectAction->setText(tr("Delete Trigger"));
        ui->editModifyObjectAction->setText(tr("Modify Trigger"));
    } else if(type == "index") {
        ui->editDeleteObjectAction->setText(tr("Delete Index"));
        ui->editModifyObjectAction->setText(tr("Modify Index"));
    } else if(type == "table") {
        ui->editDeleteObjectAction->setText(tr("Delete Table"));
        ui->editModifyObjectAction->setText(tr("Modify Table"));
    } else {
        // Nothing to do for other types. Set the buttons not visible and return.
        ui->editDeleteObjectAction->setVisible(false);
        ui->editModifyObjectAction->setVisible(false);
        return;
    }

    ui->editDeleteObjectAction->setVisible(true);
    ui->editModifyObjectAction->setVisible(true);

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

    for(QWidget* widget : QApplication::topLevelWidgets()) {
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
    bool tempDb = db.currentFile() == ":memory:";

    ui->fileCloseAction->setEnabled(enable);
    ui->fileAttachAction->setEnabled(enable);
    ui->fileCompactAction->setEnabled(enable && write);
    ui->fileExportJsonAction->setEnabled(enable);
    ui->fileExportCSVAction->setEnabled(enable);
    ui->fileExportSQLAction->setEnabled(enable);
    ui->fileImportCSVAction->setEnabled(enable && write);
    ui->editCreateTableAction->setEnabled(enable && write);
    ui->editCreateIndexAction->setEnabled(enable && write);
    ui->actionDbPrint->setEnabled(enable);
    ui->buttonNext->setEnabled(enable);
    ui->buttonPrevious->setEnabled(enable);
    ui->buttonBegin->setEnabled(enable);
    ui->buttonEnd->setEnabled(enable);
    ui->scrollAreaWidgetContents->setEnabled(enable);
    ui->buttonBoxPragmas->setEnabled(enable && write);
    ui->buttonGoto->setEnabled(enable);
    ui->editGoto->setEnabled(enable);
    ui->buttonRefresh->setEnabled(enable);
    ui->buttonPrintTable->setEnabled(enable);
    ui->actionExecuteSql->setEnabled(enable);
    ui->actionLoadExtension->setEnabled(enable);
    ui->actionSqlExecuteLine->setEnabled(enable);
    ui->actionSaveProject->setEnabled(enable && !tempDb);
    ui->actionEncryption->setEnabled(enable && write && !tempDb);
    ui->actionIntegrityCheck->setEnabled(enable);
    ui->actionQuickCheck->setEnabled(enable);
    ui->actionForeignKeyCheck->setEnabled(enable);
    ui->actionOptimize->setEnabled(enable);
    ui->buttonClearFilters->setEnabled(enable);
    ui->buttonSaveFilterAsPopup->setEnabled(enable);
    ui->dockEdit->setEnabled(enable);
    ui->dockPlot->setEnabled(enable);

    if(!enable)
        ui->actionSqlResultsSave->setEnabled(false);

    updateInsertDeleteRecordButton();
    remoteDock->enableButtons();
}

void MainWindow::enableEditing(bool enable_edit)
{
    // Don't enable anything if this is a read only database
    bool edit = enable_edit && !db.readOnly();

    // Apply settings
    ui->dataTable->setEditTriggers(edit ? QAbstractItemView::SelectedClicked | QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed : QAbstractItemView::NoEditTriggers);
    updateInsertDeleteRecordButton();
}

void MainWindow::browseTableHeaderClicked(int logicalindex)
{
    // Abort if there is more than one column selected because this tells us that the user pretty sure wants to do a range selection instead of sorting data
    if(ui->dataTable->selectionModel()->selectedColumns().count() > 1)
        return;

    // instead of the column name we just use the column index, +2 because 'rowid, *' is the projection
    BrowseDataTableSettings& settings = browseTableSettings[currentlyBrowsedTableName()];
    int dummy;
    Qt::SortOrder order;
    fromSortOrderVector(settings.sortOrder, dummy, order);
    order = order == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder;
    settings.sortOrder = toSortOrderVector(logicalindex, order);
    ui->dataTable->sortByColumn(logicalindex, order);

    // select the first item in the column so the header is bold
    // we might try to select the last selected item
    ui->dataTable->setCurrentIndex(ui->dataTable->currentIndex().sibling(0, logicalindex));

    attachPlot(ui->dataTable, m_browseTableModel, &browseTableSettings[currentlyBrowsedTableName()]);

    // Reapply the view settings. This seems to be necessary as a workaround for newer Qt versions.
    applyBrowseTableSettings(settings);
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
    pragmaValues.case_sensitive_like = db.getPragma("case_sensitive_like").toInt();

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
    ui->comboPragmaPageSize->setCurrentIndex(ui->comboPragmaPageSize->findText(QString::number(pragmaValues.page_size), Qt::MatchFixedString));
    ui->checkboxPragmaRecursiveTriggers->setChecked(pragmaValues.recursive_triggers);
    ui->checkboxPragmaSecureDelete->setChecked(pragmaValues.secure_delete);
    ui->comboboxPragmaSynchronous->setCurrentIndex(pragmaValues.synchronous);
    ui->comboboxPragmaTempStore->setCurrentIndex(pragmaValues.temp_store);
    ui->spinPragmaUserVersion->setValue(pragmaValues.user_version);
    ui->spinPragmaWalAutoCheckpoint->setValue(pragmaValues.wal_autocheckpoint);
    ui->checkboxPragmaCaseSensitiveLike->setChecked(pragmaValues.case_sensitive_like);
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
    db.setPragma("page_size", ui->comboPragmaPageSize->currentText().toInt(), pragmaValues.page_size);
    db.setPragma("recursive_triggers", ui->checkboxPragmaRecursiveTriggers->isChecked(), pragmaValues.recursive_triggers);
    db.setPragma("secure_delete", ui->checkboxPragmaSecureDelete->isChecked(), pragmaValues.secure_delete);
    db.setPragma("synchronous", ui->comboboxPragmaSynchronous->currentIndex(), pragmaValues.synchronous);
    db.setPragma("temp_store", ui->comboboxPragmaTempStore->currentIndex(), pragmaValues.temp_store);
    db.setPragma("user_version", ui->spinPragmaUserVersion->value(), pragmaValues.user_version);
    db.setPragma("wal_autocheckpoint", ui->spinPragmaWalAutoCheckpoint->value(), pragmaValues.wal_autocheckpoint);
    db.setPragma("case_sensitive_like", ui->checkboxPragmaCaseSensitiveLike->isChecked(), pragmaValues.case_sensitive_like);

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
    w->setFindFrameVisibility(ui->actionSqlFind->isChecked());
    w->getEditor()->setFocus();
    connect(w, SIGNAL(findFrameVisibilityChanged(bool)), ui->actionSqlFind, SLOT(setChecked(bool)));

    return index;
}

void MainWindow::changeSqlTab(int /*index*/)
{
    // Instead of figuring out if there are some execution results in the new tab and which statement was used to generate them,
    // we just disable the export buttons in the toolbar.
    ui->actionSqlResultsSave->setEnabled(false);
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
    SqlExecutionArea* sqlarea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());
    if(!sqlarea)
        return;

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

void MainWindow::saveSqlResultsAsCsv()
{
    qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget())->saveAsCsv();
}

void MainWindow::saveSqlResultsAsView()
{
    saveAsView(qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget())->getModel()->query());
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

void MainWindow::reloadSettings()
{
    // Set data browser font
    ui->dataTable->reloadSettings();

    setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(Settings::getValue("General", "toolbarStyle").toInt()));

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
    db.loadExtensionsFromSettings();

    // Refresh view
    dbStructureModel->reloadData();
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

    sqlb::setIdentifierQuoting(static_cast<sqlb::escapeQuoting>(Settings::getValue("editor", "identifier_quotes").toInt()));
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
        int ignmajor = Settings::getValue("checkversion", "ignmajor").toInt();
        int ignminor = Settings::getValue("checkversion", "ignminor").toInt();
        int ignpatch = Settings::getValue("checkversion", "ignpatch").toInt();

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
                Settings::setValue("checkversion", "ignmajor", major);
                Settings::setValue("checkversion", "ignminor", minor);
                Settings::setValue("checkversion", "ignpatch", patch);
            }
        }
    }
}

void MainWindow::on_actionWiki_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/sqlitebrowser/sqlitebrowser/wiki"));
}

// 'Help | Bug Report...' link will set an appropiate body, add the system information and set the label 'bug' automatically to the issue
void MainWindow::on_actionBug_report_triggered()
{
    const QString version = Application::versionString();
    const QString os = QSysInfo::prettyProductName();
    const QString kernelType = QSysInfo::kernelType();
    const QString kernelVersion = QSysInfo::kernelVersion();
    const QString arch = QSysInfo::currentCpuArchitecture();

    QString sqlite_version, sqlcipher_version;
    DBBrowserDB::getSqliteVersion(sqlite_version, sqlcipher_version);
    if(sqlcipher_version.isNull())
        sqlite_version = QString("SQLite Version ") + sqlite_version;
    else
        sqlite_version = QString("SQLCipher Version ") + sqlcipher_version + QString(" (based on SQLite %1)").arg(sqlite_version);

    const QString body =
      QString("Details for the issue\n"
              "--------------------\n\n"
              "#### What did you do?\n\n\n"
              "#### What did you expect to see?\n\n\n"
              "#### What did you see instead?\n\n\n"
              "Useful extra information\n"
              "-------------------------\n"
              "> DB4S v%1 on %2 (%3/%4) [%5]\n"
              "> using %6\n"
              "> and Qt %7")
            .arg(version, os, kernelType, kernelVersion, arch, sqlite_version, QT_VERSION_STR);

    QUrlQuery query;
    query.addQueryItem("labels", "bug");
    query.addQueryItem("body", body);

    QUrl url("https://github.com/sqlitebrowser/sqlitebrowser/issues/new");
    url.setQuery(query);
    QDesktopServices::openUrl(url);
}

// 'Help | Feature Request...' link will set an appropiate body and add the label 'enhancement' automatically to the issue
void MainWindow::on_actionFeature_Request_triggered()
{
    QUrlQuery query;

    // Add the label enhancement and use the Feature request template that
    // we have in GitHub.
    query.addQueryItem("labels", "enhancement");
    query.addQueryItem("template", "Feature_request.md");

    QUrl url("https://github.com/sqlitebrowser/sqlitebrowser/issues/new");
    url.setQuery(query);
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionSqlCipherFaq_triggered()
{
    QDesktopServices::openUrl(QUrl("https://discuss.zetetic.net/c/sqlcipher/sqlcipher-faq"));
}

void MainWindow::on_actionWebsite_triggered()
{
    QDesktopServices::openUrl(QUrl("https://sqlitebrowser.org"));
}

void MainWindow::on_actionDonatePatreon_triggered()
{
    QDesktopServices::openUrl(QUrl("https://www.patreon.com/bePatron?u=11578749"));
}

void MainWindow::updateBrowseDataColumnWidth(int section, int /*old_size*/, int new_size)
{
    QSet<int> selectedCols(ui->dataTable->selectedCols());
    sqlb::ObjectIdentifier tableName = currentlyBrowsedTableName();

    if (!selectedCols.contains(section))
    {
        browseTableSettings[tableName].columnWidths[section] = new_size;
    }
    else
    {
        ui->dataTable->blockSignals(true);
        for(int col : selectedCols)
        {
            ui->dataTable->setColumnWidth(col, new_size);
            browseTableSettings[tableName].columnWidths[col] = new_size;
        }
        ui->dataTable->blockSignals(false);
    }
}

static void loadBrowseDataTableSettings(BrowseDataTableSettings& settings, QXmlStreamReader& xml)
{
    // TODO Remove this in the near future. This file format was only created temporarily by the nightlies from the late 3.11 development period.
    if(xml.attributes().hasAttribute("sort_order_index"))
    {
        int sortOrderIndex = xml.attributes().value("sort_order_index").toInt();
        Qt::SortOrder sortOrderMode = static_cast<Qt::SortOrder>(xml.attributes().value("sort_order_mode").toInt());
        settings.sortOrder = toSortOrderVector(sortOrderIndex, sortOrderMode);
    }

    settings.showRowid = xml.attributes().value("show_row_id").toInt();
    settings.encoding = xml.attributes().value("encoding").toString();
    settings.plotXAxis = xml.attributes().value("plot_x_axis").toString();
    settings.unlockViewPk = xml.attributes().value("unlock_view_pk").toString();

    while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "table") {
        if(xml.name() == "sort")
        {
            while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "sort")
            {
                if(xml.name() == "column")
                {
                    int index = xml.attributes().value("index").toInt();
                    int mode = xml.attributes().value("mode").toInt();
                    settings.sortOrder.push_back(BrowseDataTableSettings::SortedColumn(index, mode));
                    xml.skipCurrentElement();
                }
            }
        } else if(xml.name() == "column_widths") {
            while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "column_widths") {
                if (xml.name() == "column") {
                    int index = xml.attributes().value("index").toInt();
                    settings.columnWidths[index] = xml.attributes().value("value").toInt();
                    xml.skipCurrentElement();
                }
            }
        } else if(xml.name() == "filter_values") {
            while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "filter_values") {
                if (xml.name() == "column") {
                    int index = xml.attributes().value("index").toInt();
                    settings.filterValues[index] = xml.attributes().value("value").toString();
                    xml.skipCurrentElement();
                }
            }
        } else if(xml.name() == "display_formats") {
            while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "display_formats") {
                if (xml.name() == "column") {
                    int index = xml.attributes().value("index").toInt();
                    settings.displayFormats[index] = xml.attributes().value("value").toString();
                    xml.skipCurrentElement();
                }
            }
        } else if(xml.name() == "hidden_columns") {
            while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "hidden_columns") {
                if (xml.name() == "column") {
                    int index = xml.attributes().value("index").toInt();
                    settings.hiddenColumns[index] = xml.attributes().value("value").toInt();
                    xml.skipCurrentElement();
                }
            }
        } else if(xml.name() == "plot_y_axes") {
            while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "plot_y_axes") {
                QString yAxisName;
                PlotDock::PlotSettings yAxisSettings;
                if (xml.name() == "y_axis") {
                    yAxisName = xml.attributes().value("name").toString();
                    yAxisSettings.lineStyle = xml.attributes().value("line_style").toInt();
                    yAxisSettings.pointShape = xml.attributes().value("point_shape").toInt();
                    yAxisSettings.colour = QColor (xml.attributes().value("colour").toString());
                    yAxisSettings.active = xml.attributes().value("active").toInt();
                    xml.skipCurrentElement();
                }
                settings.plotYAxes[yAxisName] = yAxisSettings;
            }
        }
    }
}
bool MainWindow::loadProject(QString filename, bool readOnly)
{
    // Show the open file dialog when no filename was passed as parameter
    if(filename.isEmpty())
    {
        filename = FileDialog::getOpenFileName(this,
                                                tr("Choose a project file to open"),
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
                    if(xml.attributes().hasAttribute("case_sensitive_like"))
                        db.setPragma("case_sensitive_like", xml.attributes().value("case_sensitive_like").toString());
                    if(xml.attributes().hasAttribute("temp_store"))
                        db.setPragma("temp_store", xml.attributes().value("temp_store").toString());
                    if(xml.attributes().hasAttribute("wal_autocheckpoint"))
                        db.setPragma("wal_autocheckpoint", xml.attributes().value("wal_autocheckpoint").toString());
                    if(xml.attributes().hasAttribute("synchronous"))
                        db.setPragma("synchronous", xml.attributes().value("synchronous").toString());
                    loadPragmas();
                } else if(xml.name() == "attached") {
                    while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "attached")
                    {
                        if(xml.name() == "db")
                        {
                            db.attach(xml.attributes().value("path").toString(), xml.attributes().value("schema").toString());
                            xml.skipCurrentElement();
                        }
                    }
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
                            // This tag is only found in old project files. In newer versions (>= 3.11) it is replaced by a new implementation.
                            // We still support loading it though we might decide to drop that support later. But for now we show a warning to the
                            // user when loading an old file.
                            if(!Settings::getValue("idontcare", "projectBrowseTable").toBool())
                            {
                                QMessageBox msgBox;
                                QPushButton* idontcarebutton = msgBox.addButton(tr("Don't show again"), QMessageBox::ActionRole);
                                msgBox.addButton(QMessageBox::Ok);
                                msgBox.setTextFormat(Qt::RichText);
                                msgBox.setWindowTitle(qApp->applicationName());
                                msgBox.setText(tr("This project file is using an old file format because it was created using DB Browser for SQLite "
                                                  "version 3.10 or lower. Loading this file format is still fully supported but we advice you to convert "
                                                  "all your project files to the new file format because support for older formats might be dropped "
                                                  "at some point in the future. You can convert your files by simply opening and re-saving them."));
                                msgBox.exec();
                                if(msgBox.clickedButton() == idontcarebutton)
                                    Settings::setValue("idontcare", "projectBrowseTable", true);
                            }

                            QString attrData = xml.attributes().value("data").toString();
                            QByteArray temp = QByteArray::fromBase64(attrData.toUtf8());
                            QDataStream stream(temp);
                            stream >> browseTableSettings;
                            xml.skipCurrentElement();
                        } else if(xml.name() == "browse_table_settings") {

                            while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "browse_table_settings") {
                                if (xml.name() == "table") {

                                    sqlb::ObjectIdentifier tableIdentifier =
                                        sqlb::ObjectIdentifier (xml.attributes().value("schema").toString(),
                                                                xml.attributes().value("name").toString());
                                    BrowseDataTableSettings settings;
                                    loadBrowseDataTableSettings(settings, xml);
                                    browseTableSettings[tableIdentifier] = settings;
                                }
                            }
                        }

                        if(ui->mainTab->currentIndex() == BrowseTab)
                        {
                            populateTable();     // Refresh view
                            sqlb::ObjectIdentifier current_table = currentlyBrowsedTableName();

                            int sortIndex;
                            Qt::SortOrder sortMode;
                            fromSortOrderVector(browseTableSettings[current_table].sortOrder, sortIndex, sortMode);
                            ui->dataTable->sortByColumn(sortIndex, sortMode);
                            showRowidColumn(browseTableSettings[current_table].showRowid);
                            unlockViewEditing(!browseTableSettings[current_table].unlockViewPk.isEmpty(), browseTableSettings[current_table].unlockViewPk);
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

static void saveBrowseDataTableSettings(const BrowseDataTableSettings& object, QXmlStreamWriter& xml)
{
    xml.writeAttribute("show_row_id", QString::number(object.showRowid));
    xml.writeAttribute("encoding", object.encoding);
    xml.writeAttribute("plot_x_axis", object.plotXAxis);
    xml.writeAttribute("unlock_view_pk", object.unlockViewPk);

    xml.writeStartElement("sort");
    for(const auto& column : object.sortOrder)
    {
        xml.writeStartElement("column");
        xml.writeAttribute("index", QString::number(column.index));
        xml.writeAttribute("mode", QString::number(column.mode));
        xml.writeEndElement();
    }
    xml.writeEndElement();

    xml.writeStartElement("column_widths");
    for(auto iter=object.columnWidths.constBegin(); iter!=object.columnWidths.constEnd(); ++iter) {
        xml.writeStartElement("column");
        xml.writeAttribute("index", QString::number(iter.key()));
        xml.writeAttribute("value", QString::number(iter.value()));
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeStartElement("filter_values");
    for(auto iter=object.filterValues.constBegin(); iter!=object.filterValues.constEnd(); ++iter) {
        xml.writeStartElement("column");
        xml.writeAttribute("index", QString::number(iter.key()));
        xml.writeAttribute("value", iter.value());
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeStartElement("display_formats");
    for(auto iter=object.displayFormats.constBegin(); iter!=object.displayFormats.constEnd(); ++iter) {
        xml.writeStartElement("column");
        xml.writeAttribute("index", QString::number(iter.key()));
        xml.writeAttribute("value", iter.value());
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeStartElement("hidden_columns");
    for(auto iter=object.hiddenColumns.constBegin(); iter!=object.hiddenColumns.constEnd(); ++iter) {
        xml.writeStartElement("column");
        xml.writeAttribute("index", QString::number(iter.key()));
        xml.writeAttribute("value", QString::number(iter.value()));
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeStartElement("plot_y_axes");
    for(auto iter=object.plotYAxes.constBegin(); iter!=object.plotYAxes.constEnd(); ++iter) {
        PlotDock::PlotSettings plotSettings = iter.value();
        xml.writeStartElement("y_axis");
        xml.writeAttribute("name", iter.key());
        xml.writeAttribute("line_style", QString::number(plotSettings.lineStyle));
        xml.writeAttribute("point_shape", QString::number(plotSettings.pointShape));
        xml.writeAttribute("colour", plotSettings.colour.name());
        xml.writeAttribute("active", QString::number(plotSettings.active));
        xml.writeEndElement();
    }
    xml.writeEndElement();
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
        xml.writeAttribute("case_sensitive_like", db.getPragma("case_sensitive_like"));
        xml.writeAttribute("temp_store", db.getPragma("temp_store"));
        xml.writeAttribute("wal_autocheckpoint", db.getPragma("wal_autocheckpoint"));
        xml.writeAttribute("synchronous", db.getPragma("synchronous"));
        xml.writeEndElement();

        // Attached databases
        xml.writeStartElement("attached");
        QString sql("PRAGMA database_list;");
        db.logSQL(sql, kLogMsg_App);
        sqlite3_stmt* db_vm;
        if(sqlite3_prepare_v2(db.get("project").get(), sql.toUtf8(), sql.toUtf8().length(), &db_vm, nullptr) == SQLITE_OK)
        {
            while(sqlite3_step(db_vm) == SQLITE_ROW)
            {
                QString schema(QString::fromUtf8((const char*)sqlite3_column_text(db_vm, 1)));
                if(schema != "main" && schema != "temp")
                {
                    QString path(QString::fromUtf8((const char*)sqlite3_column_text(db_vm, 2)));
                    xml.writeStartElement("db");
                    xml.writeAttribute("schema", schema);
                    xml.writeAttribute("path", path);
                    xml.writeEndElement();
                }
            }
            sqlite3_finalize(db_vm);
        }
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

        xml.writeStartElement("browse_table_settings");
        for(auto tableIt=browseTableSettings.constBegin(); tableIt!=browseTableSettings.constEnd(); ++tableIt) {

            xml.writeStartElement("table");
            xml.writeAttribute("schema", tableIt.key().schema());
            xml.writeAttribute("name", tableIt.key().name());
            saveBrowseDataTableSettings(tableIt.value(), xml);
            xml.writeEndElement();
        }
        // </browse_table_settings>
        xml.writeEndElement();
        // </tab_browse>
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
    BrowseDataTableSettings& settings = browseTableSettings[currentlyBrowsedTableName()];
    settings.filterValues[column] = value;
    setRecordsetLabel();

    // Reapply the view settings. This seems to be necessary as a workaround for newer Qt versions.
    applyBrowseTableSettings(settings, true);
}

void MainWindow::editEncryption()
{
#ifdef ENABLE_SQLCIPHER
    CipherDialog cipherDialog(this, true);
    if(cipherDialog.exec())
    {
        // Show progress dialog even though we can't provide any detailed progress information but this
        // process might take some time.
        QProgressDialog progress(this);
        progress.setCancelButton(nullptr);
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

        CipherSettings cipherSettings = cipherDialog.getCipherSettings();

        // Attach a new database using the new settings
        qApp->processEvents();
        if(ok)
            ok = db.executeSQL(QString("ATTACH DATABASE '%1' AS sqlitebrowser_edit_encryption KEY %2;").arg(db.currentFile() + ".enctemp").arg(cipherSettings.getPassword()),
                               false, false);
        qApp->processEvents();
        if(ok)
            ok = db.executeSQL(QString("PRAGMA sqlitebrowser_edit_encryption.cipher_page_size = %1").arg(cipherSettings.getPageSize()), false, false);

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

        sqlb::ObjectIdentifier obj(ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnSchema)).toString(),
                                   ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), DbStructureModel::ColumnName)).toString());
        tableToBrowse = obj.toDisplayString();
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
    QString stmt = ui->dbTreeWidget->model()->data(ui->dbTreeWidget->currentIndex().sibling(ui->dbTreeWidget->currentIndex().row(), 3), Qt::EditRole).toString();

    // Copy the statement to the global application clipboard
    QApplication::clipboard()->setText(stmt);
}

void MainWindow::jumpToRow(const sqlb::ObjectIdentifier& table, QString column, const QByteArray& value)
{
    // First check if table exists
    sqlb::TablePtr obj = db.getObjectByName<sqlb::Table>(table);
    if(!obj)
        return;

    // If no column name is set, assume the primary key is meant
    if(!column.size())
        column = obj->findPk()->name();

    // If column doesn't exist don't do anything
    auto column_index = sqlb::findField(obj, column);
    if(column_index == obj->fields.end())
        return;

    // Jump to table
    ui->comboBrowseTable->setCurrentIndex(ui->comboBrowseTable->findText(table.toDisplayString()));
    populateTable();

    // Set filter
    ui->dataTable->filterHeader()->setFilter(column_index-obj->fields.begin()+1, "=" + value);
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
    if(!(db.getObjectByName(currentlyBrowsedTableName())->type() == sqlb::Object::Types::Table && !db.readOnly()))
        return;

    int row = ui->dataTable->verticalHeader()->logicalIndexAt(pos);
    if (row == -1)
        return;

    // Select the row if it is not already in the selection.
    QModelIndexList rowList = ui->dataTable->selectionModel()->selectedRows();
    bool found = false;
    for (QModelIndex index : rowList) {
        if (row == index.row()) {
            found = true;
            break;
        }
    }
    if (!found)
        ui->dataTable->selectRow(row);

    rowList = ui->dataTable->selectionModel()->selectedRows();

    QString duplicateText = rowList.count() > 1 ? tr("Duplicate records") : tr("Duplicate record");

    QMenu popupRecordMenu(this);
    QAction* action = new QAction(duplicateText, &popupRecordMenu);
    // Set shortcut for documentation purposes (the actual functional shortcut is not set here)
    action->setShortcut(QKeySequence(tr("Ctrl+\"")));
    popupRecordMenu.addAction(action);

    connect(action, &QAction::triggered, [&]() {
            for (QModelIndex index : rowList) {
                duplicateRecord(index.row());
            }
    });

    QAction* deleteRecordAction = new QAction(ui->buttonDeleteRecord->text(), &popupRecordMenu);
    popupRecordMenu.addAction(deleteRecordAction);

    connect(deleteRecordAction, &QAction::triggered, [&]() {
            deleteRecord();
    });

    popupRecordMenu.exec(ui->dataTable->verticalHeader()->mapToGlobal(pos));
}

void MainWindow::editDataColumnDisplayFormat()
{
    // Get the current table name and fetch its table object, then retrieve the fields of that table and look up the index of the clicked table header
    // section using it as the table field array index. Subtract one from the header index to get the column index because in the the first (though hidden)
    // column is always the rowid column. Ultimately, get the column name from the column object
    sqlb::ObjectIdentifier current_table = currentlyBrowsedTableName();
    int field_number = sender()->property("clicked_column").toInt();
    QString field_name;
    if (db.getObjectByName(current_table)->type() == sqlb::Object::Table)
      field_name = db.getObjectByName<sqlb::Table>(current_table)->fields.at(field_number-1).name();
    else
      field_name = db.getObjectByName<sqlb::View>(current_table)->fieldNames().at(field_number-1);
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

void MainWindow::showRowidColumn(bool show, bool skipFilters)
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
    sqlb::ObjectIdentifier current_table = currentlyBrowsedTableName();
    browseTableSettings[current_table].showRowid = show;

    // Update the filter row
    if(!skipFilters)
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
    QStringList availableCodecs = toStringList(QTextCodec::availableCodecs());
    availableCodecs.removeDuplicates();
    int currentItem = availableCodecs.indexOf(encoding);

    if(forAllTables)
        question = tr("Please choose a new encoding for all tables.");
    else
        question = tr("Please choose a new encoding for this table.");
    encoding = QInputDialog::getItem(this,
                                     tr("Set encoding"),
                                     tr("%1\nLeave the field empty for using the database encoding.").arg(question),
                                     availableCodecs,
                                     currentItem,
                                     true, // editable
                                     &ok);

    // Only set the new encoding if the user clicked the OK button
    if(ok)
    {
        // Check if encoding is valid
        if(!encoding.isEmpty() && !QTextCodec::codecForName(encoding.toUtf8()))
        {
            QMessageBox::warning(this, qApp->applicationName(), tr("This encoding is either not valid or not supported."));
            return;
        }

        // Set encoding for current table
        m_browseTableModel->setEncoding(encoding);

        // Save encoding for this table
        browseTableSettings[currentlyBrowsedTableName()].encoding = encoding;

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
    sqlb::ObjectIdentifier currentTable = currentlyBrowsedTableName();

    // If this isn't a view just unlock editing and return
    if(db.getObjectByName(currentTable) && db.getObjectByName(currentTable)->type() != sqlb::Object::View)
    {
        m_browseTableModel->setPseudoPk(QString());
        enableEditing(true);
        return;
    }

    sqlb::ViewPtr obj = db.getObjectByName<sqlb::View>(currentTable);

    // If the view gets unlocked for editing and we don't have a 'primary key' for this view yet, then ask for one
    if(unlock && pk.isEmpty())
    {
        while(true)
        {
            bool ok;

            // Ask for a PK
            pk = QInputDialog::getItem(this,
                                       qApp->applicationName(),
                                       tr("Please enter a pseudo-primary key in order to enable editing on this view. "
                                          "This should be the name of a unique column in the view."),
                                       obj->fieldNames(),
                                       0,
                                       false,
                                       &ok);

            // Cancelled?
            if(!ok || pk.isEmpty()) {
                ui->actionUnlockViewEditing->setChecked(false);
                return;
            }

            // Do some basic testing of the input and if the input appears to be good, go on
            if(db.executeSQL(QString("SELECT %1 FROM %2 LIMIT 1;").arg(sqlb::escapeIdentifier(pk)).arg(currentTable.toString()), false, true))
                break;
        }
    } else if(!unlock) {
        // Locking the view is done by unsetting the pseudo-primary key
        pk.clear();
    }

    // (De)activate editing
    enableEditing(unlock);
    m_browseTableModel->setPseudoPk(pk);

    // Update checked status of the popup menu action
    ui->actionUnlockViewEditing->blockSignals(true);
    ui->actionUnlockViewEditing->setChecked(unlock);
    ui->actionUnlockViewEditing->blockSignals(false);

    // If the settings didn't change, do not try to reapply them.
    // This avoids an infinite mutual recursion.
    BrowseDataTableSettings& settings = browseTableSettings[currentTable];

    if(settings.unlockViewPk != pk) {
        // Save settings for this table
        settings.unlockViewPk = pk;
        // Reapply the view settings. This seems to be necessary as a workaround for newer Qt versions.
        applyBrowseTableSettings(settings);
    }
}

sqlb::ObjectIdentifier MainWindow::currentlyBrowsedTableName() const
{
    return sqlb::ObjectIdentifier(ui->comboBrowseTable->model()->data(dbStructureModel->index(ui->comboBrowseTable->currentIndex(),
                                                                                              DbStructureModel::ColumnSchema,
                                                                                              ui->comboBrowseTable->rootModelIndex())).toString(),
                                  ui->comboBrowseTable->currentData(Qt::EditRole).toString());  // Use the edit role here to make sure we actually get the
                                                                                                // table name without the schema bit in front of it.
}

void MainWindow::hideColumns(int column, bool hide)
{
    sqlb::ObjectIdentifier tableName = currentlyBrowsedTableName();

    // Select columns to (un)hide
    QSet<int> columns;
    if(column == -1)
    {
         if(ui->dataTable->selectedCols().size() == 0)
             columns.insert(ui->actionBrowseTableEditDisplayFormat->property("clicked_column").toInt());
         else
             columns += ui->dataTable->selectedCols();
    } else {
        columns.insert(column);
    }

    // (Un)hide requested column(s)
    for(int col : columns)
    {
        ui->dataTable->setColumnHidden(col, hide);
        if(!hide)
            ui->dataTable->setColumnWidth(col, ui->dataTable->horizontalHeader()->defaultSectionSize());
        browseTableSettings[tableName].hiddenColumns[col] = hide;
    }

    // check to see if all the columns are hidden
    bool allHidden = true;
    for(int col = 1; col < ui->dataTable->model()->columnCount(); col++)
    {
        if(!ui->dataTable->isColumnHidden(col))
        {
            allHidden = false;
            break;
        }
    }

    if(allHidden  && ui->dataTable->model()->columnCount() > 1)
        hideColumns(1, false);
}

void MainWindow::on_actionShowAllColumns_triggered()
{
    for(int col = 1; col < ui->dataTable->model()->columnCount(); col++)
    {
        if(ui->dataTable->isColumnHidden(col))
            hideColumns(col, false);
    }
}

void MainWindow::requestCollation(const QString& name, int eTextRep)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
                this,
                tr("Collation needed! Proceed?"),
                tr("A table in this database requires a special collation function '%1' "
                   "that this application can't provide without further knowledge.\n"
                   "If you choose to proceed, be aware bad things can happen to your database.\n"
                   "Create a backup!").arg(name), QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        auto pDb = db.get(tr("creating collation"));
        sqlite3_create_collation(pDb.get(), name.toUtf8(), eTextRep, nullptr, collCompare);
    }
}

void MainWindow::renameSqlTab(int index)
{
    QString new_name = QInputDialog::getText(this,
                                             qApp->applicationName(),
                                             tr("Set a new name for the SQL tab. Use the '&&' character to allow using the following character as a keyboard shortcut."),
                                             QLineEdit::EchoMode::Normal,
                                             ui->tabSqlAreas->tabText(index));

    if(!new_name.isNull())      // Don't do anything if the Cancel button was clicked
        ui->tabSqlAreas->setTabText(index, new_name);
}

void MainWindow::setFindFrameVisibility(bool show)
{
    // Set the find frame visibility for all tabs, but leave the
    // current as the last, to retain there the focus.
    for(int i=0;i<ui->tabSqlAreas->count();i++)
        if (i != ui->tabSqlAreas->currentIndex())
            qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i))->setFindFrameVisibility(show);
    if (ui->tabSqlAreas->count()>0)
        qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget())->setFindFrameVisibility(show);
}

void MainWindow::openFindReplaceDialog()
{
    // The slot for the shortcut must discover which sqltexedit widget has the focus and then open its dialog.
    SqlExecutionArea* sqlWidget = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());

    if (sqlWidget)
        sqlWidget->getEditor()->openFindReplaceDialog();
}

void MainWindow::openSqlPrintDialog()
{
    // The slot for the shortcut must discover which sqltexedit widget has the focus and then open its dialog.
    SqlExecutionArea* sqlWidget = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());

    if (sqlWidget)
        sqlWidget->getEditor()->openPrintDialog();
}

void MainWindow::saveAsView(QString query)
{
    // Let the user select a name for the new view and make sure it doesn't already exist
    QString name;
    while(true)
    {
        name = QInputDialog::getText(this, qApp->applicationName(), tr("Please specify the view name")).trimmed();
        if(name.isNull())
            return;
        if(db.getObjectByName(sqlb::ObjectIdentifier("main", name)) != nullptr)
            QMessageBox::warning(this, qApp->applicationName(), tr("There is already an object with that name. Please choose a different name."));
        else
            break;
    }

    // Create the view
    if(db.executeSQL(QString("CREATE VIEW %1 AS %2;").arg(sqlb::escapeIdentifier(name)).arg(query)))
        QMessageBox::information(this, qApp->applicationName(), tr("View successfully created."));
    else
        QMessageBox::warning(this, qApp->applicationName(), tr("Error creating view: %1").arg(db.lastError()));
}

void MainWindow::exportFilteredTable()
{
    ExportDataDialog dialog(db, ExportDataDialog::ExportFormatCsv, this, m_browseTableModel->customQuery(false));
    dialog.exec();
}

void MainWindow::saveFilterAsView()
{
    if (m_browseTableModel->filterCount() > 0)
        // Save as view a custom query without rowid
        saveAsView(m_browseTableModel->customQuery(false));
    else
        QMessageBox::information(this, qApp->applicationName(), tr("There is no filter set for this table. View will not be created."));
}

void MainWindow::duplicateRecord(int currentRow)
{
    auto row = m_browseTableModel->dittoRecord(currentRow);
    if (row.isValid())
        ui->dataTable->setCurrentIndex(row);
    else
        QMessageBox::warning(this, qApp->applicationName(), db.lastError());
}

void MainWindow::updateInsertDeleteRecordButton()
{
    // Update the delete record button to reflect number of selected records

    // NOTE: We're assuming here that the selection is always contiguous, i.e. that there are never two selected
    // rows with a non-selected row in between.
    int rows = 0;

    // If there is no model yet (because e.g. no database file is opened) there is no selection model either. So we need to check for that here
    // in order to avoid null pointer dereferences. If no selection model exists we will just continue as if no row is selected because without a
    // model you could argue there actually is no row to be selected.
    if(ui->dataTable->selectionModel())
    {
        const auto & sel = ui->dataTable->selectionModel()->selectedIndexes();
        if(sel.count())
            rows = sel.last().row() - sel.first().row() + 1;
    }

    // Enable the insert and delete buttons only if the currently browsed table or view is editable. For the delete button we additionally require
    // at least one row to be selected. For the insert button there is an extra rule to disable it when we are browsing a view because inserting
    // into a view isn't supported yet.
    bool isEditable = m_browseTableModel->isEditable() && !db.readOnly();
    ui->buttonNewRecord->setEnabled(isEditable && m_browseTableModel->pseudoPk().isEmpty());
    ui->buttonDeleteRecord->setEnabled(isEditable && rows != 0);

    if(rows > 1)
        ui->buttonDeleteRecord->setText(tr("Delete Records"));
    else
        ui->buttonDeleteRecord->setText(tr("Delete Record"));
}

void MainWindow::runSqlNewTab(const QString& query, const QString& title)
{
    QString message = tr("This action will open a new SQL tab for running:") +
                         QString("<br/><tt>%1</tt><p/>").arg(query) +
                         tr("Press Help for opening the corresponding SQLite reference page.");
    QString windowTitle = title;
    windowTitle.remove('&');

    switch (QMessageBox::information(this, windowTitle, message, QMessageBox::Ok | QMessageBox::Default, QMessageBox::Cancel | QMessageBox::Escape, QMessageBox::Help))
    {
    case QMessageBox::Ok: {
        ui->mainTab->setCurrentIndex(ExecuteTab);
        unsigned int index = openSqlTab();
        ui->tabSqlAreas->setTabText(index, title);
        qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(index))->getEditor()->setText(query);
        executeQuery();
        break;
    }
    case QMessageBox::Help: {
        QString anchor = query.toLower();
        anchor.replace(" ", "_").chop(1);
        QDesktopServices::openUrl(QUrl(QString("https://www.sqlite.org/pragma.html#") + anchor));
        break;
    }
    default:
        return;
    }
}

void MainWindow::printDbStructure ()
{
    const QTreeView* treeView = ui->dbTreeWidget;
    const QAbstractItemModel* model = treeView->model();

    const int rowCount = model->rowCount(treeView->rootIndex());
    const int columnCount = model->columnCount(treeView->rootIndex());

    QString strStream;
    QTextStream out(&strStream);

    out << "<html><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
        << QString("<title>%1</title>").arg(treeView->windowTitle())
        << "<style type=\"text/css\">pre {white-space: pre-wrap;}</style></head><body bgcolor=\"#FFFFFF\">";

    for (int row = 0; row < rowCount; row++) {

        QModelIndex headerIndex = model->index(row, 0, treeView->rootIndex());
        QString data = model->data(headerIndex).toString().toHtmlEscaped();
        out << QString("<h1>%1</h1>").arg(data);

        // Open a new table for each group of objects
        out << "<table border=1 cellspacing=0 cellpadding=2><thead><tr bgcolor=\"#F0F0F0\">";

        for (int column = 0; column < columnCount; column++) {
            // Headers
            if (!treeView->isColumnHidden(column))
                out << QString("<th>%1</th>").arg(model->headerData(column, Qt::Horizontal).toString().toHtmlEscaped());
        }
        out << "</tr></thead>";

        for (int column = 0; column < columnCount; column++) {
            QModelIndex groupIndex = model->index(row, column, treeView->rootIndex());

            // A row for the object name
            for (int rowChild = 0; rowChild < model->rowCount(groupIndex); rowChild++) {
                QModelIndex objectIndex = model->index(rowChild, column, groupIndex);
                out << "<tr>";
                for (int column2 = 0; column2 < columnCount; column2++) {
                    if (!treeView->isColumnHidden(column2)) {
                        QModelIndex cellIndex = model->index(rowChild, column2, groupIndex);
                        QString data = model->data(cellIndex).toString().toHtmlEscaped();
                        if (column2 != DbStructureModel::ColumnSQL)
                            out << QString("<td><h2>%1</h2></td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                        else
                            out << QString("<td><pre>%1</pre></td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                    }
                }
                out << "</tr>";

                // One row for each object's fields
                for (int rowChild2 = 0; rowChild2 < model->rowCount(objectIndex); rowChild2++) {
                    out << "<tr>";
                    for (int column2 = 0; column2 < columnCount; column2++) {
                        if (!treeView->isColumnHidden(column2)) {
                            QModelIndex fieldIndex = model->index(rowChild2, column2, objectIndex);
                            QString data = model->data(fieldIndex).toString().toHtmlEscaped();
                            out << QString("<td>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                        }
                    }
                    out << "</tr>";
                }
            }
        }
        out << "</table>";
    }
    out << "</body></html>";

    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);

    QPrinter printer;
    printer.setDocName(treeView->windowTitle());

    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer);
    connect(dialog, &QPrintPreviewDialog::paintRequested, [&](QPrinter *previewPrinter) {
        document->print(previewPrinter);
    });

    dialog->exec();

    delete dialog;
    delete document;
}
