#include <qapplication.h>
#include "form1.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    mainForm w;
    w.show();
    if (argc>1) {
        //first and only argument we accept is the name of the database to open
	w.fileOpen(QString(argv[1]));
    }
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
