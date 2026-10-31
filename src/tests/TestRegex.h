#ifndef TESTREGEX_H
#define TESTREGEX_H

#define REGEX_UNIT_TEST

#include <QObject>

class TestRegex : public QObject
{
    Q_OBJECT

private slots:
    void sqlQueryComments();
    void sqlQueryComments_data();
    void clipboardRows();
    void clipboardRows_data();
    void writeAllReportsPartialWrites();
    void writeAllWritesEverything();
    void flushStreamReportsFailureOfLastChunk();
    void flushStreamReportsEarlierFailure();
    void flushStreamWritesEverything();
};

#endif
