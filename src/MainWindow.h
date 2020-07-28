#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sqlitedb.h"

#include <memory>
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

    static const int MaxRecentFiles = 5;
    QAction *recentFileActs[MaxRecentFiles];

    EditDialog* editDock;
    PlotDock* plotDock;
    RemoteDock* remoteDock;
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
    bool closeFiles();
    bool closeProject();
    bool askSaveSqlTab(int index, bool& ignoreUnattachedBuffers);
    void focusSqlEditor();
    void moveDocksTo(Qt::DockWidgetArea area);

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
    void populateStructure(const QString& old_table = QString());
    void reloadSettings();

private slots:
    void createTreeContextMenu(const QPoint & qPoint);
    void createSchemaDockContextMenu(const QPoint & qPoint);
    void changeTreeSelection();
    void fileNew();
    void fileNewInMemoryDatabase();
    void populateTable();
    bool fileClose();
    void createTable();
    void createIndex();
    void compact();
    void deleteObject();
    void editObject();
    void helpWhatsThis();
    void helpAbout();
    void updateRecordText(const QPersistentModelIndex& idx, const QByteArray& text, bool isBlob);
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
    int openSqlTab(bool resetCounter = false);
    void closeSqlTab(int index, bool force = false);
    void changeSqlTab(int index);
    void openSqlFile();
    void saveSqlFile();
    void saveSqlFileAs();
    void saveSqlResultsAsCsv();
    void saveSqlResultsAsView();
    void loadExtension();
    void checkNewVersion(const QString& versionstring, const QString& url);
    void on_actionWiki_triggered() const;
    void on_actionBug_report_triggered() const;
    void on_actionFeature_Request_triggered() const;
    void on_actionSqlCipherFaq_triggered() const;
    void on_actionWebsite_triggered() const;
    void on_actionDonatePatreon_triggered() const;
    bool loadProject(QString filename = QString(), bool readOnly = false);
    void saveProject();
    void saveProjectAs();
    void fileAttach(const QString& fileName = QString());
    void editEncryption();
    void copyCurrentCreateStatement();
    void fileOpenReadOnly();
    void requestCollation(const QString& name, int eTextRep);
    void renameSqlTab(int index);
    void setFindFrameVisibility(bool show);
    void openFindReplaceDialog();
    void toggleSqlBlockComment();
    void openSqlPrintDialog();
    void runSqlNewTab(const QString& query, const QString& title, const QString& helpUrl, const bool autoRun = true);
    void printDbStructure();
    void updateDatabaseBusyStatus(bool busy, const QString& user);
    void openPreferences();
    void closeTab(int index);
    void showStatusMessage5s(QString message);
    void saveSqlFile(int tabIndex);
    void saveAll();
    void showContextMenuSqlTabBar(const QPoint& pos);
    void openUrlOrFile(const QString& urlString);
};

#endif
