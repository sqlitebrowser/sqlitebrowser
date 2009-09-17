#include "editformwindow.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a editFormWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
editFormWindow::editFormWindow(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
}

/*
 *  Destroys the object and frees any allocated resources
 */
editFormWindow::~editFormWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void editFormWindow::languageChange()
{
    retranslateUi(this);
}

void editFormWindow::fileNew()
{
    qWarning("editFormWindow::fileNew(): Not implemented yet");
}

void editFormWindow::fileOpen()
{
    qWarning("editFormWindow::fileOpen(): Not implemented yet");
}

void editFormWindow::fileSave()
{
    qWarning("editFormWindow::fileSave(): Not implemented yet");
}

void editFormWindow::fileSaveAs()
{
    qWarning("editFormWindow::fileSaveAs(): Not implemented yet");
}

void editFormWindow::filePrint()
{
    qWarning("editFormWindow::filePrint(): Not implemented yet");
}

void editFormWindow::fileExit()
{
    qWarning("editFormWindow::fileExit(): Not implemented yet");
}

void editFormWindow::editUndo()
{
    qWarning("editFormWindow::editUndo(): Not implemented yet");
}

void editFormWindow::editRedo()
{
    qWarning("editFormWindow::editRedo(): Not implemented yet");
}

void editFormWindow::editCut()
{
    qWarning("editFormWindow::editCut(): Not implemented yet");
}

void editFormWindow::editCopy()
{
    qWarning("editFormWindow::editCopy(): Not implemented yet");
}

void editFormWindow::editPaste()
{
    qWarning("editFormWindow::editPaste(): Not implemented yet");
}

void editFormWindow::editFind()
{
    qWarning("editFormWindow::editFind(): Not implemented yet");
}

void editFormWindow::helpIndex()
{
    qWarning("editFormWindow::helpIndex(): Not implemented yet");
}

void editFormWindow::helpContents()
{
    qWarning("editFormWindow::helpContents(): Not implemented yet");
}

void editFormWindow::helpAbout()
{
    qWarning("editFormWindow::helpAbout(): Not implemented yet");
}

