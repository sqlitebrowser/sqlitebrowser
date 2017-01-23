#include "EditIndexDialog.h"
#include "ui_EditIndexDialog.h"
#include "sqlitedb.h"

#include <QMessageBox>
#include <QPushButton>

EditIndexDialog::EditIndexDialog(DBBrowserDB& db, const QString& indexName, bool createIndex, QWidget* parent)
    : QDialog(parent),
      pdb(db),
      curIndex(indexName),
      index(indexName),
      newIndex(createIndex),
      ui(new Ui::EditIndexDialog)
{
    // Create UI
    ui->setupUi(this);

    // Get list of tables, sort it alphabetically and fill the combobox
    objectMap dbobjs;
    QList<sqlb::ObjectPtr> tables = pdb.objMap.values("table");
    for(auto it=tables.constBegin();it!=tables.constEnd();++it)
        dbobjs.insert((*it)->name(), (*it));
    ui->comboTableName->blockSignals(true);
    for(auto it=dbobjs.constBegin();it!=dbobjs.constEnd();++it)
        ui->comboTableName->addItem(QIcon(QString(":icons/table")), (*it)->name());
    ui->comboTableName->blockSignals(false);

    QHeaderView *tableHeaderView = ui->tableIndexColumns->horizontalHeader();
    tableHeaderView->setSectionResizeMode(0, QHeaderView::Stretch);

    // Editing an existing index?
    if(!newIndex)
    {
        // Load the current layour and fill in the dialog fields
        index = *(pdb.getObjectByName(curIndex).dynamicCast<sqlb::Index>());

        ui->editIndexName->blockSignals(true);
        ui->editIndexName->setText(index.name());
        ui->editIndexName->blockSignals(false);
        ui->checkIndexUnique->blockSignals(true);
        ui->checkIndexUnique->setChecked(index.unique());
        ui->checkIndexUnique->blockSignals(false);
        ui->comboTableName->blockSignals(true);
        ui->comboTableName->setCurrentText(index.table());
        ui->comboTableName->blockSignals(false);

        tableChanged(index.table(), true);
    } else {
        tableChanged(ui->comboTableName->currentText(), false);
    }

    // Refresh SQL preview
    updateSqlText();
}

EditIndexDialog::~EditIndexDialog()
{
    delete ui;
}

void EditIndexDialog::tableChanged(const QString& new_table, bool initialLoad)
{
    // Set the table name and clear all index columns
    if(!initialLoad)
    {
        index.setTable(new_table);
        index.clearColumns();
    }

    // And fill the table again
    QStringList fields = pdb.getObjectByName(new_table).dynamicCast<sqlb::Table>()->fieldNames();
    ui->tableIndexColumns->setRowCount(fields.size());
    for(int i=0; i < fields.size(); ++i)
    {
        // Put the name of the field in the first column
        QTableWidgetItem* name = new QTableWidgetItem(fields.at(i));
        name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableIndexColumns->setItem(i, 0, name);

        // Put a checkbox to enable usage in the index of this field in the second column
        QCheckBox* enabled = new QCheckBox(this);
        if(initialLoad && index.findColumn(fields.at(i)) != -1)
            enabled->setCheckState(Qt::Checked);
        else
            enabled->setCheckState(Qt::Unchecked);
        ui->tableIndexColumns->setCellWidget(i, 1, enabled);
        connect(enabled, static_cast<void(QCheckBox::*)(bool)>(&QCheckBox::toggled),
                [=](bool use_in_index)
        {
            if(use_in_index)
            {
                index.addColumn(sqlb::IndexedColumnPtr(new sqlb::IndexedColumn(
                                                           ui->tableIndexColumns->item(i, 0)->text(),
                                                           false,
                                                           qobject_cast<QComboBox*>(ui->tableIndexColumns->cellWidget(i, 2))->currentText())));
            } else {
                index.removeColumn(ui->tableIndexColumns->item(i, 0)->text());
            }

            checkInput();
            updateSqlText();
        });

        // And put a combobox to select the order in which to index the field in the last column
        QComboBox* order = new QComboBox(this);
        order->addItem("");
        order->addItem("ASC");
        order->addItem("DESC");
        if(initialLoad && index.findColumn(fields.at(i)) != -1)
            order->setCurrentText(index.column(index.findColumn(fields.at(i)))->order());
        ui->tableIndexColumns->setCellWidget(i, 2, order);
        connect(order, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged),
                [=](QString new_order)
        {
            int colnum = index.findColumn(fields.at(i));
            if(colnum != -1)
            {
                index.column(colnum)->setOrder(new_order);
                updateSqlText();
            }
        });
    }

    updateSqlText();
}

void EditIndexDialog::checkInput()
{
    // Check if index name is set
    bool valid = true;
    if(ui->editIndexName->text().isEmpty())
        valid = false;

    // Check if index has any columns
    if(index.columns().size() == 0)
        valid = false;

    // Only activate OK button if index data is valid
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);

    // Set the index name and the unique flag
    index.setName(ui->editIndexName->text());
    index.setUnique(ui->checkIndexUnique->isChecked());
    updateSqlText();
}

void EditIndexDialog::accept()
{
    // When editing an index, delete the old one first
    if(!newIndex)
    {
        if(!pdb.executeSQL(QString("DROP INDEX %1;").arg(sqlb::escapeIdentifier(curIndex))))
        {
            QMessageBox::warning(this, qApp->applicationName(), tr("Deleting the old index failed:\n%1").arg(pdb.lastError()));
            return;
        }
    }

    // Create the new index
    if(pdb.executeSQL(index.sql()))
        QDialog::accept();
    else
        QMessageBox::warning(this, QApplication::applicationName(), tr("Creating the index failed:\n%1").arg(pdb.lastError()));
}

void EditIndexDialog::updateSqlText()
{
    ui->sqlTextEdit->setText(index.sql());
}
