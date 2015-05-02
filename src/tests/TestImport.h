#ifndef TESTIMPORT_H
#define TESTIMPORT_H

#include <QObject>

class TestImport : public QObject
{
    Q_OBJECT

public:
    TestImport();
    ~TestImport();

private slots:
    void csvImport();
    void csvImport_data();
};

#endif
