#include "sqllogform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "sqlitedb.h"
/*
 *  Constructs a sqlLogForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
sqlLogForm::sqlLogForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
sqlLogForm::~sqlLogForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void sqlLogForm::languageChange()
{
    retranslateUi(this);
}

void sqlLogForm::closeEvent( QCloseEvent * )
{
        emit goingAway();
}

void sqlLogForm::log( QString & statement, int msgtype)
{
    if (msgtype==kLogMsg_User)
    {
        userLogText->setText(userLogText->text()+statement+"\n");
    } else {
        appLogText->setText(appLogText->text()+statement+"\n");
    }
}

void sqlLogForm::msgDBDirtyState( bool dirty)
{
    emit dbState(dirty);
}


void sqlLogForm::clearLog()
{
    if (logStack->id(logStack->visibleWidget())==kLogMsg_User)
    {
        userLogText->clear();
    } else {
    appLogText->clear();
}
}

