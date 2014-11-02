#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "gen_version.h"
#include "sqlite.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());

    ui->label_version->setText(ui->label_version->text() + " " + APP_VERSION);
    ui->label_versionqt->setText(ui->label_versionqt->text() + " " + QT_VERSION_STR);
#ifdef ENABLE_SQLCIPHER
    ui->label_versionsqlite->setText(ui->label_versionsqlite->text().replace("SQLite", "SQLCipher") + " " + SQLITE_VERSION);
#else
    ui->label_versionsqlite->setText(ui->label_versionsqlite->text() + " " + SQLITE_VERSION);
#endif
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
