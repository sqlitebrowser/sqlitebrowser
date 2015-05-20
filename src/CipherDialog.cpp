#include "CipherDialog.h"
#include "ui_CipherDialog.h"

#include <QPushButton>

CipherDialog::CipherDialog(QWidget* parent, bool encrypt) :
    QDialog(parent),
    ui(new Ui::CipherDialog),
    encryptMode(encrypt)
{
    ui->setupUi(this);

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
    delete ui;
}

QString CipherDialog::password() const
{
    return ui->editPassword->text();
}

int CipherDialog::pageSize() const
{
    return ui->spinPageSize->value();
}

void CipherDialog::checkInputFields()
{
    bool valid = true;
    if(encryptMode)
        valid = ui->editPassword->text() == ui->editPassword2->text();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}
