#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QFileDialog>

class FileDialog : public QFileDialog
{
public:
    static QString getOpenFileName(QWidget* parent = 0, const QString& caption = QString(),
                                   const QString& filter = QString(), QString* selectedFilter = 0,
                                   Options options = 0);
    static QString getSaveFileName(QWidget* parent = 0, const QString& caption = QString(),
                                   const QString& filter = QString(), QString* selectedFilter = 0,
                                   Options options = 0);
    static QString getExistingDirectory(QWidget* parent = 0, const QString& caption = QString(),
                                        Options options = 0);

    static QString getSqlDatabaseFileFilter()
    {
        return tr("SQLite database files (*.db *.sqlite *.sqlite3 *.db3);;All files (*)");
    }

private:
    static QString getFileDialogPath();
    static void setFileDialogPath(const QString& new_path);
};

#endif
