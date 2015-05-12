#include <QFile>
#include <QFileOpenEvent>
#include <QTranslator>
#include <QTextCodec>
#include <QLibraryInfo>
#include <QLocale>
#include <QDebug>

#include "Application.h"
#include "MainWindow.h"
#include "PreferencesDialog.h"

Application::Application(int& argc, char** argv) :
    QApplication(argc, argv)
{
    // Set organisation and application names
    setOrganizationName("sqlitebrowser");
    setApplicationName("DB Browser for SQLite");

    // Set character encoding to UTF8
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    // Load translations
    bool ok;
    QString name = PreferencesDialog::getSettingsValue("General", "language").toString();

    // First of all try to load the application translation file.
    m_translatorApp = new QTranslator(this);
    ok = m_translatorApp->load("sqlb_" + name,
                               QCoreApplication::applicationDirPath() + "/translations");
    // If failed then try to load .qm file from resources
    if (ok == false) {
        ok = m_translatorApp->load("sqlb_" + name, ":/translations");
    }

    if (ok == true) {
        PreferencesDialog::setSettingsValue("General", "language", name);
        installTranslator(m_translatorApp);

        // The application translation file has been found and loaded.
        // And now try to load a Qt translation file.
        // Search path:
        // 1) standard Qt translations directory;
        // 2) the application translations directory.
        m_translatorQt = new QTranslator(this);

        ok = m_translatorQt->load("qt_" + name,
                                  QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        if (ok == false)
            ok = m_translatorQt->load("qt_" + name, "translations");
        if (ok == true)
            installTranslator(m_translatorQt);
    }
    else {
        // Set the correct locale so that the program won't erroneously detect
        // a language change when the user toggles settings for the first time.
        // (it also prevents the program from always looking for a translation on launch)
        PreferencesDialog::setSettingsValue("General", "language", "en_US");
    }

    // Parse command line
    QString fileToOpen;
    QStringList sqlToExecute;
    m_dontShowMainWindow = false;
    for(int i=1;i<arguments().size();i++)
    {
        // Check next command line argument
        if(arguments().at(i) == "-h" || arguments().at(i) == "--help")
        {
            // Help
            qWarning() << qPrintable(tr("Usage: %1 [options] [db]\n").arg(argv[0]));
            qWarning() << qPrintable(tr("Possible command line arguments:"));
            qWarning() << qPrintable(tr("  -h, --help\t\tShow command line options"));
            qWarning() << qPrintable(tr("  -s, --sql  [file]\tExecute this SQL file after opening the DB"));
            qWarning() << qPrintable(tr("  -q, --quit\t\tExit application after running scripts"));
            qWarning() << qPrintable(tr("  [file]\t\tOpen this SQLite database"));
            m_dontShowMainWindow = true;
        } else if(arguments().at(i) == "-s" || arguments().at(i) == "--sql") {
            // Run SQL file: If file exists add it to list of scripts to execute
            if(++i >= arguments().size())
                qWarning() << qPrintable(tr("The -s/--sql option requires an argument"));
            else if(!QFile::exists(arguments().at(i)))
                qWarning() << qPrintable(tr("The file %1 does not exist").arg(arguments().at(i)));
            else
                sqlToExecute.append(arguments().at(i));
        } else if(arguments().at(i) == "-q" || arguments().at(i) == "--quit") {
            m_dontShowMainWindow = true;
        } else {
            // Other: Check if it's a valid file name
            if(QFile::exists(arguments().at(i)))
                fileToOpen = arguments().at(i);
            else
                qWarning() << qPrintable(tr("Invalid option/non-existant file: %1").arg(arguments().at(i)));
        }
    }

    // Show main window
    m_mainWindow = new MainWindow();
    m_mainWindow->show();
    connect(this, SIGNAL(lastWindowClosed()), this, SLOT(quit()));

    // Open database if one was specified
    if(fileToOpen.size())
    {
        if(m_mainWindow->fileOpen(fileToOpen))
        {
            // If database could be opened run the SQL scripts
            foreach(const QString& f, sqlToExecute)
            {
                QFile file(f);
                if(file.open(QIODevice::ReadOnly))
                {
                    m_mainWindow->getDb()->executeMultiSQL(file.readAll(), false, true);
                    file.close();
                }
            }
            if(!sqlToExecute.isEmpty())
                m_mainWindow->browseRefresh();
        }
    }
}

Application::~Application()
{
    delete m_mainWindow;
}

bool Application::event(QEvent* event)
{
    switch(event->type())
    {
    case QEvent::FileOpen:
        m_mainWindow->fileOpen(static_cast<QFileOpenEvent*>(event)->file());
        return true;
    default:
        return QApplication::event(event);
    }
}
