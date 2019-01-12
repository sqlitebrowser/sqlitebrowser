#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QFileDialog>

enum FileDialogTypes {
    NoSpecificType,

    CreateProjectFile,
    OpenProjectFile,

    CreateDatabaseFile,
    OpenDatabaseFile,

    CreateSQLFile,
    OpenSQLFile,

    OpenCSVFile,

    CreateDataFile,
    OpenDataFile,

    OpenExtensionFile,
    OpenCertificateFile
};

class FileDialog : public QFileDialog
{
    Q_OBJECT

public:
    static QString getOpenFileName(const FileDialogTypes dialogType, QWidget* parent = nullptr, const QString& caption = QString(),
                                   const QString& filter = QString(), QString* selectedFilter = nullptr,
                                   Options options = 0);
    static QStringList getOpenFileNames(const FileDialogTypes dialogType, QWidget* parent = nullptr, const QString& caption = QString(),
                                        const QString& filter = QString(), QString* selectedFilter = nullptr,
                                        Options options = 0);
    static QString getSaveFileName(const FileDialogTypes dialogType, QWidget* parent = nullptr, const QString& caption = QString(),
                                   const QString& filter = QString(), const QString& defaultFileName = QString(), QString* selectedFilter = nullptr,
                                   Options options = 0);
    static QString getExistingDirectory(const FileDialogTypes dialogType, QWidget* parent = nullptr, const QString& caption = QString(),
                                        Options options = 0);

    static QString getSqlDatabaseFileFilter();

private:
    static QString getFileDialogPath(const FileDialogTypes dialogType);
    static void setFileDialogPath(const FileDialogTypes dialogType, const QString& new_path);
};

#endif
