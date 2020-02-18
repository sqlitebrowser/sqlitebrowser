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
    ui->comboType->setCurrentIndex(ui->comboType->findData(Settings::getValue("proxy", "type").toString()));
    ui->editHost->setText(Settings::getValue("proxy", "host").toString());
    ui->spinPort->setValue(Settings::getValue("proxy", "port").toInt());
    ui->checkAuthentication->setChecked(Settings::getValue("proxy", "authentication").toBool());
    ui->editUser->setText(Settings::getValue("proxy", "user").toString());
    ui->editPassword->setText(Settings::getValue("proxy", "password").toString());
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
    Settings::setValue("proxy", "type", ui->comboType->currentData());
    Settings::setValue("proxy", "host", ui->editHost->text());
    Settings::setValue("proxy", "port", ui->spinPort->value());
    Settings::setValue("proxy", "authentication", ui->checkAuthentication->isChecked());
    Settings::setValue("proxy", "user", ui->editUser->text());
    Settings::setValue("proxy", "password", ui->editPassword->text());
}
