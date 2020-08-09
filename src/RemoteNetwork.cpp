#include <QApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QMessageBox>
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include <QtNetwork/QSslKey>
#include <QProgressDialog>
#include <QInputDialog>
#include <QDir>
#include <QUrlQuery>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QNetworkProxyFactory>
#include <QTimeZone>
#include <QtNetwork/QNetworkProxy>
#include <json.hpp>

#include <iterator>

#include "FileDialog.h"
#include "RemoteNetwork.h"
#include "Settings.h"
#include "sqlite.h"
#include "version.h"

using json = nlohmann::json;

RemoteNetwork::RemoteNetwork() :
    m_manager(new QNetworkAccessManager),
    m_configurationManager(new QNetworkConfigurationManager),
    m_progress(nullptr)
{
    // Update network configurations
    connect(m_configurationManager, &QNetworkConfigurationManager::updateCompleted, [this]() {
        m_manager->setConfiguration(m_configurationManager->defaultConfiguration());

        emit networkReady();
    });

    // Set up SSL configuration
    m_sslConfiguration = QSslConfiguration::defaultConfiguration();
    m_sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyPeer);

    // Load CA certs from resource file
    QDir dirCaCerts(":/certs");
    QStringList caCertsList = dirCaCerts.entryList();
    QList<QSslCertificate> caCerts;
    for(const QString& caCertName : caCertsList)
        caCerts += QSslCertificate::fromPath(":/certs/" + caCertName);
    m_sslConfiguration.setCaCertificates(caCerts);

    // Load settings and set up some more stuff while doing so
    reloadSettings();

    // Set up signals
    connect(m_manager, &QNetworkAccessManager::finished, this, &RemoteNetwork::gotReply);
    connect(m_manager, &QNetworkAccessManager::encrypted, this, &RemoteNetwork::gotEncrypted);
    connect(m_manager, &QNetworkAccessManager::sslErrors, this, &RemoteNetwork::gotError);
}

RemoteNetwork::~RemoteNetwork()
{
    delete m_manager;
    delete m_progress;
}

