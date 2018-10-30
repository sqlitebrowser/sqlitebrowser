#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sqlitedb.h"
#include "PlotDock.h"
#include "Palette.h"
#include "CondFormat.h"

#include <QMainWindow>
#include <QMap>

class QDragEnterEvent;
class EditDialog;
class QIntValidator;
class QLabel;
class QModelIndex;
class QPersistentModelIndex;
class SqliteTableModel;
class DbStructureModel;
class RemoteDock;
class RemoteDatabase;
class FindReplaceDialog;
class ExtendedTableWidget;

namespace Ui {
class MainWindow;
}

struct BrowseDataTableSettings
{
    int sortOrderIndex;
    Qt::SortOrder sortOrderMode;
    QMap<int, int> columnWidths;
    QMap<int, QString> filterValues;
    QMap<int, QVector<CondFormat>> condFormats;
    QMap<int, QString> displayFormats;
    bool showRowid;
    QString encoding;
    QString plotXAxis;
    QMap<QString, PlotDock::PlotSettings> plotYAxes;
    QString unlockViewPk;
    QMap<int, bool> hiddenColumns;

    BrowseDataTableSettings() :
        sortOrderIndex(0),
        sortOrderMode(Qt::AscendingOrder),
        showRowid(false)
    {
    }

    friend QDataStream& operator>>(QDataStream& stream, BrowseDataTableSettings& object)
    {
        stream >> object.sortOrderIndex;
        int sortordermode;
        stream >> sortordermode;
        object.sortOrderMode = static_cast<Qt::SortOrder>(sortordermode);
        stream >> object.columnWidths;
        stream >> object.filterValues;
        stream >> object.displayFormats;
        stream >> object.showRowid;
        stream >> object.encoding;

        // Versions pre 3.10.0 didn't store the following information in their project files.
        // To be absolutely sure that nothing strange happens when we read past the stream for
        // those cases, check for the end of the stream here.
        if(stream.atEnd())
            return stream;
        stream >> object.plotXAxis;
        stream >> object.plotYAxes;
        stream >> object.unlockViewPk;

        // Project files from versions before 3.11.0 didn't have these fields
        if(stream.atEnd())
            return stream;
        stream >> object.hiddenColumns;

        return stream;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    DBBrowserDB& getDb() { return db; }
    RemoteDatabase& getRemote() { return *m_remoteDb; }

    enum Tabs
    {
        StructureTab,
        BrowseTab,
        PragmaTab,
        ExecuteTab
    };

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

    enum StatementType
    {
        SelectStatement,
        AlterStatement,
        DropStatement,
        RollbackStatement,
        PragmaStatement,
        VacuumStatement,
        InsertStatement,
        UpdateStatement,
        DeleteStatement,
        CreateStatement,
        AttachStatement,
        DetachStatement,
        OtherStatement,
    };

    Ui::MainWindow* ui;

    DBBrowserDB db;

    /// the table model used in the "Browse Data" page (re-used and
    /// re-initialized when switching to another table)
    SqliteTableModel* m_browseTableModel;

    SqliteTableModel* m_currentTabTableModel;

    QMenu* popupTableMenu;
    QMenu* popupSchemaDockMenu;
    QMenu* recentFilesMenu;
    QMenu* popupOpenDbMenu;
    QMenu* popupNewRecordMenu;
    QMenu* popupSaveSqlFileMenu;
    QMenu* popupSaveSqlResultsMenu;
    QMenu* popupSaveFilterAsMenu;
    QMenu* popupBrowseDataHeaderMenu;

    QLabel* statusEncodingLabel;
    QLabel* statusEncryptionLabel;
    QLabel* statusReadOnlyLabel;

    DbStructureModel* dbStructureModel;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QAction *recentSeparatorAct;

    QMap<sqlb::ObjectIdentifier, BrowseDataTableSettings> browseTableSettings;

    RemoteDatabase* m_remoteDb;

    EditDialog* editDock;
    PlotDock* plotDock;
    RemoteDock* remoteDock;
    FindReplaceDialog* findReplaceDialog;

    QIntValidator* gotoValidator;

    QString defaultBrowseTableEncoding;

    Palette m_condFormatPalette;

    void init();
    void clearCompleterModelsFields();

    void updateRecentFileActions();
    void setCurrentFile(const QString& fileName);
    void addToRecentFilesMenu(const QString& filename);
    void activateFields(bool enable = true);
    void enableEditing(bool enable_edit);
    void saveAsView(QString query);
    void duplicateRecord(int currentRow);
    void selectTableLine(int lineToSelect);
    void attachPlot(ExtendedTableWidget* tableWidget, SqliteTableModel* model, BrowseDataTableSettings* settings = nullptr, bool keepOrResetSelection = true);

    sqlb::ObjectIdentifier currentlyBrowsedTableName() const;

    StatementType getQueryType(const QString& query) const;

    void applyBrowseTableSettings(BrowseDataTableSettings storedData, bool skipFilters = false);

protected:
    void closeEvent(QCloseEvent *) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;

public slots:
    bool fileOpen(const QString& fileName = QString(), bool dontAddToRecentFiles = false, bool readOnly = false);
    void logSql(const QString &sql, int msgtype);
    void dbState(bool dirty);
    void refresh();
    void jumpToRow(const sqlb::ObjectIdentifier& table, QString column, const QByteArray& value);
    void switchToBrowseDataTab(QString tableToBrowse = QString());
    void populateStructure(const QString& old_table = QString());

private slots:
    void createTreeContextMenu(const QPoint & qPoint);
    void createSchemaDockContextMenu(const QPoint & qPoint);
    void changeTreeSelection();
    void fileNew();
    void fileNewInMemoryDatabase();
    void populateTable();
    void clearTableBrowser();
    bool fileClose();
    void addRecord();
    void insertValues();
    void deleteRecord();
    void navigatePrevious();
    void navigateNext();
    void navigateBegin();
    void navigateEnd();
    void navigateGoto();
    void setRecordsetLabel();
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
    void openPreferences();
    void openRecentFile();
    void loadPragmas();
    void updatePragmaUi();
    void savePragmas();
    void mainTabSelected( int tabindex );
    void browseTableHeaderClicked(int logicalindex);
    unsigned int openSqlTab(bool resetCounter = false);
    void closeSqlTab(int index, bool force = false);
    void changeSqlTab(int index);
    void openSqlFile();
    void saveSqlFile();
    void saveSqlFileAs();
    void saveSqlResultsAsCsv();
    void saveSqlResultsAsView();
    void loadExtension();
    void reloadSettings();
    void checkNewVersion(const QString& versionstring, const QString& url);
    void on_actionWiki_triggered();
    void on_actionBug_report_triggered();
    void on_actionFeature_Request_triggered();
    void on_actionSqlCipherFaq_triggered();
    void on_actionWebsite_triggered();
    void on_actionDonatePatreon_triggered();
    void updateBrowseDataColumnWidth(int section, int /*old_size*/, int new_size);
    bool loadProject(QString filename = QString(), bool readOnly = false);
    void saveProject();
    void fileAttach();
    void updateFilter(int column, const QString& value);
    void addCondFormat(int column, const QString& value);
    void clearAllCondFormats(int column);
    void editEncryption();
    void on_buttonClearFilters_clicked();
    void copyCurrentCreateStatement();
    void showDataColumnPopupMenu(const QPoint& pos);
    void showRecordPopupMenu(const QPoint& pos);
    void editDataColumnDisplayFormat();
    void showRowidColumn(bool show, bool skipFilters = false);
    void browseDataSetTableEncoding(bool forAllTables = false);
    void browseDataSetDefaultTableEncoding();
    void fileOpenReadOnly();
    void unlockViewEditing(bool unlock, QString pk = QString());
    void hideColumns(int column = -1, bool hide = true);
    void on_actionShowAllColumns_triggered();
    void requestCollation(const QString& name, int eTextRep);
    void renameSqlTab(int index);
    void setFindFrameVisibility(bool show);
    void openFindReplaceDialog();
    void openSqlPrintDialog();
    void saveFilterAsView();
    void exportFilteredTable();
    void updateInsertDeleteRecordButton();
    void runSqlNewTab(const QString& query, const QString& title);
    void printDbStructure();
};

#endif
