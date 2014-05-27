#include "EditTableDialog.h"
#include "ui_EditTableDialog.h"
#include "SQLiteSyntaxHighlighter.h"
#include "sqlitetablemodel.h"
#include "sqlitedb.h"

#include <QMessageBox>
#include <QPushButton>
#include <QComboBox>
#include <QDateTime>

EditTableDialog::EditTableDialog(DBBrowserDB* db, const QString& tableName, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditTableDialog),
      pdb(db),
      curTable(tableName),
      m_table(tableName),
      m_bNewTable(true),
      m_sRestorePointName(QString("edittable_%1_save_%2").arg(curTable).arg(QDateTime::currentMSecsSinceEpoch()))
{
    // Create UI
    ui->setupUi(this);
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(itemChanged(QTreeWidgetItem*,int)));

    m_sqliteSyntaxHighlighter = new SQLiteSyntaxHighlighter(ui->sqlTextEdit->document());

    // Editing an existing table?
    if(curTable != "")
    {
        m_bNewTable = false; // we are editing an existing table

        // Existing table, so load and set the current layout
        QString sTablesql = pdb->getObjectByName(curTable).getsql();
        m_table = sqlb::Table::parseSQL(sTablesql);
        populateFields();
    }

    // And create a savepoint
    pdb->setRestorePoint(m_sRestorePointName);

    // Update UI
    ui->editTableName->setText(curTable);
    updateColumnWidth();

    checkInput();
}

EditTableDialog::~EditTableDialog()
{
    delete ui;
}

void EditTableDialog::updateColumnWidth()
{
    ui->treeWidget->setColumnWidth(kName, 190);
    ui->treeWidget->setColumnWidth(kType, 80);
    ui->treeWidget->setColumnWidth(kNotNull, 30);
    ui->treeWidget->setColumnWidth(kPrimaryKey, 30);
    ui->treeWidget->setColumnWidth(kAutoIncrement, 30);
}

void EditTableDialog::populateFields()
{
    // disconnect the itemChanged signal or the table item will
    // be updated while filling the treewidget
    disconnect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this,SLOT(itemChanged(QTreeWidgetItem*,int)));

    ui->treeWidget->clear();
    sqlb::FieldVector fields = m_table.fields();
    foreach(sqlb::FieldPtr f, fields)
    {
        QTreeWidgetItem *tbitem = new QTreeWidgetItem(ui->treeWidget);
        tbitem->setFlags(tbitem->flags() | Qt::ItemIsEditable);
        tbitem->setText(kName, f->name());
        QComboBox* typeBox = new QComboBox(ui->treeWidget);
        typeBox->setProperty("column", f->name());
        typeBox->setEditable(false);
        typeBox->addItems(sqlb::Field::Datatypes);
        int index = typeBox->findText(f->type(), Qt::MatchExactly);
        if(index == -1)
        {
            // non standard named type
            typeBox->addItem(f->type());
            index = typeBox->count() - 1;
        }
        typeBox->setCurrentIndex(index);
        connect(typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypes()));
        ui->treeWidget->setItemWidget(tbitem, kType, typeBox);

        tbitem->setCheckState(kNotNull, f->notnull() ? Qt::Checked : Qt::Unchecked);
        tbitem->setCheckState(kPrimaryKey, f->primaryKey() ? Qt::Checked : Qt::Unchecked);
        tbitem->setCheckState(kAutoIncrement, f->autoIncrement() ? Qt::Checked : Qt::Unchecked);
        tbitem->setText(kDefault, f->defaultValue());
        tbitem->setText(kCheck, f->check());
        ui->treeWidget->addTopLevelItem(tbitem);
    }

    // and reconnect
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(itemChanged(QTreeWidgetItem*,int)));
}

