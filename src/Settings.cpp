#include "Settings.h"

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QColor>
#include <QFontInfo>
#include <QLocale>
#include <QStandardPaths>
#include <QPalette>

std::unordered_map<std::string, QVariant> Settings::m_hCache;
int Settings::m_defaultFontSize;

static bool ends_with(const std::string& str, const std::string& with)
{
    return str.rfind(with) == str.size() - with.size();
}

QVariant Settings::getValue(const std::string& group, const std::string& name)
{
    // Have a look in the cache first
    auto cacheIndex = m_hCache.find(group + name);
    if(cacheIndex != m_hCache.end())
    {
        return cacheIndex->second;
    } else {
        // Nothing found in the cache, so get the value from the settings file or get the default value if there is no value set yet
        QSettings settings(QApplication::organizationName(), QApplication::organizationName());
        QVariant value = settings.value(QString::fromStdString(group + "/" + name), getDefaultValue(group, name));

        // Store this value in the cache for further usage and return it afterwards
        m_hCache.insert({group + name, value});
        return value;
    }
}

void Settings::setValue(const std::string& group, const std::string& name, const QVariant& value, bool dont_save_to_disk)
{
    // Sometime the value has to be saved for the current session only but get discarded when the application exits.
    // In order to achieve this this flag can be set which disables the save to disk mechanism and only leaves the save to cache part active.
    if(dont_save_to_disk == false)
    {
        // Set the group and save the given value
        QSettings settings(QApplication::organizationName(), QApplication::organizationName());
        settings.beginGroup(QString::fromStdString(group));
        settings.setValue(QString::fromStdString(name), value);
        settings.endGroup();
    }

    // Also change it in the cache
    m_hCache[group + name] = value;
}

QVariant Settings::getDefaultValue(const std::string& group, const std::string& name)
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
        return 50000U;

    // db/defaultsqltext?
    if(group == "db" && name == "defaultsqltext")
        return QString();

    // db/fontsize?
    if(group == "db" && name == "fontsize")
        return 10;

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
        return QString();

    // MainWindow/windowState?
    if(group == "MainWindow" && name == "windowState")
        return QString();

    // MainWindow/openTabs?
    if(group == "MainWindow" && name == "openTabs")
        return QString();

    // SQLLogDock/Log?
    if(group == "SQLLogDock" && name == "Log")
        return "Application";

    // General/recentFileList?
    if(group == "General" && name == "recentFileList")
        return QStringList();

    // General/maxRecentFiles?
    if(group == "General" && name == "maxRecentFiles")
        return 5;

    // General/language?
    if(group == "General" && name == "language")
        return QLocale::system().name();

    // General/appStyle
    if(group == "General" && name == "appStyle")
        return static_cast<int>(FollowDesktopStyle);

    // General/toolbarStyle
    if(group == "General" && name == "toolbarStyle")
        return static_cast<int>(Qt::ToolButtonTextBesideIcon);

    // General/toolbarStyleStructure
    if(group == "General" && name == "toolbarStyleStructure")
        return static_cast<int>(Qt::ToolButtonTextBesideIcon);

    // General/toolbarStyleBrowse
    if(group == "General" && name == "toolbarStyleBrowse")
        return static_cast<int>(Qt::ToolButtonIconOnly);

    // General/toolbarStyleSql
    if(group == "General" && name == "toolbarStyleSql")
        return static_cast<int>(Qt::ToolButtonIconOnly);

    // General/toolbarStyleEditCell
    if(group == "General" && name == "toolbarStyleEditCell")
        return static_cast<int>(Qt::ToolButtonIconOnly);

    if(group == "General" && name == "DBFileExtensions")
        return QObject::tr("SQLite database files (*.db *.sqlite *.sqlite3 *.db3)");

    // General/fontsize
    if(group == "General" && name == "fontsize")
        return m_defaultFontSize;

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
        if(name == "image_preview")
            return false;
        if(name == "indent_compact")
            return false;
        if(name == "auto_switch_mode")
            return true;
        if(name == "editor_word_wrap")
            return true;
        if(name == "null_text")
            return "NULL";
        if(name == "blob_text")
            return "BLOB";
        if(name == "filter_escape")
            return "\\";
        if(name == "filter_delay")
            return 200;
        if(ends_with(name, "colour"))
            return getDefaultColorValue(group, name, FollowDesktopStyle);
    }

    // syntaxhighlighter?
    if(group == "syntaxhighlighter")
    {
        // Bold? Only tables, functions and keywords are bold by default
        if(ends_with(name, "bold"))
            return name == "keyword_bold" || name == "table_bold" || name == "function_bold";

        // Italic? Nothing by default
        if(ends_with(name, "italic"))
            return false;

        // Underline? Nothing by default
        if(ends_with(name, "underline"))
            return false;

        // Colour?
        if(ends_with(name, "colour"))
            return getDefaultColorValue(group, name, FollowDesktopStyle);
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
       // Use 12 pt size as the default on macOS
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

    // editor/splitter1_sizes?
    if(group == "editor" && name == "splitter1_sizes")
        return QVariant();

    // editor/splitter2_sizes?
    if(group == "editor" && name == "splitter2_sizes")
        return QVariant();

    // editor/close_button_on_tabs?
    if(group == "editor" && name == "close_button_on_tabs")
        return true;

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

    // Proxy settings
    if(group == "proxy")
    {
        // Use system settings by default
        if(name == "type")
            return "system";
    }

    // Unknown combination of group and name? Return an invalid QVariant!
    return QVariant();
}

