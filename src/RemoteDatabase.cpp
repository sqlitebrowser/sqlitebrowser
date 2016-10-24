#include <QApplication>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QNetworkReply>
#include <QFile>

#include "RemoteDatabase.h"
#include "version.h"
#include "FileDialog.h"

RemoteDatabase::RemoteDatabase() :
    m_manager(new QNetworkAccessManager)
{
    // TODO Set up SSL configuration here

    // TODO Add support for proxies here

    // Set up signals
    connect(m_manager, &QNetworkAccessManager::finished, this, &RemoteDatabase::gotReply);
    connect(m_manager, &QNetworkAccessManager::encrypted, this, &RemoteDatabase::gotEncrypted);
    connect(m_manager, &QNetworkAccessManager::sslErrors, this, &RemoteDatabase::gotError);
}

RemoteDatabase::~RemoteDatabase()
{
    delete m_manager;
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

    // TODO Set SSL configuration here

    // Fetch database
    m_manager->get(request);

    // TODO Monitor and show download progress. Also add option to cancel running downloads
}

void RemoteDatabase::gotEncrypted(QNetworkReply* /*reply*/)
{
    // TODO Check SSL configuration here and abort reply if it's not good
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
    reply->deleteLater();
}

void RemoteDatabase::gotError(QNetworkReply* reply, const QList<QSslError>& errors)
{
    // Build an error message and short it to the user
    QString message = tr("Error opening remote database file from %1.\n%2").arg(reply->url().toString()).arg(errors.at(0).errorString());
    QMessageBox::warning(0, qApp->applicationName(), message);

    // Delete reply later, i.e. after returning from this slot function
    reply->deleteLater();
}
