#include <QSslCertificate>

#include "RemoteDock.h"
#include "ui_RemoteDock.h"
#include "Settings.h"
#include "RemoteDatabase.h"
#include "RemoteModel.h"
#include "MainWindow.h"

RemoteDock::RemoteDock(MainWindow* parent)
    : QDialog(parent),
      ui(new Ui::RemoteDock),
      remoteDatabase(parent->getRemote()),
      remoteModel(new RemoteModel(this, parent->getRemote()))
{
    ui->setupUi(this);

    // Set up model
    ui->treeStructure->setModel(remoteModel);

    // Initial setup
    reloadSettings();
}

RemoteDock::~RemoteDock()
{
    delete ui;
}

void RemoteDock::reloadSettings()
{
    // Load list of client certs
    ui->comboUser->clear();
    QStringList client_certs = Settings::getSettingsValue("remote", "client_certificates").toStringList();
    foreach(const QString& file, client_certs)
    {
        auto certs = QSslCertificate::fromPath(file);
        foreach(const QSslCertificate& cert, certs)
            ui->comboUser->addItem(cert.subjectInfo(QSslCertificate::CommonName).at(0), file);
    }
}

void RemoteDock::setNewIdentity()
{
    // Get identity
    QString identity = ui->comboUser->currentText();
    if(identity.isEmpty())
        return;

    // Get certificate file name
    QString cert = ui->comboUser->itemData(ui->comboUser->findText(identity), Qt::UserRole).toString();
    if(cert.isEmpty())
        return;

    // Open root directory. Get host name from client cert
    QString cn = remoteDatabase.clientCertificates()[cert].subjectInfo(QSslCertificate::CommonName).at(0);
    QStringList cn_parts = cn.split("@");
    if(cn_parts.size() < 2)
        return;
    remoteModel->setNewRootDir(QString("https://%1:5550/").arg(cn_parts.last()), cert);
}

void RemoteDock::fetchDatabase(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;

    // Get item
    const RemoteModelItem* item = remoteModel->modelIndexToItem(idx);

    // Only open database file
    if(item->value(RemoteModelColumnType).toString() == "database")
        remoteDatabase.fetch(item->value(RemoteModelColumnUrl).toString(), true, remoteModel->currentClientCertificate());
}
