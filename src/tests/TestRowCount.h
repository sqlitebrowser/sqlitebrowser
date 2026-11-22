#ifndef TESTROWCOUNT_H
#define TESTROWCOUNT_H

#include <QObject>

class TestRowCount : public QObject
{
    Q_OBJECT

private slots:
    void rowCountOfOrdinaryQuery();
    void rowCountBeyondIntMaxIsClamped_data();
    void rowCountBeyondIntMaxIsClamped();
};

#endif
