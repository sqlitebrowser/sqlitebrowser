/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
void editFieldForm::setInitialValues(QString name, QString type)
{
    nameLineEdit->setText(name);
    typeBox->clear();
    typeBox->insertItem(type);
    QString tString = "";
    if (type.compare(tString)!=0) typeBox->insertItem(tString);
    tString = "text";
    if (type.compare(tString)!=0) typeBox->insertItem(tString);
    tString = "numeric";
    if (type.compare(tString)!=0) typeBox->insertItem(tString);
    tString = "blob";
    if (type.compare(tString)!=0) typeBox->insertItem(tString);
    tString = "integer primary key";
    if (type.compare(tString)!=0) typeBox->insertItem(tString);
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
    name = fieldname;
    type = typeBox->currentText();
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
	QString nospaces = addForm->typeNameEdit->text().remove(" ");
	setInitialValues(nameLineEdit->text(),nospaces );
	enableSave();
   }
}
