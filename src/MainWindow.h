#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sqlitedb.h"
#include "PlotDock.h"

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

namespace Ui {
class MainWindow;
}

struct BrowseDataTableSettings
{
    int sortOrderIndex;
    Qt::SortOrder sortOrderMode;
    QMap<int, int> columnWidths;
    QMap<int, QString> filterValues;
    QMap<int, QString> displayFormats;
    bool showRowid;
    QString encoding;
    QString plotXAxis;
    QMap<QString, PlotDock::PlotSettings> plotYAxes;
    QString unlockViewPk;

    BrowseDataTableSettings() :
        sortOrderIndex(0),
        sortOrderMode(Qt::AscendingOrder),
        showRowid(false)
    {
    }

    friend QDataStream& operator<<(QDataStream& stream, const BrowseDataTableSettings& object)
    {
        stream << object.sortOrderIndex;
        stream << static_cast<int>(object.sortOrderMode);
        stream << object.columnWidths;
        stream << object.filterValues;
        stream << object.displayFormats;
        stream << object.showRowid;
        stream << object.encoding;
        stream << object.plotXAxis;
        stream << object.plotYAxes;
        stream << object.unlockViewPk;

        return stream;
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

        return stream;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

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
    } pragmaValues;

    Ui::MainWindow* ui;

    SqliteTableModel* m_browseTableModel;
    SqliteTableModel* m_currentTabTableModel;

    QMenu *popupTableMenu;
    QMenu *recentFilesMenu;
    QMenu *popupSaveSqlFileMenu;
    QMenu* popupBrowseDataHeaderMenu;

    QLabel* statusEncodingLabel;
    QLabel* statusEncryptionLabel;
    QLabel* statusReadOnlyLabel;

    DbStructureModel* dbStructureModel;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QAction *recentSeparatorAct;

    QMap<QString, BrowseDataTableSettings> browseTableSettings;

    RemoteDatabase* m_remoteDb;

    EditDialog* editDock;
    PlotDock* plotDock;
    RemoteDock* remoteDock;

    QIntValidator* gotoValidator;

    DBBrowserDB db;
    QString defaultBrowseTableEncoding;

    void init();
    void clearCompleterModelsFields();

    void updateRecentFileActions();
    void setCurrentFile(const QString& fileName);
    void addToRecentFilesMenu(const QString& filename);
    void activateFields(bool enable = true);
    void enableEditing(bool enable_edit, bool enable_insertdelete);
    void loadExtensionsFromSettings();

protected:
    void closeEvent(QCloseEvent *);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent* event);

public slots:
    bool fileOpen(const QString& fileName = QString(), bool dontAddToRecentFiles = false, bool readOnly = false);
    void logSql(const QString &sql, int msgtype);
    void dbState(bool dirty);
    void refresh();
    void jumpToRow(const QString& table, QString column, const QByteArray& value);
    void switchToBrowseDataTab(QString tableToBrowse = QString());
    void populateStructure();

private slots:
    void createTreeContextMenu(const QPoint & qPoint);
    void changeTreeSelection();
    void fileNew();
    void populateTable();
    void clearTableBrowser();
    bool fileClose();
    void addRecord();
    void deleteRecord();
    void selectTableLine( int lineToSelect );
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
    void openSqlFile();
    void saveSqlFile();
    void saveSqlFileAs();
    void loadExtension();
    void reloadSettings();
    void checkNewVersion(const QString& versionstring, const QString& url);
    void on_actionWiki_triggered();
    void on_actionBug_report_triggered();
    void on_actionSqlCipherFaq_triggered();
    void on_actionWebsite_triggered();
    void updateBrowseDataColumnWidth(int section, int /*old_size*/, int new_size);
    bool loadProject(QString filename = QString(), bool readOnly = false);
    void saveProject();
    void fileAttach();
    void updateFilter(int column, const QString& value);
    void editEncryption();
    void on_buttonClearFilters_clicked();
    void copyCurrentCreateStatement();
    void showDataColumnPopupMenu(const QPoint& pos);
    void showRecordPopupMenu(const QPoint& pos);
    void editDataColumnDisplayFormat();
    void showRowidColumn(bool show);
    void browseDataSetTableEncoding(bool forAllTables = false);
    void browseDataSetDefaultTableEncoding();
    void fileOpenReadOnly();
    void unlockViewEditing(bool unlock, QString pk = QString());
};

#endif
