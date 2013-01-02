#include "DeleteTableForm.h"
#include <QMessageBox>

/*
 *  Constructs a deleteTableForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
deleteTableForm::deleteTableForm(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
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

     if (QMessageBox::warning( this, QApplication::applicationName(),
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
    comboOptions->addItems(entries);
}


