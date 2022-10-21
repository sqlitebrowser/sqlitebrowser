#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dbstructureqitemviewfacade.h"
#include "sqlitedb.h"

#include <memory>
#include <vector>
#include <QMainWindow>

struct BrowseDataTableSettings;
class DbStructureModel;
class EditDialog;
class ExtendedTableWidget;
class FindReplaceDialog;
class PlotDock;
class RemoteDock;
class RunSql;
class SqliteTableModel;
class TableBrowser;
class TableBrowserDock;

class QDragEnterEvent;
class QModelIndex;
class QLabel;
class QPersistentModelIndex;
class QToolButton;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

friend TableBrowserDock;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    DBBrowserDB& getDb() { return db; }

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
        int case_sensitive_like;
    } pragmaValues;

    Ui::MainWindow* ui;

    DBBrowserDB db;

    SqliteTableModel* m_currentTabTableModel;

    QMenu* popupTableMenu;
    QMenu* popupSchemaDockMenu;
    QMenu* recentFilesMenu;
    QMenu* popupOpenDbMenu;
    QMenu* popupSaveSqlFileMenu;
    QMenu* popupSaveSqlResultsMenu;

    QLabel* statusEncodingLabel;
    QLabel* statusEncryptionLabel;
    QLabel* statusReadOnlyLabel;
    QToolButton* statusStopButton;
    QLabel* statusBusyLabel;

    DbStructureModel* dbStructureModel;

    static int MaxRecentFiles;
    QVector<QAction*> recentFileActs;
    QAction* clearRecentFilesAction;
    QAction* recentSeparatorAct;

    /**
     * @brief dbSelected provides a simple read only interface to the actual selected node of ui->dbTreeWidget
     */
    DbStructureQItemViewFacade *dbSelected;

    /**
     * @brief dockDbSelected provides a simple read only interface to the actual selected node of ui->treeSchemaDock
     */
    DbStructureQItemViewFacade *dockDbSelected;

    EditDialog* editDock;
    PlotDock* plotDock;
    RemoteDock* remoteDock;
    TableBrowser* currentTableBrowser;
    FindReplaceDialog* findReplaceDialog;

    std::unique_ptr<RunSql> execute_sql_worker;

    QString defaultOpenTabs;
    QByteArray defaultWindowState;

    QString currentProjectFilename;
    bool isProjectModified;

    void init();
    void clearCompleterModelsFields();

    void updateRecentFileActions();
    void clearRecentFiles();
    void setCurrentFile(const QString& fileName);
    void addToRecentFilesMenu(const QString& filename, bool read_only = false);
    void activateFields(bool enable = true);
    void saveAsView(const std::string& query);
    void attachPlot(ExtendedTableWidget* tableWidget, SqliteTableModel* model, BrowseDataTableSettings* settings = nullptr, bool keepOrResetSelection = true);

    void toggleTabVisible(QWidget* tabWidget, bool show);
    void restoreOpenTabs(QString tabs);
    QString saveOpenTabs();
    void saveProject(const QString& currentFilename);
    bool closeProject();
    bool askSaveSqlTab(int index, bool& ignoreUnattachedBuffers);
    void focusSqlEditor();
    void moveDocksTo(Qt::DockWidgetArea area);

    // Identifier of the currently browsed table in the current data browser tab
    sqlb::ObjectIdentifier currentlyBrowsedTableName() const;

    QList<TableBrowserDock*> allTableBrowserDocks() const;

protected:
    void closeEvent(QCloseEvent *) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public slots:
    bool fileOpen(const QString& fileName = QString(), bool openFromProject = false, bool readOnly = false);
    void logSql(const QString &sql, int msgtype);
    void dbState(bool dirty);
    void refresh();
    void switchToBrowseDataTab(sqlb::ObjectIdentifier tableToBrowse = sqlb::ObjectIdentifier());
    void populateStructure(const std::vector<sqlb::ObjectIdentifier>& old_tables = {});
    void fileDetachDbTree();
    void fileDetachTreeSchemaDock();
    void fileDetachTreeViewSelected(QTreeView* treeView);
    void reloadSettings();
    bool closeFiles();
    void importCSVfiles(const std::vector<QString>& inputFiles, const QString& table = QString());

private slots:
    void createTreeContextMenu(const QPoint & qPoint);
    void createSchemaDockContextMenu(const QPoint & qPoint);
    void changeTreeSelection();
    void fileNew();
    void fileNewInMemoryDatabase(bool open_create_dialog = true);
    void refreshTableBrowsers();
    bool fileClose();
    bool fileSaveAs();
    void createTable();
    void createIndex();
    void compact();
    void deleteObject();
    void editObject();
    void helpWhatsThis();
    void helpAbout();
    void updateRecordText(const QPersistentModelIndex& idx, const QByteArray& text, bool isBlob);
    void evaluateText(const QPersistentModelIndex& idx, const std::string& text);
    void toggleEditDock(bool visible);
    void dataTableSelectionChanged(const QModelIndex& index);
    void doubleClickTable(const QModelIndex& index);
    void executeQuery();
    void importTableFromCSV();
    void exportTableToCSV();
    void exportTableToJson();
    void fileSave();
    void fileRevert();
    void exportDatabaseToSQL();
    void importDatabaseFromSQL();
    void openRecentFile();
    void loadPragmas();
    void updatePragmaUi();
    void savePragmas();
    void mainTabSelected( int tabindex );
    void openSqlFile(int tabindex, QString filename);
    void openSqlFile();
    void saveSqlFile();
    void saveSqlFileAs();
    void saveSqlResultsAsCsv();
    void saveSqlResultsAsJson();
    void saveSqlResultsAsView();
    void loadExtension();
    void checkNewVersion(const QString& versionstring, const QString& url);
    void openLinkWiki() const;
    void openLinkBugReport() const;
    void openLinkFeatureRequest() const;
    void openLinkSqlCipherFaq() const;
    void openLinkWebsite() const;
    void openLinkDonatePatreon() const;
    bool loadProject(QString filename = QString(), bool readOnly = false);
    void saveProject();
    void saveProjectAs();
    void fileAttach(const QString& fileName = QString());
    void editEncryption();
    void copyCurrentCreateStatement();
    void fileOpenReadOnly();
    void requestCollation(const QString& name, int eTextRep);
    void setFindFrameVisibility(bool show);
    void openFindReplaceDialog();
    void toggleSqlBlockComment();
    void openSqlPrintDialog();
    void runSqlNewTab(const QString& query, const QString& title, const QString& helpUrl = QString(), const bool autoRun = true);
    void printDbStructure();
    void updateDatabaseBusyStatus(bool busy, const QString& user);
    void openPreferences();
    void closeTab(int index);
    void showStatusMessage5s(QString message);
    void saveSqlFile(int tabIndex);
    void saveAll();
    void openUrlOrFile(const QString& urlString);
    void newRowCountsTab();

    int openSqlTab(bool resetCounter = false);
    void closeSqlTab(int index, bool force = false, bool askSaving = true);
    void changeSqlTab(int index);
    void renameSqlTab(int index);
    void showContextMenuSqlTabBar(const QPoint& pos);

    TableBrowserDock* newTableBrowserTab(const sqlb::ObjectIdentifier& tableToBrowse = {}, bool forceHideTitleBar = false);
    void tableBrowserTabClosed();
    void changeTableBrowserTab(TableBrowserDock* dock);
};

#endif
