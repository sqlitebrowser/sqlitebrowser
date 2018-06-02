#include "CipherDialog.h"
#include "ui_CipherDialog.h"

#include <QPushButton>
#include <QRegExpValidator>

#include <QtCore/qmath.h>

CipherDialog::CipherDialog(QWidget* parent, bool encrypt) :
    QDialog(parent),
    ui(new Ui::CipherDialog),
    encryptMode(encrypt),
    rawKeyValidator(new QRegExpValidator(QRegExp("0x[a-fA-F0-9]+")))
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
}

CipherDialog::~CipherDialog()
{
    delete rawKeyValidator;
    delete ui;
}

CipherSettings::KeyFormats CipherDialog::keyFormat() const
{
    return static_cast<CipherSettings::KeyFormats>(ui->comboKeyFormat->currentIndex());
}

QString CipherDialog::password() const
{
    if(keyFormat() == CipherSettings::KeyFormats::Passphrase)
        return QString("'%1'").arg(ui->editPassword->text().replace("'", "''"));
    else
        return QString("\"x'%1'\"").arg(ui->editPassword->text().mid(2));   // Remove the '0x' part at the beginning
}

int CipherDialog::pageSize() const
{
    return ui->comboPageSize->itemData(ui->comboPageSize->currentIndex()).toInt();
}

void CipherDialog::checkInputFields()
{
    if(sender() == ui->comboKeyFormat)
    {
        if(keyFormat() == CipherSettings::KeyFormats::Passphrase)
        {
            ui->editPassword->setValidator(nullptr);
            ui->editPassword2->setValidator(nullptr);
            ui->editPassword->setPlaceholderText("");
        } else if(keyFormat() == CipherSettings::KeyFormats::RawKey) {
            ui->editPassword->setValidator(rawKeyValidator);
            ui->editPassword2->setValidator(rawKeyValidator);
            ui->editPassword->setPlaceholderText("0x...");
        }

        ui->editPassword->setText("");
        ui->editPassword2->setText("");
    }

    bool valid = true;
    if(encryptMode)
        valid = ui->editPassword->text() == ui->editPassword2->text();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}
