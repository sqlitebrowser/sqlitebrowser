#ifndef SETTINGS_H
#define SETTINGS_H

#include <QApplication>
#include <QVariant>
#include <QHash>

class Settings
{
    friend class PreferencesDialog;
public:
    static QVariant getSettingsValue(const QString& group, const QString& name);
    static void setSettingsValue(const QString& group, const QString& name, const QVariant& value, bool dont_save_to_disk = false);

private:
    Settings() { } // class is fully static

    // This works similar to getSettingsValue but returns the default value instead of the value set by the user
    static QVariant getSettingsDefaultValue(const QString& group, const QString& name);

    // Cache for storing the settings to avoid repeatedly reading the settings file all the time
    static QHash<QString, QVariant> m_hCache;
};

#endif
