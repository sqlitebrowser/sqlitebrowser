#ifndef __EXTENDEDTABLEWIDGET_H__
#define __EXTENDEDTABLEWIDGET_H__

#include <QTableWidget>

class ExtendedTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit ExtendedTableWidget(int rows, int columns, QWidget* parent = 0);
    explicit ExtendedTableWidget(QWidget* parent = 0);

private:
    void copy();

protected:
    virtual void keyPressEvent(QKeyEvent* event);
};

#endif
