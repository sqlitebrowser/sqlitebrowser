#ifndef REMOTEDATABASE_H
#define REMOTEDATABASE_H

#include <QObject>
#include <QVariant>

#include <string>

struct sqlite3;

class RemoteDatabase : public QObject
{
    Q_OBJECT

public:
    RemoteDatabase();
    ~RemoteDatabase() override;

    // This class compiles all the information on a lcao database file
    class LocalFileInfo
    {
    public:
        LocalFileInfo()
        {}

        LocalFileInfo(const std::string& _name,
                      const std::string& _url,
                      const std::string& _commit_id,
                      const std::string& _file,
                      const std::string& _branch,
                      const std::string& _identity) :
            name(_name),
            url(_url),
            commit_id(_commit_id),
            file(_file),
            branch(_branch),
            identity(_identity)
        {}

        void clear() { name = url = commit_id = file = branch = identity = {}; }
        QString user_name() const;

        std::string name;           // Database name
        std::string url;            // URL for cloning
        std::string commit_id;      // Commit ID at the time of the cloning
        std::string file;           // Name of the local file on disk
        std::string branch;         // Cloned branch
        std::string identity;       // Identity used for cloning
    };

    // Return a list of all checked out databases for a given identity
    std::vector<LocalFileInfo> localGetLocalFiles(QString identity);

    // Return information on a single file
    LocalFileInfo localGetLocalFileInfo(QString filename);

    // Delete a local database clone
    void localDeleteFile(QString filename);

    // This function checks if there already is a clone for the given combination of url and identity. It returns the filename
    // of this clone if there is or a null string if there isn't a clone yet. The identity needs to be part of this check because
    // with the url alone there could be corner cases where different versions or whatever may not be accessible for all users.
    // If the URL contains a commit id (optional), this commit id is part of the check.
    QString localExists(const QUrl& url, QString identity, const std::string& branch);

    // This function takes a file name and checks with which commit id we had checked out this file or last pushed it.
    std::string localLastCommitId(QString clientCert, const QUrl& url, const std::string& branch);

    // This function adds a new local database clone to our internal list. It does so by adding a single
    // new record to the remote dbs database. All the fields are extracted from the filename, the identity
    // and (most importantly) the url parameters. Note that for the commit id field to be correctly filled we
    // require the commit id to be part of the url parameter. Also note that this function doesn't care if the
    // database has already been added to the list or not. If you need this information you need to check it before
    // calling this function, ideally even before sending out a request to the network. The function returns the full
    // path of the newly created/updated file.
    QString localAdd(QString filename, QString identity, const QUrl& url, const std::string& new_commit_id, const std::string& branch);

private:
    // Helper functions for managing the list of locally available databases
    void localAssureOpened();
    QString localCheckFile(const QString& local_file);

    sqlite3* m_dbLocal;
};

#endif
