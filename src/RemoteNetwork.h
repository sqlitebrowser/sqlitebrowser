#ifndef REMOTENETWORK_H
#define REMOTENETWORK_H

#include <QObject>
#include <QtNetwork/QSslConfiguration>

class QNetworkAccessManager;
class QNetworkReply;

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

    enum RequestType
    {
        RequestTypeCustom,
    };

    void fetch(const QUrl& url, RequestType type, std::function<void(QByteArray)> when_finished = {}, bool synchronous = false, bool ignore_errors = false);

private:
    RemoteNetwork();
    ~RemoteNetwork() override;

    void gotReply(QNetworkReply* reply);
    void gotError(QNetworkReply* reply, const QList<QSslError>& errors);

    // This function is called for all network replies we get whether they are handled globally or individually.
    // It mainly does some error checking and returns true if the actual handler should be called.
    bool handleReply(QNetworkReply* reply);

    QNetworkAccessManager* m_manager;
};

#endif
