#ifndef TESTGLOBALFILTER_H
#define TESTGLOBALFILTER_H

#include <QObject>

class TestGlobalFilter : public QObject
{
    Q_OBJECT

private slots:
    void tokenizes_data();
    void tokenizes();
    void issueFixtureReturnsNewYork();
    void ordinaryTermsRemainAndConnected();
    void projectRoundTripPreservesRegexTerm();
};

#endif // TESTGLOBALFILTER_H
