#ifndef EXTENDEDTABLEWIDGET_H
#define EXTENDEDTABLEWIDGET_H

#include <QTableView>
#include <QStyledItemDelegate>
#include <QSortFilterProxyModel>
#include <unordered_set>
#include <set>

#include "sql/Query.h"

class QMenu;
class QMimeData;
class QDropEvent;
class QDragMoveEvent;

class FilterTableHeader;
class ItemBorderDelegate;
namespace sqlb { class ObjectIdentifier; }

// Filter proxy model that only accepts distinct non-empty values.
class UniqueFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit UniqueFilterModel(QObject* parent = nullptr);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
 private:
    std::unordered_set<std::string> m_uniqueValues;
};

// We use this class to provide editor widgets for the ExtendedTableWidget. It's used for every cell in the table view.
class ExtendedTableWidgetEditorDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ExtendedTableWidgetEditorDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

signals:
    void dataAboutToBeEdited(QModelIndex index) const;
};

class ExtendedTableWidget : public QTableView
{
    Q_OBJECT

public:
    explicit ExtendedTableWidget(QWidget* parent = nullptr);
    ~ExtendedTableWidget() override;

    FilterTableHeader* filterHeader() { return m_tableHeader; }
    void generateFilters(size_t number, bool show_rowid);

public:
    // Get set of selected columns (all cells in column has to be selected)
    std::unordered_set<size_t> selectedCols() const;
    // Get set of columns traversed by selection (only some cells in column have to be selected)
    std::unordered_set<size_t> colsInSelection() const;
    // Get set of ordered rows traversed by selection (only some cells in row have to be selected)
    std::set<size_t> rowsInSelection() const;

    int numVisibleRows() const;

    void sortByColumns(const std::vector<sqlb::OrderBy>& columns);

    void setFrozenColumns(size_t count);

    void setModel(QAbstractItemModel* item_model) override;

    void setEditTriggers(QAbstractItemView::EditTriggers editTriggers);

public slots:
    void reloadSettings();
    void selectTableLine(int lineToSelect);
    void selectTableLines(int firstLine, int count);
    void selectAll() override;
    void openPrintDialog();
    void setFilter(size_t column, const QString& value);

signals:
    void foreignKeyClicked(const sqlb::ObjectIdentifier& table, const std::string& column, const QByteArray& value);
    void switchTable(bool next);    // 'next' parameter is set to true if next table should be selected and to false if previous table should be selected
    void openFileFromDropEvent(QString);
    void selectedRowsToBeDeleted();
    void editCondFormats(int column);
    void currentIndexChanged(const QModelIndex &current, const QModelIndex &previous);
    void dataAboutToBeEdited(const QModelIndex& index);
    void requestUrlOrFileOpen(const QString& urlString);

private:
    void copyMimeData(const QModelIndexList& fromIndices, QMimeData* mimeData, const bool withHeaders, const bool inSQL);
    void copy(const bool withHeaders, const bool inSQL);
    void paste();
    void cut();

    void useAsFilter(const QString& filterOperator, bool binary = false, const QString& operatorSuffix = QString());
    void duplicateUpperCell();

    void setToNull(const QModelIndexList& indices);

    static std::vector<std::vector<QByteArray>> m_buffer;
    static QString m_generatorStamp;

    ExtendedTableWidget* m_frozen_table_view;
    size_t m_frozen_column_count;
    ItemBorderDelegate* m_item_border_delegate;
    void updateFrozenTableGeometry();

private slots:
    void vscrollbarChanged(int value);
    void cellClicked(const QModelIndex& index);
    void updateSectionWidth(int logicalIndex, int oldSize, int newSize);
    void updateSectionHeight(int logicalIndex, int oldSize, int newSize);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void updateGeometries() override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

    void resizeEvent(QResizeEvent* event) override;
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
    void scrollTo(const QModelIndex& index, ScrollHint hint = EnsureVisible) override;

    FilterTableHeader* m_tableHeader;
    QMenu* m_contextMenu;
    ExtendedTableWidgetEditorDelegate* m_editorDelegate;
};

#endif
