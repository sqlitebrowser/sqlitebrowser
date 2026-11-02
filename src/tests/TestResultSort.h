#ifndef TESTRESULTSORT_H
#define TESTRESULTSORT_H

#include <QObject>
#include <QTemporaryDir>

class TestResultSort : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir settingsDir;

private slots:
    void initTestCase();
    void sortsByColumn();
    void keepsOrderByAndLimitOfTheQuery();
    void sortsByTheRightOfTwoColumnsWithTheSameName();
    void ignoresTrailingSemicolonAndComments();
    void sortsWithStatements();
    void clearingTheSortRestoresTheOriginalQuery();
    void cyclesThroughAscendingDescendingAndNone();
    void showsSortIndicatorInTheHeader();
    void exportsTheSortedQuery();
    void doesNotSortStatementsThatCannotBeWrapped_data();
    void doesNotSortStatementsThatCannotBeWrapped();
    void newQueryResetsTheSort();
};

#endif // TESTRESULTSORT_H
