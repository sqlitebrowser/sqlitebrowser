#include <QFile>
#include <QFileOpenEvent>
#include <QTranslator>
#include <QTextCodec>
#include <QLibraryInfo>
#include <QLocale>
#include <QDebug>
#include <QAction>
#include <QFileInfo>

#include "Application.h"
#include "MainWindow.h"
#include "RemoteNetwork.h"
#include "Settings.h"
#include "version.h"
#include "sqlitedb.h"

void printArgument(const QString& argument, const QString& description)
{
    const int fieldWidth = 20;

    // Format the usage message so translators do not have to do it themselves
    if(argument.length() > fieldWidth) {
        qWarning() << qPrintable(QString("  %1").arg(argument, -fieldWidth));
        qWarning() << qPrintable(QString("  %1%2").arg("", -fieldWidth).arg(description));
    } else {
        qWarning() << qPrintable(QString("  %1%2").arg(argument, -fieldWidth).arg(description));
    }
}

Application::Application(int& argc, char** argv) :
    QApplication(argc, argv)
{
    // Get 'DB4S_SETTINGS_FILE' environment variable
    const char* env = getenv("DB4S_SETTINGS_FILE");

    // If 'DB4S_SETTINGS_FILE' environment variable exists
    if(env)
        Settings::setUserSettingsFile(QString::fromStdString(env));

    for(int i=1;i<arguments().size();i++)
    {
        if(arguments().at(i) == "-S" || arguments().at(i) == "--settings")
        {
            if(++i < arguments().size())
            {
                if(env)
                {
                    qWarning() << qPrintable(tr("The user settings file location is replaced with the argument value instead of the environment variable value."));
                    qWarning() << qPrintable(tr("Ignored environment variable(DB4S_SETTINGS_FILE) value : ") + QString::fromStdString(env));
                }
                Settings::setUserSettingsFile(arguments().at(i));
            }
        }
    }

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

        // Don't install a translator for Qt texts if no translator for DB4S texts could be loaded
        m_translatorQt = nullptr;
    }

    // On Windows, add the plugins subdirectory to the list of library directories. We need this
    // for Qt to search for more image format plugins.
#ifdef Q_WS_WIN
    QApplication::addLibraryPath(QApplication::applicationDirPath() + "/plugins");
#endif

    // Work around a bug in QNetworkAccessManager which sporadically causes high pings for Wifi connections
    // See https://bugreports.qt.io/browse/QTBUG-40332
    qputenv("QT_BEARER_POLL_TIMEOUT", QByteArray::number(INT_MAX));

    // Remember default font size
    Settings::rememberDefaultFontSize(font().pointSize());

    // Parse command line
    QString fileToOpen;
    std::vector<QString> tableToBrowse;
    std::vector<QString> csvToImport;
    QStringList sqlToExecute;
    bool readOnly = false;
    m_showMainWindow = true;
    for(int i=1;i<arguments().size();i++)
    {
        // Check next command line argument
        if(arguments().at(i) == "-h" || arguments().at(i) == "--help")
        {
            // Help
            qWarning() << qPrintable(QString("%1: %2 [%3] [<%4>|<%5>]\n").
                                     arg(tr("Usage")).arg(QFileInfo(argv[0]).fileName()).
                                     arg(tr("options")).arg(tr("database")).arg(tr("project")));

            qWarning() << qPrintable(tr("Possible command line arguments:"));
            printArgument(QString("-h, --help"),
                          tr("Show command line options"));
            printArgument(QString("-q, --quit"),
                          tr("Exit application after running scripts"));
            printArgument(QString("-s, --sql <%1>").arg(tr("file")),
                          tr("Execute this SQL file after opening the DB"));
            printArgument(QString("--import-csv <%1>").arg(tr("file")),
                          tr("Import this CSV file into the passed DB or into a new DB"));
            printArgument(QString("-t, --table <%1>").arg(tr("table")),
                          tr("Browse this table after opening the DB"));
            printArgument(QString("-R, --read-only"),
                          tr("Open database in read-only mode"));
            printArgument(QString("-S, --settings <%1>").arg(tr("settings_file")),
                          tr("Run application based on this settings file"));
            printArgument(QString("-o, --option <%1>/<%2>=<%3>").arg(tr("group")).arg(tr("settings")).arg(tr("value")),
                          tr("Run application with this setting temporarily set to value"));
            printArgument(QString("-O, --save-option <%1>/<%2>=<%3>").arg(tr("group")).arg(tr("settings")).arg(tr("value")),
                          tr("Run application saving this value for this setting"));
            printArgument(QString("-v, --version"),
                          tr("Display the current version"));
            printArgument(QString("<%1>").arg(tr("database")),
                          tr("Open this SQLite database"));
            printArgument(QString("<%1>").arg(tr("project")),
                          tr("Open this project file (*.sqbpro)"));
            m_showMainWindow = false;
        } else if(arguments().at(i) == "-v" || arguments().at(i) == "--version") {
            qWarning() << qPrintable(versionInformation());
            m_showMainWindow = false;
        } else if(arguments().at(i) == "-s" || arguments().at(i) == "--sql") {
            // Run SQL file: If file exists add it to list of scripts to execute
            if(++i >= arguments().size())
                qWarning() << qPrintable(tr("The %1 option requires an argument").arg("-s/--sql"));
            else if(!QFile::exists(arguments().at(i)))
                qWarning() << qPrintable(tr("The file %1 does not exist").arg(arguments().at(i)));
            else
                sqlToExecute.append(arguments().at(i));
        } else if(arguments().at(i) == "-t" || arguments().at(i) == "--table") {
            if(++i >= arguments().size())
                qWarning() << qPrintable(tr("The %1 option requires an argument").arg("-t/--table"));
            else
                tableToBrowse.push_back(arguments().at(i));
        } else if(arguments().at(i) == "--import-csv") {
            if(++i >= arguments().size())
                qWarning() << qPrintable(tr("The %1 option requires an argument").arg("--import-csv"));
            else if(!QFile::exists(arguments().at(i)))
                qWarning() << qPrintable(tr("The file %1 does not exist").arg(arguments().at(i)));
            else
                csvToImport.push_back(arguments().at(i));
        } else if(arguments().at(i) == "-q" || arguments().at(i) == "--quit") {
            m_showMainWindow = false;
        } else if(arguments().at(i) == "-R" || arguments().at(i) == "--read-only") {
            readOnly = true;
        } else if(arguments().at(i) == "-S" || arguments().at(i) == "--settings") {
            // This option has already been handled above
            // For here, only print the error when no parameter value is given
            if(++i >= arguments().size())
                qWarning() << qPrintable(tr("The -S/--settings option requires an argument. The option is ignored."));
        } else if(arguments().at(i) == "-o" || arguments().at(i) == "--option" ||
                  arguments().at(i) == "-O" || arguments().at(i) == "--save-option") {
            const QString optionWarning = tr("The -o/--option and -O/--save-option options require an argument in the form group/setting=value");
            bool saveToDisk = arguments().at(i) == "-O" || arguments().at(i) == "--save-option";
            if(++i >= arguments().size())
                qWarning() << qPrintable(optionWarning);
            else {
                QStringList option = arguments().at(i).split("=");
                if (option.size() != 2)
                    qWarning() << qPrintable(optionWarning);
                else {
                    QStringList setting = option.at(0).split("/");
                    if (setting.size() != 2)
                        qWarning() << qPrintable(optionWarning);
                    else {
                        QVariant value;
                        // Split string lists. This assumes they are always named "*list"
                        if (setting.at(1).endsWith("list", Qt::CaseInsensitive))
                            value = option.at(1).split(",");
                        else
                            value = option.at(1);
                        Settings::setValue(setting.at(0).toStdString(), setting.at(1).toStdString(), value, saveToDisk);
                    }
                }
            }
        } else {
            // Other: Check if it's a valid file name
            if(QFile::exists(arguments().at(i)))
                fileToOpen = arguments().at(i);
            else
                qWarning() << qPrintable(tr("Invalid option/non-existant file: %1").arg(arguments().at(i)));
        }
    }

    if(!m_showMainWindow) {
        m_mainWindow = nullptr;
        return;
    }

    // Show main window
    m_mainWindow = new MainWindow();
    m_mainWindow->show();
    connect(this, &Application::lastWindowClosed, this, &Application::quit);

    // Open database if one was specified
    if(fileToOpen.size())
    {
        if(m_mainWindow->fileOpen(fileToOpen, false, readOnly))
        {
            // If database could be opened run the SQL scripts
            for(const QString& f : sqlToExecute)
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
            for(const QString& t : tableToBrowse)
                m_mainWindow->switchToBrowseDataTab(sqlb::ObjectIdentifier("main", t.toStdString()));
            if(!tableToBrowse.empty())
                m_mainWindow->refresh();
        }
    }
    if(!csvToImport.empty())
        m_mainWindow->importCSVfiles(csvToImport);
}

