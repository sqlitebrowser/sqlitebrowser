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

    // Populate list of objects to compact
    QList<DBBrowserObject> objects = db->objMap.values("table");
    objects.append(db->objMap.values("index"));
    for(QList<DBBrowserObject>::const_iterator i=objects.constBegin();i!=objects.constEnd();++i)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeSelectedObjects);
        item->setText(0, (*i).getname());
        item->setIcon(0, QIcon(QString(":icons/%1").arg((*i).gettype())));
        ui->treeSelectedObjects->addTopLevelItem(item);
    }

    // Sort objects and select them all
    ui->treeSelectedObjects->sortByColumn(0, Qt::AscendingOrder);
    ui->treeSelectedObjects->selectAll();
}

VacuumDialog::~VacuumDialog()
{
    delete ui;
}

void VacuumDialog::accept()
{
    if(ui->treeSelectedObjects->selectedItems().count() == 0)
        return QDialog::reject();

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Commit all changes first
    db->releaseAllSavepoints();

    // All items selected?
    if(ui->treeSelectedObjects->selectedItems().count() ==  ui->treeSelectedObjects->topLevelItemCount())
    {
        // Yes, so just execute a simple vacuum command for all objects
        db->executeSQL("VACUUM;", false);
    } else {
        // No, so execute a vacuum command for each selected object individually
        QList<QTreeWidgetItem*> selection = ui->treeSelectedObjects->selectedItems();
        foreach(QTreeWidgetItem* item, selection)
            db->executeSQL(QString("VACUUM %1;").arg(sqlb::escapeIdentifier(item->text(0))), false);
    }

    QApplication::restoreOverrideCursor();
    QDialog::accept();
}
