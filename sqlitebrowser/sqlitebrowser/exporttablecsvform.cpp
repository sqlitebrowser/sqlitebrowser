#include "exporttablecsvform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a exportTableCSVForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
exportTableCSVForm::exportTableCSVForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
exportTableCSVForm::~exportTableCSVForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void exportTableCSVForm::languageChange()
{
    retranslateUi(this);
}

void exportTableCSVForm::exportPressed()
{
     option = comboOptions->currentText();
     accept();
}

void exportTableCSVForm::populateOptions(QStringList entries)
{
    comboOptions->clear();
    for ( QStringList::Iterator ct = entries.begin(); ct != entries.end(); ++ct ) {
        comboOptions->insertItem(*ct,-1);
    }
}
