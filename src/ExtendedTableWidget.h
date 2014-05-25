#ifndef __EXTENDEDTABLEWIDGET_H__
#define __EXTENDEDTABLEWIDGET_H__

#include <QTableView>
#include "FilterTableHeader.h"

class ExtendedTableWidget : public QTableView
{
    Q_OBJECT

public:
    explicit ExtendedTableWidget(QWidget* parent = 0);

    FilterTableHeader* filterHeader() { return m_tableHeader; }

private:
    void copy();
    int numVisibleRows();

private slots:
    void vscrollbarChanged(int value);

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void updateGeometries();

    FilterTableHeader* m_tableHeader;
};

#endif
