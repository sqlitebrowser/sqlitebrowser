#include "extendedmainform.h"
#include <QUrl>

ExtendedMainForm::ExtendedMainForm(QWidget* parent, const char* name, Qt::WindowFlags fl) : mainForm( parent, name, fl )
{
	setAcceptDrops(true);
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

