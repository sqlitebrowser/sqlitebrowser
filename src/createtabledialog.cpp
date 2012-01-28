#include "createtabledialog.h"
#include "ui_createtabledialog.h"

CreateTableDialog::CreateTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTableDialog)
{
    ui->setupUi(this);
}

CreateTableDialog::~CreateTableDialog()
{
    delete ui;
}
