/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/


void renameTableForm::renameClicked()
{
    QString tabname = tablenameLineEdit->text();
    if (tabname.isEmpty()) {
	QMessageBox::information( this, applicationName, "Table name can not be empty" );
	return;
    }
    if (tabname.contains(" ")>0) {
	QMessageBox::warning( this, applicationName, "Spaces are not allowed in the table name" );
	return;
    }
    accept();
}

QString renameTableForm::getTableName()
{
    return tablenameLineEdit->text();
}

void renameTableForm::setTableName(QString name)
{
    tablenameLineEdit->setText(name);
}
