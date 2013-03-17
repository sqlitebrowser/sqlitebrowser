#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include "sqlitedb.h"

#define ORDERMODE_ASC 0
#define ORDERMODE_DESC 1

class QDragEnterEvent;
class EditDialog;
class FindDialog;
class SQLiteSyntaxHighlighter;
class QStandardItemModel;
class QIntValidator;
class QLabel;
class QModelIndex;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    struct PragmaValues
    {
        int autovacuum;
        int automatic_index;
        int checkpoint_fullsync;
        int foreign_keys;
        int fullfsync;
        int ignore_check_constraints;
        QString journal_mode;
        int journal_size_limit;
        QString locking_mode;
        int max_page_count;
        int page_size;
        int recursive_triggers;
        int secure_delete;
        int synchronous;
        int temp_store;
        int user_version;
        int wal_autocheckpoint;
    } pragmaValues;

    Ui::MainWindow* ui;

    QStandardItemModel *browseTableModel;
    QStandardItemModel *queryResultListModel;
    QMenu *popupTableMenu;
    QMenu *popupFieldMenu;
    QMenu *recentFilesMenu;

    QLabel* statusEncodingLabel;

    SQLiteSyntaxHighlighter* sqliteHighlighterTabSql;
    SQLiteSyntaxHighlighter* sqliteHighlighterLogUser;
    SQLiteSyntaxHighlighter* sqliteHighlighterLogApp;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QAction *recentSeparatorAct;

    int curBrowseOrderByIndex;
    int curBrowseOrderByMode;

    EditDialog* editWin;
    FindDialog* findWin;
    QIntValidator* gotoValidator;

    DBBrowserDB db;

    void init();

    void updateRecentFileActions();
    void setCurrentFile(const QString& fileName);
    void activateFields(bool enable = true);

protected:
    void closeEvent(QCloseEvent *);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    virtual void fileOpen( const QString & fileName );
    virtual void logSql(const QString &sql, int msgtype);
    virtual void dbState(bool dirty);

private slots:
    virtual void createTreeContextMenu(const QPoint & qPoint);
    virtual void changeTreeSelection();
    virtual void addField();
    virtual void editField();
    virtual void deleteField();
    virtual void fileOpen();
    virtual void fileNew();
    virtual void populateStructure();
    virtual void populateTable(const QString & tablename , bool keepColumnWidths = false);
    virtual void resetBrowser();
    virtual void fileClose();
    virtual void fileExit();
    virtual void addRecord();
    virtual void deleteRecord();
    virtual void updateTableView(int lineToSelect , bool keepColumnWidths = false);
    virtual void selectTableLine( int lineToSelect );
    virtual void navigatePrevious();
    virtual void navigateNext();
    virtual void navigateGoto();
    virtual void setRecordsetLabel();
    virtual void browseFind( bool open );
    virtual void browseFindAway();
    virtual void browseRefresh();
    virtual void lookfor( const QString & wfield, const QString & woperator, const QString & wsearchterm );
    virtual void createTable();
    virtual void createIndex();
    virtual void compact();
    virtual void deleteObject();
    virtual void editTable();
    virtual void helpWhatsThis();
    virtual void helpAbout();
    virtual void updateRecordText(int row, int col, const QByteArray& newtext);
    virtual void editWinAway();
    virtual void editText( int row, int col );
    virtual void doubleClickTable(const QModelIndex& index);
    virtual void executeQuery();
    virtual void importTableFromCSV();
    virtual void exportTableToCSV();
    virtual void fileSave();
    virtual void fileRevert();
    virtual void exportDatabaseToSQL();
    virtual void importDatabaseFromSQL();
    virtual void openPreferences();
    virtual void openRecentFile();
    virtual void loadPragmas();
    virtual void updatePragmaUi();
    virtual void savePragmas();
    virtual void mainTabSelected( int tabindex );
    virtual void browseTableHeaderClicked(int logicalindex);
};

#endif
