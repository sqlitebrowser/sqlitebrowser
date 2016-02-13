#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QVariant>
#include <QHash>

class QTreeWidgetItem;
class QFrame;

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
    static void setSettingsValue(const QString& group, const QString& name, const QVariant& value, bool dont_save_to_disk = false);

private slots:
    virtual void loadSettings();
    virtual void saveSettings();

    virtual void chooseLocation();
    virtual void showColourDialog(QTreeWidgetItem* item, int column);
    virtual void addExtension();
    virtual void removeExtension();

private:
    Ui::PreferencesDialog *ui;

    // This works similar to getSettingsValue but returns the default value instead of the value set by the user
    static QVariant getSettingsDefaultValue(const QString& group, const QString& name);

    // Cache for storing the settings to avoid repeatedly reading the settings file all the time
    static QHash<QString, QVariant> m_hCache;

    void fillLanguageBox();
    void loadColorSetting(QFrame *frame, const QString &name);
    void saveColorSetting(QFrame *frame, const QString &name);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif
