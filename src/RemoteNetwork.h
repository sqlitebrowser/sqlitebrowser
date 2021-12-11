#ifndef REMOTENETWORK_H
#define REMOTENETWORK_H

#include <QObject>
#include <QtNetwork/QSslConfiguration>

#include <functional>
#include <map>

class QNetworkAccessManager;
class QNetworkReply;
class QProgressDialog;
class QNetworkRequest;
class QHttpMultiPart;
class QFile;

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
        RequestTypeCustom,
        RequestTypeDatabase,
        RequestTypePush,
        RequestTypeDownload,
    };

    void fetch(const QUrl& url, RequestType type, const QString& clientCert = QString(),
               std::function<void(QByteArray)> when_finished = {}, bool synchronous = false, bool ignore_errors = false);
    void push(const QString& filename, const QUrl& url, const QString& clientCert, const QString& remotename,
              const QString& commitMessage = QString(), const QString& licence = QString(), bool isPublic = false,
              const QString& branch = QString("master"), bool forcePush = false, const QString& last_commit = QString());

signals:
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

    // This function is called for all network replies we get whether they are handled globally or individually.
    // It mainly does some error checking and returns true if the actual handler should be called.
    bool handleReply(QNetworkReply* reply);

    QNetworkAccessManager* m_manager;
    QProgressDialog* m_progress;
    QSslConfiguration m_sslConfiguration;
    std::map<QString, QSslCertificate> m_clientCertFiles;
};

#endif
