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
#include <QStandardPaths>
#include <QUrlQuery>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QNetworkProxyFactory>
#include <QTimeZone>
#include <QtNetwork/QNetworkProxy>
#include <json.hpp>

#include <iterator>

#include "RemoteDatabase.h"
#include "version.h"
#include "Settings.h"
#include "sqlite.h"

using json = nlohmann::json;

RemoteDatabase::RemoteDatabase() :
    m_manager(new QNetworkAccessManager),
    m_configurationManager(new QNetworkConfigurationManager),
    m_progress(nullptr),
    m_dbLocal(nullptr)
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
    connect(m_manager, &QNetworkAccessManager::finished, this, &RemoteDatabase::gotReply);
    connect(m_manager, &QNetworkAccessManager::encrypted, this, &RemoteDatabase::gotEncrypted);
    connect(m_manager, &QNetworkAccessManager::sslErrors, this, &RemoteDatabase::gotError);
}

RemoteDatabase::~RemoteDatabase()
{
    delete m_manager;
    delete m_progress;

    // Close local storage db - but only if it was created/opened in the meantime
    if(m_dbLocal)
        sqlite3_close(m_dbLocal);
}

void RemoteDatabase::reloadSettings()
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

void RemoteDatabase::gotEncrypted(QNetworkReply* reply)
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

void RemoteDatabase::gotReply(QNetworkReply* reply)
{
    // Check if request was successful
    if(reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::warning(nullptr, qApp->applicationName(),
                             tr("Error when connecting to %1.\n%2").arg(reply->url().toString(), reply->errorString()));
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

            // Add cloned database to list of local databases
            QString saveFileAs = localAdd(reply->url().fileName(), reply->property("certfile").toString(),
                                          reply->url(), QUrlQuery(reply->url()).queryItemValue("commit").toStdString());

            // Save the downloaded data under the generated file name
            QFile file(saveFileAs);
            file.open(QIODevice::WriteOnly);
            file.write(reply->readAll());

            // Set last modified data of the new file to the one provided by the server
            // Before version 5.10, Qt didn't offer any option to set this attribute, so we're not setting it at the moment
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
            QString last_modified = reply->rawHeader("Content-Disposition");
            QRegExp regex("^.*modification-date=\"(.+)\";.*$");
            regex.setMinimal(true); // Set to non-greedy matching
            if(regex.indexIn(last_modified) != -1)
            {
                last_modified = regex.cap(1);
                bool success = file.setFileTime(QDateTime::fromString(last_modified, Qt::ISODate), QFileDevice::FileModificationTime);
                if(!success)
                    qWarning() << file.errorString();
            }
#endif

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
    case RequestTypeLicenceList:
        {
            // Read and check results
            json obj = json::parse(reply->readAll(), nullptr, false);
            if(obj.is_discarded() || !obj.is_object())
                break;

            // Parse data and build ordered licence map: order -> (short name, long name)
            std::map<int, std::pair<std::string, std::string>> licences;
            for(auto it=obj.cbegin();it!=obj.cend();++it)
                licences.insert({it.value()["order"], {it.key(), it.value()["full_name"]}});

            // Convert the map into an ordered vector and send it to anyone who's interested
            std::vector<std::pair<std::string, std::string>> licence_list;
            std::transform(licences.begin(), licences.end(), std::back_inserter(licence_list), [](const std::pair<int, std::pair<std::string, std::string>>& it) {
                return it.second;
            });
            emit gotLicenceList(licence_list);
            break;
        }
    case RequestTypeBranchList:
        {
            // Read and check results
            json obj = json::parse(reply->readAll(), nullptr, false);
            if(obj.is_discarded() || !obj.is_object())
                break;
            json obj_branches = obj["branches"];

            // Parse data and assemble branch list
            std::vector<std::string> branches;
            for(auto it=obj_branches.cbegin();it!=obj_branches.cend();++it)
                branches.push_back(it.key());

            // Get default branch
            std::string default_branch = (obj.contains("default_branch") && !obj["default_branch"].empty()) ? obj["default_branch"] : "master";

            // Send branch list to anyone who is interested
            emit gotBranchList(branches, default_branch);
            break;
        }
    case RequestTypePush:
        {
            // Read and check results
            json obj = json::parse(reply->readAll(), nullptr, false);
            if(obj.is_discarded() || !obj.is_object())
                break;

            // Create or update the record in our local checkout database
            QString saveFileAs = localAdd(reply->url().fileName(), reply->property("certfile").toString(), QString::fromStdString(obj["url"]), obj["commit_id"]);

            // If the name of the source file and the name we're saving as differ, we're doing an initial push. In this case, copy the source file to
            // the destination path to avoid redownloading it when it's first used.
            if(saveFileAs != reply->property("source_file").toString())
                QFile::copy(reply->property("source_file").toString(), saveFileAs);

            emit uploadFinished(obj["url"]);
            break;
        }
    }

    // Delete reply later, i.e. after returning from this slot function
    reply->deleteLater();
}

