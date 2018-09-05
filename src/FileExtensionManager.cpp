#include "FileExtensionManager.h"
#include "ui_FileExtensionManager.h"

FileExtensionManager::FileExtensionManager(QStringList init, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileExtensionManager)
{
    ui->setupUi(this);

    int i = 0;
    for(const QString& itemString : init)
    {
        QString description = itemString.left(itemString.indexOf('(')).trimmed();
        QString extension   = itemString;
        extension = extension.remove (0, itemString.indexOf('(')+1).remove(')').simplified().trimmed();
        if ( extension.compare("*") != 0 ) //We exclude "All files" from the table
        {
            QTableWidgetItem *newItemDescription = new QTableWidgetItem(description);
            QTableWidgetItem *newItemExtension   = new QTableWidgetItem(extension);
            ui->tableExtensions->insertRow(i);
            ui->tableExtensions->setItem(i, 0, newItemDescription);
            ui->tableExtensions->setItem(i, 1, newItemExtension);
            i++;
        }
    }

    connect(ui->buttonAdd, SIGNAL(clicked(bool)), this, SLOT(addItem()));
    connect(ui->buttonRemove, SIGNAL(clicked(bool)), this, SLOT(removeItem()));

    connect(ui->buttonDown, SIGNAL(clicked(bool)), this, SLOT(downItem()));
    connect(ui->buttonUp, SIGNAL(clicked(bool)), this, SLOT(upItem()));
}

FileExtensionManager::~FileExtensionManager()
{
    delete ui;
}

void FileExtensionManager::addItem()
{
    int i = ui->tableExtensions->rowCount();
    ui->tableExtensions->insertRow(i);
    QTableWidgetItem *newItemDescription = new QTableWidgetItem(tr("Description"));
    QTableWidgetItem *newItemExtension   = new QTableWidgetItem(tr("*.extension"));
    ui->tableExtensions->setItem(i, 0, newItemDescription);
    ui->tableExtensions->setItem(i, 1, newItemExtension);
}

void FileExtensionManager::removeItem()
{
    QList<int> selectedRows;
    for (const QTableWidgetItem* item : ui->tableExtensions->selectedItems())
    {
        if (selectedRows.contains(item->row()) == false)
        {
            selectedRows.append(item->row());
        }
    }

    qSort(selectedRows);

    for (int i = selectedRows.size()-1; i >= 0; --i)
    {
        ui->tableExtensions->removeRow(selectedRows[i]);
    }
}

void FileExtensionManager::upItem()
{
    if (ui->tableExtensions->selectedItems().isEmpty()) return;

    int selectedRow = ui->tableExtensions->selectedItems().first()->row();
    if(selectedRow == 0)
        return;

    QTableWidgetItem *t1, *t2;
    t1 = ui->tableExtensions->takeItem(selectedRow, 0);
    t2 = ui->tableExtensions->takeItem(selectedRow, 1);
    ui->tableExtensions->removeRow(selectedRow);
    ui->tableExtensions->insertRow(selectedRow-1);
    ui->tableExtensions->setItem(selectedRow-1, 0, t1);
    ui->tableExtensions->setItem(selectedRow-1, 1, t2);
    ui->tableExtensions->selectRow(selectedRow-1);
}

void FileExtensionManager::downItem()
{
    if (ui->tableExtensions->selectedItems().isEmpty()) return;

    int selectedRow = ui->tableExtensions->selectedItems().first()->row();
    if(selectedRow == ui->tableExtensions->rowCount() - 1)
        return;

    QTableWidgetItem *t1, *t2;
    t1 = ui->tableExtensions->takeItem(selectedRow, 0);
    t2 = ui->tableExtensions->takeItem(selectedRow, 1);
    ui->tableExtensions->removeRow(selectedRow);
    ui->tableExtensions->insertRow(selectedRow+1);
    ui->tableExtensions->setItem(selectedRow+1, 0, t1);
    ui->tableExtensions->setItem(selectedRow+1, 1, t2);
    ui->tableExtensions->selectRow(selectedRow+1);
}

QStringList FileExtensionManager::getDBFileExtensions()
{
    QStringList result;
    for (int i = 0; i < ui->tableExtensions->rowCount(); ++i)
    {
        result.append(QString("%1 (%2)").arg(ui->tableExtensions->item(i, 0)->text()).arg(ui->tableExtensions->item(i, 1)->text()));
    }
    return result;
}

