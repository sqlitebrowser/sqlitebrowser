#include "TestExtendedTableWidget.h"

#include "../ExtendedTableWidget.h"
#include "../Settings.h"

#include <QHeaderView>
#include <QStandardItemModel>
#include <QTemporaryDir>
#include <QtTest/QtTest>

QTEST_MAIN(TestExtendedTableWidget)

namespace
{

// Three columns of increasing width: the first one is as narrow as a column can get, the last one is far wider than
// any limit used in the tests
void fillModel(QStandardItemModel& model)
{
    model.setHorizontalHeaderLabels({"a", "b", "c"});
    for(int row = 0; row < model.rowCount(); ++row)
    {
        model.setItem(row, 0, new QStandardItem("x"));
        model.setItem(row, 1, new QStandardItem(QString(15, 'W')));
        model.setItem(row, 2, new QStandardItem(QString(400, 'W')));
    }
}

// The default row height the data browser font gets when the given padding is added to it
int expectedRowHeight(int padding)
{
    QFont font(Settings::getValue("databrowser", "font").toString());
    font.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
    return QFontMetrics(font).height() + padding;
}

// Make every column wider than its contents so a test can tell whether the columns have been resized
void widenColumns(ExtendedTableWidget& table, int width)
{
    for(int col = 0; col < table.model()->columnCount(); ++col)
        table.setColumnWidth(col, width);
}

}

void TestExtendedTableWidget::cleanup()
{
    Settings::clearValue("databrowser", "row_padding");
}

void TestExtendedTableWidget::initTestCase()
{
    // Never read or write the settings of the user running the tests
    QVERIFY(m_settingsDir.isValid());
    Settings::setUserSettingsFile(m_settingsDir.filePath("settings.ini"));
}

void TestExtendedTableWidget::fitColumnsToContentsCapsWideColumns()
{
    QStandardItemModel model(3, 3);
    fillModel(model);
    ExtendedTableWidget table;
    table.setModel(&model);

    // What Qt itself considers the width of each column when fitting it to its contents
    table.resizeColumnsToContents();
    const int natural0 = table.columnWidth(0);
    const int natural1 = table.columnWidth(1);
    const int natural2 = table.columnWidth(2);
    if(!(natural0 < natural1 && natural1 + 100 < natural2))
        QSKIP("The font of this platform does not give the test columns distinguishable widths");

    const int limit = natural1 + 40;
    widenColumns(table, natural2 + 100);

    table.fitColumnsToContents(limit);

    QCOMPARE(table.columnWidth(0), natural0);
    QCOMPARE(table.columnWidth(1), natural1);
    QCOMPARE(table.columnWidth(2), limit);
}

void TestExtendedTableWidget::fitColumnsToContentsKeepsHiddenColumnsHidden()
{
    QStandardItemModel model(3, 3);
    fillModel(model);
    ExtendedTableWidget table;
    table.setModel(&model);

    table.resizeColumnsToContents();
    const int natural0 = table.columnWidth(0);
    const int natural1 = table.columnWidth(1);
    widenColumns(table, natural1 + 100);
    table.setColumnHidden(2, true);

    table.fitColumnsToContents(10000);

    QVERIFY(table.isColumnHidden(2));
    QCOMPARE(table.columnWidth(2), 0);
    QVERIFY(!table.isColumnHidden(0));
    QVERIFY(!table.isColumnHidden(1));
    QCOMPARE(table.columnWidth(0), natural0);
    QCOMPARE(table.columnWidth(1), natural1);
}

void TestExtendedTableWidget::fitColumnsToContentsWithoutModel()
{
    ExtendedTableWidget table;
    table.fitColumnsToContents(300);
    QCOMPARE(table.horizontalHeader()->count(), 0);
}

void TestExtendedTableWidget::rowHeightPaddingDefaultsToCurrentBehaviour()
{
    // The rows have always been 10 pixels higher than the font
    QCOMPARE(Settings::getValue("databrowser", "row_padding").toInt(), 10);

    ExtendedTableWidget table;
    table.reloadSettings();
    QCOMPARE(table.verticalHeader()->defaultSectionSize(), expectedRowHeight(10));
}

void TestExtendedTableWidget::rowHeightFollowsRowHeightPadding()
{
    ExtendedTableWidget table;

    Settings::setValue("databrowser", "row_padding", 2, false);
    table.reloadSettings();
    QCOMPARE(table.verticalHeader()->defaultSectionSize(), expectedRowHeight(2));

    Settings::setValue("databrowser", "row_padding", 0, false);
    table.reloadSettings();
    QCOMPARE(table.verticalHeader()->defaultSectionSize(), expectedRowHeight(0));

    Settings::setValue("databrowser", "row_padding", 25, false);
    table.reloadSettings();
    QCOMPARE(table.verticalHeader()->defaultSectionSize(), expectedRowHeight(25));
}

void TestExtendedTableWidget::rowHeightPaddingIsNeverNegative()
{
    // A hand-edited settings file must not be able to shrink the rows below the font height
    ExtendedTableWidget table;
    Settings::setValue("databrowser", "row_padding", -7, false);
    table.reloadSettings();
    QCOMPARE(table.verticalHeader()->defaultSectionSize(), expectedRowHeight(0));
}
