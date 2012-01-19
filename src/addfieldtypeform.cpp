#include "addfieldtypeform.h"

#include <qvariant.h>
#include <qmessagebox.h>
#include <qimage.h>
#include <qpixmap.h>

#include "sqlitedb.h"
/*
 *  Constructs a addFieldTypeForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
addFieldTypeForm::addFieldTypeForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
addFieldTypeForm::~addFieldTypeForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void addFieldTypeForm::languageChange()
{
    retranslateUi(this);
}

