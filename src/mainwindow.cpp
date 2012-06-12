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
#include "SQLLogDock.h"
#include "sqlitesyntaxhighlighter.h"

void MainWindow::setupUi()
{
    if (this->objectName().isEmpty()){
        this->setObjectName(QString::fromUtf8("mainForm"));
    }

    logWin = new SQLLogDock(this);
    sqlLogAction = logWin->toggleViewAction();
    this->addDockWidget(Qt::BottomDockWidgetArea, logWin);

    //** New DB File
    fileNewAction = new QAction(this);
    fileNewAction->setObjectName(QString::fromUtf8("fileNewAction"));
    fileNewAction->setText("fileNewAction");
    fileNewAction->setIcon(QIcon(":/icons/db_new"));

    //** Open DB File
    fileOpenAction = new QAction(this);
    fileOpenAction->setObjectName(QString::fromUtf8("fileOpenAction"));
    fileOpenAction->setText("fileOpenAction");
    fileOpenAction->setIcon(QIcon(":/icons/db_open"));

    //** Save DB Action
    fileSaveAction = new QAction(this);
    fileSaveAction->setObjectName(QString::fromUtf8("fileSaveAction"));
    fileSaveAction->setText("fileSaveAction");
    fileSaveAction->setEnabled(false);
    fileSaveAction->setIcon(QIcon(":/icons/db_save"));
    QFont fntS = fileSaveAction->font();
    fntS.setBold(true);
    fileSaveAction->setFont(fntS);

    //** Db Revert
    fileRevertAction = new QAction(this);
    fileRevertAction->setObjectName(QString::fromUtf8("fileRevertAction"));
    fileRevertAction->setText("fileRevertAction");
    fileRevertAction->setEnabled(false);
    fileRevertAction->setIcon(QIcon(":/icons/db_revert"));
    QFont fntR = fileRevertAction->font();
    fntR.setBold(true);
    fileRevertAction->setFont(fntR);

    // Recent dbs
    for(int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        this->connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }


    //** Exit
    fileExitAction = new QAction(this);
    fileExitAction->setObjectName(QString::fromUtf8("fileExitAction"));
    fileExitAction->setText("fileExitAction");

    editCopyAction = new QAction(this);
    editCopyAction->setObjectName(QString::fromUtf8("editCopyAction"));
    editCopyAction->setText("editCopyAction");
    editPasteAction = new QAction(this);
    editPasteAction->setObjectName(QString::fromUtf8("editPasteAction"));
    editPasteAction->setText("editPasteAction");
    editFindAction = new QAction(this);
    editFindAction->setObjectName(QString::fromUtf8("editFindAction"));
    editFindAction->setText("editFindAction");
    editFindAction->setIcon(QIcon(":/oldimages/searchfind"));
    editFindAction->setIconVisibleInMenu(true);
    helpContentsAction = new QAction(this);
    helpContentsAction->setObjectName(QString::fromUtf8("helpContentsAction"));
    helpContentsAction->setText("helpContentsAction");
    helpIndexAction = new QAction(this);
    helpIndexAction->setObjectName(QString::fromUtf8("helpIndexAction"));
    helpIndexAction->setText("helpIndexAction");
    helpAboutAction = new QAction(this);
    helpAboutAction->setObjectName(QString::fromUtf8("helpAboutAction"));
    helpAboutAction->setText("helpAboutAction");

    //** Close Database ??
    fileCloseAction = new QAction(this);
    fileCloseAction->setObjectName(QString::fromUtf8("fileCloseAction"));
    fileCloseAction->setText("fileCloseAction");
    fileCloseAction->setEnabled(false);

    newRecordAction = new QAction(this);
    newRecordAction->setObjectName(QString::fromUtf8("newRecordAction"));
    newRecordAction->setText("newRecordAction");
    fileCompactAction = new QAction(this);
    fileCompactAction->setObjectName(QString::fromUtf8("fileCompactAction"));
    fileCompactAction->setText("fileCompactAction");
    fileCompactAction->setEnabled(false);
    helpWhatsThisAction = new QAction(this);
    helpWhatsThisAction->setObjectName(QString::fromUtf8("helpWhatsThisAction"));
    helpWhatsThisAction->setText("helpWhatsThisAction");
    helpWhatsThisAction->setIcon(QIcon(":/oldimages/whatis"));
    helpWhatsThisAction->setIconVisibleInMenu(true);

    sqlLogAction->setText("sqlLogAction");
    sqlLogAction->setIcon(QIcon(":/oldimages/log"));
    sqlLogAction->setIconVisibleInMenu(true);


    fileImportCSVAction = new QAction(this);
    fileImportCSVAction->setObjectName(QString::fromUtf8("fileImportCSVAction"));
    fileImportCSVAction->setText("fileImportCSVAction");
    fileExportCSVAction = new QAction(this);
    fileExportCSVAction->setObjectName(QString::fromUtf8("fileExportCSVAction"));
    fileExportCSVAction->setText("fileExportCSVAction");




    //fileImportAction = new QAction(this);
    //fileImportAction->setObjectName(QString::fromUtf8("fileImportAction"));
    //fileImportAction->setName("fileImportAction");
    //fileExportAction = new QAction(this);
    //fileExportAction->setObjectName(QString::fromUtf8("fileExportAction"));
    //fileExportAction->setName("fileExportAction");

    //** Create Table
    editCreateTableAction = new QAction(this);
    editCreateTableAction->setObjectName(QString::fromUtf8("editCreateTableAction"));
    editCreateTableAction->setText("editCreateTableAction");
    editCreateTableAction->setEnabled(false);
    editCreateTableAction->setIcon(QIcon(":/icons/table_create"));

    //** Delete table
    editDeleteTableAction = new QAction(this);
    editDeleteTableAction->setObjectName(QString::fromUtf8("editDeleteTableAction"));
    editDeleteTableAction->setText("editDeleteTableAction");
    editDeleteTableAction->setEnabled(false);
    editDeleteTableAction->setIcon(QIcon(":/icons/table_delete"));

    editDeleteTableActionPopup = new QAction(this);
    editDeleteTableActionPopup->setEnabled(false);
    editDeleteTableActionPopup->setIcon(QIcon(":/icons/table_delete"));

    //** Modify Table
    editModifyTableAction = new QAction(this);
    editModifyTableAction->setObjectName(QString::fromUtf8("editModifyTableAction"));
    editModifyTableAction->setText("editModifyTableAction");
    editModifyTableAction->setEnabled(false);
    editModifyTableAction->setIcon(QIcon(":/icons/table_modify"));

    editModifyTableActionPopup = new QAction(this);
    editModifyTableActionPopup->setEnabled(false);
    editModifyTableActionPopup->setIcon(QIcon(":/icons/table_modify"));


    //************************************************
    //** Add, Modify, Delete Field
    editAddFieldActionPopup = new QAction(this);
    editAddFieldActionPopup->setText("Add Field");
    editAddFieldActionPopup->setDisabled(true);
    editAddFieldActionPopup->setIcon(QIcon(":/icons/field_add"));

    editModifyFieldActionPopup = new QAction(this);
    editModifyFieldActionPopup->setText("Modify Field");
    editModifyFieldActionPopup->setDisabled(true);
    editModifyFieldActionPopup->setIcon(QIcon(":/icons/field_edit"));

    editDeleteFieldActionPopup = new QAction(this);
    editDeleteFieldActionPopup->setText("Delete Field");
    editDeleteFieldActionPopup->setDisabled(true);
    editDeleteFieldActionPopup->setIcon(QIcon(":/icons/field_delete"));

    //************************************************
    //** Create/Delete  Index
    editCreateIndexAction = new QAction(this);
    editCreateIndexAction->setObjectName(QString::fromUtf8("editCreateIndexAction"));
    editCreateIndexAction->setText("editCreateIndexAction");
    editCreateIndexAction->setEnabled(false);
    editCreateIndexAction->setIcon(QIcon(":/icons/index_create"));

    editDeleteIndexAction = new QAction(this);
    editDeleteIndexAction->setObjectName(QString::fromUtf8("editDeleteIndexAction"));
    editDeleteIndexAction->setText("editDeleteIndexAction");
    editDeleteIndexAction->setEnabled(false);
    editDeleteIndexAction->setIcon(QIcon(":/icons/index_delete"));

    fileImportSQLAction = new QAction(this);
    fileImportSQLAction->setObjectName(QString::fromUtf8("fileImportSQLAction"));
    fileImportSQLAction->setText("fileImportSQLAction");
    fileExportSQLAction = new QAction(this);
    fileExportSQLAction->setObjectName(QString::fromUtf8("fileExportSQLAction"));
    fileExportSQLAction->setText("fileExportSQLAction");
    editPreferencesAction = new QAction(this);
    editPreferencesAction->setObjectName(QString::fromUtf8("editPreferencesAction"));
    editPreferencesAction->setText("editPreferencesAction");
    widget = new QWidget(this);
    widget->setObjectName(QString::fromUtf8("widget"));
    vboxLayout = new QVBoxLayout(widget);
    vboxLayout->setSpacing(0);
    vboxLayout->setContentsMargins(0,0,0,0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    mainTab = new QTabWidget(widget);
    mainTab->setObjectName(QString::fromUtf8("mainTab"));
    structure = new QWidget();
    structure->setObjectName(QString::fromUtf8("structure"));


    vboxLayout1 = new QVBoxLayout(structure);
    vboxLayout1->setSpacing(0);
    vboxLayout1->setContentsMargins(0,0,0,0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));

    //**** Structure ***********************************
    QToolBar *dbToolbar = new QToolBar();
    vboxLayout1->addWidget(dbToolbar);
    dbToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    dbToolbar->addAction(editCreateTableAction);
    dbToolbar->addAction(editModifyTableActionPopup);
    dbToolbar->addAction(editDeleteTableActionPopup);
    dbToolbar->addSeparator();
    dbToolbar->addAction(editAddFieldActionPopup);
    dbToolbar->addAction(editModifyFieldActionPopup);
    dbToolbar->addAction(editDeleteFieldActionPopup);

    //*** Tree Widget Setup
    dbTreeWidget = new QTreeWidget();
    vboxLayout1->addWidget(dbTreeWidget);
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



    mainTab->addTab(structure, QString());
    browser = new QWidget();
    browser->setObjectName(QString::fromUtf8("browser"));
    vboxLayout2 = new QVBoxLayout(browser);
    vboxLayout2->setSpacing(6);
    vboxLayout2->setContentsMargins(11, 11, 11, 11);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    textLabel1 = new QLabel(browser);
    textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
    textLabel1->setWordWrap(false);

    hboxLayout->addWidget(textLabel1);

    comboBrowseTable = new QComboBox(browser);
    comboBrowseTable->setObjectName(QString::fromUtf8("comboBrowseTable"));
    comboBrowseTable->setMinimumSize(QSize(115, 0));
    comboBrowseTable->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    hboxLayout->addWidget(comboBrowseTable);

    buttonFind = new QPushButton(browser);
    buttonFind->setObjectName(QString::fromUtf8("buttonFind"));
    buttonFind->setIcon(QIcon(":/oldimages/searchfind"));
    buttonFind->setCheckable(true);

    hboxLayout->addWidget(buttonFind);

    buttonRefresh = new QPushButton(browser);
    buttonRefresh->setObjectName("buttonRefresh");
    buttonRefresh->setIcon(QIcon(":/icons/refresh"));
    buttonRefresh->setShortcut(QKeySequence(Qt::Key_F5));

    hboxLayout->addWidget(buttonRefresh);

    spacer1 = new QSpacerItem(51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacer1);

    buttonNewRecord = new QPushButton(browser);
    buttonNewRecord->setObjectName(QString::fromUtf8("buttonNewRecord"));

    hboxLayout->addWidget(buttonNewRecord);

    buttonDeleteRecord = new QPushButton(browser);
    buttonDeleteRecord->setObjectName(QString::fromUtf8("buttonDeleteRecord"));

    hboxLayout->addWidget(buttonDeleteRecord);


    vboxLayout2->addLayout(hboxLayout);

    dataTable = new QTableWidget(browser);
    dataTable->setObjectName(QString::fromUtf8("dataTable"));
    dataTable->setAcceptDrops(true);
    dataTable->setRowCount(0);
    dataTable->setColumnCount(0);
    dataTable->setSelectionMode(QTableWidget::SingleSelection);
    dataTable->setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );

    vboxLayout2->addWidget(dataTable);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    buttonPrevious = new QPushButton(browser);
    buttonPrevious->setObjectName(QString::fromUtf8("buttonPrevious"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(buttonPrevious->sizePolicy().hasHeightForWidth());
    buttonPrevious->setSizePolicy(sizePolicy);

    hboxLayout1->addWidget(buttonPrevious);

    labelRecordset = new QLabel(browser);
    labelRecordset->setObjectName(QString::fromUtf8("labelRecordset"));
    labelRecordset->setWordWrap(false);

    hboxLayout1->addWidget(labelRecordset);

    buttonNext = new QPushButton(browser);
    buttonNext->setObjectName(QString::fromUtf8("buttonNext"));
    sizePolicy.setHeightForWidth(buttonNext->sizePolicy().hasHeightForWidth());
    buttonNext->setSizePolicy(sizePolicy);

    hboxLayout1->addWidget(buttonNext);

    spacer4 = new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacer4);

    buttonGoto = new QPushButton(browser);
    buttonGoto->setObjectName(QString::fromUtf8("buttonGoto"));

    hboxLayout1->addWidget(buttonGoto);

    editGoto = new QLineEdit(browser);
    editGoto->setObjectName(QString::fromUtf8("editGoto"));
    sizePolicy.setHeightForWidth(editGoto->sizePolicy().hasHeightForWidth());
    editGoto->setSizePolicy(sizePolicy);

    hboxLayout1->addWidget(editGoto);


    vboxLayout2->addLayout(hboxLayout1);

    mainTab->addTab(browser, QString());
    query = new QWidget();
    query->setObjectName(QString::fromUtf8("query"));
    vboxLayout3 = new QVBoxLayout(query);
    vboxLayout3->setSpacing(6);
    vboxLayout3->setContentsMargins(11, 11, 11, 11);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    textLabel1_2 = new QLabel(query);
    textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
    textLabel1_2->setWordWrap(false);

    vboxLayout3->addWidget(textLabel1_2);

    sqlTextEdit = new QTextEdit(query);
    sqlTextEdit->setObjectName(QString::fromUtf8("sqlTextEdit"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(5));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(sqlTextEdit->sizePolicy().hasHeightForWidth());
    sqlTextEdit->setSizePolicy(sizePolicy1);
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPixelSize(11);
    sqlTextEdit->setFont(font);

    sqliteHighlighter = new SQLiteSyntaxHighlighter(sqlTextEdit->document());

    vboxLayout3->addWidget(sqlTextEdit);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    executeQueryButton = new QPushButton(query);
    executeQueryButton->setObjectName(QString::fromUtf8("executeQueryButton"));

    hboxLayout2->addWidget(executeQueryButton);

    spacer4_2 = new QSpacerItem(325, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacer4_2);


    vboxLayout3->addLayout(hboxLayout2);

    textLabel2 = new QLabel(query);
    textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
    textLabel2->setWordWrap(false);

    vboxLayout3->addWidget(textLabel2);

    queryErrorLineEdit = new QLineEdit(query);
    queryErrorLineEdit->setObjectName(QString::fromUtf8("queryErrorLineEdit"));
    queryErrorLineEdit->setReadOnly(true);
    queryErrorLineEdit->setDisabled(true);

    vboxLayout3->addWidget(queryErrorLineEdit);

    textLabel3 = new QLabel(query);
    textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
    textLabel3->setWordWrap(false);

    vboxLayout3->addWidget(textLabel3);

    queryResultListModel = new QStandardItemModel(query);

    queryResultTableView = new QTableView(query);
    queryResultTableView->setObjectName(QString::fromUtf8("queryResultTableView"));
    queryResultTableView->setSelectionMode(QTreeView::NoSelection);
    queryResultTableView->setModel(queryResultListModel);
    queryResultTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    queryResultTableView->setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );

    vboxLayout3->addWidget(queryResultTableView);

    mainTab->addTab(query, QString());

    vboxLayout->addWidget(mainTab);

    this->setCentralWidget(widget);

    //*** Setup Toolbar
    toolbarDB = new QToolBar();
    this->addToolBar(Qt::TopToolBarArea, toolbarDB);
    toolbarDB->setObjectName(QString::fromUtf8("Toolbar"));
    toolbarDB->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    viewDBToolbarAction = toolbarDB->toggleViewAction();

    //*** Setup main Menu
    menubar = this->menuBar(); //new QMenuBar(this);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    fileMenu = new QMenu(menubar);
    fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
    importMenu = new QMenu(fileMenu);
    importMenu->setObjectName(QString::fromUtf8("PopupMenuEditor_9"));
    exportMenu = new QMenu(fileMenu);
    exportMenu->setObjectName(QString::fromUtf8("PopupMenuEditor_12"));
    EditMenu = new QMenu(menubar);
    EditMenu->setObjectName(QString::fromUtf8("EditMenu"));
    ViewMenu = new QMenu(menubar);
    ViewMenu->setObjectName(QString::fromUtf8("ViewMenu"));
    PopupMenu = new QMenu(menubar);
    PopupMenu->setObjectName(QString::fromUtf8("PopupMenu"));

    toolbarDB->addAction(fileNewAction);
    toolbarDB->addAction(fileOpenAction);
    toolbarDB->addSeparator();

    toolbarDB->addAction(fileSaveAction);
    toolbarDB->addAction(fileRevertAction);
    toolbarDB->addSeparator();

    /*
    Toolbar->addAction(editCreateTableAction);
    Toolbar->addAction(editDeleteTableAction);
    Toolbar->addAction(editModifyTableAction);
    Toolbar->addSeparator();

    Toolbar->addAction(editCreateIndexAction);
    Toolbar->addAction(editDeleteIndexAction);
    Toolbar->addSeparator();

    Toolbar->addAction(sqlLogAction);
    Toolbar->addSeparator();
    Toolbar->addAction(helpWhatsThisAction);
    */

    menubar->addAction(fileMenu->menuAction());
    menubar->addAction(EditMenu->menuAction());
    menubar->addAction(ViewMenu->menuAction());
    menubar->addAction(PopupMenu->menuAction());

    fileMenu->addAction(fileNewAction);
    fileMenu->addAction(fileOpenAction);
    fileMenu->addAction(fileCloseAction);
    fileMenu->addSeparator();
    fileMenu->addAction(fileSaveAction);
    fileMenu->addAction(fileRevertAction);
    fileMenu->addAction(fileCompactAction);
    fileMenu->addSeparator();
    //fileMenu->addAction(fileImportAction);
    fileMenu->addAction(importMenu->menuAction());
    //fileMenu->addAction(fileExportAction);
    fileMenu->addAction(exportMenu->menuAction());

    recentSeparatorAct = fileMenu->addSeparator();
    for(int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);


    fileMenu->addSeparator();
    fileMenu->addAction(fileExitAction);
    importMenu->addAction(fileImportSQLAction);
    importMenu->addAction(fileImportCSVAction);
    exportMenu->addAction(fileExportSQLAction);
    exportMenu->addAction(fileExportCSVAction);
    EditMenu->addAction(editCreateTableAction);
    EditMenu->addAction(editDeleteTableAction);
    EditMenu->addAction(editModifyTableAction);
    EditMenu->addSeparator();
    EditMenu->addAction(editCreateIndexAction);
    EditMenu->addAction(editDeleteIndexAction);
    EditMenu->addSeparator();
    EditMenu->addAction(editPreferencesAction);
    ViewMenu->addAction(sqlLogAction);
    ViewMenu->addAction(viewDBToolbarAction);
    PopupMenu->addAction(helpWhatsThisAction);
    PopupMenu->addAction(helpAboutAction);

    //***********************************************8
    //** Db Tree Popup Menus
    popupTableMenu = new QMenu(this);
    popupTableMenu->addAction(editModifyTableActionPopup);
    popupTableMenu->addAction(editAddFieldActionPopup);
    popupTableMenu->addSeparator();
    popupTableMenu->addAction(editDeleteTableActionPopup);

    popupFieldMenu = new QMenu(this);
    popupFieldMenu->addAction(editModifyFieldActionPopup);
    popupFieldMenu->addAction(editDeleteFieldActionPopup);
    //spopupFieldMenu->addSeparator();
    //popupFieldMenu->addAction(editDeleteTableActionPopup);


    retranslateUi();
    QObject::connect(fileExitAction, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(fileOpenAction, SIGNAL(activated()), this, SLOT(fileOpen()));
    QObject::connect(fileNewAction, SIGNAL(activated()), this, SLOT(fileNew()));
    QObject::connect(fileCloseAction, SIGNAL(activated()), this, SLOT(fileClose()));
    QObject::connect(comboBrowseTable, SIGNAL(activated(QString)), this, SLOT(populateTable(QString)));
    QObject::connect(buttonNewRecord, SIGNAL(clicked()), this, SLOT(addRecord()));
    QObject::connect(buttonDeleteRecord, SIGNAL(clicked()), this, SLOT(deleteRecord()));
    QObject::connect(buttonPrevious, SIGNAL(clicked()), this, SLOT(navigatePrevious()));
    QObject::connect(buttonNext, SIGNAL(clicked()), this, SLOT(navigateNext()));
    QObject::connect(editGoto, SIGNAL(returnPressed()), this, SLOT(navigateGoto()));
    QObject::connect(buttonGoto, SIGNAL(clicked()), this, SLOT(navigateGoto()));
    QObject::connect(buttonFind, SIGNAL(toggled(bool)), this, SLOT(browseFind(bool)));
    QObject::connect(buttonRefresh, SIGNAL(clicked()), this, SLOT(browseRefresh()));
    QObject::connect(fileCompactAction, SIGNAL(activated()), this, SLOT(compact()));
    QObject::connect(editCopyAction, SIGNAL(activated()), this, SLOT(copy()));
    QObject::connect(editPasteAction, SIGNAL(activated()), this, SLOT(paste()));
    QObject::connect(helpWhatsThisAction, SIGNAL(activated()), this, SLOT(helpWhatsThis()));
    QObject::connect(helpAboutAction, SIGNAL(activated()), this, SLOT(helpAbout()));
    QObject::connect(dataTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(doubleClickTable(int,int)));
    QObject::connect(mainTab, SIGNAL(currentChanged(int)), this, SLOT(mainTabSelected(int)));
    QObject::connect(executeQueryButton, SIGNAL(clicked()), this, SLOT(executeQuery()));
    QObject::connect(fileImportCSVAction, SIGNAL(activated()), this, SLOT(importTableFromCSV()));
    QObject::connect(fileExportCSVAction, SIGNAL(activated()), this, SLOT(exportTableToCSV()));
    QObject::connect(fileRevertAction, SIGNAL(activated()), this, SLOT(fileRevert()));
    QObject::connect(fileSaveAction, SIGNAL(activated()), this, SLOT(fileSave()));
    QObject::connect(editDeleteIndexAction, SIGNAL(activated()), this, SLOT(deleteIndex()));
    QObject::connect(editCreateIndexAction, SIGNAL(activated()), this, SLOT(createIndex()));
    QObject::connect(editCreateTableAction, SIGNAL(activated()), this, SLOT(createTable()));

    QObject::connect(editDeleteTableAction, SIGNAL(activated()), this, SLOT(deleteTable()));
    QObject::connect(editModifyTableAction, SIGNAL(activated()), this, SLOT(editTable()));
    QObject::connect(editDeleteTableActionPopup, SIGNAL(activated()), this, SLOT(deleteTablePopup()));
    QObject::connect(editModifyTableActionPopup, SIGNAL(activated()), this, SLOT(editTablePopup()));
    QObject::connect(editAddFieldActionPopup, SIGNAL(activated()), this, SLOT(on_add_field()));
    QObject::connect(editModifyFieldActionPopup, SIGNAL(activated()), this, SLOT(on_edit_field()));

    QObject::connect(fileExportSQLAction, SIGNAL(activated()), this, SLOT(exportDatabaseToSQL()));
    QObject::connect(fileImportSQLAction, SIGNAL(activated()), this, SLOT(importDatabaseFromSQL()));
    QObject::connect(editPreferencesAction, SIGNAL(activated()), this, SLOT(openPreferences()));

    QObject::connect(dbTreeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_tree_context_menu(const QPoint &)));
    QObject::connect(dbTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(on_tree_selection_changed()));

    QMetaObject::connectSlotsByName(this);
} // setupUi

