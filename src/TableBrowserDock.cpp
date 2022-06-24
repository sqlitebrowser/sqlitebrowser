#include <QAction>
#include <QApplication>
#include <QInputDialog>
#include <QMenu>

#include "TableBrowser.h"
#include "TableBrowserDock.h"
#include "MainWindow.h"

TableBrowserDock::TableBrowserDock(QWidget* parent, MainWindow* mainWindow)
    : QDockWidget(parent),
      main_window(mainWindow),
      browser(new TableBrowser(&mainWindow->db, this))
{
    // Set context menu
    setContextMenuPolicy(Qt::CustomContextMenu);

    // Normally dock widgets are not deleted when they are closed. Make sure to delete these
    // because they can consume a lot of memory and we would need to figure out if they are
    // visible whenever we loop over them.
    setAttribute(Qt::WA_DeleteOnClose, true);

    // Set object name
    static unsigned int counter = 0;
    setObjectName("dockBrowse" + QString::number(++counter));

    // Connect dock signals
    connect(this, &TableBrowserDock::customContextMenuRequested, this, &TableBrowserDock::showContextMenuTableBrowserTabBar);

    // Connect browser signals
    connect(browser, &TableBrowser::currentTableChanged, this, [this](const sqlb::ObjectIdentifier& table) {
        // Only update dock name when no custom name was set
        if(!property("custom_title").toBool())
            setWindowTitle(QString::fromStdString(table.toDisplayString()));
    });

    connect(browser, &TableBrowser::newDockRequested, this, &TableBrowserDock::newDockRequested);

    // Update view
    browser->refresh();

    // Set browser widget
    setWidget(browser);
}

void TableBrowserDock::closeEvent(QCloseEvent*)
{
    emit closed();
}

void TableBrowserDock::setFocusStyle(bool on)
{
    // Highlight title bar when dock widget is active
    if(on)
        setStyleSheet(QStringLiteral(
            "QDockWidget::title {"
                "background:palette(AlternateBase);"
                "text-align: center;"
                "border-bottom: 2px solid palette(highlight);"
            "}"));
    else
        setStyleSheet(QStringLiteral(
            "QDockWidget::title {"
                "text-align: center;"
            "}"));
}

void TableBrowserDock::showContextMenuTableBrowserTabBar(const QPoint& pos)
{
    // Prepare all menu actions
    QAction* actionNewDock = new QAction(this);
    actionNewDock->setText(tr("New Data Browser"));
    connect(actionNewDock, &QAction::triggered, this, &TableBrowserDock::newDockRequested);

    QAction* actionRename = new QAction(this);
    actionRename->setText(tr("Rename Data Browser"));
    connect(actionRename, &QAction::triggered, this, [this]() {
        renameTableBrowserTab();
    });

    QAction* actionClose = new QAction(this);
    actionClose->setText(tr("Close Data Browser"));
    connect(actionClose, &QAction::triggered, this, [this]() {
        deleteLater();
    });

    // Show menu
    QMenu* menuTabs = new QMenu(this);
    menuTabs->addAction(actionNewDock);
    menuTabs->addAction(actionRename);
    menuTabs->addAction(actionClose);
    menuTabs->exec(mapToGlobal(pos));
}

void TableBrowserDock::renameTableBrowserTab()
{
    QString new_name = QInputDialog::getText(this,
                                             qApp->applicationName(),
                                             tr("Set a new name for the data browser. Use the '&&' character to allow using the following character as a keyboard shortcut."),
                                             QLineEdit::EchoMode::Normal,
                                             windowTitle());

    if(!new_name.isNull())      // Don't do anything if the Cancel button was clicked
    {
        setWindowTitle(new_name);
        setProperty("custom_title", true);
    }
}
