#include "EditIndexDialog.h"
#include "ui_EditIndexDialog.h"
#include "sqlitedb.h"
#include "IconCache.h"

#include <QMessageBox>
#include <QPushButton>

EditIndexDialog::EditIndexDialog(DBBrowserDB& db, const sqlb::ObjectIdentifier& indexName, bool createIndex, QWidget* parent)
    : QDialog(parent),
      pdb(db),
      curIndex(indexName),
      index(indexName.name()),
      newIndex(createIndex),
      ui(new Ui::EditIndexDialog),
      m_sRestorePointName(pdb.generateSavepointName("editindex"))
{
    // Create UI
    ui->setupUi(this);
    ui->sqlTextEdit->setReadOnly(true);

    // Get list of tables, sort it alphabetically and fill the combobox
    std::map<std::string, sqlb::ObjectIdentifier> dbobjs;  // Map from display name to full object identifier
    if(newIndex)        // If this is a new index, offer all tables of all database schemata
    {
        for(const auto& it : pdb.schemata)
        {
            for(const auto& jt : it.second.tables)
            {
                // Only show the schema name for non-main schemata
                sqlb::ObjectIdentifier obj(it.first, jt.first);
                dbobjs.insert({obj.toDisplayString(), obj});
            }
        }
    } else {            // If this is an existing index, only offer tables of the current database schema
        for(const auto& it : pdb.schemata[curIndex.schema()].tables)
        {
            // Only show the schema name for non-main schemata
            sqlb::ObjectIdentifier obj(curIndex.schema(), it.first);
            dbobjs.insert({obj.toDisplayString(), obj});
        }
    }
    ui->comboTableName->blockSignals(true);
    for(auto it=dbobjs.cbegin();it!=dbobjs.cend();++it)
        ui->comboTableName->addItem(IconCache::get("table"), QString::fromStdString(it->first), QString::fromStdString(it->second.toSerialised()));
    ui->comboTableName->blockSignals(false);

    QHeaderView *tableHeaderView = ui->tableIndexColumns->horizontalHeader();
    tableHeaderView->setSectionResizeMode(0, QHeaderView::Stretch);

    // Editing an existing index?
    if(!newIndex)
    {
        // Load the current layout and fill in the dialog fields
        index = *pdb.getIndexByName(curIndex);

        ui->editIndexName->blockSignals(true);
        ui->editIndexName->setText(QString::fromStdString(index.name()));
        ui->editIndexName->blockSignals(false);
        ui->checkIndexUnique->blockSignals(true);
        ui->checkIndexUnique->setChecked(index.unique());
        ui->checkIndexUnique->blockSignals(false);
        ui->comboTableName->blockSignals(true);
        ui->comboTableName->setCurrentText(QString::fromStdString(index.table()));
        ui->comboTableName->blockSignals(false);
        ui->editPartialClause->blockSignals(true);
        ui->editPartialClause->setText(QString::fromStdString(index.whereExpr()));
        ui->editPartialClause->blockSignals(false);

        tableChanged(QString::fromStdString(index.table()), true);
    } else {
        tableChanged(ui->comboTableName->currentText(), false);
    }

    // Add event handler for index column name changes. These are only allowed for expression columns, though.
    connect(ui->tableIndexColumns, &QTableWidget::itemChanged, this,
            [=](QTableWidgetItem* item)
    {
        index.fields[static_cast<size_t>(item->row())].setName(item->text().toStdString());
        updateSqlText();
    });

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
        index.setTable(sqlb::ObjectIdentifier(ui->comboTableName->currentData().toString().toStdString()).name());
        index.fields.clear();
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
    sqlb::TablePtr table = pdb.getTableByName(sqlb::ObjectIdentifier(ui->comboTableName->currentData().toString().toStdString()));
    if(!table)
        return;
    ui->tableTableColumns->setRowCount(static_cast<int>(table->fields.size()));
    int tableRows = 0;
    for(const auto& field: table->fields)
    {
        // When we're doing the initial loading and this field already is in the index to edit, then don't add it to the
        // list of table columns. It will be added to the list of index columns in the next step. When this is not the initial
        // loading, the index column list is empty, so this check will always be true.
        if(sqlb::findField(index, field.name()) == index.fields.end())
        {
            // Put the name of the field in the first column
            QTableWidgetItem* name = new QTableWidgetItem(QString::fromStdString(field.name()));
            name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableTableColumns->setItem(tableRows, 0, name);

            // Put the data type in the second column
            QTableWidgetItem* type = new QTableWidgetItem(QString::fromStdString(field.type()));
            type->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableTableColumns->setItem(tableRows, 1, type);

            tableRows++;
        }
    }

    // Set row count to actual count. This is needed for the initial loading, when some rows might have been omitted because they were used in the index
    ui->tableTableColumns->setRowCount(tableRows);

    // Fill the index column list. This is done separately from the table column to include expression columns (these are not found in the original
    // table) and to preserve the order of the index columns
    auto indexFields = index.fields;
    ui->tableIndexColumns->blockSignals(true);
    ui->tableIndexColumns->setRowCount(static_cast<int>(indexFields.size()));
    for(size_t i=0;i<indexFields.size();++i)
    {
        // Put the name of the field in the first column
        QTableWidgetItem* name = new QTableWidgetItem(QString::fromStdString(indexFields.at(i).name()));
        Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        if(indexFields.at(i).expression())
            flags |= Qt::ItemIsEditable;
        name->setFlags(flags);
        ui->tableIndexColumns->setItem(static_cast<int>(i), 0, name);

        // And put a combobox to select the order in which to index the field in the last column
        QComboBox* order = new QComboBox(this);
        order->addItem("");
        order->addItem("ASC");
        order->addItem("DESC");
        order->setCurrentText(QString::fromStdString(indexFields.at(i).order()).toUpper());
        ui->tableIndexColumns->setCellWidget(static_cast<int>(i), 1, order);
        connect(order, &QComboBox::currentTextChanged, this,
                [=](QString new_order)
        {
            auto colnum = sqlb::findField(index, indexFields.at(i).name());
            if(colnum != index.fields.end())
            {
                colnum->setOrder(new_order.toStdString());
                updateSqlText();
            }
        });
    }
    ui->tableIndexColumns->blockSignals(false);

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
    index.fields.emplace_back(
                ui->tableTableColumns->item(row, 0)->text().toStdString(),  // Column name
                false,                                                      // Is expression
                "");                                                        // Order

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

    // If this is an expression column and the action was triggered by a double click event instead of a button click,
    // we won't remove the expression column because it's too likely that this was only done by accident by the user.
    // Instead just open the expression column for editing.
    if(index.fields[static_cast<size_t>(row)].expression() && sender() != ui->buttonFromIndex)
    {
        ui->tableIndexColumns->editItem(ui->tableIndexColumns->item(row, 0));
        return;
    }

    // Remove column from index
    sqlb::removeField(index, ui->tableIndexColumns->item(row, 0)->text().toStdString());

    // Update UI
    updateColumnLists();
}

