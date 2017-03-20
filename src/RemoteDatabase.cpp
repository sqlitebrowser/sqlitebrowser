#include <QApplication>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QNetworkReply>
#include <QFile>
#include <QSslKey>
#include <QProgressDialog>
#include <QInputDialog>
#include <QDir>

#include "RemoteDatabase.h"
#include "version.h"
#include "Settings.h"

RemoteDatabase::RemoteDatabase() :
    m_manager(new QNetworkAccessManager),
    m_progress(nullptr),
    m_currentReply(nullptr)
{
    // Set up SSL configuration
    m_sslConfiguration = QSslConfiguration::defaultConfiguration();
    m_sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyPeer);

    // Load CA certs from resource file
    QDir dirCaCerts(":/certs");
    QStringList caCertsList = dirCaCerts.entryList();
    QList<QSslCertificate> caCerts;
    foreach(const QString& caCertName, caCertsList)
        caCerts += QSslCertificate::fromPath(":/certs/" + caCertName);
    m_sslConfiguration.setCaCertificates(caCerts);

    // Load settings and set up some more stuff while doing so
    reloadSettings();

    // Set up signals
    connect(m_manager, &QNetworkAccessManager::finished, this, &RemoteDatabase::gotReply);
    connect(m_manager, &QNetworkAccessManager::encrypted, this, &RemoteDatabase::gotEncrypted);
    connect(m_manager, &QNetworkAccessManager::sslErrors, this, &RemoteDatabase::gotError);
}

RemoteDatabase::~RemoteDatabase()
{
    delete m_manager;
    delete m_progress;
}

void RemoteDatabase::reloadSettings()
{
    // Load all configured client certificates
    m_clientCertFiles.clear();
    auto client_certs = Settings::getValue("remote", "client_certificates").toStringList();
    foreach(const QString& path, client_certs)
    {
        QFile file(path);
        file.open(QFile::ReadOnly);
        QSslCertificate cert(&file);
        file.close();
        m_clientCertFiles.insert(path, cert);
    }

    // TODO Add support for proxies here
}

void RemoteDatabase::gotEncrypted(QNetworkReply* reply)
{
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
}

void RemoteDatabase::gotReply(QNetworkReply* reply)
{
    // Check if request was successful
    if(reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::warning(0, qApp->applicationName(),
                             tr("Error when connecting to %1.\n%2").arg(reply->url().toString()).arg(reply->errorString()));
        reply->deleteLater();
        return;
    }

    // Check for redirect
    QString redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
    if(!redirectUrl.isEmpty())
    {
        // Avoid redirect loop
        if(reply->url() == redirectUrl)
        {
            reply->deleteLater();
            return;
        }
        fetch(redirectUrl, static_cast<RequestType>(reply->property("type").toInt()), reply->property("certfile").toString(), reply->property("userdata"));
        reply->deleteLater();
        return;
    }

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

            // Generate a unique file name to save the file under
            QString saveFileAs = Settings::getValue("remote", "clonedirectory").toString() +
                QString("/%2_%1.remotedb").arg(QDateTime::currentMSecsSinceEpoch()).arg(reply->url().fileName());

            // Save the downloaded data under the generated file name
            QFile file(saveFileAs);
            file.open(QIODevice::WriteOnly);
            file.write(reply->readAll());
            file.close();

            // Tell the application to open this file
            emit openFile(saveFileAs);
        }
        break;
    case RequestTypeDirectory:
        emit gotDirList(reply->readAll(), reply->property("userdata"));
        break;
    case RequestTypeNewVersionCheck:
        {
            QString version = reply->readLine().trimmed();
            QString url = reply->readLine().trimmed();
            emit gotCurrentVersion(version, url);
            break;
        }
    default:
        break;
    }

    // Delete reply later, i.e. after returning from this slot function
    m_currentReply = nullptr;
    reply->deleteLater();
}

void RemoteDatabase::gotError(QNetworkReply* reply, const QList<QSslError>& errors)
{
    // Are there any errors in here that aren't about self-signed certificates and non-matching hostnames?
    // TODO What about the hostname mismatch? Can we remove that from the list of ignored errors later?
    bool serious_errors = false;
    foreach(const QSslError& error, errors)
    {
        if(error.error() != QSslError::SelfSignedCertificate && error.error() != QSslError::HostNameMismatch)
        {
            serious_errors = true;
            break;
        }
    }

    // Just stop the error checking here and accept the reply if there were no 'serious' errors
    if(!serious_errors)
    {
        reply->ignoreSslErrors(errors);
        return;
    }

    // Build an error message and short it to the user
    QString message = tr("Error opening remote file at %1.\n%2").arg(reply->url().toString()).arg(errors.at(0).errorString());
    QMessageBox::warning(0, qApp->applicationName(), message);

    // Delete reply later, i.e. after returning from this slot function
    m_progress->reset();
    reply->deleteLater();
}

