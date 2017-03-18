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

private slots:
    void setNewIdentity();
    void fetchDatabase(const QModelIndex& idx);

private:
    Ui::RemoteDock* ui;

    RemoteDatabase& remoteDatabase;
    RemoteModel* remoteModel;
};

#endif
