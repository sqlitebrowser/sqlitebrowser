/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ExtendedTableWidget.h"
#include "qcustomplot.h"
#include "sqltextedit.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *fileNewAction;
    QAction *fileOpenAction;
    QAction *fileCloseAction;
    QAction *fileRevertAction;
    QAction *fileSaveAction;
    QAction *fileCompactAction;
    QAction *fileExitAction;
    QAction *fileImportSQLAction;
    QAction *fileImportCSVAction;
    QAction *fileExportSQLAction;
    QAction *fileExportCSVAction;
    QAction *editCreateTableAction;
    QAction *editDeleteObjectAction;
    QAction *editModifyTableAction;
    QAction *editCreateIndexAction;
    QAction *viewPreferencesAction;
    QAction *viewDBToolbarAction;
    QAction *helpWhatsThisAction;
    QAction *helpAboutAction;
    QAction *actionRecent_y_opened;
    QAction *actionSqlOpenTab;
    QAction *actionExecuteSql;
    QAction *actionSqlOpenFile;
    QAction *actionSqlSaveFile;
    QAction *actionLoadExtension;
    QAction *actionSqlExecuteLine;
    QAction *actionExportCsvPopup;
    QAction *actionWiki;
    QAction *actionBug_report;
    QAction *actionWebsite;
    QAction *actionSaveProject;
    QAction *actionOpenProject;
    QAction *fileAttachAction;
    QAction *actionEncryption;
    QAction *actionSqlSaveFileAs;
    QAction *actionSqlSaveFilePopup;
    QAction *actionEditBrowseTable;
    QAction *actionEditCopyCreateStatement;
    QAction *actionBrowseTableEditDisplayFormat;
    QAction *actionShowRowidColumn;
    QAction *actionSetTableEncoding;
    QAction *actionSetAllTablesEncoding;
    QAction *actionDittoRecord;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_7;
    QTabWidget *mainTab;
    QWidget *structure;
    QVBoxLayout *verticalLayout;
    QToolBar *dbToolbar;
    QTreeView *dbTreeWidget;
    QWidget *browser;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBrowseTable;
    QToolButton *buttonRefresh;
    QToolButton *buttonClearFilters;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonNewRecord;
    QPushButton *buttonDeleteRecord;
    ExtendedTableWidget *dataTable;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *buttonBegin;
    QToolButton *buttonPrevious;
    QLabel *labelRecordset;
    QToolButton *buttonNext;
    QToolButton *buttonEnd;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonGoto;
    QLineEdit *editGoto;
    QWidget *pragmas;
    QVBoxLayout *verticalLayout_4;
    QScrollArea *scrollareaPragmas;
    QWidget *scrollAreaWidgetContents;
    QFormLayout *formLayout;
    QLabel *labelPragmaAutoVacuum;
    QComboBox *comboboxPragmaAutoVacuum;
    QLabel *labelPragmaAutomaticIndex;
    QCheckBox *checkboxPragmaAutomaticIndex;
    QLabel *labelPragmaCheckpointFullFsync;
    QCheckBox *checkboxPragmaCheckpointFullFsync;
    QLabel *labelPragmaForeignKeys;
    QCheckBox *checkboxPragmaForeignKeys;
    QLabel *labelPragmaFullFsync;
    QCheckBox *checkboxPragmaFullFsync;
    QLabel *labelPragmaIgnoreCheckConstraints;
    QCheckBox *checkboxPragmaIgnoreCheckConstraints;
    QLabel *labelPragmaJournalMode;
    QComboBox *comboboxPragmaJournalMode;
    QLabel *labelJournalSizeLimit;
    QSpinBox *spinPragmaJournalSizeLimit;
    QLabel *labelPragmaLockingMode;
    QComboBox *comboboxPragmaLockingMode;
    QLabel *labelPragmaMaxPageCount;
    QSpinBox *spinPragmaMaxPageCount;
    QLabel *labelPragmaPageSize;
    QSpinBox *spinPragmaPageSize;
    QLabel *labelPragmaRecursiveTriggers;
    QCheckBox *checkboxPragmaRecursiveTriggers;
    QLabel *labelPragmaSecureDelete;
    QCheckBox *checkboxPragmaSecureDelete;
    QLabel *labelPragmaSynchronous;
    QComboBox *comboboxPragmaSynchronous;
    QLabel *labelPragmaTempStore;
    QComboBox *comboboxPragmaTempStore;
    QLabel *labelPragmaUserVersion;
    QSpinBox *spinPragmaUserVersion;
    QLabel *labelPragmaWalAutoCheckpoint;
    QSpinBox *spinPragmaWalAutoCheckpoint;
    QDialogButtonBox *buttonBoxPragmas;
    QWidget *query;
    QVBoxLayout *verticalLayout_3;
    QToolBar *toolbarSql;
    QTabWidget *tabSqlAreas;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *menuImport;
    QMenu *menuExport;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QStatusBar *statusbar;
    QToolBar *toolbarDB;
    QDockWidget *dockLog;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelLogSubmittedBy;
    QComboBox *comboLogSubmittedBy;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *buttonLogClear;
    QStackedWidget *stackLog;
    SqlTextEdit *editLogUser;
    SqlTextEdit *editLogApplication;
    QDockWidget *dockPlot;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_6;
    QSplitter *splitterForPlot;
    QTreeWidget *treePlotColumns;
    QCustomPlot *plotWidget;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QComboBox *comboLineType;
    QLabel *label_3;
    QComboBox *comboPointShape;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *butSavePlot;
    QToolButton *buttonLoadAllData;
    QDockWidget *dockSchema;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *verticalLayout_9;
    QTreeView *treeSchemaDock;
    QDockWidget *dockEditWindow;
    QWidget *dockWidgetContents_4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1037, 630);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/appicon"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        fileNewAction = new QAction(MainWindow);
        fileNewAction->setObjectName(QStringLiteral("fileNewAction"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/db_new"), QSize(), QIcon::Normal, QIcon::Off);
        fileNewAction->setIcon(icon1);
        fileOpenAction = new QAction(MainWindow);
        fileOpenAction->setObjectName(QStringLiteral("fileOpenAction"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/db_open"), QSize(), QIcon::Normal, QIcon::Off);
        fileOpenAction->setIcon(icon2);
        fileCloseAction = new QAction(MainWindow);
        fileCloseAction->setObjectName(QStringLiteral("fileCloseAction"));
        fileCloseAction->setEnabled(false);
        fileRevertAction = new QAction(MainWindow);
        fileRevertAction->setObjectName(QStringLiteral("fileRevertAction"));
        fileRevertAction->setEnabled(false);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/db_revert"), QSize(), QIcon::Normal, QIcon::Off);
        fileRevertAction->setIcon(icon3);
        fileSaveAction = new QAction(MainWindow);
        fileSaveAction->setObjectName(QStringLiteral("fileSaveAction"));
        fileSaveAction->setEnabled(false);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/db_save"), QSize(), QIcon::Normal, QIcon::Off);
        fileSaveAction->setIcon(icon4);
        fileCompactAction = new QAction(MainWindow);
        fileCompactAction->setObjectName(QStringLiteral("fileCompactAction"));
        fileCompactAction->setEnabled(false);
        fileExitAction = new QAction(MainWindow);
        fileExitAction->setObjectName(QStringLiteral("fileExitAction"));
        fileExitAction->setMenuRole(QAction::QuitRole);
        fileImportSQLAction = new QAction(MainWindow);
        fileImportSQLAction->setObjectName(QStringLiteral("fileImportSQLAction"));
        fileImportCSVAction = new QAction(MainWindow);
        fileImportCSVAction->setObjectName(QStringLiteral("fileImportCSVAction"));
        fileExportSQLAction = new QAction(MainWindow);
        fileExportSQLAction->setObjectName(QStringLiteral("fileExportSQLAction"));
        fileExportCSVAction = new QAction(MainWindow);
        fileExportCSVAction->setObjectName(QStringLiteral("fileExportCSVAction"));
        editCreateTableAction = new QAction(MainWindow);
        editCreateTableAction->setObjectName(QStringLiteral("editCreateTableAction"));
        editCreateTableAction->setEnabled(false);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/table_create"), QSize(), QIcon::Normal, QIcon::Off);
        editCreateTableAction->setIcon(icon5);
        editDeleteObjectAction = new QAction(MainWindow);
        editDeleteObjectAction->setObjectName(QStringLiteral("editDeleteObjectAction"));
        editDeleteObjectAction->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/table_delete"), QSize(), QIcon::Normal, QIcon::Off);
        editDeleteObjectAction->setIcon(icon6);
        editModifyTableAction = new QAction(MainWindow);
        editModifyTableAction->setObjectName(QStringLiteral("editModifyTableAction"));
        editModifyTableAction->setEnabled(false);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/table_modify"), QSize(), QIcon::Normal, QIcon::Off);
        editModifyTableAction->setIcon(icon7);
        editCreateIndexAction = new QAction(MainWindow);
        editCreateIndexAction->setObjectName(QStringLiteral("editCreateIndexAction"));
        editCreateIndexAction->setEnabled(false);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/index_create"), QSize(), QIcon::Normal, QIcon::Off);
        editCreateIndexAction->setIcon(icon8);
        viewPreferencesAction = new QAction(MainWindow);
        viewPreferencesAction->setObjectName(QStringLiteral("viewPreferencesAction"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icons/settings"), QSize(), QIcon::Normal, QIcon::Off);
        viewPreferencesAction->setIcon(icon9);
        viewPreferencesAction->setMenuRole(QAction::PreferencesRole);
        viewDBToolbarAction = new QAction(MainWindow);
        viewDBToolbarAction->setObjectName(QStringLiteral("viewDBToolbarAction"));
        viewDBToolbarAction->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icons/log_dock"), QSize(), QIcon::Normal, QIcon::Off);
        viewDBToolbarAction->setIcon(icon10);
        helpWhatsThisAction = new QAction(MainWindow);
        helpWhatsThisAction->setObjectName(QStringLiteral("helpWhatsThisAction"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/icons/whatis"), QSize(), QIcon::Normal, QIcon::Off);
        helpWhatsThisAction->setIcon(icon11);
        helpAboutAction = new QAction(MainWindow);
        helpAboutAction->setObjectName(QStringLiteral("helpAboutAction"));
        helpAboutAction->setMenuRole(QAction::AboutRole);
        actionRecent_y_opened = new QAction(MainWindow);
        actionRecent_y_opened->setObjectName(QStringLiteral("actionRecent_y_opened"));
        actionSqlOpenTab = new QAction(MainWindow);
        actionSqlOpenTab->setObjectName(QStringLiteral("actionSqlOpenTab"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/icons/open_tab"), QSize(), QIcon::Normal, QIcon::Off);
        actionSqlOpenTab->setIcon(icon12);
        actionExecuteSql = new QAction(MainWindow);
        actionExecuteSql->setObjectName(QStringLiteral("actionExecuteSql"));
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/icons/run"), QSize(), QIcon::Normal, QIcon::Off);
        actionExecuteSql->setIcon(icon13);
        actionSqlOpenFile = new QAction(MainWindow);
        actionSqlOpenFile->setObjectName(QStringLiteral("actionSqlOpenFile"));
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/icons/document_open"), QSize(), QIcon::Normal, QIcon::Off);
        actionSqlOpenFile->setIcon(icon14);
        actionSqlSaveFile = new QAction(MainWindow);
        actionSqlSaveFile->setObjectName(QStringLiteral("actionSqlSaveFile"));
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/icons/save_sql"), QSize(), QIcon::Normal, QIcon::Off);
        actionSqlSaveFile->setIcon(icon15);
        actionLoadExtension = new QAction(MainWindow);
        actionLoadExtension->setObjectName(QStringLiteral("actionLoadExtension"));
        actionLoadExtension->setEnabled(false);
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/icons/load_extension"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoadExtension->setIcon(icon16);
        actionSqlExecuteLine = new QAction(MainWindow);
        actionSqlExecuteLine->setObjectName(QStringLiteral("actionSqlExecuteLine"));
        QIcon icon17;
        icon17.addFile(QStringLiteral(":/icons/run_line"), QSize(), QIcon::Normal, QIcon::Off);
        actionSqlExecuteLine->setIcon(icon17);
        actionExportCsvPopup = new QAction(MainWindow);
        actionExportCsvPopup->setObjectName(QStringLiteral("actionExportCsvPopup"));
        actionExportCsvPopup->setEnabled(false);
        actionWiki = new QAction(MainWindow);
        actionWiki->setObjectName(QStringLiteral("actionWiki"));
        QIcon icon18;
        icon18.addFile(QStringLiteral(":/icons/browser_open"), QSize(), QIcon::Normal, QIcon::Off);
        actionWiki->setIcon(icon18);
        actionBug_report = new QAction(MainWindow);
        actionBug_report->setObjectName(QStringLiteral("actionBug_report"));
        actionBug_report->setIcon(icon18);
        actionWebsite = new QAction(MainWindow);
        actionWebsite->setObjectName(QStringLiteral("actionWebsite"));
        actionWebsite->setIcon(icon18);
        actionSaveProject = new QAction(MainWindow);
        actionSaveProject->setObjectName(QStringLiteral("actionSaveProject"));
        QIcon icon19;
        icon19.addFile(QStringLiteral(":/icons/project_save"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveProject->setIcon(icon19);
        actionOpenProject = new QAction(MainWindow);
        actionOpenProject->setObjectName(QStringLiteral("actionOpenProject"));
        QIcon icon20;
        icon20.addFile(QStringLiteral(":/icons/project_open"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenProject->setIcon(icon20);
        fileAttachAction = new QAction(MainWindow);
        fileAttachAction->setObjectName(QStringLiteral("fileAttachAction"));
        fileAttachAction->setEnabled(false);
        actionEncryption = new QAction(MainWindow);
        actionEncryption->setObjectName(QStringLiteral("actionEncryption"));
        QIcon icon21;
        icon21.addFile(QStringLiteral(":/icons/encryption"), QSize(), QIcon::Normal, QIcon::Off);
        actionEncryption->setIcon(icon21);
        actionSqlSaveFileAs = new QAction(MainWindow);
        actionSqlSaveFileAs->setObjectName(QStringLiteral("actionSqlSaveFileAs"));
        actionSqlSaveFileAs->setIcon(icon15);
        actionSqlSaveFilePopup = new QAction(MainWindow);
        actionSqlSaveFilePopup->setObjectName(QStringLiteral("actionSqlSaveFilePopup"));
        actionSqlSaveFilePopup->setIcon(icon15);
        actionEditBrowseTable = new QAction(MainWindow);
        actionEditBrowseTable->setObjectName(QStringLiteral("actionEditBrowseTable"));
        QIcon icon22;
        icon22.addFile(QStringLiteral(":/icons/table"), QSize(), QIcon::Normal, QIcon::Off);
        actionEditBrowseTable->setIcon(icon22);
        actionEditCopyCreateStatement = new QAction(MainWindow);
        actionEditCopyCreateStatement->setObjectName(QStringLiteral("actionEditCopyCreateStatement"));
        QIcon icon23;
        icon23.addFile(QStringLiteral(":/icons/copy"), QSize(), QIcon::Normal, QIcon::Off);
        actionEditCopyCreateStatement->setIcon(icon23);
        actionBrowseTableEditDisplayFormat = new QAction(MainWindow);
        actionBrowseTableEditDisplayFormat->setObjectName(QStringLiteral("actionBrowseTableEditDisplayFormat"));
        actionShowRowidColumn = new QAction(MainWindow);
        actionShowRowidColumn->setObjectName(QStringLiteral("actionShowRowidColumn"));
        actionShowRowidColumn->setCheckable(true);
        actionSetTableEncoding = new QAction(MainWindow);
        actionSetTableEncoding->setObjectName(QStringLiteral("actionSetTableEncoding"));
        actionSetAllTablesEncoding = new QAction(MainWindow);
        actionSetAllTablesEncoding->setObjectName(QStringLiteral("actionSetAllTablesEncoding"));
        actionDittoRecord = new QAction(MainWindow);
        actionDittoRecord->setObjectName(QStringLiteral("actionDittoRecord"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout_7 = new QVBoxLayout(centralwidget);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        mainTab = new QTabWidget(centralwidget);
        mainTab->setObjectName(QStringLiteral("mainTab"));
        structure = new QWidget();
        structure->setObjectName(QStringLiteral("structure"));
        verticalLayout = new QVBoxLayout(structure);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        dbToolbar = new QToolBar(structure);
        dbToolbar->setObjectName(QStringLiteral("dbToolbar"));
        dbToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        verticalLayout->addWidget(dbToolbar);

        dbTreeWidget = new QTreeView(structure);
        dbTreeWidget->setObjectName(QStringLiteral("dbTreeWidget"));
        dbTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        dbTreeWidget->setDragEnabled(true);
        dbTreeWidget->setDragDropMode(QAbstractItemView::DragDrop);
        dbTreeWidget->setAlternatingRowColors(true);
        dbTreeWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        dbTreeWidget->setAnimated(true);

        verticalLayout->addWidget(dbTreeWidget);

        mainTab->addTab(structure, QString());
        browser = new QWidget();
        browser->setObjectName(QStringLiteral("browser"));
        verticalLayout_2 = new QVBoxLayout(browser);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(browser);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        comboBrowseTable = new QComboBox(browser);
        comboBrowseTable->setObjectName(QStringLiteral("comboBrowseTable"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBrowseTable->sizePolicy().hasHeightForWidth());
        comboBrowseTable->setSizePolicy(sizePolicy);
        comboBrowseTable->setMinimumSize(QSize(150, 0));

        horizontalLayout->addWidget(comboBrowseTable);

        buttonRefresh = new QToolButton(browser);
        buttonRefresh->setObjectName(QStringLiteral("buttonRefresh"));
        QIcon icon24;
        icon24.addFile(QStringLiteral(":/icons/refresh"), QSize(), QIcon::Normal, QIcon::Off);
        buttonRefresh->setIcon(icon24);

        horizontalLayout->addWidget(buttonRefresh);

        buttonClearFilters = new QToolButton(browser);
        buttonClearFilters->setObjectName(QStringLiteral("buttonClearFilters"));
        QIcon icon25;
        icon25.addFile(QStringLiteral(":/icons/clear_filters"), QSize(), QIcon::Normal, QIcon::Off);
        buttonClearFilters->setIcon(icon25);

        horizontalLayout->addWidget(buttonClearFilters);

        horizontalSpacer = new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonNewRecord = new QPushButton(browser);
        buttonNewRecord->setObjectName(QStringLiteral("buttonNewRecord"));

        horizontalLayout->addWidget(buttonNewRecord);

        buttonDeleteRecord = new QPushButton(browser);
        buttonDeleteRecord->setObjectName(QStringLiteral("buttonDeleteRecord"));

        horizontalLayout->addWidget(buttonDeleteRecord);


        verticalLayout_2->addLayout(horizontalLayout);

        dataTable = new ExtendedTableWidget(browser);
        dataTable->setObjectName(QStringLiteral("dataTable"));
        dataTable->setAcceptDrops(true);
        dataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dataTable->setDragDropMode(QAbstractItemView::DragDrop);
        dataTable->setDefaultDropAction(Qt::CopyAction);
        dataTable->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));

        verticalLayout_2->addWidget(dataTable);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        buttonBegin = new QToolButton(browser);
        buttonBegin->setObjectName(QStringLiteral("buttonBegin"));
        buttonBegin->setEnabled(false);

        horizontalLayout_2->addWidget(buttonBegin);

        buttonPrevious = new QToolButton(browser);
        buttonPrevious->setObjectName(QStringLiteral("buttonPrevious"));
        buttonPrevious->setEnabled(false);

        horizontalLayout_2->addWidget(buttonPrevious);

        labelRecordset = new QLabel(browser);
        labelRecordset->setObjectName(QStringLiteral("labelRecordset"));

        horizontalLayout_2->addWidget(labelRecordset);

        buttonNext = new QToolButton(browser);
        buttonNext->setObjectName(QStringLiteral("buttonNext"));
        buttonNext->setEnabled(false);

        horizontalLayout_2->addWidget(buttonNext);

        buttonEnd = new QToolButton(browser);
        buttonEnd->setObjectName(QStringLiteral("buttonEnd"));
        buttonEnd->setEnabled(false);

        horizontalLayout_2->addWidget(buttonEnd);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        buttonGoto = new QPushButton(browser);
        buttonGoto->setObjectName(QStringLiteral("buttonGoto"));

        horizontalLayout_2->addWidget(buttonGoto);

        editGoto = new QLineEdit(browser);
        editGoto->setObjectName(QStringLiteral("editGoto"));

        horizontalLayout_2->addWidget(editGoto);


        verticalLayout_2->addLayout(horizontalLayout_2);

        mainTab->addTab(browser, QString());
        pragmas = new QWidget();
        pragmas->setObjectName(QStringLiteral("pragmas"));
        verticalLayout_4 = new QVBoxLayout(pragmas);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        scrollareaPragmas = new QScrollArea(pragmas);
        scrollareaPragmas->setObjectName(QStringLiteral("scrollareaPragmas"));
        scrollareaPragmas->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 552, 423));
        formLayout = new QFormLayout(scrollAreaWidgetContents);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        labelPragmaAutoVacuum = new QLabel(scrollAreaWidgetContents);
        labelPragmaAutoVacuum->setObjectName(QStringLiteral("labelPragmaAutoVacuum"));
        labelPragmaAutoVacuum->setOpenExternalLinks(true);
        labelPragmaAutoVacuum->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        formLayout->setWidget(0, QFormLayout::LabelRole, labelPragmaAutoVacuum);

        comboboxPragmaAutoVacuum = new QComboBox(scrollAreaWidgetContents);
        comboboxPragmaAutoVacuum->setObjectName(QStringLiteral("comboboxPragmaAutoVacuum"));

        formLayout->setWidget(0, QFormLayout::FieldRole, comboboxPragmaAutoVacuum);

        labelPragmaAutomaticIndex = new QLabel(scrollAreaWidgetContents);
        labelPragmaAutomaticIndex->setObjectName(QStringLiteral("labelPragmaAutomaticIndex"));
        labelPragmaAutomaticIndex->setOpenExternalLinks(true);

        formLayout->setWidget(1, QFormLayout::LabelRole, labelPragmaAutomaticIndex);

        checkboxPragmaAutomaticIndex = new QCheckBox(scrollAreaWidgetContents);
        checkboxPragmaAutomaticIndex->setObjectName(QStringLiteral("checkboxPragmaAutomaticIndex"));

        formLayout->setWidget(1, QFormLayout::FieldRole, checkboxPragmaAutomaticIndex);

        labelPragmaCheckpointFullFsync = new QLabel(scrollAreaWidgetContents);
        labelPragmaCheckpointFullFsync->setObjectName(QStringLiteral("labelPragmaCheckpointFullFsync"));
        labelPragmaCheckpointFullFsync->setOpenExternalLinks(true);

        formLayout->setWidget(2, QFormLayout::LabelRole, labelPragmaCheckpointFullFsync);

        checkboxPragmaCheckpointFullFsync = new QCheckBox(scrollAreaWidgetContents);
        checkboxPragmaCheckpointFullFsync->setObjectName(QStringLiteral("checkboxPragmaCheckpointFullFsync"));

        formLayout->setWidget(2, QFormLayout::FieldRole, checkboxPragmaCheckpointFullFsync);

        labelPragmaForeignKeys = new QLabel(scrollAreaWidgetContents);
        labelPragmaForeignKeys->setObjectName(QStringLiteral("labelPragmaForeignKeys"));
        labelPragmaForeignKeys->setOpenExternalLinks(true);

        formLayout->setWidget(3, QFormLayout::LabelRole, labelPragmaForeignKeys);

        checkboxPragmaForeignKeys = new QCheckBox(scrollAreaWidgetContents);
        checkboxPragmaForeignKeys->setObjectName(QStringLiteral("checkboxPragmaForeignKeys"));

        formLayout->setWidget(3, QFormLayout::FieldRole, checkboxPragmaForeignKeys);

        labelPragmaFullFsync = new QLabel(scrollAreaWidgetContents);
        labelPragmaFullFsync->setObjectName(QStringLiteral("labelPragmaFullFsync"));
        labelPragmaFullFsync->setOpenExternalLinks(true);

        formLayout->setWidget(4, QFormLayout::LabelRole, labelPragmaFullFsync);

        checkboxPragmaFullFsync = new QCheckBox(scrollAreaWidgetContents);
        checkboxPragmaFullFsync->setObjectName(QStringLiteral("checkboxPragmaFullFsync"));

        formLayout->setWidget(4, QFormLayout::FieldRole, checkboxPragmaFullFsync);

        labelPragmaIgnoreCheckConstraints = new QLabel(scrollAreaWidgetContents);
        labelPragmaIgnoreCheckConstraints->setObjectName(QStringLiteral("labelPragmaIgnoreCheckConstraints"));
        labelPragmaIgnoreCheckConstraints->setOpenExternalLinks(true);

        formLayout->setWidget(5, QFormLayout::LabelRole, labelPragmaIgnoreCheckConstraints);

        checkboxPragmaIgnoreCheckConstraints = new QCheckBox(scrollAreaWidgetContents);
        checkboxPragmaIgnoreCheckConstraints->setObjectName(QStringLiteral("checkboxPragmaIgnoreCheckConstraints"));

        formLayout->setWidget(5, QFormLayout::FieldRole, checkboxPragmaIgnoreCheckConstraints);

        labelPragmaJournalMode = new QLabel(scrollAreaWidgetContents);
        labelPragmaJournalMode->setObjectName(QStringLiteral("labelPragmaJournalMode"));
        labelPragmaJournalMode->setOpenExternalLinks(true);

        formLayout->setWidget(6, QFormLayout::LabelRole, labelPragmaJournalMode);

        comboboxPragmaJournalMode = new QComboBox(scrollAreaWidgetContents);
        comboboxPragmaJournalMode->setObjectName(QStringLiteral("comboboxPragmaJournalMode"));

        formLayout->setWidget(6, QFormLayout::FieldRole, comboboxPragmaJournalMode);

        labelJournalSizeLimit = new QLabel(scrollAreaWidgetContents);
        labelJournalSizeLimit->setObjectName(QStringLiteral("labelJournalSizeLimit"));
        labelJournalSizeLimit->setOpenExternalLinks(true);

        formLayout->setWidget(7, QFormLayout::LabelRole, labelJournalSizeLimit);

        spinPragmaJournalSizeLimit = new QSpinBox(scrollAreaWidgetContents);
        spinPragmaJournalSizeLimit->setObjectName(QStringLiteral("spinPragmaJournalSizeLimit"));
        spinPragmaJournalSizeLimit->setMinimum(-1);
        spinPragmaJournalSizeLimit->setMaximum(100000);

        formLayout->setWidget(7, QFormLayout::FieldRole, spinPragmaJournalSizeLimit);

        labelPragmaLockingMode = new QLabel(scrollAreaWidgetContents);
        labelPragmaLockingMode->setObjectName(QStringLiteral("labelPragmaLockingMode"));
        labelPragmaLockingMode->setOpenExternalLinks(true);

        formLayout->setWidget(8, QFormLayout::LabelRole, labelPragmaLockingMode);

        comboboxPragmaLockingMode = new QComboBox(scrollAreaWidgetContents);
        comboboxPragmaLockingMode->setObjectName(QStringLiteral("comboboxPragmaLockingMode"));

        formLayout->setWidget(8, QFormLayout::FieldRole, comboboxPragmaLockingMode);

        labelPragmaMaxPageCount = new QLabel(scrollAreaWidgetContents);
        labelPragmaMaxPageCount->setObjectName(QStringLiteral("labelPragmaMaxPageCount"));
        labelPragmaMaxPageCount->setOpenExternalLinks(true);

        formLayout->setWidget(9, QFormLayout::LabelRole, labelPragmaMaxPageCount);

        spinPragmaMaxPageCount = new QSpinBox(scrollAreaWidgetContents);
        spinPragmaMaxPageCount->setObjectName(QStringLiteral("spinPragmaMaxPageCount"));
        spinPragmaMaxPageCount->setMaximum(2000000000);

        formLayout->setWidget(9, QFormLayout::FieldRole, spinPragmaMaxPageCount);

        labelPragmaPageSize = new QLabel(scrollAreaWidgetContents);
        labelPragmaPageSize->setObjectName(QStringLiteral("labelPragmaPageSize"));
        labelPragmaPageSize->setOpenExternalLinks(true);

        formLayout->setWidget(10, QFormLayout::LabelRole, labelPragmaPageSize);

        spinPragmaPageSize = new QSpinBox(scrollAreaWidgetContents);
        spinPragmaPageSize->setObjectName(QStringLiteral("spinPragmaPageSize"));
        spinPragmaPageSize->setMinimum(512);
        spinPragmaPageSize->setMaximum(65536);

        formLayout->setWidget(10, QFormLayout::FieldRole, spinPragmaPageSize);

        labelPragmaRecursiveTriggers = new QLabel(scrollAreaWidgetContents);
        labelPragmaRecursiveTriggers->setObjectName(QStringLiteral("labelPragmaRecursiveTriggers"));
        labelPragmaRecursiveTriggers->setOpenExternalLinks(true);

        formLayout->setWidget(11, QFormLayout::LabelRole, labelPragmaRecursiveTriggers);

        checkboxPragmaRecursiveTriggers = new QCheckBox(scrollAreaWidgetContents);
        checkboxPragmaRecursiveTriggers->setObjectName(QStringLiteral("checkboxPragmaRecursiveTriggers"));

        formLayout->setWidget(11, QFormLayout::FieldRole, checkboxPragmaRecursiveTriggers);

        labelPragmaSecureDelete = new QLabel(scrollAreaWidgetContents);
        labelPragmaSecureDelete->setObjectName(QStringLiteral("labelPragmaSecureDelete"));
        labelPragmaSecureDelete->setOpenExternalLinks(true);

        formLayout->setWidget(12, QFormLayout::LabelRole, labelPragmaSecureDelete);

        checkboxPragmaSecureDelete = new QCheckBox(scrollAreaWidgetContents);
        checkboxPragmaSecureDelete->setObjectName(QStringLiteral("checkboxPragmaSecureDelete"));

        formLayout->setWidget(12, QFormLayout::FieldRole, checkboxPragmaSecureDelete);

        labelPragmaSynchronous = new QLabel(scrollAreaWidgetContents);
        labelPragmaSynchronous->setObjectName(QStringLiteral("labelPragmaSynchronous"));
        labelPragmaSynchronous->setOpenExternalLinks(true);

        formLayout->setWidget(13, QFormLayout::LabelRole, labelPragmaSynchronous);

        comboboxPragmaSynchronous = new QComboBox(scrollAreaWidgetContents);
        comboboxPragmaSynchronous->setObjectName(QStringLiteral("comboboxPragmaSynchronous"));

        formLayout->setWidget(13, QFormLayout::FieldRole, comboboxPragmaSynchronous);

        labelPragmaTempStore = new QLabel(scrollAreaWidgetContents);
        labelPragmaTempStore->setObjectName(QStringLiteral("labelPragmaTempStore"));
        labelPragmaTempStore->setOpenExternalLinks(true);

        formLayout->setWidget(14, QFormLayout::LabelRole, labelPragmaTempStore);

        comboboxPragmaTempStore = new QComboBox(scrollAreaWidgetContents);
        comboboxPragmaTempStore->setObjectName(QStringLiteral("comboboxPragmaTempStore"));

        formLayout->setWidget(14, QFormLayout::FieldRole, comboboxPragmaTempStore);

        labelPragmaUserVersion = new QLabel(scrollAreaWidgetContents);
        labelPragmaUserVersion->setObjectName(QStringLiteral("labelPragmaUserVersion"));
        labelPragmaUserVersion->setOpenExternalLinks(true);

        formLayout->setWidget(15, QFormLayout::LabelRole, labelPragmaUserVersion);

        spinPragmaUserVersion = new QSpinBox(scrollAreaWidgetContents);
        spinPragmaUserVersion->setObjectName(QStringLiteral("spinPragmaUserVersion"));
        spinPragmaUserVersion->setMaximum(2147483647);

        formLayout->setWidget(15, QFormLayout::FieldRole, spinPragmaUserVersion);

        labelPragmaWalAutoCheckpoint = new QLabel(scrollAreaWidgetContents);
        labelPragmaWalAutoCheckpoint->setObjectName(QStringLiteral("labelPragmaWalAutoCheckpoint"));
        labelPragmaWalAutoCheckpoint->setOpenExternalLinks(true);

        formLayout->setWidget(16, QFormLayout::LabelRole, labelPragmaWalAutoCheckpoint);

        spinPragmaWalAutoCheckpoint = new QSpinBox(scrollAreaWidgetContents);
        spinPragmaWalAutoCheckpoint->setObjectName(QStringLiteral("spinPragmaWalAutoCheckpoint"));
        spinPragmaWalAutoCheckpoint->setMaximum(10000);

        formLayout->setWidget(16, QFormLayout::FieldRole, spinPragmaWalAutoCheckpoint);

        scrollareaPragmas->setWidget(scrollAreaWidgetContents);

        verticalLayout_4->addWidget(scrollareaPragmas);

        buttonBoxPragmas = new QDialogButtonBox(pragmas);
        buttonBoxPragmas->setObjectName(QStringLiteral("buttonBoxPragmas"));
        buttonBoxPragmas->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);
        buttonBoxPragmas->setCenterButtons(false);

        verticalLayout_4->addWidget(buttonBoxPragmas);

        mainTab->addTab(pragmas, QString());
        query = new QWidget();
        query->setObjectName(QStringLiteral("query"));
        verticalLayout_3 = new QVBoxLayout(query);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        toolbarSql = new QToolBar(query);
        toolbarSql->setObjectName(QStringLiteral("toolbarSql"));

        verticalLayout_3->addWidget(toolbarSql);

        tabSqlAreas = new QTabWidget(query);
        tabSqlAreas->setObjectName(QStringLiteral("tabSqlAreas"));
        tabSqlAreas->setTabsClosable(true);
        tabSqlAreas->setMovable(true);

        verticalLayout_3->addWidget(tabSqlAreas);

        mainTab->addTab(query, QString());

        verticalLayout_7->addWidget(mainTab);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1037, 21));
        fileMenu = new QMenu(menubar);
        fileMenu->setObjectName(QStringLiteral("fileMenu"));
        menuImport = new QMenu(fileMenu);
        menuImport->setObjectName(QStringLiteral("menuImport"));
        menuExport = new QMenu(fileMenu);
        menuExport->setObjectName(QStringLiteral("menuExport"));
        editMenu = new QMenu(menubar);
        editMenu->setObjectName(QStringLiteral("editMenu"));
        viewMenu = new QMenu(menubar);
        viewMenu->setObjectName(QStringLiteral("viewMenu"));
        helpMenu = new QMenu(menubar);
        helpMenu->setObjectName(QStringLiteral("helpMenu"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolbarDB = new QToolBar(MainWindow);
        toolbarDB->setObjectName(QStringLiteral("toolbarDB"));
        toolbarDB->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolbarDB);
        dockLog = new QDockWidget(MainWindow);
        dockLog->setObjectName(QStringLiteral("dockLog"));
        dockLog->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_5 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        labelLogSubmittedBy = new QLabel(dockWidgetContents);
        labelLogSubmittedBy->setObjectName(QStringLiteral("labelLogSubmittedBy"));

        horizontalLayout_3->addWidget(labelLogSubmittedBy);

        comboLogSubmittedBy = new QComboBox(dockWidgetContents);
        comboLogSubmittedBy->setObjectName(QStringLiteral("comboLogSubmittedBy"));

        horizontalLayout_3->addWidget(comboLogSubmittedBy);

        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        buttonLogClear = new QPushButton(dockWidgetContents);
        buttonLogClear->setObjectName(QStringLiteral("buttonLogClear"));

        horizontalLayout_3->addWidget(buttonLogClear);


        verticalLayout_5->addLayout(horizontalLayout_3);

        stackLog = new QStackedWidget(dockWidgetContents);
        stackLog->setObjectName(QStringLiteral("stackLog"));
        editLogUser = new SqlTextEdit();
        editLogUser->setObjectName(QStringLiteral("editLogUser"));
        QFont font;
        font.setFamily(QStringLiteral("Monospace"));
        font.setPointSize(8);
        editLogUser->setFont(font);
        editLogUser->setReadOnly(true);
        stackLog->addWidget(editLogUser);
        editLogApplication = new SqlTextEdit();
        editLogApplication->setObjectName(QStringLiteral("editLogApplication"));
        editLogApplication->setFont(font);
        editLogApplication->setReadOnly(true);
        stackLog->addWidget(editLogApplication);

        verticalLayout_5->addWidget(stackLog);

        dockLog->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockLog);
        dockPlot = new QDockWidget(MainWindow);
        dockPlot->setObjectName(QStringLiteral("dockPlot"));
        dockPlot->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        verticalLayout_6 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        splitterForPlot = new QSplitter(dockWidgetContents_2);
        splitterForPlot->setObjectName(QStringLiteral("splitterForPlot"));
        splitterForPlot->setOrientation(Qt::Vertical);
        treePlotColumns = new QTreeWidget(splitterForPlot);
        treePlotColumns->setObjectName(QStringLiteral("treePlotColumns"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(2);
        sizePolicy1.setHeightForWidth(treePlotColumns->sizePolicy().hasHeightForWidth());
        treePlotColumns->setSizePolicy(sizePolicy1);
        treePlotColumns->setColumnCount(4);
        splitterForPlot->addWidget(treePlotColumns);
        treePlotColumns->header()->setDefaultSectionSize(100);
        plotWidget = new QCustomPlot(splitterForPlot);
        plotWidget->setObjectName(QStringLiteral("plotWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(8);
        sizePolicy2.setHeightForWidth(plotWidget->sizePolicy().hasHeightForWidth());
        plotWidget->setSizePolicy(sizePolicy2);
        splitterForPlot->addWidget(plotWidget);

        verticalLayout_6->addWidget(splitterForPlot);

        widget = new QWidget(dockWidgetContents_2);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);

        comboLineType = new QComboBox(widget);
        comboLineType->setObjectName(QStringLiteral("comboLineType"));

        horizontalLayout_4->addWidget(comboLineType);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        comboPointShape = new QComboBox(widget);
        comboPointShape->setObjectName(QStringLiteral("comboPointShape"));

        horizontalLayout_4->addWidget(comboPointShape);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        butSavePlot = new QPushButton(widget);
        butSavePlot->setObjectName(QStringLiteral("butSavePlot"));
        QIcon icon26;
        icon26.addFile(QStringLiteral(":/icons/save_table"), QSize(), QIcon::Normal, QIcon::Off);
        butSavePlot->setIcon(icon26);
        butSavePlot->setCheckable(false);
        butSavePlot->setAutoDefault(false);
        butSavePlot->setFlat(false);

        horizontalLayout_4->addWidget(butSavePlot);

        buttonLoadAllData = new QToolButton(widget);
        buttonLoadAllData->setObjectName(QStringLiteral("buttonLoadAllData"));
        QIcon icon27;
        icon27.addFile(QStringLiteral(":/icons/keyword"), QSize(), QIcon::Normal, QIcon::Off);
        buttonLoadAllData->setIcon(icon27);

        horizontalLayout_4->addWidget(buttonLoadAllData);


        verticalLayout_6->addWidget(widget);

        dockPlot->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockPlot);
        dockSchema = new QDockWidget(MainWindow);
        dockSchema->setObjectName(QStringLiteral("dockSchema"));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        verticalLayout_9 = new QVBoxLayout(dockWidgetContents_3);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        treeSchemaDock = new QTreeView(dockWidgetContents_3);
        treeSchemaDock->setObjectName(QStringLiteral("treeSchemaDock"));
        treeSchemaDock->setAlternatingRowColors(true);
        treeSchemaDock->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        treeSchemaDock->setAnimated(true);

        verticalLayout_9->addWidget(treeSchemaDock);

        dockSchema->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockSchema);
        dockEditWindow = new QDockWidget(MainWindow);
        dockEditWindow->setObjectName(QStringLiteral("dockEditWindow"));
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        dockEditWindow->setWidget(dockWidgetContents_4);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockEditWindow);
#ifndef QT_NO_SHORTCUT
        label->setBuddy(comboBrowseTable);
        labelPragmaAutoVacuum->setBuddy(comboboxPragmaAutoVacuum);
        labelPragmaAutomaticIndex->setBuddy(checkboxPragmaAutomaticIndex);
        labelPragmaCheckpointFullFsync->setBuddy(checkboxPragmaCheckpointFullFsync);
        labelPragmaForeignKeys->setBuddy(checkboxPragmaForeignKeys);
        labelPragmaFullFsync->setBuddy(checkboxPragmaFullFsync);
        labelPragmaIgnoreCheckConstraints->setBuddy(checkboxPragmaIgnoreCheckConstraints);
        labelPragmaJournalMode->setBuddy(comboboxPragmaJournalMode);
        labelJournalSizeLimit->setBuddy(spinPragmaJournalSizeLimit);
        labelPragmaLockingMode->setBuddy(comboboxPragmaLockingMode);
        labelPragmaMaxPageCount->setBuddy(spinPragmaMaxPageCount);
        labelPragmaPageSize->setBuddy(spinPragmaPageSize);
        labelPragmaRecursiveTriggers->setBuddy(checkboxPragmaRecursiveTriggers);
        labelPragmaSecureDelete->setBuddy(checkboxPragmaSecureDelete);
        labelPragmaSynchronous->setBuddy(comboboxPragmaSynchronous);
        labelPragmaTempStore->setBuddy(comboboxPragmaTempStore);
        labelPragmaUserVersion->setBuddy(spinPragmaUserVersion);
        labelPragmaWalAutoCheckpoint->setBuddy(spinPragmaWalAutoCheckpoint);
        labelLogSubmittedBy->setBuddy(comboLogSubmittedBy);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(mainTab, dbTreeWidget);
        QWidget::setTabOrder(dbTreeWidget, comboLogSubmittedBy);
        QWidget::setTabOrder(comboLogSubmittedBy, buttonLogClear);
        QWidget::setTabOrder(buttonLogClear, treePlotColumns);
        QWidget::setTabOrder(treePlotColumns, comboLineType);
        QWidget::setTabOrder(comboLineType, comboPointShape);
        QWidget::setTabOrder(comboPointShape, butSavePlot);
        QWidget::setTabOrder(butSavePlot, buttonLoadAllData);
        QWidget::setTabOrder(buttonLoadAllData, treeSchemaDock);
        QWidget::setTabOrder(treeSchemaDock, comboBrowseTable);
        QWidget::setTabOrder(comboBrowseTable, buttonRefresh);
        QWidget::setTabOrder(buttonRefresh, buttonClearFilters);
        QWidget::setTabOrder(buttonClearFilters, buttonNewRecord);
        QWidget::setTabOrder(buttonNewRecord, buttonDeleteRecord);
        QWidget::setTabOrder(buttonDeleteRecord, dataTable);
        QWidget::setTabOrder(dataTable, buttonBegin);
        QWidget::setTabOrder(buttonBegin, buttonPrevious);
        QWidget::setTabOrder(buttonPrevious, buttonNext);
        QWidget::setTabOrder(buttonNext, buttonEnd);
        QWidget::setTabOrder(buttonEnd, buttonGoto);
        QWidget::setTabOrder(buttonGoto, editGoto);
        QWidget::setTabOrder(editGoto, scrollareaPragmas);
        QWidget::setTabOrder(scrollareaPragmas, comboboxPragmaAutoVacuum);
        QWidget::setTabOrder(comboboxPragmaAutoVacuum, checkboxPragmaAutomaticIndex);
        QWidget::setTabOrder(checkboxPragmaAutomaticIndex, checkboxPragmaCheckpointFullFsync);
        QWidget::setTabOrder(checkboxPragmaCheckpointFullFsync, checkboxPragmaForeignKeys);
        QWidget::setTabOrder(checkboxPragmaForeignKeys, checkboxPragmaFullFsync);
        QWidget::setTabOrder(checkboxPragmaFullFsync, checkboxPragmaIgnoreCheckConstraints);
        QWidget::setTabOrder(checkboxPragmaIgnoreCheckConstraints, comboboxPragmaJournalMode);
        QWidget::setTabOrder(comboboxPragmaJournalMode, spinPragmaJournalSizeLimit);
        QWidget::setTabOrder(spinPragmaJournalSizeLimit, comboboxPragmaLockingMode);
        QWidget::setTabOrder(comboboxPragmaLockingMode, spinPragmaMaxPageCount);
        QWidget::setTabOrder(spinPragmaMaxPageCount, spinPragmaPageSize);
        QWidget::setTabOrder(spinPragmaPageSize, checkboxPragmaSecureDelete);
        QWidget::setTabOrder(checkboxPragmaSecureDelete, checkboxPragmaRecursiveTriggers);
        QWidget::setTabOrder(checkboxPragmaRecursiveTriggers, comboboxPragmaSynchronous);
        QWidget::setTabOrder(comboboxPragmaSynchronous, comboboxPragmaTempStore);
        QWidget::setTabOrder(comboboxPragmaTempStore, spinPragmaUserVersion);
        QWidget::setTabOrder(spinPragmaUserVersion, spinPragmaWalAutoCheckpoint);
        QWidget::setTabOrder(spinPragmaWalAutoCheckpoint, tabSqlAreas);

        dbToolbar->addAction(editCreateTableAction);
        dbToolbar->addAction(editModifyTableAction);
        dbToolbar->addAction(editDeleteObjectAction);
        toolbarSql->addAction(actionSqlOpenTab);
        toolbarSql->addAction(actionSqlOpenFile);
        toolbarSql->addAction(actionSqlSaveFilePopup);
        toolbarSql->addSeparator();
        toolbarSql->addAction(actionExecuteSql);
        toolbarSql->addAction(actionSqlExecuteLine);
        menubar->addAction(fileMenu->menuAction());
        menubar->addAction(editMenu->menuAction());
        menubar->addAction(viewMenu->menuAction());
        menubar->addAction(helpMenu->menuAction());
        fileMenu->addAction(fileNewAction);
        fileMenu->addAction(fileOpenAction);
        fileMenu->addAction(fileAttachAction);
        fileMenu->addAction(fileCloseAction);
        fileMenu->addSeparator();
        fileMenu->addAction(fileSaveAction);
        fileMenu->addAction(fileRevertAction);
        fileMenu->addAction(fileCompactAction);
        fileMenu->addAction(actionEncryption);
        fileMenu->addAction(actionLoadExtension);
        fileMenu->addSeparator();
        fileMenu->addAction(menuImport->menuAction());
        fileMenu->addAction(menuExport->menuAction());
        fileMenu->addSeparator();
        fileMenu->addAction(actionOpenProject);
        fileMenu->addAction(actionSaveProject);
        fileMenu->addSeparator();
        fileMenu->addAction(fileExitAction);
        menuImport->addAction(fileImportSQLAction);
        menuImport->addAction(fileImportCSVAction);
        menuExport->addAction(fileExportSQLAction);
        menuExport->addAction(fileExportCSVAction);
        editMenu->addAction(editCreateTableAction);
        editMenu->addAction(editModifyTableAction);
        editMenu->addAction(editDeleteObjectAction);
        editMenu->addSeparator();
        editMenu->addAction(editCreateIndexAction);
        viewMenu->addAction(viewDBToolbarAction);
        viewMenu->addSeparator();
        viewMenu->addAction(viewPreferencesAction);
        helpMenu->addAction(helpWhatsThisAction);
        helpMenu->addAction(actionWebsite);
        helpMenu->addAction(actionWiki);
        helpMenu->addAction(actionBug_report);
        helpMenu->addAction(helpAboutAction);
        toolbarDB->addAction(fileNewAction);
        toolbarDB->addAction(fileOpenAction);
        toolbarDB->addSeparator();
        toolbarDB->addAction(fileSaveAction);
        toolbarDB->addAction(fileRevertAction);

        retranslateUi(MainWindow);
        QObject::connect(fileExitAction, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(fileOpenAction, SIGNAL(triggered()), MainWindow, SLOT(fileOpen()));
        QObject::connect(fileNewAction, SIGNAL(triggered()), MainWindow, SLOT(fileNew()));
        QObject::connect(fileCloseAction, SIGNAL(triggered()), MainWindow, SLOT(fileClose()));
        QObject::connect(comboBrowseTable, SIGNAL(activated(QString)), MainWindow, SLOT(populateTable(QString)));
        QObject::connect(buttonNewRecord, SIGNAL(clicked()), MainWindow, SLOT(addRecord()));
        QObject::connect(buttonDeleteRecord, SIGNAL(clicked()), MainWindow, SLOT(deleteRecord()));
        QObject::connect(buttonPrevious, SIGNAL(clicked()), MainWindow, SLOT(navigatePrevious()));
        QObject::connect(buttonNext, SIGNAL(clicked()), MainWindow, SLOT(navigateNext()));
        QObject::connect(buttonGoto, SIGNAL(clicked()), MainWindow, SLOT(navigateGoto()));
        QObject::connect(editGoto, SIGNAL(returnPressed()), MainWindow, SLOT(navigateGoto()));
        QObject::connect(buttonRefresh, SIGNAL(clicked()), MainWindow, SLOT(browseRefresh()));
        QObject::connect(fileCompactAction, SIGNAL(triggered()), MainWindow, SLOT(compact()));
        QObject::connect(helpWhatsThisAction, SIGNAL(triggered()), MainWindow, SLOT(helpWhatsThis()));
        QObject::connect(helpAboutAction, SIGNAL(triggered()), MainWindow, SLOT(helpAbout()));
        QObject::connect(mainTab, SIGNAL(currentChanged(int)), MainWindow, SLOT(mainTabSelected(int)));
        QObject::connect(fileImportCSVAction, SIGNAL(triggered()), MainWindow, SLOT(importTableFromCSV()));
        QObject::connect(fileExportCSVAction, SIGNAL(triggered()), MainWindow, SLOT(exportTableToCSV()));
        QObject::connect(fileRevertAction, SIGNAL(triggered()), MainWindow, SLOT(fileRevert()));
        QObject::connect(fileSaveAction, SIGNAL(triggered()), MainWindow, SLOT(fileSave()));
        QObject::connect(editCreateIndexAction, SIGNAL(triggered()), MainWindow, SLOT(createIndex()));
        QObject::connect(editDeleteObjectAction, SIGNAL(triggered()), MainWindow, SLOT(deleteObject()));
        QObject::connect(editModifyTableAction, SIGNAL(triggered()), MainWindow, SLOT(editTable()));
        QObject::connect(fileExportSQLAction, SIGNAL(triggered()), MainWindow, SLOT(exportDatabaseToSQL()));
        QObject::connect(fileImportSQLAction, SIGNAL(triggered()), MainWindow, SLOT(importDatabaseFromSQL()));
        QObject::connect(viewPreferencesAction, SIGNAL(triggered()), MainWindow, SLOT(openPreferences()));
        QObject::connect(dbTreeWidget, SIGNAL(customContextMenuRequested(QPoint)), MainWindow, SLOT(createTreeContextMenu(QPoint)));
        QObject::connect(viewDBToolbarAction, SIGNAL(toggled(bool)), toolbarDB, SLOT(setVisible(bool)));
        QObject::connect(editCreateTableAction, SIGNAL(triggered()), MainWindow, SLOT(createTable()));
        QObject::connect(buttonBoxPragmas, SIGNAL(rejected()), MainWindow, SLOT(loadPragmas()));
        QObject::connect(buttonBoxPragmas, SIGNAL(accepted()), MainWindow, SLOT(savePragmas()));
        QObject::connect(buttonLogClear, SIGNAL(clicked()), editLogApplication, SLOT(clear()));
        QObject::connect(buttonLogClear, SIGNAL(clicked()), editLogUser, SLOT(clear()));
        QObject::connect(comboLogSubmittedBy, SIGNAL(currentIndexChanged(int)), stackLog, SLOT(setCurrentIndex(int)));
        QObject::connect(dataTable, SIGNAL(doubleClicked(QModelIndex)), MainWindow, SLOT(doubleClickTable(QModelIndex)));
        QObject::connect(tabSqlAreas, SIGNAL(tabCloseRequested(int)), MainWindow, SLOT(closeSqlTab(int)));
        QObject::connect(actionExecuteSql, SIGNAL(triggered()), MainWindow, SLOT(executeQuery()));
        QObject::connect(actionSqlOpenTab, SIGNAL(triggered()), MainWindow, SLOT(openSqlTab()));
        QObject::connect(actionSqlOpenFile, SIGNAL(triggered()), MainWindow, SLOT(openSqlFile()));
        QObject::connect(actionSqlSaveFile, SIGNAL(triggered()), MainWindow, SLOT(saveSqlFile()));
        QObject::connect(actionLoadExtension, SIGNAL(triggered()), MainWindow, SLOT(loadExtension()));
        QObject::connect(actionSqlExecuteLine, SIGNAL(triggered()), MainWindow, SLOT(executeQuery()));
        QObject::connect(actionExportCsvPopup, SIGNAL(triggered()), MainWindow, SLOT(exportTableToCSV()));
        QObject::connect(actionOpenProject, SIGNAL(triggered()), MainWindow, SLOT(loadProject()));
        QObject::connect(actionSaveProject, SIGNAL(triggered()), MainWindow, SLOT(saveProject()));
        QObject::connect(fileAttachAction, SIGNAL(triggered()), MainWindow, SLOT(fileAttach()));
        QObject::connect(actionEncryption, SIGNAL(triggered()), MainWindow, SLOT(editEncryption()));
        QObject::connect(actionSqlSaveFilePopup, SIGNAL(triggered()), MainWindow, SLOT(saveSqlFile()));
        QObject::connect(actionSqlSaveFileAs, SIGNAL(triggered()), MainWindow, SLOT(saveSqlFileAs()));
        QObject::connect(buttonEnd, SIGNAL(clicked()), MainWindow, SLOT(navigateEnd()));
        QObject::connect(buttonBegin, SIGNAL(clicked()), MainWindow, SLOT(navigateBegin()));
        QObject::connect(actionEditBrowseTable, SIGNAL(triggered()), MainWindow, SLOT(switchToBrowseDataTab()));
        QObject::connect(actionEditCopyCreateStatement, SIGNAL(triggered()), MainWindow, SLOT(copyCurrentCreateStatement()));
        QObject::connect(dataTable, SIGNAL(foreignKeyClicked(QString,QString,QByteArray)), MainWindow, SLOT(jumpToRow(QString,QString,QByteArray)));
        QObject::connect(actionBrowseTableEditDisplayFormat, SIGNAL(triggered()), MainWindow, SLOT(editDataColumnDisplayFormat()));
        QObject::connect(actionShowRowidColumn, SIGNAL(triggered(bool)), MainWindow, SLOT(showRowidColumn(bool)));
        QObject::connect(actionSetTableEncoding, SIGNAL(triggered()), MainWindow, SLOT(browseDataSetTableEncoding()));
        QObject::connect(actionSetAllTablesEncoding, SIGNAL(triggered()), MainWindow, SLOT(browseDataSetDefaultTableEncoding()));
        QObject::connect(buttonLoadAllData, SIGNAL(clicked()), MainWindow, SLOT(browseDataFetchAllData()));
        QObject::connect(actionDittoRecord, SIGNAL(triggered()), MainWindow, SLOT(dittoRecord()));

        mainTab->setCurrentIndex(3);
        tabSqlAreas->setCurrentIndex(-1);
        stackLog->setCurrentIndex(0);
        comboLineType->setCurrentIndex(1);
        comboPointShape->setCurrentIndex(0);
        butSavePlot->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "DB Browser for SQLite", 0));
        fileNewAction->setText(QApplication::translate("MainWindow", "&New Database...", 0));
#ifndef QT_NO_TOOLTIP
        fileNewAction->setToolTip(QApplication::translate("MainWindow", "Create a new database file", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        fileNewAction->setStatusTip(QApplication::translate("MainWindow", "Create a new database file", 0));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        fileNewAction->setWhatsThis(QApplication::translate("MainWindow", "This option is used to create a new database file.", 0));
#endif // QT_NO_WHATSTHIS
        fileNewAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0));
        fileOpenAction->setText(QApplication::translate("MainWindow", "&Open Database...", 0));
#ifndef QT_NO_TOOLTIP
        fileOpenAction->setToolTip(QApplication::translate("MainWindow", "Open an existing database file", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        fileOpenAction->setStatusTip(QApplication::translate("MainWindow", "Open an existing database file", 0));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        fileOpenAction->setWhatsThis(QApplication::translate("MainWindow", "This option is used to open an existing database file.", 0));
#endif // QT_NO_WHATSTHIS
        fileOpenAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        fileCloseAction->setText(QApplication::translate("MainWindow", "&Close Database", 0));
        fileCloseAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0));
        fileRevertAction->setText(QApplication::translate("MainWindow", "&Revert Changes", 0));
#ifndef QT_NO_TOOLTIP
        fileRevertAction->setToolTip(QApplication::translate("MainWindow", "Revert database to last saved state", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileRevertAction->setWhatsThis(QApplication::translate("MainWindow", "This option is used to revert the current database file to its last saved state. All changes made since the last save operation are lost.", 0));
#endif // QT_NO_WHATSTHIS
        fileSaveAction->setText(QApplication::translate("MainWindow", "&Write Changes", 0));
#ifndef QT_NO_TOOLTIP
        fileSaveAction->setToolTip(QApplication::translate("MainWindow", "Write changes to the database file", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileSaveAction->setWhatsThis(QApplication::translate("MainWindow", "This option is used to save changes to the database file.", 0));
#endif // QT_NO_WHATSTHIS
        fileSaveAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        fileCompactAction->setText(QApplication::translate("MainWindow", "Compact &Database", 0));
#ifndef QT_NO_TOOLTIP
        fileCompactAction->setToolTip(QApplication::translate("MainWindow", "Compact the database file, removing space wasted by deleted records", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        fileCompactAction->setStatusTip(QApplication::translate("MainWindow", "Compact the database file, removing space wasted by deleted records.", 0));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        fileCompactAction->setWhatsThis(QApplication::translate("MainWindow", "Compact the database file, removing space wasted by deleted records.", 0));
#endif // QT_NO_WHATSTHIS
        fileExitAction->setText(QApplication::translate("MainWindow", "E&xit", 0));
        fileExitAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0));
        fileImportSQLAction->setText(QApplication::translate("MainWindow", "&Database from SQL file...", 0));
#ifndef QT_NO_TOOLTIP
        fileImportSQLAction->setToolTip(QApplication::translate("MainWindow", "Import data from an .sql dump text file into a new or existing database.", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileImportSQLAction->setWhatsThis(QApplication::translate("MainWindow", "This option lets you import data from an .sql dump text file into a new or existing database. SQL dump files can be created on most database engines, including MySQL and PostgreSQL.", 0));
#endif // QT_NO_WHATSTHIS
        fileImportCSVAction->setText(QApplication::translate("MainWindow", "&Table from CSV file...", 0));
#ifndef QT_NO_TOOLTIP
        fileImportCSVAction->setToolTip(QApplication::translate("MainWindow", "Open a wizard that lets you import data from a comma separated text file into a database table.", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileImportCSVAction->setWhatsThis(QApplication::translate("MainWindow", "Open a wizard that lets you import data from a comma separated text file into a database table. CSV files can be created on most database and spreadsheet applications.", 0));
#endif // QT_NO_WHATSTHIS
        fileExportSQLAction->setText(QApplication::translate("MainWindow", "&Database to SQL file...", 0));
#ifndef QT_NO_TOOLTIP
        fileExportSQLAction->setToolTip(QApplication::translate("MainWindow", "Export a database to a .sql dump text file.", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileExportSQLAction->setWhatsThis(QApplication::translate("MainWindow", "This option lets you export a database to a .sql dump text file. SQL dump files contain all data necessary to recreate the database on most database engines, including MySQL and PostgreSQL.", 0));
#endif // QT_NO_WHATSTHIS
        fileExportCSVAction->setText(QApplication::translate("MainWindow", "&Table(s) as CSV file...", 0));
#ifndef QT_NO_TOOLTIP
        fileExportCSVAction->setToolTip(QApplication::translate("MainWindow", "Export a database table as a comma separated text file.", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileExportCSVAction->setWhatsThis(QApplication::translate("MainWindow", "Export a database table as a comma separated text file, ready to be imported into other database or spreadsheet applications.", 0));
#endif // QT_NO_WHATSTHIS
        editCreateTableAction->setText(QApplication::translate("MainWindow", "&Create Table...", 0));
#ifndef QT_NO_WHATSTHIS
        editCreateTableAction->setWhatsThis(QApplication::translate("MainWindow", "Open the Create Table wizard, where it is possible to define the name and fields for a new table in the database", 0));
#endif // QT_NO_WHATSTHIS
        editDeleteObjectAction->setText(QApplication::translate("MainWindow", "&Delete Table...", 0));
#ifndef QT_NO_TOOLTIP
        editDeleteObjectAction->setToolTip(QApplication::translate("MainWindow", "Delete Table", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        editDeleteObjectAction->setWhatsThis(QApplication::translate("MainWindow", "Open the Delete Table wizard, where you can select a database table to be dropped.", 0));
#endif // QT_NO_WHATSTHIS
        editModifyTableAction->setText(QApplication::translate("MainWindow", "&Modify Table...", 0));
#ifndef QT_NO_WHATSTHIS
        editModifyTableAction->setWhatsThis(QApplication::translate("MainWindow", "Open the Modify Table wizard, where it is possible to rename an existing table. It is also possible to add or delete fields form a table, as well as modify field names and types.", 0));
#endif // QT_NO_WHATSTHIS
        editCreateIndexAction->setText(QApplication::translate("MainWindow", "Create &Index...", 0));
#ifndef QT_NO_WHATSTHIS
        editCreateIndexAction->setWhatsThis(QApplication::translate("MainWindow", "Open the Create Index wizard, where it is possible to define a new index on an existing database table.", 0));
#endif // QT_NO_WHATSTHIS
        viewPreferencesAction->setText(QApplication::translate("MainWindow", "&Preferences...", 0));
#ifndef QT_NO_TOOLTIP
        viewPreferencesAction->setToolTip(QApplication::translate("MainWindow", "Open the preferences window.", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        viewPreferencesAction->setWhatsThis(QApplication::translate("MainWindow", "Open the preferences window.", 0));
#endif // QT_NO_WHATSTHIS
        viewDBToolbarAction->setText(QApplication::translate("MainWindow", "&DB Toolbar", 0));
#ifndef QT_NO_WHATSTHIS
        viewDBToolbarAction->setWhatsThis(QApplication::translate("MainWindow", "Shows or hides the Database toolbar.", 0));
#endif // QT_NO_WHATSTHIS
        viewDBToolbarAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+T", 0));
        helpWhatsThisAction->setText(QApplication::translate("MainWindow", "W&hat's This?", 0));
        helpWhatsThisAction->setShortcut(QApplication::translate("MainWindow", "Shift+F1", 0));
        helpAboutAction->setText(QApplication::translate("MainWindow", "&About...", 0));
        actionRecent_y_opened->setText(QApplication::translate("MainWindow", "&Recently opened", 0));
        actionSqlOpenTab->setText(QApplication::translate("MainWindow", "Open &tab", 0));
        actionSqlOpenTab->setShortcut(QApplication::translate("MainWindow", "Ctrl+T", 0));
        actionExecuteSql->setText(QApplication::translate("MainWindow", "&Execute SQL", 0));
#ifndef QT_NO_TOOLTIP
        actionExecuteSql->setToolTip(QApplication::translate("MainWindow", "Execute SQL [F5, Ctrl+Return]", 0));
#endif // QT_NO_TOOLTIP
        actionExecuteSql->setShortcut(QApplication::translate("MainWindow", "F5", 0));
        actionSqlOpenFile->setText(QApplication::translate("MainWindow", "Open SQL file", 0));
        actionSqlSaveFile->setText(QApplication::translate("MainWindow", "Save SQL file", 0));
        actionLoadExtension->setText(QApplication::translate("MainWindow", "&Load extension", 0));
        actionSqlExecuteLine->setText(QApplication::translate("MainWindow", "Execute current line", 0));
#ifndef QT_NO_TOOLTIP
        actionSqlExecuteLine->setToolTip(QApplication::translate("MainWindow", "Execute current line [Ctrl+E]", 0));
#endif // QT_NO_TOOLTIP
        actionSqlExecuteLine->setShortcut(QApplication::translate("MainWindow", "Ctrl+E", 0));
        actionExportCsvPopup->setText(QApplication::translate("MainWindow", "Export as CSV file", 0));
#ifndef QT_NO_TOOLTIP
        actionExportCsvPopup->setToolTip(QApplication::translate("MainWindow", "Export table as comma separated values file", 0));
#endif // QT_NO_TOOLTIP
        actionWiki->setText(QApplication::translate("MainWindow", "&Wiki...", 0));
        actionBug_report->setText(QApplication::translate("MainWindow", "Bug &report...", 0));
        actionWebsite->setText(QApplication::translate("MainWindow", "Web&site...", 0));
        actionSaveProject->setText(QApplication::translate("MainWindow", "Sa&ve Project", 0));
#ifndef QT_NO_TOOLTIP
        actionSaveProject->setToolTip(QApplication::translate("MainWindow", "Save the current session to a file", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionSaveProject->setStatusTip(QApplication::translate("MainWindow", "Save the current session to a file", 0));
#endif // QT_NO_STATUSTIP
        actionOpenProject->setText(QApplication::translate("MainWindow", "Open &Project", 0));
#ifndef QT_NO_TOOLTIP
        actionOpenProject->setToolTip(QApplication::translate("MainWindow", "Load a working session from a file", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionOpenProject->setStatusTip(QApplication::translate("MainWindow", "Load a working session from a file", 0));
#endif // QT_NO_STATUSTIP
        fileAttachAction->setText(QApplication::translate("MainWindow", "&Attach Database", 0));
        actionEncryption->setText(QApplication::translate("MainWindow", "&Set Encryption", 0));
        actionSqlSaveFileAs->setText(QApplication::translate("MainWindow", "Save SQL file as", 0));
#ifndef QT_NO_TOOLTIP
        actionSqlSaveFileAs->setToolTip(QApplication::translate("MainWindow", "Save SQL file as", 0));
#endif // QT_NO_TOOLTIP
        actionSqlSaveFilePopup->setText(QApplication::translate("MainWindow", "Save SQL file", 0));
#ifndef QT_NO_TOOLTIP
        actionSqlSaveFilePopup->setToolTip(QApplication::translate("MainWindow", "Save SQL file", 0));
#endif // QT_NO_TOOLTIP
        actionEditBrowseTable->setText(QApplication::translate("MainWindow", "&Browse Table", 0));
        actionEditCopyCreateStatement->setText(QApplication::translate("MainWindow", "Copy Create statement", 0));
#ifndef QT_NO_TOOLTIP
        actionEditCopyCreateStatement->setToolTip(QApplication::translate("MainWindow", "Copy the CREATE statement of the item to the clipboard", 0));
#endif // QT_NO_TOOLTIP
        actionBrowseTableEditDisplayFormat->setText(QApplication::translate("MainWindow", "Edit display format", 0));
#ifndef QT_NO_TOOLTIP
        actionBrowseTableEditDisplayFormat->setToolTip(QApplication::translate("MainWindow", "Edit the display format of the data in this column", 0));
#endif // QT_NO_TOOLTIP
        actionShowRowidColumn->setText(QApplication::translate("MainWindow", "Show rowid column", 0));
#ifndef QT_NO_TOOLTIP
        actionShowRowidColumn->setToolTip(QApplication::translate("MainWindow", "Toggle the visibility of the rowid column", 0));
#endif // QT_NO_TOOLTIP
        actionSetTableEncoding->setText(QApplication::translate("MainWindow", "Set encoding", 0));
#ifndef QT_NO_TOOLTIP
        actionSetTableEncoding->setToolTip(QApplication::translate("MainWindow", "Change the encoding of the text in the table cells", 0));
#endif // QT_NO_TOOLTIP
        actionSetAllTablesEncoding->setText(QApplication::translate("MainWindow", "Set encoding for all tables", 0));
#ifndef QT_NO_TOOLTIP
        actionSetAllTablesEncoding->setToolTip(QApplication::translate("MainWindow", "Change the default encoding assumed for all tables in the database", 0));
#endif // QT_NO_TOOLTIP
        actionDittoRecord->setText(QApplication::translate("MainWindow", "Duplicate record", 0));
#ifndef QT_NO_TOOLTIP
        actionDittoRecord->setToolTip(QApplication::translate("MainWindow", "Duplicate record", 0));
#endif // QT_NO_TOOLTIP
        dbToolbar->setWindowTitle(QApplication::translate("MainWindow", "toolBar1", 0));
        mainTab->setTabText(mainTab->indexOf(structure), QApplication::translate("MainWindow", "Database &Structure", 0));
        label->setText(QApplication::translate("MainWindow", "Table:", 0));
#ifndef QT_NO_TOOLTIP
        comboBrowseTable->setToolTip(QApplication::translate("MainWindow", "Select a table to browse data", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        comboBrowseTable->setWhatsThis(QApplication::translate("MainWindow", "Use this list to select a table to be displayed in the database view", 0));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        buttonRefresh->setToolTip(QApplication::translate("MainWindow", "Refresh the data in the selected table.", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonRefresh->setWhatsThis(QApplication::translate("MainWindow", "This button refreshes the data in the currently selected table.", 0));
#endif // QT_NO_WHATSTHIS
        buttonRefresh->setText(QString());
        buttonRefresh->setShortcut(QApplication::translate("MainWindow", "F5", 0));
#ifndef QT_NO_TOOLTIP
        buttonClearFilters->setToolTip(QApplication::translate("MainWindow", "Clear all filters", 0));
#endif // QT_NO_TOOLTIP
        buttonClearFilters->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonNewRecord->setToolTip(QApplication::translate("MainWindow", "Insert a new record in the current table", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonNewRecord->setWhatsThis(QApplication::translate("MainWindow", "This button creates a new, empty record in the database", 0));
#endif // QT_NO_WHATSTHIS
        buttonNewRecord->setText(QApplication::translate("MainWindow", "New Record", 0));
#ifndef QT_NO_TOOLTIP
        buttonDeleteRecord->setToolTip(QApplication::translate("MainWindow", "Delete the current record", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonDeleteRecord->setWhatsThis(QApplication::translate("MainWindow", "This button deletes the record currently selected in the database", 0));
#endif // QT_NO_WHATSTHIS
        buttonDeleteRecord->setText(QApplication::translate("MainWindow", "Delete Record", 0));
#ifndef QT_NO_WHATSTHIS
        dataTable->setWhatsThis(QApplication::translate("MainWindow", "This is the database view. You can double-click any record to edit its contents in the cell editor window.", 0));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        buttonBegin->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Scroll to the beginning</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonBegin->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>Clicking this button navigates to the beginning in the table view above.</p></body></html>", 0));
#endif // QT_NO_WHATSTHIS
        buttonBegin->setText(QApplication::translate("MainWindow", "|<", 0));
#ifndef QT_NO_TOOLTIP
        buttonPrevious->setToolTip(QApplication::translate("MainWindow", "Scroll 100 records upwards", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonPrevious->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>Clicking this button navigates 100 records upwards in the table view above.</p></body></html>", 0));
#endif // QT_NO_WHATSTHIS
        buttonPrevious->setText(QApplication::translate("MainWindow", "<", 0));
        labelRecordset->setText(QApplication::translate("MainWindow", "0 - 0 of 0", 0));
#ifndef QT_NO_TOOLTIP
        buttonNext->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Scroll 100 records downwards</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonNext->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>Clicking this button navigates 100 records downwards in the table view above.</p></body></html>", 0));
#endif // QT_NO_WHATSTHIS
        buttonNext->setText(QApplication::translate("MainWindow", ">", 0));
#ifndef QT_NO_TOOLTIP
        buttonEnd->setToolTip(QApplication::translate("MainWindow", "Scroll to the end", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonEnd->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Clicking this button navigates up to the end in the table view above.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</p></body></html>", 0));
#endif // QT_NO_WHATSTHIS
        buttonEnd->setText(QApplication::translate("MainWindow", ">|", 0));
#ifndef QT_NO_TOOLTIP
        buttonGoto->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Click here to jump to the specified record</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonGoto->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>This button is used to navigate to the record number specified in the Go to area.</p></body></html>", 0));
#endif // QT_NO_WHATSTHIS
        buttonGoto->setText(QApplication::translate("MainWindow", "Go to:", 0));
#ifndef QT_NO_TOOLTIP
        editGoto->setToolTip(QApplication::translate("MainWindow", "Enter record number to browse", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        editGoto->setWhatsThis(QApplication::translate("MainWindow", "Type a record number in this area and click the Go to: button to display the record in the database view", 0));
#endif // QT_NO_WHATSTHIS
        editGoto->setText(QApplication::translate("MainWindow", "1", 0));
        mainTab->setTabText(mainTab->indexOf(browser), QApplication::translate("MainWindow", "&Browse Data", 0));
        labelPragmaAutoVacuum->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_auto_vacuum\"><span style=\" text-decoration: underline; color:#0000ff;\">Auto Vacuum</span></a></p></body></html>", 0));
        comboboxPragmaAutoVacuum->clear();
        comboboxPragmaAutoVacuum->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "None", 0)
         << QApplication::translate("MainWindow", "Full", 0)
         << QApplication::translate("MainWindow", "Incremental", 0)
        );
        labelPragmaAutomaticIndex->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_automatic_index\"><span style=\" text-decoration: underline; color:#0000ff;\">Automatic Index</span></a></p></body></html>", 0));
        checkboxPragmaAutomaticIndex->setText(QString());
        labelPragmaCheckpointFullFsync->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_checkpoint_fullfsync\"><span style=\" text-decoration: underline; color:#0000ff;\">Checkpoint Full FSYNC</span></a></p></body></html>", 0));
        checkboxPragmaCheckpointFullFsync->setText(QString());
        labelPragmaForeignKeys->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_foreign_keys\"><span style=\" text-decoration: underline; color:#0000ff;\">Foreign Keys</span></a></p></body></html>", 0));
        checkboxPragmaForeignKeys->setText(QString());
        labelPragmaFullFsync->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_fullfsync\"><span style=\" text-decoration: underline; color:#0000ff;\">Full FSYNC</span></a></p></body></html>", 0));
        checkboxPragmaFullFsync->setText(QString());
        labelPragmaIgnoreCheckConstraints->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_ignore_check_constraints\"><span style=\" text-decoration: underline; color:#0000ff;\">Ignore Check Constraints</span></a></p></body></html>", 0));
        checkboxPragmaIgnoreCheckConstraints->setText(QString());
        labelPragmaJournalMode->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_journal_mode\"><span style=\" text-decoration: underline; color:#0000ff;\">Journal Mode</span></a></p></body></html>", 0));
        comboboxPragmaJournalMode->clear();
        comboboxPragmaJournalMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Delete", 0)
         << QApplication::translate("MainWindow", "Truncate", 0)
         << QApplication::translate("MainWindow", "Persist", 0)
         << QApplication::translate("MainWindow", "Memory", 0)
         << QApplication::translate("MainWindow", "WAL", 0)
         << QApplication::translate("MainWindow", "Off", 0)
        );
        labelJournalSizeLimit->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_journal_size_limit\"><span style=\" text-decoration: underline; color:#0000ff;\">Journal Size Limit</span></a></p></body></html>", 0));
        labelPragmaLockingMode->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_locking_mode\"><span style=\" text-decoration: underline; color:#0000ff;\">Locking Mode</span></a></p></body></html>", 0));
        comboboxPragmaLockingMode->clear();
        comboboxPragmaLockingMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Normal", 0)
         << QApplication::translate("MainWindow", "Exclusive", 0)
        );
        labelPragmaMaxPageCount->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_max_page_count\"><span style=\" text-decoration: underline; color:#0000ff;\">Max Page Count</span></a></p></body></html>", 0));
        labelPragmaPageSize->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_page_size\"><span style=\" text-decoration: underline; color:#0000ff;\">Page Size</span></a></p></body></html>", 0));
        labelPragmaRecursiveTriggers->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_recursive_triggers\"><span style=\" text-decoration: underline; color:#0000ff;\">Recursive Triggers</span></a></p></body></html>", 0));
        checkboxPragmaRecursiveTriggers->setText(QString());
        labelPragmaSecureDelete->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_secure_delete\"><span style=\" text-decoration: underline; color:#0000ff;\">Secure Delete</span></a></p></body></html>", 0));
        checkboxPragmaSecureDelete->setText(QString());
        labelPragmaSynchronous->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_synchronous\"><span style=\" text-decoration: underline; color:#0000ff;\">Synchronous</span></a></p></body></html>", 0));
        comboboxPragmaSynchronous->clear();
        comboboxPragmaSynchronous->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0)
         << QApplication::translate("MainWindow", "Normal", 0)
         << QApplication::translate("MainWindow", "Full", 0)
        );
        labelPragmaTempStore->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_temp_store\"><span style=\" text-decoration: underline; color:#0000ff;\">Temp Store</span></a></p></body></html>", 0));
        comboboxPragmaTempStore->clear();
        comboboxPragmaTempStore->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Default", 0)
         << QApplication::translate("MainWindow", "File", 0)
         << QApplication::translate("MainWindow", "Memory", 0)
        );
        labelPragmaUserVersion->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_schema_version\"><span style=\" text-decoration: underline; color:#0000ff;\">User Version</span></a></p></body></html>", 0));
        labelPragmaWalAutoCheckpoint->setText(QApplication::translate("MainWindow", "<html><head/><body><p><a href=\"http://www.sqlite.org/pragma.html#pragma_wal_autocheckpoint\"><span style=\" text-decoration: underline; color:#0000ff;\">WAL Auto Checkpoint</span></a></p></body></html>", 0));
        mainTab->setTabText(mainTab->indexOf(pragmas), QApplication::translate("MainWindow", "Edit &Pragmas", 0));
        toolbarSql->setWindowTitle(QApplication::translate("MainWindow", "toolBar1", 0));
        mainTab->setTabText(mainTab->indexOf(query), QApplication::translate("MainWindow", "E&xecute SQL", 0));
        fileMenu->setTitle(QApplication::translate("MainWindow", "&File", 0));
        menuImport->setTitle(QApplication::translate("MainWindow", "&Import", 0));
        menuExport->setTitle(QApplication::translate("MainWindow", "&Export", 0));
        editMenu->setTitle(QApplication::translate("MainWindow", "&Edit", 0));
        viewMenu->setTitle(QApplication::translate("MainWindow", "&View", 0));
        helpMenu->setTitle(QApplication::translate("MainWindow", "&Help", 0));
        toolbarDB->setWindowTitle(QApplication::translate("MainWindow", "DB Toolbar", 0));
        dockLog->setWindowTitle(QApplication::translate("MainWindow", "SQL &Log", 0));
        labelLogSubmittedBy->setText(QApplication::translate("MainWindow", "Show S&QL submitted by", 0));
        comboLogSubmittedBy->clear();
        comboLogSubmittedBy->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "User", 0)
         << QApplication::translate("MainWindow", "Application", 0)
        );
        buttonLogClear->setText(QApplication::translate("MainWindow", "&Clear", 0));
        dockPlot->setWindowTitle(QApplication::translate("MainWindow", "&Plot", 0));
        QTreeWidgetItem *___qtreewidgetitem = treePlotColumns->headerItem();
        ___qtreewidgetitem->setText(3, QApplication::translate("MainWindow", "_", 0));
        ___qtreewidgetitem->setText(2, QApplication::translate("MainWindow", "Y", 0));
        ___qtreewidgetitem->setText(1, QApplication::translate("MainWindow", "X", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Columns", 0));
        label_2->setText(QApplication::translate("MainWindow", "Line type:", 0));
        comboLineType->clear();
        comboLineType->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "None", 0)
         << QApplication::translate("MainWindow", "Line", 0)
         << QApplication::translate("MainWindow", "StepLeft", 0)
         << QApplication::translate("MainWindow", "StepRight", 0)
         << QApplication::translate("MainWindow", "StepCenter", 0)
         << QApplication::translate("MainWindow", "Impulse", 0)
        );
        label_3->setText(QApplication::translate("MainWindow", "Point shape:", 0));
        comboPointShape->clear();
        comboPointShape->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "None", 0)
         << QApplication::translate("MainWindow", "Cross", 0)
         << QApplication::translate("MainWindow", "Plus", 0)
         << QApplication::translate("MainWindow", "Circle", 0)
         << QApplication::translate("MainWindow", "Disc", 0)
         << QApplication::translate("MainWindow", "Square", 0)
         << QApplication::translate("MainWindow", "Diamond", 0)
         << QApplication::translate("MainWindow", "Star", 0)
         << QApplication::translate("MainWindow", "Triangle", 0)
         << QApplication::translate("MainWindow", "TriangleInverted", 0)
         << QApplication::translate("MainWindow", "CrossSquare", 0)
         << QApplication::translate("MainWindow", "PlusSquare", 0)
         << QApplication::translate("MainWindow", "CrossCircle", 0)
         << QApplication::translate("MainWindow", "PlusCircle", 0)
         << QApplication::translate("MainWindow", "Peace", 0)
        );
#ifndef QT_NO_TOOLTIP
        butSavePlot->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Save current plot...</p><p>File format chosen by extension (png, jpg, pdf, bmp)</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        butSavePlot->setStatusTip(QApplication::translate("MainWindow", "Save current plot...", 0));
#endif // QT_NO_STATUSTIP
        butSavePlot->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonLoadAllData->setToolTip(QApplication::translate("MainWindow", "Load all data. This has only an effect if not all data has been fetched from the table yet due to the partial fetch mechanism.", 0));
#endif // QT_NO_TOOLTIP
        dockSchema->setWindowTitle(QApplication::translate("MainWindow", "DB Sche&ma", 0));
        dockEditWindow->setWindowTitle(QApplication::translate("MainWindow", "Edit &cell", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
