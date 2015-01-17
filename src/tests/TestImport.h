#ifndef TESTIMPORT_H
#define TESTIMPORT_H

#include <QObject>
#include <QApplication>

class TestImport : public QObject
{
    Q_OBJECT

public:
    TestImport();
    ~TestImport();

private:
    int argcount;
    char *args[1]; // the size must match what 'argcount' is set to
    QCoreApplication* app;

private slots:
    void csvImport();
    void csvImport_data();
};

#endif
