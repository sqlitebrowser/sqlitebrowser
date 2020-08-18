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

    TableBrowser* tableBrowser();

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    MainWindow* main_window;
};

#endif
