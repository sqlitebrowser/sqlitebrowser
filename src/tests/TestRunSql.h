#ifndef TESTRUNSQL_H
#define TESTRUNSQL_H

#include <QObject>

class TestRunSql : public QObject
{
    Q_OBJECT

private slots:
    void queryTypeIgnoresLeadingComments_data();
    void queryTypeIgnoresLeadingComments();
    void vacuumPrecededByComment();
    void vacuumFollowingModifyingStatementWithComment();
    void vacuumInto();
    void commentedVacuumIsNotVacuum();
};

#endif // TESTRUNSQL_H
