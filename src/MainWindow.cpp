#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Application.h"
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
#include "RemoteDock.h"
#include "FindReplaceDialog.h"
#include "RunSql.h"
#include "ExtendedTableWidget.h"
#include "Data.h"
#include "TableBrowser.h"
#include "TableBrowserDock.h"

#include <chrono>
#include <QFile>
#include <QTextStream>
#include <QWhatsThis>
#include <QMessageBox>
#include <QPersistentModelIndex>
#include <QDragEnterEvent>
#include <QScrollBar>
#include <QMimeData>
#include <QDesktopServices>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QInputDialog>
#include <QProgressDialog>
#include <QTextEdit>
#include <QClipboard>
#include <QShortcut>
#include <QUrlQuery>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPushButton>
#include <QTemporaryFile>
#include <QToolButton>

#ifdef Q_OS_MACX //Needed only on macOS
    #include <QOpenGLWidget>
#endif

#include <limits>

int MainWindow::MaxRecentFiles;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      db(),
      editDock(new EditDialog(this)),
      plotDock(new PlotDock(this)),
      remoteDock(new RemoteDock(this)),
      currentTableBrowser(nullptr),
      findReplaceDialog(new FindReplaceDialog(this)),
      execute_sql_worker(nullptr),
      isProjectModified(false)
{
    ui->setupUi(this);
    init();

    activateFields(false);
    updateRecentFileActions();
}

MainWindow::~MainWindow()
{
    delete dbSelected;
    delete dockDbSelected;
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
    ui->verticalLayout->addWidget(ogl);
    ogl->setHidden(true);
#endif

    // Automatic update check
#ifdef CHECKNEWVERSION
    // Check for a new version if automatic update check aren't disabled in the settings dialog
    if(Settings::getValue("checkversion", "enabled").toBool())
    {
        RemoteNetwork::get().fetch(QUrl("https://download.sqlitebrowser.org/currentrelease"), RemoteNetwork::RequestTypeCustom,
                                   QString(), [this](const QByteArray& reply) {
            QList<QByteArray> info = reply.split('\n');
            if(info.size() >= 2)
            {
                QString version = info.at(0).trimmed();
                QString url = info.at(1).trimmed();
                checkNewVersion(version, url);
            }
        }, false, true);
    }
#endif

    // create facade objects to dbTreeWidgets
    dbSelected = new DbStructureQItemViewFacade(*ui->dbTreeWidget);
    dockDbSelected = new DbStructureQItemViewFacade(*ui->treeSchemaDock);

    // Connect SQL logging and database state setting to main window
    connect(&db, &DBBrowserDB::dbChanged, this, &MainWindow::dbState, Qt::QueuedConnection);
    connect(&db, &DBBrowserDB::sqlExecuted, this, &MainWindow::logSql, Qt::QueuedConnection);
    connect(&db, &DBBrowserDB::requestCollation, this, &MainWindow::requestCollation);

    // Set up DB structure tab
    dbStructureModel = new DbStructureModel(db, this);
    connect(&db, &DBBrowserDB::structureUpdated, this, [this]() {
        std::vector<sqlb::ObjectIdentifier> old_tables;
        for(const auto& d : allTableBrowserDocks())
            old_tables.push_back(d->tableBrowser()->currentlyBrowsedTableName());

        dbStructureModel->reloadData();

        populateStructure(old_tables);
    });
    ui->dbTreeWidget->setModel(dbStructureModel);
    ui->dbTreeWidget->setColumnWidth(DbStructureModel::ColumnName, 300);
    ui->dbTreeWidget->setColumnHidden(DbStructureModel::ColumnObjectType, true);
    ui->dbTreeWidget->setColumnHidden(DbStructureModel::ColumnSchema, true);

    // Set up DB schema dock
    ui->treeSchemaDock->setModel(dbStructureModel);
    ui->treeSchemaDock->setColumnHidden(DbStructureModel::ColumnObjectType, true);
    ui->treeSchemaDock->setColumnHidden(DbStructureModel::ColumnSchema, true);

    // Create initial table browser tab
    ui->tabBrowsers->setWindowFlags(Qt::Widget);
    ui->tabBrowsers->setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);
    newTableBrowserTab();

    // Create docks
    ui->dockEdit->setWidget(editDock);
    ui->dockPlot->setWidget(plotDock);
    ui->dockRemote->setWidget(remoteDock);

    // Set up edit dock
    editDock->setReadOnly(true);

    // Restore window geometry
    restoreGeometry(Settings::getValue("MainWindow", "geometry").toByteArray());

    // Save default and restore window state
    defaultWindowState = saveState();
    restoreState(Settings::getValue("MainWindow", "windowState").toByteArray());

    // Save default and restore open tab order if the openTabs setting is saved.
    defaultOpenTabs = saveOpenTabs();
    restoreOpenTabs(Settings::getValue("MainWindow", "openTabs").toString());

    // Restore dock state settings
    ui->comboLogSubmittedBy->setCurrentIndex(ui->comboLogSubmittedBy->findText(Settings::getValue("SQLLogDock", "Log").toString()));

    // Add keyboard shortcuts
    QShortcut* shortcutBrowseRefreshF5 = new QShortcut(QKeySequence("F5"), this);
    connect(shortcutBrowseRefreshF5, &QShortcut::activated, this, &MainWindow::refresh);
    QShortcut* shortcutBrowseRefreshCtrlR = new QShortcut(QKeySequence("Ctrl+R"), this);
    connect(shortcutBrowseRefreshCtrlR, &QShortcut::activated, this, &MainWindow::refresh);
    QShortcut* shortcutBrowseRefreshCtrlEnter = new QShortcut(QKeySequence("Ctrl+Enter"), this);
    connect(shortcutBrowseRefreshCtrlEnter, &QShortcut::activated, this, &MainWindow::refresh);

    // Add print shortcut for the DB Structure tab (dbTreeWidget) with context to the widget, so other print shortcuts aren't eclipsed.
    QShortcut* shortcutPrint = new QShortcut(QKeySequence(QKeySequence::Print), ui->dbTreeWidget, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutPrint, &QShortcut::activated, this, &MainWindow::printDbStructure);

    QShortcut* closeTabShortcut = new QShortcut(tr("Ctrl+W"), ui->tabSqlAreas, nullptr, nullptr, Qt::WidgetWithChildrenShortcut);
    connect(closeTabShortcut, &QShortcut::activated, this, [this]() {
        if(ui->tabSqlAreas->currentIndex() >= 0)
          closeSqlTab(ui->tabSqlAreas->currentIndex());
    });

    // Shortcuts for advancing and going back in the SQL Execution area tabs, independently of the widget which has focus.
    // This emulates the shortcuts provided by QTabWidget.
    QShortcut* shortcutNextTab = new QShortcut(QKeySequence(tr("Ctrl+Tab")), ui->tabSqlAreas, nullptr, nullptr, Qt::WidgetWithChildrenShortcut);
    connect(shortcutNextTab, &QShortcut::activated, this, [this]() {
        if(ui->tabSqlAreas->currentIndex() == ui->tabSqlAreas->count() - 1)
            ui->tabSqlAreas->setCurrentIndex(0);
        else
            ui->tabSqlAreas->setCurrentIndex(ui->tabSqlAreas->currentIndex() + 1);
        focusSqlEditor();
    });

    QShortcut* shortcutPreviousTab = new QShortcut(QKeySequence(tr("Ctrl+Shift+Tab")), ui->tabSqlAreas, nullptr, nullptr, Qt::WidgetWithChildrenShortcut);
    connect(shortcutPreviousTab, &QShortcut::activated, this, [this]() {
        if(ui->tabSqlAreas->currentIndex() == 0)
            ui->tabSqlAreas->setCurrentIndex(ui->tabSqlAreas->count() - 1);
        else
            ui->tabSqlAreas->setCurrentIndex(ui->tabSqlAreas->currentIndex() - 1);
        focusSqlEditor();
    });

    // This is the counterpart of Ctrl+PgDown in SQL editor, which focus out of the editor.
    // This one sets the focus again in the editor.
    QShortcut* shortcutFocusEditor = new QShortcut(QKeySequence(tr("Ctrl+PgUp")), ui->tabSqlAreas, nullptr, nullptr, Qt::WidgetWithChildrenShortcut);
    connect(shortcutFocusEditor, &QShortcut::activated, this, &MainWindow::focusSqlEditor);

    // Get MaxRecentFiles value from QSettings.
    MaxRecentFiles = Settings::getValue("General", "maxRecentFiles").toInt();
    recentFileActs.resize(MaxRecentFiles);

    // Create the actions for the recently opened dbs list
    for(int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], &QAction::triggered, this, &MainWindow::openRecentFile);
    }
    for(int i = 0; i < MaxRecentFiles; ++i)
        ui->fileRecentFiles->insertAction(ui->fileExitAction, recentFileActs[i]);

    recentSeparatorAct = ui->fileRecentFiles->insertSeparator(ui->fileExitAction);
    clearRecentFilesAction = ui->fileRecentFiles->addAction(tr("Clear List"));
    ui->fileRecentFiles->insertAction(ui->fileExitAction, clearRecentFilesAction);
    connect(clearRecentFilesAction, &QAction::triggered, this, &MainWindow::clearRecentFiles);

    // Create popup menus
    popupTableMenu = new QMenu(this);
    popupTableMenu->addAction(ui->actionEditBrowseTable);
    popupTableMenu->addAction(ui->editModifyObjectAction);
    popupTableMenu->addAction(ui->editDeleteObjectAction);
    popupTableMenu->addAction(ui->fileDetachAction);
    popupTableMenu->addSeparator();
    popupTableMenu->addAction(ui->actionEditCopyCreateStatement);
    popupTableMenu->addAction(ui->actionExportCsvPopup);

    popupSchemaDockMenu = new QMenu(this);
    popupSchemaDockMenu->addAction(ui->actionPopupSchemaDockBrowseTable);
    popupSchemaDockMenu->addAction(ui->actionPopupSchemaDockDetachDatabase);
    popupSchemaDockMenu->addSeparator();
    popupSchemaDockMenu->addAction(ui->actionDropSelectQueryCheck);
    popupSchemaDockMenu->addAction(ui->actionDropQualifiedCheck);
    popupSchemaDockMenu->addAction(ui->actionEnquoteNamesCheck);

    popupOpenDbMenu = new QMenu(this);
    popupOpenDbMenu->addAction(ui->fileOpenAction);
    popupOpenDbMenu->addAction(ui->fileOpenReadOnlyAction);
    ui->fileOpenActionPopup->setMenu(popupOpenDbMenu);

    popupSaveSqlFileMenu = new QMenu(this);
    popupSaveSqlFileMenu->addAction(ui->actionSqlSaveFile);
    popupSaveSqlFileMenu->addAction(ui->actionSqlSaveFileAs);
    ui->actionSqlSaveFilePopup->setMenu(popupSaveSqlFileMenu);

    popupSaveSqlResultsMenu = new QMenu(this);
    popupSaveSqlResultsMenu->addAction(ui->actionSqlResultsExportCsv);
    popupSaveSqlResultsMenu->addAction(ui->actionSqlResultsExportJson);
    popupSaveSqlResultsMenu->addAction(ui->actionSqlResultsSaveAsView);
    ui->actionSqlResultsSave->setMenu(popupSaveSqlResultsMenu);
    qobject_cast<QToolButton*>(ui->toolbarSql->widgetForAction(ui->actionSqlResultsSave))->setPopupMode(QToolButton::InstantPopup);

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

    // Connect the tabCloseRequested to the actual closeTab function.
    // This must be done before the connections for checking the actions in the View menu so
    // they are updated accordingly.
    connect(ui->mainTab, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);

    // Add entries for toggling the visibility of main tabs
    for (QWidget* widget : {ui->structure, ui->browser, ui->pragmas, ui->query}) {
        QAction* action = ui->viewMenu->addAction(QIcon(":/icons/open_sql"), widget->accessibleName());
        action->setObjectName(widget->accessibleName());
        action->setCheckable(true);
        action->setChecked(ui->mainTab->indexOf(widget) != -1);
        connect(action, &QAction::toggled, this, [=](bool show) { toggleTabVisible(widget, show); });
        // Connect tabCloseRequested for setting checked the appropriate menu entry.
        // Note these are called after the actual tab is closed only because they are connected
        // after connecting closeTab.
        connect(ui->mainTab, &QTabWidget::tabCloseRequested, this, [=](int /*index*/) {
                action->setChecked(ui->mainTab->indexOf(widget) != -1);
            });
    }

    ui->viewMenu->addSeparator();

    QMenu* layoutMenu = new QMenu(tr("Window Layout"), this);
    ui->viewMenu->addMenu(layoutMenu);

    QAction* resetLayoutAction = layoutMenu->addAction(tr("Reset Window Layout"));
    resetLayoutAction->setShortcut(QKeySequence(tr("Alt+0")));
    connect(resetLayoutAction, &QAction::triggered, this, [=]() {
            restoreState(defaultWindowState);
            restoreOpenTabs(defaultOpenTabs);
            ui->viewDBToolbarAction->setChecked(!ui->toolbarDB->isHidden());
            ui->viewExtraDBToolbarAction->setChecked(!ui->toolbarExtraDB->isHidden());
            ui->viewProjectToolbarAction->setChecked(!ui->toolbarProject->isHidden());
        });
    QAction* simplifyLayoutAction = layoutMenu->addAction(tr("Simplify Window Layout"));
    simplifyLayoutAction->setShortcut(QKeySequence(tr("Shift+Alt+0")));
    connect(simplifyLayoutAction, &QAction::triggered, this, [=]() {
            ui->viewMenu->findChild<QAction *>(ui->pragmas->accessibleName())->activate(QAction::Trigger);
            ui->dockLog->hide();
            ui->dockPlot->hide();
            ui->dockSchema->hide();
            ui->dockEdit->hide();
            ui->dockRemote->hide();
        });
    QAction* atBottomLayoutAction = layoutMenu->addAction(tr("Dock Windows at Bottom"));
    connect(atBottomLayoutAction, &QAction::triggered, this, [=]() {
            moveDocksTo(Qt::BottomDockWidgetArea);
        });
    QAction* atLeftLayoutAction = layoutMenu->addAction(tr("Dock Windows at Left Side"));
    connect(atLeftLayoutAction, &QAction::triggered, this, [=]() {
            moveDocksTo(Qt::LeftDockWidgetArea);
        });
    QAction* atTopLayoutAction = layoutMenu->addAction(tr("Dock Windows at Top"));
    connect(atTopLayoutAction, &QAction::triggered, this, [=]() {
            moveDocksTo(Qt::TopDockWidgetArea);
        });

    // Set Alt+[1-4] shortcuts for opening the corresponding tab in that position.
    // Note that it is safe to call setCurrentIndex with a tab that is currently closed,
    // since setCurrentIndex does nothing in that case.
    QShortcut* setTab1Shortcut = new QShortcut(QKeySequence("Alt+1"), this);
    connect(setTab1Shortcut, &QShortcut::activated, this, [this]() { ui->mainTab->setCurrentIndex(0); });
    QShortcut* setTab2Shortcut = new QShortcut(QKeySequence("Alt+2"), this);
    connect(setTab2Shortcut, &QShortcut::activated, this, [this]() { ui->mainTab->setCurrentIndex(1); });
    QShortcut* setTab3Shortcut = new QShortcut(QKeySequence("Alt+3"), this);
    connect(setTab3Shortcut, &QShortcut::activated, this, [this]() { ui->mainTab->setCurrentIndex(2); });
    QShortcut* setTab4Shortcut = new QShortcut(QKeySequence("Alt+4"), this);
    connect(setTab4Shortcut, &QShortcut::activated, this, [this]() { ui->mainTab->setCurrentIndex(3); });

    // If we're not compiling in SQLCipher, hide its FAQ link in the help menu
