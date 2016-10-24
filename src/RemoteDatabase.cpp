#include <QApplication>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QNetworkReply>
#include <QFile>

#include "RemoteDatabase.h"
#include "version.h"
#include "FileDialog.h"

RemoteDatabase::RemoteDatabase() :
    m_manager(new QNetworkAccessManager),
    m_currentReply(nullptr)
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

    // Fetch database and save pending reply. Note that we're only supporting one active download here at the moment.
    m_currentReply = m_manager->get(request);

    // Initialise the progress dialog for this request
    m_progress.setWindowModality(Qt::ApplicationModal);
    m_progress.setCancelButtonText(tr("Cancel"));
    m_progress.setLabelText(tr("Downloading remote database from\n%1.").arg(url));
    m_progress.show();
    qApp->processEvents();
    connect(m_currentReply, &QNetworkReply::downloadProgress, this, &RemoteDatabase::updateProgress);
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
    m_currentReply = nullptr;
    m_progress.hide();
    reply->deleteLater();
}

void RemoteDatabase::gotError(QNetworkReply* reply, const QList<QSslError>& errors)
{
    // Build an error message and short it to the user
    QString message = tr("Error opening remote database file from %1.\n%2").arg(reply->url().toString()).arg(errors.at(0).errorString());
    QMessageBox::warning(0, qApp->applicationName(), message);

    // Delete reply later, i.e. after returning from this slot function
    m_progress.hide();
    reply->deleteLater();
}

void RemoteDatabase::updateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    // Update progress dialog
    if(bytesTotal == -1)
    {
        // We don't know anything about the current progress, but it's still downloading
        m_progress.setMinimum(0);
        m_progress.setMaximum(0);
        m_progress.setValue(0);
    } else if(bytesReceived == bytesTotal) {
        // The download has finished
        m_progress.hide();
    } else {
        // It's still downloading and we know the current progress
        m_progress.setMinimum(0);
        m_progress.setMaximum(bytesTotal);
        m_progress.setValue(bytesReceived);
    }

    // Check if the Cancel button has been pressed
    qApp->processEvents();
    if(m_currentReply && m_progress.wasCanceled())
    {
        m_currentReply->abort();
        m_progress.hide();
    }
}
