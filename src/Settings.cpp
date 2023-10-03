#include "Settings.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QColor>
#include <QFontInfo>
#include <QLocale>
#include <QStandardPaths>
#include <QPalette>

QString Settings::userSettingsFile;
QSettings* Settings::settings;
std::unordered_map<std::string, QVariant> Settings::m_hCache;
int Settings::m_defaultFontSize;

static bool ends_with(const std::string& str, const std::string& with)
{
    return str.rfind(with) == str.size() - with.size();
}

void Settings::setUserSettingsFile(const QString& settings_file_path)
{
    userSettingsFile = settings_file_path;
}

bool Settings::isValidSettingsFile(const QString& settings_file_path)
{
    /*
    Variable that stores whether or not the settings file requested by the user is a normal settings file
    If the file does not exist and is newly created, the if statement below is not executed, so the default value is set to true
    */

    bool isNormalUserSettingsFile = true;

    // Code that verifies that the settings file requested by the user is a normal settings file
    if(settings_file_path != nullptr)
    {
        QFile *file = new QFile;
        file->setFileName(settings_file_path);

        if(file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            // define comparison string --> "[General]\n" --> first line of any DB4S settings file
            QString header("[" + QString::fromStdString(szINI::SEC_GENERAL) + "]\n");
            if(file->exists() &&
              QString::compare(header, file->readLine(), Qt::CaseInsensitive) != 0)
                isNormalUserSettingsFile = false;
        }

        file->close();
    }

    return isNormalUserSettingsFile;
}

void Settings::setSettingsObject()
{
    // If an object has already been created, it is terminated to reduce overhead
    if(settings)
        return;

    const bool isNormalUserSettingsFile = isValidSettingsFile(userSettingsFile);

    if(userSettingsFile == nullptr)
    {
        settings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::organizationName());
    } else {
        if(isNormalUserSettingsFile)
        {
            settings = new QSettings(userSettingsFile, QSettings::IniFormat);

            // Code to verify that the user does not have access to the requested settings file
            if(settings->status() == QSettings::AccessError) {
                qWarning() << qPrintable("The given settings file can NOT access. Please check the permission for the file.");
                qWarning() << qPrintable("So, the -S/--settings option is ignored.");

                // Since you do not have permission to the file, delete the existing assignment and assign the standard
                delete settings;
                settings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::organizationName());
            }
        } else {
            qWarning() << qPrintable("The given settings file is not a normal settings file. Please check again.");
            qWarning() << qPrintable("So, the -S/--settings option is ignored.");
            settings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::organizationName());
        }
    }
}

QVariant Settings::getValue(const std::string& section, const std::string& key)
{
    // Have a look in the cache first
    auto cacheIndex = m_hCache.find(section + key);
    if(cacheIndex != m_hCache.end())
    {
        return cacheIndex->second;
    } else {
        // Nothing found in the cache, so get the value from the settings file or get the default value if there is no value set yet
        setSettingsObject();
        QVariant value = settings->value(QString::fromStdString(section + "/" + key), getDefaultValue(section, key));

        // Store this value in the cache for further usage and return it afterwards
        m_hCache.insert({section + key, value});
        return value;
    }
}

void Settings::setValue(const std::string& section, const std::string& key, const QVariant& value, bool save_to_disk)
{
    // Sometime the value has to be saved for the current session only but get discarded when the application exits.
    // In order to achieve this this flag can be set which disables the save to disk mechanism and only leaves the save to cache part active.
    if(save_to_disk)
    {
        setSettingsObject();
        // Set the group and save the given value
        settings->beginGroup(QString::fromStdString(section));
        settings->setValue(QString::fromStdString(key), value);
        settings->endGroup();
    }

    // Also change it in the cache
    m_hCache[section + key] = value;
}

