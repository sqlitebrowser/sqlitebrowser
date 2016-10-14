#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sqltextedit.h"
#include "sqlitedb.h"

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
class QNetworkReply;
class QNetworkAccessManager;
class QTreeWidgetItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    DBBrowserDB& getDb() { return db; }

    struct PlotSettings
    {
        int lineStyle;
        int pointShape;
        QColor colour;

        friend QDataStream& operator<<(QDataStream& stream, const MainWindow::PlotSettings& object)
        {
            stream << object.lineStyle;
            stream << object.pointShape;
            stream << object.colour;

            return stream;
        }
        friend QDataStream& operator>>(QDataStream& stream, MainWindow::PlotSettings& object)
        {
            stream >> object.lineStyle;
            stream >> object.pointShape;
            stream >> object.colour;

            return stream;
        }
    };

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
        QMap<QString, PlotSettings> plotYAxes;

        friend QDataStream& operator<<(QDataStream& stream, const MainWindow::BrowseDataTableSettings& object)
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

            return stream;
        }
        friend QDataStream& operator>>(QDataStream& stream, MainWindow::BrowseDataTableSettings& object)
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

            return stream;
        }
    };

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

    enum PlotColumns
    {
        PlotColumnField = 0,
        PlotColumnX = 1,
        PlotColumnY = 2,
        PlotColumnDummy = 3
    };

    Ui::MainWindow* ui;

    SqliteTableModel* m_browseTableModel;
    SqliteTableModel* m_currentTabTableModel;
    SqliteTableModel* m_currentPlotModel;
    QMenu *popupTableMenu;
    QMenu *recentFilesMenu;
    QMenu *popupSaveSqlFileMenu;
    QMenu* popupBrowseDataHeaderMenu;
    QMenu* popupRecordMenu;

    QLabel* statusEncodingLabel;
    QLabel* statusEncryptionLabel;
    QLabel* statusReadOnlyLabel;

    DbStructureModel* dbStructureModel;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QAction *recentSeparatorAct;

    QMap<QString, BrowseDataTableSettings> browseTableSettings;

    EditDialog* editDock;
    QIntValidator* gotoValidator;

    DBBrowserDB db;
    QString defaultBrowseTableEncoding;

    QNetworkAccessManager* m_NetworkManager;

    void init();
    void clearCompleterModelsFields();

    void updateRecentFileActions();
    void setCurrentFile(const QString& fileName);
    void addToRecentFilesMenu(const QString& filename);
    void activateFields(bool enable = true);
    void loadExtensionsFromSettings();

protected:
    void closeEvent(QCloseEvent *);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent* event);

public slots:
    bool fileOpen(const QString& fileName = QString(), bool dontAddToRecentFiles = false);
    void logSql(const QString &sql, int msgtype);
    void dbState(bool dirty);
    void browseRefresh();
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
    void dittoRecord();
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
    void editTable();
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
    void on_actionOpen_Remote_triggered();
    void on_actionSave_Remote_triggered();
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
    void httpresponse(QNetworkReply* reply);
    void updatePlot(SqliteTableModel* model, bool update = true, bool keepOrResetSelection = true);
    void on_treePlotColumns_itemChanged(QTreeWidgetItem *item, int column);
    void on_treePlotColumns_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_butSavePlot_clicked();
    void on_actionWiki_triggered();
    void on_actionBug_report_triggered();
    void on_actionSqlCipherFaq_triggered();
    void on_actionWebsite_triggered();
    void updateBrowseDataColumnWidth(int section, int /*old_size*/, int new_size);
    bool loadProject(QString filename = QString());
    void saveProject();
    void fileAttach();
    void updateFilter(int column, const QString& value);
    void editEncryption();
    void on_buttonClearFilters_clicked();
    void copyCurrentCreateStatement();
    void on_comboLineType_currentIndexChanged(int index);
    void on_comboPointShape_currentIndexChanged(int index);
    void showDataColumnPopupMenu(const QPoint& pos);
    void showRecordPopupMenu(const QPoint& pos);
    void editDataColumnDisplayFormat();
    void showRowidColumn(bool show);
    void browseDataSetTableEncoding(bool forAllTables = false);
    void browseDataSetDefaultTableEncoding();
    void browseDataFetchAllData();
};

#endif