#ifndef ENABLE_SQLCIPHER
    ui->actionSqlCipherFaq->setVisible(false);
#endif

    // Set statusbar fields
    statusBusyLabel = new QLabel(ui->statusbar);
    statusBusyLabel->setEnabled(false);
    statusBusyLabel->setVisible(false);
    statusBusyLabel->setToolTip(tr("The database is currenctly busy."));
    ui->statusbar->addPermanentWidget(statusBusyLabel);

    statusStopButton = new QToolButton(ui->statusbar);
    statusStopButton->setVisible(false);
    statusStopButton->setIcon(QIcon(":icons/cancel"));
    statusStopButton->setToolTip(tr("Click here to interrupt the currently running query."));
    statusStopButton->setMaximumSize(ui->statusbar->geometry().height() - 6, ui->statusbar->geometry().height() - 6);
    statusStopButton->setAutoRaise(true);
    ui->statusbar->addPermanentWidget(statusStopButton);

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
    connect(ui->editModifyObjectAction, &QAction::changed, this, [=]() {
        ui->editModifyObjectAction->setIconText(ui->editModifyObjectAction->text());
        ui->editModifyObjectAction->setToolTip(ui->editModifyObjectAction->text());
    });
    connect(ui->editDeleteObjectAction, &QAction::changed, this, [=]() {
        ui->editDeleteObjectAction->setIconText(ui->editDeleteObjectAction->text());
        ui->editDeleteObjectAction->setToolTip(ui->editDeleteObjectAction->text());
    });

    // When clicking the interrupt query button in the status bar, ask SQLite to interrupt the current query
    connect(statusStopButton, &QToolButton::clicked, this, [this]() {
       db.interruptQuery();
    });

    // Connect some more signals and slots
    connect(editDock, &EditDialog::recordTextUpdated, this, &MainWindow::updateRecordText);
    connect(editDock, &EditDialog::evaluateText, this, &MainWindow::evaluateText);
    connect(editDock, &EditDialog::requestUrlOrFileOpen, this, &MainWindow::openUrlOrFile);
    connect(ui->dbTreeWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::changeTreeSelection);
    connect(ui->dockEdit, &QDockWidget::visibilityChanged, this, &MainWindow::toggleEditDock);
    connect(remoteDock, SIGNAL(openFile(QString)), this, SLOT(fileOpen(QString)));
    connect(ui->actionDropSelectQueryCheck, &QAction::toggled, dbStructureModel, &DbStructureModel::setDropSelectQuery);
    connect(ui->actionDropQualifiedCheck, &QAction::toggled, dbStructureModel, &DbStructureModel::setDropQualifiedNames);
    connect(ui->actionEnquoteNamesCheck, &QAction::toggled, dbStructureModel, &DbStructureModel::setDropEnquotedNames);
    connect(&db, &DBBrowserDB::databaseInUseChanged, this, &MainWindow::updateDatabaseBusyStatus);

    ui->actionDropSelectQueryCheck->setChecked(Settings::getValue("SchemaDock", "dropSelectQuery").toBool());
    ui->actionDropQualifiedCheck->setChecked(Settings::getValue("SchemaDock", "dropQualifiedNames").toBool());
    ui->actionEnquoteNamesCheck->setChecked(Settings::getValue("SchemaDock", "dropEnquotedNames").toBool());

    connect(ui->actionSqlStop, &QAction::triggered, this, [this]() {
       if(execute_sql_worker && execute_sql_worker->isRunning())
           execute_sql_worker->stop();
    });

    // Connect tool pragmas
    connect(ui->actionIntegrityCheck, &QAction::triggered, this, [this]() {
            runSqlNewTab("PRAGMA integrity_check;", ui->actionIntegrityCheck->text(), "https://www.sqlite.org/pragma.html#pragma_integrity_check");
    });
    connect(ui->actionQuickCheck, &QAction::triggered, this, [this]() {
            runSqlNewTab("PRAGMA quick_check;", ui->actionQuickCheck->text(), "https://www.sqlite.org/pragma.html#pragma_quick_check");
    });
    connect(ui->actionForeignKeyCheck, &QAction::triggered, this, [this]() {
            runSqlNewTab("PRAGMA foreign_key_check;", ui->actionForeignKeyCheck->text(), "https://www.sqlite.org/pragma.html#pragma_foreign_key_check");
    });
    connect(ui->actionOptimize, &QAction::triggered, this, [this]() {
            runSqlNewTab("PRAGMA optimize;", ui->actionOptimize->text(), "https://www.sqlite.org/pragma.html#pragma_optimize");
    });

    // Action for switching the table via the Database Structure tab
    connect(ui->actionPopupSchemaDockBrowseTable, &QAction::triggered, this, [this]() {
            switchToBrowseDataTab(dockDbSelected->object());
            refresh();  // Required in case the Browse Data tab already was the active main tab
    });

    // Set other window settings
    setAcceptDrops(true);
    setWindowTitle(QApplication::applicationName());

    // Add the documentation of shortcuts, which aren't otherwise visible in the user interface, to some buttons.
    addShortcutsTooltip(ui->actionDbPrint);
    addShortcutsTooltip(ui->actionSqlOpenTab);
    addShortcutsTooltip(ui->actionSqlOpenFile);
    addShortcutsTooltip(ui->actionSqlPrint);
    addShortcutsTooltip(ui->actionExecuteSql, {shortcutBrowseRefreshF5->key(), shortcutBrowseRefreshCtrlR->key()});
    addShortcutsTooltip(ui->actionSqlExecuteLine);
    addShortcutsTooltip(ui->actionSqlFind);
    addShortcutsTooltip(ui->actionSqlFindReplace);
    addShortcutsTooltip(ui->actionSqlToggleComment);

    // Since in some keyboards pressing F keys is cumbersome, add an alternate shortcut and document
    // it in the toolbar.
    ui->fileCloseAction->setShortcuts({QKeySequence(tr("Ctrl+F4")), QKeySequence(tr("Ctrl+Alt+W"))});
    ui->actionCloseProject->setShortcuts({QKeySequence(tr("Ctrl+Shift+W")), QKeySequence(tr("Ctrl+Shift+F4"))});
    addShortcutsTooltip(ui->fileCloseAction);

    // Load all settings
    reloadSettings();

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

