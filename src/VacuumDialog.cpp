#include "VacuumDialog.h"
#include "ui_VacuumDialog.h"
#include "sqlitedb.h"

#include <QTreeWidgetItem>

VacuumDialog::VacuumDialog(DBBrowserDB* _db, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::VacuumDialog),
    db(_db)
{
    // Create UI
    ui->setupUi(this);

    // Show warning if DB is dirty
    ui->labelSavepointWarning->setVisible(db->getDirty());

    // Populate list of objects to compact. We just support vacuuming the different schemas here.
    for(const auto& it : db->schemata)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeDatabases);
        item->setText(0, QString::fromStdString(it.first));
        item->setIcon(0, QIcon(QString(":icons/database")));
        ui->treeDatabases->addTopLevelItem(item);
    }

    // Select the first item which should always be the main schema
    ui->treeDatabases->setCurrentItem(ui->treeDatabases->topLevelItem(0));
}

VacuumDialog::~VacuumDialog()
{
    delete ui;
}

void VacuumDialog::accept()
{
    if(ui->treeDatabases->selectedItems().count() == 0)
        return QDialog::reject();

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Commit all changes first
    db->releaseAllSavepoints();

    // Loop through all selected databases and vacuum them individually
    QList<QTreeWidgetItem*> selection = ui->treeDatabases->selectedItems();
    for(const QTreeWidgetItem* item : selection)
        db->executeSQL("VACUUM " + sqlb::escapeIdentifier(item->text(0).toStdString()), false);

    QApplication::restoreOverrideCursor();
    QDialog::accept();
}
