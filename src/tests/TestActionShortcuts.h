#ifndef TESTACTIONSHORTCUTS_H
#define TESTACTIONSHORTCUTS_H

#include <QObject>

class TestActionShortcuts : public QObject
{
    Q_OBJECT

private slots:
    void recordActionsHaveShortcuts_data();
    void recordActionsHaveShortcuts();
    void shortcutsAreUnique();
    void shortcutsDoNotShadowStandardKeys();
};

#endif // TESTACTIONSHORTCUTS_H
