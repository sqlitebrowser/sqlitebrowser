#ifndef TESTSETTINGS_H
#define TESTSETTINGS_H

#include <QObject>
#include <QTemporaryDir>

class TestSettings : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void userSettingsFileMayBeEmpty();
    void importAcceptsEmptyFile();
    void importAcceptsSettingsFile();
    void importRejectsOtherFiles_data();
    void importRejectsOtherFiles();

private:
    QString writeFile(const QString& name, const QByteArray& content) const;

    QTemporaryDir m_dir;
    QString m_userSettingsFile;
};

#endif // TESTSETTINGS_H
