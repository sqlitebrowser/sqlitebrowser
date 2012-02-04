#ifndef MAINFORM_H
#define MAINFORM_H

#include <qvariant.h>

/* QT_BEGIN_NAMESPACE and QT_END_NAMESPACE weren't introduced into QT right away... */
#ifndef QT_BEGIN_NAMESPACE
#define QT_BEGIN_NAMESPACE
#endif

#ifndef QT_END_NAMESPACE
#define QT_END_NAMESPACE
#endif

#include <QTableView>
#include <QStandardItemModel>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QHeaderView>

#include <QtGui/QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QtGui/QToolBar>

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

#include <QtCore/QPoint>
#include <QtCore/QVariant>

#include "QtGui/QClipboard"
#include "QtGui/QRegExpValidator"
#include "sqlitedb.h"

#include "SQLLogDock.h"

QT_BEGIN_NAMESPACE

class Ui_mainForm
{
public:
    SQLLogDock * logWin;

    QAction *fileNewAction;
    QAction *fileOpenAction;
    QAction *fileExitAction;
    QAction *editCopyAction;
    QAction *editPasteAction;
    QAction *editFindAction;
    QAction *helpContentsAction;
    QAction *helpIndexAction;
    QAction *helpAboutAction;
    QAction *fileCloseAction;
    QAction *newRecordAction;
    QAction *fileCompactAction;
    QAction *helpWhatsThisAction;
    QAction *sqlLogAction;
    QAction *fileImportCSVAction;
    QAction *fileExportCSVAction;
    QAction *fileSaveAction;
    QAction *fileRevertAction;
    //QAction *fileImportAction;
    //QAction *fileExportAction;
    QAction *editCreateTableAction;
    QAction *editDeleteTableAction;
    QAction *editModifyTableAction;
    QAction *editCreateIndexAction;
    QAction *editDeleteIndexAction;
    QAction *fileImportSQLAction;
    QAction *fileExportSQLAction;
    QAction *editPreferencesAction;
    QWidget *widget;
    QVBoxLayout *vboxLayout;
    QTabWidget *mainTab;
    QWidget *structure;
    QVBoxLayout *vboxLayout1;

    QTreeWidget *dbTreeWidget;
    QMenu *popupTableMenu;
    QMenu *popupFieldMenu;

    QAction *editModifyTableActionPopup;
    QAction *editDeleteTableActionPopup;
    QAction *editAddFieldActionPopup;
    QAction *editModifyFieldActionPopup;
    QAction *editDeleteFieldActionPopup;

    QWidget *browser;
    QVBoxLayout *vboxLayout2;
    QHBoxLayout *hboxLayout;
    QLabel *textLabel1;
    QComboBox *comboBrowseTable;
    QPushButton *buttonFind;
    QPushButton *buttonRefresh;
    QSpacerItem *spacer1;
    QPushButton *buttonNewRecord;
    QPushButton *buttonDeleteRecord;
    QTableWidget *dataTable;
    QHBoxLayout *hboxLayout1;
    QPushButton *buttonPrevious;
    QLabel *labelRecordset;
    QPushButton *buttonNext;
    QSpacerItem *spacer4;
    QPushButton *buttonGoto;
    QLineEdit *editGoto;
    QWidget *query;
    QVBoxLayout *vboxLayout3;
    QLabel *textLabel1_2;
    QTextEdit *sqlTextEdit;
    QHBoxLayout *hboxLayout2;
    QPushButton *executeQueryButton;
    QSpacerItem *spacer4_2;
    QLabel *textLabel2;
    QLineEdit *queryErrorLineEdit;
    QLabel *textLabel3;
    QTableView *queryResultTableView;
    QStandardItemModel *queryResultListModel;
    QToolBar *Toolbar;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *importMenu;
    QMenu *exportMenu;
    QMenu *EditMenu;
    QMenu *ViewMenu;
    QMenu *PopupMenu;
    QMenu *recentFilesMenu;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QAction *recentSeparatorAct;

