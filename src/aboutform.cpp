#include "aboutform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "sqlitedb.h"
/*
 *  Constructs a aboutForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
aboutForm::aboutForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
aboutForm::~aboutForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void aboutForm::languageChange()
{
    retranslateUi(this);
}

void aboutForm::init()
{
    this->setCaption(applicationName);
    //aboutBrowser->setText(aboutText);
}