QVariant Settings::getDefaultValue(const std::string& section, const std::string& key)
{
    // db/defaultencoding?
    if(section == szINI::SEC_DATABASE && key == szINI::KEY_DEFAULT_ENCODING)
        return "UTF-8";

    // db/savedefaultlocation?
    if(section == szINI::SEC_DATABASE && key == szINI::KEY_SAVE_DEFAULT_LOCATION)
        return 2;

    // db/defaultlocation?
    if(section == szINI::SEC_DATABASE && key == szINI::KEY_DEFAULT_LOCATION)
        return QDir::homePath();

    // db/lastlocation?
    if(section == szINI::SEC_DATABASE && key == szINI::KEY_LAST_LOCATION)
        return getValue(szINI::SEC_DATABASE, szINI::KEY_DEFAULT_LOCATION);

    // db/hideschemalinebreaks?
    if(section == szINI::SEC_DATABASE && key == szINI::KEY_HIDE_SCHEMA_LINEBREAKS)
        return true;

    // db/foreignkeys?
    if(section == szINI::SEC_DATABASE && key == szINI::KEY_FOREIGN_KEYS)
        return true;

    // db/prefetchsize?
    if(section == szINI::SEC_DATABASE && key == szINI::KEY_PREFETCH_SIZE)
        return 50000U;

    // db/defaultsqltext?
    if(section == szINI::SEC_DATABASE && key == szINI::KEY_DEFAULT_SQL_TEXT)
        return QString();

    // db/fontsize?
    if(section == szINI::SEC_DATABASE && key == szINI::KEY_FONTSIZE)
        return 10;

    // exportcsv/firstrowheader?
    if(section == szINI::SEC_EXPORT_CSV && key == szINI::KEY_FIRST_ROW_HEADER)
        return true;

    // exportcsv/separator?
    if(section == szINI::SEC_EXPORT_CSV && key == szINI::KEY_SEPARATOR)
        return ',';

    // exportcsv/quotecharacter?
    if(section == szINI::SEC_EXPORT_CSV && key == szINI::KEY_QUOTE_CHARACTER)
        return '"';

    // importcsv group?
    if(section == szINI::SEC_IMPORT_CSV)
    {
        if(key == szINI::KEY_FIRST_ROW_HEADER)
            return false;
        if(key == szINI::KEY_TRIM_FIELDS)
            return true;
        if(key == szINI::KEY_SEPARATE_TABLES)
            return false;
        if(key == szINI::KEY_SEPARATOR)
            return ',';
        if(key == szINI::KEY_QUOTE_CHARACTER)
            return '"';
        if(key == szINI::KEY_ENCODING)
            return "UTF-8";
        if(key == szINI::KEY_LOCAL_CONVENTIONS)
            return false;
    }

    // exportsql group?
    if(section == szINI::SEC_EXPORT_SQL)
    {
        if(key == szINI::KEY_INSERT_COL_NAMES || key == szINI::KEY_INSERT_MULTIPLE || key == szINI::KEY_KEEP_ORIGINAL)
            return false;
        if(key == szINI::KEY_OLD_SCHEMA)
            return 0;
    }

    // newline character
    if (section == szINI::SEC_EXPORT_CSV && key == szINI::KEY_NEWLINE_CHARACTERS)
#ifdef Q_OS_WIN
        return "\r\n";
#else
        return "\n";
#endif

    // exportjson/prettyprint?
    if(section == szINI::SEC_EXPORT_JSON && key == szINI::KEY_PRETTY_PRINT)
        return true;

    // MainWindow/geometry?
    if(section == szINI::SEC_MAIN_WNDW && key == szINI::KEY_GEOMETRY)
        return QString();

    // MainWindow/windowState?
    if(section == szINI::SEC_MAIN_WNDW && key == szINI::KEY_WINDOW_STATE)
        return QString();

    // MainWindow/openTabs?
    if(section == szINI::SEC_MAIN_WNDW && key == szINI::KEY_OPEN_TABS)
        return QString();

    // SQLLogDock/Log?
    if(section == szINI::SEC_SQL_LOG_DOCK && key == szINI::KEY_LOG)
        return "Application";

    // General/recentFileList?
    if(section == szINI::SEC_GENERAL && key == szINI::KEY_RECENT_FILE_LIST)
        return QStringList();

    // General/maxRecentFiles?
    if(section == szINI::SEC_GENERAL && key == szINI::KEY_MAX_RECENT_FILES)
        return 5;

    // General/language?
    if(section == szINI::SEC_GENERAL && key == szINI::KEY_LANGUAGE)
        return QLocale::system().name();

    // General/appStyle
    if(section == szINI::SEC_GENERAL && key == szINI::KEY_APPSTYLE)
        return static_cast<int>(FollowDesktopStyle);

    // General/toolbarStyle
    if(section == szINI::SEC_GENERAL && key == szINI::KEY_TB_STYLE_APP)
        return static_cast<int>(Qt::ToolButtonTextBesideIcon);

    // General/toolbarStyleStructure
    if(section == szINI::SEC_GENERAL && key == szINI::KEY_TB_STYLE_STRUCTURE)
        return static_cast<int>(Qt::ToolButtonTextBesideIcon);

    // General/toolbarStyleBrowse
    if(section == szINI::SEC_GENERAL && key == szINI::KEY_TB_STYLE_BROWSE)
        return static_cast<int>(Qt::ToolButtonIconOnly);

    // General/toolbarStyleSql
    if(section == szINI::SEC_GENERAL && key == szINI::KEY_TB_STYLE_SQL)
        return static_cast<int>(Qt::ToolButtonIconOnly);

    // General/toolbarStyleEditCell
    if(section == szINI::SEC_GENERAL && key == szINI::KEY_TB_STYLE_EDIT_CELL)
        return static_cast<int>(Qt::ToolButtonIconOnly);

    if(section == szINI::SEC_GENERAL && key == szINI::KEY_DB_FILE_EXTS)
        return QObject::tr("SQLite database files (*.db *.sqlite *.sqlite3 *.db3)");

    // General/fontsize
    if(section == szINI::SEC_GENERAL && key == szINI::KEY_FONTSIZE)
        return m_defaultFontSize;

    // General/promptsqltabsinnewproject
    if(section == szINI::SEC_GENERAL && key == szINI::KEY_PROMPT_SQL_TABS_IN_NEWPRJ)
        return true;

    // checkversion group?
    if(section == szINI::SEC_CHCK_VERSION)
    {
        if(key == szINI::KEY_ENABLED)
            return true;
        if(key == szINI::KEY_IGNMAJOR)
            return 999;
        if(key == szINI::KEY_IGNMINOR || key == szINI::KEY_IGNPATCH)
            return 0;
    }

    // Data Browser/NULL Fields
    if(section == szINI::SEC_DATA_BROWSER)
    {
        if(key == szINI::KEY_FONT) {
            QFont font("Monospace");
            font.setStyleHint(QFont::TypeWriter);
            return QFontInfo(font).family();
        }
        if(key == szINI::KEY_FONTSIZE)
            return 10;
        if(key == szINI::KEY_SYMBOL_LIMIT)
            return 5000;
        if(key == szINI::KEY_ROWS_LIMIT)
            return 10'000'000;
        if(key == szINI::KEY_COMPLETE_THRESHOLD)
            return 1000;
        if(key == szINI::KEY_IMAGE_PREVIEW)
            return false;
        if(key == szINI::KEY_INDENT_COMPACT)
            return false;
        if(key == szINI::KEY_AUTO_SWITCH_MODE)
            return true;
        if(key == szINI::KEY_EDITOR_WORD_WRAP)
            return true;
        if(key == szINI::KEY_NULL_TEXT)
            return "NULL";
        if(key == szINI::KEY_BLOB_TEXT)
            return "BLOB";
        if(key == szINI::KEY_FILTER_ESCAPE)
            return "\\";
        if(key == szINI::KEY_FILTER_DELAY)
            return 200;
        if(ends_with(key, szINI::KEY_ANY_COLOUR))
            return getDefaultColorValue(section, key, FollowDesktopStyle);
    }

    // syntaxhighlighter?
    if(section == szINI::SEC_SYNTAX_HIGHLIGHTER)
    {
        // Bold? Only tables, functions and keywords are bold by default
        if(ends_with(key, szINI::KEY_ANY_BOLD))
            return key == szINI::KEY_KEYWORD_BOLD || key == szINI::KEY_TABLE_BOLD || key == szINI::KEY_FUNC_BOLD;

        // Italic? Nothing by default
        if(ends_with(key, szINI::KEY_ANY_ITALIC))
            return false;

        // Underline? Nothing by default
        if(ends_with(key, szINI::KEY_ANY_UNDERLINE))
            return false;

        // Colour?
        if(ends_with(key, szINI::KEY_ANY_COLOUR))
            return getDefaultColorValue(section, key, FollowDesktopStyle);
    }

    // editor/font?
    if(section == szINI::SEC_EDITOR && key == szINI::KEY_FONT)
    {
        QFont font("Monospace");
        font.setStyleHint(QFont::TypeWriter);
        return QFontInfo(font).family();
    }

    // editor/fontsize or log/fontsize?
    if((section == szINI::SEC_EDITOR || section == szINI::SEC_LOG) && key == szINI::KEY_FONTSIZE)
#ifdef Q_OS_MAC
       // Use 12 pt size as the default on macOS
        return 12;
#else
        return 9;
#endif

    if(section == szINI::SEC_EDITOR)
    {
        if(key == szINI::KEY_TABSIZE)
            return 4;
        if(key == szINI::KEY_INDENTATION_USE_TABS)
            return true;
    }

    // editor/wrap_lines
    if(section == szINI::SEC_EDITOR && key == szINI::KEY_WRAP_LINES)
        return 0; // QsciScintilla::WrapNone

    // editor/identifier_quotes
    if(section == szINI::SEC_EDITOR && key == szINI::KEY_IDENTIFIER_QUOTES)
        return 0; // sqlb::DoubleQuotes

    // editor/auto_completion?
    if(section == szINI::SEC_EDITOR && key == szINI::KEY_AUTO_COMPLETION)
        return true;

    // editor/upper_keywords?
    if(section == szINI::SEC_EDITOR && key == szINI::KEY_UPPER_KEYWORDS)
        return true;

    // editor/error_indicators?
    if(section == szINI::SEC_EDITOR && key == szINI::KEY_ERROR_INDICATORS)
        return true;

    // editor/horizontal_tiling?
    if(section == szINI::SEC_EDITOR && key == szINI::KEY_HORZ_TILING)
        return false;

    // editor/splitter1_sizes?
    if(section == szINI::SEC_EDITOR && key == szINI::KEY_SPLITTER1_SIZES)
        return QVariant();

    // editor/splitter2_sizes?
    if(section == szINI::SEC_EDITOR && key == szINI::KEY_SPLITTER2_SIZES)
        return QVariant();

    // editor/close_button_on_tabs?
    if(section == szINI::SEC_EDITOR && key == szINI::KEY_CLOSE_BUTTON_ON_TABS)
        return true;

    // extensions/list?
    if(section == szINI::SEC_EXTENSIONS && key == szINI::KEY_LIST)
        return QStringList();

    // extensions/disableregex?
    if(section == szINI::SEC_EXTENSIONS && key == szINI::KEY_DISABLE_REGEX)
        return false;

    // extensions/enable_load_extension?
    if(section == szINI::SEC_EXTENSIONS && key == szINI::KEY_ENABLE_LOAD_EXTENSION)
        return false;

    // PlotDock/lineType or pointShape?
    if(section == szINI::SEC_PLOT_DOCK)
    {
        // QCPGraph::lsLine
        if(key == szINI::KEY_LINE_TYPE)
            return 1;

        // QCPScatterStyle::ssDisk
        if(key == szINI::KEY_POINT_SHAPE)
            return 4;
    }


    // SchemaDock Drag & drop settings
    if(section == szINI::SEC_SCHEMA_DOCK)
    {
        if(key == szINI::KEY_DROP_SELECT_QUERY)
            return true;

        if(key == szINI::KEY_DROP_QUALIFIED_NAMES)
            return false;

        if(key == szINI::KEY_DROP_ENQUOTED_NAMES)
            return true;
    }

    // Remote settings?
    if(section == szINI::SEC_REMOTE)
    {
        // Enable the File → Remote menu by default
        if(key == szINI::KEY_ACTIVE)
            return true;

        // Clone directory
        if(key == szINI::KEY_CLONE_DIR)
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
            return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#else
            return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
#endif
    }

    // Proxy settings
    if(section == szINI::SEC_PROXY)
    {
        // Use system settings by default
        if(key == szINI::KEY_TYPE)
            return "system";
    }

    // Unknown combination of group and name? Return an invalid QVariant!
    return QVariant();
}

