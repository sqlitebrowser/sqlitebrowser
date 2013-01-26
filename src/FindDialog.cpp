#include "FindDialog.h"
#include "ui_FindDialog.h"

FindDialog::FindDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::FindDialog)
{
    ui->setupUi(this);
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::showResults(const resultMap& rmap)
{
    ui->tableResults->setSortingEnabled(false);
    ui->tableResults->clearContents();
    resultMap::const_iterator it;
    int rowNum;
    ui->tableResults->setRowCount(rmap.size());
    for(it=rmap.begin(),rowNum=0;it!=rmap.end();++it,rowNum++)
    {
        QString firstline = it.value().section('\n', 0, 0);
        ui->tableResults->setItem(rowNum, 0, new QTableWidgetItem(QString::number(it.key() + 1)));
        ui->tableResults->setItem(rowNum, 1, new QTableWidgetItem(firstline));
    }
    QString results = tr("Found: %1").arg(ui->tableResults->rowCount());
    ui->labelNumberResults->setText(results);
    ui->tableResults->setSortingEnabled(true);
}

void FindDialog::find()
{
    emit lookfor(ui->comboColumn->currentText(), ui->comboOperator->currentText(), ui->editSearchString->text());
}

void FindDialog::resetFields(const QStringList& fieldlist)
{
    ui->comboColumn->clear();
    ui->comboColumn->addItems(fieldlist);
    ui->editSearchString->setText("");
    ui->comboOperator->setCurrentIndex(0);
    ui->tableResults->clearContents();
    ui->labelNumberResults->setText(tr("Found: 0"));
}

void FindDialog::recordSelected(QTableWidgetItem* witem)
{
    if(witem)
    {
        int recNum = ui->tableResults->item(witem->row(), 0)->text().toInt();
        emit showrecord(recNum - 1);
    }
}

void FindDialog::closeEvent(QCloseEvent*)
{
        emit goingAway();
}