    void setupUi(QMainWindow *mainForm)
    {
        if (mainForm->objectName().isEmpty()){
            mainForm->setObjectName(QString::fromUtf8("mainForm"));
        }
        //TODO get from settings and save last position
        mainForm->resize(800, 552);


        logWin = new SQLLogDock(mainForm);
        sqlLogAction = logWin->toggleViewAction();
        mainForm->addDockWidget(Qt::BottomDockWidgetArea, logWin);

        //** New DB File
        fileNewAction = new QAction(mainForm);
        fileNewAction->setObjectName(QString::fromUtf8("fileNewAction"));
        fileNewAction->setText("fileNewAction");
        fileNewAction->setIcon(QIcon(":/icons/db_new"));

        //** Open DB File
        fileOpenAction = new QAction(mainForm);
        fileOpenAction->setObjectName(QString::fromUtf8("fileOpenAction"));
        fileOpenAction->setText("fileOpenAction");
        fileOpenAction->setIcon(QIcon(":/icons/db_open"));

        //** Save DB Action
        fileSaveAction = new QAction(mainForm);
        fileSaveAction->setObjectName(QString::fromUtf8("fileSaveAction"));
        fileSaveAction->setText("fileSaveAction");
        fileSaveAction->setEnabled(false);
        fileSaveAction->setIcon(QIcon(":/icons/db_save"));
        QFont fntS = fileSaveAction->font();
        fntS.setBold(true);
        fileSaveAction->setFont(fntS);

        //** Db Revert
        fileRevertAction = new QAction(mainForm);
        fileRevertAction->setObjectName(QString::fromUtf8("fileRevertAction"));
        fileRevertAction->setText("fileRevertAction");
        fileRevertAction->setEnabled(false);
        fileRevertAction->setIcon(QIcon(":/icons/db_revert"));
        QFont fntR = fileRevertAction->font();
        fntR.setBold(true);
        fileRevertAction->setFont(fntR);

        // Recent dbs
        for(int i = 0; i < MaxRecentFiles; ++i) {
            recentFileActs[i] = new QAction(mainForm);
            recentFileActs[i]->setVisible(false);
            mainForm->connect(recentFileActs[i], SIGNAL(triggered()), mainForm, SLOT(openRecentFile()));
        }


        //** Exit
        fileExitAction = new QAction(mainForm);
        fileExitAction->setObjectName(QString::fromUtf8("fileExitAction"));
        fileExitAction->setText("fileExitAction");

        editCopyAction = new QAction(mainForm);
        editCopyAction->setObjectName(QString::fromUtf8("editCopyAction"));
        editCopyAction->setText("editCopyAction");
        editPasteAction = new QAction(mainForm);
        editPasteAction->setObjectName(QString::fromUtf8("editPasteAction"));
        editPasteAction->setText("editPasteAction");
        editFindAction = new QAction(mainForm);
        editFindAction->setObjectName(QString::fromUtf8("editFindAction"));
        editFindAction->setText("editFindAction");
        editFindAction->setIcon(QIcon(":/oldimages/searchfind"));
        editFindAction->setIconVisibleInMenu(true);
        helpContentsAction = new QAction(mainForm);
        helpContentsAction->setObjectName(QString::fromUtf8("helpContentsAction"));
        helpContentsAction->setText("helpContentsAction");
        helpIndexAction = new QAction(mainForm);
        helpIndexAction->setObjectName(QString::fromUtf8("helpIndexAction"));
        helpIndexAction->setText("helpIndexAction");
        helpAboutAction = new QAction(mainForm);
        helpAboutAction->setObjectName(QString::fromUtf8("helpAboutAction"));
        helpAboutAction->setText("helpAboutAction");

        //** Close Database ??
        fileCloseAction = new QAction(mainForm);
        fileCloseAction->setObjectName(QString::fromUtf8("fileCloseAction"));
        fileCloseAction->setText("fileCloseAction");
        fileCloseAction->setEnabled(false);

        newRecordAction = new QAction(mainForm);
        newRecordAction->setObjectName(QString::fromUtf8("newRecordAction"));
        newRecordAction->setText("newRecordAction");
        fileCompactAction = new QAction(mainForm);
        fileCompactAction->setObjectName(QString::fromUtf8("fileCompactAction"));
        fileCompactAction->setText("fileCompactAction");
        fileCompactAction->setEnabled(false);
        helpWhatsThisAction = new QAction(mainForm);
        helpWhatsThisAction->setObjectName(QString::fromUtf8("helpWhatsThisAction"));
        helpWhatsThisAction->setText("helpWhatsThisAction");
        helpWhatsThisAction->setIcon(QIcon(":/oldimages/whatis"));
        helpWhatsThisAction->setIconVisibleInMenu(true);

        sqlLogAction->setText("sqlLogAction");
        sqlLogAction->setIcon(QIcon(":/oldimages/log"));
        sqlLogAction->setIconVisibleInMenu(true);


        fileImportCSVAction = new QAction(mainForm);
        fileImportCSVAction->setObjectName(QString::fromUtf8("fileImportCSVAction"));
        fileImportCSVAction->setText("fileImportCSVAction");
        fileExportCSVAction = new QAction(mainForm);
        fileExportCSVAction->setObjectName(QString::fromUtf8("fileExportCSVAction"));
        fileExportCSVAction->setText("fileExportCSVAction");




        //fileImportAction = new QAction(mainForm);
        //fileImportAction->setObjectName(QString::fromUtf8("fileImportAction"));
        //fileImportAction->setName("fileImportAction");
        //fileExportAction = new QAction(mainForm);
        //fileExportAction->setObjectName(QString::fromUtf8("fileExportAction"));
        //fileExportAction->setName("fileExportAction");

        //** Create Table
        editCreateTableAction = new QAction(mainForm);
        editCreateTableAction->setObjectName(QString::fromUtf8("editCreateTableAction"));
        editCreateTableAction->setText("editCreateTableAction");
        editCreateTableAction->setEnabled(false);
        editCreateTableAction->setIcon(QIcon(":/icons/table_create"));

        //** Delete table
        editDeleteTableAction = new QAction(mainForm);
        editDeleteTableAction->setObjectName(QString::fromUtf8("editDeleteTableAction"));
        editDeleteTableAction->setText("editDeleteTableAction");
        editDeleteTableAction->setEnabled(false);
        editDeleteTableAction->setIcon(QIcon(":/icons/table_delete"));

        editDeleteTableActionPopup = new QAction(mainForm);
        editDeleteTableActionPopup->setEnabled(false);
        editDeleteTableActionPopup->setIcon(QIcon(":/icons/table_delete"));

        //** Modify Table
        editModifyTableAction = new QAction(mainForm);
        editModifyTableAction->setObjectName(QString::fromUtf8("editModifyTableAction"));
        editModifyTableAction->setText("editModifyTableAction");
        editModifyTableAction->setEnabled(false);
        editModifyTableAction->setIcon(QIcon(":/icons/table_modify"));

        editModifyTableActionPopup = new QAction(mainForm);
        editModifyTableActionPopup->setEnabled(false);
        editModifyTableActionPopup->setIcon(QIcon(":/icons/table_modify"));


        //************************************************
        //** Add, Modify, Delete Field
        editAddFieldActionPopup = new QAction(mainForm);
        editAddFieldActionPopup->setText("Add Field");
        editAddFieldActionPopup->setDisabled(true);
        editAddFieldActionPopup->setIcon(QIcon(":/icons/field_add"));

        editModifyFieldActionPopup = new QAction(mainForm);
        editModifyFieldActionPopup->setText("Modify Field");
        editModifyFieldActionPopup->setDisabled(true);
        editModifyFieldActionPopup->setIcon(QIcon(":/icons/field_edit"));

        editDeleteFieldActionPopup = new QAction(mainForm);
        editDeleteFieldActionPopup->setText("Delete Field");
        editDeleteFieldActionPopup->setDisabled(true);
        editDeleteFieldActionPopup->setIcon(QIcon(":/icons/field_delete"));

        //************************************************
        //** Create/Delete  Index
        editCreateIndexAction = new QAction(mainForm);
        editCreateIndexAction->setObjectName(QString::fromUtf8("editCreateIndexAction"));
        editCreateIndexAction->setText("editCreateIndexAction");
        editCreateIndexAction->setEnabled(false);
        editCreateIndexAction->setIcon(QIcon(":/icons/index_create"));

        editDeleteIndexAction = new QAction(mainForm);
        editDeleteIndexAction->setObjectName(QString::fromUtf8("editDeleteIndexAction"));
        editDeleteIndexAction->setText("editDeleteIndexAction");
        editDeleteIndexAction->setEnabled(false);
        editDeleteIndexAction->setIcon(QIcon(":/icons/index_delete"));

        fileImportSQLAction = new QAction(mainForm);
        fileImportSQLAction->setObjectName(QString::fromUtf8("fileImportSQLAction"));
        fileImportSQLAction->setText("fileImportSQLAction");
        fileExportSQLAction = new QAction(mainForm);
        fileExportSQLAction->setObjectName(QString::fromUtf8("fileExportSQLAction"));
        fileExportSQLAction->setText("fileExportSQLAction");
        editPreferencesAction = new QAction(mainForm);
        editPreferencesAction->setObjectName(QString::fromUtf8("editPreferencesAction"));
        editPreferencesAction->setText("editPreferencesAction");
        widget = new QWidget(mainForm);
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

        hboxLayout->addWidget(comboBrowseTable);

        buttonFind = new QPushButton(browser);
        buttonFind->setObjectName(QString::fromUtf8("buttonFind"));
        buttonFind->setIcon(QIcon(":/oldimages/searchfind"));
        buttonFind->setCheckable(true);

        hboxLayout->addWidget(buttonFind);

        buttonRefresh = new QPushButton(browser);
        buttonRefresh->setObjectName("buttonRefresh");
        buttonRefresh->setIcon(QIcon(":/icons/refresh"));

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

        vboxLayout3->addWidget(queryResultTableView);

        mainTab->addTab(query, QString());

        vboxLayout->addWidget(mainTab);

        mainForm->setCentralWidget(widget);

        //*** Setup Toolbar
        Toolbar = new QToolBar();
        mainForm->addToolBar(Qt::TopToolBarArea, Toolbar);
        Toolbar->setObjectName(QString::fromUtf8("Toolbar"));
        Toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        //*** Setup main Menu
        menubar = mainForm->menuBar(); //new QMenuBar(mainForm);
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

        Toolbar->addAction(fileNewAction);
        Toolbar->addAction(fileOpenAction);
        Toolbar->addSeparator();

        Toolbar->addAction(fileSaveAction);
        Toolbar->addAction(fileRevertAction);
        Toolbar->addSeparator();

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
        PopupMenu->addAction(helpWhatsThisAction);
        PopupMenu->addAction(helpAboutAction);

        //***********************************************8
        //** Db Tree Popup Menus
        popupTableMenu = new QMenu(mainForm);
        popupTableMenu->addAction(editModifyTableActionPopup);
        popupTableMenu->addAction(editAddFieldActionPopup);
        popupTableMenu->addSeparator();
        popupTableMenu->addAction(editDeleteTableActionPopup);

        popupFieldMenu = new QMenu(mainForm);
        popupFieldMenu->addAction(editModifyFieldActionPopup);
        popupFieldMenu->addAction(editDeleteFieldActionPopup);
        //spopupFieldMenu->addSeparator();
        //popupFieldMenu->addAction(editDeleteTableActionPopup);


        retranslateUi(mainForm);
        QObject::connect(fileExitAction, SIGNAL(activated()), mainForm, SLOT(fileExit()));
        QObject::connect(fileOpenAction, SIGNAL(activated()), mainForm, SLOT(fileOpen()));
        QObject::connect(fileNewAction, SIGNAL(activated()), mainForm, SLOT(fileNew()));
        QObject::connect(fileCloseAction, SIGNAL(activated()), mainForm, SLOT(fileClose()));
        QObject::connect(comboBrowseTable, SIGNAL(activated(QString)), mainForm, SLOT(populateTable(QString)));
        QObject::connect(buttonNewRecord, SIGNAL(clicked()), mainForm, SLOT(addRecord()));
        QObject::connect(buttonDeleteRecord, SIGNAL(clicked()), mainForm, SLOT(deleteRecord()));
        QObject::connect(buttonPrevious, SIGNAL(clicked()), mainForm, SLOT(navigatePrevious()));
        QObject::connect(buttonNext, SIGNAL(clicked()), mainForm, SLOT(navigateNext()));
        QObject::connect(editGoto, SIGNAL(returnPressed()), mainForm, SLOT(navigateGoto()));
        QObject::connect(buttonGoto, SIGNAL(clicked()), mainForm, SLOT(navigateGoto()));
        QObject::connect(buttonFind, SIGNAL(toggled(bool)), mainForm, SLOT(browseFind(bool)));
        QObject::connect(buttonRefresh, SIGNAL(clicked()), mainForm, SLOT(browseRefresh()));
        QObject::connect(fileCompactAction, SIGNAL(activated()), mainForm, SLOT(compact()));
        QObject::connect(editCopyAction, SIGNAL(activated()), mainForm, SLOT(copy()));
        QObject::connect(editPasteAction, SIGNAL(activated()), mainForm, SLOT(paste()));
        QObject::connect(helpWhatsThisAction, SIGNAL(activated()), mainForm, SLOT(helpWhatsThis()));
        QObject::connect(helpAboutAction, SIGNAL(activated()), mainForm, SLOT(helpAbout()));
        QObject::connect(dataTable, SIGNAL(cellDoubleClicked(int,int)), mainForm, SLOT(doubleClickTable(int,int)));
        QObject::connect(mainTab, SIGNAL(selected(QString)), mainForm, SLOT(mainTabSelected(QString)));
        QObject::connect(sqlLogAction, SIGNAL(toggled(bool)), mainForm, SLOT(toggleLogWindow(bool)));
        QObject::connect(executeQueryButton, SIGNAL(clicked()), mainForm, SLOT(executeQuery()));
        QObject::connect(fileImportCSVAction, SIGNAL(activated()), mainForm, SLOT(importTableFromCSV()));
        QObject::connect(fileExportCSVAction, SIGNAL(activated()), mainForm, SLOT(exportTableToCSV()));
        QObject::connect(fileRevertAction, SIGNAL(activated()), mainForm, SLOT(fileRevert()));
        QObject::connect(fileSaveAction, SIGNAL(activated()), mainForm, SLOT(fileSave()));
        QObject::connect(editDeleteIndexAction, SIGNAL(activated()), mainForm, SLOT(deleteIndex()));
        QObject::connect(editCreateIndexAction, SIGNAL(activated()), mainForm, SLOT(createIndex()));
        QObject::connect(editCreateTableAction, SIGNAL(activated()), mainForm, SLOT(createTable()));

        QObject::connect(editDeleteTableAction, SIGNAL(activated()), mainForm, SLOT(deleteTable()));
        QObject::connect(editModifyTableAction, SIGNAL(activated()), mainForm, SLOT(editTable()));
        QObject::connect(editDeleteTableActionPopup, SIGNAL(activated()), mainForm, SLOT(deleteTablePopup()));
        QObject::connect(editModifyTableActionPopup, SIGNAL(activated()), mainForm, SLOT(editTablePopup()));
        QObject::connect(editAddFieldActionPopup, SIGNAL(activated()), mainForm, SLOT(on_add_field()));
        QObject::connect(editModifyFieldActionPopup, SIGNAL(activated()), mainForm, SLOT(on_edit_field()));

        QObject::connect(fileExportSQLAction, SIGNAL(activated()), mainForm, SLOT(exportDatabaseToSQL()));
        QObject::connect(fileImportSQLAction, SIGNAL(activated()), mainForm, SLOT(importDatabaseFromSQL()));
        QObject::connect(editPreferencesAction, SIGNAL(activated()), mainForm, SLOT(openPreferences()));

        QObject::connect(dbTreeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), mainForm, SLOT(on_tree_context_menu(const QPoint &)));
        QObject::connect(dbTreeWidget, SIGNAL(itemSelectionChanged()), mainForm, SLOT(on_tree_selection_changed()));

