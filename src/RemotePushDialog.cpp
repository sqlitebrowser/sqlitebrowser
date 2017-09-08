#include <QPushButton>
#include <QUrlQuery>

#include "RemotePushDialog.h"
#include "ui_RemotePushDialog.h"
#include "RemoteDatabase.h"

RemotePushDialog::RemotePushDialog(QWidget* parent, RemoteDatabase& remote, const QString& host, const QString& clientCert, const QString& name) :
    QDialog(parent),
    ui(new Ui::RemotePushDialog),
    m_host(host),
    m_clientCert(clientCert),
    remoteDatabase(remote)
{
    // Create UI
    ui->setupUi(this);

    // Set start values
    ui->editName->setText(name);

    // Enable/disable accept button
    checkInput();

    // Fetch list of available licences
    connect(&remoteDatabase, &RemoteDatabase::gotLicenceList, this, &RemotePushDialog::fillInLicences);
    remoteDatabase.fetch(host + "licence/list", RemoteDatabase::RequestTypeLicenceList, clientCert);

    // Prepare fetching list of available branches
    connect(&remoteDatabase, &RemoteDatabase::gotBranchList, this, &RemotePushDialog::fillInBranches);
    reloadBranchList();
}

RemotePushDialog::~RemotePushDialog()
{
    delete ui;
}

void RemotePushDialog::checkInput()
{
    // Update public/private check box text
    if(ui->checkPublic->isChecked())
        ui->checkPublic->setText(tr("Database will be public. Everyone has read access to it."));
    else
        ui->checkPublic->setText(tr("Database will be private. Only you have access to it."));

    // Check input
    bool valid = true;

    if(ui->editName->text().trimmed().isEmpty())
        valid = false;

    if(ui->editCommitMessage->toPlainText().size() > 1024)
        valid = false;

    if(ui->comboBranch->currentText().size() < 1 || ui->comboBranch->currentText().size() > 32)
        valid = false;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

void RemotePushDialog::accept()
{
    QDialog::accept();
}

QString RemotePushDialog::name() const
{
    return ui->editName->text().trimmed();
}

QString RemotePushDialog::commitMessage() const
{
    return ui->editCommitMessage->toPlainText().trimmed();
}

QString RemotePushDialog::licence() const
{
    return ui->comboLicence->currentData(Qt::UserRole).toString();
}

bool RemotePushDialog::isPublic() const
{
    return ui->checkPublic->isChecked();
}

QString RemotePushDialog::branch() const
{
    return ui->comboBranch->currentText();
}

void RemotePushDialog::fillInLicences(const QMap<QString, QString>& licences)
{
    // Clear licence list and add default item for unspecified licence
    ui->comboLicence->clear();
    ui->comboLicence->addItem(tr("Unspecified"), QString());

    // Parse licence list and fill combo box. Show the full name to the user and use the short name as user data.
    for(auto it=licences.constBegin();it!=licences.constEnd();++it)
        ui->comboLicence->addItem(it.value(), it.key());
}

void RemotePushDialog::fillInBranches(const QStringList& branches, const QString& default_branch)
{
    // Clear branch list and add the default branch
    ui->comboBranch->clear();
    ui->comboBranch->addItem(default_branch);

    // Add rest of the branch list to the combo box
    foreach(const QString& branch, branches)
    {
        if(branch != default_branch)
            ui->comboBranch->addItem(branch);
    }
}

void RemotePushDialog::reloadBranchList()
{
    // Assemble query URL
    QUrl url(m_host + "branch/list");
    QUrlQuery query;
    query.addQueryItem("username", remoteDatabase.getInfoFromClientCert(m_clientCert, RemoteDatabase::CertInfoUser));
    query.addQueryItem("folder", "/");
    query.addQueryItem("dbname", ui->editName->text());
    url.setQuery(query);

    // Send request
    remoteDatabase.fetch(url.toString(), RemoteDatabase::RequestTypeBranchList, m_clientCert);
}
