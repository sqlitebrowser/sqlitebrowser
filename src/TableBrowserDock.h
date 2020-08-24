#ifndef TABLEBROWSERDOCK_H
#define TABLEBROWSERDOCK_H

#include <QDockWidget>

class MainWindow;
class TableBrowser;

class TableBrowserDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit TableBrowserDock(QWidget* parent, MainWindow* mainWindow);
    void setFocusStyle(bool on);

    TableBrowser* tableBrowser() { return browser; }

signals:
    void closed();
    void newDockRequested();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    MainWindow* main_window;
    TableBrowser* browser;

    void showContextMenuTableBrowserTabBar(const QPoint& pos);
    void renameTableBrowserTab();
};

#endif
