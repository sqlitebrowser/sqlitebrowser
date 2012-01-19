#include "deleteindexform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qmessagebox.h"
/*
 *  Constructs a deleteIndexForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
deleteIndexForm::deleteIndexForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
deleteIndexForm::~deleteIndexForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void deleteIndexForm::languageChange()
{
    retranslateUi(this);
}

void deleteIndexForm::confirmDelete()
{
    QString msg = "Are you sure you want to delete index ";
    msg.append(comboOptions->currentText());
    msg.append("?");

     if (QMessageBox::warning( this, applicationName,
       msg,
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No | QMessageBox::Escape )
     == QMessageBox::Yes ){
        option = comboOptions->currentText();
        accept();
    }
}

void deleteIndexForm::populateOptions(QStringList entries)
{
    comboOptions->clear();
    for ( QStringList::Iterator ct = entries.begin(); ct != entries.end(); ++ct ) {
        comboOptions->insertItem(*ct,-1);
    }
}


