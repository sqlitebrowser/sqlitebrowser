#include <QPushButton>
#include <QUrlQuery>
#include <QRegExpValidator>

#include <json.hpp>

#include "RemotePushDialog.h"
#include "ui_RemotePushDialog.h"
#include "RemoteNetwork.h"

using json = nlohmann::json;

RemotePushDialog::RemotePushDialog(QWidget* parent, const QString& host, const QString& clientCert,
                                   const QString& name, const QString& branch, const QString& user) :
    QDialog(parent),
    ui(new Ui::RemotePushDialog),
    m_host(host),
    m_clientCert(clientCert),
    m_nameValidator(new QRegExpValidator(QRegExp("^[a-z,A-Z,0-9,\\.,\\-,\\_,\\(,\\),\\+,\\ ]+$"), this)),
    m_branchValidator(new QRegExpValidator(QRegExp("^[a-z,A-Z,0-9,\\^,\\.,\\-,\\_,\\/,\\(,\\),\\:,\\&,\\ )]+$"), this))
{
    // Create UI
    ui->setupUi(this);
    ui->editName->setValidator(m_nameValidator);
    ui->comboBranch->setValidator(m_branchValidator);
    ui->comboUser->setValidator(m_nameValidator);

    // Set start values
    ui->editName->setText(name);

    // Fill in usernames
    ui->comboUser->addItem(RemoteNetwork::get().getInfoFromClientCert(m_clientCert, RemoteNetwork::CertInfoUser));
    if(!user.isEmpty())
        ui->comboUser->addItem(user);

    // Enable/disable accept button
    checkInput();

    // Fetch list of available licences
    RemoteNetwork::get().fetch(host + "licence/list", RemoteNetwork::RequestTypeCustom, clientCert, [this](const QByteArray& reply) {
        // Clear licence list
        ui->comboLicence->clear();

        // Read and check results
        json obj = json::parse(reply, nullptr, false);
        if(obj.is_discarded() || !obj.is_object())
            return;

        // Parse data and build ordered licence map: order -> (short name, long name)
        std::map<int, std::pair<std::string, std::string>> licences;
        for(auto it=obj.cbegin();it!=obj.cend();++it)
            licences.insert({it.value()["order"], {it.key(), it.value()["full_name"]}});

        // Parse licence list and fill combo box. Show the full name to the user and use the short name as user data.
        for(auto it=licences.begin();it!=licences.end();++it)
            ui->comboLicence->addItem(QString::fromStdString(it->second.second), QString::fromStdString(it->second.first));
    });

    // Fetch list of existing branches
    reloadBranchList(branch);
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

    if(ui->comboUser->currentText().trimmed().isEmpty())
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

QString RemotePushDialog::user() const
{
    return ui->comboUser->currentText();
}

bool RemotePushDialog::forcePush() const
{
    return ui->checkForce->isChecked();
}

void RemotePushDialog::reloadBranchList(const QString& select_branch)
{
    QUrl url(m_host + "branch/list");
    QUrlQuery query;
    query.addQueryItem("username", ui->comboUser->currentText());
    query.addQueryItem("folder", "/");
    query.addQueryItem("dbname", ui->editName->text());
    url.setQuery(query);
    RemoteNetwork::get().fetch(url.toString(), RemoteNetwork::RequestTypeCustom, m_clientCert, [this, select_branch](const QByteArray& reply) {
        // Read and check results
        json obj = json::parse(reply, nullptr, false);
        if(obj.is_discarded() || !obj.is_object())
            return;
        json obj_branches = obj["branches"];

        // Get default branch
        std::string default_branch = (obj.contains("default_branch") && !obj["default_branch"].empty()) ? obj["default_branch"] : "main";

        // Clear branch list and add the default branch
        ui->comboBranch->clear();
        ui->comboBranch->addItem(QString::fromStdString(default_branch));

        // Parse data and assemble branch list
        std::vector<std::string> branches;
        for(auto it=obj_branches.cbegin();it!=obj_branches.cend();++it)
        {
            if(it.key() != default_branch)
                ui->comboBranch->addItem(QString::fromStdString(it.key()));
        }

        // If a branch was suggested, select it now
        if(!select_branch.isEmpty())
            ui->comboBranch->setCurrentIndex(ui->comboBranch->findText(select_branch));
    }, false, true);
}
