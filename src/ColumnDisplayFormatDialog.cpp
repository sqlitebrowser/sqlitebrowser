#include "ColumnDisplayFormatDialog.h"
#include "ui_ColumnDisplayFormatDialog.h"
#include "sqlitetypes.h"

ColumnDisplayFormatDialog::ColumnDisplayFormatDialog(const QString& colname, QString current_format, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::ColumnDisplayFormatDialog),
      column_name(colname)
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
    ui->comboDisplayFormat->addItem(tr("Julian day to date"), "julian");
    ui->comboDisplayFormat->addItem(tr("Unix epoch to date"), "epoch");
    ui->comboDisplayFormat->addItem(tr("Unix epoch to local time"), "epochLocalTime");
    ui->comboDisplayFormat->addItem(tr("Windows DATE to date"), "winDate");
    ui->comboDisplayFormat->addItem(tr("Date as dd/mm/yyyy"), "ddmmyyyyDate");
    ui->comboDisplayFormat->insertSeparator(ui->comboDisplayFormat->count());
    ui->comboDisplayFormat->addItem(tr("Lower case"), "lower");
    ui->comboDisplayFormat->addItem(tr("Upper case"), "upper");
    ui->labelDisplayFormat->setText(ui->labelDisplayFormat->text().arg(column_name));

    formatFunctions["decimal"] = "printf('%d', " + sqlb::escapeIdentifier(column_name) + ")";
    formatFunctions["exponent"] = "printf('%e', " + sqlb::escapeIdentifier(column_name) + ")";
    formatFunctions["hexblob"] = "hex(" + sqlb::escapeIdentifier(column_name) + ")";
    formatFunctions["hex"] = "printf('0x%x', " + sqlb::escapeIdentifier(column_name) + ")";
    formatFunctions["octal"] = "printf('%o', " + sqlb::escapeIdentifier(column_name) + ")";
    formatFunctions["round"] = "round(" + sqlb::escapeIdentifier(column_name) + ")";
    formatFunctions["appleDate"] = "datetime('2001-01-01', " + sqlb::escapeIdentifier(column_name) + " || ' seconds')";
    formatFunctions["javaEpoch"] = "strftime('%Y-%m-%d %H:%M:%S.', " + sqlb::escapeIdentifier(column_name) +
            "/1000, 'unixepoch') || (" + sqlb::escapeIdentifier(column_name) + "%1000)";
    formatFunctions["julian"] = "datetime(" + sqlb::escapeIdentifier(column_name) + ")";
    formatFunctions["epoch"] = "datetime(" + sqlb::escapeIdentifier(column_name) + ", 'unixepoch')";
    formatFunctions["epochLocalTime"] = "datetime(" + sqlb::escapeIdentifier(column_name) + ", 'unixepoch', 'localtime')";
    formatFunctions["winDate"] = "datetime('1899-12-30', " + sqlb::escapeIdentifier(column_name) + " || ' days')";
    formatFunctions["ddmmyyyyDate"] = "strftime('%d/%m/%Y', " + sqlb::escapeIdentifier(column_name) + ")";
    formatFunctions["lower"] = "lower(" + sqlb::escapeIdentifier(column_name) + ")";
    formatFunctions["upper"] = "upper(" + sqlb::escapeIdentifier(column_name) + ")";

    // Set the current format, if it's empty set the default format
    if(current_format.isEmpty())
    {
        ui->comboDisplayFormat->setCurrentIndex(0);
        updateSqlCode();
    } else {
        QString formatName;
        for(auto& formatKey : formatFunctions.keys()) {
            if(current_format == formatFunctions.value(formatKey)) {
                formatName = formatKey;
                break;
            }
        }

        if(formatName.isEmpty()) {
            ui->comboDisplayFormat->insertSeparator(ui->comboDisplayFormat->count());
            ui->comboDisplayFormat->addItem(tr("Custom"), "custom");
            formatName = "custom";
        }
        ui->comboDisplayFormat->setCurrentIndex(ui->comboDisplayFormat->findData(formatName));
        ui->editDisplayFormat->setText(current_format);
    }
}

ColumnDisplayFormatDialog::~ColumnDisplayFormatDialog()
{
    delete ui;
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
    QString format = ui->comboDisplayFormat->currentData().toString();

    if(format == "default")
        ui->editDisplayFormat->setText(sqlb::escapeIdentifier(column_name));
    else
        ui->editDisplayFormat->setText(formatFunctions.value(format));

}
