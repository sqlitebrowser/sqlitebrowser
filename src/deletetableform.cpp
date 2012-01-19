#include "deletetableform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qmessagebox.h"
/*
 *  Constructs a deleteTableForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
deleteTableForm::deleteTableForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
deleteTableForm::~deleteTableForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void deleteTableForm::languageChange()
{
    retranslateUi(this);
}

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


