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

    void fetchDatabase(const QString& url);

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
};

#endif