void RemoteNetwork::reloadSettings()
{
    // Load all configured client certificates
    m_clientCertFiles.clear();
    auto client_certs = Settings::getValue("remote", "client_certificates").toStringList();
    for(const QString& path : client_certs)
    {
        QFile file(path);
        file.open(QFile::ReadOnly);
        QSslCertificate cert(&file);
        file.close();
        m_clientCertFiles.insert({path, cert});
    }

    // Always add the default certificate for anonymous access to dbhub.io
    {
        QFile file(":/user_certs/public.cert.pem");
        file.open(QFile::ReadOnly);
        QSslCertificate cert(&file);
        file.close();
        m_clientCertFiles.insert({":/user_certs/public.cert.pem", cert});
    }

    // Configure proxy to use
    {
        QString type = Settings::getValue("proxy", "type").toString();

        QNetworkProxy proxy;
        if(type == "system")
        {
            // For system settings we have to get the system-wide proxy and use that

            // Get list of proxies for accessing dbhub.io via HTTPS and use the first one
            auto list = QNetworkProxyFactory::systemProxyForQuery(QNetworkProxyQuery(QUrl("https://db4s.dbhub.io/")));
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

void RemoteNetwork::gotEncrypted(QNetworkReply* reply)
{
#ifdef Q_OS_MAC
    // Temporary workaround for now, as Qt 5.8 and below doesn't support
    // verifying certificates on OSX: https://bugreports.qt.io/browse/QTBUG-56973
    // Hopefully this is fixed in Qt 5.9
    return;
#else
    // Verify the server's certificate using our CA certs
    auto verificationErrors = reply->sslConfiguration().peerCertificate().verify(m_sslConfiguration.caCertificates());
    bool good = false;
    if(verificationErrors.size() == 0)
    {
        good = true;
    } else if(verificationErrors.size() == 1) {
        // Ignore any self signed certificate errors
        if(verificationErrors.at(0).error() == QSslError::SelfSignedCertificate || verificationErrors.at(0).error() == QSslError::SelfSignedCertificateInChain)
            good = true;
    }

    // If the server certificate didn't turn out to be good, abort the reply here
    if(!good)
        reply->abort();
#endif
}

void RemoteNetwork::gotReply(QNetworkReply* reply)
{
    // What type of data is this?
    RequestType type = static_cast<RequestType>(reply->property("type").toInt());

    // Hide progress dialog before opening a file dialog to make sure the progress dialog doesn't interfer with the file dialog
    if(type == RequestTypeDatabase || type == RequestTypePush)
        m_progress->reset();

    // Handle the reply data
    switch(type)
    {
    case RequestTypeDatabase:
        {
            // It's a database file.

            // Get last modified date as provided by the server
            QDateTime last_modified;
            QString content_disposition = reply->rawHeader("Content-Disposition");
            QRegExp regex("^.*modification-date=\"(.+)\";.*$");
            regex.setMinimal(true); // Set to non-greedy matching
            if(regex.indexIn(content_disposition) != -1)
                last_modified = QDateTime::fromString(regex.cap(1), Qt::ISODate);

            // Extract all other information from reply and send it to slots
            emit fetchFinished(reply->url().fileName(),
                               reply->property("certfile").toString(),
                               reply->url(),
                               QUrlQuery(reply->url()).queryItemValue("commit").toStdString(),
                               QUrlQuery(reply->url()).queryItemValue("branch").toStdString(),
                               last_modified,
                               reply);
        }
        break;
    case RequestTypePush:
        {
            // Read and check results
            json obj = json::parse(reply->readAll(), nullptr, false);
            if(obj.is_discarded() || !obj.is_object())
                break;

            // Extract all information from reply and send it to slots
            emit pushFinished(reply->url().fileName(),
                              reply->property("certfile").toString(),
                              QString::fromStdString(obj["url"]),
                              obj["commit_id"],
                              QUrlQuery(QUrl(QString::fromStdString(obj["url"]))).queryItemValue("branch").toStdString(),
                              reply->property("source_file").toString());
            break;
        }
    case RequestTypeDownload:
        {
            // It's a download

            // Where should we save it?
            QString path = FileDialog::getSaveFileName(FileDialogTypes::CreateDatabaseFile,
                                                       nullptr,
                                                       tr("Choose a location to save the file"),
                                                       QString(),
                                                       reply->url().fileName() + "_" + QUrlQuery(reply->url()).queryItemValue("commit") + ".db");
            if(path.isEmpty())
                break;

            // Save the downloaded data in that file
            QFile file(path);
            file.open(QIODevice::WriteOnly);
            file.write(reply->readAll());
            file.close();
        }
        break;
    }

    // Delete reply later, i.e. after returning from this slot function
    reply->deleteLater();
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

    // Build an error message and short it to the user
    QString message = tr("Error opening remote file at %1.\n%2").arg(reply->url().toString(), errors.at(0).errorString());
    QMessageBox::warning(nullptr, qApp->applicationName(), message);

    // Delete reply later, i.e. after returning from this slot function
    if(m_progress)
        m_progress->reset();
    reply->deleteLater();
}

void RemoteNetwork::updateProgress(qint64 bytesTransmitted, qint64 bytesTotal)
{
    // Find out to which pending reply this progress update belongs
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    // Update progress dialog
    if(bytesTotal == -1)
    {
        // We don't know anything about the current progress, but it's still downloading
        m_progress->setMinimum(0);
        m_progress->setMaximum(0);
        m_progress->setValue(0);
    } else if(bytesTransmitted == bytesTotal) {
        // The download has finished
        m_progress->reset();
    } else {
        // It's still downloading and we know the current progress

        // Were using a range 0 to 10000 here, the progress dialog will calculate 0% to 100% values from that. The reason we're not using
        // the byte counts as-is is that they're 64bit wide while the progress dialog takes only 32bit values, so for large files the values
        // would lose precision. The reason why we're not using a range 0 to 100 is that our range increases the precision a bit and this way
        // we're prepared if the progress dialog will show decimal numbers one day on one platform.
        m_progress->setMinimum(0);
        m_progress->setMaximum(10000);
        m_progress->setValue(static_cast<int>((static_cast<float>(bytesTransmitted) / static_cast<float>(bytesTotal)) * 10000.0f));
    }

    // Check if the Cancel button has been pressed
    if(reply && m_progress->wasCanceled())
    {
        reply->abort();
        m_progress->reset();
    }
}

const QList<QSslCertificate>& RemoteNetwork::caCertificates() const
{
    static QList<QSslCertificate> certs = m_sslConfiguration.caCertificates();
    return certs;
}

QString RemoteNetwork::getInfoFromClientCert(const QString& cert, CertInfo info) const
{
    // Get the common name of the certificate and split it into user name and server address
    QString cn = m_clientCertFiles.at(cert).subjectInfo(QSslCertificate::CommonName).at(0);
    QStringList cn_parts = cn.split("@");
    if(cn_parts.size() < 2)
        return QString();

    // Return requested part of the CN
    if(info == CertInfoUser)
    {
        return cn_parts.first();
    } else if(info == CertInfoServer) {
        // Assemble the full URL from the host name. We use port 443 by default but for
        // local development purposes we use 5550 instead.
        QString host = cn_parts.last();
        host = QString("https://%1%2/").arg(host).arg(host.contains("docker-dev") ? ":5550" : "");
        return host;
    }

    return QString();
}

bool RemoteNetwork::prepareSsl(QNetworkRequest* request, const QString& clientCert)
{
    // Check if client cert exists
    const QSslCertificate& cert = m_clientCertFiles[clientCert];
    if(cert.isNull())
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), tr("Error: Invalid client certificate specified."));
        return false;
    }

    // Load private key for the client certificate
    QFile fileClientCert(clientCert);
    fileClientCert.open(QFile::ReadOnly);
    QSslKey clientKey(&fileClientCert, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);
    while(clientKey.isNull())
    {
        // If the private key couldn't be read, we assume it's password protected. So ask the user for the correct password and try reading it
        // again. If the user cancels the password dialog, abort the whole process.
        QString password = QInputDialog::getText(nullptr, qApp->applicationName(), tr("Please enter the passphrase for this client certificate in order to authenticate."));
        if(password.isEmpty())
            return false;
        clientKey = QSslKey(&fileClientCert, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, password.toUtf8());
    }
    fileClientCert.close();

    // Set client certificate (from the cache) and private key (just loaded)
    m_sslConfiguration.setLocalCertificate(cert);
    m_sslConfiguration.setPrivateKey(clientKey);

    // Apply SSL configuration
    request->setSslConfiguration(m_sslConfiguration);

    return true;
}

