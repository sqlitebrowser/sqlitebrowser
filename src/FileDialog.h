#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QFileDialog>

class FileDialog : public QFileDialog
{
    Q_OBJECT

public:
    static QString getOpenFileName(QWidget* parent = nullptr, const QString& caption = QString(),
                                   const QString& filter = QString(), QString* selectedFilter = nullptr,
                                   Options options = 0);
    static QStringList getOpenFileNames(QWidget* parent = nullptr, const QString& caption = QString(),
                                        const QString& filter = QString(), QString* selectedFilter = nullptr,
                                        Options options = 0);
    static QString getSaveFileName(QWidget* parent = nullptr, const QString& caption = QString(),
                                   const QString& filter = QString(), const QString& defaultFileName = QString(), QString* selectedFilter = nullptr,
                                   Options options = 0);
    static QString getExistingDirectory(QWidget* parent = nullptr, const QString& caption = QString(),
                                        Options options = 0);

    static QString getSqlDatabaseFileFilter();

private:
    static QString getFileDialogPath();
    static void setFileDialogPath(const QString& new_path);
};

#endif
