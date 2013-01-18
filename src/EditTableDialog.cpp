#include "EditTableDialog.h"
#include "ui_EditTableDialog.h"
#include "EditFieldDialog.h"
#include <QMessageBox>
#include <QPushButton>
#include "sqlitedb.h"

EditTableDialog::EditTableDialog(DBBrowserDB* db, QString tableName, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditTableDialog),
      pdb(db),
      curTable(tableName)
{
    // Create UI
    ui->setupUi(this);

    // Editing an existing table?
    if(curTable != "")
    {
        // Existing table, so load and set the current layout
        populateFields();

        // And create a savepoint
        pdb->executeSQL(QString("SAVEPOINT edittable_%1_save;").arg(curTable));
    }

    // Update UI
    ui->editTableName->setText(curTable);
    checkInput();
}

EditTableDialog::~EditTableDialog()
{
    delete ui;
}

void EditTableDialog::populateFields()
{
    //make sure we are not using cached information
    pdb->updateSchema();

    fields= pdb->getTableFields(curTable);
    types= pdb->getTableTypes(curTable);
    ui->treeWidget->model()->removeRows(0, ui->treeWidget->model()->rowCount());
    QStringList::Iterator tt = types.begin();
    for ( QStringList::Iterator ct = fields.begin(); ct != fields.end(); ++ct ) {
        QTreeWidgetItem *fldItem = new QTreeWidgetItem();
        fldItem->setText( 0, *ct  );
        fldItem->setText( 1, *tt );
        ui->treeWidget->addTopLevelItem(fldItem);
        ++tt;
    }
}

void EditTableDialog::accept()
{
    // Are we editing an already existing table or designing a new one? In the first case there is a table name set,
    // in the latter the current table name is empty
    if(curTable == "")
    {
        // Creation of new table

        // Prepare creation of the table
        QList<DBBrowserField> tbl_structure;
        for(int i=0;i<ui->treeWidget->topLevelItemCount();i++)
            tbl_structure.push_back(DBBrowserField(ui->treeWidget->topLevelItem(i)->text(0), ui->treeWidget->topLevelItem(i)->text(1)));

        // Create table
        if(!pdb->createTable(ui->editTableName->text(), tbl_structure))
        {
            QMessageBox::warning(this, QApplication::applicationName(), tr("Error creating table. Message from database engine:\n%1").arg(pdb->lastErrorMessage));
            return;
        }
    } else {
        // Editing of old table

        // Rename table if necessary
        if(ui->editTableName->text() != curTable)
        {
            QApplication::setOverrideCursor( Qt::WaitCursor ); // this might take time
            if(!pdb->renameTable(curTable, ui->editTableName->text()))
            {
                QApplication::restoreOverrideCursor();
                QMessageBox::warning(this, QApplication::applicationName(), pdb->lastErrorMessage);
                return;
            } else {
                QApplication::restoreOverrideCursor();
            }
        }

        // Release the savepoint
        pdb->executeSQL(QString("RELEASE SAVEPOINT edittable_%1_save;").arg(curTable));
    }

    QDialog::accept();
}

void EditTableDialog::reject()
{
    // Have we been in the process of editing an old table?
    if(curTable != "")
    {
        // Then rollback to our savepoint
         pdb->executeSQL(QString("ROLLBACK TO SAVEPOINT edittable_%1_save;").arg(curTable));
    }

    QDialog::reject();
}

void EditTableDialog::checkInput()
{
    ui->editTableName->setText(ui->editTableName->text().trimmed());
    bool valid = true;
    if(ui->editTableName->text().isEmpty() || ui->editTableName->text().contains(" "))
        valid = false;
    if(ui->treeWidget->topLevelItemCount() == 0)
        valid = false;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

void EditTableDialog::editField()
{
    if(!ui->treeWidget->currentItem())
        return;

    // Show the edit dialog
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    EditFieldDialog dialog(pdb, curTable == "", curTable, item->text(0), item->text(1), this);
    if(dialog.exec())
    {
        item->setText(0, dialog.field_name);
        item->setText(1, dialog.field_type);
    }
}

void EditTableDialog::addField()
{
    EditFieldDialog dialog(pdb, true, curTable, "", "", this);
    if(dialog.exec())
    {
        QTreeWidgetItem *tbitem = new QTreeWidgetItem(ui->treeWidget);
        tbitem->setText(0, dialog.field_name);
        tbitem->setText(1, dialog.field_type);
        ui->treeWidget->addTopLevelItem(tbitem);
        checkInput();
    }
}

void EditTableDialog::removeField()
{
    // Is there any item selected to delete?
    if(!ui->treeWidget->currentItem())
        return;

    // Are we creating a new table or editing an old one?
    if(curTable == "")
    {
        // Creating a new one

        // Just delete that item. At this point there is no DB table to edit or data to be lost anyway
        delete ui->treeWidget->currentItem();
    } else {
        // Editing an old one

        // Ask user wether he really wants to delete that column
        QString msg = tr("Are you sure you want to delete the field '%1'?\nAll data currently stored in this field will be lost.").arg(ui->treeWidget->currentItem()->text(0));
        if(QMessageBox::warning(this, QApplication::applicationName(), msg, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape) == QMessageBox::Yes)
        {
            if(!pdb->dropColumn(curTable, ui->treeWidget->currentItem()->text(0)))
            {
                QMessageBox::warning(0, QApplication::applicationName(), pdb->lastErrorMessage);
            } else {
                delete ui->treeWidget->currentItem();
            }
        }
    }

    checkInput();
}

void EditTableDialog::fieldSelectionChanged()
{
    ui->renameFieldButton->setEnabled(ui->treeWidget->selectionModel()->hasSelection());
    ui->removeFieldButton->setEnabled(ui->treeWidget->selectionModel()->hasSelection());
}