void RemoteDatabase::gotError(QNetworkReply* reply, const QList<QSslError>& errors)
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

void RemoteDatabase::updateProgress(qint64 bytesTransmitted, qint64 bytesTotal)
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

const QList<QSslCertificate>& RemoteDatabase::caCertificates() const
{
    static QList<QSslCertificate> certs = m_sslConfiguration.caCertificates();
    return certs;
}

QString RemoteDatabase::getInfoFromClientCert(const QString& cert, CertInfo info) const
{
    // Get the common name of the certificate and split it into user name and server address
    QString cn = m_clientCertFiles.at(cert).subjectInfo(QSslCertificate::CommonName).at(0);
    QStringList cn_parts = cn.split("@");
    if(cn_parts.size() < 2)
        return QString();

    // Return requested part of the CN
    if(info == CertInfoUser)
        return cn_parts.first();
    else if(info == CertInfoServer)
        return cn_parts.last();
    else
        return QString();
}

bool RemoteDatabase::prepareSsl(QNetworkRequest* request, const QString& clientCert)
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

void RemoteDatabase::prepareProgressDialog(QNetworkReply* reply, bool upload, const QString& url)
{
    // Instantiate progress dialog and apply some basic settings
    if(!m_progress)
        m_progress = new QProgressDialog();
    m_progress->reset();
    m_progress->setWindowModality(Qt::NonModal);
    m_progress->setCancelButtonText(tr("Cancel"));

    // Set dialog text
    if(upload)
        m_progress->setLabelText(tr("Uploading remote database to\n%1").arg(url));
    else
        m_progress->setLabelText(tr("Downloading remote database from\n%1").arg(url));

    // Show dialog
    m_progress->show();

    // Make sure the dialog is updated
    if(upload)
        connect(reply, &QNetworkReply::uploadProgress, this, &RemoteDatabase::updateProgress);
    else
        connect(reply, &QNetworkReply::downloadProgress, this, &RemoteDatabase::updateProgress);
}

void RemoteDatabase::fetch(const QString& url, RequestType type, const QString& clientCert, QVariant userdata)
{
    // Check if network is accessible. If not, abort right here
    if(m_manager->networkAccessible() == QNetworkAccessManager::NotAccessible)
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), tr("Error: The network is not accessible."));
        return;
    }

    // If this is a request for a database there is a chance that we've already cloned that database. So check for that first
    if(type == RequestTypeDatabase)
    {
        QString exists = localExists(url, clientCert);
        if(!exists.isEmpty())
        {
            // Database has already been cloned! So open the local file instead of fetching the one from the
            // server again.
            emit openFile(exists);
            return;
        }
    }

    // Build network request
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", QString("%1 %2").arg(qApp->organizationName(), APP_VERSION).toUtf8());

    // Set SSL configuration when trying to access a file via the HTTPS protocol.
    // Skip this step when no client certificate was specified. In this case the default HTTPS configuration is used.
    bool https = QUrl(url).scheme().compare("https", Qt::CaseInsensitive) == 0;
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
    reply->setProperty("userdata", userdata);

    // Initialise the progress dialog for this request, but only if this is a database file. Directory listing are small enough to be loaded
    // without progress dialog.
    if(type == RequestTypeDatabase)
        prepareProgressDialog(reply, false, url);
}

