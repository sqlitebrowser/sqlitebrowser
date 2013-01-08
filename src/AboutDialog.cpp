#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include <sqlite3.h>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setFixedSize(669, 306);

    ui->label_version->setText(ui->label_version->text() + " " + APP_VERSION);
    ui->label_versionqt->setText(ui->label_versionqt->text() + " " + QT_VERSION_STR);
    ui->label_versionsqlite->setText(ui->label_versionsqlite->text() + " " + SQLITE_VERSION);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