void MainWindow::retranslateUi()
{
    this->setWindowTitle(QObject::tr("Browser"));

    fileNewAction->setIconText(QObject::tr("New Database"));
    fileNewAction->setText(QObject::tr("&New Database"));
    fileNewAction->setToolTip(QObject::tr("Create a new database file"));
    fileNewAction->setWhatsThis(QObject::tr("This option is used to create a new database file."));
    fileNewAction->setShortcut(QObject::tr("Ctrl+N"));

    fileOpenAction->setIconText(QObject::tr("Open Database"));
    fileOpenAction->setText(QObject::tr("&Open Database"));
    fileOpenAction->setToolTip(QObject::tr("Open an existing database file"));
    fileOpenAction->setWhatsThis(QObject::tr("This option is used to open an existing database file."));
    fileOpenAction->setShortcut(QObject::tr("Ctrl+O"));

    fileExitAction->setIconText(QObject::tr("Exit"));
    fileExitAction->setText(QObject::tr("E&xit"));
    fileExitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

    editCopyAction->setIconText(QObject::tr("Copy"));
    editCopyAction->setText(QObject::tr("&Copy"));
    editCopyAction->setWhatsThis(QObject::tr("Copies the selected text to the clipboard"));
    editCopyAction->setShortcut(QObject::tr("Ctrl+C"));

    editPasteAction->setIconText(QObject::tr("Paste"));
    editPasteAction->setText(QObject::tr("&Paste"));
    editPasteAction->setWhatsThis(QObject::tr("Pastes text from the clipboard into the current text insertion point"));
    editPasteAction->setShortcut(QObject::tr("Ctrl+V"));

    editFindAction->setIconText(QObject::tr("Find"));
    editFindAction->setText(QObject::tr("&Find..."));
    editFindAction->setShortcut(QObject::tr("F"));

    helpContentsAction->setIconText(QObject::tr("Contents"));
    helpContentsAction->setText(QObject::tr("&Contents..."));
    helpContentsAction->setShortcut(QString());

    helpIndexAction->setIconText(QObject::tr("Index"));
    helpIndexAction->setText(QObject::tr("&Index..."));
    helpIndexAction->setShortcut(QString());

    helpAboutAction->setIconText(QObject::tr("About"));
    helpAboutAction->setText(QObject::tr("&About"));
    helpAboutAction->setShortcut(QString());

    fileCloseAction->setIconText(QObject::tr("Close Database"));
    fileCloseAction->setText(QObject::tr("Close Database"));
    fileCloseAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));

    newRecordAction->setIconText(QObject::tr("New Record"));
    newRecordAction->setText(QObject::tr("New Record"));

    fileCompactAction->setIconText(QObject::tr("Compact Database"));
    fileCompactAction->setText(QObject::tr("Compact Database"));
    fileCompactAction->setStatusTip(QObject::tr("Compact the database file, removing space wasted by deleted records."));
    fileCompactAction->setWhatsThis(QObject::tr("Compact the database file, removing space wasted by deleted records."));

    helpWhatsThisAction->setIconText(QObject::tr("What's This?"));
    helpWhatsThisAction->setText(QObject::tr("What's This?"));
    helpWhatsThisAction->setShortcut(QObject::tr("Shift+F1"));

    sqlLogAction->setIconText(QObject::tr("SQL Log"));
    sqlLogAction->setText(QObject::tr("SQL &Log"));
    sqlLogAction->setWhatsThis(QObject::tr("Display or hide the SQL log window, which stores all SQL commands issued by the user or the application."));

    viewDBToolbarAction->setText(QObject::tr("&DB Toolbar"));
    sqlLogAction->setWhatsThis(QObject::tr("Shows or hides the Database toolbar."));

    fileImportCSVAction->setIconText(QObject::tr("Table from CSV file"));
    fileImportCSVAction->setText(QObject::tr("Table from CSV file"));
    fileImportCSVAction->setToolTip(QObject::tr("Open a wizard that lets you import data from a comma separated text file into a database table."));
    fileImportCSVAction->setWhatsThis(QObject::tr("Open a wizard that lets you import data from a comma separated text file into a database table. CSV files can be created on most database and spreadsheet applications."));

    fileExportCSVAction->setIconText(QObject::tr("Table as CSV file"));
    fileExportCSVAction->setText(QObject::tr("Table as CSV file"));
    fileExportCSVAction->setToolTip(QObject::tr("Export a database table as a comma separated text file."));
    fileExportCSVAction->setWhatsThis(QObject::tr("Export a database table as a comma separated text file, ready to be imported into other database or spreadsheet applications."));

    fileSaveAction->setIconText(QObject::tr("Write Changes"));
    fileSaveAction->setText(QObject::tr("Revert Changes"));
    fileSaveAction->setToolTip(QObject::tr("Write changes to the database file"));
    fileSaveAction->setWhatsThis(QObject::tr("This option is used to save changes to the database file."));

    fileRevertAction->setIconText(QObject::tr("Revert Changes"));
    fileRevertAction->setText(QObject::tr("Revert Changes"));
    fileRevertAction->setToolTip(QObject::tr("Revert database to last saved state"));
    fileRevertAction->setWhatsThis(QObject::tr("This option is used to revert the current database file to its last saved state. All changes made since the last save operation are lost."));

    //fileImportAction->setIconText(QObject::tr("Import"));
    //fileImportAction->setText(QObject::tr("Import"));
    //fileExportAction->setIconText(QObject::tr("Export"));
    //fileExportAction->setText(QObject::tr("Export"));

    editCreateTableAction->setIconText(QObject::tr("Create Table"));
    editCreateTableAction->setText(QObject::tr("Create Table"));
    editCreateTableAction->setWhatsThis(QObject::tr("Open the Create Table wizard, where it is possible to define the name and fields for a new table in the database"));

    editDeleteTableAction->setIconText(QObject::tr("Delete Table"));
    editDeleteTableAction->setText(QObject::tr("Delete Table"));

    editDeleteTableActionPopup->setIconText(QObject::tr("Delete Table"));
    editDeleteTableActionPopup->setText(QObject::tr("Delete Table"));


    editDeleteTableAction->setWhatsThis(QObject::tr("Open the Delete Table wizard, where you can select a database table to be dropped."));

    editModifyTableAction->setIconText(QObject::tr("Modify Table"));
    editModifyTableAction->setText(QObject::tr("Modify Table"));

    editModifyTableActionPopup->setIconText(QObject::tr("Modify Table"));
    editModifyTableActionPopup->setText(QObject::tr("Modify Table"));

    editModifyTableAction->setWhatsThis(QObject::tr("Open the Modify Table wizard, where it is possible to rename an existing table. It is also possible to add or delete fields form a table, as well as modify field names and types."));

    editCreateIndexAction->setIconText(QObject::tr("Create Index"));
    editCreateIndexAction->setText(QObject::tr("Create Index"));
    editCreateIndexAction->setWhatsThis(QObject::tr("Open the Create Index wizard, where it is possible to define a new index on an existing database table."));

    editDeleteIndexAction->setIconText(QObject::tr("Delete Index"));
    editDeleteIndexAction->setText(QObject::tr("Delete Index"));
    editDeleteIndexAction->setWhatsThis(QObject::tr("Open the Delete Index wizard, where you can select a database index to be dropped."));

    fileImportSQLAction->setIconText(QObject::tr("Database from SQL file"));
    fileImportSQLAction->setText(QObject::tr("Database from SQL file"));
    fileImportSQLAction->setToolTip(QObject::tr("Import data from an .sql dump text file into a new or existing database."));
    fileImportSQLAction->setWhatsThis(QObject::tr("This option lets you import data from an .sql dump text file into a new or existing database. SQL dump files can be created on most database engines, including MySQL and PostgreSQL."));

    fileExportSQLAction->setIconText(QObject::tr("Database to SQL file"));
    fileExportSQLAction->setText(QObject::tr("Database to SQL file"));
    fileExportSQLAction->setToolTip(QObject::tr("Export a database to a .sql dump text file."));
    fileExportSQLAction->setWhatsThis(QObject::tr("This option lets you export a database to a .sql dump text file. SQL dump files contain all data necessary to recreate the database on most database engines, including MySQL and PostgreSQL."));

    editPreferencesAction->setIconText(QObject::tr("Preferences"));
    editPreferencesAction->setText(QObject::tr("&Preferences"));
    editPreferencesAction->setToolTip(QObject::tr("Open the preferences window.."));
    editPreferencesAction->setWhatsThis(QObject::tr("Open the preferences window."));

    mainTab->setProperty("toolTip", QVariant(QString()));
    mainTab->setTabText(mainTab->indexOf(structure), QObject::tr("Database &Structure"));

    textLabel1->setText(QObject::tr("Table:"));

    comboBrowseTable->clear();
    comboBrowseTable->insertItems(0, QStringList()
        << QObject::tr("select")
        << QObject::tr("users")
    );
    comboBrowseTable->setProperty("toolTip", QVariant(QObject::tr("Select a table to browse data")));
    comboBrowseTable->setProperty("whatsThis", QVariant(QObject::tr("Use this list to select a table to be displayed in the database view")));

    buttonFind->setText(QString());
    buttonFind->setProperty("toolTip", QVariant(QObject::tr("Open or close the floating find window")));
    buttonFind->setProperty("whatsThis", QVariant(QObject::tr("This button toggles the appearance of the Find window, used to search records in the database view")));
    buttonFind->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));


    buttonRefresh->setProperty("toolTip", QVariant( QObject::tr("Refresh the data in the selected table.")));
    buttonRefresh->setProperty("whatsThis", QVariant( QObject::tr("This button refreshes the data in the currently selected table.")));

    buttonNewRecord->setText(QObject::tr("New Record"));
    buttonNewRecord->setProperty("toolTip", QVariant(QObject::tr("Insert a new record in the current table")));
    buttonNewRecord->setProperty("whatsThis", QVariant(QObject::tr("This button creates a new, empty record in the database")));

    buttonDeleteRecord->setText(QObject::tr("Delete Record"));
    buttonDeleteRecord->setProperty("toolTip", QVariant(QObject::tr("Delete the current record")));
    buttonDeleteRecord->setProperty("whatsThis", QVariant(QObject::tr("This button deletes the record currently selected in the database")));


    dataTable->setProperty("toolTip", QVariant(QString()));
    dataTable->setProperty("whatsThis", QVariant(QObject::tr("This is the database view. You can double-click any record to edit its contents in the cell editor window.")));

    buttonPrevious->setText(QObject::tr("<"));
    buttonPrevious->setProperty("toolTip", QVariant(QObject::tr("Go to previous record set page")));
    buttonPrevious->setProperty("whatsThis", QVariant(QObject::tr("This button is used to navigate to the previous set of 1000 records in the database view")));

    labelRecordset->setText(QObject::tr("1000 - 2000 of 100000"));

    buttonNext->setText(QObject::tr(">"));
    buttonNext->setProperty("toolTip", QVariant(QObject::tr("Go to next record set page")));
    buttonNext->setProperty("whatsThis", QVariant(QObject::tr("This button is used to navigate to the next 1000 records set in the database view")));

    buttonGoto->setText(QObject::tr("Go to:"));
    buttonGoto->setProperty("toolTip", QVariant(QObject::tr("Click here to browse the specified record ")));
    buttonGoto->setProperty("whatsThis", QVariant(QObject::tr("This button is used to navigate to the record number specied in the Go to: area.")));

    editGoto->setText(QObject::tr("0"));
    editGoto->setProperty("toolTip", QVariant(QObject::tr("Enter record number to browse")));
    editGoto->setProperty("whatsThis", QVariant(QObject::tr("Type a record number in this area and click the Go to: button to display the record in the database view")));

    mainTab->setTabText(mainTab->indexOf(browser), QObject::tr("&Browse Data"));
    textLabel1_2->setText(QObject::tr("SQL string:"));

    sqlTextEdit->setProperty("toolTip", QVariant(QObject::tr("Type SQL string here")));
    sqlTextEdit->setProperty("whatsThis", QVariant(QObject::tr("Type the SQL query in this area, and press the button below to execute it. Results and error messages will be shown in the fields at the bottom of this form.")));

    executeQueryButton->setText(QObject::tr("Execute query"));
    executeQueryButton->setProperty("toolTip", QVariant(QObject::tr("Execute SQL query")));
    executeQueryButton->setProperty("whatsThis", QVariant(QObject::tr("This button is used to execute the SQL query entered in the text area above.")));

    textLabel2->setText(QObject::tr("Error message from database engine:"));

    queryErrorLineEdit->setProperty("toolTip", QVariant(QObject::tr("Error messages")));
    queryErrorLineEdit->setProperty("whatsThis", QVariant(QObject::tr("Messages returned from the database engine when the SQL query is executed are displayed in this area.")));

    textLabel3->setText(QObject::tr("Data returned:"));

    queryResultTableView->setProperty("toolTip", QVariant(QObject::tr("Query generated data")));
    queryResultTableView->setProperty("whatsThis", QVariant(QObject::tr("This table displays data returned from the database engine as a result of the SQL query. You can not modify data directly on this view, only consult it.")));

    mainTab->setTabText(mainTab->indexOf(query), QObject::tr("E&xecute SQL"));
    //Toolbar->setLabel(QObject::tr("Toolbar"));
    fileMenu->setTitle(QObject::tr("&File"));
    importMenu->setTitle(QObject::tr("&Import"));
    exportMenu->setTitle(QObject::tr("&Export"));
    EditMenu->setTitle(QObject::tr("&Edit"));
    ViewMenu->setTitle(QObject::tr("&View"));
    PopupMenu->setTitle(QObject::tr("&Help"));
} // retranslateUi

