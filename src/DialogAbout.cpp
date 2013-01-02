#include "DialogAbout.h"
#include "ui_DialogAbout.h"
#include "sqlite_source/sqlite3.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
    this->setFixedSize(669, 306);

    ui->label_version->setText(ui->label_version->text() + " " + APP_VERSION);
    ui->label_versionqt->setText(ui->label_versionqt->text() + " " + QT_VERSION_STR);
    ui->label_versionsqlite->setText(ui->label_versionsqlite->text() + " " + SQLITE_VERSION);
}

DialogAbout::~DialogAbout()
{
    delete ui;
}
