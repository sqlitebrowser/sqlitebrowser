#ifndef TESTIMPORTCSVOPTIONS_H
#define TESTIMPORTCSVOPTIONS_H

#include <QObject>
#include <QTemporaryDir>

class TestImportCsvOptions : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    void schemaChoices();
    void importIntoSchema_data();
    void importIntoSchema();
    void importIgnoresSameNameInOtherSchema();
    void importIntoMainWhenTempHasSameName();

    void matchColumnsByName_data();
    void matchColumnsByName();
    void matchColumnsByNameNeedsHeader();

private:
    QTemporaryDir m_dir;
};

#endif
