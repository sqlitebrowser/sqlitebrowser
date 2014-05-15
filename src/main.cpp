#include "Application.h"

int main( int argc, char ** argv )
{
    // Create application object. All the initialisation stuff happens in there
    Application a(argc, argv);

    // Quit application now if user doesn't want to see the UI
    if(a.dontShowMainWindow())
        return 0;

    // Run application
    return a.exec();
}
