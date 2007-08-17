//Added by qt3to4:
#include <QCloseEvent>
/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
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