void RemoteNetwork::prepareProgressDialog(QNetworkReply* reply, bool upload, const QUrl& url)
{
    // Instantiate progress dialog and apply some basic settings
    if(!m_progress)
        m_progress = new QProgressDialog();
    m_progress->reset();
    m_progress->setWindowModality(Qt::NonModal);
    m_progress->setCancelButtonText(tr("Cancel"));

    // Set dialog text
    QString url_for_display = url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery);
    if(upload)
        m_progress->setLabelText(tr("Uploading remote database to\n%1").arg(url_for_display));
    else
        m_progress->setLabelText(tr("Downloading remote database from\n%1").arg(url_for_display));

    // Show dialog
    m_progress->show();

    // Make sure the dialog is updated
    if(upload)
        connect(reply, &QNetworkReply::uploadProgress, this, &RemoteNetwork::updateProgress);
    else
        connect(reply, &QNetworkReply::downloadProgress, this, &RemoteNetwork::updateProgress);
}

void RemoteNetwork::fetch(const QUrl& url, RequestType type, const QString& clientCert,
                          std::function<void(QByteArray)> when_finished, bool synchronous)
{
    // Check if network is accessible. If not, abort right here
    if(m_manager->networkAccessible() == QNetworkAccessManager::NotAccessible)
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), tr("Error: The network is not accessible."));
        return;
    }

    // Build network request
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", QString("%1 %2").arg(qApp->organizationName(), APP_VERSION).toUtf8());
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
#endif

    // Set SSL configuration when trying to access a file via the HTTPS protocol.
    // Skip this step when no client certificate was specified. In this case the default HTTPS configuration is used.
    bool https = url.scheme().compare("https", Qt::CaseInsensitive) == 0;
    if(https && !clientCert.isNull())
    {
        // If configuring the SSL connection fails, abort the request here
        if(!prepareSsl(&request, clientCert))
            return;
    }

    // Clear access cache if necessary
    clearAccessCache(clientCert);

    // Fetch database and prepare pending reply for future processing
    QNetworkReply* reply = m_manager->get(request);
    reply->setProperty("type", type);
    reply->setProperty("certfile", clientCert);

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

    // Initialise the progress dialog for this request, but only if this is a database file or a download.
    // Directory listing and similar are small enough to be loaded without progress dialog.
    if(type == RequestTypeDatabase || type == RequestTypeDownload)
        prepareProgressDialog(reply, false, url);
}

