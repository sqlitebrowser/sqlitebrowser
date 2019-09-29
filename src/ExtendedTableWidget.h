#ifndef EXTENDEDTABLEWIDGET_H
#define EXTENDEDTABLEWIDGET_H

#include <QTableView>
#include <QStyledItemDelegate>
#include <QSortFilterProxyModel>
#include <unordered_set>

#include "sql/Query.h"

class QMenu;
class QMimeData;
class QDropEvent;
class QDragMoveEvent;

class FilterTableHeader;
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
};

class ExtendedTableWidget : public QTableView
{
    Q_OBJECT

public:
    explicit ExtendedTableWidget(QWidget* parent = nullptr);

    FilterTableHeader* filterHeader() { return m_tableHeader; }

public:
    // Get set of selected columns (all cells in column has to be selected)
    std::unordered_set<int> selectedCols() const;
    // Get set of columns traversed by selection (only some cells in column has to be selected)
    std::unordered_set<int> colsInSelection() const;

    int numVisibleRows() const;

    void sortByColumns(const std::vector<sqlb::SortedColumn>& columns);

public slots:
    void reloadSettings();
    void selectTableLine(int lineToSelect);
    void selectTableLines(int firstLine, int count);
    void selectAll() override;
    void openPrintDialog();

signals:
    void foreignKeyClicked(const sqlb::ObjectIdentifier& table, const QString& column, const QByteArray& value);
    void switchTable(bool next);    // 'next' parameter is set to true if next table should be selected and to false if previous table should be selected
    void openFileFromDropEvent(QString);
    void selectedRowsToBeDeleted();
    void editCondFormats(int column);
    // Make the inherited protected signal public
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    void copyMimeData(const QModelIndexList& fromIndices, QMimeData* mimeData, const bool withHeaders, const bool inSQL);
    void copy(const bool withHeaders, const bool inSQL);
    void paste();

    void useAsFilter(const QString& filterOperator, bool binary = false, const QString& operatorSuffix = "");
    void duplicateUpperCell();

    static std::vector<std::vector<QByteArray>> m_buffer;
    static QString m_generatorStamp;

private slots:
    void vscrollbarChanged(int value);
    void cellClicked(const QModelIndex& index);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void updateGeometries() override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

    FilterTableHeader* m_tableHeader;
    QMenu* m_contextMenu;
    ExtendedTableWidgetEditorDelegate* m_editorDelegate;
};

#endif