void RemoteDatabase::push(const QString& filename, const QString& url, const QString& clientCert, const QString& remotename,
                          const QString& commitMessage, const QString& licence, bool isPublic, const QString& branch, bool forcePush)
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
    QDateTime last_modified = QFileInfo(filename).lastModified();
    last_modified.toOffsetFromUtc(0);

    // Prepare HTTP multi part data containing all the information about the commit we're about to push
    QHttpMultiPart* multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    addPart(multipart, "file", file, remotename);
    addPart(multipart, "commitmsg", commitMessage);
    addPart(multipart, "licence", licence);
    addPart(multipart, "public", isPublic ? "true" : "false");
    addPart(multipart, "branch", branch);
    addPart(multipart, "commit", QString::fromStdString(localLastCommitId(clientCert, url)));
    addPart(multipart, "force", forcePush ? "true" : "false");
    addPart(multipart, "lastmodified", last_modified.toString("yyyy-MM-dd'T'HH:mm:ss'Z'"));

    // Set SSL configuration when trying to access a file via the HTTPS protocol
    bool https = QUrl(url).scheme().compare("https", Qt::CaseInsensitive) == 0;
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

void RemoteDatabase::addPart(QHttpMultiPart* multipart, const QString& name, const QString& value) const
{
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%1\"").arg(name));
    part.setBody(value.toUtf8());

    multipart->append(part);
}

void RemoteDatabase::addPart(QHttpMultiPart* multipart, const QString& name, QFile* file, const QString& filename) const
{
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%1\"; filename=\"%2\"").arg(name, filename));
    part.setBodyDevice(file);
    file->setParent(multipart);     // Close the file and delete the file object as soon as the multi-part object is destroyed

    multipart->append(part);
}

void RemoteDatabase::localAssureOpened()
{
    // This function should be called first in each RemoteDatabase::local* function. It assures the database for storing
    // the local database information is opened and ready. If the database file doesn't exist yet it is created by this
    // function. If the database file is already created and opened this function does nothing. The reason to open the
    // database on first use instead of doing that in the constructor of this class is that this way no database file is
    // going to be created and no database handle is held when it's not actually needed. For people not interested in
    // the dbhub.io functionality this means no unnecessary files being created.

    // Check if database is already opened and return if it is
    if(m_dbLocal)
        return;

    // Open file
    QString database_file = QStandardPaths::writableLocation(
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
                QStandardPaths::AppDataLocation
#else
                QStandardPaths::GenericDataLocation
#endif
                ) + "/remotedbs.db";
    if(sqlite3_open_v2(database_file.toUtf8(), &m_dbLocal, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) != SQLITE_OK)
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), tr("Error opening local databases list.\n%1").arg(QString::fromUtf8(sqlite3_errmsg(m_dbLocal))));
        return;
    }

    // Create local local table if it doesn't exists yet
    char* errmsg;
    QString statement = QString("CREATE TABLE IF NOT EXISTS \"local\"("
                                "\"id\" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                                "\"identity\" TEXT NOT NULL,"
                                "\"name\" TEXT NOT NULL,"
                                "\"url\" TEXT NOT NULL,"
                                "\"commit_id\" TEXT NOT NULL,"
                                "\"file\" TEXT NOT NULL UNIQUE,"
                                "\"modified\" INTEGER DEFAULT 0,"
                                "\"branch\" TEXT NOT NULL DEFAULT \"master\""
                                ")");
    if(sqlite3_exec(m_dbLocal, statement.toUtf8(), nullptr, nullptr, &errmsg) != SQLITE_OK)
    {
        QMessageBox::warning(nullptr, qApp->applicationName(), tr("Error creating local databases list.\n%1").arg(QString::fromUtf8(errmsg)));
        sqlite3_free(errmsg);
        sqlite3_close(m_dbLocal);
        m_dbLocal = nullptr;
        return;
    }
}

