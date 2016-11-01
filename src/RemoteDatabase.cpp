#include <QApplication>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QNetworkReply>
#include <QFile>
#include <QSslKey>
#include <QProgressDialog>

#include "RemoteDatabase.h"
#include "version.h"
#include "FileDialog.h"

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

    // Load client cert
    QFile fileClientCert("client.crt");
    fileClientCert.open(QFile::ReadOnly);
    QSslCertificate clientCert(&fileClientCert);
    fileClientCert.close();
    m_sslConfiguration.setLocalCertificate(clientCert);

    // Load private key
    QFile fileClientKey("client.key");
    fileClientKey.open(QFile::ReadOnly);
    QSslKey clientKey(&fileClientKey, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, "password");
    fileClientKey.close();
    m_sslConfiguration.setPrivateKey(clientKey);

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
    // TODO Add support for proxies here
}

void RemoteDatabase::fetchDatabase(const QString& url)
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

    // Set SSL configuration when trying to access a file via the HTTPS protocol
    bool https = QUrl(url).scheme().compare("https", Qt::CaseInsensitive) == 0;
    if(https)
        request.setSslConfiguration(m_sslConfiguration);

    // Fetch database and save pending reply. Note that we're only supporting one active download here at the moment.
    m_currentReply = m_manager->get(request);

    // Initialise the progress dialog for this request
    if(!m_progress)
        m_progress = new QProgressDialog();
    m_progress->setWindowModality(Qt::ApplicationModal);
    m_progress->setCancelButtonText(tr("Cancel"));
    m_progress->setLabelText(tr("Downloading remote database from\n%1.").arg(url));
    m_progress->show();
    qApp->processEvents();
    connect(m_currentReply, &QNetworkReply::downloadProgress, this, &RemoteDatabase::updateProgress);
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
                             tr("Error opening remote database file from %1.\n%2").arg(reply->url().toString()).arg(reply->errorString()));
        reply->deleteLater();
        return;
    }

    // Ask user where to store the database file
    QString saveFileAs = FileDialog::getSaveFileName(0, qApp->applicationName(), FileDialog::getSqlDatabaseFileFilter(), reply->url().fileName());
    if(!saveFileAs.isEmpty())
    {
        // Save the downloaded data under the selected file name
        QFile file(saveFileAs);
        file.open(QIODevice::WriteOnly);
        file.write(reply->readAll());
        file.close();

        // Tell the application to open this file
        emit openFile(saveFileAs);
    }

    // Delete reply later, i.e. after returning from this slot function
    m_currentReply = nullptr;
    m_progress->hide();
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
    QString message = tr("Error opening remote database file from %1.\n%2").arg(reply->url().toString()).arg(errors.at(0).errorString());
    QMessageBox::warning(0, qApp->applicationName(), message);

    // Delete reply later, i.e. after returning from this slot function
    m_progress->hide();
    reply->deleteLater();
}

void RemoteDatabase::updateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    // Update progress dialog
    if(bytesTotal == -1)
    {
        // We don't know anything about the current progress, but it's still downloading
        m_progress->setMinimum(0);
        m_progress->setMaximum(0);
        m_progress->setValue(0);
    } else if(bytesReceived == bytesTotal) {
        // The download has finished
        m_progress->hide();
    } else {
        // It's still downloading and we know the current progress
        m_progress->setMinimum(0);
        m_progress->setMaximum(bytesTotal);
        m_progress->setValue(bytesReceived);
    }

    // Check if the Cancel button has been pressed
    qApp->processEvents();
    if(m_currentReply && m_progress->wasCanceled())
    {
        m_currentReply->abort();
        m_progress->hide();
    }
}

const QList<QSslCertificate>& RemoteDatabase::caCertificates() const
{
    static QList<QSslCertificate> certs = m_sslConfiguration.caCertificates();
    return certs;
}
