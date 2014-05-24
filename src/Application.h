#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <QApplication>

class MainWindow;
class QTranslator;

class Application : public QApplication
{
    Q_OBJECT

public:
    explicit Application(int& argc, char** argv);

    bool dontShowMainWindow() { return m_dontShowMainWindow; }

protected:
    bool event(QEvent* event);

private:
    bool m_dontShowMainWindow;
    MainWindow* m_mainWindow;
    QTranslator* m_translatorQt;
    QTranslator* m_translatorApp;
};

#endif
