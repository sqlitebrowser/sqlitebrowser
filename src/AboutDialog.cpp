#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "version.h"
#include "sqlite.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->label_version->setText(tr("Version ") + APP_VERSION + "\n\n" +
                               tr("Qt Version ") + QT_VERSION_STR + "\n\n" +
#ifdef ENABLE_SQLCIPHER
                               tr("SQLCipher Version ") + SQLITE_VERSION
#else
                               tr("SQLite Version ") + SQLITE_VERSION
#endif
                               );
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