void EditTableDialog::accept()
{
    // Are we editing an already existing table or designing a new one? In the first case there is a table name set,
    // in the latter the current table name is empty
    if(m_bNewTable)
    {
        // Creation of new table
        if(!pdb->executeSQL(m_table.sql()))
        {
            QMessageBox::warning(
                this,
                QApplication::applicationName(),
                tr("Error creating table. Message from database engine:\n%1").arg(pdb->lastErrorMessage));
            return;
        }
    } else {
        // Editing of old table

        // Rename table if necessary
        if(ui->editTableName->text() != curTable)
        {
            if(!pdb->renameTable(curTable, ui->editTableName->text()))
            {
                QMessageBox::warning(this, QApplication::applicationName(), pdb->lastErrorMessage);
                return;
            }
        }
    }

    QDialog::accept();
}

void EditTableDialog::reject()
{    
    // Then rollback to our savepoint
    pdb->revert(m_sRestorePointName);

    QDialog::reject();
}

void EditTableDialog::updateSqlText()
{
    ui->sqlTextEdit->clear();
    ui->sqlTextEdit->insertPlainText(m_table.sql());
}

void EditTableDialog::checkInput()
{
    QString normTableName = ui->editTableName->text().trimmed();
    bool valid = true;
    if(normTableName.isEmpty() || normTableName.contains("`"))
        valid = false;
    if(ui->treeWidget->topLevelItemCount() == 0)
        valid = false;
    m_table.setName(normTableName);
    updateSqlText();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

void EditTableDialog::updateTypes()
{
    QComboBox* typeBox = qobject_cast<QComboBox*>(sender());
    if(typeBox)
    {
        QString type = typeBox->currentText();
        QString column = sender()->property("column").toString();

        int index;
        for(index=0; index < m_table.fields().size(); ++index)
        {
            if(m_table.fields().at(index)->name() == column)
                break;
        }

        m_table.fields().at(index)->setType(type);
        if(!m_bNewTable)
            pdb->renameColumn(curTable, column, m_table.fields().at(index));
        checkInput();
    }
}

void EditTableDialog::itemChanged(QTreeWidgetItem *item, int column)
{
    int index = ui->treeWidget->indexOfTopLevelItem(item);
    if(index < m_table.fields().count())
    {
        sqlb::FieldPtr field = m_table.fields().at(index);
        bool callRenameColumn = false;
        QString oldFieldName = field->name();

        switch(column)
        {
        case kName:
            field->setName(item->text(column));
            qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(item, kType))->setProperty("column", item->text(column));
            if(!m_bNewTable)
                callRenameColumn = true;
            break;
        case kType:
            // see updateTypes() SLOT
            break;
        case kPrimaryKey:
        {
            field->setPrimaryKey(item->checkState(column) == Qt::Checked);
            if(item->checkState(column) == Qt::Checked)
            {
                // this will unset any other autoincrement
                for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
                {
                    QTreeWidgetItem* tbitem = ui->treeWidget->topLevelItem(i);
                    if(tbitem != item)
                        tbitem->setCheckState(kAutoIncrement, Qt::Unchecked);
                }
            } else {
                item->setCheckState(kAutoIncrement, Qt::Unchecked);
            }
            if(!m_bNewTable)
                callRenameColumn = true;
        }
        break;
        case kNotNull:
        {
            // When editing an existing table and trying to set a column to Not Null an extra check is needed
            if(!m_bNewTable && item->checkState(column) == Qt::Checked)
            {
                // Because our renameColumn() function fails when setting a column to Not Null when it already contains some NULL values
                // we need to check for this case and cancel here. Maybe we can think of some way to modify the INSERT INTO ... SELECT statement
                // to at least replace all troublesome NULL values by the default value
                SqliteTableModel m(this, pdb);
                m.setQuery(QString("SELECT COUNT(rowid) FROM `%1` WHERE `%2` IS NULL;").arg(curTable).arg(field->name()));
                if(m.data(m.index(0, 0)).toInt() > 0)
                {
                    // There is a NULL value, so print an error message, uncheck the combobox, and return here
                    QMessageBox::information(this, qApp->applicationName(), tr("There is at least one row with this field set to NULL. "
                                                                               "This makes it impossible to set this flag. Please change the table data first."));
                    item->setCheckState(column, Qt::Unchecked);
                    return;
                }
            }
            field->setNotNull(item->checkState(column) == Qt::Checked);
            if(!m_bNewTable)
                callRenameColumn = true;
        }
        break;
        case kAutoIncrement:
        {
            bool ischecked = item->checkState(column) == Qt::Checked;
            if(ischecked)
            {
                // First check if the contents of this column are all integers. If not this field cannot be set to AI
                if(!m_bNewTable)
                {
                    SqliteTableModel m(this, pdb);
                    m.setQuery(QString("SELECT COUNT(*) FROM `%1` WHERE `%2` <> CAST(`%3` AS INTEGER);").arg(curTable).arg(field->name()).arg(field->name()));
                    if(m.data(m.index(0, 0)).toInt() > 0)
                    {
                        // There is a non-integer value, so print an error message, uncheck the combobox, and return here
                        QMessageBox::information(this, qApp->applicationName(), tr("There is at least one row with a non-integer value in this field. "
                                                                                   "This makes it impossible to set the AI flag. Please change the table data first."));
                        item->setCheckState(column, Qt::Unchecked);
                        return;
                    }
                }

                // Make sure the data type is set to integer
                QComboBox* comboType = qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(item, kType));
                comboType->setCurrentIndex(comboType->findText("INTEGER"));
                item->setCheckState(kPrimaryKey, Qt::Checked);

                // this will unset all other primary keys
                // there can't be more than one autoincrement pk
                for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
                {
                    QTreeWidgetItem* tbitem = ui->treeWidget->topLevelItem(i);
                    if(tbitem != item)
                    {
                        tbitem->setCheckState(kAutoIncrement, Qt::Unchecked);
                        tbitem->setCheckState(kPrimaryKey, Qt::Unchecked);
                    }
                }
            }
            field->setAutoIncrement(ischecked);

            if(!m_bNewTable)
                callRenameColumn = true;
        }
        break;
        case kDefault:
            field->setDefaultValue(item->text(column));
            if(!m_bNewTable)
                callRenameColumn = true;
            break;
        case kCheck:
            field->setCheck(item->text(column));
            if(!m_bNewTable)
                callRenameColumn = true;
            break;
        }

        if(callRenameColumn)
            pdb->renameColumn(curTable, oldFieldName, field);
    }

    checkInput();
}

