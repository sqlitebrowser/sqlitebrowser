#ifndef FILEEXTENSIONMANAGER_H
#define FILEEXTENSIONMANAGER_H

#include <QDialog>

namespace Ui {
class FileExtensionManager;
}

class FileExtensionManager : public QDialog
{
    Q_OBJECT

public:
    explicit FileExtensionManager(QWidget *parent = 0);
    ~FileExtensionManager();

    QStringList getDBFileExtesions();

private:
    Ui::FileExtensionManager *ui;

public slots:
    void addItem();
    void removeItem();
    void upItem();
    void downItem();
};

#endif // FILEEXTENSIONMANAGER_H
