//---------------------------------------------------------------
// ExtendedMainForm: This class extends the mainForm class
//                       to support drag and drop of sql file.
//
// Developer:        Arvind Kumar Gupta
//                   arvind.your@gmail.com
//---------------------------------------------------------------

#include "extendedmainform.h"
#include <QUrl>

ExtendedMainForm::ExtendedMainForm(QWidget* parent) : mainForm( parent )
{
	setAcceptDrops(true); // Enable the drop feature on this form
}

ExtendedMainForm::~ExtendedMainForm(void)
{
}

void ExtendedMainForm::dragEnterEvent(QDragEnterEvent *event)
{
	if( event->mimeData()->hasFormat("text/uri-list") )
			event->acceptProposedAction();
}


void ExtendedMainForm::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();

	if( urls.isEmpty() )
		return;

	QString fileName = urls.first().toLocalFile();

	if( !fileName.isEmpty() && fileName.endsWith("db") )
			fileOpen(fileName);
}

