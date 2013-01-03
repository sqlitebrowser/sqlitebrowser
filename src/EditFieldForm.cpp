#include "EditFieldForm.h"
#include "ui_EditFieldForm.h"
#include "sqlitedb.h"
#include <QPushButton>

editFieldForm::editFieldForm(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::editFieldForm)
{
    ui->setupUi(this);

    ui->radioTEXT->setProperty("field_type", "TEXT");
    ui->radioNUMERIC->setProperty("field_type", "NUMERIC");
    ui->radioBLOB->setProperty("field_type", "BLOB");
    ui->radioINTPRIMARY->setProperty("field_type", "INTEGER PRIMARY KEY");
    ui->radioCustom->setProperty("field_type", "__custom__");
}

editFieldForm::~editFieldForm()
{
    delete ui;
}

void editFieldForm::setInitialValues(DBBrowserDB *db, bool is_new, QString table, QString fld_name, QString fld_type)
{
    pdb = db;
    original_field_name = QString(fld_name);
    table_name = table;
    ui->nameLineEdit->setText(fld_name);

    this->is_new = is_new;
    setWindowIcon(QIcon(is_new ? ":/icons/field_add" : ":/icons/field_edit"));
    if(table == "")
        setWindowTitle(tr("Add new field to new table"));
    else
        setWindowTitle(is_new ? tr("New Field in '%1'").arg(table_name) : tr("Change Field in '%1'").arg(table_name));

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

    checkInput();
}

void editFieldForm::accept()
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
            qDebug(pdb->lastErrorMessage.toUtf8());
            return;
        }
    }
    QDialog::accept();
}

void editFieldForm::checkInput()
{
    ui->nameLineEdit->setText(ui->nameLineEdit->text().trimmed());
    bool valid = true;
    if(ui->nameLineEdit->text().isEmpty() || ui->nameLineEdit->text().contains(" "))
        valid = false;
    if(ui->radioCustom->isChecked() && ui->txtCustomType->text().isEmpty())
            valid = false;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}