bool MainWindow::fileOpen(const QString& fileName, bool openFromProject, bool readOnly)
{
    bool retval = false;

    QString wFile = fileName;
    // QFile::exist will produce error message if passed empty string.
    // Test string length before usage w/ QFile to silence warning
    if (wFile.isEmpty() || !QFile::exists(wFile))
    {
        wFile = FileDialog::getOpenFileName(
                    OpenDatabaseFile,
                    this,
                    tr("Choose a database file")
#ifndef Q_OS_MAC // Filters on OS X are buggy
                    , FileDialog::getSqlDatabaseFileFilter()
#endif
                    );
    }
    // catch situation where user has canceled file selection from dialog
    if(!wFile.isEmpty() && QFile::exists(wFile) )
    {
        // Try opening it as a project file first. If confirmed, this will include closing current
        // database and project files.
        if(loadProject(wFile, readOnly))
        {
            retval = true;
        } else if(isTextOnlyFile(wFile)) {
            // If it's a text file, cannot be an SQLite/SQLCipher database,
            // so try to import it as CSV.
            importCSVfiles({wFile});
        } else {
            // Close the database. If the user didn't want to close it, though, stop here
            if (db.isOpen())
                if(!fileClose())
                    return false;

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
                if(!openFromProject) {
                    addToRecentFilesMenu(wFile, readOnly);
                    // When a new DB file has been open while a project is open, set the project modified.
                    if(!currentProjectFilename.isEmpty())
                        isProjectModified = true;
                }
                if(ui->tabSqlAreas->count() == 0)
                    openSqlTab(true);
                else if(ui->mainTab->currentWidget() == ui->pragmas)
                    loadPragmas();

                refreshTableBrowsers();

                // Update remote dock
                remoteDock->fileOpened(wFile);

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
    QString fileName = FileDialog::getSaveFileName(
                           CreateDatabaseFile,
                           this,
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
        refreshTableBrowsers();
        if(ui->tabSqlAreas->count() == 0)
            openSqlTab(true);
        createTable();
    }
}

void MainWindow::fileNewInMemoryDatabase(bool open_create_dialog)
{
    // Open an in-memory database. We use open() instead of create() here because the extra work create() does is not needed
    // when no files are stored on disk.
    db.open(":memory:");

    setCurrentFile(tr("In-Memory database"));
    statusEncodingLabel->setText(db.getPragma("encoding"));
    statusEncryptionLabel->setVisible(false);
    statusReadOnlyLabel->setVisible(false);
    remoteDock->fileOpened(":memory:");
    refreshTableBrowsers();
    if(ui->tabSqlAreas->count() == 0)
        openSqlTab(true);
    if(open_create_dialog)
        createTable();
}

void MainWindow::populateStructure(const std::vector<sqlb::ObjectIdentifier>& old_tables)
{
    // Refresh the structure tab
    ui->dbTreeWidget->setRootIndex(dbStructureModel->index(1, 0));      // Show the 'All' part of the db structure
    ui->dbTreeWidget->expandToDepth(0);
    ui->treeSchemaDock->setRootIndex(dbStructureModel->index(1, 0));    // Show the 'All' part of the db structure
    ui->treeSchemaDock->expandToDepth(0);

    // Refresh the browse data tabs
    const auto all_table_browsers = allTableBrowserDocks();
    for(int i=0;i<all_table_browsers.size()&&i<static_cast<int>(old_tables.size());i++)
        all_table_browsers.at(i)->tableBrowser()->setStructure(dbStructureModel, old_tables.at(static_cast<size_t>(i)));

    // Cancel here if no database is opened
    if(!db.isOpen())
        return;

    // Update table and column names for syntax highlighting
    SqlUiLexer::QualifiedTablesMap qualifiedTablesMap;
    for(const auto& it : db.schemata)
    {
        SqlUiLexer::TablesAndColumnsMap tablesToColumnsMap;

        for(const auto& jt : it.second.tables)
        {
            QString objectname = QString::fromStdString(jt.first);

            for(const auto& f : jt.second->fields)
                tablesToColumnsMap[objectname].push_back(QString::fromStdString(f.name()));
        }

        qualifiedTablesMap[QString::fromStdString(it.first)] = tablesToColumnsMap;
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

void MainWindow::refreshTableBrowsers()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for(const auto& d : allTableBrowserDocks())
    {
        // When in the Browse Data tab update all docks. Otherwise just update the floating ones because they might
        // be visible even when another tab is active.
        if(ui->mainTab->currentWidget() == ui->browser || d->isFloating())
        {
            TableBrowser* t = d->tableBrowser();
            if(t)
                t->refresh();
        }
    }
    QApplication::restoreOverrideCursor();
}

bool MainWindow::fileSaveAs() {

    QString fileName =  FileDialog::getSaveFileName(
                    OpenDatabaseFile,
                    this,
                    tr("Choose a database file to save under"),
                    FileDialog::getSqlDatabaseFileFilter()
                    );
    // catch situation where user has canceled file selection from dialog
    if(!fileName.isEmpty()) {
        bool result = db.saveAs(fileName.toStdString());
        if(result) {
            setCurrentFile(fileName);
            addToRecentFilesMenu(fileName);
        } else {
            QMessageBox::warning(this, QApplication::applicationName(),
                                 tr("Error while saving the database to the new file."));
        }
        return result;
    } else {
        return false;
    }
}

bool MainWindow::fileClose()
{
    // Stop any running SQL statements before closing the database
    if(execute_sql_worker && execute_sql_worker->isRunning())
    {
        if(QMessageBox::warning(this, qApp->applicationName(),
                                tr("You are still executing SQL statements. Closing the database now will stop their execution, possibly "
                                   "leaving the database in an inconsistent state. Are you sure you want to close the database?"),
                                QMessageBox::Yes, QMessageBox::Cancel | QMessageBox::Default | QMessageBox::Escape) == QMessageBox::Cancel)
            return false;

        execute_sql_worker->stop();
        execute_sql_worker->wait();
    }

    // Close the database but stop the closing process here if the user pressed the cancel button in there
    if(!db.close())
        return false;

    TableBrowser::resetSharedSettings();
    setCurrentFile(QString());
    loadPragmas();
    statusEncryptionLabel->setVisible(false);
    statusReadOnlyLabel->setVisible(false);

    // Reset the table browser of the Browse Data tab.
    // We call delete here to avoid the closed() signal being emitted
    // which would open a new dock.
    for(auto d : allTableBrowserDocks())
        delete d;
    newTableBrowserTab();
    allTableBrowserDocks().at(0)->tableBrowser()->setEnabled(false);

    // Clear edit dock
    editDock->setCurrentIndex(QModelIndex());

    // Clear the SQL Log
    ui->editLogApplication->clear();
    ui->editLogUser->clear();
    ui->editLogErrorLog->clear();

    // Remove completion and highlighting for identifiers
    SqlTextEdit::sqlLexer->setTableNames(SqlUiLexer::QualifiedTablesMap());
    for(int i=0; i < ui->tabSqlAreas->count(); i++)
        qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i))->getEditor()->reloadKeywords();

    // Clear remote dock
    remoteDock->fileOpened(QString());

    return true;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
    if(closeFiles())
    {
        Settings::setValue("MainWindow", "geometry", saveGeometry());
        Settings::setValue("MainWindow", "windowState", saveState());
        Settings::setValue("MainWindow", "openTabs", saveOpenTabs());

        Settings::setValue("SQLLogDock", "Log", ui->comboLogSubmittedBy->currentText());
        Settings::setValue("SchemaDock", "dropSelectQuery", ui->actionDropSelectQueryCheck->isChecked());
        Settings::setValue("SchemaDock", "dropQualifiedNames", ui->actionDropQualifiedCheck->isChecked());
        Settings::setValue("SchemaDock", "dropEnquotedNames", ui->actionEnquoteNamesCheck->isChecked());

        SqlExecutionArea::saveState();

        Settings::sync();

        QMainWindow::closeEvent(event);
    } else {
        event->ignore();
    }
}

bool MainWindow::closeFiles()
{
    bool ignoreUnattachedBuffers = false;
    // Ask for saving all modified open SQL files in their files and all the unattached tabs in a project file.
    for(int i=0; i<ui->tabSqlAreas->count(); i++) {
        // Ask for saving and comply with cancel answer.
        if(!askSaveSqlTab(i, ignoreUnattachedBuffers))
            return false;
    }

    // Now all tabs can be closed at once without asking user.
    // Close tabs in reverse order (so indexes are not changed in the process).
    for(int i=ui->tabSqlAreas->count()-1; i>=0; i--)
        closeSqlTab(i, /* force */ true, /* askSaving */ false);

    return closeProject();
}

bool MainWindow::closeProject()
{
    if(!currentProjectFilename.isEmpty() && isProjectModified) {
        QMessageBox::StandardButton reply = QMessageBox::question
            (nullptr,
             QApplication::applicationName(),
             tr("Do you want to save the changes made to the project file '%1'?").
             arg(QFileInfo(currentProjectFilename).fileName()),
             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch(reply) {
        case QMessageBox::Save:
            saveProject();
            break;
        case QMessageBox::Cancel:
            return false;
        default:
            break;
        }
    }
    currentProjectFilename.clear();
    return fileClose();
}

void MainWindow::attachPlot(ExtendedTableWidget* tableWidget, SqliteTableModel* model, BrowseDataTableSettings* settings, bool keepOrResetSelection)
{
    plotDock->updatePlot(model, settings, true, keepOrResetSelection);
    // Disconnect previous connection
    disconnect(plotDock, &PlotDock::pointsSelected, nullptr, nullptr);
    if(tableWidget) {
        // Connect plot selection to the current table results widget.
        connect(plotDock, &PlotDock::pointsSelected, tableWidget, &ExtendedTableWidget::selectTableLines);
        connect(tableWidget, &ExtendedTableWidget::destroyed, plotDock, &PlotDock::resetPlot);
        // Disconnect requestUrlOrFileOpen in order to make sure that there is only one connection. Otherwise we can open it several times.
        disconnect(tableWidget, &ExtendedTableWidget::requestUrlOrFileOpen, this, &MainWindow::openUrlOrFile);
        connect(tableWidget, &ExtendedTableWidget::requestUrlOrFileOpen, this, &MainWindow::openUrlOrFile);
    }
}

void MainWindow::refresh()
{
    // What the Refresh function does depends on the currently active tab. This way the keyboard shortcuts (F5 and Ctrl+R)
    // always perform some meaningful task; they just happen to be context dependent in the function they trigger.
    QWidget* currentTab = ui->mainTab->currentWidget();
    if (currentTab == ui->structure) {
        // Refresh the schema
        db.updateSchema();
    } else if (currentTab == ui->browser) {
        // Refresh the schema and reload the current table
        refreshTableBrowsers();
    } else if (currentTab == ui->pragmas) {
        // Reload pragma values
        loadPragmas();
    } else if (currentTab == ui->query) {
        // (Re-)Run the current SQL query
        executeQuery();
    }
}

void MainWindow::createTable()
{
    EditTableDialog dialog(db, sqlb::ObjectIdentifier(), true, this);
    if(dialog.exec())
    {
        refreshTableBrowsers();
    }
}

void MainWindow::createIndex()
{
    EditIndexDialog dialog(db, sqlb::ObjectIdentifier(), true, this);
    if(dialog.exec())
        refreshTableBrowsers();
}

void MainWindow::compact()
{
    VacuumDialog dialog(&db, this);
    dialog.exec();
}

void MainWindow::deleteObject()
{
    // Get name and type of object to delete
    sqlb::ObjectIdentifier obj = dbSelected->object();
    QString type = dbSelected->objectType();

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
    if(QMessageBox::warning(this, QApplication::applicationName(), message.arg(QString::fromStdString(obj.name())),
                            QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        // Delete the table
        QString statement = QString("DROP %1 %2;").arg(type.toUpper(), QString::fromStdString(obj.toString()));
        if(!db.executeSQL(statement.toStdString()))
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
            refreshTableBrowsers();
            changeTreeSelection();
        }
    }
}

void MainWindow::editObject()
{
    if(!dbSelected->hasSelection())
        return;

    // Get name and type of the object to edit
    sqlb::ObjectIdentifier obj = dbSelected->object();
    QString type = dbSelected->objectType();

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

        EditTableDialog dialog(db, obj, false, this);
        bool ok = dialog.exec();

        // If foreign_keys were enabled, we must commit or rollback the transaction so the foreign_keys pragma can be restored.
        if (foreign_keys == "1") {
            if (!db.querySingleValueFromDb("PRAGMA " + sqlb::escapeIdentifier(obj.schema()) + ".foreign_key_check").isNull()) {
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
            for(const auto& d : allTableBrowserDocks())
            {
                if(d->tableBrowser()->currentlyBrowsedTableName() == obj)
                    d->tableBrowser()->clearFilters();
            }
            refreshTableBrowsers();
        }
    } else if(type == "index") {
        EditIndexDialog dialog(db, obj, false, this);
        if(dialog.exec())
            refreshTableBrowsers();
    } else if(type == "view") {
        sqlb::TablePtr view = db.getTableByName(obj);
        runSqlNewTab(QString("DROP VIEW IF EXISTS %1;\n%2").arg(QString::fromStdString(obj.toString()), QString::fromStdString(view->sql())),
                     tr("Edit View %1").arg(QString::fromStdString(obj.toDisplayString())),
                     "https://www.sqlite.org/lang_createview.html",
                     /* autoRun */ false);
    } else if(type == "trigger") {
        sqlb::TriggerPtr trigger = db.getTriggerByName(obj);
        runSqlNewTab(QString("DROP TRIGGER %1;\n%2").arg(QString::fromStdString(obj.toString()), QString::fromStdString(trigger->sql())),
                     tr("Edit Trigger %1").arg(QString::fromStdString(obj.toDisplayString())),
                     "https://www.sqlite.org/lang_createtrigger.html",
                     /* autoRun */ false);
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

void MainWindow::evaluateText(const QPersistentModelIndex& idx, const std::string& text)
{
    QByteArray value = db.querySingleValueFromDb("SELECT " + text, /* log */ true, DBBrowserDB::Wait);
    m_currentTabTableModel->setTypedData(idx, !isTextOnly(value), value);
}

void MainWindow::toggleEditDock(bool visible)
{
    if (!visible) {
        // Update main window
        ui->tabBrowsers->setFocus();
    } else {
        // fill edit dock with actual data, when the current index has changed while the dock was invisible.
        // (note that this signal is also emitted when the widget is docked or undocked, so we have to avoid
        // reloading data when the user is editing and (un)docks the editor).
        if (currentTableBrowser && editDock->currentIndex() != currentTableBrowser->currentIndex())
            editDock->setCurrentIndex(currentTableBrowser->currentIndex());
    }
}

void MainWindow::doubleClickTable(const QModelIndex& index)
{
    // Cancel on invalid index
    if (!index.isValid()) {
        return;
    }

    // * Don't allow editing of other objects than tables and editable views
    bool isEditingAllowed = !db.readOnly() && currentTableBrowser && m_currentTabTableModel == currentTableBrowser->model() && currentTableBrowser->model()->isEditable(index);

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

    TableBrowserDock* dock = qobject_cast<TableBrowserDock*>(index.model()->parent()->parent());
    if(dock)
        changeTableBrowserTab(dock);

    bool editingAllowed = !db.readOnly() && currentTableBrowser && m_currentTabTableModel == currentTableBrowser->model() && currentTableBrowser->model()->isEditable(index);

    // Don't allow editing of other objects than tables and editable views
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
 */
void MainWindow::executeQuery()
{
    // Make sure a database is opened. This is necessary because we allow opened SQL editor tabs even if no database is loaded. Hitting F5 or similar
    // then might call this function.
    if(!db.isOpen())
        return;

    // Check if other task is still running and stop it if necessary
    if(execute_sql_worker && execute_sql_worker->isRunning())
    {
        // Ask the user and do nothing if he/she doesn't want to interrupt the running query
        if(QMessageBox::warning(this, qApp->applicationName(),
                                tr("You are already executing SQL statements. Do you want to stop them in order to execute the current "
                                   "statements instead? Note that this might leave the database in an inconsistent state."),
                                QMessageBox::Yes, QMessageBox::Cancel | QMessageBox::Default | QMessageBox::Escape) == QMessageBox::Cancel)
            return;

        // Stop the running query
        execute_sql_worker->stop();
        execute_sql_worker->wait();
    }

    // Get current SQL tab and editor
    SqlExecutionArea* sqlWidget = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());
    SqlTextEdit* editor = sqlWidget->getEditor();
    auto* current_tab = ui->tabSqlAreas->currentWidget();
    const QString tabName = ui->tabSqlAreas->tabText(ui->tabSqlAreas->currentIndex()).remove('&');

    // Remove any error indicators
    editor->clearErrorIndicators();
    sqlWidget->getStatusEdit()->clear();
    sqlWidget->getStatusEdit()->setStyleSheet("");

    // Determine execution mode: execute all, execute selection or execute current line
    enum executionMode
    {
        All,
        Selection,
        Line
    } mode;
    if(sender() && sender()->objectName() == "actionSqlExecuteLine")
        mode = Line;
    else if(!sqlWidget->getSelectedSql().isEmpty())
        mode = Selection;
    else
        mode = All;

    // Get SQL code to execute. This depends on the execution mode.
    int execute_from_position = 0;      // Where we want to start the execution in the query string
    int execute_to_position = 0;        // Where we roughly want to end the execution in the query string

    switch(mode)
    {
    case Selection:
        {
            // Start and end positions are start and end positions from the selection
            int execute_from_line, execute_from_index, execute_to_line, execute_to_index;
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
            int execute_from_line, dummy;
            editor->getCursorPosition(&execute_from_line, &dummy);
            execute_from_position = editor->positionFromLineIndex(execute_from_line, 0);

            // Need to set the end position here before adjusting the start line
            int execute_to_line = execute_from_line;
            int execute_to_index = editor->text(execute_to_line).remove('\n').remove('\r').length();     // This chops the line break at the end of the line
            execute_to_position = editor->positionFromLineIndex(execute_to_line, execute_to_index);

            QByteArray firstPartEntireSQL = sqlWidget->getSql().toUtf8().left(execute_from_position);
            if(firstPartEntireSQL.lastIndexOf(';') != -1)
                execute_from_position -= firstPartEntireSQL.length() - firstPartEntireSQL.lastIndexOf(';') - 1;

            db.logSQL(tr("-- EXECUTING LINE IN '%1'\n--").arg(tabName), kLogMsg_User);
        } break;
    case All:
        {
            // Start position is the first byte, end position the last.
            // Note that we use byte positions that might differ from character positions.
            execute_to_position = editor->length();

            db.logSQL(tr("-- EXECUTING ALL IN '%1'\n--").arg(tabName), kLogMsg_User);
        } break;
    }

    // Prepare a lambda function for logging the results of a query
    auto query_logger = [this, sqlWidget, editor](bool ok, const QString& status_message, int from_position, int to_position) {
        int execute_from_line, execute_from_index;
        editor->lineIndexFromPosition(from_position, &execute_from_line, &execute_from_index);

        // Special case: if the start position is at the end of a line, then move to the beginning of next line.
        // Otherwise for the typical case, the line reference is one less than expected.
        // Note that execute_from_index uses character positions and not byte positions, so at() can be used.
        QChar char_at_index = editor->text(execute_from_line).at(execute_from_index);
        if (char_at_index == '\r' || char_at_index == '\n') {
            execute_from_line++;
            // The next lines could be empty, so skip all of them too.
            while(editor->text(execute_from_line).trimmed().isEmpty())
                execute_from_line++;
            execute_from_index = 0;
        }

        // If there was an error highlight the erroneous SQL statement
        if(!ok)
        {
            int end_of_current_statement_line, end_of_current_statement_index;
            editor->lineIndexFromPosition(to_position, &end_of_current_statement_line, &end_of_current_statement_index);
            editor->setErrorIndicator(execute_from_line, execute_from_index, end_of_current_statement_line, end_of_current_statement_index);

            editor->setCursorPosition(execute_from_line, execute_from_index);
        }

        // Log the query and the result message.
        // The query takes the last placeholder as it may itself contain the sequence '%' + number.
        QString query = editor->text(from_position, to_position);
        QString log_message = "-- " + tr("At line %1:").arg(execute_from_line+1) + "\n" + query.trimmed() + "\n-- " + tr("Result: %1").arg(status_message);
        db.logSQL(log_message, kLogMsg_User);

        log_message = tr("Result: %2").arg(status_message) + "\n" + tr("At line %1:").arg(execute_from_line+1) + "\n" + query.trimmed();
        // Update the execution area
        sqlWidget->finishExecution(log_message, ok);
    };

    // Get the statement(s) to execute. When in selection mode crop the query string at exactly the end of the selection to make sure SQLite has
    // no chance to execute any further.
    QString sql = sqlWidget->getSql();
    if(mode == Selection)
        sql = sql.toUtf8().left(execute_to_position);   // We have to convert to a QByteArray here because QScintilla gives us the position in bytes, not in characters.

    // Prepare the SQL worker to run the query. We set the context of each signal-slot connection to the current SQL execution area.
    // This means that if the tab is closed all these signals are automatically disconnected so the lambdas won't be called for a not
    // existing execution area.
    execute_sql_worker.reset(new RunSql(db, sql, execute_from_position, execute_to_position, true));

    connect(execute_sql_worker.get(), &RunSql::structureUpdated, sqlWidget, [this]() {
        db.updateSchema();
    }, Qt::QueuedConnection);
    connect(execute_sql_worker.get(), &RunSql::statementErrored, sqlWidget, [query_logger, this](const QString& status_message, int from_position, int to_position) {
        ui->actionSqlResultsSave->setEnabled(false);
        ui->actionSqlResultsSaveAsView->setEnabled(false);

        query_logger(false, status_message, from_position, to_position);
    }, Qt::QueuedConnection);
    connect(execute_sql_worker.get(), &RunSql::statementExecuted, sqlWidget, [query_logger, this](const QString& status_message, int from_position, int to_position) {
        ui->actionSqlResultsSave->setEnabled(false);
        ui->actionSqlResultsSaveAsView->setEnabled(false);

        query_logger(true, status_message, from_position, to_position);
        execute_sql_worker->startNextStatement();
    }, Qt::QueuedConnection);
    connect(execute_sql_worker.get(), &RunSql::statementReturnsRows, sqlWidget, [query_logger, this, sqlWidget](const QString& query, int from_position, int to_position, qint64 time_in_ms_so_far) {
        auto time_start = std::chrono::high_resolution_clock::now();

        ui->actionSqlResultsSave->setEnabled(true);
        ui->actionSqlResultsSaveAsView->setEnabled(!db.readOnly());

        auto * model = sqlWidget->getModel();
        model->setQuery(query);

        // Wait until the initial loading of data (= first chunk and row count) has been performed
        auto conn = std::make_shared<QMetaObject::Connection>();
        *conn = connect(model, &SqliteTableModel::finishedFetch, this, [=](int begin, int end) {
            // Skip callback if it's is a chunk load.
            // Now query_logger displays the correct value for "rows returned", not "Prefetch block size"
            if (begin == 0 && end > 0) {
                return;
            }
            disconnect(*conn);

            attachPlot(sqlWidget->getTableResult(), sqlWidget->getModel());

            connect(sqlWidget->getTableResult()->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::dataTableSelectionChanged);
            connect(sqlWidget->getTableResult(), &QTableView::doubleClicked, this, &MainWindow::doubleClickTable);

            auto time_end = std::chrono::high_resolution_clock::now();
            auto time_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_end-time_start);
            query_logger(true, tr("%1 rows returned in %2ms").arg(model->rowCount()).arg(time_in_ms.count()+time_in_ms_so_far), from_position, to_position);
            execute_sql_worker->startNextStatement();
        });
    }, Qt::QueuedConnection);
    connect(execute_sql_worker.get(), &RunSql::confirmSaveBeforePragmaOrVacuum, sqlWidget, [this]() {
        if(QMessageBox::question(nullptr, QApplication::applicationName(),
                                 tr("Setting PRAGMA values or vacuuming will commit your current transaction.\nAre you sure?"),
                                 QMessageBox::Yes | QMessageBox::Default,
                                 QMessageBox::No | QMessageBox::Escape) == QMessageBox::No)
            execute_sql_worker->stop();

    }, Qt::BlockingQueuedConnection);
    connect(execute_sql_worker.get(), &RunSql::finished, sqlWidget, [this, current_tab, sqlWidget]() {
        // We work with a pointer to the current tab here instead of its index because the user might reorder the tabs in the meantime.
        // We set different icons for general tabs, which are either new or loaded from the project file, and for tabs loaded from a file.
        if(sqlWidget->fileName().isEmpty())
            ui->tabSqlAreas->setTabIcon(ui->tabSqlAreas->indexOf(current_tab), QIcon(":/icons/open_sql"));
        else
            ui->tabSqlAreas->setTabIcon(ui->tabSqlAreas->indexOf(current_tab), QIcon(":/icons/document_open"));

        // Set no-running-query state
        ui->tabSqlAreas->tabBar()->setTabData(ui->tabSqlAreas->indexOf(current_tab), QVariant(false));

        // We don't need to check for the current SQL tab here because two concurrently running queries are not allowed
        ui->actionSqlExecuteLine->setEnabled(true);
        ui->actionExecuteSql->setEnabled(true);
        ui->actionSqlStop->setEnabled(false);
        sqlWidget->getEditor()->setReadOnly(false);

        // Show Done message
        if(sqlWidget->inErrorState())
            sqlWidget->getStatusEdit()->setPlainText(tr("Execution finished with errors.") + "\n" + sqlWidget->getStatusEdit()->toPlainText());
        else
            sqlWidget->getStatusEdit()->setPlainText(tr("Execution finished without errors.") + "\n" + sqlWidget->getStatusEdit()->toPlainText());
    });

    // Add an hourglass icon to the current tab to indicate that there's a running execution in there.
    ui->tabSqlAreas->setTabIcon(ui->tabSqlAreas->currentIndex(), QIcon(":icons/hourglass"));
    // We use the tab data to check whether a specific SQL tab is currently running a query or not.
    ui->tabSqlAreas->tabBar()->setTabData(ui->tabSqlAreas->currentIndex(), QVariant(true));

    // Deactivate the buttons to start a query and activate the button to stop the query
    ui->actionSqlExecuteLine->setEnabled(false);
    ui->actionExecuteSql->setEnabled(false);
    ui->actionSqlStop->setEnabled(true);

    // Make the SQL editor widget read-only. We do this because the error indicators would be misplaced if the user changed the SQL text during execution
    sqlWidget->getEditor()->setReadOnly(true);

    // Reset model and clear plot dock
    sqlWidget->getModel()->reset();
    attachPlot(sqlWidget->getTableResult(), sqlWidget->getModel());

    // Start the execution
    execute_sql_worker->start();
}

void MainWindow::mainTabSelected(int /*tabindex*/)
{
    editDock->setReadOnly(true);

    if(ui->mainTab->currentWidget() == ui->browser)
    {
        refreshTableBrowsers();
    } else if(ui->mainTab->currentWidget() == ui->pragmas) {
        loadPragmas();
    } else if(ui->mainTab->currentWidget() == ui->query) {
        SqlExecutionArea* sqlWidget = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());

        if (sqlWidget) {
            m_currentTabTableModel = sqlWidget->getModel();

            dataTableSelectionChanged(sqlWidget->getTableResult()->currentIndex());
        }
    }
}

void MainWindow::importCSVfiles(const std::vector<QString>& inputFiles, const QString& table)
{
    if (!inputFiles.empty())
    {
        // Allow importing to a new in-memory database that can be saved later.
        if(!db.isOpen())
            fileNewInMemoryDatabase(/* open_create_dialog */ false);

        ImportCsvDialog dialog(inputFiles, &db, this, table);
        if (dialog.exec())
            refreshTableBrowsers();
    }
}

void MainWindow::importTableFromCSV()
{
    std::vector<QString> validFiles;

    // Are we importing from a file or from the clipboard?
    if(sender() == ui->fileImportCSVAction)
    {
        // Ask user to specify file(s)

        QStringList file_filter;
        file_filter << FILE_FILTER_CSV
                    << FILE_FILTER_TSV
                    << FILE_FILTER_DSV
                    << FILE_FILTER_TXT
                    << FILE_FILTER_DAT
                    << FILE_FILTER_ALL;

        const QStringList wFiles = FileDialog::getOpenFileNames(
                                 OpenCSVFile,
                                 this,
                                 tr("Choose text files"),
                                 file_filter.join(";;"));

        for(const auto& file : wFiles) {
            if (QFile::exists(file))
                validFiles.push_back(file);
        }
        importCSVfiles(validFiles);

    } else if(sender() == ui->actionFileImportCsvClipboard) {
        // Save clipboard content to temporary file

        QTemporaryFile temp("csv_clipboard");
        temp.open();
        QClipboard* clipboard = QGuiApplication::clipboard();
        temp.write(clipboard->text().toUtf8());
        temp.close();
        validFiles.push_back(temp.fileName());

        // Note that the temporary file will be removed when the object is
        // destroyed, so the reading must be done in the same scope.
        importCSVfiles(validFiles);
    }

}

void MainWindow::exportTableToCSV()
{
    // Get the current table name if we are in the Browse Data tab
    sqlb::ObjectIdentifier current_table;
    if(ui->mainTab->currentWidget() == ui->structure)
    {
        QString type = dbSelected->objectType();
        if(type == "table" || type == "view")
        {
            current_table = dbSelected->object();
        }
    } else if(ui->mainTab->currentWidget() == ui->browser) {
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
    if(ui->mainTab->currentWidget() == ui->structure)
    {
        QString type = dbSelected->objectType();
        if(type == "table" || type == "view")
        {
            current_table = dbSelected->object();
        }
    } else if(ui->mainTab->currentWidget() == ui->browser) {
        current_table = currentlyBrowsedTableName();
    }

    // Open dialog
    ExportDataDialog dialog(db, ExportDataDialog::ExportFormatJson, this, "", current_table);
    dialog.exec();
}

void MainWindow::dbState(bool dirty)
{
    ui->fileSaveAction->setEnabled(dirty);
    ui->fileRevertAction->setEnabled(dirty);
    // Unfortunately, sqlite does not allow to backup the DB while there are pending savepoints,
    // so we cannot "Save As" when the DB is dirty.
    ui->fileSaveAsAction->setEnabled(!dirty);
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
            refreshTableBrowsers();
        }
    }
}

void MainWindow::exportDatabaseToSQL()
{
    QString current_table;
    if(ui->mainTab->currentWidget() == ui->browser)
        current_table = QString::fromStdString(currentlyBrowsedTableName().name());

    ExportSqlDialog dialog(&db, this, current_table);
    dialog.exec();
}

void MainWindow::importDatabaseFromSQL()
{
    QStringList file_filter;
    file_filter << FILE_FILTER_SQL
                << FILE_FILTER_TXT
                << FILE_FILTER_ALL;

    // Get file name to import
    QString fileName = FileDialog::getOpenFileName(
                OpenSQLFile,
                this,
                tr("Choose a file to import"),
                file_filter.join(";;"));

    // Cancel when file doesn't exist
    if(!QFile::exists(fileName))
        return;

    // If there is already a database file opened ask the user whether to import into
    // this one or a new one. If no DB is opened just ask for a DB name directly
    QString newDbFile;
    if((db.isOpen() && QMessageBox::question(this,
                                            QApplication::applicationName(),
                                            tr("Do you want to create a new database file to hold the imported data?\n"
                                               "If you answer no we will attempt to import the data in the SQL file to the current database."),
                                            QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) || !db.isOpen())
    {
        newDbFile = FileDialog::getSaveFileName(
                    CreateDatabaseFile,
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

        // Create the new file and open it in the browser
        db.create(newDbFile);
        db.close();
        fileOpen(newDbFile);
    }

    // Defer foreign keys. Just deferring them instead of disabling them should work fine because in the import we only expect CREATE and INSERT
    // statements which unlike in the Edit Table dialog shouldn't trigger any problems.
    QString foreignKeysOldSettings = db.getPragma("defer_foreign_keys");
    db.setPragma("defer_foreign_keys", "1");

    // Open, read, execute and close file
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    QByteArray filedata = f.readAll();
    removeBom(filedata);
    bool ok = db.executeMultiSQL(filedata, newDbFile.size() == 0);
    // Restore cursor before asking the user to accept the message
    QApplication::restoreOverrideCursor();
    if(!ok)
        QMessageBox::warning(this, QApplication::applicationName(), tr("Error importing data: %1").arg(db.lastError()));
    else if(db.getPragma("foreign_keys") == "1" && !db.querySingleValueFromDb("PRAGMA foreign_key_check").isNull())
        QMessageBox::warning(this, QApplication::applicationName(), tr("Import completed. Some foreign key constraints are violated. Please fix them before saving."));
    else
        QMessageBox::information(this, QApplication::applicationName(), tr("Import completed."));
    f.close();

    // Restore the former foreign key settings
    db.setPragma("defer_foreign_keys", foreignKeysOldSettings);

    // Refresh views
    db.updateSchema();
    refreshTableBrowsers();
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
    if(!dbSelected->hasSelection())
        return;

    QString type = dbSelected->objectType();
    if(type == "table" || type == "view" || type == "trigger" || type == "index" || type == "database")
    {
        // needed for first click on treeView as for first time change QItemSelectionModel::currentChanged doesn't fire
        changeTreeSelection();
        popupTableMenu->exec(ui->dbTreeWidget->mapToGlobal(qPoint));
    }
}

//** DB Schema Dock Context Menu
void MainWindow::createSchemaDockContextMenu(const QPoint &qPoint)
{
    bool enable_browse_table = false;
    bool enable_detach_file = false;

    if(dockDbSelected->hasSelection())
    {
        QString type = dockDbSelected->objectType();
        if(type == "table" || type == "view")
            enable_browse_table = true;
        else if(type == "database" && dockDbSelected->schema() != "main" && dockDbSelected->schema() != "temp")
            enable_detach_file = true;
    }
    ui->actionPopupSchemaDockBrowseTable->setEnabled(enable_browse_table);
    ui->actionPopupSchemaDockDetachDatabase->setEnabled(enable_detach_file);

    popupSchemaDockMenu->exec(ui->treeSchemaDock->mapToGlobal(qPoint));
}

void MainWindow::changeTreeSelection()
{
    // Just assume first that something's selected that can not be edited at all
    ui->editDeleteObjectAction->setEnabled(false);
    ui->editModifyObjectAction->setEnabled(false);
    ui->actionEditBrowseTable->setEnabled(false);
    ui->actionExportCsvPopup->setEnabled(false);
    ui->fileDetachAction->setEnabled(false);
    ui->actionEditCopyCreateStatement->setEnabled(false);

    ui->fileDetachAction->setVisible(false);

    if(!dbSelected->hasSelection())
        return;

    // Change the text and tooltips of the actions
    QString type = dbSelected->objectType();
    QString schema = dbSelected->schema();

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
    } else if(type == "database") {
        ui->editDeleteObjectAction->setVisible(false);
        ui->editModifyObjectAction->setVisible(false);
        ui->fileDetachAction->setVisible(true);
        ui->fileDetachAction->setEnabled(!(schema == "main" || schema == "temp"));
        return;
    } else {
        // Nothing to do for other types. Set the buttons not visible and return.
        ui->editDeleteObjectAction->setVisible(false);
        ui->editModifyObjectAction->setVisible(false);
        ui->fileDetachAction->setVisible(false);
        ui->actionEditCopyCreateStatement->setEnabled(true);
        return;
    }

    ui->editDeleteObjectAction->setVisible(true);
    ui->editModifyObjectAction->setVisible(true);
    ui->actionEditCopyCreateStatement->setEnabled(true);

    // Activate actions
    ui->editDeleteObjectAction->setEnabled(!db.readOnly());
    ui->editModifyObjectAction->setEnabled(!db.readOnly());

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
    {
        QString file = action->data().toString();
        bool read_only = false;
        if(file.startsWith("[ro]"))     // Check if file is in read-only
        {
            file = file.mid(4);
            read_only = true;
        }

        // Holding the Shift key while opening a recent file inverts the read only flag
        if(Application::keyboardModifiers().testFlag(Qt::ShiftModifier))
            read_only = !read_only;

        if(fileOpen(file, false, read_only))
        {
            if(read_only)
                ui->statusbar->showMessage(tr("Opened '%1' in read-only mode from recent file list").arg(file));
            else
                ui->statusbar->showMessage(tr("Opened '%1' from recent file list").arg(file));
        }
    }
}

void MainWindow::updateRecentFileActions()
{
    // Get recent files list from settings
    QStringList files = Settings::getValue("General", "recentFileList").toStringList();

    // Check if files still exist and remove any non-existent file
    for(int i=0;i<files.size();i++)
    {
        // Remove preceding "[ro]" mark for check
        QString file = files.at(i);
        if(file.startsWith("[ro]"))
            file = file.mid(4);

        if(!QFileInfo::exists(file))
        {
            files.removeAt(i);
            i--;
        }
    }

    // Store updated list
    Settings::setValue("General", "recentFileList", files);

    int numRecentFiles = qMin(files.size(), MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString file = files[i];
        bool read_only = false;
        if(file.startsWith("[ro]"))     // Check if file is in read-only
        {
            file = file.mid(4);
            read_only = true;
        }

        QString text = tr("&%1 %2%3").arg(i + 1).arg(QDir::toNativeSeparators(file), read_only ? tr(" (read only)") : "");
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);

        // Add shortcut for opening the file using the keyboard. However, if the application is configured to store
        // more than nine recently opened files don't set shortcuts for the later ones which wouldn't be single digit anymore.
        if(i < 9)
        {
            recentFileActs[i]->setShortcuts({
                                                QKeySequence(static_cast<int>(Qt::CTRL + (Qt::Key_1+static_cast<unsigned int>(i)))),
                                                QKeySequence(static_cast<int>(Qt::CTRL + Qt::SHIFT + (Qt::Key_1+static_cast<unsigned int>(i))))
                                            });
        }
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    setWindowFilePath(fileName);
    if(currentProjectFilename.isEmpty() && fileName.isEmpty())
        setWindowTitle(QApplication::applicationName());
    else if(currentProjectFilename.isEmpty())
        setWindowTitle(QApplication::applicationName() + " - " + QDir::toNativeSeparators(fileName));
    else {
        QFileInfo projectFileInfo(currentProjectFilename);
        QFileInfo dbFileInfo(fileName);
        QString dbFileName;
        if(dbFileInfo.path() == projectFileInfo.path())
            dbFileName = dbFileInfo.fileName();
        else
            dbFileName = QDir::toNativeSeparators(fileName);
        setWindowTitle(QApplication::applicationName() + " - " + QDir::toNativeSeparators(currentProjectFilename) + " [" + dbFileName + "]");
    }
    activateFields(!fileName.isEmpty());
    if(!fileName.isEmpty())
        dbState(db.getDirty());
}

void MainWindow::addToRecentFilesMenu(const QString& filename, bool read_only)
{
    QFileInfo info(filename);
    QString path = info.absoluteFilePath();
    if(read_only)
        path = "[ro]" + path;

    QStringList files = Settings::getValue("General", "recentFileList").toStringList();

    files.removeAll(path);
    files.prepend(path);
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
    const QList<QUrl> urls = event->mimeData()->urls();

    if( urls.isEmpty() )
        return;

    QString fileName = urls.first().toLocalFile();

    if(!fileName.isEmpty()) {

        // If there is no open database, the only possible option is to open the file.
        if (!db.isOpen()) {
            fileOpen(fileName);
            return;
        }
        bool ok;
        const QString open = tr("Open Database or Project");
        const QString attach = tr("Attach Database...");
        const QString import = tr("Import CSV file(s)...");
        QString action = QInputDialog::getItem(this,
                                   qApp->applicationName(),
                                   tr("Select the action to apply to the dropped file(s). <br/>"
                                      "Note: only 'Import' will process more than one file.", "", urls.count()),
                                   {open, attach, import},
                                   0,
                                   false,
                                   &ok);
        if(ok) {
            if (action == open) {
                fileOpen(fileName);
            } else if (action == attach) {
                fileAttach(fileName);
            } else if (action == import) {

                std::vector<QString> validFiles;
                for(const auto& url : urls) {
                    if (QFile::exists(url.toLocalFile()))
                        validFiles.push_back(url.toLocalFile());
                }
                ImportCsvDialog dialog(validFiles, &db, this);
                if (dialog.exec())
                    refreshTableBrowsers();
            }
        }
    }
}

void MainWindow::activateFields(bool enable)
{
    bool write = !db.readOnly();
    bool tempDb = db.currentFile() == ":memory:";

    ui->fileCloseAction->setEnabled(enable);
    ui->fileSaveAsAction->setEnabled(enable);
    ui->fileAttachAction->setEnabled(enable);
    ui->fileCompactAction->setEnabled(enable && write);
    ui->fileExportJsonAction->setEnabled(enable);
    ui->fileExportCSVAction->setEnabled(enable);
    ui->fileExportSQLAction->setEnabled(enable);
    ui->fileImportCSVAction->setEnabled(enable && write);
    ui->actionFileImportCsvClipboard->setEnabled(enable && write);
    ui->editCreateTableAction->setEnabled(enable && write);
    ui->editCreateIndexAction->setEnabled(enable && write);
    ui->actionDbPrint->setEnabled(enable);
    ui->scrollAreaWidgetContents->setEnabled(enable);
    ui->buttonBoxPragmas->setEnabled(enable && write);
    ui->actionExecuteSql->setEnabled(enable);
    ui->actionLoadExtension->setEnabled(enable);
    ui->actionSqlExecuteLine->setEnabled(enable);
    ui->actionSaveProject->setEnabled(enable && !tempDb);
    ui->actionSaveProjectAs->setEnabled(enable && !tempDb);
    ui->actionSaveAll->setEnabled(enable && !tempDb);
    ui->actionEncryption->setEnabled(enable && write && !tempDb);
    ui->actionIntegrityCheck->setEnabled(enable);
    ui->actionQuickCheck->setEnabled(enable);
    ui->actionForeignKeyCheck->setEnabled(enable);
    ui->actionOptimize->setEnabled(enable);
    ui->actionRowCounts->setEnabled(enable);
    ui->dockEdit->setEnabled(enable);
    ui->dockPlot->setEnabled(enable);

    if(!enable)
        ui->actionSqlResultsSave->setEnabled(false);

    remoteDock->enableButtons();

    for(const auto& d : allTableBrowserDocks())
        d->tableBrowser()->setEnabled(enable);
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    for(const auto& d : allTableBrowserDocks())
        d->tableBrowser()->updateRecordsetLabel();
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
    isProjectModified = true;

    updatePragmaUi();
}

void MainWindow::logSql(const QString& sql, int msgtype)
{
    if(msgtype == kLogMsg_User)
    {
        ui->editLogUser->append(sql + "\n");
        ui->editLogUser->verticalScrollBar()->setValue(ui->editLogUser->verticalScrollBar()->maximum());
    } else if(msgtype == kLogMsg_App) {
        ui->editLogApplication->append(sql + "\n");
        ui->editLogApplication->verticalScrollBar()->setValue(ui->editLogApplication->verticalScrollBar()->maximum());
    } else if(msgtype == kLogMsg_ErrorLog) {
        ui->editLogErrorLog->append(sql + "\n");
        ui->editLogErrorLog->verticalScrollBar()->setValue(ui->editLogErrorLog->verticalScrollBar()->maximum());
    }
}

// Ask user to save the buffer in the specified tab index.
// ignoreUnattachedBuffers is used to store answer about buffers not linked to files, so user is only asked once about them.
// Return true unless user wants to cancel the invoking action.
bool MainWindow::askSaveSqlTab(int index, bool& ignoreUnattachedBuffers)
{
    SqlExecutionArea* sqlExecArea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(index));
    const bool isPromptSQLTabsInNewProject = Settings::getValue("General", "promptsqltabsinnewproject").toBool();

    if(sqlExecArea->getEditor()->isModified()) {
        if(sqlExecArea->fileName().isEmpty() && !ignoreUnattachedBuffers && isPromptSQLTabsInNewProject) {
            // Once the project is saved, remaining SQL tabs will not be modified, so this is only expected to be asked once.
            QString message = currentProjectFilename.isEmpty() ?
                tr("Do you want to save the changes made to SQL tabs in a new project file?") :
                tr("Do you want to save the changes made to SQL tabs in the project file '%1'?").
                arg(QFileInfo(currentProjectFilename).fileName());
            QMessageBox::StandardButton reply = QMessageBox::question(nullptr,
                                                                      QApplication::applicationName(),
                                                                      message,
                                                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            switch(reply) {
            case QMessageBox::Save:
                saveProject();
                break;
            case QMessageBox::Cancel:
                return false;
            default:
                ignoreUnattachedBuffers = true;
                break;
            }
        } else if(!sqlExecArea->fileName().isEmpty()) {
            QMessageBox::StandardButton reply =
                QMessageBox::question(nullptr,
                                      QApplication::applicationName(),
                                      tr("Do you want to save the changes made to the SQL file %1?").
                                      arg(QFileInfo(sqlExecArea->fileName()).fileName()),
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            switch(reply) {
            case QMessageBox::Save:
                saveSqlFile(index);
                break;
            case QMessageBox::Cancel:
                return false;
            default:
                break;
            }
        }
    }
    return true;
}

void MainWindow::closeSqlTab(int index, bool force, bool askSaving)
{
    // Check if we're still executing statements from this tab and stop them before proceeding
    if(ui->tabSqlAreas->tabBar()->tabData(index).toBool())
    {
        if(QMessageBox::warning(this, qApp->applicationName(),
                                tr("The statements in the tab '%1' are still executing. Closing the tab will stop the "
                                   "execution. This might leave the database in an inconsistent state. Are you sure "
                                   "you want to close the tab?").arg(ui->tabSqlAreas->tabBar()->tabText(index)),
                                QMessageBox::Yes,
                                QMessageBox::Cancel | QMessageBox::Default | QMessageBox::Escape) == QMessageBox::Cancel)
            return;

        execute_sql_worker->stop();
        execute_sql_worker->wait();
    }
    // Ask for saving and comply with cancel answer.
    bool ignoreUnattachedBuffers = false;
    if (askSaving && !askSaveSqlTab(index, ignoreUnattachedBuffers))
        return;
    // Remove the tab and delete the widget
    QWidget* w = ui->tabSqlAreas->widget(index);
    ui->tabSqlAreas->removeTab(index);
    delete w;

    // Don't let an empty tab widget
    if(ui->tabSqlAreas->count() == 0 && !force)
        openSqlTab(true);

    // Set focus to the currently selected editor tab.
    focusSqlEditor();
}

int MainWindow::openSqlTab(bool resetCounter)
{
    static int tabNumber = 0;

    if(resetCounter)
        tabNumber = 0;

    // Create new tab, add it to the tab widget and select it
    SqlExecutionArea* w = new SqlExecutionArea(db, this);
    int index = ui->tabSqlAreas->addTab(w, QString("SQL %1").arg(++tabNumber));
    ui->tabSqlAreas->setCurrentIndex(index);
    w->setFindFrameVisibility(ui->actionSqlFind->isChecked());
    // Disable the find dialog in the SQL tabs, since the shortcut
    // would interfere with the search bar and it'd be anyway redundant.
    w->getEditor()->setEnabledFindDialog(false);
    w->getEditor()->setFocus();
    connect(w, &SqlExecutionArea::findFrameVisibilityChanged, ui->actionSqlFind, &QAction::setChecked);

    // Connect now the find shortcut to the editor with widget context, so it isn't ambiguous with other Scintilla Widgets.
    QShortcut* shortcutFind = new QShortcut(ui->actionSqlFind->shortcut(), w->getEditor(), nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutFind, &QShortcut::activated, ui->actionSqlFind, &QAction::toggle);
    ui->tabSqlAreas->setTabIcon(index, QIcon(":icons/open_sql"));
    // The new tab is not currently running a query
    ui->tabSqlAreas->tabBar()->setTabData(index, false);

    return index;
}

void MainWindow::changeSqlTab(int index)
{
    // Instead of figuring out if there are some execution results in the new tab and which statement was used to generate them,
    // we just disable the export buttons in the toolbar.
    ui->actionSqlResultsSave->setEnabled(false);

    // Check if the new tab is currently running a query or not
    if(!ui->tabSqlAreas->tabBar()->tabData(index).toBool())
    {
        // Not running a query

        ui->actionSqlExecuteLine->setEnabled(db.isOpen());
        ui->actionExecuteSql->setEnabled(db.isOpen());
        ui->actionSqlStop->setEnabled(false);
    } else {
        // Running a query

        ui->actionSqlExecuteLine->setEnabled(false);
        ui->actionExecuteSql->setEnabled(false);
        ui->actionSqlStop->setEnabled(true);
    }
    SqlExecutionArea* sqlWidget = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());
    if (sqlWidget) {
      m_currentTabTableModel = sqlWidget->getModel();

      dataTableSelectionChanged(sqlWidget->getTableResult()->currentIndex());
    }
}

void MainWindow::openSqlFile(int tabindex, QString filename)
{
    SqlExecutionArea* sqlarea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(tabindex));
    sqlarea->openFile(filename);
    QFileInfo fileinfo(filename);
    ui->tabSqlAreas->setTabText(tabindex, fileinfo.fileName());
    ui->tabSqlAreas->setTabIcon(tabindex, QIcon(":/icons/document_open"));
}

void MainWindow::openSqlFile()
{
    const QStringList wfiles = FileDialog::getOpenFileNames(
                OpenSQLFile,
                this,
                tr("Select SQL file to open"),
                tr("Text files(*.sql *.txt);;All files(*)"));

    for(const QString& file: wfiles)
    {
        if(QFile::exists(file))
        {
            // Decide whether to open a new tab or take the current one
            int index;
            SqlExecutionArea* current_tab = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());
            if(current_tab && current_tab->getSql().isEmpty() && current_tab->getModel()->rowCount() == 0)
                index = ui->tabSqlAreas->currentIndex();
            else
                index = openSqlTab();

            openSqlFile(index, file);
        }
    }
}

