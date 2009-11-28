#ifndef MAINFORM_H
#define MAINFORM_H

#include <qvariant.h>


#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3ListView>
#include <Qt3Support/Q3MainWindow>
#include <Qt3Support/Q3MimeSourceFactory>
#include <Qt3Support/Q3Table>
#include <Qt3Support/Q3TextEdit>
#include <Qt3Support/Q3ToolBar>
#include <QtCore/QVariant>
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
#include "editform.h"
#include "findform.h"
#include "QtGui/QClipboard"
#include "QtGui/QRegExpValidator"
#include "sqlitedb.h"
#include "sqllogform.h"

QT_BEGIN_NAMESPACE

class Ui_mainForm
{
public:
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
    Q3ListView *dblistView;
    QWidget *browser;
    QVBoxLayout *vboxLayout2;
    QHBoxLayout *hboxLayout;
    QLabel *textLabel1;
    QComboBox *comboBrowseTable;
    QPushButton *buttonFind;
    QSpacerItem *spacer1;
    QPushButton *buttonNewRecord;
    QPushButton *buttonDeleteRecord;
    Q3Table *dataTable;
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
    Q3TextEdit *sqlTextEdit;
    QHBoxLayout *hboxLayout2;
    QPushButton *executeQueryButton;
    QSpacerItem *spacer4_2;
    QLabel *textLabel2;
    QLineEdit *queryErrorLineEdit;
    QLabel *textLabel3;
    Q3ListView *queryResultListView;
    Q3ToolBar *Toolbar;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *PopupMenuEditor_9;
    QMenu *PopupMenuEditor_12;
    QMenu *EditMenu;
    QMenu *ViewMenu;
    QMenu *PopupMenu;

