#ifndef TESTEXTENDEDTABLEWIDGET_H
#define TESTEXTENDEDTABLEWIDGET_H

#include <QObject>
#include <QTemporaryDir>

class TestExtendedTableWidget : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void fitColumnsToContentsCapsWideColumns();
    void fitColumnsToContentsKeepsHiddenColumnsHidden();
    void fitColumnsToContentsWithoutModel();
    void rowHeightPaddingDefaultsToCurrentBehaviour();
    void rowHeightFollowsRowHeightPadding();
    void rowHeightPaddingIsNeverNegative();
    void cleanup();

private:
    QTemporaryDir m_settingsDir;
};

#endif
