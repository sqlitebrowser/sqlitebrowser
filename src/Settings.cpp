#include "Settings.h"

#include <iomanip>
#include <array>
#include <sstream>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QColor>
#include <QStandardPaths>
#include <QPalette>

// constant font point size applicable to:
// general , editor, and log font settings
#ifdef Q_OS_MAC
    int Settings::DEFAULT_FONT_SIZE{12};
#else
    int Settings::DEFAULT_FONT_SIZE{9};
#endif
// Define platform EOL
#ifdef Q_OS_WIN
    const QString DEFAULT_EOL{"\r\n"};
#else
    const QString DEFAULT_EOL{"\n"};
#endif

QString Settings::userSettingsFile;
QSettings* Settings::settings;
// Initialize Settings Cache
std::unordered_map<SectionKeyPair, QVariant, pair_hash> Settings::m_hCache{};
// Populate Default Values Storage
// Any Appearance/Syntax formatting is defaulted to "FollowDesktopStyle" values
// Keys referencing colour settings established in processDefaultColours function
std::unordered_map<SectionKeyPair, QVariant, pair_hash> Settings::m_hDefault =
{
    {{szINI::SEC_CHCK_VERSION, szINI::KEY_ENABLED}, QVariant(true)},
    {{szINI::SEC_CHCK_VERSION, szINI::KEY_IGNMAJOR}, QVariant(999)},
    {{szINI::SEC_CHCK_VERSION, szINI::KEY_IGNMINOR}, QVariant(0)},
    {{szINI::SEC_CHCK_VERSION, szINI::KEY_IGNPATCH}, QVariant(0)},

    {{szINI::SEC_DATA_BROWSER, szINI::KEY_AUTO_SWITCH_MODE}, QVariant(true)},
    {{szINI::SEC_DATA_BROWSER, szINI::KEY_BLOB_TEXT}, QVariant("BLOB")},
    {{szINI::SEC_DATA_BROWSER, szINI::KEY_COMPLETE_THRESHOLD}, QVariant(1000)},
    {{szINI::SEC_DATA_BROWSER, szINI::KEY_EDITOR_WORD_WRAP}, QVariant(true)},
    {{szINI::SEC_DATA_BROWSER, szINI::KEY_FILTER_DELAY}, QVariant(200)},
    {{szINI::SEC_DATA_BROWSER, szINI::KEY_FILTER_ESCAPE}, QVariant("\\")},
    {{szINI::SEC_DATA_BROWSER, szINI::KEY_FONT }, QVariant()},                  // see processDefaultFixedFont()
    {{szINI::SEC_DATA_BROWSER, szINI::KEY_FONTSIZE}, QVariant(10)},
    {{szINI::SEC_DATA_BROWSER, szINI::KEY_IMAGE_PREVIEW}, QVariant(false)},
    {{szINI::SEC_DATA_BROWSER, szINI::KEY_INDENT_COMPACT}, QVariant(false)},
    {{szINI::SEC_DATA_BROWSER, szINI::KEY_NULL_TEXT}, QVariant("NULL")},
    {{szINI::SEC_DATA_BROWSER, szINI::KEY_SYMBOL_LIMIT}, QVariant(5000)},
    {{szINI::SEC_DATA_BROWSER, szINI::KEY_ROWS_LIMIT}, QVariant(10'000'000)},

    {{szINI::SEC_DATABASE, szINI::KEY_DEFAULT_ENCODING}, QVariant("UTF-8")},
    {{szINI::SEC_DATABASE, szINI::KEY_DEFAULT_FIELD_TYPE}, QVariant(0)},
    {{szINI::SEC_DATABASE, szINI::KEY_DEFAULT_LOCATION}, QVariant(QDir::homePath())},
    {{szINI::SEC_DATABASE, szINI::KEY_DEFAULT_SQL_TEXT}, QVariant(QString())},
    {{szINI::SEC_DATABASE, szINI::KEY_DONT_ASK_COLLATION}, QVariant(false)},
    {{szINI::SEC_DATABASE, szINI::KEY_FONT }, QVariant()},                      // see processDefaultFixedFont()
    {{szINI::SEC_DATABASE, szINI::KEY_FONTSIZE}, QVariant(10)},
    {{szINI::SEC_DATABASE, szINI::KEY_FOREIGN_KEYS}, QVariant(true)},
    {{szINI::SEC_DATABASE, szINI::KEY_HIDE_SCHEMA_LINEBREAKS}, QVariant(true)},
    {{szINI::SEC_DATABASE, szINI::KEY_LAST_LOCATION}, QVariant(QDir::homePath())},
    {{szINI::SEC_DATABASE, szINI::KEY_PREFETCH_SIZE}, QVariant(50000U)},
    {{szINI::SEC_DATABASE, szINI::KEY_SAVE_DEFAULT_LOCATION}, QVariant(2)},     // Remember last location for session only

    {{szINI::SEC_DONT_CARE, szINI::KEY_PROJECT_BROWSE_TABLE}, QVariant(false)},

    {{szINI::SEC_EDITOR, szINI::KEY_AUTO_COMPLETION}, QVariant(true)},
    {{szINI::SEC_EDITOR, szINI::KEY_CLOSE_BUTTON_ON_TABS}, QVariant(true)},
    {{szINI::SEC_EDITOR, szINI::KEY_ERROR_INDICATORS}, QVariant(true)},
    {{szINI::SEC_EDITOR, szINI::KEY_FONT}, QVariant()},                         // see processDefaultFixedFont()
    {{szINI::SEC_EDITOR, szINI::KEY_FONTSIZE}, QVariant(Settings::DEFAULT_FONT_SIZE)},
    {{szINI::SEC_EDITOR, szINI::KEY_HORZ_TILING}, QVariant(false)},
    {{szINI::SEC_EDITOR, szINI::KEY_IDENTIFIER_QUOTES}, QVariant(0)},           // sqlb::DoubleQuotes
    {{szINI::SEC_EDITOR, szINI::KEY_INDENTATION_USE_TABS}, QVariant(true)},
    {{szINI::SEC_EDITOR, szINI::KEY_SPLITTER1_SIZES}, QVariant()},
    {{szINI::SEC_EDITOR, szINI::KEY_SPLITTER2_SIZES}, QVariant()},
    {{szINI::SEC_EDITOR, szINI::KEY_TABSIZE}, QVariant(4)},
    {{szINI::SEC_EDITOR, szINI::KEY_UPPER_KEYWORDS}, QVariant(true)},
    {{szINI::SEC_EDITOR, szINI::KEY_WRAP_LINES}, QVariant(0)},                  // QsciScintilla::WrapNone

    {{szINI::SEC_EXPORT_CSV, szINI::KEY_FIRST_ROW_HEADER}, QVariant(true)},
    {{szINI::SEC_EXPORT_CSV, szINI::KEY_NEWLINE_CHARACTERS }, QVariant(DEFAULT_EOL)},
    {{szINI::SEC_EXPORT_CSV, szINI::KEY_QUOTE_CHARACTER}, QVariant("\"")},
    {{szINI::SEC_EXPORT_CSV, szINI::KEY_SEPARATOR}, QVariant(",")},

    {{szINI::SEC_EXPORT_JSON, szINI::KEY_PRETTY_PRINT}, QVariant(true)},

    {{szINI::SEC_EXPORT_SQL, szINI::KEY_INSERT_COL_NAMES}, QVariant(false)},
    {{szINI::SEC_EXPORT_SQL, szINI::KEY_INSERT_MULTIPLE}, QVariant(false)},
    {{szINI::SEC_EXPORT_SQL, szINI::KEY_KEEP_ORIGINAL}, QVariant(false)},
    {{szINI::SEC_EXPORT_SQL, szINI::KEY_OLD_SCHEMA}, QVariant(0)},

    {{szINI::SEC_EXTENSIONS, szINI::KEY_DISABLE_REGEX}, QVariant(false)},
    {{szINI::SEC_EXTENSIONS, szINI::KEY_ENABLE_LOAD_EXTENSION}, QVariant(false)},
    {{szINI::SEC_EXTENSIONS, szINI::KEY_LIST}, QVariant(QStringList())},

    {{szINI::SEC_GENERAL, szINI::KEY_APPSTYLE}, QVariant(static_cast<int>(Settings::FollowDesktopStyle))},
    {{szINI::SEC_GENERAL, szINI::KEY_DB_FILE_EXTS},
        QVariant(QObject::tr("SQLite database files (*.db *.sqlite *.sqlite3 *.db3)"))},
    {{szINI::SEC_GENERAL, szINI::KEY_FONTSIZE }, QVariant(Settings::DEFAULT_FONT_SIZE)},
    {{szINI::SEC_GENERAL, szINI::KEY_LANGUAGE}, QVariant(QLocale::system().name())},
    {{szINI::SEC_GENERAL, szINI::KEY_MAX_RECENT_FILES}, QVariant(5)},
    {{szINI::SEC_GENERAL, szINI::KEY_PROMPT_SQL_TABS_IN_NEWPRJ}, QVariant(true)},
    {{szINI::SEC_GENERAL, szINI::KEY_RECENT_FILE_LIST}, QVariant(QStringList())},
    {{szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_APP}, QVariant(static_cast<int>(Qt::ToolButtonTextBesideIcon))},
    {{szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_BROWSE}, QVariant(static_cast<int>(Qt::ToolButtonIconOnly))},
    {{szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_EDIT_CELL}, QVariant(static_cast<int>(Qt::ToolButtonIconOnly))},
    {{szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_SQL}, QVariant(static_cast<int>(Qt::ToolButtonIconOnly))},
    {{szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_STRUCTURE}, QVariant(static_cast<int>(Qt::ToolButtonTextBesideIcon))},

    {{szINI::SEC_IMPORT_CSV, szINI::KEY_ENCODING}, QVariant("UTF-8")},
    {{szINI::SEC_IMPORT_CSV, szINI::KEY_FIRST_ROW_HEADER}, QVariant(false)},
    {{szINI::SEC_IMPORT_CSV, szINI::KEY_LOCAL_CONVENTIONS}, QVariant(false)},
    {{szINI::SEC_IMPORT_CSV, szINI::KEY_QUOTE_CHARACTER}, QVariant("\"")},
    {{szINI::SEC_IMPORT_CSV, szINI::KEY_SEPARATE_TABLES}, QVariant(false)},
    {{szINI::SEC_IMPORT_CSV, szINI::KEY_SEPARATOR}, QVariant(",")},
    {{szINI::SEC_IMPORT_CSV, szINI::KEY_TRIM_FIELDS}, QVariant(true)},

    {{szINI::SEC_LOG, szINI::KEY_FONT}, QVariant()},                            // see processDefaultFixedFont()
    {{szINI::SEC_LOG, szINI::KEY_FONTSIZE}, QVariant(Settings::DEFAULT_FONT_SIZE)},

    {{szINI::SEC_MAIN_WNDW, szINI::KEY_GEOMETRY}, QVariant(QString())},
    {{szINI::SEC_MAIN_WNDW, szINI::KEY_OPEN_TABS}, QVariant(QString())},
    {{szINI::SEC_MAIN_WNDW, szINI::KEY_WINDOW_STATE}, QVariant(QString())},

    {{szINI::SEC_PLOT_DOCK, szINI::KEY_LINE_TYPE}, QVariant(1)},                // QCPGraph::lsLine
    {{szINI::SEC_PLOT_DOCK, szINI::KEY_POINT_SHAPE}, QVariant(4)},              // QCPScatterStyle::ssDisk
    {{szINI::SEC_PLOT_DOCK, szINI::KEY_SPLITTER_SIZE}, QVariant(QString())},

    {{szINI::SEC_PROXY, szINI::KEY_AUTHENTICATION}, QVariant(false)},
    {{szINI::SEC_PROXY, szINI::KEY_HOST}, QVariant(QString())},
    {{szINI::SEC_PROXY, szINI::KEY_PASSWORD}, QVariant(QString())},
    {{szINI::SEC_PROXY, szINI::KEY_PORT}, QVariant(0)},
    {{szINI::SEC_PROXY, szINI::KEY_TYPE}, QVariant("system")},
    {{szINI::SEC_PROXY, szINI::KEY_USER}, QVariant(QString())},

    {{szINI::SEC_REMOTE, szINI::KEY_ACTIVE}, QVariant(true)},
    {{szINI::SEC_REMOTE, szINI::KEY_CLIENT_CERT}, QVariant(QStringList())},
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    {{szINI::SEC_REMOTE, szINI::KEY_CLONE_DIR},
        QVariant(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))},
