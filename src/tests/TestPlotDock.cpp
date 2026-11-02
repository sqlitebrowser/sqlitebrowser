#include "TestPlotDock.h"

#include "../PlotDock.h"
#include "../Settings.h"
#include "../sqlitedb.h"
#include "../sqlitetablemodel.h"
#include "../sql/Query.h"

#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QProxyStyle>
#include <QSignalSpy>
#include <QStyleFactory>
#include <QStyleOption>
#include <QToolButton>
#include <QtTest/QtTest>

#include <algorithm>

QTEST_MAIN(TestPlotDock)

namespace
{

// Paints the background of tool buttons in a fixed colour whatever the palette says. Some platform themes and styles (e.g. the
// GTK ones) do not take the button colour from the palette either, which is what a style sheet without a border relies on.
class IgnoresPaletteStyle : public QProxyStyle
{
public:
    explicit IgnoresPaletteStyle(QStyle* base) : QProxyStyle(base) {}

    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override
    {
        if(element == PE_PanelButtonTool || element == PE_PanelButtonCommand)
            painter->fillRect(option->rect, QColor(200, 200, 200));
        else
            QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

QStyle* createStyle(const QString& name)
{
    if(name == "ignores palette")
        return new IgnoresPaletteStyle(QStyleFactory::create("Fusion"));
    return QStyleFactory::create(name);
}

// The share of the pixels of the widget which are clearly red (as opposed to grey or white)
double redShare(QWidget* widget)
{
    const QImage image = widget->grab().toImage();
    int red = 0;
    for(int y = 0; y < image.height(); ++y)
    {
        for(int x = 0; x < image.width(); ++x)
        {
            const QColor c = image.pixelColor(x, y);
            if(c.red() >= 200 && c.red() - std::max(c.green(), c.blue()) >= 40)
                ++red;
        }
    }
    return static_cast<double>(red) / (image.width() * image.height());
}

}

void TestPlotDock::initTestCase()
{
    // Keep the settings away from the ones of the user and make the model fetch its data in small chunks
    QVERIFY(m_settingsDir.isValid());
    Settings::setUserSettingsFile(m_settingsDir.filePath("settings.ini"));
    Settings::setValue("db", "prefetchsize", 10, false);
}

void TestPlotDock::loadAllDataButtonTurnsRed_data()
{
    QTest::addColumn<QString>("style");

    QTest::newRow("Fusion") << "Fusion";
    QTest::newRow("Windows") << "Windows";
    QTest::newRow("native style which ignores the palette") << "ignores palette";
}

void TestPlotDock::loadAllDataButtonTurnsRed()
{
    QFETCH(QString, style);

    QApplication::setStyle(createStyle(style));

    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    DBBrowserDB db;
    QVERIFY(db.create(dir.filePath("test.db")));
    QVERIFY(db.executeSQL("CREATE TABLE t(a INTEGER PRIMARY KEY, b REAL)"));
    QVERIFY(db.executeSQL("WITH RECURSIVE c(x) AS (SELECT 1 UNION ALL SELECT x+1 FROM c WHERE x<100) INSERT INTO t SELECT x, x*1.5 FROM c"));
    db.updateSchema();

    // More rows than fit in a single chunk, so that not all of the data has been fetched
    SqliteTableModel model(db, nullptr, QString(), true);
    QSignalSpy rowCountSpy(&model, &SqliteTableModel::finishedRowCount);
    model.setQuery(sqlb::Query(sqlb::ObjectIdentifier("main", "t")));
    QVERIFY(rowCountSpy.wait(5000) || rowCountSpy.count() > 0);
    model.waitUntilIdle();
    QCOMPARE(model.rowCount(), 100);
    QVERIFY(!model.isCacheComplete());

    PlotDock dock;
    dock.updatePlot(&model, nullptr, true, true);
    dock.show();
    QVERIFY(QTest::qWaitForWindowExposed(&dock));

    auto button = dock.findChild<QToolButton*>("buttonLoadAllData");
    QVERIFY(button);
    QVERIFY(button->isEnabled());
    QVERIFY2(redShare(button) > 0.5, "The button which loads the rest of the data does not stand out in red");
}

void TestPlotDock::loadAllDataButtonIsPlainWhenAllDataIsLoaded()
{
    QApplication::setStyle(createStyle("Fusion"));

    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    DBBrowserDB db;
    QVERIFY(db.create(dir.filePath("test.db")));
    QVERIFY(db.executeSQL("CREATE TABLE t(a INTEGER PRIMARY KEY, b REAL)"));
    QVERIFY(db.executeSQL("INSERT INTO t VALUES(1, 1.5), (2, 3.0), (3, 4.5)"));
    db.updateSchema();

    SqliteTableModel model(db, nullptr, QString(), true);
    QSignalSpy rowCountSpy(&model, &SqliteTableModel::finishedRowCount);
    model.setQuery(sqlb::Query(sqlb::ObjectIdentifier("main", "t")));
    QVERIFY(rowCountSpy.wait(5000) || rowCountSpy.count() > 0);
    model.waitUntilIdle();
    QVERIFY(model.isCacheComplete());

    PlotDock dock;
    dock.updatePlot(&model, nullptr, true, true);
    dock.show();
    QVERIFY(QTest::qWaitForWindowExposed(&dock));

    auto button = dock.findChild<QToolButton*>("buttonLoadAllData");
    QVERIFY(button);
    QVERIFY(!button->isEnabled());
    QVERIFY(button->styleSheet().isEmpty());
    QVERIFY(redShare(button) < 0.1);
}
