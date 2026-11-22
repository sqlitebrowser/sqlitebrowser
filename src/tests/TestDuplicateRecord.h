#ifndef TESTDUPLICATERECORD_H
#define TESTDUPLICATERECORD_H

#include <QObject>

class TestDuplicateRecord : public QObject
{
    Q_OBJECT

private slots:
    void duplicateKeepsExistingRecords_data();
    void duplicateKeepsExistingRecords();
    void duplicateTwiceWithoutRowidCompositeKey();
    void duplicateSkipsUniqueColumns();
};

#endif // TESTDUPLICATERECORD_H
