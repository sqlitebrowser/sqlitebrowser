#ifndef TESTIMPORTCSVDIALOG_H
#define TESTIMPORTCSVDIALOG_H

#include <QObject>
#include <QTemporaryDir>

class TestImportCsvDialog : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    void quotedValues_data();
    void quotedValues();

private:
    QTemporaryDir m_dir;
};

#endif