void EditIndexDialog::checkInput()
{
    // Check if index name is set
    bool valid = true;
    if(ui->editIndexName->text().isEmpty())
        valid = false;

    // Check if a table is selected (this is especially important in the case where there are no tables in the database yet).
    if(ui->comboTableName->currentText().isNull())
        valid = false;

    // Check if index has any columns
    if(index.fields.size() == 0)
        valid = false;

    // Only activate OK button if index data is valid
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);

    // Set the index name and the unique flag
    index.setName(ui->editIndexName->text().toStdString());
    index.setUnique(ui->checkIndexUnique->isChecked());
    index.setWhereExpr(ui->editPartialClause->text().toStdString());
    updateSqlText();
}

void EditIndexDialog::accept()
{
    // When editing an index, delete the old one first
    if(!newIndex)
    {
        if(!pdb.executeSQL("DROP INDEX IF EXISTS " + curIndex.toString()))
        {
            QMessageBox::warning(this, qApp->applicationName(), tr("Deleting the old index failed:\n%1").arg(pdb.lastError()));
            return;
        }
    }

    // Create the new index in the schema of the selected table
    if(pdb.executeSQL(index.sql(sqlb::ObjectIdentifier(ui->comboTableName->currentData().toString().toStdString()).schema())))
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
    ui->sqlTextEdit->setText(QString::fromStdString(index.sql(sqlb::ObjectIdentifier(ui->comboTableName->currentData().toString().toStdString()).schema())));
}

void EditIndexDialog::moveColumnUp()
{
    moveCurrentColumn(false);
}

void EditIndexDialog::moveColumnDown()
{
    moveCurrentColumn(true);
}

void EditIndexDialog::moveCurrentColumn(bool down)
{
    // Get current row number and calculate row number after the movement. Check the values
    int currentRow = ui->tableIndexColumns->currentRow();
    if(currentRow == -1)
        return;
    int newRow = currentRow + (down ? 1 : -1);
    if(newRow < 0)
        return;
    if(newRow >= ui->tableIndexColumns->rowCount())
        return;

    // Swap the columns
    std::swap(index.fields[static_cast<size_t>(currentRow)], index.fields[static_cast<size_t>(newRow)]);

    // Update UI
    updateColumnLists();

    // Select old row at new position
    ui->tableIndexColumns->selectRow(newRow);
}

void EditIndexDialog::addExpressionColumn()
{
    // Check if there already is an empty expression column
    auto field_it = sqlb::findField(index, "");
    int row = static_cast<int>(std::distance(index.fields.begin(), field_it));
    if(field_it == index.fields.end())
    {
        // There is no empty expression column yet, so add one.

        // Add new expression column to the index
        index.fields.emplace_back(
                    "",                          // Column name
                    true,                        // Is expression
                    "");                         // Order

        // Update UI
        updateColumnLists();

        // Get row number of new column
        row = ui->tableIndexColumns->rowCount() - 1;
    }

    // Now we should have the row number of the empty expression column, no matter if it was newly added or it already existed.
    // Select the row for editing
    ui->tableIndexColumns->editItem(ui->tableIndexColumns->item(row, 0));
}