    void setupUi(Q3MainWindow *mainForm)
    {
        if (mainForm->objectName().isEmpty())
            mainForm->setObjectName(QString::fromUtf8("mainForm"));
        mainForm->resize(702, 552);
        fileNewAction = new QAction(mainForm);
        fileNewAction->setObjectName(QString::fromUtf8("fileNewAction"));
        fileNewAction->setName("fileNewAction");
        const QIcon icon = QIcon(qPixmapFromMimeSource("new.png"));
        fileNewAction->setIcon(icon);
        fileOpenAction = new QAction(mainForm);
        fileOpenAction->setObjectName(QString::fromUtf8("fileOpenAction"));
        fileOpenAction->setName("fileOpenAction");
        const QIcon icon1 = QIcon(qPixmapFromMimeSource("open.png"));
        fileOpenAction->setIcon(icon1);
        fileExitAction = new QAction(mainForm);
        fileExitAction->setObjectName(QString::fromUtf8("fileExitAction"));
        fileExitAction->setName("fileExitAction");
        editCopyAction = new QAction(mainForm);
        editCopyAction->setObjectName(QString::fromUtf8("editCopyAction"));
        editCopyAction->setName("editCopyAction");
        editPasteAction = new QAction(mainForm);
        editPasteAction->setObjectName(QString::fromUtf8("editPasteAction"));
        editPasteAction->setName("editPasteAction");
        editFindAction = new QAction(mainForm);
        editFindAction->setObjectName(QString::fromUtf8("editFindAction"));
        editFindAction->setName("editFindAction");
        const QIcon icon2 = QIcon(qPixmapFromMimeSource("searchfind.png"));
        editFindAction->setIcon(icon2);
        helpContentsAction = new QAction(mainForm);
        helpContentsAction->setObjectName(QString::fromUtf8("helpContentsAction"));
        helpContentsAction->setName("helpContentsAction");
        helpIndexAction = new QAction(mainForm);
        helpIndexAction->setObjectName(QString::fromUtf8("helpIndexAction"));
        helpIndexAction->setName("helpIndexAction");
        helpAboutAction = new QAction(mainForm);
        helpAboutAction->setObjectName(QString::fromUtf8("helpAboutAction"));
        helpAboutAction->setName("helpAboutAction");
        fileCloseAction = new QAction(mainForm);
        fileCloseAction->setObjectName(QString::fromUtf8("fileCloseAction"));
        fileCloseAction->setName("fileCloseAction");
        fileCloseAction->setEnabled(false);
        newRecordAction = new QAction(mainForm);
        newRecordAction->setObjectName(QString::fromUtf8("newRecordAction"));
        newRecordAction->setName("newRecordAction");
        fileCompactAction = new QAction(mainForm);
        fileCompactAction->setObjectName(QString::fromUtf8("fileCompactAction"));
        fileCompactAction->setName("fileCompactAction");
        fileCompactAction->setEnabled(false);
        helpWhatsThisAction = new QAction(mainForm);
        helpWhatsThisAction->setObjectName(QString::fromUtf8("helpWhatsThisAction"));
        helpWhatsThisAction->setName("helpWhatsThisAction");
        const QIcon icon3 = QIcon(qPixmapFromMimeSource("whatis.png"));
        helpWhatsThisAction->setIcon(icon3);
        sqlLogAction = new QAction(mainForm);
        sqlLogAction->setObjectName(QString::fromUtf8("sqlLogAction"));
        sqlLogAction->setName("sqlLogAction");
        sqlLogAction->setCheckable(true);
        const QIcon icon4 = QIcon(qPixmapFromMimeSource("log.png"));
        sqlLogAction->setIcon(icon4);
        fileImportCSVAction = new QAction(mainForm);
        fileImportCSVAction->setObjectName(QString::fromUtf8("fileImportCSVAction"));
        fileImportCSVAction->setName("fileImportCSVAction");
        fileExportCSVAction = new QAction(mainForm);
        fileExportCSVAction->setObjectName(QString::fromUtf8("fileExportCSVAction"));
        fileExportCSVAction->setName("fileExportCSVAction");
        fileSaveAction = new QAction(mainForm);
        fileSaveAction->setObjectName(QString::fromUtf8("fileSaveAction"));
        fileSaveAction->setName("fileSaveAction");
        fileSaveAction->setEnabled(false);
        const QIcon icon5 = QIcon(qPixmapFromMimeSource("save.png"));
        fileSaveAction->setIcon(icon5);
        fileRevertAction = new QAction(mainForm);
        fileRevertAction->setObjectName(QString::fromUtf8("fileRevertAction"));
        fileRevertAction->setName("fileRevertAction");
        fileRevertAction->setEnabled(false);
        const QIcon icon6 = QIcon(qPixmapFromMimeSource("revert.png"));
        fileRevertAction->setIcon(icon6);
        //fileImportAction = new QAction(mainForm);
        //fileImportAction->setObjectName(QString::fromUtf8("fileImportAction"));
        //fileImportAction->setName("fileImportAction");
        //fileExportAction = new QAction(mainForm);
        //fileExportAction->setObjectName(QString::fromUtf8("fileExportAction"));
        //fileExportAction->setName("fileExportAction");
        editCreateTableAction = new QAction(mainForm);
        editCreateTableAction->setObjectName(QString::fromUtf8("editCreateTableAction"));
        editCreateTableAction->setName("editCreateTableAction");
        editCreateTableAction->setEnabled(false);
        const QIcon icon7 = QIcon(qPixmapFromMimeSource("create_table.png"));
        editCreateTableAction->setIcon(icon7);
        editDeleteTableAction = new QAction(mainForm);
        editDeleteTableAction->setObjectName(QString::fromUtf8("editDeleteTableAction"));
        editDeleteTableAction->setName("editDeleteTableAction");
        editDeleteTableAction->setEnabled(false);
        const QIcon icon8 = QIcon(qPixmapFromMimeSource("delete_table.png"));
        editDeleteTableAction->setIcon(icon8);
        editModifyTableAction = new QAction(mainForm);
        editModifyTableAction->setObjectName(QString::fromUtf8("editModifyTableAction"));
        editModifyTableAction->setName("editModifyTableAction");
        editModifyTableAction->setEnabled(false);
        const QIcon icon9 = QIcon(qPixmapFromMimeSource("modify_table.png"));
        editModifyTableAction->setIcon(icon9);
        editCreateIndexAction = new QAction(mainForm);
        editCreateIndexAction->setObjectName(QString::fromUtf8("editCreateIndexAction"));
        editCreateIndexAction->setName("editCreateIndexAction");
        editCreateIndexAction->setEnabled(false);
        const QIcon icon10 = QIcon(qPixmapFromMimeSource("create_index.png"));
        editCreateIndexAction->setIcon(icon10);
        editDeleteIndexAction = new QAction(mainForm);
        editDeleteIndexAction->setObjectName(QString::fromUtf8("editDeleteIndexAction"));
        editDeleteIndexAction->setName("editDeleteIndexAction");
        editDeleteIndexAction->setEnabled(false);
        const QIcon icon11 = QIcon(qPixmapFromMimeSource("delete_index.png"));
        editDeleteIndexAction->setIcon(icon11);
        fileImportSQLAction = new QAction(mainForm);
        fileImportSQLAction->setObjectName(QString::fromUtf8("fileImportSQLAction"));
        fileImportSQLAction->setName("fileImportSQLAction");
        fileExportSQLAction = new QAction(mainForm);
        fileExportSQLAction->setObjectName(QString::fromUtf8("fileExportSQLAction"));
        fileExportSQLAction->setName("fileExportSQLAction");
        editPreferencesAction = new QAction(mainForm);
        editPreferencesAction->setObjectName(QString::fromUtf8("editPreferencesAction"));
        editPreferencesAction->setName("editPreferencesAction");
        widget = new QWidget(mainForm);
        widget->setObjectName(QString::fromUtf8("widget"));
        vboxLayout = new QVBoxLayout(widget);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        mainTab = new QTabWidget(widget);
        mainTab->setObjectName(QString::fromUtf8("mainTab"));
        structure = new QWidget();
        structure->setObjectName(QString::fromUtf8("structure"));
        vboxLayout1 = new QVBoxLayout(structure);
        vboxLayout1->setSpacing(6);
        vboxLayout1->setContentsMargins(11, 11, 11, 11);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        dblistView = new Q3ListView(structure);
        dblistView->addColumn(QApplication::translate("mainForm", "Name", 0, QApplication::UnicodeUTF8));
        dblistView->header()->setClickEnabled(false, dblistView->header()->count() - 1);
        dblistView->header()->setResizeEnabled(true, dblistView->header()->count() - 1);
        dblistView->addColumn(QApplication::translate("mainForm", "Object", 0, QApplication::UnicodeUTF8));
        dblistView->header()->setClickEnabled(false, dblistView->header()->count() - 1);
        dblistView->header()->setResizeEnabled(true, dblistView->header()->count() - 1);
        dblistView->addColumn(QApplication::translate("mainForm", "Type", 0, QApplication::UnicodeUTF8));
        dblistView->header()->setClickEnabled(false, dblistView->header()->count() - 1);
        dblistView->header()->setResizeEnabled(true, dblistView->header()->count() - 1);
        dblistView->addColumn(QApplication::translate("mainForm", "Schema", 0, QApplication::UnicodeUTF8));
        dblistView->header()->setClickEnabled(false, dblistView->header()->count() - 1);
        dblistView->header()->setResizeEnabled(true, dblistView->header()->count() - 1);
        dblistView->setObjectName(QString::fromUtf8("dblistView"));
        dblistView->setResizePolicy(Q3ScrollView::Manual);
        dblistView->setSelectionMode(Q3ListView::NoSelection);
        dblistView->setRootIsDecorated(true);
        dblistView->setResizeMode(Q3ListView::LastColumn);

        vboxLayout1->addWidget(dblistView);

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
        buttonFind->setIcon(QPixmap(qPixmapFromMimeSource("searchfind.png")));
        buttonFind->setCheckable(true);

        hboxLayout->addWidget(buttonFind);

        spacer1 = new QSpacerItem(51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer1);

        buttonNewRecord = new QPushButton(browser);
        buttonNewRecord->setObjectName(QString::fromUtf8("buttonNewRecord"));

        hboxLayout->addWidget(buttonNewRecord);

        buttonDeleteRecord = new QPushButton(browser);
        buttonDeleteRecord->setObjectName(QString::fromUtf8("buttonDeleteRecord"));

        hboxLayout->addWidget(buttonDeleteRecord);


        vboxLayout2->addLayout(hboxLayout);

        dataTable = new Q3Table(browser);
        dataTable->setObjectName(QString::fromUtf8("dataTable"));
        dataTable->setAcceptDrops(true);
        dataTable->setResizePolicy(Q3Table::Default);
        dataTable->setVScrollBarMode(Q3Table::Auto);
        dataTable->setNumRows(0);
        dataTable->setNumCols(0);
        dataTable->setReadOnly(true);
        dataTable->setSelectionMode(Q3Table::Single);
        dataTable->setFocusStyle(Q3Table::FollowStyle);

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

        sqlTextEdit = new Q3TextEdit(query);
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

        vboxLayout3->addWidget(queryErrorLineEdit);

        textLabel3 = new QLabel(query);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setWordWrap(false);

        vboxLayout3->addWidget(textLabel3);

        queryResultListView = new Q3ListView(query);
        queryResultListView->setObjectName(QString::fromUtf8("queryResultListView"));
        queryResultListView->setResizePolicy(Q3ScrollView::Manual);
        queryResultListView->setSelectionMode(Q3ListView::NoSelection);
        queryResultListView->setResizeMode(Q3ListView::AllColumns);

        vboxLayout3->addWidget(queryResultListView);

        mainTab->addTab(query, QString());

        vboxLayout->addWidget(mainTab);

        mainForm->setCentralWidget(widget);
        Toolbar = new Q3ToolBar(mainForm);
        Toolbar->setObjectName(QString::fromUtf8("Toolbar"));
        menubar = new QMenuBar(mainForm);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        fileMenu = new QMenu(menubar);
        fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
        PopupMenuEditor_9 = new QMenu(fileMenu);
        PopupMenuEditor_9->setObjectName(QString::fromUtf8("PopupMenuEditor_9"));
        PopupMenuEditor_12 = new QMenu(fileMenu);
        PopupMenuEditor_12->setObjectName(QString::fromUtf8("PopupMenuEditor_12"));
        EditMenu = new QMenu(menubar);
        EditMenu->setObjectName(QString::fromUtf8("EditMenu"));
        ViewMenu = new QMenu(menubar);
        ViewMenu->setObjectName(QString::fromUtf8("ViewMenu"));
        PopupMenu = new QMenu(menubar);
        PopupMenu->setObjectName(QString::fromUtf8("PopupMenu"));

        Toolbar->addAction(fileNewAction);
        Toolbar->addAction(fileOpenAction);
        Toolbar->addAction(fileSaveAction);
        Toolbar->addAction(fileRevertAction);
        Toolbar->addSeparator();
        Toolbar->addAction(editCreateTableAction);
        Toolbar->addAction(editDeleteTableAction);
        Toolbar->addAction(editModifyTableAction);
        Toolbar->addAction(editCreateIndexAction);
        Toolbar->addAction(editDeleteIndexAction);
        Toolbar->addSeparator();
        Toolbar->addAction(sqlLogAction);
        Toolbar->addSeparator();
        Toolbar->addAction(helpWhatsThisAction);
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
        fileMenu->addAction(PopupMenuEditor_9->menuAction());
        //fileMenu->addAction(fileExportAction);
        fileMenu->addAction(PopupMenuEditor_12->menuAction());
        fileMenu->addSeparator();
        fileMenu->addAction(fileExitAction);
        PopupMenuEditor_9->addAction(fileImportSQLAction);
        PopupMenuEditor_9->addAction(fileImportCSVAction);
        PopupMenuEditor_12->addAction(fileExportSQLAction);
        PopupMenuEditor_12->addAction(fileExportCSVAction);
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
        QObject::connect(fileCompactAction, SIGNAL(activated()), mainForm, SLOT(compact()));
        QObject::connect(editCopyAction, SIGNAL(activated()), mainForm, SLOT(copy()));
        QObject::connect(editPasteAction, SIGNAL(activated()), mainForm, SLOT(paste()));
        QObject::connect(helpWhatsThisAction, SIGNAL(activated()), mainForm, SLOT(helpWhatsThis()));
        QObject::connect(helpAboutAction, SIGNAL(activated()), mainForm, SLOT(helpAbout()));
        QObject::connect(dataTable, SIGNAL(doubleClicked(int,int,int,QPoint)), mainForm, SLOT(doubleClickTable(int,int,int,QPoint)));
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
        QObject::connect(fileExportSQLAction, SIGNAL(activated()), mainForm, SLOT(exportDatabaseToSQL()));
        QObject::connect(fileImportSQLAction, SIGNAL(activated()), mainForm, SLOT(importDatabaseFromSQL()));
        QObject::connect(editPreferencesAction, SIGNAL(activated()), mainForm, SLOT(openPreferences()));

        QMetaObject::connectSlotsByName(mainForm);
    } // setupUi

