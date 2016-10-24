#ifndef REMOTEDATABASE_H
#define REMOTEDATABASE_H

#include <QObject>

class QNetworkAccessManager;
class QString;
class QNetworkReply;
class QSslError;

class RemoteDatabase : public QObject
{
    Q_OBJECT

public:
    RemoteDatabase();
    virtual ~RemoteDatabase();

    void fetchDatabase(const QString& url);

signals:
    void openFile(QString path);

private:
    void gotEncrypted(QNetworkReply* reply);
    void gotReply(QNetworkReply* reply);
    void gotError(QNetworkReply* reply, const QList<QSslError>& errors);

    QNetworkAccessManager* m_manager;
};

#endif