void MainWindow::saveSqlFile(int tabIndex)
{
    SqlExecutionArea* sqlarea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(tabIndex));
    if(!sqlarea)
        return;

    // If this SQL file hasn't been saved before open the Save As dialog. Otherwise just use the old file name for saving
    if(sqlarea->fileName().isEmpty())
    {
        saveSqlFileAs();
    } else {
        sqlarea->saveFile(sqlarea->fileName());
    }
}

void MainWindow::saveSqlFile()
{
    saveSqlFile(ui->tabSqlAreas->currentIndex());
}

void MainWindow::saveSqlFileAs()
{
    SqlExecutionArea* sqlarea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());
    if(!sqlarea)
        return;

    QStringList file_filter;
    file_filter << FILE_FILTER_SQL
                << FILE_FILTER_TXT
                << FILE_FILTER_ALL;
    QString file = FileDialog::getSaveFileName(
                CreateSQLFile,
                this,
                tr("Select file name"),
                file_filter.join(";;"));

    if(!file.isEmpty())
    {
        sqlarea->saveFile(file);

        QFileInfo fileinfo(file);
        ui->tabSqlAreas->setTabText(ui->tabSqlAreas->currentIndex(), fileinfo.fileName());
        ui->tabSqlAreas->setTabIcon(ui->tabSqlAreas->currentIndex(), QIcon(":/icons/document_open"));
    }
}

