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
    QCoreApplication* app;

private slots:
    void csvImport();
    void csvImport_data();
};

#endif
