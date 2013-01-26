#include "EditFieldDialog.h"
#include "ui_EditFieldDialog.h"
#include "sqlitedb.h"
#include <QPushButton>
#include <QMessageBox>

EditFieldDialog::EditFieldDialog(DBBrowserDB* db, bool new_field, const QString& table, const QString& fld_name, const QString& fld_type, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditFieldDialog),
      pdb(db),
      original_field_name(fld_name),
      table_name(table),
      is_new(new_field)
{
    // Create window and set its properties
    ui->setupUi(this);

    setWindowIcon(QIcon(is_new ? ":/icons/field_add" : ":/icons/field_edit"));
    if(table == "")
        setWindowTitle(tr("Add new field to new table"));
    else
        setWindowTitle(is_new ? tr("New Field in '%1'").arg(table_name) : tr("Change Field in '%1'").arg(table_name));

    // Associate the radio buttons with their relative SQL data type
    ui->radioTEXT->setProperty("field_type", "TEXT");
    ui->radioNUMERIC->setProperty("field_type", "NUMERIC");
    ui->radioBLOB->setProperty("field_type", "BLOB");
    ui->radioINTPRIMARY->setProperty("field_type", "INTEGER PRIMARY KEY");
    ui->radioCustom->setProperty("field_type", "__custom__");

    // Set the current settings
    ui->nameLineEdit->setText(fld_name);
    QList<QAbstractButton *> buttons =  ui->groupRadioTypes->buttons();
    bool custom = true;
    for(int i = 0; i < buttons.size(); ++i){
        if( buttons.at(i)->property("field_type").toString() == fld_type.toUpper()){
             buttons.at(i)->setChecked(true);
             custom = false;
             break;
        }
    }
    if(custom)
    {
        ui->radioCustom->setChecked(true);
        ui->txtCustomType->setText(fld_type);
    }

    // Check the current input values
    checkInput();
}

EditFieldDialog::~EditFieldDialog()
{
    delete ui;
}

void EditFieldDialog::accept()
{
    field_name = ui->nameLineEdit->text();
    field_type = ui->groupRadioTypes->checkedButton()->property("field_type").toString();
    if(field_type == "__custom__")
        field_type = ui->txtCustomType->text();

    // Only change DB when dialog was not opened for a newly created table, i.e. table name is ""
    if(table_name != "")
    {
        bool ok;
        if(is_new)
            ok = pdb->createColumn(table_name, field_name, field_type);
        else
            ok = pdb->renameColumn(table_name, original_field_name, field_name, field_type);
        if(!ok){
            QMessageBox::warning(this, QApplication::applicationName(), pdb->lastErrorMessage);
            qDebug(pdb->lastErrorMessage.toUtf8());
            return;
        }
    }
    QDialog::accept();
}

void EditFieldDialog::checkInput()
{
    ui->nameLineEdit->setText(ui->nameLineEdit->text().trimmed());
    bool valid = true;
    if(ui->nameLineEdit->text().isEmpty() || ui->nameLineEdit->text().contains(" "))
        valid = false;
    if(ui->radioCustom->isChecked() && ui->txtCustomType->text().isEmpty())
            valid = false;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}
