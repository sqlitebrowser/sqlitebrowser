#ifndef __EXTENDEDMAINFORM__
#define	__EXTENDEDMAINFORM__

#include "form1.h"
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

