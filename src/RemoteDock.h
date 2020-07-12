#ifndef REMOTEDOCK_H
#define REMOTEDOCK_H

#include <QDialog>

class RemoteCommitsModel;
class RemoteDatabase;
class RemoteMetadataBranchInfo;
class RemoteMetadataReleaseInfo;
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

    // This function should be called whenever a database file is opened.
    // It checks whether the file is a checkout of a tracked remote database
    // and updates some of the fields in the remote dock if it is.
    // Call it with an empty file name if the database is closed.
    void fileOpened(const QString& filename);

public slots:
    void reject() override;

private slots:
    void setNewIdentity(const QString& identity);
    void fetchDatabase(const QModelIndex& idx);
    void pushDatabase();
    void newDirectoryNode(const QModelIndex& parent);
    void switchToMainView();
    void openLocalFile(const QModelIndex& idx);
    void showMetadata(const std::vector<RemoteMetadataBranchInfo>& branches, const std::string& commits,
                      const std::vector<RemoteMetadataReleaseInfo>& releases, const std::vector<RemoteMetadataReleaseInfo>& tags,
                      const std::string& default_branch);

signals:
    void openFile(QString file);

private:
    Ui::RemoteDock* ui;

    MainWindow* mainWindow;

    RemoteDatabase& remoteDatabase;
    RemoteModel* remoteModel;
    RemoteLocalFilesModel* remoteLocalFilesModel;
    RemoteCommitsModel* remoteCommitsModel;

    std::string current_commit_json;

    void refreshLocalFileList();
};

#endif