void MainWindow::saveSqlResultsAsCsv()
{
    qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget())->saveAsCsv();
}

void MainWindow::saveSqlResultsAsJson()
{
    qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget())->saveAsJson();
}

void MainWindow::saveSqlResultsAsView()
{
    saveAsView(qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget())->getModel()->query());
}

void MainWindow::loadExtension()
{
    QStringList file_filter;
    file_filter << FILE_FILTER_DYN
                << FILE_FILTER_ALL;

    QString file = FileDialog::getOpenFileName(
                OpenExtensionFile,
                this,
                tr("Select extension file"),
                file_filter.join(";;"));

    if(file.isEmpty())
        return;

    if(db.loadExtension(file))
        QMessageBox::information(this, QApplication::applicationName(), tr("Extension successfully loaded."));
    else
        QMessageBox::warning(this, QApplication::applicationName(), tr("Error loading extension: %1").arg(db.lastError()));
}

void MainWindow::reloadSettings()
{
    // Set default application font size
    qobject_cast<Application*>(qApp)->reloadSettings();

    // Set data browser font
    for(const auto& d : allTableBrowserDocks())
        d->tableBrowser()->reloadSettings();

    // Set max recent files
    const int newMaxRecentFiles = Settings::getValue("General", "maxRecentFiles").toInt();

    if(MaxRecentFiles < newMaxRecentFiles) {
        // If user increase max recent files value.
        ui->fileRecentFiles->removeAction(clearRecentFilesAction);

        recentFileActs.resize(newMaxRecentFiles);
        for(int i = MaxRecentFiles; i < newMaxRecentFiles; ++i) {
            recentFileActs[i] = new QAction(this);
            recentFileActs[i]->setVisible(false);
            connect(recentFileActs[i], &QAction::triggered, this, &MainWindow::openRecentFile);
        }

        for(int i = 0; i < newMaxRecentFiles; ++i)
            ui->fileRecentFiles->insertAction(ui->fileExitAction, recentFileActs[i]);

        ui->fileRecentFiles->insertSeparator(ui->fileExitAction);
        ui->fileRecentFiles->insertAction(ui->fileExitAction, clearRecentFilesAction);

        MaxRecentFiles = newMaxRecentFiles;
        updateRecentFileActions();
    } else if (MaxRecentFiles > newMaxRecentFiles) {
        // If user decrease max recent files value.
        for(int i = (MaxRecentFiles - 1); i >= newMaxRecentFiles; --i) {
            ui->fileRecentFiles->removeAction(recentFileActs[i]);
            delete recentFileActs[i];
        }

        recentFileActs.resize(newMaxRecentFiles);
        MaxRecentFiles = newMaxRecentFiles;
        updateRecentFileActions();
    }

    Settings::AppStyle style = static_cast<Settings::AppStyle>(Settings::getValue("General", "appStyle").toInt());

    switch (style) {
    case Settings::FollowDesktopStyle :
        qApp->setStyleSheet("");

        break;
    case Settings::DarkStyle :
    case Settings::LightStyle :
        QFile f(style == Settings::DarkStyle ? ":qdarkstyle/dark/style.qss" : ":qdarkstyle/light/style.qss");
        if (!f.exists()) {
            QMessageBox::warning(this, qApp->applicationName(),
                               tr("Could not find resource file: %1").arg(f.fileName()));
        } else {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);
            qApp->setStyleSheet(ts.readAll());
        }
        break;
    }

    setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(Settings::getValue("General", "toolbarStyle").toInt()));
    ui->dbToolbar->setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(Settings::getValue("General", "toolbarStyleStructure").toInt()));
    ui->toolbarSql->setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(Settings::getValue("General", "toolbarStyleSql").toInt()));

    // Set prefetch sizes for lazy population of table models
    for(int i=0;i<ui->tabSqlAreas->count();++i)
        qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i))->reloadSettings();

    // Prepare log font
    QFont logfont("Monospace");
    logfont.setStyleHint(QFont::TypeWriter);
    logfont.setPointSize(Settings::getValue("log", "fontsize").toInt());

    // Set font for SQL logs and edit dialog
    ui->editLogApplication->reloadSettings();
    ui->editLogUser->reloadSettings();
    ui->editLogErrorLog->reloadSettings();
    ui->editLogApplication->setFont(logfont);
    ui->editLogUser->setFont(logfont);
    ui->editLogErrorLog->setFont(logfont);
    editDock->reloadSettings();

    // Set font for database structure views
    QFont structure_font = ui->dbTreeWidget->font();
    structure_font.setPointSize(Settings::getValue("db", "fontsize").toInt());
    ui->dbTreeWidget->setFont(structure_font);
    ui->treeSchemaDock->setFont(structure_font);

    // Load extensions
    db.loadExtensionsFromSettings();

    // Refresh view
    emit db.structureUpdated();
    refreshTableBrowsers();

    // Hide or show the remote dock as needed
    bool showRemoteActions = Settings::getValue("remote", "active").toBool();
    ui->viewMenu->actions().at(4)->setVisible(showRemoteActions);
    if(!showRemoteActions)
        ui->dockRemote->setHidden(true);

    // Reload remote dock settings
    remoteDock->reloadSettings();

    sqlb::setIdentifierQuoting(static_cast<sqlb::escapeQuoting>(Settings::getValue("editor", "identifier_quotes").toInt()));

    ui->tabSqlAreas->setTabsClosable(Settings::getValue("editor", "close_button_on_tabs").toBool());
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