        QMetaObject::connectSlotsByName(mainForm);
    } // setupUi

    void retranslateUi(QMainWindow *mainForm);

};

namespace Ui {
    class mainForm: public Ui_mainForm {};
} // namespace Ui

QT_END_NAMESPACE

class SQLLogDock;
class editForm;
class findForm;

class mainForm : public QMainWindow, public Ui::mainForm
{
    Q_OBJECT

public:
    mainForm(QWidget* parent = 0);
    ~mainForm();

    editForm * editWin;
    QClipboard * clipboard;
    findForm * findWin;
    int recAtTop;
    int recsPerView;
    QIntValidator * gotoValidator;
    QString defaultlocation;

private:
    void updateRecentFileActions();
    void setCurrentFile(const QString& fileName);

public slots:
    virtual void on_tree_context_menu(const QPoint & qPoint);
    virtual void on_tree_selection_changed();
    virtual void on_add_field();
    virtual void on_edit_field();

    virtual void fileOpen( const QString & fileName );
    virtual void fileOpen();
    virtual void fileNew();
    virtual void populateStructure();
    virtual void populateTable( const QString & tablename );
    virtual void resetBrowser();
    virtual void fileClose();
    virtual void fileExit();
    virtual void closeEvent( QCloseEvent * );
    virtual void addRecord();
    virtual void deleteRecord();
    virtual void updateTableView( int lineToSelect );
    virtual void selectTableLine( int lineToSelect );
    virtual void navigatePrevious();
    virtual void navigateNext();
    virtual void navigateGoto();
    virtual void setRecordsetLabel();
    virtual void browseFind( bool open );
    virtual void browseFindAway();
    virtual void browseRefresh();
    virtual void lookfor( const QString & wfield, const QString & woperator, const QString & wsearchterm );
    virtual void showrecord( int dec );
    virtual void createTable();
    virtual void createIndex();
    virtual void compact();
    virtual void deleteTable();
    virtual void editTable();
    virtual void editTablePopup();
    virtual void deleteIndex();
    virtual void copy();
    virtual void paste();
    virtual void helpWhatsThis();
    virtual void helpAbout();
    virtual void updateRecordText( int row, int col, QString newtext );
    virtual void logWinAway();
    virtual void editWinAway();
    virtual void editText( int row, int col );
    virtual void doubleClickTable( int row, int col );
    virtual void executeQuery();
    virtual void mainTabSelected( const QString & tabname );
    virtual void toggleLogWindow( bool enable );
    virtual void importTableFromCSV();
    virtual void exportTableToCSV();
    virtual void dbState( bool dirty );
    virtual void fileSave();
    virtual void fileRevert();
    virtual void exportDatabaseToSQL();
    virtual void importDatabaseFromSQL();
    virtual void openPreferences();
    virtual void updatePreferences();
    virtual void openRecentFile();

protected:
    DBBrowserDB db;

protected slots:
    virtual void languageChange();



private:
    void init();
    void destroy();

};

#endif // MAINFORM_H