QColor Settings::getDefaultColorValue(const std::string& group, const std::string& name, AppStyle style)
{
    // Data Browser/NULL & Binary Fields
    if(group == "databrowser")
    {
        switch (style) {
        case FollowDesktopStyle :
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
            break;
        case DarkStyle :
            if(name == "null_fg_colour")
                return QColor("#787878");
            if(name == "null_bg_colour")
                return QColor("#19232D");
            if(name == "reg_fg_colour")
                return QColor("#F0F0F0");
            if(name == "reg_bg_colour")
                return QColor("#19232D");
            if(name == "bin_fg_colour")
                return QColor("#787878");
            if(name == "bin_bg_colour")
                return QColor("#19232D");
            break;
        }
    }

    // syntaxhighlighter?
    if(group == "syntaxhighlighter")
    {
        // Colour?
        if(ends_with(name, "colour"))
        {
            QColor backgroundColour;
            QColor foregroundColour;

            switch (style) {
            case FollowDesktopStyle :
                backgroundColour = QPalette().color(QPalette::Active, QPalette::Base);
                foregroundColour = QPalette().color(QPalette::Active, QPalette::Text);
                break;
            case DarkStyle :
                foregroundColour = QColor("#F0F0F0");
                backgroundColour = QColor("#19232D");
                break;
            }
            if(name == "foreground_colour")
                return foregroundColour;
            else if(name == "background_colour")
                return backgroundColour;
            else if(name == "selected_fg_colour")
                return QPalette().color(QPalette::Active, QPalette::HighlightedText);
            else if(name == "selected_bg_colour")
                return QPalette().color(QPalette::Active, QPalette::Highlight);

            // Detect and provide sensible defaults for dark themes
            if (backgroundColour.value() < foregroundColour.value()) {
                if(name == "keyword_colour")
                    return QColor(82, 148, 226);
                else if(name == "function_colour")
                    return QColor(Qt::yellow);
                else if(name == "table_colour")
                    return QColor(Qt::cyan);
                else if(name == "comment_colour")
                    return QColor(Qt::green);
                else if(name == "identifier_colour")
                    return QColor(Qt::magenta);
                else if(name == "string_colour")
                    return QColor(Qt::lightGray);
                else if(name == "currentline_colour")
                    return backgroundColour.lighter(150);
                else if(name == "highlight_colour")
                    return QColor(79, 148, 205);
            } else {
                if(name == "keyword_colour")
                    return QColor(Qt::darkBlue);
                else if(name == "function_colour")
                    return QColor(Qt::blue);
                else if(name == "table_colour")
                    return QColor(Qt::darkCyan);
                else if(name == "comment_colour")
                    return QColor(Qt::darkGreen);
                else if(name == "identifier_colour")
                    return QColor(Qt::darkMagenta);
                else if(name == "string_colour")
                    return QColor(Qt::red);
                else if(name == "currentline_colour")
                    return QColor(236, 236, 245);
                else if(name == "highlight_colour")
                    return QColor(Qt::cyan);
            }
        }
    }

    // Unknown combination of group and name? Return an invalid QColor!
    return QColor();
}

void Settings::clearValue(const std::string& group, const std::string& name)
{
    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
    settings.beginGroup(QString::fromStdString(group));
    settings.remove(QString::fromStdString(name));
    settings.endGroup();
    m_hCache.clear();
}

void Settings::restoreDefaults ()
{
    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
    settings.clear();
    m_hCache.clear();
}