QColor Settings::getDefaultColorValue(const std::string& section, const std::string& key, AppStyle style)
{
    // Data Browser/NULL & Binary Fields
    if(section == szINI::SEC_DATA_BROWSER)
    {
        // The switch on style can be removed if the following issue is fixed:
        // https://github.com/ColinDuquesnoy/QDarkStyleSheet/issues/171
        switch (style) {
        case FollowDesktopStyle :
            if(key == szINI::KEY_NULL_FG_COLOUR)
                return QColor(Qt::lightGray).name();
            if(key == szINI::KEY_NULL_BG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::Base).name();
            if(key == szINI::KEY_REG_FG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::Text).name();
            if(key == szINI::KEY_REG_BG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::Base).name();
            if(key == szINI::KEY_FMT_FG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::Text).name();
            if(key == szINI::KEY_FMT_BG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::AlternateBase).name();
            if(key == szINI::KEY_BIN_FG_COLOUR)
                return QColor(Qt::lightGray).name();
            if(key == szINI::KEY_BIN_BG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::Base).name();
            break;
        case DarkStyle :
            if(key == szINI::KEY_NULL_FG_COLOUR)
                return QColor(0x78, 0x78, 0x78);
            if(key == szINI::KEY_NULL_BG_COLOUR)
                return QColor(0x19, 0x23, 0x2D);
            if(key == szINI::KEY_REG_FG_COLOUR)
                return QColor(0xF0, 0xF0, 0xF0);
            if(key == szINI::KEY_REG_BG_COLOUR)
                return QColor(0x19, 0x23, 0x2D);
            if(key == szINI::KEY_FMT_FG_COLOUR)
                return QColor(0xF0, 0xF0, 0xF0);
            if(key == szINI::KEY_FMT_BG_COLOUR)
                return QColor(0x19, 0x23, 0x2D);
            if(key == szINI::KEY_BIN_FG_COLOUR)
                return QColor(0x78, 0x78, 0x78);
            if(key == szINI::KEY_BIN_BG_COLOUR)
                return QColor(0x19, 0x23, 0x2D);
            break;
        case LightStyle :
            if(key == szINI::KEY_NULL_FG_COLOUR || key == szINI::KEY_BIN_FG_COLOUR)
                return QColor(0xA5, 0xA9, 0xAC);
            if(key == szINI::KEY_NULL_BG_COLOUR || key == szINI::KEY_BIN_BG_COLOUR)
                return QColor(0xFA, 0xFA, 0xFA);
            if(key == szINI::KEY_REG_FG_COLOUR)
                return QColor(0x00, 0x00, 0x00);
            if(key == szINI::KEY_REG_BG_COLOUR)
                return QColor(0xFA, 0xFA, 0xFA);
            if(key == szINI::KEY_FMT_FG_COLOUR)
                return QColor(0x00, 0x00, 0x00);
            if(key == szINI::KEY_FMT_BG_COLOUR)
                return QColor(0xFA, 0xFA, 0xFA);
            break;
        }
    }

    // syntaxhighlighter?
    if(section == szINI::SEC_SYNTAX_HIGHLIGHTER)
    {
        // Colour?
        if(ends_with(key, szINI::KEY_ANY_COLOUR))
        {
            QColor backgroundColour;
            QColor foregroundColour;

            switch (style) {
            case FollowDesktopStyle :
                backgroundColour = QPalette().color(QPalette::Active, QPalette::Base);
                foregroundColour = QPalette().color(QPalette::Active, QPalette::Text);
                break;
            case DarkStyle :
                foregroundColour = QColor(0xF0, 0xF0, 0xF0);
                backgroundColour = QColor(0x19, 0x23, 0x2D);
                break;
            case LightStyle :
                foregroundColour = QColor(0x00, 0x00, 0x00);
                backgroundColour = QColor(0xFA, 0xFA, 0xFA);
                break;
            }
            if(key == szINI::KEY_FG_COLOUR)
                return foregroundColour;
            else if(key == szINI::KEY_BG_COLOUR)
                return backgroundColour;
            else if(key == szINI::KEY_SELECTED_FG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::HighlightedText);
            else if(key == szINI::KEY_SELECTED_BG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::Highlight);

            // Detect and provide sensible defaults for dark themes
            if (backgroundColour.value() < foregroundColour.value()) {
                if(key == szINI::KEY_KEYWORD_COLOUR)
                    return QColor(82, 148, 226);
                else if(key == szINI::KEY_FUNC_COLOUR)
                    return QColor(Qt::yellow);
                else if(key == szINI::KEY_TABLE_COLOUR)
                    return QColor(Qt::cyan);
                else if(key == szINI::KEY_COMMENT_COLOUR)
                    return QColor(Qt::green);
                else if(key == szINI::KEY_IDENTIFIER_COLOUR)
                    return QColor(221, 160, 221);
                else if(key == szINI::KEY_STRING_COLOUR)
                    return QColor(Qt::lightGray);
                else if(key == szINI::KEY_CUR_LINE_COLOUR)
                    return backgroundColour.lighter(150);
                else if(key == szINI::KEY_HIGHLIGHT_COLOUR)
                    return QColor(79, 148, 205);
            } else {
                if(key == szINI::KEY_KEYWORD_COLOUR)
                    return QColor(Qt::darkBlue);
                else if(key == szINI::KEY_FUNC_COLOUR)
                    return QColor(Qt::blue);
                else if(key == szINI::KEY_TABLE_COLOUR)
                    return QColor(Qt::darkCyan);
                else if(key == szINI::KEY_COMMENT_COLOUR)
                    return QColor(Qt::darkGreen);
                else if(key == szINI::KEY_IDENTIFIER_COLOUR)
                    return QColor(Qt::darkMagenta);
                else if(key == szINI::KEY_STRING_COLOUR)
                    return QColor(Qt::red);
                else if(key == szINI::KEY_CUR_LINE_COLOUR)
                    return QColor(236, 236, 245);
                else if(key == szINI::KEY_HIGHLIGHT_COLOUR)
                    return QColor(Qt::cyan);
            }
        }
    }

    // Unknown combination of group and name? Return an invalid QColor!
    return QColor();
}

