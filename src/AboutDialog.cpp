#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "version.h"
#include "sqlite.h"
#include "sqlitedb.h"

AboutDialog::AboutDialog(DBBrowserDB* _db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog),
    db(_db)
{
#ifdef ENABLE_SQLCIPHER
    // Retrieve the SQLCipher version
    QString sqlcipher_version = db->getPragma("cipher_version");
#endif

    // Display the About dialog
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    ui->label_version->setText(tr("Version ") + APP_VERSION + "\n\n" +
                               tr("Qt Version ") + QT_VERSION_STR + "\n\n" +
#ifdef ENABLE_SQLCIPHER
                               tr("SQLCipher Version ") + sqlcipher_version + "\n\n" +
#endif
                               tr("SQLite Version ") + SQLITE_VERSION
                               );
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