#else
    {{szINI::SEC_REMOTE, szINI::KEY_CLONE_DIR},
        QVariant(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation))},
#endif
    {{szINI::SEC_SCHEMA_DOCK, szINI::KEY_DROP_ENQUOTED_NAMES}, QVariant(true)},
    {{szINI::SEC_SCHEMA_DOCK, szINI::KEY_DROP_QUALIFIED_NAMES}, QVariant(false)},
    {{szINI::SEC_SCHEMA_DOCK, szINI::KEY_DROP_SELECT_QUERY}, QVariant(true)},

    {{szINI::SEC_SQL_LOG_DOCK, szINI::KEY_LOG}, QVariant("Application")},

    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_BG_BOLD}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_BG_ITALIC}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_BG_UNDERLINE}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_COMMENT_BOLD}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_COMMENT_ITALIC}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_COMMENT_UNDERLINE}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_CUR_LINE_BOLD}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_CUR_LINE_ITALIC}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_CUR_LINE_UNDERLINE}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_FG_BOLD}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_FG_ITALIC}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_FG_UNDERLINE}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_FUNC_BOLD}, QVariant(true)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_FUNC_ITALIC}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_FUNC_UNDERLINE}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_HIGHLIGHT_BOLD}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_HIGHLIGHT_ITALIC}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_HIGHLIGHT_UNDERLINE}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_IDENTIFIER_BOLD}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_IDENTIFIER_ITALIC}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_IDENTIFIER_UNDERLINE}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_KEYWORD_BOLD}, QVariant(true)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_KEYWORD_ITALIC}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_KEYWORD_UNDERLINE}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_SELECTED_BG_BOLD}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_SELECTED_BG_ITALIC}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_SELECTED_BG_UNDERLINE}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_SELECTED_FG_BOLD}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_SELECTED_FG_ITALIC}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_SELECTED_FG_UNDERLINE}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_STRING_BOLD}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_STRING_ITALIC}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_STRING_UNDERLINE}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_TABLE_BOLD}, QVariant(true)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_TABLE_ITALIC}, QVariant(false)},
    {{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_TABLE_UNDERLINE}, QVariant(false)},
};

