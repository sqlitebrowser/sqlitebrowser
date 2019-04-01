#include "EditTableDialog.h"
#include "Settings.h"
#include "ForeignKeyEditorDelegate.h"
#include "ui_EditTableDialog.h"
#include "sqlitetablemodel.h"
#include "sqlitedb.h"

#include <QMessageBox>
#include <QPushButton>
#include <QComboBox>
#include <QDateTime>
#include <QKeyEvent>
#include <algorithm>

EditTableDialog::EditTableDialog(DBBrowserDB& db, const sqlb::ObjectIdentifier& tableName, bool createTable, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditTableDialog),
      pdb(db),
      curTable(tableName),
      m_table(tableName.name()),
      m_bNewTable(createTable),
      m_sRestorePointName(pdb.generateSavepointName("edittable"))
{
    // Create UI
    ui->setupUi(this);
    ui->widgetExtension->setVisible(false);
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(itemChanged(QTreeWidgetItem*,int)));

    // Set item delegate for foreign key column
    m_fkEditorDelegate = new ForeignKeyEditorDelegate(db, m_table, this);
    ui->treeWidget->setItemDelegateForColumn(kForeignKey, m_fkEditorDelegate);

    // Editing an existing table?
    if(m_bNewTable == false)
    {
        // Existing table, so load and set the current layout
        m_table = *(pdb.getObjectByName<sqlb::Table>(curTable));
        ui->labelEditWarning->setVisible(!m_table.fullyParsed());

        // Initialise the list of tracked columns for table layout changes
        for(const auto& field : m_table.fields)
            trackColumns[field.name()] = field.name();

        // Set without rowid checkbox and schema dropdown. No need to trigger any events here as we're only loading a table exactly as it is stored by SQLite, so no need
        // for error checking etc.
        ui->checkWithoutRowid->blockSignals(true);
        ui->checkWithoutRowid->setChecked(m_table.isWithoutRowidTable());
        ui->checkWithoutRowid->blockSignals(false);
        ui->comboSchema->blockSignals(true);
        ui->comboSchema->addItems(pdb.schemata.keys());         // Load list of database schemata
        ui->comboSchema->setCurrentText(curTable.schema());
        ui->comboSchema->blockSignals(false);

        populateFields();
    } else {
        ui->comboSchema->addItems(pdb.schemata.keys());         // Load list of database schemata
        ui->comboSchema->setCurrentText("main");                // Always create tables in the main schema by default
        ui->labelEditWarning->setVisible(false);
    }

    // And create a savepoint
    pdb.setSavepoint(m_sRestorePointName);

    // Update UI
    ui->editTableName->setText(curTable.name());
    updateColumnWidth();

    checkInput();
}

EditTableDialog::~EditTableDialog()
{
    delete ui;
}

void EditTableDialog::keyPressEvent(QKeyEvent *evt)
{
    if((evt->modifiers() & Qt::ControlModifier)
            && (evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return))
    {
        accept();
        return;
    }
    if(evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return)
        return;
    QDialog::keyPressEvent(evt);
}

void EditTableDialog::updateColumnWidth()
{
    ui->treeWidget->setColumnWidth(kName, 190);
    ui->treeWidget->setColumnWidth(kType, 100);
    ui->treeWidget->setColumnWidth(kNotNull, 30);
    ui->treeWidget->setColumnWidth(kPrimaryKey, 30);
    ui->treeWidget->setColumnWidth(kAutoIncrement, 30);
    ui->treeWidget->setColumnWidth(kUnique, 30);
    ui->treeWidget->setColumnWidth(kForeignKey, 500);
}