void RemoteDatabase::updateProgress(qint64 bytesTransmitted, qint64 bytesTotal)
{
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
        m_progress->setMinimum(0);
        m_progress->setMaximum(bytesTotal);
        m_progress->setValue(bytesTransmitted);
    }

    // Check if the Cancel button has been pressed
    qApp->processEvents();
    if(m_currentReply && m_progress->wasCanceled())
    {
        m_currentReply->abort();
        m_progress->reset();
    }
}

const QList<QSslCertificate>& RemoteDatabase::caCertificates() const
{
    static QList<QSslCertificate> certs = m_sslConfiguration.caCertificates();
    return certs;
}

bool RemoteDatabase::prepareSsl(QNetworkRequest* request, const QString& clientCert)
{
    // Check if client cert exists
    const QSslCertificate& cert = m_clientCertFiles[clientCert];
    if(cert.isNull())
    {
        QMessageBox::warning(0, qApp->applicationName(), tr("Error: Invalid client certificate specified."));
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
        QString password = QInputDialog::getText(0, qApp->applicationName(), tr("Please enter the passphrase for this client certificate in order to authenticate."));
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

void RemoteDatabase::prepareProgressDialog(bool upload, const QString& url)
{
    // Instantiate progress dialog and apply some basic settings
    if(!m_progress)
        m_progress = new QProgressDialog();
    m_progress->setWindowModality(Qt::ApplicationModal);
    m_progress->setCancelButtonText(tr("Cancel"));

    // Set dialog text
    if(upload)
        m_progress->setLabelText(tr("Uploading remote database to\n%1.").arg(url));
    else
        m_progress->setLabelText(tr("Downloading remote database from\n%1.").arg(url));

    // Show dialog
    m_progress->show();
    qApp->processEvents();

    // Make sure the dialog is updated
    if(upload)
        connect(m_currentReply, &QNetworkReply::uploadProgress, this, &RemoteDatabase::updateProgress);
    else
        connect(m_currentReply, &QNetworkReply::downloadProgress, this, &RemoteDatabase::updateProgress);
}

void RemoteDatabase::fetch(const QString& url, RequestType type, const QString& clientCert, QVariant userdata)
{
    // Check if network is accessible. If not, abort right here
    if(m_manager->networkAccessible() == QNetworkAccessManager::NotAccessible)
    {
        QMessageBox::warning(0, qApp->applicationName(), tr("Error: The network is not accessible."));
        return;
    }

    // Build network request
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", QString("%1 %2").arg(qApp->organizationName()).arg(APP_VERSION).toUtf8());

    // Set SSL configuration when trying to access a file via the HTTPS protocol.
    // Skip this step when no client certificate was specified. In this case the default HTTPS configuration is used.
    bool https = QUrl(url).scheme().compare("https", Qt::CaseInsensitive) == 0;
    if(https && !clientCert.isNull())
    {
        // If configuring the SSL connection fails, abort the request here
        if(!prepareSsl(&request, clientCert))
            return;
    }

    // Fetch database and save pending reply. Note that we're only supporting one active download here at the moment.
    m_currentReply = m_manager->get(request);
    m_currentReply->setProperty("type", type);
    m_currentReply->setProperty("certfile", clientCert);
    m_currentReply->setProperty("userdata", userdata);

    // Initialise the progress dialog for this request, but only if this is a database file. Directory listing are small enough to be loaded
    // without progress dialog.
    if(type == RequestTypeDatabase)
        prepareProgressDialog(false, url);
}

void RemoteDatabase::push(const QString& filename, const QString& url, const QString& clientCert)
{
    // Check if network is accessible. If not, abort right here
    if(m_manager->networkAccessible() == QNetworkAccessManager::NotAccessible)
    {
        QMessageBox::warning(0, qApp->applicationName(), tr("Error: The network is not accessible."));
        return;
    }

    // Open the file to send and check if it exists
    QFile file(filename);
    if(!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(0, qApp->applicationName(), tr("Error: Cannot open the file for sending."));
        return;
    }

    // Build network request
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", QString("%1 %2").arg(qApp->organizationName()).arg(APP_VERSION).toUtf8());

    // Set SSL configuration when trying to access a file via the HTTPS protocol
    bool https = QUrl(url).scheme().compare("https", Qt::CaseInsensitive) == 0;
    if(https)
    {
        // If configuring the SSL connection fails, abort the request here
        if(!prepareSsl(&request, clientCert))
            return;
    }

    // Get file data
    // TODO: Don't read the entire file here but directly pass the file handle to the put() call below in order
    // to read larger files chunk by chunk.
    QByteArray file_data = file.readAll();
    file.close();

    // Fetch database and save pending reply. Note that we're only supporting one active download here at the moment.
    m_currentReply = m_manager->put(request, file_data);
    m_currentReply->setProperty("type", RequestTypePush);

    // Initialise the progress dialog for this request
    prepareProgressDialog(true, url);
}
