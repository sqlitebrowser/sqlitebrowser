#ifndef EXTENDEDTABLEWIDGET_H
#define EXTENDEDTABLEWIDGET_H

#include <QTableView>
#include "FilterTableHeader.h"
#include <QSet>
#include <QDropEvent>
#include <QDragMoveEvent>

class ExtendedTableWidget : public QTableView
{
    Q_OBJECT

public:
    explicit ExtendedTableWidget(QWidget* parent = 0);

    FilterTableHeader* filterHeader() { return m_tableHeader; }

public:
    QSet<int> selectedCols();

signals:
    void foreignKeyClicked(const QString& table, const QString& column, const QByteArray& value);

private:
    void copy();
    void paste();
    int numVisibleRows();

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
};

#endif
