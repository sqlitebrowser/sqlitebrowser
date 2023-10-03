#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <QFontDatabase>
#include <QLocale>
#include <QSettings>
#include <QVariant>

// ref: https://www.techiedelight.com/use-std-pair-key-std-unordered_map-cpp/
typedef std::pair<std::string, std::string> SectionKeyPair;

struct pair_hash
{
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

struct pair_compare
{
    template<class T1, class T2>
    bool operator()(const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) const
    {
        if (lhs.first == rhs.first)
            return lhs.second < rhs.second;
        else
            return lhs.first < rhs.first;
    }
};

namespace szINI
{
    // Settings identification strings constants
    // SEC_ -> INI Section
    // KEY_ -> INI Key name in Section
    // strings used in multiple places
    const std::string KEY_FONT = "font";
    const std::string KEY_FONTSIZE = "fontsize";
    const std::string KEY_FIRST_ROW_HEADER = "firstrowheader";
    const std::string KEY_SEPARATOR = "separator";
    const std::string KEY_QUOTE_CHARACTER = "quotecharacter";

    // Group - checkversion
    const std::string SEC_CHCK_VERSION = "checkversion";
    const std::string KEY_ENABLED = "enabled";
    const std::string KEY_IGNMAJOR = "ignmajor";
    const std::string KEY_IGNMINOR = "ignminor";
    const std::string KEY_IGNPATCH = "ignpatch";

    // Group - databrowser
    // includes KEY_FONT
    const std::string SEC_DATA_BROWSER = "databrowser";
    const std::string KEY_AUTO_SWITCH_MODE = "auto_switch_mode";
    const std::string KEY_BIN_BG_COLOUR = "bin_bg_colour";
    const std::string KEY_BIN_FG_COLOUR = "bin_fg_colour";
    const std::string KEY_BLOB_TEXT = "blob_text";
    const std::string KEY_COMPLETE_THRESHOLD = "complete_threshold";
    const std::string KEY_EDITOR_WORD_WRAP = "editor_word_wrap";
    const std::string KEY_FILTER_DELAY = "filter_delay";
    const std::string KEY_FILTER_ESCAPE = "filter_escape";
    const std::string KEY_FMT_BG_COLOUR = "formatted_bg_colour";
    const std::string KEY_FMT_FG_COLOUR = "formatted_fg_colour";
    const std::string KEY_IMAGE_PREVIEW = "image_preview";
    const std::string KEY_INDENT_COMPACT = "indent_compact";
    const std::string KEY_NULL_BG_COLOUR = "null_bg_colour";
    const std::string KEY_NULL_FG_COLOUR = "null_fg_colour";
    const std::string KEY_NULL_TEXT = "null_text";
    const std::string KEY_REG_BG_COLOUR = "reg_bg_colour";
    const std::string KEY_REG_FG_COLOUR = "reg_fg_colour";
    const std::string KEY_ROWS_LIMIT = "rows_limit";
    const std::string KEY_SYMBOL_LIMIT = "symbol_limit";

    // Group - db
    // includes KEY_FONT, KEY_FONTSIZE
    const std::string SEC_DATABASE = "db";
    const std::string KEY_DEFAULT_ENCODING = "defaultencoding";
    const std::string KEY_DEFAULT_FIELD_TYPE = "defaultfieldtype";
    const std::string KEY_DEFAULT_LOCATION = "defaultlocation";
    const std::string KEY_DEFAULT_SQL_TEXT = "defaultsqltext";
    const std::string KEY_DONT_ASK_COLLATION = "dont_ask_collation";
    const std::string KEY_FOREIGN_KEYS = "foreignkeys";
    const std::string KEY_HIDE_SCHEMA_LINEBREAKS = "hideschemalinebreaks";
    const std::string KEY_LAST_LOCATION = "lastlocation";
    const std::string KEY_PREFETCH_SIZE = "prefetchsize";
    const std::string KEY_SAVE_DEFAULT_LOCATION = "savedefaultlocation";

    // Group - idontcare (pre v3.12 project setting preference)
    const std::string SEC_DONT_CARE = "idontcare";
    const std::string KEY_PROJECT_BROWSE_TABLE = "projectBrowseTable";

