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
    };

    void fetch(const QString& url, RequestType type, const QString& clientCert = QString(), QVariant userdata = QVariant());
    void push(const QString& filename, const QString& url, const QString& clientCert,
              const QString& commitMessage = QString(), const QString& licence = QString(), bool isPublic = false);

signals:
    void gotDirList(QString json, QVariant userdata);
    void openFile(QString path);
    void gotCurrentVersion(QString version, QString url);
    void gotLicenceList(QMap<QString, QString> licences);

private:
    void gotEncrypted(QNetworkReply* reply);
    void gotReply(QNetworkReply* reply);
    void gotError(QNetworkReply* reply, const QList<QSslError>& errors);
    void updateProgress(qint64 bytesTransmitted, qint64 bytesTotal);
    bool prepareSsl(QNetworkRequest* request, const QString& clientCert);
    void prepareProgressDialog(bool upload, const QString& url);

    // Helper functions for managing the list of locally available databases
    void localAssureOpened();
    void localAdd(QString filename, QString identity, const QUrl& url);
    QString localExists(const QUrl& url, QString identity);

    QNetworkAccessManager* m_manager;
    QProgressDialog* m_progress;
    QNetworkReply* m_currentReply;
    QSslConfiguration m_sslConfiguration;
    QMap<QString, QSslCertificate> m_clientCertFiles;
    sqlite3* m_dbLocal;
};

#endif
