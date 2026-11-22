#ifndef TESTROWLOADER_H
#define TESTROWLOADER_H

#include <QObject>

class TestRowLoader : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void queryRows_data();
    void queryRows();
    void valuesBeyondFirstChunk();
    void otherQueriesKeepTheirStatement_data();
    void otherQueriesKeepTheirStatement();
};

#endif // TESTROWLOADER_H
