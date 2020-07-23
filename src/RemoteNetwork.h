#ifndef REMOTENETWORK_H
#define REMOTENETWORK_H

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

class RemoteNetwork : public QObject
{
    Q_OBJECT

public:
    static RemoteNetwork& get()
    {
        static RemoteNetwork instance;
        return instance;
    }

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
        RequestTypeDownload,
    };

    void fetch(const QUrl& url, RequestType type, const QString& clientCert = QString(), QVariant userdata = QVariant());
    void push(const QString& filename, const QUrl& url, const QString& clientCert, const QString& remotename,
              const QString& commitMessage = QString(), const QString& licence = QString(), bool isPublic = false,
              const QString& branch = QString("master"), bool forcePush = false, const QString& last_commit = QString());

signals:
    // As soon as you can safely open a network connection, this signal is emitted. This can be used to delay early network requests
    // which might otherwise fail.
    void networkReady();

    // These signals are emitted when the fetch() calls are finished that are not requesting a remote database but other data, like
    // a directory listing or the licence list.
    void gotDirList(QString json, QVariant userdata);
    void gotCurrentVersion(QString version, QString url);
    void gotLicenceList(std::vector<std::pair<std::string, std::string>> licences);
    void gotBranchList(std::vector<std::string> branches, std::string default_branch);
    void gotMetadata(std::vector<RemoteMetadataBranchInfo> branches, std::string commits,
                     std::vector<RemoteMetadataReleaseInfo> releases, std::vector<RemoteMetadataReleaseInfo> tags,
                     std::string default_branch, std::string web_page);

    // The fetchFinished() signal is emitted when a fetch() call for a database is finished
    void fetchFinished(QString filename, QString identity, const QUrl& url, std::string new_commit_id, std::string branch,
                       QDateTime last_modified, QIODevice* device);

    // The pushFinished() signal is emitted when a push() call is finished, i.e. a database upload has completed.
    void pushFinished(QString filename, QString identity, const QUrl& url, std::string new_commit_id, std::string branch, QString source_file);

private:
    RemoteNetwork();
    ~RemoteNetwork() override;

    void gotEncrypted(QNetworkReply* reply);
    void gotReply(QNetworkReply* reply);
    void gotError(QNetworkReply* reply, const QList<QSslError>& errors);
    void updateProgress(qint64 bytesTransmitted, qint64 bytesTotal);
    bool prepareSsl(QNetworkRequest* request, const QString& clientCert);
    void prepareProgressDialog(QNetworkReply* reply, bool upload, const QUrl& url);

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
};

#endif
