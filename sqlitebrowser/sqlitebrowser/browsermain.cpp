/*
** This file is part of SQLite Database Browser
** http://sqlitebrowser.sourceforge.net
**
** Originally developed by Mauricio Piacentini, Tabuleiro
**
** The author disclaims copyright to this source code.  
** Consult the LICENSING file for known restrictions
**
*/

#include <qapplication.h>
#include "form1.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    mainForm w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