void EditTableDialog::populateFields()
{
    // disconnect the itemChanged signal or the table item will
    // be updated while filling the treewidget
    disconnect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this,SLOT(itemChanged(QTreeWidgetItem*,int)));

    ui->treeWidget->clear();
    const auto& fields = m_table.fields;
    QStringList pk = m_table.primaryKey();
    for(const sqlb::Field& f : fields)
    {
        QTreeWidgetItem *tbitem = new QTreeWidgetItem(ui->treeWidget);
        tbitem->setFlags(tbitem->flags() | Qt::ItemIsEditable);
        tbitem->setText(kName, f.name());
        QComboBox* typeBox = new QComboBox(ui->treeWidget);
        typeBox->setProperty("column", f.name());
        typeBox->setEditable(true);
        typeBox->addItems(DBBrowserDB::Datatypes);
        int index = typeBox->findText(f.type(), Qt::MatchExactly);
        if(index == -1)
        {
            // non standard named type
            typeBox->addItem(f.type());
            index = typeBox->count() - 1;
        }
        typeBox->setCurrentIndex(index);
        typeBox->installEventFilter(this);
        connect(typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypes()));
        ui->treeWidget->setItemWidget(tbitem, kType, typeBox);

        tbitem->setCheckState(kNotNull, f.notnull() ? Qt::Checked : Qt::Unchecked);
        tbitem->setCheckState(kPrimaryKey, contains(pk, f.name()) ? Qt::Checked : Qt::Unchecked);
        tbitem->setCheckState(kAutoIncrement, f.autoIncrement() ? Qt::Checked : Qt::Unchecked);
        tbitem->setCheckState(kUnique, f.unique() ? Qt::Checked : Qt::Unchecked);

        // For the default value check if it is surrounded by parentheses and if that's the case
        // add a '=' character before the entire string to match the input format we're expecting
        // from the user when using functions in the default value field.
        if(f.defaultValue().startsWith('(') && f.defaultValue().endsWith(')'))
            tbitem->setText(kDefault, "=" + f.defaultValue());
        else
            tbitem->setText(kDefault, f.defaultValue());

        tbitem->setText(kCheck, f.check());

        auto fk = std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(m_table.constraint({f.name()}, sqlb::Constraint::ForeignKeyConstraintType));
        if(fk)
            tbitem->setText(kForeignKey, fk->toString());
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
        if(!pdb.executeSQL(m_table.sql(ui->comboSchema->currentText())))
        {
            QMessageBox::warning(
                this,
                QApplication::applicationName(),
                tr("Error creating table. Message from database engine:\n%1").arg(pdb.lastError()));
            return;
        }
    } else {
        // Editing of old table

        // Apply all changes to the actual table in the database
        if(!pdb.alterTable(curTable, m_table, trackColumns, ui->comboSchema->currentText()))
        {
            QMessageBox::warning(this, QApplication::applicationName(), pdb.lastError());
            return;
        }
    }

    QDialog::accept();
}

void EditTableDialog::reject()
{    
    // Then rollback to our savepoint
    pdb.revertToSavepoint(m_sRestorePointName);

    QDialog::reject();
}

void EditTableDialog::updateSqlText()
{
    ui->sqlTextEdit->setText(m_table.sql(ui->comboSchema->currentText()));
}

void EditTableDialog::checkInput()
{
    QString normTableName = ui->editTableName->text();
    bool valid = true;
    if(normTableName.isEmpty())
        valid = false;
    if(ui->treeWidget->topLevelItemCount() == 0)
        valid = false;
    if (normTableName != m_table.name()) {
        const QString oldTableName = m_table.name();
        m_table.setName(normTableName);
        m_fkEditorDelegate->updateTablesList(oldTableName);

        // update fk's that refer to table itself recursively
        const auto& fields = m_table.fields;
        for(const sqlb::Field& f : fields) {
            auto fk = std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(m_table.constraint({f.name()}, sqlb::Constraint::ForeignKeyConstraintType));
            if(fk && oldTableName == fk->table())
                fk->setTable(normTableName);
        }

        populateFields();
    }

    updateSqlText();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

void EditTableDialog::updateTypes(QObject *object)
{
    QComboBox* typeBox = qobject_cast<QComboBox*>(object);
    if(typeBox)
    {
        QString type = typeBox->currentText();
        QString column = typeBox->property("column").toString();

        for(size_t index=0; index < m_table.fields.size(); ++index)
        {
            if(m_table.fields.at(index).name() == column)
            {
                m_table.fields.at(index).setType(type);
                break;
            }
        }

        checkInput();
    }
}

void EditTableDialog::updateTypes()
{
    updateTypes(sender());
}

bool EditTableDialog::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::FocusOut)
    {
        updateTypes(object);
    }
    return false;
}

