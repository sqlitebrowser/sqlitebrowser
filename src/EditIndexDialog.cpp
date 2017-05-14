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
      ui(new Ui::EditIndexDialog),
      m_sRestorePointName(pdb.generateSavepointName("editindex"))
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
        // Load the current layout and fill in the dialog fields
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
        ui->editPartialClause->blockSignals(true);
        ui->editPartialClause->setText(index.whereExpr());
        ui->editPartialClause->blockSignals(false);

        tableChanged(index.table(), true);
    } else {
        tableChanged(ui->comboTableName->currentText(), false);
    }

    // Create a savepoint to revert back to
    pdb.setSavepoint(m_sRestorePointName);
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

    // Stop here if table name is empty
    if(new_table.isEmpty())
    {
        // Call checkInput() before to make sure the OK button is disabled
        checkInput();
        return;
    }

    updateColumnLists();
}

void EditIndexDialog::updateColumnLists()
{
    // Fill the table column list
    sqlb::FieldInfoList tableFields = pdb.getObjectByName(index.table()).dynamicCast<sqlb::Table>()->fieldInformation();
    ui->tableTableColumns->setRowCount(tableFields.size());
    int tableRows = 0;
    for(int i=0;i<tableFields.size();++i)
    {
        // When we're doing the initial loading and this field already is in the index to edit, then don't add it to the
        // list of table columns. It will be added to the list of index columns in the next step. When this is not the initial
        // loading, the index column list is empty, so this check will always be true.
        if(index.findColumn(tableFields.at(i).name) == -1)
        {
            // Put the name of the field in the first column
            QTableWidgetItem* name = new QTableWidgetItem(tableFields.at(i).name);
            name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableTableColumns->setItem(tableRows, 0, name);

            // Put the data type in the second column
            QTableWidgetItem* type = new QTableWidgetItem(tableFields.at(i).type);
            type->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableTableColumns->setItem(tableRows, 1, type);

            tableRows++;
        }
    }

    // Set row count to actual count. This is needed for the intial loading, when some rows might have been omitted because they were used in the index
    ui->tableTableColumns->setRowCount(tableRows);

    // Fill the index column list. This is done separately from the table column to include expression columns (these are not found in the original
    // table) and to preserve the order of the index columns
    sqlb::FieldInfoList indexFields = index.fieldInformation();
    ui->tableIndexColumns->setRowCount(indexFields.size());
    for(int i=0;i<indexFields.size();++i)
    {
        // Put the name of the field in the first column
        QTableWidgetItem* name = new QTableWidgetItem(indexFields.at(i).name);
        name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableIndexColumns->setItem(i, 0, name);

        // And put a combobox to select the order in which to index the field in the last column
        QComboBox* order = new QComboBox(this);
        order->addItem("");
        order->addItem("ASC");
        order->addItem("DESC");
        order->setCurrentText(indexFields.at(i).type.toUpper());
        ui->tableIndexColumns->setCellWidget(i, 1, order);
        connect(order, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged),
                [=](QString new_order)
        {
            int colnum = index.findColumn(indexFields.at(i).name);
            if(colnum != -1)
            {
                index.column(colnum)->setOrder(new_order);
                updateSqlText();
            }
        });
    }

    checkInput();
}

void EditIndexDialog::addToIndex(const QModelIndex& idx)
{
    // Get current row number
    int row;
    if(idx.isValid())
        row = idx.row();
    else
        row = ui->tableTableColumns->currentRow();

    // No row selected? Abort.
    if(row == -1)
        return;

    // Add field to index
    index.addColumn(sqlb::IndexedColumnPtr(new sqlb::IndexedColumn(
                                               ui->tableTableColumns->item(row, 0)->text(),     // Column name
                                               false,                                           // Is expression
                                               "")));                                           // Order

    // Update UI
    updateColumnLists();
}

void EditIndexDialog::removeFromIndex(const QModelIndex& idx)
{
    // Get current row number
    int row;
    if(idx.isValid())
        row = idx.row();
    else
        row = ui->tableIndexColumns->currentRow();

    // No row selected? Abort.
    if(row == -1)
        return;

    // Remove column from index
    index.removeColumn(ui->tableIndexColumns->item(row, 0)->text());

    // Update UI
    updateColumnLists();
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
    index.setWhereExpr(ui->editPartialClause->text());
    updateSqlText();
}

void EditIndexDialog::accept()
{
    // When editing an index, delete the old one first
    if(!newIndex)
    {
        if(!pdb.executeSQL(QString("DROP INDEX IF EXISTS %1;").arg(sqlb::escapeIdentifier(curIndex))))
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

void EditIndexDialog::reject()
{
    // Rollback to our savepoint
    pdb.revertToSavepoint(m_sRestorePointName);

    QDialog::reject();
}

void EditIndexDialog::updateSqlText()
{
    ui->sqlTextEdit->setText(index.sql());
}
