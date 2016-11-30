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
    ui->comboDisplayFormat->addItem(tr("Decimal number"), "decimal");
    ui->comboDisplayFormat->addItem(tr("Exponent notation"), "exponent");
    ui->comboDisplayFormat->addItem(tr("Hex blob"), "hexblob");
    ui->comboDisplayFormat->addItem(tr("Hex number"), "hex");
    ui->comboDisplayFormat->addItem(tr("Apple NSDate to date"), "appleDate");
    ui->comboDisplayFormat->addItem(tr("Julian day to date"), "julian");
    ui->comboDisplayFormat->addItem(tr("Lower case"), "lower");
    ui->comboDisplayFormat->addItem(tr("Octal number"), "octal");
    ui->comboDisplayFormat->addItem(tr("Round number"), "round");
    ui->comboDisplayFormat->addItem(tr("Unix epoch to date"), "epoch");
    ui->comboDisplayFormat->addItem(tr("Upper case"), "upper");
    ui->comboDisplayFormat->addItem(tr("Windows DATE to date"), "winDate");
    ui->labelDisplayFormat->setText(ui->labelDisplayFormat->text().arg(column_name));

    // Set the current format, if it's empty set the default format
    if(current_format.isEmpty())
    {
        ui->comboDisplayFormat->setCurrentIndex(0);
        updateSqlCode();
    } else {
        ui->comboDisplayFormat->addItem(tr("Custom"), "custom");
        ui->comboDisplayFormat->setCurrentIndex(ui->comboDisplayFormat->findData("custom"));
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
    else if(format == "lower")
        ui->editDisplayFormat->setText("lower(" + sqlb::escapeIdentifier(column_name) + ")");
    else if(format == "upper")
        ui->editDisplayFormat->setText("upper(" + sqlb::escapeIdentifier(column_name) + ")");
    else if(format == "epoch")
        ui->editDisplayFormat->setText("datetime(" + sqlb::escapeIdentifier(column_name) + ", 'unixepoch')");
    else if(format == "winDate")
        ui->editDisplayFormat->setText("datetime ('1899-12-30', " + sqlb::escapeIdentifier(column_name) + " || \" days\" )");
    else if(format == "appleDate")
        ui->editDisplayFormat->setText("datetime ('2001-01-01', " + sqlb::escapeIdentifier(column_name) + " || \" seconds\" )");
    else if(format == "julian")
        ui->editDisplayFormat->setText("datetime(" + sqlb::escapeIdentifier(column_name) + ")");
    else if(format == "round")
        ui->editDisplayFormat->setText("round(" + sqlb::escapeIdentifier(column_name) + ")");
    else if(format == "hex")
        ui->editDisplayFormat->setText("printf('0x%x', " + sqlb::escapeIdentifier(column_name) + ")");
    else if(format == "octal")
        ui->editDisplayFormat->setText("printf('%o', " + sqlb::escapeIdentifier(column_name) + ")");
    else if(format == "exponent")
        ui->editDisplayFormat->setText("printf('%e', " + sqlb::escapeIdentifier(column_name) + ")");
    else if(format == "hexblob")
        ui->editDisplayFormat->setText("hex(" + sqlb::escapeIdentifier(column_name) + ")");
    else if(format == "decimal")
        ui->editDisplayFormat->setText("printf('%d', " + sqlb::escapeIdentifier(column_name) + ")");
}
