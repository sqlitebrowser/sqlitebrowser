#ifndef TESTIMPORT_H
#define TESTIMPORT_H

#include <QObject>

class TestImport : public QObject
{
    Q_OBJECT

private slots:
    void csvImport();
    void csvImport_data();
};

#endif
