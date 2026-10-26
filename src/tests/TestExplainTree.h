#ifndef TESTEXPLAINTREE_H
#define TESTEXPLAINTREE_H

#include <QObject>

class TestExplainTree : public QObject
{
    Q_OBJECT

private slots:
    void detectsExplainQueryPlan();
    void flatPlan();
    void nestedPlan();
    void outOfOrderIds();
    void unknownParentIsTopLevel();
    void cycleDoesNotLoop();
    void emptyInput();
    void indentText();
    void realPlanFromSqlite();
    void delegateIndentsDetailColumn();
};

#endif
