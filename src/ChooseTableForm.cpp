#include "ChooseTableForm.h"

/*
 *  Constructs a chooseTableForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
chooseTableForm::chooseTableForm(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
chooseTableForm::~chooseTableForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void chooseTableForm::languageChange()
{
    retranslateUi(this);
}

void chooseTableForm::editPressed()
{
        option = comboOptions->currentText();
        accept();
}

void chooseTableForm::populateOptions(QStringList entries)
{
    comboOptions->clear();
    comboOptions->addItems(entries);
}