void RemoteNetwork::push(const QString& filename, const QUrl& url, const QString& clientCert, const QString& remotename,
                         const QString& commitMessage, const QString& licence, bool isPublic, const QString& branch,
                         bool forcePush, const QString& last_commit)
{
    // Check if network is accessible. If not, abort right here
    if(m_manager->networkAccessible() == QNetworkAccessManager::NotAccessible)
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), tr("Error: The network is not accessible."));
        return;
    }

    // Open the file to send and check if it exists
    QFile* file = new QFile(filename);
    if(!file->open(QFile::ReadOnly))
    {
        delete file;
        QMessageBox::warning(nullptr, qApp->applicationName(), tr("Error: Cannot open the file for sending."));
        return;
    }

    // Build network request
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", QString("%1 %2").arg(qApp->organizationName(), APP_VERSION).toUtf8());

    // Get the last modified date of the file and prepare it for conversion into the ISO date format
    QDateTime last_modified = QFileInfo(filename).lastModified().toOffsetFromUtc(0);

    // Prepare HTTP multi part data containing all the information about the commit we're about to push
    QHttpMultiPart* multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    addPart(multipart, "file", file, remotename);
    addPart(multipart, "commitmsg", commitMessage);
    addPart(multipart, "licence", licence);
    addPart(multipart, "public", isPublic ? "true" : "false");
    addPart(multipart, "branch", branch);
    addPart(multipart, "force", forcePush ? "true" : "false");
    addPart(multipart, "lastmodified", last_modified.toString("yyyy-MM-dd'T'HH:mm:ss'Z'"));

    // Only add commit id if one was provided
    if(!last_commit.isEmpty())
        addPart(multipart, "commit", last_commit);

    // Set SSL configuration when trying to access a file via the HTTPS protocol
    bool https = url.scheme().compare("https", Qt::CaseInsensitive) == 0;
    if(https)
    {
        // If configuring the SSL connection fails, abort the request here
        if(!prepareSsl(&request, clientCert))
        {
            delete file;
            return;
        }
    }

    // Clear access cache if necessary
    clearAccessCache(clientCert);

    // Put database to remote server and save pending reply for future processing
    QNetworkReply* reply = m_manager->post(request, multipart);
    reply->setProperty("type", RequestTypePush);
    reply->setProperty("certfile", clientCert);
    reply->setProperty("source_file", filename);
    multipart->setParent(reply);        // Delete the multi-part object along with the reply

    // Initialise the progress dialog for this request
    prepareProgressDialog(reply, true, url);
}

void RemoteNetwork::addPart(QHttpMultiPart* multipart, const QString& name, const QString& value) const
{
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%1\"").arg(name));
    part.setBody(value.toUtf8());

    multipart->append(part);
}

void RemoteNetwork::addPart(QHttpMultiPart* multipart, const QString& name, QFile* file, const QString& filename) const
{
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%1\"; filename=\"%2\"").arg(name, filename));
    part.setBodyDevice(file);
    file->setParent(multipart);     // Close the file and delete the file object as soon as the multi-part object is destroyed

    multipart->append(part);
}

void RemoteNetwork::clearAccessCache(const QString& clientCert)
{
    // When the client certificate is different from the one before, clear the access and authentication cache.
    // Otherwise Qt might use the old certificate again.
    static QString lastClientCert;
    if(lastClientCert != clientCert)
    {
        lastClientCert = clientCert;
        m_manager->clearAccessCache();
    }
}

bool RemoteNetwork::handleReply(QNetworkReply* reply)
{
    // Check if request was successful
    if(reply->error() != QNetworkReply::NoError)
    {
        // Do not show error message when operation was cancelled on purpose
        if(reply->error() != QNetworkReply::OperationCanceledError)
        {
            QMessageBox::warning(nullptr, qApp->applicationName(),
                                 reply->errorString() + "\n" + reply->readAll());
        }

        reply->deleteLater();
        return false;
    }

    return true;
}
