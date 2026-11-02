#ifndef TESTLOCALENUMBERS_H
#define TESTLOCALENUMBERS_H

#include <QObject>
#include <QTemporaryDir>

class TestLocaleNumbers : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();

    void offByDefault();
    void formatsNumbers_data();
    void formatsNumbers();
    void keepsEditedValue();
    void followsLocale_data();
    void followsLocale();
    void keepsDisplayFormat();
    void leavesSqlResults();

private:
    QTemporaryDir m_dir;
};

#endif
