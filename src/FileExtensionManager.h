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
    explicit FileExtensionManager(QStringList init, QWidget *parent = nullptr);
    ~FileExtensionManager() override;

    QStringList getDBFileExtensions() const;

private:
    Ui::FileExtensionManager *ui;

public slots:
    void addItem();
    void removeItem();
    void upItem();
    void downItem();
};

#endif // FILEEXTENSIONMANAGER_H
