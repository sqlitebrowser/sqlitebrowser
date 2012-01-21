//---------------------------------------------------------------
// ExtendedMainForm: This class extends the mainForm class
//                       to support drag and drop of sql file.
//
// Developer:        Arvind Kumar Gupta
//                   arvind.your@gmail.com
//---------------------------------------------------------------

#ifndef __EXTENDEDMAINFORM__
#define	__EXTENDEDMAINFORM__

#include "mainwindow.h"
#include <QDragEnterEvent>

class ExtendedMainForm : public mainForm
{
public:
	ExtendedMainForm(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
	~ExtendedMainForm(void);

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
};

#endif // __EXTENDEDMAINFORM__

