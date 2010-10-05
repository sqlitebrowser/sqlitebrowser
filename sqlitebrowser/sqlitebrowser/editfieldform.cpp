#include "editfieldform.h"

#include <qvariant.h>
#include <qmessagebox.h>
#include <qimage.h>
#include <qpixmap.h>

#include "sqlitedb.h"
#include "addfieldtypeform.h"
/*
 *  Constructs a editFieldForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
editFieldForm::editFieldForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
editFieldForm::~editFieldForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void editFieldForm::languageChange()
{
    retranslateUi(this);
}

void editFieldForm::setDB(DBBrowserDB &db)
{
    this->pdb = db;
}

void editFieldForm::setInitialValues(bool is_new, QString table, QString fld_name, QString fld_type)
{

    original_field_name = QString(fld_name);

    table_name = table;
    nameLineEdit->setText(fld_name);
    QList<QAbstractButton *> buttons =  groupRadioTypes->buttons();
    for(int i = 0; i < buttons.size(); ++i){
        if( buttons.at(i)->property("field_type").toString() == fld_type){
             buttons.at(i)->setChecked(true);
             return;
        }
    }
    //TODO enable custom
}

void editFieldForm::confirmEdit()
{
    QString fieldname = nameLineEdit->text();
    if (fieldname.isEmpty()) {
 QMessageBox::information( this, applicationName, "Field name can not be empty" );
        return;
    }
    if (fieldname.contains(" ")>0) {
        QMessageBox::warning( this, applicationName, "Spaces are not allowed in the field name" );
        return;
    }
    field_name = fieldname;
    //field_type = typeBox->currentText();
    field_type = groupRadioTypes->checkedButton()->property("field_type").toString();
    bool ok = pdb.createColumn(table_name, field_name, field_type);
    if(!ok){
        qDebug(pdb.lastErrorMessage);
        return;
    }
    //qDebug("");
   // if(!pdb.executeSQL(sql)){
//        qDebug(pdb.lastErrorMessage);
    //    return;
   // }
    accept();
}

void editFieldForm::enableSave()
{
    saveButton->setEnabled(true);
}

void editFieldForm::getCustomType()
{
    addFieldTypeForm * addForm = new addFieldTypeForm( this, "addfieldtype", TRUE );
    if (addForm->exec())
   {
 //QString nospaces = addForm->typeNameEdit->text().remove(" ");
 QString nospaces = addForm->typeNameEdit->text();
        //setInitialValues( nameLineEdit->text(), nospaces );
        enableSave();
   }
}


void editFieldForm::on_radio_button_clicked(QAbstractButton *button){
    qDebug("YES");
    //qDebug(button->property("field_type").toString());
}
