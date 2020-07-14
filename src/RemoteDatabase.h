#ifndef REMOTEDATABASE_H
#define REMOTEDATABASE_H

#include <QObject>
#include <QtNetwork/QSslConfiguration>

#include <map>

class QNetworkAccessManager;
class QNetworkConfigurationManager;
class QNetworkReply;
class QProgressDialog;
class QNetworkRequest;
class QHttpMultiPart;
class QFile;
struct sqlite3;

class RemoteMetadataBranchInfo
{
public:
    RemoteMetadataBranchInfo(const std::string& _name, const std::string& _commit_id, const std::string& _description, unsigned int _commit_count) :
        name(_name),
        commit_id(_commit_id),
        description(_description),
        commit_count(_commit_count)
    {}
    RemoteMetadataBranchInfo() :
        commit_count(0)
    {}

    std::string name;
    std::string commit_id;
    std::string description;
    unsigned int commit_count;
};

class RemoteMetadataReleaseInfo
{
public:
    RemoteMetadataReleaseInfo(const std::string& _name, const std::string& _commit_id, const std::string& _date,
                              const std::string& _description, const std::string& _email,
                              const std::string& _user_name, unsigned int _size) :
        name(_name),
        commit_id(_commit_id),
        date(_date),
        description(_description),
        email(_email),
        user_name(_user_name),
        size(_size)
    {}
    RemoteMetadataReleaseInfo() :
        size(0)
    {}

    std::string name;
    std::string commit_id;
    std::string date;
    std::string description;
    std::string email;
    std::string user_name;
    unsigned long size;
};

class RemoteDatabase : public QObject
{
    Q_OBJECT

public:
    RemoteDatabase();
    ~RemoteDatabase() override;

    void reloadSettings();

    enum CertInfo
    {
        CertInfoUser,
        CertInfoServer,
    };

    const QList<QSslCertificate>& caCertificates() const;
    const std::map<QString, QSslCertificate>& clientCertificates() const { return m_clientCertFiles; }
    QString getInfoFromClientCert(const QString& cert, CertInfo info) const;

    enum RequestType
    {
        RequestTypeDatabase,
        RequestTypeDirectory,
        RequestTypeNewVersionCheck,
        RequestTypePush,
        RequestTypeLicenceList,
        RequestTypeBranchList,
        RequestTypeMetadata,
    };

    void fetch(const QString& url, RequestType type, const QString& clientCert = QString(), QVariant userdata = QVariant());
    void push(const QString& filename, const QString& url, const QString& clientCert, const QString& remotename,
              const QString& commitMessage = QString(), const QString& licence = QString(), bool isPublic = false,
              const QString& branch = QString("master"), bool forcePush = false);

    // These function can be used to get information on locally checked out databases
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

        std::string name;
        std::string url;
        std::string commit_id;
        std::string file;
        std::string branch;
        std::string identity;
    };
    std::vector<LocalFileInfo> localGetLocalFiles(QString identity);    // Return a list of all checked out databases for a given identity
    LocalFileInfo localGetLocalFileInfo(QString filename);              // Return information on a single file

signals:
    // As soon as you can safely open a network connection, this signal is emitted. This can be used to delay early network requests
    // which might otherwise fail.
    void networkReady();

    // The openFile signal is emitted whenever a remote database file shall be opened in the main window. This happens when the
    // fetch() call for a database is finished, either by actually downloading the database or opening the local clone.
    void openFile(QString path);

    // These signals are emitted when the fetch() calls are finished that are not requesting a remote database but other data, like
    // a directory listing or the licence list.
    void gotDirList(QString json, QVariant userdata);
    void gotCurrentVersion(QString version, QString url);
    void gotLicenceList(std::vector<std::pair<std::string, std::string>> licences);
    void gotBranchList(std::vector<std::string> branches, std::string default_branch);
    void gotMetadata(std::vector<RemoteMetadataBranchInfo> branches, std::string commits,
                     std::vector<RemoteMetadataReleaseInfo> releases, std::vector<RemoteMetadataReleaseInfo> tags,
                     std::string default_branch);

    // The uploadFinished() signal is emitted when a push() call is finished, i.e. a database upload has completed.
    void uploadFinished(std::string url);

private:
    void gotEncrypted(QNetworkReply* reply);
    void gotReply(QNetworkReply* reply);
    void gotError(QNetworkReply* reply, const QList<QSslError>& errors);
    void updateProgress(qint64 bytesTransmitted, qint64 bytesTotal);
    bool prepareSsl(QNetworkRequest* request, const QString& clientCert);
    void prepareProgressDialog(QNetworkReply* reply, bool upload, const QString& url);

    // Helper functions for managing the list of locally available databases
    void localAssureOpened();
    QString localAdd(QString filename, QString identity, const QUrl& url, const std::string& new_commit_id, const std::string& branch);
    QString localExists(const QUrl& url, QString identity);
    QString localCheckFile(const QString& local_file);
    std::string localLastCommitId(QString clientCert, const QUrl& url, const std::string& branch);

    // Helper functions for building multi-part HTTP requests
    void addPart(QHttpMultiPart* multipart, const QString& name, const QString& value) const;
    void addPart(QHttpMultiPart* multipart, const QString& name, QFile* file, const QString& filename) const;

    // Before using a new client certificate we need to clear the access and authentication cache of the network manager
    // object. Otherwise Qt might reuse the old certificate if the requested URL has been used before.
    void clearAccessCache(const QString& clientCert);

    QNetworkAccessManager* m_manager;
    QNetworkConfigurationManager* m_configurationManager;
    QProgressDialog* m_progress;
    QSslConfiguration m_sslConfiguration;
    std::map<QString, QSslCertificate> m_clientCertFiles;
    sqlite3* m_dbLocal;
};

#endif