/*
 *  Constructs a mainForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
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
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MainWindow::languageChange()
{
    retranslateUi();
}

void MainWindow::init()
{
    
    findWin = 0;
    editWin = 0;

    clipboard = QApplication::clipboard();

    recsPerView = 1000;
    recAtTop = 0;
    gotoValidator = new QIntValidator(0, 0, this);
    editGoto->setValidator(gotoValidator);
    gotoValidator->setRange ( 0, 0);
    resetBrowser();
    this->setWindowTitle(QApplication::applicationName());
    this->setWindowIcon( QPixmap( g_applicationIconName ) );
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

void MainWindow::populateTable( const QString & tablename)
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

void MainWindow::resetBrowser()
{
    recAtTop = 0;
    QString sCurrentTable = comboBrowseTable->currentText();
    comboBrowseTable->clear();
    QStringList tab = db.getTableNames();
    if(!tab.isEmpty()) {
        comboBrowseTable->addItems(tab);
    }
    setRecordsetLabel();
    int pos = comboBrowseTable->findText(sCurrentTable);
    pos = pos == -1 ? 0 : pos;
    comboBrowseTable->setCurrentIndex(pos);
    populateTable(comboBrowseTable->currentText());
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

void MainWindow::updateTableView(int lineToSelect)
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

                QTableWidgetItem* item = new QTableWidgetItem(content);
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                item->setToolTip(wrapText(content));
                dataTable->setItem( rowNum, colNum, item);
                colNum++;
                //}
            }
            rowNum++;
            if (rowNum==recsThisView) break;
        }

    }

    for(int i = 0; i < dataTable->columnCount(); ++i)
    {
        dataTable->resizeColumnToContents(i);
        if( dataTable->columnWidth(i) > 400 )
            dataTable->setColumnWidth(i, 400);
    }
    //dataTable->clearSelection(true);
    if (lineToSelect!=-1){
        //qDebug("inside selection");
        selectTableLine(lineToSelect);
    }
    setRecordsetLabel();
    QApplication::restoreOverrideCursor();
}

void MainWindow::selectTableLine(int lineToSelect)
{
    dataTable->clearSelection();
    dataTable->selectRow(lineToSelect);
    dataTable->setCurrentCell(lineToSelect, 0);
    dataTable->scrollToItem(dataTable->itemAt(lineToSelect, 0));
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
    QString typed = editGoto->text();
    bool ok;
    int dec = typed.toInt( &ok);
    if (dec==0) dec=1;
    if (dec>db.getRecordCount()) dec = db.getRecordCount();

    recAtTop = ((dec-1)/recsPerView)*recsPerView;
    updateTableView(dec-recAtTop-1);
    editGoto->setText(QString::number(dec,10));
}

void MainWindow::setRecordsetLabel()
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
    buttonFind->toggle();
}

void MainWindow::browseRefresh()
{
    populateTable(comboBrowseTable->currentText());
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
    fileCloseAction->setEnabled(true);
    fileCompactAction->setEnabled(true);
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


void MainWindow::paste()
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


void MainWindow::helpWhatsThis()
{
    QWhatsThis::enterWhatsThisMode ();
}


void MainWindow::helpAbout()
{
    AboutDialog * aForm = new AboutDialog( this );
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
    dataTable->setItem( row - recAtTop, col, item);

}

void MainWindow::logWinAway()
{
    sqlLogAction->toggle();
}

void MainWindow::editWinAway()
{
    editWin->hide();
    activateWindow();
    dataTable->setRangeSelected( QTableWidgetSelectionRange(editWin->curRow - recAtTop, editWin->curCol, editWin->curRow- recAtTop, editWin->curCol), true);
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
    QString query = sqlTextEdit->toPlainText().trimmed();
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
        queryErrorLineEdit->setText(lastErrorMessage);
        queryResultTableView->resizeColumnsToContents();

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
    fileSaveAction->setEnabled(dirty);
    fileRevertAction->setEnabled(dirty);
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
        editDeleteTableActionPopup->setDisabled(false);
        editModifyTableActionPopup->setDisabled(false);
        popupTableMenu->exec( dbTreeWidget->mapToGlobal(qPoint) );

    }else if(cItem->text(1) == "field"){
        popupFieldMenu->exec( dbTreeWidget->mapToGlobal(qPoint) );
    }
}
//** Tree selection changed
void MainWindow::on_tree_selection_changed(){
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
    fileCloseAction->setEnabled(enable);
    fileCompactAction->setEnabled(enable);
    editCreateTableAction->setEnabled(enable);
    editDeleteTableAction->setEnabled(enable);
    editModifyTableAction->setEnabled(enable);
    editCreateIndexAction->setEnabled(enable);
    editDeleteIndexAction->setEnabled(enable);

    executeQueryButton->setEnabled(enable);
}