void EditTableDialog::itemChanged(QTreeWidgetItem *item, int column)
{
    int index = ui->treeWidget->indexOfTopLevelItem(item);
    if(index < static_cast<int>(m_table.fields.size()))
    {
        sqlb::Field& field = m_table.fields.at(index);
        QString oldFieldName = field.name();

        switch(column)
        {
        case kName:
            {
            // When a field of that name already exists, show a warning to the user and don't apply the new name. This is done by searching for an
            // existing field with the new name. There is one exception, however, to this rule: if the field that we have found is the same field
            // as the one we are currently trying to rename, this means the user is trying to rename the field to essentially the same name but
            // with different case. Example: if I rename column 'COLUMN' to 'column', findField() is going to return the current field number
            // because it's doing a case-independent search and it can't return another field number because SQLite prohibits duplicate field
            // names (no matter the case). So when this happens we just allow the renaming because there's no harm to be expected from it.
            auto foundField = sqlb::findField(m_table, item->text(column));
            if(foundField != m_table.fields.end() && foundField-m_table.fields.begin() != index)
            {
                QMessageBox::warning(this, qApp->applicationName(), tr("There already is a field with that name. Please rename it first or choose a different "
                                                                       "name for this field."));
                // Reset the name to the old value but avoid calling this method again for that automatic change
                ui->treeWidget->blockSignals(true);
                item->setText(column, oldFieldName);
                ui->treeWidget->blockSignals(false);
                return;
            }

            // When editing an exiting table, check if any foreign keys would cause trouble in case this name is edited
            if(!m_bNewTable)
            {
                QStringList pk = m_table.primaryKey();
                for(const sqlb::ObjectPtr& fkobj : pdb.schemata[curTable.schema()].values("table"))
                {
                    auto fks = std::dynamic_pointer_cast<sqlb::Table>(fkobj)->constraints(QStringList(), sqlb::Constraint::ForeignKeyConstraintType);
                    for(const sqlb::ConstraintPtr& fkptr : fks)
                    {
                        auto fk = std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(fkptr);
                        if(fk->table() == m_table.name())
                        {
                            if(fk->columns().contains(field.name()) || contains(pk, field.name()))
                            {
                                QMessageBox::warning(this, qApp->applicationName(), tr("This column is referenced in a foreign key in table %1 and thus "
                                                                                       "its name cannot be changed.")
                                                     .arg(fkobj->name()));
                                // Reset the name to the old value but avoid calling this method again for that automatic change
                                ui->treeWidget->blockSignals(true);
                                item->setText(column, oldFieldName);
                                ui->treeWidget->blockSignals(false);
                                return;
                            }
                        }
                    }
                }
            }

            field.setName(item->text(column));
            m_table.renameKeyInAllConstraints(oldFieldName, item->text(column));
            qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(item, kType))->setProperty("column", item->text(column));

            // Update the field name in the map of old column names to new column names
            if(!m_bNewTable)
            {
                for(const auto& key : trackColumns.keys())
                {
                    if(trackColumns[key] == oldFieldName)
                        trackColumns[key] = field.name();
                }
            }
            } break;
        case kType:
            // see updateTypes() SLOT
            break;
        case kPrimaryKey:
        {
            // Check if there already is a primary key
            if(m_table.constraint(QStringList(), sqlb::Constraint::PrimaryKeyConstraintType))
            {
                // There already is a primary key for this table. So edit that one as there always can only be one primary key anyway.
                QStringList& pk = m_table.primaryKeyRef();
                if(item->checkState(column) == Qt::Checked)
                    pk.push_back(field.name());
                else
                    pk.erase(std::remove(pk.begin(), pk.end(), field.name()), pk.end());
            } else if(item->checkState(column) == Qt::Checked) {
                // There is no primary key in the table yet. This means we need to add a default one.
                m_table.addConstraint({field.name()}, sqlb::ConstraintPtr(new sqlb::PrimaryKeyConstraint()));
            }

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
                SqliteTableModel m(pdb, this);
                m.setQuery(QString("SELECT COUNT(%1) FROM %2 WHERE coalesce(NULL,%3) IS NULL;")
                           .arg(sqlb::escapeIdentifier(pdb.getObjectByName<sqlb::Table>(curTable)->rowidColumns()).join(","))
                           .arg(curTable.toString())
                           .arg(sqlb::escapeIdentifier(field.name())));
                if(!m.completeCache())
                {
                    // If we couldn't load all data because the cancel button was clicked, just unset the checkbox again and stop.
                    item->setCheckState(column, Qt::Unchecked);
                    return;
                }
                if(m.data(m.index(0, 0)).toInt() > 0)
                {
                    // There is a NULL value, so print an error message, uncheck the combobox, and return here
                    QMessageBox::information(this, qApp->applicationName(), tr("There is at least one row with this field set to NULL. "
                                                                               "This makes it impossible to set this flag. Please change the table data first."));
                    item->setCheckState(column, Qt::Unchecked);
                    return;
                }
            }
            field.setNotNull(item->checkState(column) == Qt::Checked);
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
                    SqliteTableModel m(pdb, this);
                    m.setQuery(QString("SELECT COUNT(*) FROM %1 WHERE %2 <> CAST(%3 AS INTEGER);")
                               .arg(curTable.toString())
                               .arg(sqlb::escapeIdentifier(field.name()))
                               .arg(sqlb::escapeIdentifier(field.name())));
                    if(!m.completeCache())
                    {
                        // If we couldn't load all data because the cancel button was clicked, just unset the checkbox again and stop.
                        item->setCheckState(column, Qt::Unchecked);
                        return;
                    }
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
            field.setAutoIncrement(ischecked);
        }
        break;
        case kUnique:
        {
            // When editing an existing table and trying to set a column to unique an extra check is needed
            if(!m_bNewTable && item->checkState(column) == Qt::Checked)
            {
                // Because our renameColumn() function fails when setting a column to unique when it already contains the same values
                SqliteTableModel m(pdb, this);
                m.setQuery(QString("SELECT COUNT(%2) FROM %1;").arg(curTable.toString()).arg(sqlb::escapeIdentifier(field.name())));
                if(!m.completeCache())
                {
                    // If we couldn't load all data because the cancel button was clicked, just unset the checkbox again and stop.
                    item->setCheckState(column, Qt::Unchecked);
                    return;
                }
                int rowcount = m.data(m.index(0, 0)).toInt();
                m.setQuery(QString("SELECT COUNT(DISTINCT %2) FROM %1;").arg(curTable.toString()).arg(sqlb::escapeIdentifier(field.name())));
                if(!m.completeCache())
                {
                    // If we couldn't load all data because the cancel button was clicked, just unset the checkbox again and stop.
                    item->setCheckState(column, Qt::Unchecked);
                    return;
                }
                int uniquecount = m.data(m.index(0, 0)).toInt();
                if(rowcount != uniquecount)
                {
                    // There is a NULL value, so print an error message, uncheck the combobox, and return here
                    QMessageBox::information(this, qApp->applicationName(), tr("Column '%1' has duplicate data.\n").arg(field.name())
                                             + tr("This makes it impossible to enable the 'Unique' flag. Please remove the duplicate data, which will allow the 'Unique' flag to then be enabled."));
                    item->setCheckState(column, Qt::Unchecked);
                    return;
                }
            }
            field.setUnique(item->checkState(column) == Qt::Checked);
        }
        break;
        case kDefault:
        {
            QString new_value = item->text(column);
            // If the default value isn't a SQL keyword perform an extra check: If it isn't numeric but doesn't start and end with quotes,
            // add the quotes
            if(new_value.size() && new_value.compare("null", Qt::CaseInsensitive) &&
                    new_value.compare("current_time", Qt::CaseInsensitive) &&
                    new_value.compare("current_date", Qt::CaseInsensitive) &&
                    new_value.compare("current_timestamp", Qt::CaseInsensitive))
            {
                QChar first_char = new_value.trimmed().at(0);
                if(!((first_char == '\'' || first_char == '"') && new_value.trimmed().endsWith(first_char)))
                {
                    bool is_numeric;
                    new_value.toDouble(&is_numeric);
                    if(!is_numeric)
                    {
                        if(new_value.trimmed().startsWith("=(") && new_value.trimmed().endsWith(')'))
                        {
                            new_value = new_value.trimmed().mid(1); // Leave the brackets as they are needed for a valid SQL expression
                        } else {
                            new_value = QString("'%1'").arg(new_value.replace("'", "''"));
                            item->setText(column, new_value);
                        }
                    }
                }
            }
            field.setDefaultValue(new_value);
        }
        break;
        case kCheck:
            field.setCheck(item->text(column));
            break;
        case kForeignKey:
            // handled in delegate
            break;
        }
    }

    checkInput();
}

