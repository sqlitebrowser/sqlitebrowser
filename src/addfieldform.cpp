#include "addfieldform.h"

#include <qvariant.h>
#include <qmessagebox.h>
#include <qimage.h>
#include <qpixmap.h>

#include "sqlitedb.h"
#include "addfieldtypeform.h"
/*
 *  Constructs a addFieldForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
addFieldForm::addFieldForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
addFieldForm::~addFieldForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void addFieldForm::languageChange()
{
    retranslateUi(this);
}

void addFieldForm::setInitialValues(QString name, QString type)
{
    nameLineEdit->setText(name);
    typeBox->clear();
    typeBox->insertItem(type);
    QString tString = "";
    if (type.compare(tString)!=0) typeBox->insertItem(tString);
    tString = "TEXT";
    if (type.compare(tString)!=0) typeBox->insertItem(tString);
    tString = "NUMERIC";
    if (type.compare(tString)!=0) typeBox->insertItem(tString);
    tString = "BLOB";
    if (type.compare(tString)!=0) typeBox->insertItem(tString);
    tString = "INTEGER PRIMARY KEY";
    if (type.compare(tString)!=0) typeBox->insertItem(tString);
}

void addFieldForm::confirmAddField()
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
    fname = fieldname;
    ftype = typeBox->currentText();
    accept();
}

void addFieldForm::getCustomType()
{
    addFieldTypeForm * addForm = new addFieldTypeForm( this, "addfieldtype", TRUE );
    if (addForm->exec())
   {
 //QString nospaces = addForm->typeNameEdit->text().remove(" ");
 QString nospaces = addForm->typeNameEdit->text();
 setInitialValues(nameLineEdit->text(),nospaces );
   }
}
