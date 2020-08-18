#include "TableBrowser.h"
#include "TableBrowserDock.h"
#include "MainWindow.h"

TableBrowserDock::TableBrowserDock(QWidget* parent, MainWindow* mainWindow)
    : QDockWidget(parent),
      main_window(mainWindow)
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    // Normally dock widgets are not deleted when they are closed. Make sure to delete these
    // because they can consume a lot of memory and we would need to figure out if they are
    // visible whenever we loop over them.
    setAttribute(Qt::WA_DeleteOnClose, true);
}

void TableBrowserDock::closeEvent(QCloseEvent*)
{
    emit closed();
}

void TableBrowserDock::setFocusStyle(bool on)
{
    // Highlight title bar when dock widget is active
    if(on)
        setStyleSheet("QDockWidget::title {background:palette(highlight);}");
    else
        setStyleSheet(QString());
}

TableBrowser* TableBrowserDock::tableBrowser()
{
    return qobject_cast<TableBrowser*>(widget());
}