void EditTableDialog::addField()
{
    QTreeWidgetItem *tbitem = new QTreeWidgetItem(ui->treeWidget);
    tbitem->setFlags(tbitem->flags() | Qt::ItemIsEditable);

    // Find an unused name for the field by starting with 'Fieldx' where x is the number of fields + 1.
    // If this name happens to exist already, increase x by one until we find an unused name.
    {
        unsigned int field_number = ui->treeWidget->topLevelItemCount();
        QString field_name;
        do
        {
            field_name = "Field" + QString::number(field_number);
            field_number++;
        } while(sqlb::findField(m_table, field_name) != m_table.fields.end());
        tbitem->setText(kName, field_name);
    }

    QComboBox* typeBox = new QComboBox(ui->treeWidget);
    typeBox->setProperty("column", tbitem->text(kName));
    typeBox->setEditable(true);
    typeBox->addItems(DBBrowserDB::Datatypes);

    int defaultFieldTypeIndex = Settings::getValue("db", "defaultfieldtype").toInt();

    if (defaultFieldTypeIndex < DBBrowserDB::Datatypes.count())
    {
        typeBox->setCurrentIndex(defaultFieldTypeIndex);
    }

    ui->treeWidget->setItemWidget(tbitem, kType, typeBox);
    typeBox->installEventFilter(this);
    connect(typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypes()));

    tbitem->setCheckState(kNotNull, Qt::Unchecked);
    tbitem->setCheckState(kPrimaryKey, Qt::Unchecked);
    tbitem->setCheckState(kAutoIncrement, Qt::Unchecked);
    tbitem->setCheckState(kUnique, Qt::Unchecked);

    ui->treeWidget->addTopLevelItem(tbitem);
    ui->treeWidget->scrollToBottom();
    ui->treeWidget->editItem(tbitem, 0);

    // add field to table object
    m_table.fields.emplace_back(tbitem->text(kName), typeBox->currentText());

    // Add the new column to the list of tracked columns to indicate it has been added
    if(!m_bNewTable)
        trackColumns.insert(QString(), tbitem->text(kName));

    checkInput();
}

