#ifndef EXTENDEDTABLEWIDGET_H
#define EXTENDEDTABLEWIDGET_H

#include <QTableView>
#include <QSet>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QStyledItemDelegate>
#include <QSortFilterProxyModel>

class QMenu;
class QMimeData;
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
    QSet<QString> m_uniqueValues;
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
    QSet<int> selectedCols();
    int numVisibleRows();

public slots:
    void reloadSettings();
    void selectTableLine(int lineToSelect);
    void selectTableLines(int firstLine, int count);

signals:
    void foreignKeyClicked(const sqlb::ObjectIdentifier& table, const QString& column, const QByteArray& value);
    void switchTable(bool next);    // 'next' parameter is set to true if next table should be selected and to false if previous table should be selected
    void openFileFromDropEvent(QString);
    void selectedRowsToBeDeleted();

private:
    void copyMimeData(const QModelIndexList& fromIndices, QMimeData* mimeData, const bool withHeaders, const bool inSQL);
    void copy(const bool withHeaders, const bool inSQL);
    void paste();
    QString escapeCopiedData(const QByteArray& data) const;
    void openPrintDialog();

    void useAsFilter(const QString& filterOperator, bool binary = false);
    void duplicateUpperCell();

    typedef QList<QByteArray> QByteArrayList;
    static QList<QByteArrayList> m_buffer;
    static QString m_generatorStamp;

private slots:
    void vscrollbarChanged(int value);
    void cellClicked(const QModelIndex& index);

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void updateGeometries();
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dragMoveEvent(QDragMoveEvent* event);
    virtual void dropEvent(QDropEvent* event);

    FilterTableHeader* m_tableHeader;
    QMenu* m_contextMenu;
    ExtendedTableWidgetEditorDelegate* m_editorDelegate;
};

#endif
