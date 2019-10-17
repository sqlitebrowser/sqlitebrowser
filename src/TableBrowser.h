#ifndef TABLEBROWSER_H
#define TABLEBROWSER_H

#include "CondFormat.h"
#include "PlotDock.h"
#include "sql/Query.h"

#include <QMap>
#include <QModelIndex>
#include <QWidget>

#include <functional>
#include <unordered_set>

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
    using CondFormatMap = QMap<size_t, std::vector<CondFormat>>;
    sqlb::Query query;                              // NOTE: We only store the sort order in here (for now)
    QMap<int, int> columnWidths;
    QMap<int, QString> filterValues;
    CondFormatMap condFormats;
    CondFormatMap rowIdFormats;
    QMap<int, QString> displayFormats;
    bool showRowid;
    QString encoding;
    QString plotXAxis;
    QMap<QString, PlotDock::PlotSettings> plotYAxes;
    QString unlockViewPk;
    QMap<int, bool> hiddenColumns;
    std::vector<QString> globalFilters;

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

    QMap<sqlb::ObjectIdentifier, BrowseDataTableSettings> allSettings() const { return m_settings; }
    BrowseDataTableSettings& settings(const sqlb::ObjectIdentifier& object);
    void setSettings(const sqlb::ObjectIdentifier& table, const BrowseDataTableSettings& table_settings);

    void setStructure(QAbstractItemModel* model, const QString& old_table = QString());
    void updateStructure();

    SqliteTableModel* model() { return m_model; }

    QModelIndex currentIndex() const;

    void setDefaultEncoding(const QString& encoding) { m_defaultEncoding = encoding; }
    QString defaultEncoding() const { return m_defaultEncoding; }

public slots:
    void setEnabled(bool enable);
    void updateTable();
    void clearFilters();
    void reloadSettings();
    void setCurrentTable(const sqlb::ObjectIdentifier& name);
    void updateRecordsetLabel();
    void jumpToRow(const sqlb::ObjectIdentifier& table, std::string column, const QByteArray& value);

signals:
    void projectModified();
    void selectionChanged(QModelIndex index);
    void selectionChangedByDoubleClick(QModelIndex index);
    void statusMessageRequested(QString message);
    void updatePlot(ExtendedTableWidget* tableWidget, SqliteTableModel* model, BrowseDataTableSettings* settings, bool keepOrResetSelection);
    void createView(std::string sql);
    void requestFileOpen(QString file);

private slots:
    void clear();
    void updateFilter(size_t column, const QString& value);
    void addCondFormatFromFilter(size_t column, const QString& value);
    void addCondFormat(bool isRowIdFormat, size_t column, const CondFormat& newCondFormat);
    void clearAllCondFormats(size_t column);
    void clearRowIdFormats(const QModelIndex index);
    void editCondFormats(size_t column);
    void applySettings(const BrowseDataTableSettings& storedData, bool skipFilters = false);
    void enableEditing(bool enable_edit);
    void showRowidColumn(bool show, bool skipFilters = false);
    void unlockViewEditing(bool unlock, QString pk = QString());
    void hideColumns(int column = -1, bool hide = true);
    void on_actionShowAllColumns_triggered();
    void updateInsertDeleteRecordButton();
    void duplicateRecord(int currentRow);
    void headerClicked(int logicalindex);
    void updateColumnWidth(int section, int /*old_size*/, int new_size);
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
    void editDisplayFormat();
    void exportFilteredTable();
    void saveFilterAsView();
    void setTableEncoding(bool forAllTables = false);
    void setDefaultTableEncoding();

private:
    void find(const QString& expr, bool forward, bool include_first = false);

private:
    Ui::TableBrowser* ui;
    QIntValidator* gotoValidator;
    QMenu* popupNewRecordMenu;
    QMenu* popupSaveFilterAsMenu;
    QMenu* popupHeaderMenu;

    DBBrowserDB* db;

    QAbstractItemModel* dbStructureModel;

    /// the table model used in the "Browse Data" page (re-used and
    /// re-initialized when switching to another table)
    SqliteTableModel* m_model;

    static QMap<sqlb::ObjectIdentifier, BrowseDataTableSettings> m_settings;  // This is static, so settings are shared between instances
    static QString m_defaultEncoding;

    Palette m_condFormatPalette;

    void modifySingleFormat(const bool isRowIdFormat, const QString& filter, const QModelIndex refIndex,
                            std::function<void(CondFormat&)> changeFunction);
    void modifyFormat(std::function<void(CondFormat&)> changeFunction);
};

#endif