Application::~Application()
{
    if(m_mainWindow)
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

QString Application::versionString()
{
    // Nightly builds use a patch number of 99, and for these we include the build date in the version string.  For
    // our release versions (which use any other patch number) we don't include the build date. This should avoid
    // confusion about what is more important, version number or build date, and about different build dates for
    // the same version. This also should help making release builds reproducible out of the box.
#if PATCH_VERSION == 99
    return QString("%1 (%2)").arg(APP_VERSION, __DATE__);
#else
    return QString("%1").arg(APP_VERSION);
#endif
}

QString Application::versionInformation()
{
    QString sqlite_version, sqlcipher_version;
    DBBrowserDB::getSqliteVersion(sqlite_version, sqlcipher_version);
    if(sqlcipher_version.isNull())
        sqlite_version = tr("SQLite Version ") + sqlite_version;
    else
        sqlite_version = tr("SQLCipher Version %1 (based on SQLite %2)").arg(sqlcipher_version, sqlite_version);

    return
        tr("DB Browser for SQLite Version %1.").arg(versionString() + "\n\n" +
        tr("Built for %1, running on %2").arg(QSysInfo::buildAbi(), QSysInfo::currentCpuArchitecture()) + "\n" +
        tr("Qt Version %1").arg(QT_VERSION_STR) + "\n" +
        sqlite_version
        );
}

void Application::reloadSettings()
{
    // Network settings
    RemoteNetwork::get().reloadSettings();

    // Font settings
    QFont f = font();
    f.setPointSize(Settings::getValue("General", "fontsize").toInt());
    setFont(f);
}

// Functions for documenting the shortcuts in the user interface using native names
static QString shortcutsTip(const QList<QKeySequence>& keys)
{
    QString tip;

    if (!keys.isEmpty()) {
        tip = " [";

        for (const auto& shortcut : keys)
            tip.append(shortcut.toString(QKeySequence::NativeText) + ", ");
        tip.chop(2);

        tip.append("]");
    }
    return tip;
}

void addShortcutsTooltip(QAction* action, const QList<QKeySequence>& extraKeys)
{
    if (!action->shortcuts().isEmpty() || !extraKeys.isEmpty())
        action->setToolTip(action->toolTip() + shortcutsTip(action->shortcuts() + extraKeys));
}
