#ifndef REMOTEDOCK_H
#define REMOTEDOCK_H

#include <QDialog>

class RemoteDatabase;
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
    ~RemoteDock();

    void reloadSettings();
    void enableButtons();

private slots:
    void setNewIdentity();
    void fetchDatabase(const QModelIndex& idx);
    void pushDatabase();
    void newDirectoryNode(const QModelIndex& parent);

private:
    Ui::RemoteDock* ui;

    MainWindow* mainWindow;

    RemoteDatabase& remoteDatabase;
    RemoteModel* remoteModel;
};

#endif
