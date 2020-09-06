#ifndef SETTINGS_H
#define SETTINGS_H

#include <unordered_map>
#include <QSettings>
#include <QVariant>

class Settings
{
    friend class PreferencesDialog;

public:
    enum AppStyle {
        FollowDesktopStyle,
        DarkStyle
    };

    static void setUserConfigurationFile(const QString userConfigurationFileArg);
    static QVariant getValue(const std::string& group, const std::string& name);
    static void setValue(const std::string& group, const std::string& name, const QVariant& value, bool dont_save_to_disk = false);
    static void clearValue(const std::string& group, const std::string& name);
    static void restoreDefaults();

    static void rememberDefaultFontSize(int size) { m_defaultFontSize = size; }

private:
    Settings() = delete;    // class is fully static

    // This works similar to getValue but returns the default value instead of the value set by the user
    static QVariant getDefaultValue(const std::string& group, const std::string& name);

    // This works similar to getDefaultValue but returns the default color value based on the passed application style
    // instead of the current palette.
    static QColor getDefaultColorValue(const std::string& group, const std::string& name, AppStyle style);

    // User configuration file path
    static QString userConfigurationFile;

    // QSettings object
    static QSettings* settings;

    // This works initialize QSettings object
    static void setSettingsObject();

    // Cache for storing the settings to avoid repeatedly reading the settings file all the time
    static std::unordered_map<std::string, QVariant> m_hCache;

    // Default UI font size
    static int m_defaultFontSize;
};

#endif
