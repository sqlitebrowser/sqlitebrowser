#include "FileDialog.h"
#include "Settings.h"

QString FileDialog::getOpenFileName(QWidget* parent, const QString& caption, const QString &filter, QString *selectedFilter, Options options)
{
    QString result = QFileDialog::getOpenFileName(parent, caption, getFileDialogPath(), filter, selectedFilter, options);
    if(!result.isEmpty())
        setFileDialogPath(result);
    return result;
}

QStringList FileDialog::getOpenFileNames(QWidget *parent, const QString &caption, const QString &filter, QString *selectedFilter, QFileDialog::Options options)
{
    QStringList result = QFileDialog::getOpenFileNames(parent, caption, getFileDialogPath(), filter, selectedFilter, options);
    if(!result.isEmpty())
    {
        QFileInfo path = QFileInfo(result.first());
        setFileDialogPath(path.absolutePath());
    }
    return result;
}

QString FileDialog::getSaveFileName(QWidget* parent, const QString& caption, const QString& filter, const QString& defaultFileName, QString* selectedFilter, Options options)
{
    QString dir = getFileDialogPath();
    if(!defaultFileName.isEmpty())
        dir += "/" + defaultFileName;

    QString result = QFileDialog::getSaveFileName(parent, caption, dir, filter, selectedFilter, options);
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
    switch(Settings::getValue("db", "savedefaultlocation").toInt())
    {
    case 0:     // Remember last location
    case 2:     // Remember last location for current session only
        return Settings::getValue("db", "lastlocation").toString();
    case 1:     // Always use this locations
        return Settings::getValue("db", "defaultlocation").toString();
    default:
        return "";
    }
}

void FileDialog::setFileDialogPath(const QString& new_path)
{
    QString dir = QFileInfo(new_path).absolutePath();

    switch(Settings::getValue("db", "savedefaultlocation").toInt())
    {
    case 0:     // Remember last location
        Settings::setValue("db", "lastlocation", dir);
        break;
    case 2:     // Remember last location for current session only
        Settings::setValue("db", "lastlocation", dir, true);
        break;
    case 1:     // Always use this locations
        break;  // Do nothing
    }
}