void MainWindow::openLinkWiki() const
{
    QDesktopServices::openUrl(QUrl("https://github.com/sqlitebrowser/sqlitebrowser/wiki"));
}

// 'Help | Bug Report...' link will set an appropriate body, add the system information and set the label 'bug' automatically to the issue
void MainWindow::openLinkBugReport() const
{
    const QString version = Application::versionString();
    const QString os = QSysInfo::prettyProductName();
    const QString kernelType = QSysInfo::kernelType();
    const QString kernelVersion = QSysInfo::kernelVersion();
    const QString arch = QSysInfo::currentCpuArchitecture();
    const QString built_for = QSysInfo::buildAbi();

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
              "> DB4S v%1 [built for %2] on %3 (%4/%5) [%6]\n"
              "> using %7\n"
              "> and Qt %8")
            .arg(version, built_for, os, kernelType, kernelVersion, arch, sqlite_version, QT_VERSION_STR);

    QUrlQuery query;
    query.addQueryItem("labels", "bug");
    query.addQueryItem("body", body);

    QUrl url("https://github.com/sqlitebrowser/sqlitebrowser/issues/new");
    url.setQuery(query);
    QDesktopServices::openUrl(url);
}

// 'Help | Feature Request...' link will set an appropriate body and add the label 'enhancement' automatically to the issue
void MainWindow::openLinkFeatureRequest() const
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

void MainWindow::openLinkSqlCipherFaq() const
{
    QDesktopServices::openUrl(QUrl("https://discuss.zetetic.net/c/sqlcipher/sqlcipher-faq"));
}

void MainWindow::openLinkWebsite() const
{
    QDesktopServices::openUrl(QUrl("https://sqlitebrowser.org"));
}

void MainWindow::openLinkDonatePatreon() const
{
    QDesktopServices::openUrl(QUrl("https://www.patreon.com/bePatron?u=11578749"));
}

static void loadCondFormatMap(BrowseDataTableSettings::CondFormatMap& condFormats, QXmlStreamReader& xml, const QString& encoding)
{
    const QStringRef name = xml.name();

    while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != name) {
        if (xml.name() == "column") {
            size_t index = xml.attributes().value("index").toUInt();
            while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "column") {
                if(xml.name() == "format") {
                    QFont font;
                    if (xml.attributes().hasAttribute("font"))
                        font.fromString(xml.attributes().value("font").toString());
                    else
                        Settings::getValue("databrowser", "font").toString();

                    CondFormat::Alignment align;
                    if (xml.attributes().hasAttribute("align"))
                        align = static_cast<CondFormat::Alignment>(xml.attributes().value("align").toInt());
                    else
                        align = CondFormat::AlignLeft;

                    condFormats[index].emplace_back(xml.attributes().value("condition").toString(),
                                                    QColor(xml.attributes().value("foreground").toString()),
                                                    QColor(xml.attributes().value("background").toString()),
                                                    font, align, encoding);
                    xml.skipCurrentElement();
                }
            }
        }
    }
}

