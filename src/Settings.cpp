#include "Settings.h"

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QColor>
#include <QFontInfo>
#include <QLocale>
#include <QStandardPaths>
#include <QPalette>

QHash<QString, QVariant> Settings::m_hCache;

QVariant Settings::getValue(const QString& group, const QString& name)
{
    // Have a look in the cache first
    auto cacheIndex = m_hCache.find(group + name);
    if(cacheIndex != m_hCache.end())
    {
        return cacheIndex.value();
    } else {
        // Nothing found in the cache, so get the value from the settings file or get the default value if there is no value set yet
        QSettings settings(QApplication::organizationName(), QApplication::organizationName());
        QVariant value = settings.value(group + "/" + name, getDefaultValue(group, name));

        // Store this value in the cache for further usage and return it afterwards
        m_hCache.insert(group + name, value);
        return value;
    }
}

void Settings::setValue(const QString& group, const QString& name, const QVariant& value, bool dont_save_to_disk)
{
    // Sometime the value has to be saved for the current session only but get discarded when the application exits.
    // In order to achieve this this flag can be set which disables the save to disk mechanism and only leaves the save to cache part active.
    if(dont_save_to_disk == false)
    {
        // Set the group and save the given value
        QSettings settings(QApplication::organizationName(), QApplication::organizationName());
        settings.beginGroup(group);
        settings.setValue(name, value);
        settings.endGroup();
    }

    // Also change it in the cache
    m_hCache[group + name] = value;
}

QVariant Settings::getDefaultValue(const QString& group, const QString& name)
{
    // db/defaultencoding?
    if(group == "db" && name == "defaultencoding")
        return "UTF-8";

    // db/savedefaultlocation?
    if(group == "db" && name == "savedefaultlocation")
        return 2;

    // db/defaultlocation?
    if(group == "db" && name == "defaultlocation")
        return QDir::homePath();

    // db/lastlocation?
    if(group == "db" && name == "lastlocation")
        return getValue("db", "defaultlocation");

    // db/hideschemalinebreaks?
    if(group == "db" && name == "hideschemalinebreaks")
        return true;

    // db/foreignkeys?
    if(group == "db" && name == "foreignkeys")
        return true;

    // db/prefetchsize?
    if(group == "db" && name == "prefetchsize")
        return 50000;

    // db/defaultsqltext?
    if(group == "db" && name == "defaultsqltext")
        return "";

    // exportcsv/firstrowheader?
    if(group == "exportcsv" && name == "firstrowheader")
        return true;

    // exportcsv/separator?
    if(group == "exportcsv" && name == "separator")
        return ',';

    // exportcsv/quotecharacter?
    if(group == "exportcsv" && name == "quotecharacter")
        return '"';

    // importcsv group?
    if(group == "importcsv")
    {
        if(name == "firstrowheader")
            return false;
        if(name == "trimfields")
            return true;
        if(name == "separatetables")
            return false;
        if(name == "separator")
            return ',';
        if(name == "quotecharacter")
            return '"';
        if(name == "encoding")
            return "UTF-8";
    }

    // exportsql group?
    if(group == "exportsql")
    {
        if(name == "insertcolnames" || name == "insertmultiple")
            return false;
        if(name == "oldschema")
            return 0;
    }

    // newline character
    if (group == "exportcsv" && name == "newlinecharacters")
#ifdef Q_OS_WIN
        return "\r\n";
#else
        return "\n";
#endif

    // exportjson/prettyprint?
    if(group == "exportjson" && name == "prettyprint")
        return true;

    // MainWindow/geometry?
    if(group == "MainWindow" && name == "geometry")
        return "";

    // MainWindow/windowState?
    if(group == "MainWindow" && name == "windowState")
        return "";

    // MainWindow/openTabs?
    if(group == "MainWindow" && name == "openTabs")
        return "";

    // SQLLogDock/Log?
    if(group == "SQLLogDock" && name == "Log")
        return "Application";

    // General/recentFileList?
    if(group == "General" && name == "recentFileList")
        return QStringList();

    // General/language?
    if(group == "General" && name == "language")
        return QLocale::system().name();

    // General/toolbarStyle
    if(group == "General" && name == "toolbarStyle")
        return static_cast<int>(Qt::ToolButtonTextBesideIcon);

    if(group == "General" && name == "DBFileExtensions")
        return QObject::tr("SQLite database files (*.db *.sqlite *.sqlite3 *.db3)");

    // checkversion group?
    if(group == "checkversion")
    {
        if(name == "enabled")
            return true;
        if(name == "ignmajor")
            return 999;
        if(name == "ignminor" || name == "ignpatch")
            return 0;
    }

    // Data Browser/NULL Fields
    if(group == "databrowser")
    {
        if(name == "font")
            return QFont().defaultFamily();
        if(name == "fontsize")
            return 10;
        if(name == "symbol_limit")
            return 5000;
        if(name == "complete_threshold")
            return 1000;
        if(name == "indent_compact")
            return false;
        if(name == "auto_switch_mode")
            return true;
        if(name == "null_text")
            return "NULL";
        if(name == "blob_text")
            return "BLOB";
        if(name == "filter_escape")
            return "\\";
        if(name == "filter_delay")
            return 200;
        if(name == "null_fg_colour")
            return QColor(Qt::lightGray).name();
        if(name == "null_bg_colour")
            return QPalette().color(QPalette::Active, QPalette::Base).name();
        if(name == "reg_fg_colour")
            return QPalette().color(QPalette::Active, QPalette::Text).name();
        if(name == "reg_bg_colour")
            return QPalette().color(QPalette::Active, QPalette::Base).name();
        if(name == "bin_fg_colour")
            return QColor(Qt::lightGray).name();
        if(name == "bin_bg_colour")
            return QPalette().color(QPalette::Active, QPalette::Base).name();
    }

    // syntaxhighlighter?
    if(group == "syntaxhighlighter")
    {
        // Bold? Only tables, functions and keywords are bold by default
        if(name.right(4) == "bold")
            return name == "keyword_bold" || name == "table_bold" || name == "function_bold";

        // Italic? Nothing by default
        if(name.right(6) == "italic")
            return false;

        // Underline? Nothing by default
        if(name.right(9) == "underline")
            return false;

        // Colour?
        if(name.right(6) == "colour")
        {
            QColor backgroundColour = QPalette().color(QPalette::Active, QPalette::Base);
            QColor foregroundColour = QPalette().color(QPalette::Active, QPalette::Text);

            if(name == "foreground_colour")
                return foregroundColour.name();
            else if(name == "background_colour")
                return backgroundColour.name();

            // Detect and provide sensible defaults for dark themes
            if (backgroundColour.value() < foregroundColour.value()) {
                if(name == "keyword_colour")
                  return QColor(82, 148, 226).name();
                else if(name == "function_colour")
                    return QColor(Qt::yellow).name();
                else if(name == "table_colour")
                    return QColor(Qt::cyan).name();
                else if(name == "comment_colour")
                    return QColor(Qt::green).name();
                else if(name == "identifier_colour")
                    return QColor(Qt::magenta).name();
                else if(name == "string_colour")
                    return QColor(Qt::lightGray).name();
                else if(name == "currentline_colour")
                    return backgroundColour.lighter(150).name();
                else if(name == "background_colour")
                    return backgroundColour.name();
            } else {
                if(name == "keyword_colour")
                    return QColor(Qt::darkBlue).name();
                else if(name == "function_colour")
                    return QColor(Qt::blue).name();
                else if(name == "table_colour")
                    return QColor(Qt::darkCyan).name();
                else if(name == "comment_colour")
                    return QColor(Qt::darkGreen).name();
                else if(name == "identifier_colour")
                    return QColor(Qt::darkMagenta).name();
                else if(name == "string_colour")
                    return QColor(Qt::red).name();
                else if(name == "currentline_colour")
                    return QColor(236, 236, 245).name();
                else if(name == "background_colour")
                    return backgroundColour.name();
            }
        }
    }

    // editor/font?
    if(group == "editor" && name == "font")
    {
        QFont font("Monospace");
        font.setStyleHint(QFont::TypeWriter);
        return QFontInfo(font).family();
    }

    // editor/fontsize or log/fontsize?
    if((group == "editor" || group == "log") && name == "fontsize")
#ifdef Q_OS_MAC
       // Use 12 pt size as the default on OSX
        return 12;
#else
        return 9;
#endif

    if(group == "editor")
    {
        if(name == "tabsize")
        {
            return 4;
        }
    }

    // editor/wrap_lines
    if(group == "editor" && name == "wrap_lines")
        return 0; // QsciScintilla::WrapNone

    // editor/identifier_quotes
    if(group == "editor" && name == "identifier_quotes")
        return 0; // sqlb::DoubleQuotes

    // editor/auto_completion?
    if(group == "editor" && name == "auto_completion")
        return true;

    // editor/upper_keywords?
    if(group == "editor" && name == "upper_keywords")
        return true;

    // editor/error_indicators?
    if(group == "editor" && name == "error_indicators")
        return true;

    // editor/horizontal_tiling?
    if(group == "editor" && name == "horizontal_tiling")
        return false;

    // extensions/list?
    if(group == "extensions" && name == "list")
        return QStringList();

    // extensions/disableregex?
    if(group == "extension" && name == "disableregex")
        return false;

    // extensions/enable_load_extension?
    if(group == "extension" && name == "enable_load_extension")
        return false;

    // PlotDock/lineType or pointShape?
    if(group == "PlotDock")
    {
        // QCPGraph::lsLine
        if(name == "lineType")
            return 1;

        // QCPScatterStyle::ssDisk
        if(name == "pointShape")
            return 4;
    }


    // SchemaDock Drag & drop settings
    if(group == "SchemaDock")
    {
        if(name == "dropQualifiedNames")
            return false;

        if(name == "dropEnquotedNames")
            return true;
    }

    // Remote settings?
    if(group == "remote")
    {
        // Enable the File → Remote menu by default
        if(name == "active")
            return true;

        // Clone directory
        if(name == "clonedirectory")
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
            return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#else
            return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
#endif
    }

    // Unknown combination of group and name? Return an invalid QVariant!
    return QVariant();
}

void Settings::restoreDefaults ()
{
    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
    settings.clear();
    m_hCache.clear();
}
