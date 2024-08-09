#include "CipherDialog.h"
#include "ui_CipherDialog.h"
#include "sqlitedb.h"

#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QDesktopServices>
#include <QUrl>

#include <QtCore/qmath.h>

CipherDialog::CipherDialog(QWidget* parent, bool encrypt) :
    QDialog(parent),
    ui(new Ui::CipherDialog),
    encryptMode(encrypt),
    rawKeyValidator(new QRegularExpressionValidator(QRegularExpression("\\A(0x[a-fA-F0-9]+)\\Z")))
{
    ui->setupUi(this);

    int minimumPageSizeExponent = 9;
    int maximumPageSizeExponent = 16;
    int defaultPageSizeExponent = 10;

    for(int exponent = minimumPageSizeExponent; exponent <= maximumPageSizeExponent; exponent++)
    {
        int pageSize = static_cast<int>(qPow(2, exponent));
        ui->comboPageSize->addItem(QLocale().toString(pageSize), pageSize);

        if (exponent == defaultPageSizeExponent)
            ui->comboPageSize->setCurrentIndex(exponent - minimumPageSizeExponent);
    }

    ui->comboPageSize->setMinimumWidth(ui->editPassword->width());

    if(encrypt)
    {
        ui->labelDialogDescription->setText(tr("Please set a key to encrypt the database.\nNote that if you change any of the other, optional, settings you'll need "
                                               "to re-enter them as well every time you open the database file.\nLeave the password fields empty to disable the "
                                               "encryption.\nThe encryption process might take some time and you should have a backup copy of your database! Unsaved "
                                               "changes are applied before modifying the encryption."));
    } else {
        ui->labelDialogDescription->setText(tr("Please enter the key used to encrypt the database.\nIf any of the other settings were altered for this database file "
                                               "you need to provide this information as well."));
        ui->editPassword2->setVisible(false);
        ui->labelPassword2->setVisible(false);
    }

    // Set the default encryption settings depending on the SQLCipher version we use
    QString sqlite_version, sqlcipher_version;
    DBBrowserDB::getSqliteVersion(sqlite_version, sqlcipher_version);
    if(sqlcipher_version.startsWith('4'))
        ui->radioEncryptionSqlCipher4->setChecked(true);
    else
        ui->radioEncryptionSqlCipher3->setChecked(true);
}

CipherDialog::~CipherDialog()
{
    delete rawKeyValidator;
    delete ui;
}

CipherSettings CipherDialog::getCipherSettings() const
{
    CipherSettings::KeyFormats keyFormat = CipherSettings::getKeyFormat(ui->comboKeyFormat->currentIndex());
    std::string password = ui->editPassword->text().toStdString();
    int pageSize = ui->comboPageSize->itemData(ui->comboPageSize->currentIndex()).toInt();

    CipherSettings cipherSettings;

    cipherSettings.setKeyFormat(keyFormat);
    cipherSettings.setPassword(password);
    cipherSettings.setPageSize(pageSize);
    cipherSettings.setKdfIterations(ui->spinKdfIterations->value());
    cipherSettings.setHmacAlgorithm("HMAC_" + ui->comboHmacAlgorithm->currentText().toStdString());
    cipherSettings.setKdfAlgorithm("PBKDF2_HMAC_" + ui->comboKdfAlgorithm->currentText().toStdString());
    cipherSettings.setPlaintextHeaderSize(ui->plaintextHeaderSize->value());

    return cipherSettings;
}

void CipherDialog::checkInputFields()
{
    CipherSettings::KeyFormats keyFormat = CipherSettings::getKeyFormat(ui->comboKeyFormat->currentIndex());
    if(sender() == ui->comboKeyFormat)
    {
        if(keyFormat == CipherSettings::KeyFormats::Passphrase)
        {
            ui->editPassword->setValidator(nullptr);
            ui->editPassword2->setValidator(nullptr);
            ui->editPassword->setPlaceholderText("");
        } else if(keyFormat == CipherSettings::KeyFormats::RawKey) {
            ui->editPassword->setValidator(rawKeyValidator);
            ui->editPassword2->setValidator(rawKeyValidator);
            ui->editPassword->setPlaceholderText("0x...");
        }

        ui->editPassword->setText("");
        ui->editPassword2->setText("");
    }

    bool valid = true;
    if(encryptMode) {
        const QString password1 = ui->editPassword->text();
        valid = password1 == ui->editPassword2->text();

        if (keyFormat == CipherSettings::KeyFormats::RawKey) {
            valid &= password1.isEmpty() || password1.length() > 2;
        }
    }

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

void CipherDialog::toggleEncryptionSettings()
{
    if(ui->radioEncryptionSqlCipher3->isChecked())
    {
        // SQLCipher3
        ui->comboPageSize->setCurrentText(QLocale().toString(1024));
        ui->spinKdfIterations->setValue(64000);
        ui->comboHmacAlgorithm->setCurrentText("SHA1");
        ui->comboKdfAlgorithm->setCurrentText("SHA1");
        ui->plaintextHeaderSize->setValue(0);

        ui->comboPageSize->setEnabled(false);
        ui->spinKdfIterations->setEnabled(false);
        ui->comboHmacAlgorithm->setEnabled(false);
        ui->comboKdfAlgorithm->setEnabled(false);
        ui->plaintextHeaderSize->setEnabled(false);
    } else if(ui->radioEncryptionSqlCipher4->isChecked()) {
        // SQLCipher4
        ui->comboPageSize->setCurrentText(QLocale().toString(4096));
        ui->spinKdfIterations->setValue(256000);
        ui->comboHmacAlgorithm->setCurrentText("SHA512");
        ui->comboKdfAlgorithm->setCurrentText("SHA512");
        ui->plaintextHeaderSize->setValue(0);

        ui->comboPageSize->setEnabled(false);
        ui->spinKdfIterations->setEnabled(false);
        ui->comboHmacAlgorithm->setEnabled(false);
        ui->comboKdfAlgorithm->setEnabled(false);
        ui->plaintextHeaderSize->setEnabled(false);
    } else if(ui->radioEncryptionCustom->isChecked()) {
        // Custom

        ui->comboPageSize->setEnabled(true);
        ui->spinKdfIterations->setEnabled(true);
        ui->comboHmacAlgorithm->setEnabled(true);
        ui->comboKdfAlgorithm->setEnabled(true);
        ui->plaintextHeaderSize->setEnabled(true);
    }
}

void CipherDialog::help() {
    QDesktopServices::openUrl(QUrl("https://github.com/sqlitebrowser/sqlitebrowser/wiki/Encrypted-Databases"));
}
