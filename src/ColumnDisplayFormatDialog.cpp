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
    ui->comboDisplayFormat->addItem(tr("Julian day to date"), "julian");
    ui->comboDisplayFormat->addItem(tr("Unix epoch to date"), "epoch");
    ui->comboDisplayFormat->addItem(tr("Unix epoch to local time"), "epochLocalTime");
    ui->comboDisplayFormat->addItem(tr("Windows DATE to date"), "winDate");
    ui->comboDisplayFormat->addItem(tr("Date as dd/mm/yyyy"), "ddmmyyyyDate");
    ui->comboDisplayFormat->insertSeparator(ui->comboDisplayFormat->count());
    ui->comboDisplayFormat->addItem(tr("Lower case"), "lower");
    ui->comboDisplayFormat->addItem(tr("Upper case"), "upper");
    ui->comboDisplayFormat->insertSeparator(ui->comboDisplayFormat->count());
    ui->comboDisplayFormat->addItem(tr("Custom"), "custom");

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
