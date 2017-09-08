#ifndef REMOTEDATABASE_H
#define REMOTEDATABASE_H

#include <QObject>
#include <QSslConfiguration>

class QNetworkAccessManager;
class QString;
class QNetworkReply;
class QSslError;
class QProgressDialog;
class QNetworkRequest;
class QHttpMultiPart;
class QFile;
struct sqlite3;

class RemoteDatabase : public QObject
{
    Q_OBJECT

public:
    RemoteDatabase();
    virtual ~RemoteDatabase();

    void reloadSettings();

    enum CertInfo
    {
        CertInfoUser,
        CertInfoServer,
    };

    const QList<QSslCertificate>& caCertificates() const;
    const QMap<QString, QSslCertificate>& clientCertificates() const { return m_clientCertFiles; }
    QString getInfoFromClientCert(const QString& cert, CertInfo info) const;

    enum RequestType
    {
        RequestTypeDatabase,
        RequestTypeDirectory,
        RequestTypeNewVersionCheck,
        RequestTypePush,
        RequestTypeLicenceList,
        RequestTypeBranchList,
    };

    void fetch(const QString& url, RequestType type, const QString& clientCert = QString(), QVariant userdata = QVariant());
    void push(const QString& filename, const QString& url, const QString& clientCert, const QString& remotename,
              const QString& commitMessage = QString(), const QString& licence = QString(), bool isPublic = false, const QString& branch = QString("master"));

signals:
    // The openFile signal is emitted whenever a remote database file shall be opened in the main window. This happens when the
    // fetch() call for a database is finished, either by actually downloading the database or opening the local clone.
    void openFile(QString path);

    // These signals are emitted when the fetch() calls are finished that are not requesting a remote database but other data, like
    // a directory listing or the licence list.
    void gotDirList(QString json, QVariant userdata);
    void gotCurrentVersion(QString version, QString url);
    void gotLicenceList(QMap<QString, QString> licences);
    void gotBranchList(QStringList branches, QString default_branch);

    // The uploadFinished() signal is emitted when a push() call is finished, i.e. a database upload has completed.
    void uploadFinished(QString url);

private:
    void gotEncrypted(QNetworkReply* reply);
    void gotReply(QNetworkReply* reply);
    void gotError(QNetworkReply* reply, const QList<QSslError>& errors);
    void updateProgress(qint64 bytesTransmitted, qint64 bytesTotal);
    bool prepareSsl(QNetworkRequest* request, const QString& clientCert);
    void prepareProgressDialog(QNetworkReply* reply, bool upload, const QString& url);

    // Helper functions for managing the list of locally available databases
    void localAssureOpened();
    void localAdd(QString filename, QString identity, const QUrl& url);
    QString localExists(const QUrl& url, QString identity);
    QString localCheckFile(const QString& local_file);

    // Helper functions for building multi-part HTTP requests
    void addPart(QHttpMultiPart* multipart, const QString& name, const QString& value);
    void addPart(QHttpMultiPart* multipart, const QString& name, QFile* file, const QString& filename);

    // Before using a new client certificate we need to clear the access and authentication cache of the network manager
    // object. Otherwise Qt might reuse the old certificate if the requested URL has been used before.
    void clearAccessCache(const QString& clientCert);

    QNetworkAccessManager* m_manager;
    QProgressDialog* m_progress;
    QSslConfiguration m_sslConfiguration;
    QMap<QString, QSslCertificate> m_clientCertFiles;
    sqlite3* m_dbLocal;
};

#endif
