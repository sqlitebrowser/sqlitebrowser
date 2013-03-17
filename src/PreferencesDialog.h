#ifndef __PREFERENCESDIALOG_H__
#define __PREFERENCESDIALOG_H__

#include <QDialog>
#include <QVariant>
#include <QHash>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget* parent = 0);
    ~PreferencesDialog();

    // Use these methods to access the application settings.
    static QVariant getSettingsValue(const QString& group, const QString& name);
    static void setSettingsValue(const QString& group, const QString& name, const QVariant& value);

private slots:
    virtual void chooseLocation();
    virtual void loadSettings();
    virtual void saveSettings();

private:
    Ui::PreferencesDialog *ui;

    // This works similar to getSettingsValue but returns the default value instead of the value set by the user
    static QVariant getSettingsDefaultValue(const QString& group, const QString& name);

    // Cache for storing the settings to avoid repeatedly reading the settings file all the time
    static QHash<QString, QVariant> m_hCache;
};

#endif
