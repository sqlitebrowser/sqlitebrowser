#ifndef TABLEBROWSER_H
#define TABLEBROWSER_H

#include "CondFormat.h"
#include "PlotDock.h"
#include "sql/Query.h"

#include <QModelIndex>
#include <QWidget>

#include <functional>
#include <map>
#include <unordered_set>
#include <vector>

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
    using CondFormatMap = std::map<size_t, std::vector<CondFormat>>;
    std::vector<sqlb::OrderBy> sortColumns;
    std::map<int, int> columnWidths;
    std::map<std::string, QString> filterValues;
    CondFormatMap condFormats;
    CondFormatMap rowIdFormats;
    std::map<size_t, QString> displayFormats;
    bool showRowid;
    QString encoding;
    QString plotXAxis;
    std::vector<std::map<QString, PlotDock::PlotSettings>> plotYAxes;
    QString unlockViewPk;
    std::map<int, bool> hiddenColumns;
    std::vector<QString> globalFilters;
    size_t frozenColumns;

    BrowseDataTableSettings() :
        showRowid(false),
        plotYAxes({std::map<QString, PlotDock::PlotSettings>(), std::map<QString, PlotDock::PlotSettings>()}),
        unlockViewPk("_rowid_"),
        frozenColumns(0)
    {
    }
};

class TableBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit TableBrowser(DBBrowserDB* _db, QWidget* parent = nullptr);
    ~TableBrowser() override;

    void reset();
    static void resetSharedSettings();

    sqlb::ObjectIdentifier currentlyBrowsedTableName() const;

    static std::map<sqlb::ObjectIdentifier, BrowseDataTableSettings> allSettings() { return m_settings; }
    static BrowseDataTableSettings& settings(const sqlb::ObjectIdentifier& object);
    static void setSettings(const sqlb::ObjectIdentifier& table, const BrowseDataTableSettings& table_settings);

    void setStructure(QAbstractItemModel* model, const sqlb::ObjectIdentifier& old_table = sqlb::ObjectIdentifier{});

    SqliteTableModel* model() { return m_model; }

    QModelIndex currentIndex() const;

    static void setDefaultEncoding(const QString& encoding) { m_defaultEncoding = encoding; }
    static QString defaultEncoding() { return m_defaultEncoding; }

public slots:
    void setEnabled(bool enable);
    void refresh();
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
    void currentTableChanged(sqlb::ObjectIdentifier table);
    void foreignKeyClicked(sqlb::ObjectIdentifier table, std::string column, QByteArray value);
    void dataAboutToBeEdited(const QModelIndex& index);
    void prepareForFilter();
    void newDockRequested();

private slots:
    void clear();
    void updateFilter(size_t column, const QString& value);
    void addCondFormatFromFilter(size_t column, const QString& value);
    void addCondFormat(bool isRowIdFormat, size_t column, const CondFormat& newCondFormat);
    void clearAllCondFormats(size_t column);
    void clearRowIdFormats(const QModelIndex index);
    void editCondFormats(size_t column);
    void enableEditing(bool enable_edit);
    void showRowidColumn(bool show);
    void freezeColumns(size_t columns);
    void unlockViewEditing(bool unlock, QString pk = QString());
    void hideColumns(int column = -1, bool hide = true);
    void showAllColumns();
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
    void clearSorting();
    void editDisplayFormat();
    void exportCsvFilteredTable();
    void exportJsonFilteredTable();
    void saveFilterAsView();
    void setTableEncoding(bool forAllTables = false);
    void setDefaultTableEncoding();
    void copyColumnName();
    void fetchedData();

private:
    enum class ReplaceMode
    {
        NoReplace,
        ReplaceNext,
        ReplaceAll,
    };
    void find(const QString& expr, bool forward, bool include_first = false, ReplaceMode replace = ReplaceMode::NoReplace);

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

    static std::map<sqlb::ObjectIdentifier, BrowseDataTableSettings> m_settings;  // This is static, so settings are shared between instances
    static QString m_defaultEncoding;

    Palette m_condFormatPalette;
    bool m_adjustRows;
    bool m_columnsResized;

    void modifySingleFormat(const bool isRowIdFormat, const QString& filter, const QModelIndex refIndex,
                            std::function<void(CondFormat&)> changeFunction);
    void modifyFormat(std::function<void(CondFormat&)> changeFunction);

    sqlb::Query buildQuery(const BrowseDataTableSettings& storedData, const sqlb::ObjectIdentifier& tablename) const;
    void applyModelSettings(const BrowseDataTableSettings& storedData, const sqlb::Query& query);
    void applyViewportSettings(const BrowseDataTableSettings& storedData, const sqlb::ObjectIdentifier& tablename);
    void generateFilters();
};

#endif
