#include "FileDialog.h"
#include "PreferencesDialog.h"

QString FileDialog::getOpenFileName(QWidget* parent, const QString& caption, const QString &filter, QString *selectedFilter, Options options)
{
    QString result = QFileDialog::getOpenFileName(parent, caption, getFileDialogPath(), filter, selectedFilter, options);
    if(!result.isEmpty())
        setFileDialogPath(result);
    return result;
}

QString FileDialog::getSaveFileName(QWidget* parent, const QString& caption, const QString& filter, QString* selectedFilter, Options options)
{
    QString result = QFileDialog::getSaveFileName(parent, caption, getFileDialogPath(), filter, selectedFilter, options);
    if(!result.isEmpty())
        setFileDialogPath(result);
    return result;
}

QString FileDialog::getExistingDirectory(QWidget* parent, const QString& caption, Options options)
{
    QString result = QFileDialog::getExistingDirectory(parent, caption, getFileDialogPath(), options);
    if(!result.isEmpty())
        setFileDialogPath(result);
    return result;
}

QString FileDialog::getFileDialogPath()
{
    switch(PreferencesDialog::getSettingsValue("db", "savedefaultlocation").toInt())
    {
    case 0:     // Remember last location
    case 2:     // Remember last location for current session only
        return PreferencesDialog::getSettingsValue("db", "lastlocation").toString();
    case 1:     // Always use this locations
        return PreferencesDialog::getSettingsValue("db", "defaultlocation").toString();
    default:
        return "";
    }
}

void FileDialog::setFileDialogPath(const QString& new_path)
{
    QString dir = QFileInfo(new_path).absolutePath();

    switch(PreferencesDialog::getSettingsValue("db", "savedefaultlocation").toInt())
    {
    case 0:     // Remember last location
        PreferencesDialog::setSettingsValue("db", "lastlocation", dir);
        break;
    case 2:     // Remember last location for current session only
        PreferencesDialog::setSettingsValue("db", "lastlocation", dir, true);
        break;
    case 1:     // Always use this locations
        break;  // Do nothing
    }
}
