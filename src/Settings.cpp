#include "Settings.h"

#include <QDir>
#include <QSettings>
#include <QColor>
#include <QFontInfo>
#include <QLocale>
#include <QStandardPaths>

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

    // SQLLogDock/Log?
    if(group == "SQLLogDock" && name == "Log")
        return "Application";

    // General/recentFileList?
    if(group == "General" && name == "recentFileList")
        return QStringList();

    // General/language?
    if(group == "General" && name == "language")
        return QLocale::system().name();

    // checkversion/enabled
    if(group == "checkversion" && name == "enabled")
        return true;

    // Data Browser/NULL Fields
    if(group == "databrowser")
    {
        if(name == "font")
            return QFont().defaultFamily();
        if(name == "fontsize")
            return 10;
        if(name == "symbol_limit")
            return 5000;
        if(name == "null_text")
            return "NULL";
        if(name == "filter_escape")
            return "\\";
        if(name == "filter_delay")
            return 200;
        if(name == "null_fg_colour")
            return QColor(Qt::lightGray).name();
        if(name == "null_bg_colour")
            return QColor(Qt::white).name();
        if(name == "reg_fg_colour")
            return QColor(Qt::black).name();
        if(name == "reg_bg_colour")
            return QColor(Qt::white).name();
        if(name == "bin_fg_colour")
            return QColor(Qt::lightGray).name();
        if(name == "bin_bg_colour")
            return QColor(Qt::white).name();
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

    // editor/auto_completion?
    if(group == "editor" && name == "auto_completion")
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

    // Remote settings?
    if(group == "remote")
    {
        // Enable the File → Remote menu by default
        if(name == "active")
            return true;

        // Clone directory
        if(name == "clonedirectory")
            return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }

    // Unknown combination of group and name? Return an invalid QVariant!
    return QVariant();
}
