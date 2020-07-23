#include <QPushButton>
#include <QUrlQuery>
#include <QRegExpValidator>

#include "RemotePushDialog.h"
#include "ui_RemotePushDialog.h"
#include "RemoteNetwork.h"

RemotePushDialog::RemotePushDialog(QWidget* parent, const QString& host, const QString& clientCert,
                                   const QString& name, const QString& branch) :
    QDialog(parent),
    ui(new Ui::RemotePushDialog),
    m_host(host),
    m_clientCert(clientCert),
    m_suggestedBranch(branch),
    m_nameValidator(new QRegExpValidator(QRegExp("^[a-z,A-Z,0-9,\\.,\\-,\\_,\\(,\\),\\+,\\ ]+$"), this)),
    m_branchValidator(new QRegExpValidator(QRegExp("^[a-z,A-Z,0-9,\\^,\\.,\\-,\\_,\\/,\\(,\\),\\:,\\&,\\ )]+$"), this))
{
    // Create UI
    ui->setupUi(this);
    ui->editName->setValidator(m_nameValidator);
    ui->comboBranch->setValidator(m_branchValidator);

    // Set start values
    ui->editName->setText(name);

    // Enable/disable accept button
    checkInput();

    // Fetch list of available licences
    connect(&RemoteNetwork::get(), &RemoteNetwork::gotLicenceList, this, &RemotePushDialog::fillInLicences);
    RemoteNetwork::get().fetch(host + "licence/list", RemoteNetwork::RequestTypeLicenceList, clientCert);

    // Prepare fetching list of available branches
    connect(&RemoteNetwork::get(), &RemoteNetwork::gotBranchList, this, &RemotePushDialog::fillInBranches);
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

    // Update the foce push check box text
    if(ui->checkForce->isChecked())
        ui->checkForce->setText(tr("Use with care. This can cause remote commits to be deleted."));
    else
        ui->checkForce->setText(" ");   // The space character here is required to avoid annoying resizes when toggling the checkbox

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

bool RemotePushDialog::forcePush() const
{
    return ui->checkForce->isChecked();
}

void RemotePushDialog::fillInLicences(const std::vector<std::pair<std::string, std::string>>& licences)
{
    // Clear licence list
    ui->comboLicence->clear();

    // Parse licence list and fill combo box. Show the full name to the user and use the short name as user data.
    for(const auto& it : licences)
        ui->comboLicence->addItem(QString::fromStdString(it.second), QString::fromStdString(it.first));
}

void RemotePushDialog::fillInBranches(const std::vector<std::string>& branches, const std::string& default_branch)
{
    // Clear branch list and add the default branch
    ui->comboBranch->clear();
    ui->comboBranch->addItem(QString::fromStdString(default_branch));

    // Add rest of the branch list to the combo box
    for(const std::string& branch : branches)
    {
        if(branch != default_branch)
            ui->comboBranch->addItem(QString::fromStdString(branch));
    }

    // If a branch was suggested, select it now
    if(!m_suggestedBranch.isEmpty())
        ui->comboBranch->setCurrentIndex(ui->comboBranch->findText(m_suggestedBranch));
}

void RemotePushDialog::reloadBranchList()
{
    // Assemble query URL
    QUrl url(m_host + "branch/list");
    QUrlQuery query;
    query.addQueryItem("username", RemoteNetwork::get().getInfoFromClientCert(m_clientCert, RemoteNetwork::CertInfoUser));
    query.addQueryItem("folder", "/");
    query.addQueryItem("dbname", ui->editName->text());
    url.setQuery(query);

    // Send request
    RemoteNetwork::get().fetch(url.toString(), RemoteNetwork::RequestTypeBranchList, m_clientCert);
}
