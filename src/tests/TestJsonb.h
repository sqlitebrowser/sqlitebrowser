#ifndef TESTJSONB_H
#define TESTJSONB_H

#include <QObject>

class TestJsonb : public QObject
{
    Q_OBJECT

private slots:
    void convertsObjectAndArray();
    void rejectsOtherData();
};

#endif // TESTJSONB_H
