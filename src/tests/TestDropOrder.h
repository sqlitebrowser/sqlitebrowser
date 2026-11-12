#ifndef TESTDROPORDER_H
#define TESTDROPORDER_H

#include <QObject>

class TestDropOrder : public QObject
{
    Q_OBJECT

private slots:
    void dropStatementMatchesSingleDelete();
    void dependantsComeFirst();
    void duplicatesAndUnknownTypesAreRemoved();
    void sameTypeKeepsSelectionOrder();
    void orderedStatementsExecuteWithoutErrors();
    void naiveSelectionOrderFails();
};

#endif // TESTDROPORDER_H