void EditTableDialog::removeField()
{
    // Is there any item selected to delete?
    if(!ui->treeWidget->currentItem())
        return;

    // If we are editing an existing table, ask the user for confirmation
    if(!m_bNewTable)
    {
        QString msg = tr("Are you sure you want to delete the field '%1'?\nAll data currently stored in this field will be lost.").arg(ui->treeWidget->currentItem()->text(0));
        if(QMessageBox::warning(this, QApplication::applicationName(), msg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            return;

        // Update the map of tracked columns to indicate the column is deleted
        QString name = ui->treeWidget->currentItem()->text(0);
        for(const auto& key : trackColumns.keys())
        {
            if(trackColumns[key] == name)
                trackColumns[key] = QString();
        }
    }

    // Just delete that item. At this point there is no DB table to edit or data to be lost anyway
    m_table.fields.erase(m_table.fields.begin() + ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem()));
    m_table.removeKeyFromAllConstraints(ui->treeWidget->currentItem()->text(kName));
    delete ui->treeWidget->currentItem();

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

    // Save the combobox first by making a copy
    QComboBox* oldCombo = qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(ui->treeWidget->topLevelItem(currentRow), kType));
    QComboBox* newCombo = new QComboBox(ui->treeWidget);
    newCombo->setProperty("column", oldCombo->property("column"));
    newCombo->installEventFilter(this);
    connect(newCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypes()));
    newCombo->setEditable(true);
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
    std::swap(m_table.fields[newRow], m_table.fields[currentRow]);

    // Update the SQL preview
    updateSqlText();
}

void EditTableDialog::setWithoutRowid(bool without_rowid)
{
    if(without_rowid)
    {
        // Before setting the without rowid flag, first perform a check to see if the table meets all the required criteria for without rowid tables
        auto pks = m_table.primaryKey();
        for(const auto& pk_name : pks)
        {
            auto pk = sqlb::findField(m_table, pk_name);
            if(pk == m_table.fields.end() || pk->autoIncrement())
            {
                QMessageBox::information(this, QApplication::applicationName(),
                                         tr("Please add a field which meets the following criteria before setting the without rowid flag:\n"
                                            " - Primary key flag set\n"
                                            " - Auto increment disabled"));

                // Reset checkbox state to unchecked. Block any signals while doing this in order to avoid an extra call to
                // this function being triggered.
                ui->checkWithoutRowid->blockSignals(true);
                ui->checkWithoutRowid->setChecked(false);
                ui->checkWithoutRowid->blockSignals(false);
                return;
            }
        }

        // If it does, override the the rowid column names of the table object with the names of the primary keys.
        m_table.setRowidColumns(pks);
    } else {
        // If the without rowid flag is unset no further checks are required. Just set the rowid column name back to "_rowid_"
        m_table.setRowidColumns({"_rowid_"});
    }

    // Update the SQL preview
    updateSqlText();
}

void EditTableDialog::changeSchema(const QString& /*schema*/)
{
    // Update the SQL preview
    updateSqlText();
}
