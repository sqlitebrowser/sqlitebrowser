#ifndef TESTPLOTDOCK_H
#define TESTPLOTDOCK_H

#include <QObject>
#include <QTemporaryDir>

class TestPlotDock : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void loadAllDataButtonTurnsRed_data();
    void loadAllDataButtonTurnsRed();
    void loadAllDataButtonIsPlainWhenAllDataIsLoaded();

private:
    QTemporaryDir m_settingsDir;
};

#endif // TESTPLOTDOCK_H