    // Group - editor
    // includes KEY_FONT, KEY_FONTSIZE
    const std::string SEC_EDITOR = "editor";
    const std::string KEY_AUTO_COMPLETION = "auto_completion";
    const std::string KEY_CLOSE_BUTTON_ON_TABS = "close_button_on_tabs";
    const std::string KEY_ERROR_INDICATORS = "error_indicators";
    const std::string KEY_HORZ_TILING = "horizontal_tiling";
    const std::string KEY_IDENTIFIER_QUOTES = "identifier_quotes";
    const std::string KEY_INDENTATION_USE_TABS = "indentation_use_tabs";
    const std::string KEY_SPLITTER1_SIZES = "splitter1_sizes";
    const std::string KEY_SPLITTER2_SIZES = "splitter2_sizes";
    const std::string KEY_TABSIZE = "tabsize";
    const std::string KEY_UPPER_KEYWORDS = "upper_keywords";
    const std::string KEY_WRAP_LINES = "wrap_lines";

    // Group - exportcsv
    // includes KEY_FIRST_ROW_HEADER, KEY_QUOTE_CHARACTER, KEY_SEPARATOR
    const std::string SEC_EXPORT_CSV = "exportcsv";
    const std::string KEY_NEWLINE_CHARACTERS = "newlinecharacters";

    // Group - exportjson
    const std::string SEC_EXPORT_JSON = "exportjson";
    const std::string KEY_PRETTY_PRINT = "prettyprint";

    // Group - exportsql
    const std::string SEC_EXPORT_SQL = "exportsql";
    const std::string KEY_INSERT_COL_NAMES = "insertcolnames";
    const std::string KEY_INSERT_MULTIPLE = "insertmultiple";
    const std::string KEY_KEEP_ORIGINAL = "keeporiginal";
    const std::string KEY_OLD_SCHEMA = "oldschema";

    // Group - Extensions
    const std::string SEC_EXTENSIONS = "extensions";
    const std::string KEY_DISABLE_REGEX = "disableregex";
    const std::string KEY_ENABLE_LOAD_EXTENSION = "enable_load_extension";
    const std::string KEY_LIST = "list";

    // Group - General
    // includes KEY_FONTSIZE
    const std::string SEC_GENERAL = "General";
    const std::string KEY_APPSTYLE = "appStyle";
    const std::string KEY_DB_FILE_EXTS = "DBFileExtensions";
    const std::string KEY_LANGUAGE = "language";
    const std::string KEY_MAX_RECENT_FILES = "maxRecentFiles";
    const std::string KEY_PROMPT_SQL_TABS_IN_NEWPRJ = "promptsqltabsinnewproject";
    const std::string KEY_RECENT_FILE_LIST = "recentFileList";
    const std::string KEY_TB_STYLE_APP = "toolbarStyle";
    const std::string KEY_TB_STYLE_BROWSE = "toolbarStyleBrowse";
    const std::string KEY_TB_STYLE_EDIT_CELL = "toolbarStyleEditCell";
    const std::string KEY_TB_STYLE_SQL = "toolbarStyleSql";
    const std::string KEY_TB_STYLE_STRUCTURE = "toolbarStyleStructure";

    // Group - importcsv
    // includes KEY_FIRST_ROW_HEADER, KEY_QUOTE_CHARACTER, KEY_SEPARATOR
    const std::string SEC_IMPORT_CSV = "importcsv";
    const std::string KEY_ENCODING = "encoding";
    const std::string KEY_LOCAL_CONVENTIONS = "localconventions";
    const std::string KEY_SEPARATE_TABLES = "separatetables";
    const std::string KEY_TRIM_FIELDS = "trimfields";

    // Group - log
    // includes KEY_FONT, KEY_FONT_SIZE
    const std::string SEC_LOG = "log";

    // Group - MainWindow
    const std::string SEC_MAIN_WNDW = "MainWindow";
    const std::string KEY_GEOMETRY = "geometry";
    const std::string KEY_OPEN_TABS = "openTabs";
    const std::string KEY_WINDOW_STATE = "windowState";

    // Group - PlotDock
    const std::string SEC_PLOT_DOCK = "PlotDock";
    const std::string KEY_LINE_TYPE = "lineType";
    const std::string KEY_POINT_SHAPE = "pointShape";
    const std::string KEY_SPLITTER_SIZE = "splitterSize";

