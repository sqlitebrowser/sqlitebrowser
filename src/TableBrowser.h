#ifndef TABLEBROWSER_H
#define TABLEBROWSER_H

#include "CondFormat.h"
#include "PlotDock.h"
#include "sql/Query.h"

#include <QMap>
#include <QModelIndex>
#include <QWidget>

class DBBrowserDB;
class ExtendedTableWidget;
class SqliteTableModel;

class QAbstractItemModel;
class QIntValidator;

namespace Ui {
class TableBrowser;
}

struct BrowseDataTableSettings
{
    sqlb::Query query;                              // NOTE: We only store the sort order in here (for now)
    QMap<int, int> columnWidths;
    QMap<int, QString> filterValues;
    QMap<int, std::vector<CondFormat>> condFormats;
    QMap<int, QString> displayFormats;
    bool showRowid;
    QString encoding;
    QString plotXAxis;
    QMap<QString, PlotDock::PlotSettings> plotYAxes;
    QString unlockViewPk;
    QMap<int, bool> hiddenColumns;

    BrowseDataTableSettings() :
        showRowid(false),
        unlockViewPk("_rowid_")
    {
    }

    friend QDataStream& operator>>(QDataStream& stream, BrowseDataTableSettings& object)
    {
        int sortOrderIndex, sortOrderMode;
        stream >> sortOrderIndex;
        stream >> sortOrderMode;
        object.query.orderBy().emplace_back(sortOrderIndex, sortOrderMode == Qt::AscendingOrder ? sqlb::Ascending : sqlb::Descending);
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

class TableBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit TableBrowser(QWidget* parent = nullptr);
    ~TableBrowser();
    void init(DBBrowserDB* _db);
    void reset();

    sqlb::ObjectIdentifier currentlyBrowsedTableName() const;

    QMap<sqlb::ObjectIdentifier, BrowseDataTableSettings> allSettings() const { return browseTableSettings; }
    BrowseDataTableSettings& settings(const sqlb::ObjectIdentifier& object);
    void setSettings(const sqlb::ObjectIdentifier& table, const BrowseDataTableSettings& table_settings);

    void setStructure(QAbstractItemModel* model, const QString& old_table = QString());
    void updateStructure();

    SqliteTableModel* model() { return m_browseTableModel; }

    QModelIndex currentIndex() const;

    void setDefaultEncoding(const QString& encoding) { defaultBrowseTableEncoding = encoding; }
    QString defaultEncoding() const { return defaultBrowseTableEncoding; }

public slots:
    void setEnabled(bool enable);
    void updateTable();
    void clearFilters();
    void reloadSettings();
    void setCurrentTable(const sqlb::ObjectIdentifier& name);
    void updateRecordsetLabel();
    void jumpToRow(const sqlb::ObjectIdentifier& table, QString column, const QByteArray& value);

signals:
    void projectModified();
    void selectionChanged(QModelIndex index);
    void selectionChangedByDoubleClick(QModelIndex index);
    void statusMessageRequested(QString message);
    void updatePlot(ExtendedTableWidget* tableWidget, SqliteTableModel* model, BrowseDataTableSettings* settings, bool keepOrResetSelection);
    void createView(QString sql);
    void requestFileOpen(QString file);

private slots:
    void clearTableBrowser();
    void updateFilter(int column, const QString& value);
    void addCondFormat(int column, const QString& value);
    void clearAllCondFormats(int column);
    void editCondFormats(int column);
    void applyBrowseTableSettings(const BrowseDataTableSettings& storedData, bool skipFilters = false);
    void enableEditing(bool enable_edit);
    void showRowidColumn(bool show, bool skipFilters = false);
    void unlockViewEditing(bool unlock, QString pk = QString());
    void hideColumns(int column = -1, bool hide = true);
    void on_actionShowAllColumns_triggered();
    void updateInsertDeleteRecordButton();
    void duplicateRecord(int currentRow);
    void browseTableHeaderClicked(int logicalindex);
    void updateBrowseDataColumnWidth(int section, int /*old_size*/, int new_size);
    void showDataColumnPopupMenu(const QPoint& pos);
    void showRecordPopupMenu(const QPoint& pos);
    void addRecord();
    void insertValues();
    void deleteRecord();
    void navigatePrevious();
    void navigateNext();
    void navigateBegin();
    void navigateEnd();
    void navigateGoto();
    void selectTableLine(int lineToSelect);
    void on_actionClearFilters_triggered();
    void on_actionClearSorting_triggered();
    void editDataColumnDisplayFormat();
    void exportFilteredTable();
    void saveFilterAsView();
    void browseDataSetTableEncoding(bool forAllTables = false);
    void browseDataSetDefaultTableEncoding();

private:
    Ui::TableBrowser* ui;
    QIntValidator* gotoValidator;
    QMenu* popupNewRecordMenu;
    QMenu* popupSaveFilterAsMenu;
    QMenu* popupBrowseDataHeaderMenu;

    DBBrowserDB* db;

    QAbstractItemModel* dbStructureModel;

    /// the table model used in the "Browse Data" page (re-used and
    /// re-initialized when switching to another table)
    SqliteTableModel* m_browseTableModel;

    static QMap<sqlb::ObjectIdentifier, BrowseDataTableSettings> browseTableSettings;  // This is static, so settings are shared between instances
    static QString defaultBrowseTableEncoding;

    Palette m_condFormatPalette;
};

#endif