// Deprecated section/key identities that may still exist in a user's settings file.
// On initialization, old section/key [first pair] will be translated to new section/key[second pair].
std::map<SectionKeyPair, SectionKeyPair, pair_compare> Settings::m_hDeprecated =
{
    // Add new items to this container as --> {{old section, old key}, {new section, new key}},
    //
    // 2023Aug - misspelling of key value persistent previously in project sources
    {{szINI::SEC_DATABASE, "lastlocations"}, {szINI::SEC_DATABASE, szINI::KEY_LAST_LOCATION}},
};

// Section/Key identities that may still exist in user's settings file that
// DO NOT HAVE a replacement. Each entry is a section/key pair to be removed
// from settings file.
std::vector<SectionKeyPair> Settings::m_hDeleted =
{
    // Add new items to this container as --> {{old section, old key}},
    // preference is using string literals, not variables or constants, for items to be removed.
};

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
            if(settings->status() == QSettings::AccessError)
            {
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
    processDeprecatedSettings();
    processDeletedSettings();
    processDefaultFixedFont();
    processDefaultColours();
}

void Settings::setUserSettingsFile(const QString& settings_file_path)
{
    userSettingsFile = settings_file_path;
}

QVariant Settings::getValue(const std::string& section, const std::string& key)
{
    auto qValue = new QVariant();
    if (section.empty() || key.empty())
    {
        qDebug() << "Empty Arguments(getValue): "
                 << QString::fromStdString(section)
                 << "\t"
                 << QString::fromStdString(key);
        return *qValue;
    }
    if (fetchCacheValue(section, key, qValue))
    {
        // return value found in Cached Settings
        return *qValue;
    }
    if (fetchFileValue(section, key, qValue))
    {
        // Store value found in settings file in the cache and return value
        m_hCache.insert({{section, key}, *qValue});
        return *qValue;
    }
    if (fetchDefaultValue(section, key, qValue))
    {
        // Store value found in Default in the cache
        m_hCache.insert({{section, key}, *qValue});
        // return the default value
        return *qValue;
    }
    // Search not in Cache, File, nor Default
    // Log the Section/Key to the console
    qWarning().nospace() << "=======================================================\n"
                         << "Value for: "
                         << QString::fromStdString(section)
                         << " | "
                         << QString::fromStdString(key)
                         << " NOT Found!!\n"
                         << "Removing section/key value from settings!!\n"
                         << "=======================================================\n";
    clearValue(section, key);
    // return the empty QVariant object
    return *qValue;
}