    // Group - proxy
    const std::string SEC_PROXY = "proxy";
    const std::string KEY_AUTHENTICATION = "authentication";
    const std::string KEY_HOST = "host";
    const std::string KEY_PASSWORD = "password";
    const std::string KEY_PORT = "port";
    const std::string KEY_TYPE = "type";
    const std::string KEY_USER = "user";

    // Group - remote
    const std::string SEC_REMOTE = "remote";
    const std::string KEY_ACTIVE = "active";
    const std::string KEY_CLIENT_CERT = "client_certificates";
    const std::string KEY_CLONE_DIR = "clonedirectory";

    // Group - SchemaDock
    const std::string SEC_SCHEMA_DOCK = "SchemaDock";
    const std::string KEY_DROP_ENQUOTED_NAMES = "dropEnquotedNames";
    const std::string KEY_DROP_QUALIFIED_NAMES = "dropQualifiedNames";
    const std::string KEY_DROP_SELECT_QUERY = "dropSelectQuery";

    // Group - SQLLogDock
    const std::string SEC_SQL_LOG_DOCK = "SQLLogDock";
    const std::string KEY_LOG = "Log";

    // Group - syntaxhighlighter
    const std::string SEC_SYNTAX_HIGHLIGHTER = "syntaxhighlighter";
    // postfixes for bulk setting of attributes
    const std::string KEY_ANY_BOLD = "_bold";
    const std::string KEY_ANY_COLOUR = "_colour";
    const std::string KEY_ANY_ITALIC = "_italic";
    const std::string KEY_ANY_UNDERLINE = "_underline";
    // prefixes for bulk setting of attributes
    const std::string KEY_SHP_NULL_BG = "null_bg";
    const std::string KEY_SHP_NULL_FG = "null_fg";
    const std::string KEY_SHP_REG_BG = "reg_bg";
    const std::string KEY_SHP_REG_FG = "reg_fg";
    const std::string KEY_SHP_FMT_BG = "formatted_bg";
    const std::string KEY_SHP_FMT_FG = "formatted_fg";
    const std::string KEY_SHP_BIN_BG = "bin_bg";
    const std::string KEY_SHP_BIN_FG = "bin_fg";
    // specific syntax highlighter attributes
    const std::string KEY_BG_BOLD = "background_bold";
    const std::string KEY_BG_COLOUR = "background_colour";
    const std::string KEY_BG_ITALIC = "background_italic";
    const std::string KEY_BG_UNDERLINE = "background_underline";
    const std::string KEY_COMMENT_BOLD = "comment_bold";
    const std::string KEY_COMMENT_COLOUR = "comment_colour";
    const std::string KEY_COMMENT_ITALIC = "comment_italic";
    const std::string KEY_COMMENT_UNDERLINE = "comment_underline";
    const std::string KEY_CUR_LINE_BOLD = "currentline_bold";
    const std::string KEY_CUR_LINE_COLOUR = "currentline_colour";
    const std::string KEY_CUR_LINE_ITALIC = "currentline_italic";
    const std::string KEY_CUR_LINE_UNDERLINE = "currentline_underline";
    const std::string KEY_FG_BOLD = "foreground_bold";
    const std::string KEY_FG_COLOUR = "foreground_colour";
    const std::string KEY_FG_ITALIC = "foreground_italic";
    const std::string KEY_FG_UNDERLINE = "foreground_underline";
    const std::string KEY_FUNC_BOLD = "function_bold";
    const std::string KEY_FUNC_COLOUR = "function_colour";
    const std::string KEY_FUNC_ITALIC = "function_italic";
    const std::string KEY_FUNC_UNDERLINE = "function_underline";
    const std::string KEY_HIGHLIGHT_BOLD = "highlight_bold";
    const std::string KEY_HIGHLIGHT_COLOUR = "highlight_colour";
    const std::string KEY_HIGHLIGHT_ITALIC = "highlight_italic";
    const std::string KEY_HIGHLIGHT_UNDERLINE = "highlight_underline";
    const std::string KEY_IDENTIFIER_BOLD = "identifier_bold";
    const std::string KEY_IDENTIFIER_COLOUR = "identifier_colour";
    const std::string KEY_IDENTIFIER_ITALIC = "identifier_italic";
    const std::string KEY_IDENTIFIER_UNDERLINE = "identifier_underline";
    const std::string KEY_KEYWORD_BOLD = "keyword_bold";
    const std::string KEY_KEYWORD_COLOUR = "keyword_colour";
    const std::string KEY_KEYWORD_ITALIC = "keyword_italic";
    const std::string KEY_KEYWORD_UNDERLINE = "keyword_underline";
    const std::string KEY_SELECTED_BG_BOLD = "selected_bg_bold";
    const std::string KEY_SELECTED_BG_COLOUR = "selected_bg_colour";
    const std::string KEY_SELECTED_BG_ITALIC = "selected_bg_italic";
    const std::string KEY_SELECTED_BG_UNDERLINE = "selected_bg_underline";
    const std::string KEY_SELECTED_FG_BOLD = "selected_fg_bold";
    const std::string KEY_SELECTED_FG_COLOUR = "selected_fg_colour";
    const std::string KEY_SELECTED_FG_ITALIC = "selected_fg_italic";
    const std::string KEY_SELECTED_FG_UNDERLINE = "selected_fg_underline";
    const std::string KEY_STRING_BOLD = "string_bold";
    const std::string KEY_STRING_COLOUR = "string_colour";
    const std::string KEY_STRING_ITALIC = "string_italic";
    const std::string KEY_STRING_UNDERLINE = "string_underline";
    const std::string KEY_TABLE_BOLD = "table_bold";
    const std::string KEY_TABLE_COLOUR = "table_colour";
    const std::string KEY_TABLE_ITALIC = "table_italic";
    const std::string KEY_TABLE_UNDERLINE = "table_underline";
}

class Settings
{
    friend class PreferencesDialog;

public:
    enum AppStyle {
        FollowDesktopStyle,
        DarkStyle,
        LightStyle
    };

