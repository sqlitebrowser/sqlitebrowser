/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
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
 QString nospaces = addForm->typeNameEdit->text().remove(" ");
 setInitialValues(nameLineEdit->text(),nospaces );
   }
}
