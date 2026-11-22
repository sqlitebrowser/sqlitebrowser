#ifndef TESTTABLEMODEL_H
#define TESTTABLEMODEL_H

#include <QObject>

class TestTableModel : public QObject
{
    Q_OBJECT

private slots:
    void setDataOnCachedRow();
    void setDataOnUncachedRowLoadsRowAndUpdates();
    void queryThatCannotBePreparedReportsError();
};

#endif // TESTTABLEMODEL_H