    // Initialize QSettings object
    static void setSettingsObject();

    static void setUserSettingsFile(const QString& settings_file_path);
    static QVariant getValue(const std::string& section, const std::string& key);
    static void setValue(
            const std::string& section,
            const std::string& key,
            const QVariant& value,
            bool save_to_disk = true
    );
    static void clearValue(const std::string& section, const std::string& key);
    static void restoreDefaults();

    static void exportSettings(const QString& fileName);
    static bool importSettings(const QString& fileName);
    static void sync();
    static void debug_cache();
    static void debug_default();

private:
    // class is fully static
    Settings() = delete;        // NOLINT - clang-tidy wants deleted member functions to be public

    // Verify that the settings file identified by its path is a normal settings file
    static bool isValidSettingsFile(const QString& settings_file_path);

    // Return the stored default value instead of the value set by the user
    static QVariant getDefaultValue(const std::string& section, const std::string& key);

    // Return the default color value based on the passed application style instead of the current palette.
    static QColor getDefaultColourValue(const std::string& section, const std::string& key, AppStyle style);

    // Search settings cache for section/key. Return true if found and set by-reference variable to found value
    static bool fetchCacheValue(const std::string& section, const std::string& key, QVariant* foundData);

    // Search settings file for section/key. Return true if found and set by-reference variable to found value
    static bool fetchFileValue(const std::string& section, const std::string& key, QVariant* foundData);

    // Search settings defaults for section/key. Return true if found and set by-reference variable to found value
    static bool fetchDefaultValue(const std::string& section, const std::string& key, QVariant* foundData);

    // Utility functions to obtain QFont writing system from user's locale
    static QFontDatabase::WritingSystem writingSystemFromScript(QLocale::Script script);
    static QFontDatabase::WritingSystem writingSystemFromLocale();

    // Utility function to determine the fixed font and default value
    static void processDefaultFixedFont();

    // Utility function to establish default colour values
    static void processDefaultColours();

    // Utility function for examining setting key strings
    static bool ends_with(const std::string& str, const std::string& with);

    // Application Default Font Point Size
    static int DEFAULT_FONT_SIZE;

    // User settings file path
    static QString userSettingsFile;

    // QSettings object
    static QSettings* settings;

    // Cache for storing the settings to avoid repeatedly reading the settings file all the time
    static std::unordered_map<SectionKeyPair, QVariant, pair_hash> m_hCache;

    // Default Values storage
    static std::unordered_map<SectionKeyPair, QVariant, pair_hash> m_hDefault;
};

#endif
