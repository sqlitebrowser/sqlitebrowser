#include "FileDialog.h"
#include "Settings.h"

QString FileDialog::getOpenFileName(const FileDialogTypes dialogType, QWidget* parent, const QString& caption, const QString &filter, QString *selectedFilter, Options options)
{
    QString result = QFileDialog::getOpenFileName(parent, caption, getFileDialogPath(dialogType), filter, selectedFilter, options);
    if(!result.isEmpty())
        setFileDialogPath(dialogType, result);
    return result;
}

QStringList FileDialog::getOpenFileNames(const FileDialogTypes dialogType, QWidget *parent, const QString &caption, const QString &filter, QString *selectedFilter, QFileDialog::Options options)
{
    QStringList result = QFileDialog::getOpenFileNames(parent, caption, getFileDialogPath(dialogType), filter, selectedFilter, options);
    if(!result.isEmpty())
    {
        QFileInfo path = QFileInfo(result.first());
        setFileDialogPath(dialogType, path.absolutePath());
    }
    return result;
}

QString FileDialog::getSaveFileName(const FileDialogTypes dialogType, QWidget* parent, const QString& caption, const QString& filter, const QString& defaultFileName, QString* selectedFilter, Options options)
{
    QString dir = getFileDialogPath(dialogType);
    if(!defaultFileName.isEmpty())
        dir += "/" + defaultFileName;

    QString result = QFileDialog::getSaveFileName(parent, caption, dir, filter, selectedFilter, options);
    if(!result.isEmpty())
        setFileDialogPath(dialogType, result);
    return result;
}

QString FileDialog::getExistingDirectory(const FileDialogTypes dialogType, QWidget* parent, const QString& caption, Options options)
{
    QString result = QFileDialog::getExistingDirectory(parent, caption, getFileDialogPath(dialogType), options);
    if(!result.isEmpty())
        setFileDialogPath(dialogType, result);
    return result;
}

QString FileDialog::getFileDialogPath(const FileDialogTypes dialogType)
{
    switch(Settings::getValue("db", "savedefaultlocation").toInt())
    {
    case 0:     // Remember last location
    case 2: {   // Remember last location for current session only
        QHash<QString, QVariant> lastLocations = Settings::getValue("db", "lastlocations").toHash();

        return lastLocations[QString::number(static_cast<int>(dialogType))].toString();
    }
    case 1:     // Always use this locations
        return Settings::getValue("db", "defaultlocation").toString();
    default:
        return QString();
    }
}

void FileDialog::setFileDialogPath(const FileDialogTypes dialogType, const QString& new_path)
{
    QString dir = QFileInfo(new_path).absolutePath();
    QHash<QString, QVariant> lastLocations = Settings::getValue("db", "lastlocations").toHash();

    lastLocations[QString::number(static_cast<int>(dialogType))] = dir;

    switch(Settings::getValue("db", "savedefaultlocation").toInt())
    {
    case 0:     // Remember last location
        Settings::setValue("db", "lastlocations", lastLocations);
        break;
    case 2:     // Remember last location for current session only
        Settings::setValue("db", "lastlocations", lastLocations, false);
        break;
    case 1:     // Always use this locations
        break;  // Do nothing
    }
}

QString FileDialog::getSqlDatabaseFileFilter()
{
    return Settings::getValue("General", "DBFileExtensions").toString() + ";;" + QObject::tr("All files (*)"); //Always add "All files (*)" to the available filters
}
