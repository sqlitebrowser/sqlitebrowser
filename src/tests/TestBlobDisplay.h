#ifndef TESTBLOBDISPLAY_H
#define TESTBLOBDISPLAY_H

#include <QObject>
#include <QTemporaryDir>

class TestBlobDisplay : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir settingsDir;

private slots:
    void initTestCase();
    void init();

    // This one has to run first because it checks the setting before any other test changes it
    void isOffByDefault();
    void showsPlaceholderWhenOff();
    void showsSmallBlobsAsHex();
    void usesTheConfiguredPlaceholderForLargerBlobs();
    void treatsTheLimitAsInclusive();
    void doesNotChangeOtherValues();
    void doesNotChangeTheEditRole();
    void cutsLongHexAtTheSymbolLimit();
};

#endif // TESTBLOBDISPLAY_H
