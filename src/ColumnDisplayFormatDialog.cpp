#include <QMessageBox>

#include "ColumnDisplayFormatDialog.h"
#include "ui_ColumnDisplayFormatDialog.h"
#include "sql/sqlitetypes.h"
#include "sqlitedb.h"

ColumnDisplayFormatDialog::ColumnDisplayFormatDialog(DBBrowserDB& db, const sqlb::ObjectIdentifier& tableName, const QString& colname, const QString& current_format, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::ColumnDisplayFormatDialog),
      column_name(colname),
      pdb(db),
      curTable(tableName)
{
    // Create UI
    ui->setupUi(this);
    ui->comboDisplayFormat->addItem(tr("Default"), "default");
    ui->comboDisplayFormat->insertSeparator(ui->comboDisplayFormat->count());
    ui->comboDisplayFormat->addItem(tr("Decimal number"), "decimal");
    ui->comboDisplayFormat->addItem(tr("Exponent notation"), "exponent");
    ui->comboDisplayFormat->addItem(tr("Hex blob"), "hexblob");
    ui->comboDisplayFormat->addItem(tr("Hex number"), "hex");
    ui->comboDisplayFormat->addItem(tr("Octal number"), "octal");
    ui->comboDisplayFormat->addItem(tr("Round number"), "round");
    ui->comboDisplayFormat->insertSeparator(ui->comboDisplayFormat->count());
    ui->comboDisplayFormat->addItem(tr("Apple NSDate to date"), "appleDate");
    ui->comboDisplayFormat->addItem(tr("Java epoch (milliseconds) to date"), "javaEpoch");
    ui->comboDisplayFormat->addItem(tr(".NET DateTime.Ticks to date"), "dotNetTicks");
    ui->comboDisplayFormat->addItem(tr("Julian day to date"), "julian");
    ui->comboDisplayFormat->addItem(tr("Unix epoch to date"), "epoch");
    ui->comboDisplayFormat->addItem(tr("Unix epoch to local time"), "epochLocalTime");
    ui->comboDisplayFormat->addItem(tr("WebKit / Chromium epoch to date"), "webkitEpoch");
    ui->comboDisplayFormat->addItem(tr("WebKit / Chromium epoch to local time"), "webkitEpochLocalTime");
    ui->comboDisplayFormat->addItem(tr("Windows DATE to date"), "winDate");
    ui->comboDisplayFormat->addItem(tr("Date as dd/mm/yyyy"), "ddmmyyyyDate");
    ui->comboDisplayFormat->insertSeparator(ui->comboDisplayFormat->count());
    ui->comboDisplayFormat->addItem(tr("Lower case"), "lower");
    ui->comboDisplayFormat->addItem(tr("Upper case"), "upper");
    ui->comboDisplayFormat->insertSeparator(ui->comboDisplayFormat->count());
    ui->comboDisplayFormat->addItem(tr("Binary GUID to text"), "guid");
    ui->comboDisplayFormat->addItem(tr("SpatiaLite Geometry to SVG"), "geomToSVG");
    ui->comboDisplayFormat->insertSeparator(ui->comboDisplayFormat->count());
    ui->comboDisplayFormat->addItem(tr("Custom"), "custom");

    ui->labelDisplayFormat->setText(ui->labelDisplayFormat->text().arg(column_name));

    const QString e_column_name = sqlb::escapeIdentifier(column_name);

    formatFunctions["decimal"] = "printf('%d', " + e_column_name + ")";
    formatFunctions["exponent"] = "printf('%e', " + e_column_name + ")";
    formatFunctions["hexblob"] = "hex(" + e_column_name + ")";
    formatFunctions["hex"] = "printf('0x%x', " + e_column_name + ")";
    formatFunctions["octal"] = "printf('%o', " + e_column_name + ")";
    formatFunctions["round"] = "round(" + e_column_name + ")";
    formatFunctions["appleDate"] = "datetime('2001-01-01', " + e_column_name + " || ' seconds')";
    formatFunctions["javaEpoch"] = "strftime('%Y-%m-%d %H:%M:%S.', " + e_column_name +
            "/1000, 'unixepoch') || (" + e_column_name + "%1000)";
    formatFunctions["dotNetTicks"] = "datetime(" + e_column_name + " / 10000000 - 62135596800, 'unixepoch')";
    formatFunctions["julian"] = "datetime(" + e_column_name + ")";
    formatFunctions["epoch"] = "datetime(" + e_column_name + ", 'unixepoch')";
    formatFunctions["epochLocalTime"] = "datetime(" + e_column_name + ", 'unixepoch', 'localtime')";
    formatFunctions["webkitEpoch"] = "datetime(" + e_column_name + " / 1e6 - 11644473600, 'unixepoch')";
    formatFunctions["webkitEpochLocalTime"] = "datetime(" + e_column_name + " / 1e6 - 11644473600, 'unixepoch', 'localtime')";
    formatFunctions["winDate"] = "datetime('1899-12-30', " + e_column_name + " || ' days')";
    formatFunctions["ddmmyyyyDate"] = "strftime('%d/%m/%Y', " + e_column_name + ")";
    formatFunctions["lower"] = "lower(" + e_column_name + ")";
    formatFunctions["upper"] = "upper(" + e_column_name + ")";
    formatFunctions["guid"] = "substr(hex(" + e_column_name + "), 7, 2) || " +
                              "substr(hex(" + e_column_name + "), 5, 2) || " +
                              "substr(hex(" + e_column_name + "), 3, 2) || " +
                              "substr(hex(" + e_column_name + "), 1, 2) || '-' || " +
                              "substr(hex(" + e_column_name + "), 11, 2) || " +
                              "substr(hex(" + e_column_name + "), 9, 2) || '-' || " +
                              "substr(hex(" + e_column_name + "), 15, 2) || " +
                              "substr(hex(" + e_column_name + "), 13, 2) || '-' || " +
                              "substr(hex(" + e_column_name + "), 17, 4) || '-' || " +
                              "substr(hex(" + e_column_name + "), 21, 12)";
    formatFunctions["geomToSVG"] = QString(
R"('<svg xmlns="http://www.w3.org/2000/svg">'
|| '<path d="'
|| AsSVG(
        ScaleCoords(
            ST_Translate(%1, -St_MinX(%1), -St_MaxY(%1), 0)
        , 639 / (MAX(St_MaxX(%1) - St_MinX(%1), St_MaxY(%1) - St_MinY(%1))))
   , 1, 5)
|| '" stroke="darkblue" fill="#b5cfed" stroke-width="1"/>'
|| '</svg>')").arg(e_column_name);

    // Set the current format, if it's empty set the default format
    if(current_format.isEmpty())
    {
        ui->comboDisplayFormat->setCurrentIndex(0);
        updateSqlCode();
    } else {
        // When it doesn't match any predefined format, it is considered custom
        QString formatName = "custom";
        auto it = std::find_if(formatFunctions.begin(), formatFunctions.end(), [current_format](const std::pair<std::string, QString>& s) {
            return s.second == current_format;
        });
        if(it != formatFunctions.end())
            formatName = QString::fromStdString(it->first);
        ui->comboDisplayFormat->setCurrentIndex(ui->comboDisplayFormat->findData(formatName));
        ui->editDisplayFormat->setText(current_format);
    }
}