static void loadBrowseDataTableSettings(BrowseDataTableSettings& settings, sqlb::TablePtr obj, QXmlStreamReader& xml)
{
    settings.showRowid = xml.attributes().value("show_row_id").toInt();
    settings.encoding = xml.attributes().value("encoding").toString();
    settings.plotXAxis = xml.attributes().value("plot_x_axis").toString();
    settings.unlockViewPk = xml.attributes().value("unlock_view_pk").toString();
    if(xml.attributes().hasAttribute("freeze_columns"))
        settings.frozenColumns = xml.attributes().value("freeze_columns").toUInt();

    while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "table") {
        if(xml.name() == "sort")
        {
            while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "sort")
            {
                if(xml.name() == "column")
                {
                    int index = xml.attributes().value("index").toInt();
                    int mode = xml.attributes().value("mode").toInt();
                    if(static_cast<size_t>(index) < obj->fields.size())
                        settings.sortColumns.emplace_back(obj->fields.at(static_cast<size_t>(index)).name(), mode == Qt::AscendingOrder ? sqlb::OrderBy::Ascending : sqlb::OrderBy::Descending);
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
                    size_t index = xml.attributes().value("index").toUInt();
                    QString value = xml.attributes().value("value").toString();
                    if(!value.isEmpty())
                        settings.filterValues[obj->fields.at(index).name()] = value;
                    xml.skipCurrentElement();
                }
            }
        } else if(xml.name() == "conditional_formats") {
            loadCondFormatMap(settings.condFormats, xml, settings.encoding);
        } else if(xml.name() == "row_id_formats") {
            loadCondFormatMap(settings.rowIdFormats, xml, settings.encoding);
        } else if(xml.name() == "display_formats") {
            while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "display_formats") {
                if (xml.name() == "column") {
                    size_t index = xml.attributes().value("index").toUInt();
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
                QString y1AxisName;
                QString y2AxisName;
                PlotDock::PlotSettings y1AxisSettings;
                PlotDock::PlotSettings y2AxisSettings;
                if (xml.name() == "y_axis") {
                    y1AxisName = xml.attributes().value("name").toString();
                    y1AxisSettings.lineStyle = xml.attributes().value("line_style").toInt();
                    y1AxisSettings.pointShape = xml.attributes().value("point_shape").toInt();
                    y1AxisSettings.colour = QColor (xml.attributes().value("colour").toString());
                    y1AxisSettings.active = xml.attributes().value("active").toInt();
                    xml.skipCurrentElement();
                }
                settings.plotYAxes[0][y1AxisName] = y1AxisSettings;
                if (xml.name() == "y2_axis") {
                  y2AxisName = xml.attributes().value("name").toString();
                  y2AxisSettings.lineStyle = xml.attributes().value("line_style").toInt();
                  y2AxisSettings.pointShape = xml.attributes().value("point_shape").toInt();
                  y2AxisSettings.colour = QColor (xml.attributes().value("colour").toString());
                  y2AxisSettings.active = xml.attributes().value("active").toInt();
                  xml.skipCurrentElement();
                }
                settings.plotYAxes[1][y2AxisName] = y2AxisSettings;
            }
        } else if(xml.name() == "global_filter") {
            while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "global_filter")
            {
                if(xml.name() == "filter")
                {
                    QString value = xml.attributes().value("value").toString();
                    settings.globalFilters.push_back(value);
                    xml.skipCurrentElement();
                }
            }
        }
    }
}
bool MainWindow::loadProject(QString filename, bool readOnly)
{
    // Show the open file dialog when no filename was passed as parameter
    if(filename.isEmpty())
    {
        filename = FileDialog::getOpenFileName(
                       OpenProjectFile,
                       this,
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

        // We are going to open a new project, so close the possible current one before opening another.
        // Stop the opening process here if the user pressed the cancel button in there.
        if(!closeFiles())
            return false;

        addToRecentFilesMenu(filename, readOnly);
        currentProjectFilename = filename;

        while(!xml.atEnd() && !xml.hasError())
        {
            // Read next token
            QXmlStreamReader::TokenType token = xml.readNext();

            // Handle element start
            if(token == QXmlStreamReader::StartElement)
            {
                if(xml.name() == "db")
                {
                    // Read only?
                    if(xml.attributes().hasAttribute("readonly") && xml.attributes().value("readonly").toInt())
                        readOnly = true;

                    // DB file
                    QString dbfilename = xml.attributes().value("path").toString();
                    if(!QFile::exists(dbfilename)) {
                        dbfilename = QFileInfo(filename).absolutePath() + QDir::separator() + dbfilename;
                        // New DB filename is pending to be saved
                        isProjectModified = true;
                    }
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
                        if(xml.name() == "main_tabs") {
                            // Currently open tabs
                            restoreOpenTabs(xml.attributes().value("open").toString());
                            // Currently selected open tab
                            ui->mainTab->setCurrentIndex(xml.attributes().value("current").toString().toInt());
                            xml.skipCurrentElement();
                        } else if(xml.name() == "current_tab") {
                            // Currently selected tab (3.11 or older format, first restore default open tabs)
                            restoreOpenTabs(defaultOpenTabs);
                            ui->mainTab->setCurrentIndex(xml.attributes().value("id").toString().toInt());
                            xml.skipCurrentElement();
                        }
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
                    // Close all open tabs first. We call delete here to avoid the
                    // closed() signal being emitted which would open a new dock.
                    for(auto d : allTableBrowserDocks())
                        delete d;

                    // Browse Data tab settings
                    while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "tab_browse")
                    {
                        if(xml.name() == "current_table")
                        {
                            // TODO This attribute was only created until version 3.12.0 which means we can remove support for this
                            // in the future.

                            // Currently selected table
                            QString table_name = xml.attributes().value("name").toString();
                            sqlb::ObjectIdentifier currentTable;
                            if(!currentTable.fromSerialised(table_name.toStdString()))
                            {
                                // This is an old project file format which doesn't yet contain serialised table identifiers. This means
                                // we have to try our best to unserialise this one manually. The only problem is when the name of an
                                // attached database or of a table contains a dot character. In that case the name becomes ambigious and
                                // we just try to split it at the first dot. I don't think it affects many (if any) project files. But if
                                // it turn out to be wrong, we can always add a loop here which checks for any possible combination of schema
                                // and table name whether an object with that combination exists.
                                // TODO: Delete this code in the future when we don't expect there to be any project files in the old format anymore.
                                if(table_name.contains('.'))
                                {
                                    currentTable.setSchema(table_name.left(table_name.indexOf('.')).toStdString());
                                    currentTable.setName(table_name.mid(table_name.indexOf('.')+1).toStdString());
                                } else {
                                    currentTable.setName(table_name.toStdString());
                                }
                            }

                            if (!currentTable.isEmpty())
                                newTableBrowserTab(currentTable);

                            xml.skipCurrentElement();
                        } else if(xml.name() == "table") {
                            // New browser tab
                            sqlb::ObjectIdentifier table;
                            table.fromSerialised(xml.attributes().value("table").toString().toStdString());
                            QDockWidget* dock = newTableBrowserTab(table);
                            dock->setWindowTitle(xml.attributes().value("title").toString());

                            unsigned int dock_id = xml.attributes().value("dock_id").toUInt();
                            if(dock_id)
                                dock->setObjectName("dockBrowse" + QString::number(dock_id));

                            if(xml.attributes().value("custom_title").toString() == "1")
                                dock->setProperty("custom_title", true);

                            xml.skipCurrentElement();
                        } else if(xml.name() == "dock_state") {
                            // Dock state
                            ui->tabBrowsers->restoreState(QByteArray::fromHex(xml.attributes().value("state").toString().toUtf8()));

                            xml.skipCurrentElement();
                        } else if(xml.name() == "default_encoding") {
                            // Default text encoding
                            TableBrowser::setDefaultEncoding(xml.attributes().value("codec").toString());
                            xml.skipCurrentElement();
                        } else if(xml.name() == "browsetable_info") {
                            // This tag is only found in old project files. In newer versions (>= 3.11) it is replaced by a new implementation.
                            // 3.12 is the last version to support loading this file format, so just show a warning here.
                            if(!Settings::getValue("idontcare", "projectBrowseTable").toBool())
                            {
                                QMessageBox msgBox;
                                QPushButton* idontcarebutton = msgBox.addButton(tr("Don't show again"), QMessageBox::ActionRole);
                                msgBox.addButton(QMessageBox::Ok);
                                msgBox.setTextFormat(Qt::RichText);
                                msgBox.setWindowTitle(qApp->applicationName());
                                msgBox.setText(tr("This project file is using an old file format because it was created using DB Browser for SQLite "
                                                  "version 3.10 or lower. Loading this file format is no longer fully supported. If you want to load "
                                                  "it completely, please use DB Browser for SQLite version 3.12 to convert it to the new file format."));
                                msgBox.exec();
                                if(msgBox.clickedButton() == idontcarebutton)
                                    Settings::setValue("idontcare", "projectBrowseTable", false);
                            }

                            xml.skipCurrentElement();
                        } else if(xml.name() == "browse_table_settings") {

                            while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != "browse_table_settings") {
                                if (xml.name() == "table") {

                                    sqlb::ObjectIdentifier tableIdentifier =
                                        sqlb::ObjectIdentifier (xml.attributes().value("schema").toString().toStdString(),
                                                                xml.attributes().value("name").toString().toStdString());
                                    BrowseDataTableSettings settings;
                                    loadBrowseDataTableSettings(settings, db.getTableByName(tableIdentifier), xml);
                                    TableBrowser::setSettings(tableIdentifier, settings);
                                }
                            }
                        } else {
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
                            int index = openSqlTab();

                            // if it has the filename attribute, it's a linked file, otherwise it's
                            // a free SQL buffer saved in the project.
                            if(xml.attributes().hasAttribute("filename")) {
                                openSqlFile(index, xml.attributes().value("filename").toString());
                                xml.skipCurrentElement();
                            } else {
                                ui->tabSqlAreas->setTabText(index, xml.attributes().value("name").toString());
                                SqlTextEdit* sqlEditor = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(index))->getEditor();
                                sqlEditor->setText(xml.readElementText());
                                sqlEditor->setModified(false);
                            }
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

        if(ui->mainTab->currentWidget() == ui->browser) {
            refreshTableBrowsers();     // Refresh view
        }

        isProjectModified = false;

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

static void saveCondFormatMap(const QString& elementName, const BrowseDataTableSettings::CondFormatMap& condFormats, QXmlStreamWriter& xml)
{
    xml.writeStartElement(elementName);
    for(auto iter=condFormats.cbegin(); iter!=condFormats.cend(); ++iter) {
        xml.writeStartElement("column");
        xml.writeAttribute("index", QString::number(iter->first));
        for(const auto& format : iter->second) {
            xml.writeStartElement("format");
            xml.writeAttribute("condition", format.filter());
            xml.writeAttribute("background", format.backgroundColor().name());
            xml.writeAttribute("foreground", format.foregroundColor().name());
            xml.writeAttribute("font", format.font().toString());
            xml.writeAttribute("align", QString().setNum(format.alignment()));
            xml.writeEndElement();
        }
        xml.writeEndElement();
    }
    xml.writeEndElement();
}

static void saveBrowseDataTableSettings(const BrowseDataTableSettings& object, sqlb::TablePtr obj, QXmlStreamWriter& xml)
{
    xml.writeAttribute("show_row_id", QString::number(object.showRowid));
    xml.writeAttribute("encoding", object.encoding);
    xml.writeAttribute("plot_x_axis", object.plotXAxis);
    xml.writeAttribute("unlock_view_pk", object.unlockViewPk);
    xml.writeAttribute("freeze_columns", QString::number(object.frozenColumns));

    xml.writeStartElement("sort");
    for(const auto& column : object.sortColumns)
    {
        xml.writeStartElement("column");
        xml.writeAttribute("index", QString::number(sqlb::getFieldNumber(obj, column.expr)));
        xml.writeAttribute("mode", QString::number(column.direction));
        xml.writeEndElement();
    }
    xml.writeEndElement();

    xml.writeStartElement("column_widths");
    for(auto iter=object.columnWidths.cbegin(); iter!=object.columnWidths.cend(); ++iter) {
        xml.writeStartElement("column");
        xml.writeAttribute("index", QString::number(iter->first));
        xml.writeAttribute("value", QString::number(iter->second));
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeStartElement("filter_values");
    for(auto iter=object.filterValues.cbegin(); iter!=object.filterValues.cend(); ++iter) {
        xml.writeStartElement("column");
        xml.writeAttribute("index", QString::number(sqlb::getFieldNumber(obj, iter->first)));
        xml.writeAttribute("value", iter->second);
        xml.writeEndElement();
    }
    xml.writeEndElement();
    saveCondFormatMap("conditional_formats", object.condFormats, xml);
    saveCondFormatMap("row_id_formats", object.rowIdFormats, xml);
    xml.writeStartElement("display_formats");
    for(auto iter=object.displayFormats.cbegin(); iter!=object.displayFormats.cend(); ++iter) {
        xml.writeStartElement("column");
        xml.writeAttribute("index", QString::number(iter->first));
        xml.writeAttribute("value", iter->second);
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeStartElement("hidden_columns");
    for(auto iter=object.hiddenColumns.cbegin(); iter!=object.hiddenColumns.cend(); ++iter) {
        xml.writeStartElement("column");
        xml.writeAttribute("index", QString::number(iter->first));
        xml.writeAttribute("value", QString::number(iter->second));
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeStartElement("plot_y_axes");
    for(auto iter=object.plotYAxes[0].cbegin(); iter!=object.plotYAxes[0].cend(); ++iter) {
        PlotDock::PlotSettings plotSettings = iter->second;
        xml.writeStartElement("y_axis");
        xml.writeAttribute("name", iter->first);
        xml.writeAttribute("line_style", QString::number(plotSettings.lineStyle));
        xml.writeAttribute("point_shape", QString::number(plotSettings.pointShape));
        xml.writeAttribute("colour", plotSettings.colour.name());
        xml.writeAttribute("active", QString::number(plotSettings.active));
        xml.writeEndElement();
    }
    for(auto iter=object.plotYAxes[1].cbegin(); iter!=object.plotYAxes[1].cend(); ++iter) {
      PlotDock::PlotSettings plotSettings = iter->second;
      xml.writeStartElement("y2_axis");
      xml.writeAttribute("name", iter->first);
      xml.writeAttribute("line_style", QString::number(plotSettings.lineStyle));
      xml.writeAttribute("point_shape", QString::number(plotSettings.pointShape));
      xml.writeAttribute("colour", plotSettings.colour.name());
      xml.writeAttribute("active", QString::number(plotSettings.active));
      xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeStartElement("global_filter");
    for(const auto& v : object.globalFilters)
    {
        xml.writeStartElement("filter");
        xml.writeAttribute("value", v);
        xml.writeEndElement();
    }
    xml.writeEndElement();
}

void MainWindow::saveProject(const QString& currentFilename)
{
    QString filename;
    if(currentFilename.isEmpty()) {
        QString basePathName = db.currentFile();
        // Remove database suffix
        basePathName.chop(QFileInfo(basePathName).suffix().size()+1);
        filename = FileDialog::getSaveFileName(
                           CreateProjectFile,
                           this,
                           tr("Choose a filename to save under"),
                           FILE_FILTER_SQLPRJ,
                           basePathName);
    } else
        filename = currentFilename;

    if(!filename.isEmpty())
    {
        // Make sure the file has got a .sqbpro ending
        if(!filename.endsWith(FILE_EXT_SQLPRJ_DEFAULT, Qt::CaseInsensitive))
            filename.append(FILE_EXT_SQLPRJ_DEFAULT);

        QFile file(filename);
        bool opened = file.open(QFile::WriteOnly | QFile::Text);
        if(!opened) {
            QMessageBox::warning(this, qApp->applicationName(),
                               tr("Could not open project file for writing.\nReason: %1").arg(file.errorString()));
            currentProjectFilename.clear();
            return;
        }
        currentProjectFilename = filename;
        QApplication::setOverrideCursor(Qt::WaitCursor);

        QXmlStreamWriter xml(&file);
        xml.writeStartDocument();
        xml.writeStartElement("sqlb_project");

        // Database file name
        xml.writeStartElement("db");
        QFileInfo dbFileInfo (db.currentFile());
        QFileInfo projectFileInfo (filename);

        // Store a relative filename if the path to project and to DB is the same.
        if(dbFileInfo.absolutePath() == projectFileInfo.absolutePath())
            xml.writeAttribute("path", dbFileInfo.fileName());
        else
            xml.writeAttribute("path", db.currentFile());

        xml.writeAttribute("readonly", QString::number(db.readOnly()));
        xml.writeAttribute("foreign_keys", db.getPragma("foreign_keys"));
        xml.writeAttribute("case_sensitive_like", db.getPragma("case_sensitive_like"));
        xml.writeAttribute("temp_store", db.getPragma("temp_store"));
        xml.writeAttribute("wal_autocheckpoint", db.getPragma("wal_autocheckpoint"));
        xml.writeAttribute("synchronous", db.getPragma("synchronous"));
        xml.writeEndElement();

        // Attached databases
        xml.writeStartElement("attached");
        db.executeSQL("PRAGMA database_list;", false, true, [&xml](int, std::vector<QByteArray> values, std::vector<QByteArray>) -> bool {
            auto schema = values.at(1);
            if(schema != "main" && schema != "temp")
            {
                auto path = values.at(2);
                xml.writeStartElement("db");
                xml.writeAttribute("schema", schema);
                xml.writeAttribute("path", path);
                xml.writeEndElement();
            }
            return false;
        });
        xml.writeEndElement();

        // Window settings
        xml.writeStartElement("window");
        xml.writeStartElement("main_tabs");   // Currently open tabs
        xml.writeAttribute("open", saveOpenTabs());
        xml.writeAttribute("current", QString::number(ui->mainTab->currentIndex()));
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

        for(const auto d : allTableBrowserDocks())
        {
            xml.writeStartElement("table");
            xml.writeAttribute("title", d->windowTitle());
            xml.writeAttribute("custom_title", d->property("custom_title").toBool() ? "1" : "0");
            xml.writeAttribute("dock_id", d->objectName().mid(10)); // 10 is the length of "dockBrowse"
            xml.writeAttribute("table", QString::fromStdString(d->tableBrowser()->currentlyBrowsedTableName().toSerialised()));
            xml.writeEndElement();
        }

        xml.writeStartElement("dock_state");
        xml.writeAttribute("state", ui->tabBrowsers->saveState().toHex());
        xml.writeEndElement();

        xml.writeStartElement("default_encoding");  // Default encoding for text stored in tables
        xml.writeAttribute("codec", TableBrowser::defaultEncoding());
        xml.writeEndElement();

        xml.writeStartElement("browse_table_settings");
        const auto settings = TableBrowser::allSettings();
        for(auto tableIt=settings.cbegin(); tableIt!=settings.cend(); ++tableIt) {

            xml.writeStartElement("table");
            xml.writeAttribute("schema", QString::fromStdString(tableIt->first.schema()));
            xml.writeAttribute("name", QString::fromStdString(tableIt->first.name()));

            auto obj = db.getTableByName(tableIt->first);
            saveBrowseDataTableSettings(tableIt->second, obj, xml);
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
            SqlExecutionArea* sqlArea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i));
            QString sqlFilename = sqlArea->fileName();
            xml.writeStartElement("sql");
            xml.writeAttribute("name", ui->tabSqlAreas->tabText(i));
            if(sqlFilename.isEmpty()) {
                xml.writeCharacters(sqlArea->getSql());
                sqlArea->getEditor()->setModified(false);
            } else {
                xml.writeAttribute("filename", sqlFilename);
                // Backwards compatibility, so older versions do not break.
                xml.writeCharacters(tr("-- Reference to file \"%1\" (not supported by this version) --").arg(sqlFilename));
            }
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
        setCurrentFile(db.currentFile());
        isProjectModified = false;
        showStatusMessage5s(tr("Project saved to file '%1'").arg(currentProjectFilename));
        QApplication::restoreOverrideCursor();
    }
}

void MainWindow::saveProject()
{
    saveProject(currentProjectFilename);
}

void MainWindow::saveProjectAs()
{
    saveProject(QString());
}

void MainWindow::fileAttach(const QString& fileName)
{
    QString file;
    if (fileName.isEmpty()) {

        // Get file name of database to attach
        file = FileDialog::getOpenFileName(
            OpenDatabaseFile,
            this,
            tr("Choose a database file"),
            FileDialog::getSqlDatabaseFileFilter());
    } else
        file = fileName;

    if(!QFile::exists(file))
        return;

    // Attach it
    db.attach(file);
    isProjectModified = true;
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

        const QString temporalFile = db.currentFile() + ".enctemp";

        // Create the new file first or it won't work
        if(ok)
        {
            QFile file(temporalFile);
            file.open(QFile::WriteOnly);
            file.close();
        }

        CipherSettings cipherSettings = cipherDialog.getCipherSettings();

        // Attach a new database using the new settings
        qApp->processEvents();
        if(ok)
            ok = db.executeSQL("ATTACH DATABASE '" + temporalFile.toStdString() + "' AS sqlitebrowser_edit_encryption KEY " + cipherSettings.getPassword() + ";",
                               false, false);
        qApp->processEvents();
        if(ok)
            ok = db.executeSQL("PRAGMA sqlitebrowser_edit_encryption.cipher_page_size = " + std::to_string(cipherSettings.getPageSize()), false, false);
        if(ok)
            ok = db.executeSQL("PRAGMA sqlitebrowser_edit_encryption.cipher_hmac_algorithm = " + cipherSettings.getHmacAlgorithm(), false, false);
        if(ok)
            ok = db.executeSQL("PRAGMA sqlitebrowser_edit_encryption.cipher_kdf_algorithm = " + cipherSettings.getKdfAlgorithm(), false, false);
        if(ok)
            ok = db.executeSQL("PRAGMA sqlitebrowser_edit_encryption.kdf_iter = " + std::to_string(cipherSettings.getKdfIterations()), false, false);
        if (ok)
            ok = db.executeSQL("PRAGMA sqlitebrowser_edit_encryption.cipher_plaintext_header_size = " + std::to_string(cipherSettings.getPlaintextHeaderSize()), false, false);

        // Export the current database to the new one
        qApp->processEvents();
        if(ok)
            ok = db.executeSQL("SELECT sqlcipher_export('sqlitebrowser_edit_encryption');", false, false);

        // Set user version of the new database
        qApp->processEvents();
        if (ok)
            ok = db.executeSQL("PRAGMA sqlitebrowser_edit_encryption.user_version = " + std::to_string(db.getPragma("user_version").toInt()) + ";", false, false);

        // We need to detach the database before proceeding
        qApp->processEvents();
        if (ok)
            ok = db.executeSQL("DETACH sqlitebrowser_edit_encryption;", false, false);

        // Check for errors
        qApp->processEvents();
        if(ok)
        {
            // No errors: Then close the current database, switch names, open the new one and if that succeeded delete the old one

            const QString currentFile = db.currentFile();
            fileClose();
            QFile::rename(currentFile, temporalFile + "old");
            QFile::rename(temporalFile, currentFile);
            if(fileOpen(currentFile))
                QFile::remove(temporalFile + "old");
        } else {
            QMessageBox::warning(this, qApp->applicationName(), db.lastError());
        }
    }
#endif
}

void MainWindow::switchToBrowseDataTab(sqlb::ObjectIdentifier tableToBrowse)
{
    // If no table name was provided get the currently selected table fromt he structure tab
    if(tableToBrowse.isEmpty())
    {
        // Cancel here if there is no selection
        if(!dbSelected->hasSelection())
            return;

        tableToBrowse.setSchema(dbSelected->schema().toStdString());
        tableToBrowse.setName(dbSelected->name().toStdString());
    }

    TableBrowserDock* d = newTableBrowserTab(tableToBrowse);

    if (ui->mainTab->indexOf(ui->browser) == -1)
        ui->mainTab->addTab(ui->browser, ui->browser->accessibleName());

    // Don't emit a main tab changed signal here because all it would do is refreshing the just loaded table view
    ui->mainTab->blockSignals(true);
    ui->mainTab->setCurrentWidget(ui->browser);
    ui->mainTab->blockSignals(false);

    // Bring new tab to foreground
    Application::processEvents();   // For some reason this is required for raise() to work here.
    d->raise();
}

void MainWindow::fileDetachDbTree()
{
    fileDetachTreeViewSelected(ui->dbTreeWidget);
}

void MainWindow::fileDetachTreeSchemaDock()
{
    fileDetachTreeViewSelected(ui->treeSchemaDock);
}

void MainWindow::fileDetachTreeViewSelected(QTreeView* treeView)
{
    // Cancel here if there is no selection
    if (!treeView || !treeView->selectionModel()->hasSelection())
    {
        return;
    }

    // get the currently selected attached database from treeView parameter
    std::string attached_as = treeView->model()->data(treeView->currentIndex().sibling(treeView->currentIndex().row(), DbStructureModel::ColumnSchema), Qt::EditRole).toString().toStdString();
    if (db.detach(attached_as))
    {
        isProjectModified = true;
    }
}

void MainWindow::copyCurrentCreateStatement()
{
    // Cancel if no field is currently selected
    if(!dbSelected->hasSelection())
        return;

    // Get the CREATE statement from the SQL column
    QString stmt = dbSelected->sql();

    // Copy the statement to the global application clipboard
    QApplication::clipboard()->setText(stmt);
}

void MainWindow::fileOpenReadOnly()
{
    // Redirect to 'standard' fileOpen(), with the read only flag set
    fileOpen(QString(), false, true);
}

void MainWindow::requestCollation(const QString& name, int eTextRep)
{
    // Show message box
    if(!Settings::getValue("db", "dont_ask_collation").toBool())
    {
        QMessageBox msgbox;
        QPushButton* button_dont_ask_again = msgbox.addButton(tr("Yes. Don't ask again"), QMessageBox::ActionRole);
        msgbox.addButton(QMessageBox::Yes);
        msgbox.addButton(QMessageBox::No);
        msgbox.setTextFormat(Qt::RichText);
        msgbox.setWindowTitle(tr("Collation needed! Proceed?"));
        msgbox.setText(tr("A table in this database requires a special collation function '%1' "
                          "that this application can't provide without further knowledge.\n"
                          "If you choose to proceed, be aware bad things can happen to your database.\n"
                          "Create a backup!").arg(name));
        int reply = msgbox.exec();

        // Remember Don't ask again setting and proceed if either that button or the Yes button was clicked.
        // Cancel here if the No button was clicked
        if(msgbox.clickedButton() == button_dont_ask_again)
        {
            Settings::setValue("db", "dont_ask_collation", false);
        } else if(reply == QMessageBox::No) {
            return;
        }
    }

    // Add collation
    auto pDb = db.get(tr("creating collation"));
    sqlite3_create_collation(pDb.get(), name.toUtf8(), eTextRep, nullptr, collCompare);
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

void MainWindow::toggleSqlBlockComment()
{
    // The slot for the shortcut must discover which sqltexedit widget has the focus
    SqlExecutionArea* sqlWidget = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());

    if (sqlWidget)
        sqlWidget->getEditor()->toggleBlockComment();
}

void MainWindow::openSqlPrintDialog()
{
    // The slot for the shortcut must discover which sqltexedit widget has the focus and then open its dialog.
    SqlExecutionArea* sqlWidget = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());

    if (sqlWidget)
        sqlWidget->getEditor()->openPrintDialog();
}

void MainWindow::saveAsView(const std::string& query)
{
    // Let the user select a name for the new view and make sure it doesn't already exist
    QString name;
    while(true)
    {
        name = QInputDialog::getText(this, qApp->applicationName(), tr("Please specify the view name")).trimmed();
        if(name.isNull())
            return;
        if(db.getTableByName(sqlb::ObjectIdentifier("main", name.toStdString())))
            QMessageBox::warning(this, qApp->applicationName(), tr("There is already an object with that name. Please choose a different name."));
        else
            break;
    }

    // Create the view
    if(db.executeSQL("CREATE VIEW " + sqlb::escapeIdentifier(name.toStdString()) + " AS " + query + ";"))
        QMessageBox::information(this, qApp->applicationName(), tr("View successfully created."));
    else
        QMessageBox::warning(this, qApp->applicationName(), tr("Error creating view: %1").arg(db.lastError()));
}

void MainWindow::runSqlNewTab(const QString& query, const QString& title, const QString& helpUrl, const bool autoRun)
{
    QString message;

    if(autoRun)
        message = tr("This action will open a new SQL tab for running:");
    else
        message = tr("This action will open a new SQL tab with the following statements for you to edit and run:");

    message += QString("<blockquote><tt>%1</tt></blockquote>").arg(query.size() > 1000 ? query.left(1000) + "\n[...]" : query);
    if(!helpUrl.isEmpty())
            message += tr("Press Help for opening the corresponding SQLite reference page.");

    QString windowTitle = title;
    windowTitle.remove('&');
    auto result = QMessageBox::information(this,
                                           windowTitle,
                                           message,
                                           QMessageBox::Ok | QMessageBox::Default,
                                           QMessageBox::Cancel | QMessageBox::Escape,
                                           helpUrl.isEmpty() ? 0 : QMessageBox::Help);

    switch (result)
    {
    case QMessageBox::Ok: {
        if (ui->mainTab->indexOf(ui->query) == -1)
            ui->mainTab->addTab(ui->query, ui->query->accessibleName());
        ui->mainTab->setCurrentWidget(ui->query);
        int index = openSqlTab();
        ui->tabSqlAreas->setTabText(index, title);
        qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(index))->getEditor()->setText(query);
        if(autoRun)
            executeQuery();
        break;
    }
    case QMessageBox::Help: {
        QDesktopServices::openUrl(QUrl(helpUrl));
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
        QString strData = model->data(headerIndex).toString().toHtmlEscaped();
        out << QString("<h1>%1</h1>").arg(strData);

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
                        QString header_data = model->data(cellIndex).toString().toHtmlEscaped();
                        if (column2 != DbStructureModel::ColumnSQL)
                            out << QString("<td><h2>%1</h2></td>").arg((!header_data.isEmpty()) ? header_data : QString("&nbsp;"));
                        else
                            out << QString("<td><pre>%1</pre></td>").arg((!header_data.isEmpty()) ? header_data : QString("&nbsp;"));
                    }
                }
                out << "</tr>";

                // One row for each object's fields
                for (int rowChild2 = 0; rowChild2 < model->rowCount(objectIndex); rowChild2++) {
                    out << "<tr>";
                    for (int column2 = 0; column2 < columnCount; column2++) {
                        if (!treeView->isColumnHidden(column2)) {
                            QModelIndex fieldIndex = model->index(rowChild2, column2, objectIndex);
                            QString field_data = model->data(fieldIndex).toString().toHtmlEscaped();
                            out << QString("<td>%1</td>").arg((!field_data.isEmpty()) ? field_data : QString("&nbsp;"));
                        }
                    }
                    out << "</tr>";
                }
            }
        }
        out << "</table>";
    }
    out << "</body></html>";

    QPrinter printer;
    printer.setDocName(treeView->windowTitle());

    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer);
    connect(dialog, &QPrintPreviewDialog::paintRequested, this, [strStream](QPrinter *previewPrinter) {
        QTextDocument document;
        document.setHtml(strStream);
        document.print(previewPrinter);
    });

    dialog->exec();
    delete dialog;

}

void MainWindow::updateDatabaseBusyStatus(bool busy, const QString& user)
{
    statusBusyLabel->setText(tr("Busy (%1)").arg(user));
    statusBusyLabel->setVisible(busy);
    statusStopButton->setVisible(busy);
}

void MainWindow::closeTab(int index)
{
    ui->mainTab->removeTab(index);
}

void MainWindow::toggleTabVisible(QWidget* tabWidget, bool show)
{
    if (show)
        ui->mainTab->addTab(tabWidget, tabWidget->accessibleName());
    else
        ui->mainTab->removeTab(ui->mainTab->indexOf(tabWidget));
}

void MainWindow::restoreOpenTabs(QString tabs)
{
    // Split the tab list, skipping the empty parts so the empty string turns to an empty list
    // and not a list of one empty string.
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    const QStringList tabList = tabs.split(' ', QString::SkipEmptyParts);
#else
    const QStringList tabList = tabs.split(' ', Qt::SkipEmptyParts);
#endif

    // Clear the tabs and then add them in the order specified by the setting.
    // Use the accessibleName attribute for restoring the tab label.
    if (!tabList.isEmpty()) {
        // Avoid flickering while clearing and adding tabs.
        ui->mainTab->setUpdatesEnabled(false);
        ui->mainTab->clear();
        for (const auto& objectName : tabList) {
            for (QWidget* widget : {ui->structure, ui->browser, ui->pragmas, ui->query})
                if (widget->objectName() == objectName) {
                    ui->mainTab->addTab(widget, widget->accessibleName());
                    break;
                }
        }
        ui->mainTab->setUpdatesEnabled(true);
        // Force the update of the View menu toggable entries
        // (it doesn't seem to be a better way)
        emit ui->mainTab->tabCloseRequested(-1);
    }
}

QString MainWindow::saveOpenTabs()
{
    QString openTabs;
    for (int i=0; i < ui->mainTab->count(); i++)
        openTabs.append(ui->mainTab->widget(i)->objectName() + ' ');
    openTabs.chop(1);
    return openTabs;
}

void MainWindow::showStatusMessage5s(QString message)
{
    ui->statusbar->showMessage(message, 5000);
}

void MainWindow::saveAll()
{
    for(int i=0; i<ui->tabSqlAreas->count(); i++) {
        SqlExecutionArea* sqlExecArea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(i));
        if(sqlExecArea->getEditor()->isModified() && !sqlExecArea->fileName().isEmpty())
            saveSqlFile(i);
    }
    if(!currentProjectFilename.isEmpty())
        saveProject();
    fileSave();

}

