#ifndef REMOTEDATABASE_H
#define REMOTEDATABASE_H

#include <QObject>
#include <QSslConfiguration>

class QNetworkAccessManager;
class QString;
class QNetworkReply;
class QSslError;
class QProgressDialog;

class RemoteDatabase : public QObject
{
    Q_OBJECT

public:
    RemoteDatabase();
    virtual ~RemoteDatabase();

    void reloadSettings();

    const QList<QSslCertificate>& caCertificates() const;
    const QMap<QString, QSslCertificate>& clientCertificates() const { return m_clientCertFiles; }

    void fetchDatabase(const QString& url, const QString& clientCert);

signals:
    void openFile(QString path);

private:
    void gotEncrypted(QNetworkReply* reply);
    void gotReply(QNetworkReply* reply);
    void gotError(QNetworkReply* reply, const QList<QSslError>& errors);
    void updateProgress(qint64 bytesReceived, qint64 bytesTotal);

    QNetworkAccessManager* m_manager;
    QProgressDialog* m_progress;
    QNetworkReply* m_currentReply;
    QSslConfiguration m_sslConfiguration;
    QMap<QString, QSslCertificate> m_clientCertFiles;
};

#endif