    void retranslateUi(Q3MainWindow *mainForm)
    {
        mainForm->setWindowTitle(QApplication::translate("mainForm", "Browser", 0, QApplication::UnicodeUTF8));
        fileNewAction->setIconText(QApplication::translate("mainForm", "New Database", 0, QApplication::UnicodeUTF8));
        fileNewAction->setText(QApplication::translate("mainForm", "&New Database", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileNewAction->setToolTip(QApplication::translate("mainForm", "Create a new database file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileNewAction->setWhatsThis(QApplication::translate("mainForm", "This option is used to create a new database file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        fileNewAction->setShortcut(QApplication::translate("mainForm", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setIconText(QApplication::translate("mainForm", "Open Database", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setText(QApplication::translate("mainForm", "&Open Database", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileOpenAction->setToolTip(QApplication::translate("mainForm", "Open an existing database file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileOpenAction->setWhatsThis(QApplication::translate("mainForm", "This option is used to open an existing database file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        fileOpenAction->setShortcut(QApplication::translate("mainForm", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        fileExitAction->setIconText(QApplication::translate("mainForm", "Exit", 0, QApplication::UnicodeUTF8));
        fileExitAction->setText(QApplication::translate("mainForm", "E&xit", 0, QApplication::UnicodeUTF8));
        fileExitAction->setShortcut(QString());
        editCopyAction->setIconText(QApplication::translate("mainForm", "Copy", 0, QApplication::UnicodeUTF8));
        editCopyAction->setText(QApplication::translate("mainForm", "&Copy", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        editCopyAction->setWhatsThis(QApplication::translate("mainForm", "Copies the selected text to the clipboard", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        editCopyAction->setShortcut(QApplication::translate("mainForm", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        editPasteAction->setIconText(QApplication::translate("mainForm", "Paste", 0, QApplication::UnicodeUTF8));
        editPasteAction->setText(QApplication::translate("mainForm", "&Paste", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        editPasteAction->setWhatsThis(QApplication::translate("mainForm", "Pastes text from the clipboard into the current text insertion point", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        editPasteAction->setShortcut(QApplication::translate("mainForm", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        editFindAction->setIconText(QApplication::translate("mainForm", "Find", 0, QApplication::UnicodeUTF8));
        editFindAction->setText(QApplication::translate("mainForm", "&Find...", 0, QApplication::UnicodeUTF8));
        editFindAction->setShortcut(QApplication::translate("mainForm", "F", 0, QApplication::UnicodeUTF8));
        helpContentsAction->setIconText(QApplication::translate("mainForm", "Contents", 0, QApplication::UnicodeUTF8));
        helpContentsAction->setText(QApplication::translate("mainForm", "&Contents...", 0, QApplication::UnicodeUTF8));
        helpContentsAction->setShortcut(QString());
        helpIndexAction->setIconText(QApplication::translate("mainForm", "Index", 0, QApplication::UnicodeUTF8));
        helpIndexAction->setText(QApplication::translate("mainForm", "&Index...", 0, QApplication::UnicodeUTF8));
        helpIndexAction->setShortcut(QString());
        helpAboutAction->setIconText(QApplication::translate("mainForm", "About", 0, QApplication::UnicodeUTF8));
        helpAboutAction->setText(QApplication::translate("mainForm", "&About", 0, QApplication::UnicodeUTF8));
        helpAboutAction->setShortcut(QString());
        fileCloseAction->setIconText(QApplication::translate("mainForm", "Close Database", 0, QApplication::UnicodeUTF8));
        fileCloseAction->setText(QApplication::translate("mainForm", "Close Database", 0, QApplication::UnicodeUTF8));
        newRecordAction->setIconText(QApplication::translate("mainForm", "New Record", 0, QApplication::UnicodeUTF8));
        newRecordAction->setText(QApplication::translate("mainForm", "New Record", 0, QApplication::UnicodeUTF8));
        fileCompactAction->setIconText(QApplication::translate("mainForm", "Compact Database", 0, QApplication::UnicodeUTF8));
        fileCompactAction->setText(QApplication::translate("mainForm", "Compact Database", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        fileCompactAction->setStatusTip(QApplication::translate("mainForm", "Compact the database file, removing space wasted by deleted records.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        fileCompactAction->setWhatsThis(QApplication::translate("mainForm", "Compact the database file, removing space wasted by deleted records.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        helpWhatsThisAction->setIconText(QApplication::translate("mainForm", "What's This?", 0, QApplication::UnicodeUTF8));
        helpWhatsThisAction->setText(QApplication::translate("mainForm", "What's This?", 0, QApplication::UnicodeUTF8));
        helpWhatsThisAction->setShortcut(QApplication::translate("mainForm", "Shift+F1", 0, QApplication::UnicodeUTF8));
        sqlLogAction->setIconText(QApplication::translate("mainForm", "SQL Log", 0, QApplication::UnicodeUTF8));
        sqlLogAction->setText(QApplication::translate("mainForm", "SQL Log", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        sqlLogAction->setWhatsThis(QApplication::translate("mainForm", "Display or hide the SQL log window, which stores all SQL commands issued by the user or the application.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        fileImportCSVAction->setIconText(QApplication::translate("mainForm", "Table from CSV file", 0, QApplication::UnicodeUTF8));
        fileImportCSVAction->setText(QApplication::translate("mainForm", "Table from CSV file", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileImportCSVAction->setToolTip(QApplication::translate("mainForm", "Open a wizard that lets you import data from a comma separated text file into a database table.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileImportCSVAction->setWhatsThis(QApplication::translate("mainForm", "Open a wizard that lets you import data from a comma separated text file into a database table. CSV files can be created on most database and spreadsheet applications.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        fileExportCSVAction->setIconText(QApplication::translate("mainForm", "Table as CSV file", 0, QApplication::UnicodeUTF8));
        fileExportCSVAction->setText(QApplication::translate("mainForm", "Table as CSV file", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileExportCSVAction->setToolTip(QApplication::translate("mainForm", "Export a database table as a comma separated text file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileExportCSVAction->setWhatsThis(QApplication::translate("mainForm", "Export a database table as a comma separated text file, ready to be imported into other database or spreadsheet applications.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        fileSaveAction->setIconText(QApplication::translate("mainForm", "Save Database", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setText(QApplication::translate("mainForm", "Save Database", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileSaveAction->setToolTip(QApplication::translate("mainForm", "Save changes to the database file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileSaveAction->setWhatsThis(QApplication::translate("mainForm", "This option is used to save changes to the database file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        fileRevertAction->setIconText(QApplication::translate("mainForm", "Revert Database", 0, QApplication::UnicodeUTF8));
        fileRevertAction->setText(QApplication::translate("mainForm", "Revert Database", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileRevertAction->setToolTip(QApplication::translate("mainForm", "Revert database to last saved state", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileRevertAction->setWhatsThis(QApplication::translate("mainForm", "This option is used to revert the current database file to its last saved state. All changes made since the last save operation are lost.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        //fileImportAction->setIconText(QApplication::translate("mainForm", "Import", 0, QApplication::UnicodeUTF8));
        //fileImportAction->setText(QApplication::translate("mainForm", "Import", 0, QApplication::UnicodeUTF8));
        //fileExportAction->setIconText(QApplication::translate("mainForm", "Export", 0, QApplication::UnicodeUTF8));
        //fileExportAction->setText(QApplication::translate("mainForm", "Export", 0, QApplication::UnicodeUTF8));
        editCreateTableAction->setIconText(QApplication::translate("mainForm", "Create Table", 0, QApplication::UnicodeUTF8));
        editCreateTableAction->setText(QApplication::translate("mainForm", "Create Table", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        editCreateTableAction->setWhatsThis(QApplication::translate("mainForm", "Open the Create Table wizard, where it is possible to define the name and fields for a new table in the database", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        editDeleteTableAction->setIconText(QApplication::translate("mainForm", "Delete Table", 0, QApplication::UnicodeUTF8));
        editDeleteTableAction->setText(QApplication::translate("mainForm", "Delete Table", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        editDeleteTableAction->setWhatsThis(QApplication::translate("mainForm", "Open the Delete Table wizard, where you can select a database table to be dropped.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        editModifyTableAction->setIconText(QApplication::translate("mainForm", "Modify Table", 0, QApplication::UnicodeUTF8));
        editModifyTableAction->setText(QApplication::translate("mainForm", "Modify Table", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        editModifyTableAction->setWhatsThis(QApplication::translate("mainForm", "Open the Modify Table wizard, where it is possible to rename an existing table. It is also possible to add or delete fields form a table, as well as modify field names and types.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        editCreateIndexAction->setIconText(QApplication::translate("mainForm", "Create Index", 0, QApplication::UnicodeUTF8));
        editCreateIndexAction->setText(QApplication::translate("mainForm", "Create Index", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        editCreateIndexAction->setWhatsThis(QApplication::translate("mainForm", "Open the Create Index wizard, where it is possible to define a new index on an existing database table.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        editDeleteIndexAction->setIconText(QApplication::translate("mainForm", "Delete Index", 0, QApplication::UnicodeUTF8));
        editDeleteIndexAction->setText(QApplication::translate("mainForm", "Delete Index", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        editDeleteIndexAction->setWhatsThis(QApplication::translate("mainForm", "Open the Delete Index wizard, where you can select a database index to be dropped.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        fileImportSQLAction->setIconText(QApplication::translate("mainForm", "Database from SQL file", 0, QApplication::UnicodeUTF8));
        fileImportSQLAction->setText(QApplication::translate("mainForm", "Database from SQL file", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileImportSQLAction->setToolTip(QApplication::translate("mainForm", "Import data from an .sql dump text file into a new or existing database.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileImportSQLAction->setWhatsThis(QApplication::translate("mainForm", "This option lets you import data from an .sql dump text file into a new or existing database. SQL dump files can be created on most database engines, including MySQL and PostgreSQL.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        fileExportSQLAction->setIconText(QApplication::translate("mainForm", "Database to SQL file", 0, QApplication::UnicodeUTF8));
        fileExportSQLAction->setText(QApplication::translate("mainForm", "Database to SQL file", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileExportSQLAction->setToolTip(QApplication::translate("mainForm", "Export a database to a .sql dump text file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        fileExportSQLAction->setWhatsThis(QApplication::translate("mainForm", "This option lets you export a database to a .sql dump text file. SQL dump files contain all data necessary to recreate the database on most database engines, including MySQL and PostgreSQL.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        editPreferencesAction->setIconText(QApplication::translate("mainForm", "Preferences", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        editPreferencesAction->setToolTip(QApplication::translate("mainForm", "Open the preferences window..", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        editPreferencesAction->setWhatsThis(QApplication::translate("mainForm", "Open the preferences window.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        mainTab->setProperty("toolTip", QVariant(QString()));
#endif // QT_NO_TOOLTIP
        dblistView->header()->setLabel(0, QApplication::translate("mainForm", "Name", 0, QApplication::UnicodeUTF8));
        dblistView->header()->setLabel(1, QApplication::translate("mainForm", "Object", 0, QApplication::UnicodeUTF8));
        dblistView->header()->setLabel(2, QApplication::translate("mainForm", "Type", 0, QApplication::UnicodeUTF8));
        dblistView->header()->setLabel(3, QApplication::translate("mainForm", "Schema", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        dblistView->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "This area shows the structure of your database, including all tables and indexes.", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        mainTab->setTabText(mainTab->indexOf(structure), QApplication::translate("mainForm", "Database Structure", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("mainForm", "Table:", 0, QApplication::UnicodeUTF8));
        comboBrowseTable->clear();
        comboBrowseTable->insertItems(0, QStringList()
         << QApplication::translate("mainForm", "select", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainForm", "users", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        comboBrowseTable->setProperty("toolTip", QVariant(QApplication::translate("mainForm", "Select a table to browse data", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        comboBrowseTable->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "Use this list to select a table to be displayed in the database view", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        buttonFind->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonFind->setProperty("toolTip", QVariant(QApplication::translate("mainForm", "Open or close the floating find window", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonFind->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "This button toggles the appearance of the Find window, used to search records in the database view", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        buttonNewRecord->setText(QApplication::translate("mainForm", "New Record", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonNewRecord->setProperty("toolTip", QVariant(QApplication::translate("mainForm", "Insert a new record in the current table", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonNewRecord->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "This button creates a new, empty record in the database", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        buttonDeleteRecord->setText(QApplication::translate("mainForm", "Delete Record", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonDeleteRecord->setProperty("toolTip", QVariant(QApplication::translate("mainForm", "Delete the current record", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonDeleteRecord->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "This button deletes the record currently selected in the database", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        dataTable->setProperty("toolTip", QVariant(QString()));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        dataTable->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "This is the database view. You can double-click any record to edit its contents in the cell editor window.", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        buttonPrevious->setText(QApplication::translate("mainForm", "<", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonPrevious->setProperty("toolTip", QVariant(QApplication::translate("mainForm", "Go to previous record set page", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonPrevious->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "This button is used to navigate to the previous set of 1000 records in the database view", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        labelRecordset->setText(QApplication::translate("mainForm", "1000 - 2000 of 100000", 0, QApplication::UnicodeUTF8));
        buttonNext->setText(QApplication::translate("mainForm", ">", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonNext->setProperty("toolTip", QVariant(QApplication::translate("mainForm", "Go to next record set page", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonNext->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "This button is used to navigate to the next 1000 records set in the database view", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        buttonGoto->setText(QApplication::translate("mainForm", "Go to:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonGoto->setProperty("toolTip", QVariant(QApplication::translate("mainForm", "Click here to browse the specified record ", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonGoto->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "This button is used to navigate to the record number specied in the Go to: area.", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        editGoto->setText(QApplication::translate("mainForm", "0", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        editGoto->setProperty("toolTip", QVariant(QApplication::translate("mainForm", "Enter record number to browse", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        editGoto->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "Type a record number in this area and click the Go to: button to display the record in the database view", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        mainTab->setTabText(mainTab->indexOf(browser), QApplication::translate("mainForm", "Browse Data", 0, QApplication::UnicodeUTF8));
        textLabel1_2->setText(QApplication::translate("mainForm", "SQL string:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        sqlTextEdit->setProperty("toolTip", QVariant(QApplication::translate("mainForm", "Type SQL string here", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        sqlTextEdit->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "Type the SQL query in this area, and press the button below to execute it. Results and error messages will be shown in the fields at the bottom of this form.", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        executeQueryButton->setText(QApplication::translate("mainForm", "Execute query", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        executeQueryButton->setProperty("toolTip", QVariant(QApplication::translate("mainForm", "Execute SQL query", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        executeQueryButton->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "This button is used to execute the SQL query entered in the text area above.", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        textLabel2->setText(QApplication::translate("mainForm", "Error message from database engine:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        queryErrorLineEdit->setProperty("toolTip", QVariant(QApplication::translate("mainForm", "Error messages", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        queryErrorLineEdit->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "Messages returned from the database engine when the SQL query is executed are displayed in this area.", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        textLabel3->setText(QApplication::translate("mainForm", "Data returned:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        queryResultListView->setProperty("toolTip", QVariant(QApplication::translate("mainForm", "Query generated data", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        queryResultListView->setProperty("whatsThis", QVariant(QApplication::translate("mainForm", "This table displays data returned from the database engine as a result of the SQL query. You can not modify data directly on this view, only consult it.", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        mainTab->setTabText(mainTab->indexOf(query), QApplication::translate("mainForm", "Execute SQL", 0, QApplication::UnicodeUTF8));
        Toolbar->setLabel(QApplication::translate("mainForm", "Toolbar", 0, QApplication::UnicodeUTF8));
        fileMenu->setTitle(QApplication::translate("mainForm", "&File", 0, QApplication::UnicodeUTF8));
        PopupMenuEditor_9->setTitle(QApplication::translate("mainForm", "Import", 0, QApplication::UnicodeUTF8));
        PopupMenuEditor_12->setTitle(QApplication::translate("mainForm", "Export", 0, QApplication::UnicodeUTF8));
        EditMenu->setTitle(QApplication::translate("mainForm", "Edit", 0, QApplication::UnicodeUTF8));
        ViewMenu->setTitle(QApplication::translate("mainForm", "View", 0, QApplication::UnicodeUTF8));
        PopupMenu->setTitle(QApplication::translate("mainForm", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class mainForm: public Ui_mainForm {};
} // namespace Ui

QT_END_NAMESPACE

class mainForm : public Q3MainWindow, public Ui::mainForm
{
    Q_OBJECT

public:
    mainForm(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~mainForm();

    sqlLogForm * logWin;
    editForm * editWin;
    QClipboard * clipboard;
    findForm * findWin;
    int recAtTop;
    int recsPerView;
    QIntValidator * gotoValidator;
    QString defaultlocation;

public slots:
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
    virtual void lookfor( const QString & wfield, const QString & woperator, const QString & wsearchterm );
    virtual void showrecord( int dec );
    virtual void createTable();
    virtual void createIndex();
    virtual void compact();
    virtual void deleteTable();
    virtual void editTable();
    virtual void deleteIndex();
    virtual void copy();
    virtual void paste();
    virtual void helpWhatsThis();
    virtual void helpAbout();
    virtual void updateRecordText( int row, int col, QString newtext );
    virtual void logWinAway();
    virtual void editWinAway();
    virtual void editText( int row, int col );
    virtual void doubleClickTable( int row, int col, int button, const QPoint & mousepoint );
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

protected:
    DBBrowserDB db;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // MAINFORM_H