void Settings::clearValue(const std::string& section, const std::string& key)
{
    setSettingsObject();
    settings->beginGroup(QString::fromStdString(section));
    settings->remove(QString::fromStdString(key));
    settings->endGroup();
    m_hCache.clear();
}

void Settings::restoreDefaults ()
{
    setSettingsObject();
    settings->clear();
    m_hCache.clear();
}

void Settings::exportSettings(const QString& fileName)
{
    QSettings exportSettings(fileName, QSettings::IniFormat);

    const QStringList groups = settings->childGroups();
    for(const QString& currentGroup : groups)
    {
        settings->beginGroup(currentGroup);
        const QStringList keys = settings->childKeys();
        for(const QString& currentKey : keys)
        {
            exportSettings.beginGroup(currentGroup);
            exportSettings.setValue(currentKey, getValue((currentGroup.toStdString()), (currentKey.toStdString())));
            exportSettings.endGroup();
        }
        settings->endGroup();
    }
}

bool Settings::importSettings(const QString& fileName)
{
    if(!isValidSettingsFile(fileName))
        return false;

    QSettings importSettings(fileName, QSettings::IniFormat);

    const QStringList groups = importSettings.childGroups();
    for(const QString& currentGroup : groups)
    {
        importSettings.beginGroup(currentGroup);
        const QStringList keys = importSettings.childKeys();
        for(const QString& currentKey : keys)
        {
            settings->beginGroup(currentGroup);
            settings->setValue(currentKey, importSettings.value(currentKey));
            settings->endGroup();
        }
        importSettings.endGroup();
    }

    m_hCache.clear();
    return true;
}

void Settings::sync()
{
    settings->sync();
}
