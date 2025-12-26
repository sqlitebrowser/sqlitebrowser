#include <QApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QMessageBox>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkProxy>

#include "RemoteNetwork.h"
#include "Settings.h"
#include "version.h"

RemoteNetwork::RemoteNetwork() :
    m_manager(new QNetworkAccessManager)
{
    // Load settings and set up some more stuff while doing so
    reloadSettings();

    // Set up signals
    connect(m_manager, &QNetworkAccessManager::sslErrors, this, &RemoteNetwork::gotError);
}

RemoteNetwork::~RemoteNetwork()
{
    delete m_manager;
}

void RemoteNetwork::reloadSettings()
{
    // Configure proxy to use
    {
        QString type = Settings::getValue("proxy", "type").toString();

        QNetworkProxy proxy;
        if(type == "system")
        {
            // For system settings we have to get the system-wide proxy and use that

            // Get list of proxies for accessing sqlitebrowser.org via HTTPS and use the first one
            auto list = QNetworkProxyFactory::systemProxyForQuery(QNetworkProxyQuery(QUrl("https://sqlitebrowser.org/")));
            proxy = list.front();
        } else {
            // For any other type we have to set up our own proxy configuration

            // Retrieve the required settings
            QString host = Settings::getValue("proxy", "host").toString();
            unsigned short port = static_cast<unsigned short>(Settings::getValue("proxy", "port").toUInt());
            bool authentication = Settings::getValue("proxy", "authentication").toBool();

            if(type == "http")
                proxy.setType(QNetworkProxy::HttpProxy);
            else if(type == "socks5")
                proxy.setType(QNetworkProxy::Socks5Proxy);
            else
                proxy.setType(QNetworkProxy::NoProxy);

            proxy.setHostName(host);
            proxy.setPort(port);

            // Only set authentication details when authentication is required
            if(authentication)
            {
                QString user = Settings::getValue("proxy", "user").toString();
                QString password = Settings::getValue("proxy", "password").toString();

                proxy.setUser(user);
                proxy.setPassword(password);
            }
        }

        // Start using the new proxy configuration
        QNetworkProxy::setApplicationProxy(proxy);
    }
}

void RemoteNetwork::gotReply(QNetworkReply* reply)
{
    // What type of data is this?
    RequestType type = static_cast<RequestType>(reply->property("type").toInt());

    // Handle the reply data
    switch(type)
    {
    case RequestTypeCustom:
        break;
    }

    // Delete reply later, i.e. after returning from this slot function
    reply->deleteLater();
}

void RemoteNetwork::fetch(const QUrl& url, RequestType type, std::function<void(QByteArray)> when_finished, bool synchronous, bool ignore_errors)
{
    // Build network request
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", QString("%1 %2").arg(qApp->organizationName(), APP_VERSION).toUtf8());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
#elif QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
#endif

    // Fetch database and prepare pending reply for future processing
    QNetworkReply* reply = m_manager->get(request);
    reply->setProperty("type", type);
    reply->setProperty("ignore_errors", ignore_errors);

    // Hook up custom handler when there is one and global handler otherwise
    if(when_finished)
    {
        connect(reply, &QNetworkReply::finished, reply, [this, when_finished, reply]() {
            if(handleReply(reply))
                when_finished(reply->readAll());
        });
    } else {
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            if(handleReply(reply))
                gotReply(reply);
        });
    }

    // When the synchrounous flag is set we wait for the request to finish before continuing
    if(synchronous)
    {
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
    }
}

void RemoteNetwork::gotError(QNetworkReply* reply, const QList<QSslError>& errors)
{
    // Are there any errors in here that aren't about self-signed certificates and non-matching hostnames?
    bool serious_errors = std::any_of(errors.begin(), errors.end(), [](const QSslError& error) { return error.error() != QSslError::SelfSignedCertificate; });

    // Just stop the error checking here and accept the reply if there were no 'serious' errors
    if(!serious_errors)
    {
        reply->ignoreSslErrors(errors);
        return;
    }

    // Build an error message and show it to the user
    QString message = tr("Error opening remote file at %1.\n%2").arg(reply->url().toString(), errors.at(0).errorString());
    QMessageBox::warning(nullptr, qApp->applicationName(), message);

    // Delete reply later, i.e. after returning from this slot function
    reply->deleteLater();
}

bool RemoteNetwork::handleReply(QNetworkReply* reply)
{
    // Check if request was successful
    if(reply->error() != QNetworkReply::NoError)
    {
        // Do not show error message when operation was cancelled on purpose
        if(reply->error() != QNetworkReply::OperationCanceledError && !reply->property("ignore_errors").toBool())
        {
            QMessageBox::warning(nullptr, qApp->applicationName(),
                                 reply->errorString() + "\n" + reply->readAll());
        }

        reply->deleteLater();
        return false;
    }

    return true;
}
