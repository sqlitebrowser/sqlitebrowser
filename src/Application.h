#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class MainWindow;
class QTranslator;

class Application : public QApplication
{
    Q_OBJECT

public:
    explicit Application(int& argc, char** argv);
    virtual ~Application();

    bool dontShowMainWindow() { return m_dontShowMainWindow; }

    MainWindow* mainWindow() { return m_mainWindow; }

protected:
    bool event(QEvent* event);

private:
    bool m_dontShowMainWindow;
    MainWindow* m_mainWindow;
    QTranslator* m_translatorQt;
    QTranslator* m_translatorApp;
};

#endif