void EditTableDialog::addField()
{
    QTreeWidgetItem *tbitem = new QTreeWidgetItem(ui->treeWidget);
    tbitem->setFlags(tbitem->flags() | Qt::ItemIsEditable);
    tbitem->setText(kName, "Field" + QString::number(ui->treeWidget->topLevelItemCount()));
    QComboBox* typeBox = new QComboBox(ui->treeWidget);
    typeBox->setProperty("column", tbitem->text(kName));
    typeBox->setEditable(false);
    typeBox->addItems(sqlb::Field::Datatypes);
    ui->treeWidget->setItemWidget(tbitem, kType, typeBox);
    connect(typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypes()));

    tbitem->setCheckState(kNotNull, Qt::Unchecked);
    tbitem->setCheckState(kPrimaryKey, Qt::Unchecked);
    tbitem->setCheckState(kAutoIncrement, Qt::Unchecked);
    ui->treeWidget->addTopLevelItem(tbitem);

    // add field to table object
    sqlb::FieldPtr f(new sqlb::Field(
                      tbitem->text(kName),
                      typeBox->currentText()
                      ));
    m_table.addField(f);

    // Actually add the new column to the table if we're editing an existing table
    if(!m_bNewTable)
        pdb->addColumn(curTable, f);

    checkInput();
}

