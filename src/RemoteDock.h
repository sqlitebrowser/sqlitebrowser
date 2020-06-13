#ifndef REMOTEDOCK_H
#define REMOTEDOCK_H

#include <QDialog>

class RemoteDatabase;
class RemoteLocalFilesModel;
class RemoteModel;
class MainWindow;

namespace Ui {
class RemoteDock;
}

class RemoteDock : public QDialog
{
    Q_OBJECT

public:
    explicit RemoteDock(MainWindow* parent);
    ~RemoteDock() override;

    void reloadSettings();
    void enableButtons();

public slots:
    void reject() override;

private slots:
    void setNewIdentity(const QString& identity);
    void fetchDatabase(const QModelIndex& idx);
    void pushDatabase();
    void newDirectoryNode(const QModelIndex& parent);
    void switchToMainView();
    void openLocalFile(const QModelIndex& idx);

signals:
    void openFile(QString file);

private:
    Ui::RemoteDock* ui;

    MainWindow* mainWindow;

    RemoteDatabase& remoteDatabase;
    RemoteModel* remoteModel;
    RemoteLocalFilesModel* remoteLocalFilesModel;

    void refreshLocalFileList();
};

#endif
