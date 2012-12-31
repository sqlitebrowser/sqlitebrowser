#include "editfieldform.h"
#include "ui_editfieldform.h"
#include "sqlitedb.h"
#include <QPushButton>

/*
 *  Constructs a editFieldForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
editFieldForm::editFieldForm(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl),
      ui(new Ui::editFieldForm)
{
    ui->setupUi(this);

    ui->radioTEXT->setProperty("field_type", "TEXT");
    ui->radioNUMERIC->setProperty("field_type", "NUMERIC");
    ui->radioBLOB->setProperty("field_type", "BLOB");
    ui->radioINTPRIMARY->setProperty("field_type", "INTEGER PRIMARY KEY");
    ui->radioCustom->setProperty("field_type", "__custom__");
}

/*
 *  Destroys the object and frees any allocated resources
 */
editFieldForm::~editFieldForm()
{
    delete ui;
}

void editFieldForm::setDB(DBBrowserDB &db)
{
    this->pdb = db;
}

void editFieldForm::setInitialValues(bool is_new, QString table, QString fld_name, QString fld_type)
{
    original_field_name = QString(fld_name);
    table_name = table;
    ui->nameLineEdit->setText(fld_name);

    this->is_new = is_new;
    setWindowIcon(QIcon(is_new ? ":/icons/field_add" : ":/icons/field_edit"));
    setWindowTitle(is_new
                   ? QString("New Field in '%1'").arg(table_name)
                   : QString("Change Field in '%1'").arg(table_name)
                );

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
    bool ok;
    if(is_new)
        ok = pdb.createColumn(table_name, field_name, field_type);
    else
        ok = pdb.renameColumn(table_name, original_field_name, field_name, field_type);
    if(!ok){
        qDebug(pdb.lastErrorMessage.toUtf8());
        return;
    }
    accept();
}

void editFieldForm::checkInput()
{
    ui->nameLineEdit->setText(ui->nameLineEdit->text().trimmed());
    ui->txtCustomType->setText(ui->txtCustomType->text().trimmed());
    bool valid = true;
    if(ui->nameLineEdit->text().isEmpty() || ui->nameLineEdit->text().contains(" "))
        valid = false;
    if(ui->radioCustom->isChecked() && (ui->txtCustomType->text().isEmpty() || ui->txtCustomType->text().contains(" ")))
            valid = false;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}
