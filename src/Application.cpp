#include <QFile>
#include <QFileOpenEvent>
#include <QTranslator>
#include <QTextCodec>
#include <QLibraryInfo>
#include <QLocale>
#include <QDebug>

#include "Application.h"
#include "MainWindow.h"
#include "Settings.h"
#include "version.h"

Application::Application(int& argc, char** argv) :
    QApplication(argc, argv)
{
    // Set organisation and application names
    setOrganizationName("sqlitebrowser");
    setApplicationName("DB Browser for SQLite");

    // Set character encoding to UTF8
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // Load translations
    bool ok;
    QString name = Settings::getValue("General", "language").toString();

    // First of all try to load the application translation file.
    m_translatorApp = new QTranslator(this);
    ok = m_translatorApp->load("sqlb_" + name,
                               QCoreApplication::applicationDirPath() + "/translations");
    // If failed then try to load .qm file from resources
    if (ok == false) {
        ok = m_translatorApp->load("sqlb_" + name, ":/translations");
    }

    if (ok == true) {
        Settings::setValue("General", "language", name);
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
    } else {
        // Set the correct locale so that the program won't erroneously detect
        // a language change when the user toggles settings for the first time.
        // (it also prevents the program from always looking for a translation on launch)
        Settings::setValue("General", "language", "en_US");
    }

    // Parse command line
    QString fileToOpen;
    QString tableToBrowse;
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
            qWarning() << qPrintable(tr("  -q, --quit\t\tExit application after running scripts"));
            qWarning() << qPrintable(tr("  -s, --sql [file]\tExecute this SQL file after opening the DB"));
            qWarning() << qPrintable(tr("  -t, --table [table]\tBrowse this table after opening the DB"));
            qWarning() << qPrintable(tr("  -v, --version\t\tDisplay the current version"));
            qWarning() << qPrintable(tr("  [file]\t\tOpen this SQLite database"));
            m_dontShowMainWindow = true;
        } else if(arguments().at(i) == "-v" || arguments().at(i) == "--version") {
            // Distinguish between high and low patch version numbers. High numbers as in x.y.99 indicate nightly builds or
            // beta releases. For these we want to include the build date. For the release versions we don't add the release
            // date in order to avoid confusion about what is more important, version number or build date, and about different
            // build dates for the same version. This also should help making release builds reproducible out of the box.
#if PATCH_VERSION >= 99
            QString build_date = QString(" (%1)").arg(__DATE__);
#else
            QString build_date;
#endif
            qWarning() << qPrintable(tr("This is DB Browser for SQLite version %1%2.").arg(APP_VERSION).arg(build_date));
            m_dontShowMainWindow = true;
        } else if(arguments().at(i) == "-s" || arguments().at(i) == "--sql") {
            // Run SQL file: If file exists add it to list of scripts to execute
            if(++i >= arguments().size())
                qWarning() << qPrintable(tr("The -s/--sql option requires an argument"));
            else if(!QFile::exists(arguments().at(i)))
                qWarning() << qPrintable(tr("The file %1 does not exist").arg(arguments().at(i)));
            else
                sqlToExecute.append(arguments().at(i));
        } else if(arguments().at(i) == "-t" || arguments().at(i) == "--table") {
            if(++i >= arguments().size())
                qWarning() << qPrintable(tr("The -t/--table option requires an argument"));
            else
                tableToBrowse = arguments().at(i);
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
                    m_mainWindow->getDb().executeMultiSQL(file.readAll(), false, true);
                    file.close();
                }
            }
            if(!sqlToExecute.isEmpty())
                m_mainWindow->refresh();

            // Jump to table if the -t/--table parameter was set
            if(!tableToBrowse.isEmpty())
                m_mainWindow->switchToBrowseDataTab(tableToBrowse);
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
