#ifndef MAINFORM_H
#define MAINFORM_H

#include "sqltextedit.h"
#include "sqlitedb.h"

#include <QMainWindow>
#include <QStandardItemModel>

class QDragEnterEvent;
class EditDialog;
class SQLiteSyntaxHighlighter;
class QStandardItemModel;
class QIntValidator;
class QLabel;
class QModelIndex;
class SqliteTableModel;
class DbStructureModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

    DBBrowserDB* getDb() { return &db; }

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

    SqliteTableModel* m_browseTableModel;
    QMenu *popupTableMenu;
    QMenu *recentFilesMenu;

    QLabel* statusEncodingLabel;

    SQLiteSyntaxHighlighter* sqliteHighlighterLogUser;
    SQLiteSyntaxHighlighter* sqliteHighlighterLogApp;

    DbStructureModel* dbStructureModel;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QAction *recentSeparatorAct;

    int curBrowseOrderByIndex;
    Qt::SortOrder curBrowseOrderByMode;

    EditDialog* editWin;
    QIntValidator* gotoValidator;

    DBBrowserDB db;

    QStandardItemModel completerModelTables;
    SqlTextEdit::FieldCompleterModelMap completerModelsFields;

    void init();

    void updateRecentFileActions();
    void setCurrentFile(const QString& fileName);
    void activateFields(bool enable = true);
    void loadExtensionsFromSettings();

protected:
    void closeEvent(QCloseEvent *);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    virtual bool fileOpen(const QString& fileName);
    virtual void logSql(const QString &sql, int msgtype);
    virtual void dbState(bool dirty);
    virtual void browseRefresh();

private slots:
    virtual void createTreeContextMenu(const QPoint & qPoint);
    virtual void changeTreeSelection();
    virtual void fileOpen();
    virtual void fileNew();
    virtual void populateStructure();
    virtual void populateTable(const QString& tablename);
    virtual void resetBrowser();
    virtual void fileClose();
    virtual void addRecord();
    virtual void deleteRecord();
    virtual void selectTableLine( int lineToSelect );
    virtual void navigatePrevious();
    virtual void navigateNext();
    virtual void navigateGoto();
    virtual void setRecordsetLabel();
    virtual void createTable();
    virtual void createIndex();
    virtual void compact();
    virtual void deleteObject();
    virtual void editTable();
    virtual void helpWhatsThis();
    virtual void helpAbout();
    virtual void updateRecordText(int row, int col, const QByteArray& newtext);
    virtual void editWinAway();
    virtual void editText(const QModelIndex& index);
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
    virtual void createSyntaxHighlighters();
    virtual unsigned int openSqlTab(bool resetCounter = false);
    virtual void closeSqlTab(int index, bool force = false);
    virtual void openSqlFile();
    virtual void saveSqlFile();
    virtual void loadExtension();
    virtual void reloadSettings();
};

#endif
