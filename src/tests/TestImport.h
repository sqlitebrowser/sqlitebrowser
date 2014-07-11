#ifndef __TESTIMPORT_H__
#define __TESTIMPORT_H__

#include <QObject>

class TestImport : public QObject
{
    Q_OBJECT

private slots:
    void csvImport();
    void csvImport_data();
};

#endif
