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

EditTableDialog::EditTableDialog(DBBrowserDB& db, const QString& tableName, bool createTable, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditTableDialog),
      pdb(db),
      curTable(tableName),
      m_table(tableName),
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
        m_table = *(pdb.getObjectByName(curTable).dynamicCast<sqlb::Table>());
        ui->labelEditWarning->setVisible(!m_table.fullyParsed());

        // Set without rowid and temporary checkboxex. No need to trigger any events here as we're only loading a table exactly as it is stored by SQLite, so no need
        // for error checking etc.
        ui->checkWithoutRowid->blockSignals(true);
        ui->checkWithoutRowid->setChecked(m_table.isWithoutRowidTable());
        ui->checkTemporary->setChecked(m_table.isTemporary());
        ui->checkWithoutRowid->blockSignals(false);

        populateFields();
    } else {
        ui->labelEditWarning->setVisible(false);
    }

    // And create a savepoint
    pdb.setSavepoint(m_sRestorePointName);

    // Update UI
    ui->editTableName->setText(curTable);
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
        emit accept();
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
    sqlb::FieldVector fields = m_table.fields();
    sqlb::FieldVector pk = m_table.primaryKey();
    foreach(sqlb::FieldPtr f, fields)
    {
        QTreeWidgetItem *tbitem = new QTreeWidgetItem(ui->treeWidget);
        tbitem->setFlags(tbitem->flags() | Qt::ItemIsEditable);
        tbitem->setText(kName, f->name());
        QComboBox* typeBox = new QComboBox(ui->treeWidget);
        typeBox->setProperty("column", f->name());
        typeBox->setEditable(true);
        typeBox->addItems(sqlb::Field::Datatypes);
        int index = typeBox->findText(f->type(), Qt::MatchExactly);
        if(index == -1)
        {
            // non standard named type
            typeBox->addItem(f->type());
            index = typeBox->count() - 1;
        }
        typeBox->setCurrentIndex(index);
        typeBox->installEventFilter(this);
        connect(typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypes()));
        ui->treeWidget->setItemWidget(tbitem, kType, typeBox);

        tbitem->setCheckState(kNotNull, f->notnull() ? Qt::Checked : Qt::Unchecked);
        tbitem->setCheckState(kPrimaryKey, pk.contains(f) ? Qt::Checked : Qt::Unchecked);
        tbitem->setCheckState(kAutoIncrement, f->autoIncrement() ? Qt::Checked : Qt::Unchecked);
        tbitem->setCheckState(kUnique, f->unique() ? Qt::Checked : Qt::Unchecked);

        // For the default value check if it is surrounded by parentheses and if that's the case
        // add a '=' character before the entire string to match the input format we're expecting
        // from the user when using functions in the default value field.
        if(f->defaultValue().startsWith('(') && f->defaultValue().endsWith(')'))
            tbitem->setText(kDefault, "=" + f->defaultValue());
        else
            tbitem->setText(kDefault, f->defaultValue());

        tbitem->setText(kCheck, f->check());

        QSharedPointer<sqlb::ForeignKeyClause> fk = m_table.constraint({f}, sqlb::Constraint::ForeignKeyConstraintType).dynamicCast<sqlb::ForeignKeyClause>();
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
        if(!pdb.executeSQL(m_table.sql()))
        {
            QMessageBox::warning(
                this,
                QApplication::applicationName(),
                tr("Error creating table. Message from database engine:\n%1").arg(pdb.lastError()));
            return;
        }
    } else {
        // Editing of old table

        // Rename table if necessary
        if(ui->editTableName->text() != curTable)
        {
            if(!pdb.renameTable(curTable, ui->editTableName->text()))
            {
                QMessageBox::warning(this, QApplication::applicationName(), pdb.lastError());
                return;
            }
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
    ui->sqlTextEdit->setText(m_table.sql());
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

        bool fksEnabled = (pdb.getPragma("foreign_keys") == "1");

        // update fk's that refer to table itself recursively
        sqlb::FieldVector fields = m_table.fields();
        foreach(sqlb::FieldPtr f, fields) {
            QSharedPointer<sqlb::ForeignKeyClause> fk = m_table.constraint({f}, sqlb::Constraint::ForeignKeyConstraintType).dynamicCast<sqlb::ForeignKeyClause>();
            if(!fk.isNull()) {
                if (oldTableName == fk->table()) {
                    fk->setTable(normTableName);
                    if(!fksEnabled)
                        pdb.renameColumn(curTable, m_table, f->name(), f, 0);
                }
            }
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

        int index;
        for(index=0; index < m_table.fields().size(); ++index)
        {
            if(m_table.fields().at(index)->name() == column)
                break;
        }

        m_table.fields().at(index)->setType(type);
        if(!m_bNewTable)
            pdb.renameColumn(curTable, m_table, column, m_table.fields().at(index));
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
    if(index < m_table.fields().count())
    {
        sqlb::FieldPtr field = m_table.fields().at(index);
        bool callRenameColumn = false;
        QString oldFieldName = field->name();

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
            int foundField = m_table.findField(item->text(column));
            if(foundField != -1 && foundField != index)
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
                sqlb::FieldVector pk = m_table.primaryKey();
                foreach(const sqlb::ObjectPtr& fkobj, pdb.objMap.values("table"))
                {
                    QList<sqlb::ConstraintPtr> fks = fkobj.dynamicCast<sqlb::Table>()->constraints(sqlb::FieldVector(), sqlb::Constraint::ForeignKeyConstraintType);
                    foreach(sqlb::ConstraintPtr fkptr, fks)
                    {
                        QSharedPointer<sqlb::ForeignKeyClause> fk = fkptr.dynamicCast<sqlb::ForeignKeyClause>();
                        if(fk->table() == m_table.name())
                        {
                            if(fk->columns().contains(field->name()) || pk.contains(field))
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

            field->setName(item->text(column));
            qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(item, kType))->setProperty("column", item->text(column));
            if(!m_bNewTable)
                callRenameColumn = true;
            } break;
        case kType:
            // see updateTypes() SLOT
            break;
        case kPrimaryKey:
        {
            // Check if there already is a primary key
            if(m_table.constraint(sqlb::FieldVector(), sqlb::Constraint::PrimaryKeyConstraintType))
            {
                // There already is a primary key for this table. So edit that one as there always can only be one primary key anyway.
                sqlb::FieldVector& pk = m_table.primaryKeyRef();
                if(item->checkState(column) == Qt::Checked)
                    pk.push_back(field);
                else
                    pk.removeAll(field);
            } else if(item->checkState(column) == Qt::Checked) {
                // There is no primary key in the table yet. This means we need to add a default one.
                m_table.addConstraint({field}, sqlb::ConstraintPtr(new sqlb::PrimaryKeyConstraint()));
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
                SqliteTableModel m(pdb, this);
                m.setQuery(QString("SELECT COUNT(%1) FROM %2 WHERE %3 IS NULL;")
                           .arg(sqlb::escapeIdentifier(pdb.getObjectByName(curTable).dynamicCast<sqlb::Table>()->rowidColumn()))
                           .arg(sqlb::escapeIdentifier(curTable))
                           .arg(sqlb::escapeIdentifier(field->name())));
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
                    SqliteTableModel m(pdb, this);
                    m.setQuery(QString("SELECT COUNT(*) FROM %1 WHERE %2 <> CAST(%3 AS INTEGER);")
                               .arg(sqlb::escapeIdentifier(curTable))
                               .arg(sqlb::escapeIdentifier(field->name()))
                               .arg(sqlb::escapeIdentifier(field->name())));
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
        case kUnique:
        {
            // When editing an existing table and trying to set a column to unique an extra check is needed
            if(!m_bNewTable && item->checkState(column) == Qt::Checked)
            {
                // Because our renameColumn() function fails when setting a column to unique when it already contains the same values
                SqliteTableModel m(pdb, this);
                m.setQuery(QString("SELECT COUNT(%2) FROM %1;").arg(sqlb::escapeIdentifier(curTable)).arg(sqlb::escapeIdentifier(field->name())));
                int rowcount = m.data(m.index(0, 0)).toInt();
                m.setQuery(QString("SELECT COUNT(DISTINCT %2) FROM %1;").arg(sqlb::escapeIdentifier(curTable)).arg(sqlb::escapeIdentifier(field->name())));
                int uniquecount = m.data(m.index(0, 0)).toInt();
                if(rowcount != uniquecount)
                {
                    // There is a NULL value, so print an error message, uncheck the combobox, and return here
                    QMessageBox::information(this, qApp->applicationName(), tr("Column '%1' has no unique data.\n").arg(field->name())
                                             + tr("This makes it impossible to set this flag. Please change the table data first."));
                    item->setCheckState(column, Qt::Unchecked);
                    return;
                }
            }
            field->setUnique(item->checkState(column) == Qt::Checked);

            if(!m_bNewTable)
                callRenameColumn = true;
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
            field->setDefaultValue(new_value);
            if(!m_bNewTable)
                callRenameColumn = true;
        }
        break;
        case kCheck:
            field->setCheck(item->text(column));
            if(!m_bNewTable)
                callRenameColumn = true;
            break;
        case kForeignKey:
            // handled in delegate

            if(!m_bNewTable)
                callRenameColumn = true;
            break;
        }

        if(callRenameColumn)
        {
            if(!pdb.renameColumn(curTable, m_table, oldFieldName, field))
                QMessageBox::warning(this, qApp->applicationName(), tr("Modifying this column failed. Error returned from database:\n%1").arg(pdb.lastError()));
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
        } while(m_table.findField(field_name) != -1);
        tbitem->setText(kName, field_name);
    }

    QComboBox* typeBox = new QComboBox(ui->treeWidget);
    typeBox->setProperty("column", tbitem->text(kName));
    typeBox->setEditable(true);
    typeBox->addItems(sqlb::Field::Datatypes);

    int defaultFieldTypeIndex = Settings::getValue("db", "defaultfieldtype").toInt();

    if (defaultFieldTypeIndex < sqlb::Field::Datatypes.count())
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
    sqlb::FieldPtr f(new sqlb::Field(
                      tbitem->text(kName),
                      typeBox->currentText()
                      ));
    m_table.addField(f);

    // Actually add the new column to the table if we're editing an existing table
    if(!m_bNewTable)
        pdb.addColumn(curTable, f);

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

        // Ask user whether he really wants to delete that column
        QString msg = tr("Are you sure you want to delete the field '%1'?\nAll data currently stored in this field will be lost.").arg(ui->treeWidget->currentItem()->text(0));
        if(QMessageBox::warning(this, QApplication::applicationName(), msg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            if(!pdb.renameColumn(curTable, m_table, ui->treeWidget->currentItem()->text(0), sqlb::FieldPtr()))
            {
                QMessageBox::warning(0, QApplication::applicationName(), pdb.lastError());
            } else {
                //relayout
                m_table = *(pdb.getObjectByName(curTable).dynamicCast<sqlb::Table>());
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
        sqlb::FieldVector fields = m_table.fields();
        std::swap(fields[newRow], fields[currentRow]);
        m_table.setFields(fields);
    } else {
        // Editing an old one

        // Move the actual column
        if(!pdb.renameColumn(
                    curTable,
                    m_table,
                    ui->treeWidget->currentItem()->text(0),
                    m_table.fields().at(ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem())),
                    (down ? 1 : -1)
                ))
        {
            QMessageBox::warning(0, QApplication::applicationName(), pdb.lastError());
        } else {
            // Reload table SQL
            m_table = *(pdb.getObjectByName(curTable).dynamicCast<sqlb::Table>());
            populateFields();

            // Select old item at new position
            ui->treeWidget->setCurrentIndex(ui->treeWidget->indexAt(QPoint(1, 1)).sibling(newRow, 0));
        }
    }

    // Update the SQL preview
    updateSqlText();
}

void EditTableDialog::setWithoutRowid(bool without_rowid)
{
    if(without_rowid)
    {
        // Before setting the without rowid flag, first perform a check to see if the table meets all the required criteria for without rowid tables
        int pk = m_table.findPk();
        if(pk == -1 || m_table.fields().at(pk)->autoIncrement())
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

        // If it does, override the the rowid column name of the table object with the name of the primary key.
        m_table.setRowidColumn(m_table.fields().at(pk)->name());
    } else {
        // If the without rowid flag is unset no further checks are required. Just set the rowid column name back to "_rowid_"
        m_table.setRowidColumn("_rowid_");
    }

    // Update the SQL preview
    updateSqlText();

    // Update table if we're editing an existing table
    if(!m_bNewTable)
    {
        if(!pdb.renameColumn(curTable, m_table, QString(), sqlb::FieldPtr(), 0))
        {
            QMessageBox::warning(this, QApplication::applicationName(),
                                 tr("Setting the rowid column for the table failed. Error message:\n%1").arg(pdb.lastError()));
        }
    }
}

void EditTableDialog::setTemporary(bool is_temp)
{
    // Set the temporary flag in the table definition
    m_table.setTemporary(is_temp);

    // Update the SQL preview
    updateSqlText();

    // Update table if we're editing an existing table
    if(!m_bNewTable)
    {
        if(!pdb.renameColumn(curTable, m_table, QString(), sqlb::FieldPtr(), 0))
        {
            QMessageBox::warning(this, QApplication::applicationName(),
                                 tr("Setting the temporary flag for the table failed. Error message:\n%1").arg(pdb.lastError()));
        }
    }
}
