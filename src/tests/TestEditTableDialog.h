#ifndef TESTEDITTABLEDIALOG_H
#define TESTEDITTABLEDIALOG_H

#include <QObject>

class TestEditTableDialog : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void showsGeneratedColumns();
    void addVirtualGeneratedColumn();
    void addStoredGeneratedColumn();
    void changeGeneratedExpression();
    void switchBetweenVirtualAndStored();
    void convertOrdinaryColumnToGenerated();
    void convertGeneratedColumnToOrdinary();
    void createTableWithGeneratedColumn();
    void generatedColumnCannotBePrimaryKey();
    void generatedColumnCannotHaveDefault();
};

#endif
