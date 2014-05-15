#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <QApplication>

class MainWindow;

class Application : public QApplication
{
    Q_OBJECT

public:
    explicit Application(int& argc, char** argv);

    bool dontShowMainWindow() { return m_dontShowMainWindow; }

private:
    bool m_dontShowMainWindow;
    MainWindow* m_mainWindow;
};

#endif