ColumnDisplayFormatDialog::~ColumnDisplayFormatDialog()
{
    delete ui;
}

void ColumnDisplayFormatDialog::showEvent(QShowEvent *)
{
    adjustSize();
}

QString ColumnDisplayFormatDialog::selectedDisplayFormat() const
{
    if(ui->comboDisplayFormat->currentData().toString() == "default")
        return QString();
    else
        return ui->editDisplayFormat->text();
}

void ColumnDisplayFormatDialog::updateSqlCode()
{
    std::string format = ui->comboDisplayFormat->currentData().toString().toStdString();

    if(format == "default")
        ui->editDisplayFormat->setText(sqlb::escapeIdentifier(column_name));
    else if(format != "custom")
        ui->editDisplayFormat->setText(formatFunctions.at(format));
}

void ColumnDisplayFormatDialog::accept()
{
    QString errorMessage;

    // Accept the SQL code if it's the column name (default), it contains a function invocation applied to the column name and it can be
    // executed without errors returning only one column.
    // Users could still devise a way to break this, but this is considered good enough for letting them know about simple incorrect
    // cases.
    if(!(ui->editDisplayFormat->text() == sqlb::escapeIdentifier(column_name) ||
         ui->editDisplayFormat->text().contains(QRegularExpression("[a-z]+[a-z_0-9]* *\\(.*" + QRegularExpression::escape(sqlb::escapeIdentifier(column_name)) + ".*\\)", QRegularExpression::CaseInsensitiveOption))))
        errorMessage = tr("Custom display format must contain a function call applied to %1").arg(sqlb::escapeIdentifier(column_name));
    else {
        // Execute a query using the display format and check that it only returns one column.
        int customNumberColumns = 0;

        DBBrowserDB::execCallback callback = [&customNumberColumns](int numberColumns, std::vector<QByteArray>, std::vector<QByteArray>) -> bool {
            customNumberColumns = numberColumns;
            // Return false so the query is not aborted and no error is reported.
            return false;
        };
        if(!pdb.executeSQL("SELECT " + ui->editDisplayFormat->text().toStdString() + " FROM " + curTable.toString() + " LIMIT 1",
                           false, true, callback))
            errorMessage = tr("Error in custom display format. Message from database engine:\n\n%1").arg(pdb.lastError());
        else if(customNumberColumns != 1)
            errorMessage = tr("Custom display format must return only one column but it returned %1.").arg(customNumberColumns);

    }
    if(!errorMessage.isEmpty())
        QMessageBox::warning(this, QApplication::applicationName(), errorMessage);
    else
        QDialog::accept();
}

void ColumnDisplayFormatDialog::setCustom(bool modified)
{
    // If the SQL code is modified by user, select the custom value in the combo-box
    if(modified && ui->editDisplayFormat->hasFocus())
        ui->comboDisplayFormat->setCurrentIndex(ui->comboDisplayFormat->findData("custom"));
}
