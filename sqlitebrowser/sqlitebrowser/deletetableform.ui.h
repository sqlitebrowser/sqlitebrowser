/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/


void deleteTableForm::confirmDelete()
{
    QString msg = "Are you sure you want to delete table ";
    msg.append(comboOptions->currentText());
    msg.append("? \n All data in the table will be lost");
    
     if (QMessageBox::warning( this, applicationName,
       msg,
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No | QMessageBox::Escape )
     == QMessageBox::Yes ){
	option = comboOptions->currentText();
	accept();
    }
}

void deleteTableForm::populateOptions(QStringList entries)
{
    comboOptions->clear();
    for ( QStringList::Iterator ct = entries.begin(); ct != entries.end(); ++ct ) {
	comboOptions->insertItem(*ct,-1);
    }
}