void Settings::setValue(const std::string& section, const std::string& key, const QVariant& value, bool save_to_disk)
{
    // Sometimes the value has to be saved for the current session only and gets discarded when the application exits.
    // The save_to_disk flag disables the writing mechanism for the supplied section/key/value arguments.
    if(save_to_disk)
    {
        // Set the group and save the given value
        settings->beginGroup(QString::fromStdString(section));
        settings->setValue(QString::fromStdString(key), value);
        settings->endGroup();
    }
    // Also push value into cache
    m_hCache[{section, key}] = value;
}

void Settings::clearValue(const std::string& section, const std::string& key)
{
    settings->beginGroup(QString::fromStdString(section));
    settings->remove(QString::fromStdString(key));
    settings->endGroup();
    m_hCache.erase({section, key});
}

void Settings::restoreDefaults ()
{
    settings->clear();
    m_hCache.clear();
}

void Settings::exportSettings(const QString& fileName)
{
    QSettings exportSettings(fileName, QSettings::IniFormat);

    const QStringList sections = settings->childGroups();
    for(const QString& currentSection : sections)
    {
        settings->beginGroup(currentSection);
        const QStringList keys = settings->childKeys();
        for(const QString& currentKey : keys)
        {
            exportSettings.beginGroup(currentSection);
            exportSettings.setValue(currentKey, getValue((currentSection.toStdString()), (currentKey.toStdString())));
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

    const QStringList sections = importSettings.childGroups();
    for(const QString& currentSection : sections)
    {
        importSettings.beginGroup(currentSection);
        const QStringList keys = importSettings.childKeys();
        for(const QString& currentKey : keys)
        {
            settings->beginGroup(currentSection);
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

void Settings::debug_cache()
{
    std::cout << "Cache Values" << std::endl
              << "---------------" << std::endl
              << "File: " << userSettingsFile.toStdString() << std::endl
              << "---------------" << std::endl;
    std::stringstream buffer;
    // unordered map sorted when items added into map container
    std::map<SectionKeyPair, QVariant, pair_compare> temp_debug;
    for (const auto& item : m_hCache)
    {
        temp_debug[{item.first.first, item.first.second}] = item.second;
    }
    for (const auto& item : temp_debug)
    {
        buffer << "Section:"
               << std::setw(18) << item.first.first     // SectionKeyPair.section
               << "\tKey:"
               << std::setw(28) << item.first.second    // SectionKeyPair.key
               << "\tValue:["
               << item.second.toString().toStdString()
               << "]"
               << std::endl;
    }
    std::cout << buffer.str() << std::endl;
}

void Settings::debug_default()
{
    std::cout << "Default Values" << std::endl
              << "---------------" << std::endl;
    std::stringstream buffer;
    // unordered map sorted when items added into map container
    std::map<SectionKeyPair, QVariant, pair_compare> temp_debug;
    for (const auto& item : m_hDefault)
    {
        temp_debug[{item.first.first, item.first.second}] = item.second;
    }
    for (const auto& item : temp_debug)
    {
        buffer << "Section:"
               << std::setw(18) << item.first.first     // SectionKeyPair.section
               << "\tKey:"
               << std::setw(28) << item.first.second    // SectionKeyPair.key
               << "\tValue:["
               << item.second.toString().toStdString()
               << "]"
               << std::endl;
    }
    std::cout << buffer.str() << std::endl;
}

bool Settings::isValidSettingsFile(const QString& settings_file_path)
{
    /*
    Variable storing settings file path requested by the user is a normal settings file if
    the file does not exist and is newly created. In this instance, the if statement below
    is not executed. Therefore, the default value is set to true.
    */
    bool isNormalUserSettingsFile = true;

    // Code that verifies that the settings file requested by the user is a normal settings file
    if(settings_file_path != nullptr)
    {
        auto *file = new QFile;
        file->setFileName(settings_file_path);

        if(file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            // define comparison string
            // "[General]\n" --> first line of any DB4S settings file
            QString header("[" + QString::fromStdString(szINI::SEC_GENERAL) + "]\n");
            if(file->exists() &&
               QString::compare(header, file->readLine(), Qt::CaseInsensitive) != 0)
                isNormalUserSettingsFile = false;
        }

        file->close();
    }
    qDebug() << "Settings File: "
             << settings_file_path
             << "\nReturn: "
             << QString(isNormalUserSettingsFile ? "True" : "False");
    return isNormalUserSettingsFile;
}

QVariant Settings::getDefaultValue(const std::string& section, const std::string& key)
{
    auto qValue = new QVariant();
    if (section.empty() || key.empty())
    {
        qDebug() << "Empty Arguments(getDefaultValue): "
                 << QString::fromStdString(section)
                 << "\t"
                 << QString::fromStdString(key);
        return *qValue;
    }
    if (!fetchDefaultValue(section, key, qValue))
    {
        // Log the Section/Key to the console
        qWarning() << "Value for"
                   << QString::fromStdString(section)
                   << "|"
                   << QString::fromStdString(key)
                   << " NOT Found!!";
        // return the empty QVariant object
    }
    return *qValue;
}

QColor Settings::getDefaultColourValue(const std::string& section, const std::string& key, AppStyle style)
{
    // Data Browser/NULL & Binary Fields
    if(section == szINI::SEC_DATA_BROWSER)
    {
        // The switch on style can be removed if the following issue is fixed:
        // https://github.com/ColinDuquesnoy/QDarkStyleSheet/issues/171
        switch (style)
        {
        case FollowDesktopStyle :
            if(key == szINI::KEY_NULL_FG_COLOUR)
                return QColor{Qt::lightGray};
            if(key == szINI::KEY_NULL_BG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::Base);
            if(key == szINI::KEY_REG_FG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::Text);
            if(key == szINI::KEY_REG_BG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::Base);
            if(key == szINI::KEY_FMT_FG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::Text);
            if(key == szINI::KEY_FMT_BG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::AlternateBase);
            if(key == szINI::KEY_BIN_FG_COLOUR)
                return QColor{Qt::lightGray};
            if(key == szINI::KEY_BIN_BG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::Base);
            break;
        case DarkStyle :
            if(key == szINI::KEY_NULL_FG_COLOUR)
                return QColor{0x78, 0x78, 0x78};
            if(key == szINI::KEY_NULL_BG_COLOUR)
                return QColor{0x19, 0x23, 0x2D};
            if(key == szINI::KEY_REG_FG_COLOUR)
                return QColor{0xF0, 0xF0, 0xF0};
            if(key == szINI::KEY_REG_BG_COLOUR)
                return QColor{0x19, 0x23, 0x2D};
            if(key == szINI::KEY_FMT_FG_COLOUR)
                return QColor{0xF0, 0xF0, 0xF0};
            if(key == szINI::KEY_FMT_BG_COLOUR)
                return QColor{0x19, 0x23, 0x2D};
            if(key == szINI::KEY_BIN_FG_COLOUR)
                return QColor{0x78, 0x78, 0x78};
            if(key == szINI::KEY_BIN_BG_COLOUR)
                return QColor{0x19, 0x23, 0x2D};
            break;
        case LightStyle :
            if(key == szINI::KEY_NULL_FG_COLOUR || key == szINI::KEY_BIN_FG_COLOUR)
                return QColor{0xA5, 0xA9, 0xAC};
            if(key == szINI::KEY_NULL_BG_COLOUR || key == szINI::KEY_BIN_BG_COLOUR)
                return QColor{0xFA, 0xFA, 0xFA};
            if(key == szINI::KEY_REG_FG_COLOUR)
                return QColor{0x00, 0x00, 0x00};
            if(key == szINI::KEY_REG_BG_COLOUR)
                return QColor{0xFA, 0xFA, 0xFA};
            if(key == szINI::KEY_FMT_FG_COLOUR)
                return QColor{0x00, 0x00, 0x00};
            if(key == szINI::KEY_FMT_BG_COLOUR)
                return QColor{0xFA, 0xFA, 0xFA};
            break;
        }
    }

    if (section == szINI::SEC_SYNTAX_HIGHLIGHTER)
    {
        if (ends_with(key, szINI::KEY_ANY_COLOUR))
        {
            QColor backgroundColour;
            QColor foregroundColour;

            switch (style)
            {
            case FollowDesktopStyle :
                backgroundColour = QPalette().color(QPalette::Active, QPalette::Base);
                foregroundColour = QPalette().color(QPalette::Active, QPalette::Text);
                break;
            case DarkStyle :
                foregroundColour = QColor{0xF0, 0xF0, 0xF0};
                backgroundColour = QColor{0x19, 0x23, 0x2D};
                break;
            case LightStyle :
                foregroundColour = QColor{0x00, 0x00, 0x00};
                backgroundColour = QColor{0xFA, 0xFA, 0xFA};
                break;
            }
            if(key == szINI::KEY_FG_COLOUR)
                return foregroundColour;
            if(key == szINI::KEY_BG_COLOUR)
                return backgroundColour;
            if(key == szINI::KEY_SELECTED_FG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::HighlightedText);
            if(key == szINI::KEY_SELECTED_BG_COLOUR)
                return QPalette().color(QPalette::Active, QPalette::Highlight);

            // Detect and provide sensible defaults for dark themes
            if (backgroundColour.value() < foregroundColour.value())
            {
                if(key == szINI::KEY_KEYWORD_COLOUR)
                    return QColor{82, 148, 226};
                if(key == szINI::KEY_FUNC_COLOUR)
                    return QColor{Qt::yellow};
                if(key == szINI::KEY_TABLE_COLOUR)
                    return QColor{Qt::cyan};
                if(key == szINI::KEY_COMMENT_COLOUR)
                    return QColor{Qt::green};
                if(key == szINI::KEY_IDENTIFIER_COLOUR)
                    return QColor{221, 160, 221};
                if(key == szINI::KEY_STRING_COLOUR)
                    return QColor{Qt::lightGray};
                if(key == szINI::KEY_CUR_LINE_COLOUR)
                    return backgroundColour.lighter(150);
                if(key == szINI::KEY_HIGHLIGHT_COLOUR)
                    return QColor{79, 148, 205};
            } else {
                if(key == szINI::KEY_KEYWORD_COLOUR)
                    return QColor{Qt::darkBlue};
                if(key == szINI::KEY_FUNC_COLOUR)
                    return QColor{Qt::blue};
                if(key == szINI::KEY_TABLE_COLOUR)
                    return QColor{Qt::darkCyan};
                if(key == szINI::KEY_COMMENT_COLOUR)
                    return QColor{Qt::darkGreen};
                if(key == szINI::KEY_IDENTIFIER_COLOUR)
                    return QColor{Qt::darkMagenta};
                if(key == szINI::KEY_STRING_COLOUR)
                    return QColor{Qt::red};
                if(key == szINI::KEY_CUR_LINE_COLOUR)
                    return QColor{236, 236, 245};
                if(key == szINI::KEY_HIGHLIGHT_COLOUR)
                    return QColor{Qt::cyan};
            }
        }
    }
    // Unknown combination of section and key!
    // Log the missed colour search and return an invalid QColor.
    qWarning() << "No value for GetDefaultColour "
               << QString::fromStdString(section)
               << "|"
               << QString::fromStdString(key);
    return QColor{};
}

bool Settings::fetchCacheValue(const std::string& section, const std::string& key, QVariant* foundData)
{
    // Search settings cache for section/key.
    // IF FOUND, return true and set pointer variable contents to found value
    // ELSE return false - Pointer variable contents are indeterminate.
    // Assumption: section/key assumed to be non-empty, valid strings.
    bool bReturn{false};
    auto cacheIndex = m_hCache.find({section, key});
    if (cacheIndex != m_hCache.end())
    {
        bReturn = true;
        foundData->setValue(cacheIndex->second);
    }
    return bReturn;
}

bool Settings::fetchFileValue(const std::string& section, const std::string& key, QVariant* foundData)
{
    // Search settings file for section/key.
    // IF FOUND, return true and set pointer variable contents to found value
    // ELSE return false - Pointer variable contents are indeterminate.
    // Assumption: section/key assumed to be non-empty, valid strings.
    bool bReturn{false};
    auto str_sect_key = QString::fromStdString(section + "/" + key);
    if (settings->contains(str_sect_key))
    {
        bReturn = true;
        foundData->setValue(settings->value(str_sect_key));
    }
    return bReturn;
}

bool Settings::fetchDefaultValue(const std::string& section, const std::string& key, QVariant* foundData)
{
    // Search settings default for section/key.
    // IF FOUND, return true and set pointer variable contents to found value
    // ELSE return false - Pointer variable contents are indeterminate.
    // Assumption: section/key assumed to be non-empty, valid strings.
    bool bReturn{false};
    auto defaultIndex = m_hDefault.find({section, key});
    if (defaultIndex != m_hDefault.end())
    {
        bReturn = true;
        foundData->setValue(defaultIndex->second);
    }
    return bReturn;
}

void Settings::processDeprecatedSettings()
{
    // Search for each item in deprecated storage in settings file.
    // If found, remove old section/key from settings and
    // establish new section/key (preserving associated value).
    auto qValue = new QVariant();
    for (const auto& search_item: m_hDeprecated)
    {
        const auto& old_sect_key = search_item.first;
        const auto& new_sect_key = search_item.second;
        if (fetchFileValue(old_sect_key.first, old_sect_key.second, qValue))
        {
            clearValue(old_sect_key.first, old_sect_key.second);
            setValue(new_sect_key.first, new_sect_key.second, *qValue);
            qValue = new QVariant();
            qDebug() << "Replaced:"
                     << QString::fromStdString(old_sect_key.first)
                     << "|"
                     << QString::fromStdString(old_sect_key.second)
                     << "\t with "
                     << QString::fromStdString(new_sect_key.first)
                     << "|"
                     << QString::fromStdString(new_sect_key.second);
        }
    }
    // Action completed - storage no longer required.
    m_hDeprecated.clear();
}

void Settings::processDeletedSettings()
{
    // Search for each item in deleted storage in settings file.
    // If found, remove old section/key from settings with value.
    auto qValue = new QVariant();
    for (const auto& search_item: m_hDeleted)
    {
        if (fetchFileValue(search_item.first, search_item.second, qValue))
        {
            clearValue(search_item.first, search_item.second);
            qValue = new QVariant();
            qDebug() << "Removed:"
                     << QString::fromStdString(search_item.first)
                     << "|"
                     << QString::fromStdString(search_item.second);
        }
    }
    // Action completed - storage no longer required.
    m_hDeleted.clear();
}

// Taken from Qt/QtBase/QFontComboBox source file
// ref: https://code.qt.io/cgit/qt/qtbase.git/tree/src/widgets/widgets/qfontcombobox.cpp
// Utilized as part of determination of suitable fixed width font
// Remove after QTBUG-116223 (https://bugreports.qt.io/browse/QTBUG-116223) is resolved.
QFontDatabase::WritingSystem Settings::writingSystemFromScript(QLocale::Script script)
{
    switch (script) {
        case QLocale::ArabicScript:
            return QFontDatabase::Arabic;
        case QLocale::CyrillicScript:
            return QFontDatabase::Cyrillic;
        case QLocale::GurmukhiScript:
            return QFontDatabase::Gurmukhi;
        case QLocale::SimplifiedHanScript:
            return QFontDatabase::SimplifiedChinese;
        case QLocale::TraditionalHanScript:
            return QFontDatabase::TraditionalChinese;
        case QLocale::LatinScript:
            return QFontDatabase::Latin;
        case QLocale::ArmenianScript:
            return QFontDatabase::Armenian;
        case QLocale::BengaliScript:
            return QFontDatabase::Bengali;
        case QLocale::DevanagariScript:
            return QFontDatabase::Devanagari;
        case QLocale::GeorgianScript:
            return QFontDatabase::Georgian;
        case QLocale::GreekScript:
            return QFontDatabase::Greek;
        case QLocale::GujaratiScript:
            return QFontDatabase::Gujarati;
        case QLocale::HebrewScript:
            return QFontDatabase::Hebrew;
        case QLocale::JapaneseScript:
            return QFontDatabase::Japanese;
        case QLocale::KhmerScript:
            return QFontDatabase::Khmer;
        case QLocale::KannadaScript:
            return QFontDatabase::Kannada;
        case QLocale::KoreanScript:
            return QFontDatabase::Korean;
        case QLocale::LaoScript:
            return QFontDatabase::Lao;
        case QLocale::MalayalamScript:
            return QFontDatabase::Malayalam;
        case QLocale::MyanmarScript:
            return QFontDatabase::Myanmar;
        case QLocale::TamilScript:
            return QFontDatabase::Tamil;
        case QLocale::TeluguScript:
            return QFontDatabase::Telugu;
        case QLocale::ThaanaScript:
            return QFontDatabase::Thaana;
        case QLocale::ThaiScript:
            return QFontDatabase::Thai;
        case QLocale::TibetanScript:
            return QFontDatabase::Tibetan;
        case QLocale::SinhalaScript:
            return QFontDatabase::Sinhala;
        case QLocale::SyriacScript:
            return QFontDatabase::Syriac;
        case QLocale::OriyaScript:
            return QFontDatabase::Oriya;
        case QLocale::OghamScript:
            return QFontDatabase::Ogham;
        case QLocale::RunicScript:
            return QFontDatabase::Runic;
        case QLocale::NkoScript:
            return QFontDatabase::Nko;
        default:
            return QFontDatabase::Any;
    }
}

// Taken from Qt/QtBase/QFontComboBox source file
// ref: https://code.qt.io/cgit/qt/qtbase.git/tree/src/widgets/widgets/qfontcombobox.cpp
// Utilized as part of determination of suitable fixed width font
// Remove after QTBUG-116223 (https://bugreports.qt.io/browse/QTBUG-116223) is resolved.
QFontDatabase::WritingSystem Settings::writingSystemFromLocale()
{
    QStringList uiLanguages = QLocale::system().uiLanguages();
    QLocale::Script script;
    if (!uiLanguages.isEmpty())
        script = QLocale(uiLanguages.at(0)).script();
    else
        script = QLocale::system().script();

    return writingSystemFromScript(script);
}

void Settings::processDefaultFixedFont()
{
    QFontDatabase fontDB;
    QString fixed_family(QFontDatabase::systemFont(QFontDatabase::FixedFont).family());
    // Font "SANITY" Check
    // Selected fixed font must be listed in font database
    // AND be a FixedPitch (i.e. fixed width) font.
    // NOTE ASSUMPTION: a fixed pitch font will be found on the system
    QFontDatabase::WritingSystem fontWriteSys = writingSystemFromLocale();
    QStringList check_families(fontDB.families(fontWriteSys));
    bool bCheck_Found = check_families.contains(fixed_family);
    bool bCheck_IsFixed = fontDB.isFixedPitch(fixed_family);
    if (!bCheck_Found || !bCheck_IsFixed)
    {
        // sanity check failed
        // find first available fixed pitched font
        QStringList font_families(fontDB.families(fontWriteSys));
        for (const auto& each_font_name : font_families)
        {
            bool isFixed = fontDB.isFixedPitch(each_font_name);
            qDebug() << "Testing font: "
                     << each_font_name
                     << " fixed?: "
                     << (isFixed ? "True" : "False");
            if (!isFixed)
                continue;
            fixed_family = each_font_name;
            break;
        }
        qWarning().nospace() << "=======================================================\n"
                             << "Unable to obtain default fixed font. Qt Environment\n"
                             << "may be misconfigured. Using first fixed font available\n"
                             << "from system: " << fixed_family << "\n"
                             << "=======================================================\n";
    } else {
        qDebug() << "System Fixed Font: " << fixed_family;
    }
    m_hDefault[{szINI::SEC_DATA_BROWSER, szINI::KEY_FONT }] = QVariant(fixed_family);
    m_hDefault[{szINI::SEC_DATABASE, szINI::KEY_FONT }] = QVariant(fixed_family);
    m_hDefault[{szINI::SEC_EDITOR, szINI::KEY_FONT }] = QVariant(fixed_family);
    m_hDefault[{szINI::SEC_LOG, szINI::KEY_FONT }] = QVariant(fixed_family);
}

void Settings::processDefaultColours()
{
    // using the current application style, establish colour values in default settings storage.
    std::array<SectionKeyPair, 20> colourPairs
    {
        SectionKeyPair{szINI::SEC_DATA_BROWSER, szINI::KEY_BIN_BG_COLOUR},
        SectionKeyPair{szINI::SEC_DATA_BROWSER, szINI::KEY_BIN_FG_COLOUR},
        SectionKeyPair{szINI::SEC_DATA_BROWSER, szINI::KEY_FMT_BG_COLOUR},
        SectionKeyPair{szINI::SEC_DATA_BROWSER, szINI::KEY_FMT_FG_COLOUR},
        SectionKeyPair{szINI::SEC_DATA_BROWSER, szINI::KEY_NULL_BG_COLOUR},
        SectionKeyPair{szINI::SEC_DATA_BROWSER, szINI::KEY_NULL_FG_COLOUR},
        SectionKeyPair{szINI::SEC_DATA_BROWSER, szINI::KEY_REG_BG_COLOUR},
        SectionKeyPair{szINI::SEC_DATA_BROWSER, szINI::KEY_REG_FG_COLOUR},
        SectionKeyPair{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_BG_COLOUR},
        SectionKeyPair{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_COMMENT_COLOUR},
        SectionKeyPair{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_CUR_LINE_COLOUR},
        SectionKeyPair{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_FG_COLOUR},
        SectionKeyPair{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_FUNC_COLOUR},
        SectionKeyPair{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_HIGHLIGHT_COLOUR},
        SectionKeyPair{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_IDENTIFIER_COLOUR},
        SectionKeyPair{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_KEYWORD_COLOUR},
        SectionKeyPair{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_SELECTED_BG_COLOUR},
        SectionKeyPair{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_SELECTED_FG_COLOUR},
        SectionKeyPair{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_STRING_COLOUR},
        SectionKeyPair{szINI::SEC_SYNTAX_HIGHLIGHTER, szINI::KEY_TABLE_COLOUR}
    };

    AppStyle style = static_cast<Settings::AppStyle>(getValue(szINI::SEC_GENERAL, szINI::KEY_APPSTYLE).toInt());
    for (const auto& sec_key : colourPairs)
    {
        const auto& section = sec_key.first;
        const auto& key = sec_key.second;
        m_hDefault[{section, key}] = QVariant(getDefaultColourValue(section, key, style));
    }
}

bool Settings::ends_with(const std::string& str, const std::string& with)
{
    return str.rfind(with) == str.size() - with.size();
}
