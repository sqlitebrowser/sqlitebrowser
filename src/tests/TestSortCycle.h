#ifndef TESTSORTCYCLE_H
#define TESTSORTCYCLE_H

#include <QObject>

class TestSortCycle : public QObject
{
    Q_OBJECT

private slots:
    void firstClickSortsAscending();
    void singleClickToggleKeepsTwoStates();
    void singleClickCycleEndsWithoutSorting();
    void singleClickOnOtherColumnReplacesSorting();
    void multiColumnToggleKeepsTwoStates();
    void multiColumnCycleRemovesOnlyThatColumn();
    void multiColumnAppendsNewColumn();
};

#endif // TESTSORTCYCLE_H