QString RemoteDatabase::localAdd(QString filename, QString identity, const QUrl& url, const std::string& new_commit_id)
{
    // This function adds a new local database clone to our internal list. It does so by adding a single
    // new record to the remote dbs database. All the fields are extracted from the filename, the identity
    // and (most importantly) the url parameters. Note that for the commit id field to be correctly filled we
    // require the commit id to be part of the url parameter. Also note that this function doesn't care if the
    // database has already been added to the list or not. If you need this information you need to check it before
    // calling this function, ideally even before sending out a request to the network. The function returns the full
    // path of the newly created/updated file.

    localAssureOpened();

    // Remove the path
    QFileInfo f(identity);
    identity = f.fileName();

    // Check if this file has already been checked in
    std::string last_commit_id = localLastCommitId(identity, url.toString());
    if(last_commit_id.empty())
    {
        // The file hasn't been checked in yet. So add a new record for it.

        // Generate a new file name to save the file under
        filename = QString("%2_%1.remotedb").arg(QDateTime::currentMSecsSinceEpoch()).arg(filename);

        // Insert database into local database list
        QString sql = QString("INSERT INTO local(identity, name, url, commit_id, file) VALUES(?, ?, ?, ?, ?)");
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(m_dbLocal, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
            return QString();

        if(sqlite3_bind_text(stmt, 1, identity.toUtf8(), identity.toUtf8().length(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 2, url.fileName().toUtf8(), url.fileName().toUtf8().length(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 3, url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8(),
                             url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8().length(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 4, new_commit_id.c_str(), static_cast<int>(new_commit_id.size()), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 5, filename.toUtf8(), filename.size(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        sqlite3_finalize(stmt);

        // Return full path to the new file
        return Settings::getValue("remote", "clonedirectory").toString() + "/" + filename;
    }

    // If we get here, the file has been checked in before. Check next if it has been updated in the meantime.
    if(last_commit_id != new_commit_id)
    {
        // The file has already been checked in and the commit ids are different. If they weren't we wouldn't need to update anything

        QString sql = QString("UPDATE local SET commit_id=? WHERE identity=? AND url=?");
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(m_dbLocal, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
            return QString();

        if(sqlite3_bind_text(stmt, 1, new_commit_id.c_str(), static_cast<int>(new_commit_id.size()), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 2, identity.toUtf8(), identity.toUtf8().length(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_bind_text(stmt, 3, url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8(),
                             url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8().length(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            return QString();
        }

        sqlite3_finalize(stmt);
    }

    // If we got here, the file was already checked in (and was either updated or not (obviously)). This mean we can just return the file name as
    // we know it.
    return localExists(url, identity);
}

QString RemoteDatabase::localExists(const QUrl& url, QString identity)
{
    // This function checks if there already is a clone for the given combination of url and identity. It returns the filename
    // of this clone if there is or a null string if there isn't a clone yet. The identity needs to be part of this check because
    // with the url alone there could be corner cases where different versions or whatever may not be accessible for all users.

    localAssureOpened();

    // Extract commit id from url and remove query part afterwards
    QString url_commit_id = QUrlQuery(url).queryItemValue("commit");

    // Query commit id and filename for the given combination of url and identity
    QString sql = QString("SELECT id, commit_id, file FROM local WHERE url=? AND identity=?");
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(m_dbLocal, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
        return QString();

    if(sqlite3_bind_text(stmt, 1, url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8(), url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8().length(), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return QString();
    }

    QFileInfo f(identity);                  // Remove the path
    identity = f.fileName();
    if(sqlite3_bind_text(stmt, 2, identity.toUtf8(), identity.toUtf8().length(), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return QString();
    }

    if(sqlite3_step(stmt) != SQLITE_ROW)
    {
        // If there was either an error or no record was found for this combination of url and
        // identity, stop here.
        sqlite3_finalize(stmt);
        return QString();
    }

    // Having come here we can assume that at least some local clone for the given combination of
    // url and identity exists. So extract all the information we have on it.
    //int local_id = sqlite3_column_int(stmt, 0);
    QString local_commit_id = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    QString local_file = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    sqlite3_finalize(stmt);

    // There are three possibilities now: either we didn't get any commit id in the URL in which case we just return the file we got, no matter what.
    // Or the requested commit id is the same as the local commit id in which case we return the file we got as well.
    // Or the requested commit id differ in which case we ask the user what to do.
    if(url_commit_id.isNull() || local_commit_id == url_commit_id)
    {
        // Both commit ids are the same. That's the perfect match, so we can download the local file if it still exists
        return localCheckFile(local_file);
    } else {
        // The commit ids differ. That means we have another version locally checked out than we're trying to download. Because the commit ids are
        // only calculated on the server side and we're currently always checking out the latest version this can only mean that the remote version has
        // been updated, i.e. is newer than the local version.

        // TODO Support multiple checkouts of the same database at different versions at the same time. For this we need to be more intelligent with
        // comparing the commit ids.

        // Ask the user what to do: open the local version or updating to the new remote version
        if(QMessageBox::question(nullptr, qApp->applicationName(),
                                 tr("The remote database has been updated since the last checkout. Do you want to update the local database to the newest version? Note "
                                    "that this discards any changes you have made locally! If you don't want to lose local changes, click No to open the local version."),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            // User wants to download the newest version. So delete the entry from the clones database and delete the local database copy and return an empty
            // string to indicate a redownload request.

            // Build full path to database file and delete it
            QFile::remove(Settings::getValue("remote", "clonedirectory").toString() + "/" + local_file);

            // Remove the old entry from the local clones database to enforce a redownload. The file column should be unique for the entire table because the
            // files are all in the same directory and their names need to be unique because of this.
            QString delete_sql = QString("DELETE FROM local WHERE file=?");
            sqlite3_stmt* delete_stmt;
            if(sqlite3_prepare_v2(m_dbLocal, delete_sql.toUtf8(), -1, &delete_stmt, nullptr) != SQLITE_OK)
                return QString();
            if(sqlite3_bind_text(delete_stmt, 1, local_file.toUtf8(), local_file.toUtf8().length(), SQLITE_TRANSIENT))
            {
                sqlite3_finalize(delete_stmt);
                return QString();
            }
            sqlite3_step(delete_stmt);
            sqlite3_finalize(delete_stmt);

            // Return an empty string to indicate a redownload request
            return QString();
        } else {
            // User wants to open the local version. So build the full path and return it if the file still exists.
            return localCheckFile(local_file);
        }
    }
}

QString RemoteDatabase::localCheckFile(const QString& local_file)
{
    // This function takes the file name of a locally cloned database and checks if this file still exists. If it has been deleted in the meantime it returns
    // an empty string and deletes the file from the clone database. If the file still exists, it returns the full path to the file.

    localAssureOpened();

    // Build the full path to where the file should be
    QString full_path = Settings::getValue("remote", "clonedirectory").toString() + "/" + local_file;

    // Check if the database still exists. If so return its path, if not return an empty string to redownload it
    if(QFile::exists(full_path))
    {
        return full_path;
    } else {
        // Remove the apparently invalid entry from the local clones database to avoid future lookups and confusions. The file column should
        // be unique for the entire table because the files are all in the same directory and their names need to be unique because of this.
        QString sql = QString("DELETE FROM local WHERE file=?");
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(m_dbLocal, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
            return QString();
        if(sqlite3_bind_text(stmt, 1, local_file.toUtf8(), local_file.toUtf8().length(), SQLITE_TRANSIENT))
        {
            sqlite3_finalize(stmt);
            return QString();
        }
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        // Return empty string to indicate a redownload request
        return QString();
    }
}

std::string RemoteDatabase::localLastCommitId(QString identity, const QUrl& url)
{
    // This function takes a file name and checks with which commit id we had checked out this file or last pushed it.

    localAssureOpened();

    // Query commit id for that file name
    QString sql = QString("SELECT commit_id FROM local WHERE identity=? AND url=?");
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(m_dbLocal, sql.toUtf8(), -1, &stmt, nullptr) != SQLITE_OK)
        return std::string();

    QFileInfo f(identity);                  // Remove the path
    identity = f.fileName();
    if(sqlite3_bind_text(stmt, 1, identity.toUtf8(), identity.toUtf8().length(), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return std::string();
    }

    if(sqlite3_bind_text(stmt, 2, url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8(),
                         url.toString(QUrl::PrettyDecoded | QUrl::RemoveQuery).toUtf8().size(), SQLITE_TRANSIENT))
    {
        sqlite3_finalize(stmt);
        return std::string();
    }

    if(sqlite3_step(stmt) != SQLITE_ROW)
    {
        // If there was either an error or no record was found for this file name, stop here.
        sqlite3_finalize(stmt);
        return std::string();
    }

    // Having come here we can assume that at least some local clone with the given file name
    std::string local_commit_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);

    return local_commit_id;
}

void RemoteDatabase::clearAccessCache(const QString& clientCert)
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
