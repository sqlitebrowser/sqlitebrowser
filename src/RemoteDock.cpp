#include <QDesktopServices>
#include <QFileInfo>
#include <QUrl>

#include "RemoteDock.h"
#include "ui_RemoteDock.h"
#include "Settings.h"
#include "RemoteDatabase.h"
#include "RemoteModel.h"
#include "MainWindow.h"
#include "RemotePushDialog.h"
#include "PreferencesDialog.h"

RemoteDock::RemoteDock(MainWindow* parent)
    : QDialog(parent),
      ui(new Ui::RemoteDock),
      mainWindow(parent),
      remoteDatabase(parent->getRemote()),
      remoteModel(new RemoteModel(this, parent->getRemote()))
{
    ui->setupUi(this);

    // Set up model
    ui->treeStructure->setModel(remoteModel);

    // Reload the directory tree when a database upload has finished
    connect(&remoteDatabase, &RemoteDatabase::uploadFinished, remoteModel, &RemoteModel::refresh);

    // Whenever a new directory listing has been parsed, check if it was a new root dir and, if so, open the user's directory
    connect(remoteModel, &RemoteModel::directoryListingParsed, this, &RemoteDock::newDirectoryNode);

    // When the Preferences link is clicked in the no-certificates-label, open the preferences dialog. For other links than the ones we know,
    // just open them in a web browser
    connect(ui->labelNoCert, &QLabel::linkActivated, [this](const QString& link) {
        if(link == "#preferences")
        {
            PreferencesDialog dialog(mainWindow, PreferencesDialog::TabRemote);
            if(dialog.exec())
                mainWindow->reloadSettings();
        } else {
            QDesktopServices::openUrl(QUrl(link));
        }
    });

    // Initial setup
    reloadSettings();
}

RemoteDock::~RemoteDock()
{
    delete ui;
}

void RemoteDock::reloadSettings()
{
    // Clear list of client certificates and add a dummy entry which does nothing except serve as
    // an explanation to the user.
    ui->comboUser->clear();
    ui->comboUser->addItem(tr("Select an identity to connect"), "dummy");

    // Load list of client certs
    QStringList client_certs = Settings::getValue("remote", "client_certificates").toStringList();
    for(const QString& file : client_certs)
    {
        auto certs = QSslCertificate::fromPath(file);
        for(const QSslCertificate& cert : certs)
            ui->comboUser->addItem(cert.subjectInfo(QSslCertificate::CommonName).at(0), file);
    }

    // Add public certificate for anonymous read-only access to dbhub.io
    ui->comboUser->addItem(tr("Public"), ":/user_certs/public.cert.pem");
}

void RemoteDock::setNewIdentity(const QString& identity)
{
    // Do nothing if the dummy entry was selected
    if(ui->comboUser->currentData() == "dummy")
        return;

    // Check if the dummy item is still there and remove it if it is
    if(ui->comboUser->itemData(0) == "dummy")
        ui->comboUser->removeItem(0);

    // Get certificate file name
    QString cert = ui->comboUser->itemData(ui->comboUser->findText(identity), Qt::UserRole).toString();
    if(cert.isEmpty())
        return;

    // Open root directory. Get host name from client cert
    QString host = remoteDatabase.getInfoFromClientCert(cert, RemoteDatabase::CertInfoServer);
    remoteModel->setNewRootDir(QString("https://%1:443/").arg(host), cert);

    // Enable buttons if necessary
    enableButtons();
}

void RemoteDock::fetchDatabase(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;

    // Get item
    const RemoteModelItem* item = remoteModel->modelIndexToItem(idx);

    // Only open database file
    if(item->value(RemoteModelColumnType).toString() == "database")
        remoteDatabase.fetch(item->value(RemoteModelColumnUrl).toString(), RemoteDatabase::RequestTypeDatabase, remoteModel->currentClientCertificate());
}

void RemoteDock::enableButtons()
{
    bool db_opened = mainWindow->getDb().isOpen();
    bool logged_in = !remoteModel->currentClientCertificate().isEmpty();

    ui->buttonPushDatabase->setEnabled(db_opened && logged_in);
}

void RemoteDock::pushDatabase()
{
    // If the currently active identity is the read-only public access to dbhub.io, don't show the Push Database dialog because it won't work anyway.
    // Instead switch to an explanation offering some advice to create and import a proper certificate.
    if(remoteModel->currentClientCertificate() == ":/user_certs/public.cert.pem")
    {
        ui->stack->setCurrentIndex(1);
        return;
    }

    // The default suggestion for a database name is the local file name. If it is a remote file (like when it initially was fetched using DB4S),
    // the extra bit of information at the end of the name gets removed first.
    QString name = QFileInfo(mainWindow->getDb().currentFile()).fileName();
    name = name.remove(QRegExp("_[0-9]+.remotedb$"));

    // Show the user a dialog for setting all the commit details
    QString host = QString("https://%1:443/").arg(remoteDatabase.getInfoFromClientCert(remoteModel->currentClientCertificate(), RemoteDatabase::CertInfoServer));
    RemotePushDialog pushDialog(this, remoteDatabase, host, remoteModel->currentClientCertificate(), name);
    if(pushDialog.exec() != QDialog::Accepted)
        return;

    // Build push URL
    QString url = host;
    url.append(remoteDatabase.getInfoFromClientCert(remoteModel->currentClientCertificate(), RemoteDatabase::CertInfoUser));
    url.append("/");
    url.append(pushDialog.name());

    // Push database
    remoteDatabase.push(mainWindow->getDb().currentFile(), url, remoteModel->currentClientCertificate(), pushDialog.name(),
                        pushDialog.commitMessage(), pushDialog.licence(), pushDialog.isPublic(), pushDialog.branch(), pushDialog.forcePush());
}

void RemoteDock::newDirectoryNode(const QModelIndex& parent)
{
    // Was this a new root dir?
    if(!parent.isValid())
    {
        // Then check if there is a directory with the current user name

        // Get current user name
        QString user = remoteDatabase.getInfoFromClientCert(remoteModel->currentClientCertificate(), RemoteDatabase::CertInfoUser);

        for(int i=0;i<remoteModel->rowCount(parent);i++)
        {
            QModelIndex child = remoteModel->index(i, RemoteModelColumnName, parent);
            if(child.data().toString() == user)
                ui->treeStructure->expand(child);
        }
    }
}

void RemoteDock::reject()
{
    // We override this, to ensure the Escape key doesn't make this dialog
    // dock go away
    return;
}

void RemoteDock::switchToMainView()
{
    ui->stack->setCurrentIndex(0);
}
