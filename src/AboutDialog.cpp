#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "Application.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->label_version->setText(Application::versionInformation());
}

void AboutDialog::showEvent(QShowEvent *)
{
    this->adjustSize();
    this->setFixedSize(this->width(), this->height());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