void EditTableDialog::removeField()
{
    // Is there any item selected to delete?
    if(!ui->treeWidget->currentItem())
        return;

    // Are we creating a new table or editing an old one?
    if(m_bNewTable)
    {
        // Creating a new one

        // Just delete that item. At this point there is no DB table to edit or data to be lost anyway
        sqlb::FieldVector fields = m_table.fields();
        fields.remove(ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem()));
        m_table.setFields(fields);
        delete ui->treeWidget->currentItem();
    } else {
        // Editing an old one

        // Ask user wether he really wants to delete that column
        QString msg = tr("Are you sure you want to delete the field '%1'?\nAll data currently stored in this field will be lost.").arg(ui->treeWidget->currentItem()->text(0));
        if(QMessageBox::warning(this, QApplication::applicationName(), msg, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape) == QMessageBox::Yes)
        {
            if(!pdb->renameColumn(curTable, ui->treeWidget->currentItem()->text(0), sqlb::FieldPtr()))
            {
                QMessageBox::warning(0, QApplication::applicationName(), pdb->lastErrorMessage);
            } else {
                //relayout
                QString sTablesql = pdb->getObjectByName(curTable).getsql();
                m_table = sqlb::Table::parseSQL(sTablesql);
                populateFields();
            }
        }
    }

    checkInput();
}

void EditTableDialog::fieldSelectionChanged()
{
    bool hasSelection = ui->treeWidget->selectionModel()->hasSelection();

    // Enable the remove and the move up/down buttons if a field is selected, disable it otherwise
    ui->removeFieldButton->setEnabled(hasSelection);
    ui->buttonMoveUp->setEnabled(hasSelection);
    ui->buttonMoveDown->setEnabled(hasSelection);

    // If the selected line is the first one disable the move up button, it it's the last one disable the move down button
    if(hasSelection)
    {
        ui->buttonMoveUp->setEnabled(ui->treeWidget->selectionModel()->currentIndex().row() != 0);
        ui->buttonMoveDown->setEnabled(ui->treeWidget->selectionModel()->currentIndex().row() != ui->treeWidget->topLevelItemCount() - 1);
    }
}

void EditTableDialog::moveUp()
{
    moveCurrentField(false);
}

void EditTableDialog::moveDown()
{
    moveCurrentField(true);
}

void EditTableDialog::moveCurrentField(bool down)
{
    int currentRow = ui->treeWidget->currentIndex().row();
    int newRow = currentRow + (down ? 1 : -1);

    // Are we creating a new table or editing an old one?
    if(m_bNewTable)
    {
        // Creating a new one

        // Save the combobox first by making a copy
        QComboBox* oldCombo = qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(ui->treeWidget->topLevelItem(currentRow), kType));
        QComboBox* newCombo = new QComboBox(ui->treeWidget);
        newCombo->setProperty("column", oldCombo->property("column"));
        connect(newCombo, SIGNAL(activated(int)), this, SLOT(updateTypes()));
        newCombo->setEditable(false);
        for(int i=0; i < oldCombo->count(); ++i)
            newCombo->addItem(oldCombo->itemText(i));
        newCombo->setCurrentIndex(oldCombo->currentIndex());

        // Now, just remove the item and insert it at it's new position, then restore the combobox
        QTreeWidgetItem* item = ui->treeWidget->takeTopLevelItem(currentRow);
        ui->treeWidget->insertTopLevelItem(newRow, item);
        ui->treeWidget->setItemWidget(item, kType, newCombo);

        // Select the old item at its new position
        ui->treeWidget->setCurrentIndex(ui->treeWidget->currentIndex().sibling(newRow, 0));

        // Finally update the table SQL
        sqlb::FieldVector fields = m_table.fields();
        std::swap(fields[newRow], fields[currentRow]);
        m_table.setFields(fields);
    } else {
        // Editing an old one

        // Move the actual column
        if(!pdb->renameColumn(
                    curTable,
                    ui->treeWidget->currentItem()->text(0),
                    m_table.fields().at(ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem())),
                    (down ? 1 : -1)
                ))
        {
            QMessageBox::warning(0, QApplication::applicationName(), pdb->lastErrorMessage);
        } else {
            // Reload table SQL
            QString sTablesql = pdb->getObjectByName(curTable).getsql();
            m_table = sqlb::Table::parseSQL(sTablesql);
            populateFields();

            // Select old item at new position
            ui->treeWidget->setCurrentIndex(ui->treeWidget->indexAt(QPoint(1, 1)).sibling(newRow, 0));
        }
    }

    // Update the SQL preview
    updateSqlText();
}
