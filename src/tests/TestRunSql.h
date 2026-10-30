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
};

#endif // TESTRUNSQL_H
