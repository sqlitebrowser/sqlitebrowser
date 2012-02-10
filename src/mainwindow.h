#ifndef MAINFORM_H
#define MAINFORM_H

#include <QVariant>
#include <QTableView>
#include <QStandardItemModel>
#include <QtGui/QTreeWidget>

#include <QtGui/QMainWindow>
#include <QTableWidget>
#include <QTextEdit>

#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
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
#include <QDragEnterEvent>

#include "sqlitedb.h"

class SQLLogDock;
class editForm;
class findForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
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

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

    editForm * editWin;
    QClipboard * clipboard;
    findForm * findWin;
    int recAtTop;
    int recsPerView;
    QIntValidator * gotoValidator;
    QString defaultlocation;

private:
    void init();
    void destroy();
    void setupUi();
    void retranslateUi();

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

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

protected slots:
    virtual void languageChange();

};

#endif // MAINFORM_H
