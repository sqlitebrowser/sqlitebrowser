#include "TestSettings.h"

#include "../Settings.h"

#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QtTest/QtTest>

QTEST_MAIN(TestSettings)

QString TestSettings::writeFile(const QString& name, const QByteArray& content) const
{
    const QString path = m_dir.filePath(name);
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly) || file.write(content) != content.size())
        qFatal("Could not write the test file %s", qPrintable(path));
    return path;
}

void TestSettings::initTestCase()
{
    QVERIFY(m_dir.isValid());

    // The settings which are not stored in the settings file given by the user must not end up in the ones of the real user
    QCoreApplication::setOrganizationName("sqlitebrowser-tests");
    QCoreApplication::setApplicationName("TestSettings");
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, m_dir.filePath("native"));
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_dir.filePath("native"));

    // A settings file which has been created in advance but which doesn't contain anything yet, as it is the case with a
    // brand new settings file for a portable installation. It is used for all settings from here on.
    m_userSettingsFile = writeFile("user-settings.ini", QByteArray());
    Settings::setUserSettingsFile(m_userSettingsFile);
}

void TestSettings::userSettingsFileMayBeEmpty()
{
    Settings::setValue("test", "stored_in_the_given_file", 42);
    Settings::sync();

    QFile file(m_userSettingsFile);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QVERIFY2(file.readAll().contains("stored_in_the_given_file=42"), "The settings file given by the user was not used");
}

void TestSettings::importAcceptsEmptyFile()
{
    QVERIFY(Settings::importSettings(writeFile("empty.ini", QByteArray())));
}

void TestSettings::importAcceptsSettingsFile()
{
    const QString path = writeFile("normal.ini", "[%General]\nlanguage=de\n\n[db]\nprefetchsize=123\n");

    QVERIFY(Settings::importSettings(path));
    QCOMPARE(Settings::getValue("db", "prefetchsize").toInt(), 123);
}

void TestSettings::importRejectsOtherFiles_data()
{
    QTest::addColumn<QByteArray>("content");

    QTest::newRow("text") << QByteArray("This is not a settings file\n");
    QTest::newRow("csv") << QByteArray("a,b,c\n1,2,3\n");
    QTest::newRow("sqlite database") << QByteArray("SQLite format 3\0\x10\0\x01\x01\0\x40\x20\x20", 16);
}

void TestSettings::importRejectsOtherFiles()
{
    QFETCH(QByteArray, content);

    QVERIFY(!Settings::importSettings(writeFile("other.ini", content)));
}
