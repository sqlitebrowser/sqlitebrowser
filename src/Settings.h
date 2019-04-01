#ifndef SETTINGS_H
#define SETTINGS_H

#include <QHash>
#include <QVariant>

class Settings
{
    friend class PreferencesDialog;

public:

    enum AppStyle {
        FollowDesktopStyle,
        DarkStyle
    };
    static QVariant getValue(const QString& group, const QString& name);
    static void setValue(const QString& group, const QString& name, const QVariant& value, bool dont_save_to_disk = false);
    static void restoreDefaults();

private:
    Settings() { } // class is fully static

    // This works similar to getValue but returns the default value instead of the value set by the user
    static QVariant getDefaultValue(const QString& group, const QString& name);

    // This works similar to getDefaultValue but returns the default color value based on the passed application style
    // instead of the current palette.
    static QColor getDefaultColorValue(const QString& group, const QString& name, AppStyle style);

    // Cache for storing the settings to avoid repeatedly reading the settings file all the time
    static QHash<QString, QVariant> m_hCache;
};

#endif