void MainWindow::showContextMenuSqlTabBar(const QPoint& pos)
{
    // Don't show context menu if the mouse click was outside of all the tabs
    int tab = ui->tabSqlAreas->tabBar()->tabAt(pos);
    if(tab == -1)
        return;

    // Prepare all menu actions
    QAction* actionRename = new QAction(this);
    actionRename->setText(tr("Rename Tab"));
    connect(actionRename, &QAction::triggered, this, [this, tab]() {
        renameSqlTab(tab);
    });

    QAction* actionDuplicate = new QAction(this);
    actionDuplicate->setText(tr("Duplicate Tab"));
    connect(actionDuplicate, &QAction::triggered, this, [this, tab]() {
        QString tab_name = ui->tabSqlAreas->tabText(tab).remove("&").remove(QRegExp(" \\(\\d+\\)$"));
        QString new_tab_name;
        for(int i=1;;i++)
        {
            new_tab_name = tab_name + QString(" (%1)").arg(i);
            bool name_already_exists = false;
            for(int j=0;j<ui->tabSqlAreas->count();j++)
            {
                if(ui->tabSqlAreas->tabText(j).remove("&") == new_tab_name)
                {
                    name_already_exists = true;
                    break;
                }
            }

            if(!name_already_exists)
                break;
        }

        int new_tab = openSqlTab();
        ui->tabSqlAreas->setTabText(new_tab, new_tab_name);

        SqlExecutionArea* old_area = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(tab));
        SqlExecutionArea* new_area = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->widget(new_tab));
        new_area->setSql(old_area->getSql());
    });

    QAction* actionClose = new QAction(this);
    actionClose->setText(tr("Close Tab"));
    actionClose->setShortcut(tr("Ctrl+W"));
    connect(actionClose, &QAction::triggered, this, [this, tab]() {
        closeSqlTab(tab);
    });

    // Show menu
    QMenu* menuTabs = new QMenu(this);
    menuTabs->addAction(actionRename);
    menuTabs->addAction(actionDuplicate);
    menuTabs->addAction(actionClose);
    menuTabs->exec(ui->tabSqlAreas->mapToGlobal(pos));
}

void MainWindow::openUrlOrFile(const QString& urlString)
{
    QUrl url = QUrl::fromUserInput(urlString, QFileInfo(db.currentFile()).path(), QUrl::AssumeLocalFile);
    if(url.isValid()) {
        if(QDesktopServices::openUrl(url))
            showStatusMessage5s(tr("Opening '%1'...").arg(url.toDisplayString()));
        else
            showStatusMessage5s(tr("There was an error opening '%1'...").arg(url.toDisplayString()));

    } else
        showStatusMessage5s(tr("Value is not a valid URL or filename: %1").arg(url.errorString()));
}

void MainWindow::focusSqlEditor()
{
    SqlExecutionArea* sqlArea = qobject_cast<SqlExecutionArea*>(ui->tabSqlAreas->currentWidget());
    if(sqlArea)
        sqlArea->getEditor()->setFocus();
}

void MainWindow::moveDocksTo(Qt::DockWidgetArea area)
{
    addDockWidget(area, ui->dockEdit);
    addDockWidget(area, ui->dockLog);
    tabifyDockWidget(ui->dockLog, ui->dockPlot);
    tabifyDockWidget(ui->dockLog, ui->dockSchema);
    tabifyDockWidget(ui->dockLog, ui->dockRemote);
}

void MainWindow::clearRecentFiles()
{
    Settings::clearValue("General", "recentFileList");

    for(int i=0; i < MaxRecentFiles; ++i)
        recentFileActs[i]->setVisible(false);
}

sqlb::ObjectIdentifier MainWindow::currentlyBrowsedTableName() const
{
    if(currentTableBrowser)
        return currentTableBrowser->currentlyBrowsedTableName();
    else
        return sqlb::ObjectIdentifier{};
}

void MainWindow::tableBrowserTabClosed()
{
    // The closed() signal is emitted before the dock is destroyed. Since we
    // always want to have at least one dock open we need to check if this is
    // the last dock which is closed instead of if there is no dock remaining.
    if(allTableBrowserDocks().size() == 1)
    {
        newTableBrowserTab({}, /* forceHideTitlebar */ true);
    } else {
        // If only one dock will be left, make sure its titlebar is hidden,
        // unless it's floating, so it can be closed or restored.
        if(allTableBrowserDocks().size() == 2) {
            for(auto dock : allTableBrowserDocks())
                if(!dock->isFloating())
                    dock->setTitleBarWidget(new QWidget(dock));
        }
        // If the currently active tab is closed activate another tab
        if(currentTableBrowser && sender() == currentTableBrowser->parent())
        {
            auto all_docks = allTableBrowserDocks();
            all_docks.front()->activateWindow();
            changeTableBrowserTab(all_docks.front());
        }
    }
}

TableBrowserDock* MainWindow::newTableBrowserTab(const sqlb::ObjectIdentifier& tableToBrowse, bool forceHideTitleBar)
{
    // Prepare new dock
    TableBrowserDock* d = new TableBrowserDock(ui->tabBrowsers, this);
    connect(d, &TableBrowserDock::closed, this, &MainWindow::tableBrowserTabClosed);

    // Initialise widget
    d->tableBrowser()->setStructure(dbStructureModel, tableToBrowse);
    d->tableBrowser()->setEnabled(ui->fileCloseAction->isEnabled());

    // Connect signals and slots
    connect(d, &TableBrowserDock::newDockRequested, this, [this]() {
        newTableBrowserTab();
    });
    connect(d->tableBrowser(), &TableBrowser::projectModified, this, [this]() {
       isProjectModified = true;
    });
    connect(d->tableBrowser()->model(), &SqliteTableModel::dataChanged, this, &MainWindow::dataTableSelectionChanged);
    connect(d->tableBrowser(), &TableBrowser::dataAboutToBeEdited, this, &MainWindow::dataTableSelectionChanged);
    connect(d->tableBrowser(), &TableBrowser::selectionChanged, this, &MainWindow::dataTableSelectionChanged);
    connect(d->tableBrowser(), &TableBrowser::selectionChangedByDoubleClick, this, &MainWindow::doubleClickTable);
    connect(d->tableBrowser(), &TableBrowser::updatePlot, this, &MainWindow::attachPlot);
    connect(d->tableBrowser(), &TableBrowser::createView, this, &MainWindow::saveAsView);
    connect(d->tableBrowser(), &TableBrowser::requestFileOpen, this, [this](const QString& file) {
        fileOpen(file);
    });
    connect(d->tableBrowser(), &TableBrowser::statusMessageRequested, ui->statusbar, [this](const QString& message) {
        ui->statusbar->showMessage(message);
    });
    connect(d->tableBrowser(), &TableBrowser::foreignKeyClicked, this, [this](const sqlb::ObjectIdentifier& table, std::string column, const QByteArray& value) {
        TableBrowserDock* foreign_key_dock = newTableBrowserTab(table);
        foreign_key_dock->tableBrowser()->jumpToRow(table, column, value);
        Application::processEvents();   // For some reason this is required for raise() to work here.
        foreign_key_dock->raise();
    });
    connect(d->tableBrowser()->model(), &SqliteTableModel::finishedFetch, this, [this, d](){
        auto& settings = d->tableBrowser()->settings(d->tableBrowser()->currentlyBrowsedTableName());
        plotDock->updatePlot(d->tableBrowser()->model(), &settings, true, false);
    });
    connect(d->tableBrowser(), &TableBrowser::prepareForFilter, editDock, &EditDialog::promptSaveData);

    // Restore titlebar for any other widget
    for(auto dock : allTableBrowserDocks())
        dock->setTitleBarWidget(nullptr);

    // Hide titlebar if it is the only one dock
    if(allTableBrowserDocks().size() == 1 || forceHideTitleBar)
        d->setTitleBarWidget(new QWidget(d));

    // Set up dock and add it to the tab
    ui->tabBrowsers->addDockWidget(Qt::TopDockWidgetArea, d);
    if(allTableBrowserDocks().size() > 1)
#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
        ui->tabBrowsers->tabifyDockWidget(allTableBrowserDocks().front(), d);
#else
        ui->tabBrowsers->tabifyDockWidget(allTableBrowserDocks().constFirst(), d);
#endif

    // Set current model and browser
    d->activateWindow();
    changeTableBrowserTab(d);

    d->tableBrowser()->refresh();

    return d;
}

void MainWindow::changeTableBrowserTab(TableBrowserDock* dock)
{
    currentTableBrowser = dock->tableBrowser();

    if(dock->tableBrowser() && dock->tableBrowser()->model() != m_currentTabTableModel)
    {
        // Make sure only the title of the active dock is highlighted
        for(auto d : allTableBrowserDocks())
            d->setFocusStyle(false);
        dock->setFocusStyle(true);

        m_currentTabTableModel = dock->tableBrowser()->model();
        const auto current_table_name = dock->tableBrowser()->currentlyBrowsedTableName();
        if(!current_table_name.isEmpty())
            plotDock->updatePlot(m_currentTabTableModel, &dock->tableBrowser()->settings(current_table_name), true, false);
    }
}

QList<TableBrowserDock*> MainWindow::allTableBrowserDocks() const
{
    return ui->tabBrowsers->findChildren<TableBrowserDock*>();
}

void MainWindow::newRowCountsTab()
{
    QString sql;
    for(const auto& it : db.schemata["main"].tables)
    {
        sql += QString("SELECT %1 AS \"name\", '%2' AS \"type\", COUNT(*) AS \"rows\" FROM %3\nUNION ").arg(
                    QString::fromStdString(sqlb::escapeString(it.first)),
                    QString::fromStdString(it.second->isView() ? "view" : "table"),
                    QString::fromStdString(sqlb::escapeIdentifier(it.first)));
    }
    sql.chop(7);    // Remove the last "\nUNION " at the end

    runSqlNewTab(sql, ui->actionRowCounts->text());
}
