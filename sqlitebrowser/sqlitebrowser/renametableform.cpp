#include "renametableform.h"

#include <qvariant.h>
#include <qmessagebox.h>
#include <qimage.h>
#include <qpixmap.h>

#include "sqlitedb.h"
/*
 *  Constructs a renameTableForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
renameTableForm::renameTableForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
renameTableForm::~renameTableForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void renameTableForm::languageChange()
{
    retranslateUi(this);
}

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
