#ifndef EXTENDEDTABLEWIDGET_H
#define EXTENDEDTABLEWIDGET_H

#include <QTableView>
#include <QSet>
#include <QDropEvent>
#include <QDragMoveEvent>

class QMenu;
class FilterTableHeader;
namespace sqlb { class ObjectIdentifier; }

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

signals:
    void foreignKeyClicked(const sqlb::ObjectIdentifier& table, const QString& column, const QByteArray& value);
    void switchTable(bool next);    // 'next' parameter is set to true if next table should be selected and to false if previous table should be selected
    void openFileFromDropEvent(QString);

private:
    void copy(const bool withHeaders = false);
    void paste();
    QString escapeCopiedData(const QByteArray& data) const;

    void useAsFilter();

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
};

#endif
