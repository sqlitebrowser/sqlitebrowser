#include "ProxyDialog.h"
#include "ui_ProxyDialog.h"
#include "Settings.h"

ProxyDialog::ProxyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProxyDialog)
{
    ui->setupUi(this);

    // Add different proxy types. We use user data strings for addressing them later
    ui->comboType->addItem(tr("None"), "none");
    ui->comboType->addItem(tr("System settings"), "system");
    ui->comboType->addItem(tr("HTTP"), "http");
    ui->comboType->addItem(tr("Socks v5"), "socks5");

    // Load current settings
    ui->comboType->setCurrentIndex(ui->comboType->findData(Settings::getValue(szINI::SEC_PROXY, szINI::KEY_TYPE).toString()));
    ui->editHost->setText(Settings::getValue(szINI::SEC_PROXY, szINI::KEY_HOST).toString());
    ui->spinPort->setValue(Settings::getValue(szINI::SEC_PROXY, szINI::KEY_PORT).toInt());
    ui->checkAuthentication->setChecked(Settings::getValue(szINI::SEC_PROXY, szINI::KEY_AUTHENTICATION).toBool());
    ui->editUser->setText(Settings::getValue(szINI::SEC_PROXY, szINI::KEY_USER).toString());
    ui->editPassword->setText(Settings::getValue(szINI::SEC_PROXY, szINI::KEY_PASSWORD).toString());
}

ProxyDialog::~ProxyDialog()
{
    delete ui;
}

void ProxyDialog::proxyTypeChanged(int /*new_index*/)
{
    // When selecting the "None" or "System settings" proxy types, disable all the other input fields
    bool proxy_configuration = (ui->comboType->currentData() != "none" && ui->comboType->currentData() != "system");

    ui->editHost->setEnabled(proxy_configuration);
    ui->spinPort->setEnabled(proxy_configuration);
    ui->checkAuthentication->setEnabled(proxy_configuration);
    ui->editUser->setEnabled(ui->checkAuthentication->isChecked() && proxy_configuration);      // Enable user name and password only if the...
    ui->editPassword->setEnabled(ui->checkAuthentication->isChecked() && proxy_configuration);  // ... Authentication Required checkbox is checked
}

void ProxyDialog::authenticationRequiredChanged(bool required)
{
    ui->editUser->setEnabled(required);
    ui->editPassword->setEnabled(required);
}

void ProxyDialog::saveSettings() const
{
    Settings::setValue(szINI::SEC_PROXY, szINI::KEY_TYPE, ui->comboType->currentData());
    Settings::setValue(szINI::SEC_PROXY, szINI::KEY_HOST, ui->editHost->text());
    Settings::setValue(szINI::SEC_PROXY, szINI::KEY_PORT, ui->spinPort->value());
    Settings::setValue(szINI::SEC_PROXY, szINI::KEY_AUTHENTICATION, ui->checkAuthentication->isChecked());
    Settings::setValue(szINI::SEC_PROXY, szINI::KEY_USER, ui->editUser->text());
    Settings::setValue(szINI::SEC_PROXY, szINI::KEY_PASSWORD, ui->editPassword->text());
}
