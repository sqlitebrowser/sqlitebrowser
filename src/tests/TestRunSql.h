#ifndef TESTRUNSQL_H
#define TESTRUNSQL_H

#include <QObject>

class TestRunSql : public QObject
{
    Q_OBJECT

private slots:
    // Executing a part of the SQL code (e.g. the current line) with multibyte characters in front of it
    void executeRangeAfterMultibyteText_data();
    void executeRangeAfterMultibyteText();
    void executeEverythingWithMultibyteText();
    void executeBlankRangeAfterMultibyteText();

    // Executing the current line must not run any statement that starts after the end of the line
    void executeCurrentLine_data();
    void executeCurrentLine();

    void getCurrentLineRange_data();
    void getCurrentLineRange();
    void findStatementStart_data();
    void findStatementStart();
};

#endif // TESTRUNSQL_H
