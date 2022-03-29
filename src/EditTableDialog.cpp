#include "EditTableDialog.h"
#include "Settings.h"
#include "ForeignKeyEditorDelegate.h"
#include "ui_EditTableDialog.h"
#include "sqlitetablemodel.h"
#include "sqlitedb.h"
#include "SelectItemsPopup.h"

#include <QComboBox>
#include <QDateTime>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>

#include <algorithm>

Q_DECLARE_METATYPE(sqlb::ConstraintPtr)
Q_DECLARE_METATYPE(sqlb::StringVector)
Q_DECLARE_METATYPE(sqlb::IndexedColumnVector)

// Styled Item Delegate for non-editable columns
class NoEditDelegate: public QStyledItemDelegate {
public:
    explicit NoEditDelegate(QObject* parent=nullptr): QStyledItemDelegate(parent) {}
    QWidget* createEditor(QWidget* /* parent */, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const override {
        return nullptr;
    }
};

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
    connect(ui->treeWidget, &QTreeWidget::itemChanged, this, &EditTableDialog::fieldItemChanged);
    connect(ui->tableConstraints, &QTableWidget::itemChanged, this, &EditTableDialog::constraintItemChanged);

    // Set item delegate for foreign key column
    m_fkEditorDelegate = new ForeignKeyEditorDelegate(db, m_table, this);
    ui->treeWidget->setItemDelegateForColumn(kForeignKey, m_fkEditorDelegate);
    // Disallow edition of checkable columns or columns with combo-boxes
    ui->treeWidget->setItemDelegateForColumn(kType, new NoEditDelegate(this));
    ui->treeWidget->setItemDelegateForColumn(kNotNull, new NoEditDelegate(this));
    ui->treeWidget->setItemDelegateForColumn(kPrimaryKey, new NoEditDelegate(this));
    ui->treeWidget->setItemDelegateForColumn(kAutoIncrement, new NoEditDelegate(this));
    ui->treeWidget->setItemDelegateForColumn(kUnique, new NoEditDelegate(this));
    ui->treeWidget->setItemDelegateForColumn(kCollation, new NoEditDelegate(this));

    // Set up popup menu for adding constraints
    QMenu* constraint_menu = new QMenu(this);
    constraint_menu->addAction(ui->actionAddPrimaryKey);
    constraint_menu->addAction(ui->actionAddForeignKey);
    constraint_menu->addAction(ui->actionAddUniqueConstraint);
    constraint_menu->addAction(ui->actionAddCheckConstraint);
    connect(ui->actionAddPrimaryKey, &QAction::triggered, this, [this]() { addConstraint(TableConstraintType::PrimaryKey); });
    connect(ui->actionAddForeignKey, &QAction::triggered, this, [this]() { addConstraint(TableConstraintType::ForeignKey); });
    connect(ui->actionAddUniqueConstraint, &QAction::triggered, this, [this]() { addConstraint(TableConstraintType::Unique); });
    connect(ui->actionAddCheckConstraint, &QAction::triggered, this, [this]() { addConstraint(TableConstraintType::Check); });
    ui->buttonAddConstraint->setMenu(constraint_menu);

    // Get list of all collations
    db.executeSQL("PRAGMA collation_list;", false, true, [this](int column_count, std::vector<QByteArray> columns, std::vector<QByteArray>) -> bool {
        if(column_count >= 2)
            m_collationList.push_back(columns.at(1));
        return false;
    });
    if(!m_collationList.contains(""))
        m_collationList.push_back("");
    m_collationList.sort();

    // Editing an existing table?
    if(m_bNewTable == false)
    {
        // Existing table, so load and set the current layout
        m_table = *pdb.getTableByName(curTable);
        ui->labelEditWarning->setVisible(!m_table.fullyParsed());

        // Initialise the list of tracked columns for table layout changes
        for(const auto& field : m_table.fields)
            trackColumns[QString::fromStdString(field.name())] = QString::fromStdString(field.name());

        // Set without rowid checkbox and schema dropdown. No need to trigger any events here as we're only loading a table exactly as it is stored by SQLite, so no need
        // for error checking etc.
        ui->checkWithoutRowid->blockSignals(true);
        ui->checkWithoutRowid->setChecked(m_table.withoutRowidTable());
        ui->checkWithoutRowid->blockSignals(false);
        ui->checkStrict->blockSignals(true);
        ui->checkStrict->setChecked(m_table.isStrict());
        ui->checkStrict->blockSignals(false);
        ui->comboSchema->blockSignals(true);
        for(const auto& n : pdb.schemata)                       // Load list of database schemata
            ui->comboSchema->addItem(QString::fromStdString(n.first));
        ui->comboSchema->setCurrentText(QString::fromStdString(curTable.schema()));
        ui->comboSchema->blockSignals(false);

        if(m_table.primaryKey())
        {
            ui->checkWithoutRowid->blockSignals(true);
            ui->comboOnConflict->setCurrentText(QString::fromStdString(m_table.primaryKey()->conflictAction()).toUpper());
            ui->checkWithoutRowid->blockSignals(false);
        }

        populateFields();
        populateConstraints();
    } else {
        for(const auto& n : pdb.schemata)                       // Load list of database schemata
            ui->comboSchema->addItem(QString::fromStdString(n.first));
        ui->comboSchema->setCurrentText("main");                // Always create tables in the main schema by default
        ui->labelEditWarning->setVisible(false);
    }

    // Enable/disable remove constraint button depending on whether a constraint is selected
    connect(ui->tableConstraints, &QTableWidget::itemSelectionChanged, this, [this]() {
        bool hasSelection = ui->tableConstraints->selectionModel()->hasSelection();
        ui->buttonRemoveConstraint->setEnabled(hasSelection);
    });

    // And create a savepoint
    pdb.setSavepoint(m_sRestorePointName);

    // Update UI
    ui->editTableName->setText(QString::fromStdString(curTable.name()));
    updateColumnWidth();

    // Allow editing of constraint columns by double clicking the columns column of the constraints table
    connect(ui->tableConstraints, &QTableWidget::itemDoubleClicked, this, [this](QTableWidgetItem* item) {
        // Check whether the double clicked item is in the columns column
        if(item->column() == kConstraintColumns)
        {
            sqlb::StringVector columns = ui->tableConstraints->item(item->row(), kConstraintColumns)->data(Qt::UserRole).value<sqlb::StringVector>();
            if(columns.empty())
            {
                // When we've got no columns, try the other type of columns vector
                sqlb::IndexedColumnVector indexed_columns = ui->tableConstraints->item(item->row(), kConstraintColumns)->data(Qt::UserRole).value<sqlb::IndexedColumnVector>();
                std::transform(indexed_columns.begin(), indexed_columns.end(), std::back_inserter(columns), [](const auto& e) { return e.name(); });
            }
            sqlb::ConstraintPtr constraint = ui->tableConstraints->item(item->row(), kConstraintType)->data(Qt::UserRole).value<sqlb::ConstraintPtr>();

            // Do not allow editing the columns list of a CHECK constraint because CHECK constraints are independent of column lists
            if(constraint->type() == sqlb::Constraint::CheckConstraintType)
                return;

            // Show the select items popup dialog
            SelectItemsPopup* dialog = new SelectItemsPopup(m_table.fieldNames(), columns, this);
            QRect item_rect = ui->tableConstraints->visualItemRect(item);
            dialog->move(ui->tableConstraints->mapToGlobal(QPoint(ui->tableConstraints->x() + item_rect.x(),
                                                                  ui->tableConstraints->y() + item_rect.y() + item_rect.height() / 2)));
            dialog->show();

            // When clicking the Apply button in the popup dialog, save the new columns list
            connect(dialog, &SelectItemsPopup::accepted, this, [this, dialog, constraint, columns]() {
                // Check if column selection changed at all
                sqlb::StringVector new_columns = dialog->selectedItems();
                if(columns != new_columns)
                {
                    // Remove the constraint with the old columns and add a new one with the new columns
                    m_table.removeConstraint(constraint);
                    if(constraint->type() == sqlb::Constraint::PrimaryKeyConstraintType)
                        m_table.addConstraint(new_columns, std::dynamic_pointer_cast<sqlb::PrimaryKeyConstraint>(constraint));
                    else if(constraint->type() == sqlb::Constraint::UniqueConstraintType)
                            m_table.addConstraint(new_columns, std::dynamic_pointer_cast<sqlb::UniqueConstraint>(constraint));
                    else if(constraint->type() == sqlb::Constraint::ForeignKeyConstraintType)
                        m_table.addConstraint(new_columns, std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(constraint));

                    // Update the UI
                    populateFields();
                    populateConstraints();
                    updateSqlText();
                }
            });
        }
    });

    // (De-)activate fields
    checkInput();

    ui->sqlTextEdit->setReadOnly(true);
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
    ui->treeWidget->setColumnWidth(kType, 150);
    ui->treeWidget->setColumnWidth(kNotNull, 25);
    ui->treeWidget->setColumnWidth(kPrimaryKey, 25);
    ui->treeWidget->setColumnWidth(kAutoIncrement, 25);
    ui->treeWidget->setColumnWidth(kUnique, 25);
    ui->treeWidget->setColumnWidth(kForeignKey, 500);

    ui->tableConstraints->setColumnWidth(kConstraintColumns, 180);
    ui->tableConstraints->setColumnWidth(kConstraintType, 130);
    ui->tableConstraints->setColumnWidth(kConstraintName, 130);
    ui->tableConstraints->setColumnWidth(kConstraintSql, 300);
}

void EditTableDialog::populateFields()
{
    // Disable the itemChanged signal or the table item will be updated while filling the treewidget
    ui->treeWidget->blockSignals(true);

    ui->treeWidget->clear();
    const auto& fields = m_table.fields;
    const auto pk = m_table.primaryKey();
    const auto pkColumns = m_table.primaryKeyColumns();
    for(const sqlb::Field& f : fields)
    {
        QTreeWidgetItem *tbitem = new QTreeWidgetItem(ui->treeWidget);
        tbitem->setFlags(tbitem->flags() | Qt::ItemIsEditable);
        tbitem->setText(kName, QString::fromStdString(f.name()));
        QComboBox* typeBox = new QComboBox(ui->treeWidget);
        typeBox->setProperty("column", QString::fromStdString(f.name()));
        typeBox->setEditable(!m_table.isStrict());  // Strict tables do not allow arbitrary types
        typeBox->addItems(m_table.isStrict() ? DBBrowserDB::DatatypesStrict : DBBrowserDB::Datatypes);
        int index = typeBox->findText(QString::fromStdString(f.type()), Qt::MatchFixedString);
        if(index == -1)
        {
            // non standard named type
            typeBox->addItem(QString::fromStdString(f.type()));
            index = typeBox->count() - 1;
        }
        typeBox->setCurrentIndex(index);
        typeBox->installEventFilter(this);
        connect(typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypeAndCollation()));

        ui->treeWidget->setItemWidget(tbitem, kType, typeBox);

        tbitem->setCheckState(kNotNull, f.notnull() ? Qt::Checked : Qt::Unchecked);
        tbitem->setCheckState(kPrimaryKey, pk && contains(pkColumns, f.name()) ? Qt::Checked : Qt::Unchecked);
        tbitem->setCheckState(kAutoIncrement, pk && pk->autoIncrement() && contains(pkColumns, f.name()) ? Qt::Checked : Qt::Unchecked);
        tbitem->setCheckState(kUnique, f.unique() ? Qt::Checked : Qt::Unchecked);

        // For the default value check if it is surrounded by parentheses and if that's the case
        // add a '=' character before the entire string to match the input format we're expecting
        // from the user when using functions in the default value field.
        if(!f.defaultValue().empty() && f.defaultValue().front() == '(' && f.defaultValue().back() == ')')
            tbitem->setText(kDefault, "=" + QString::fromStdString(f.defaultValue()));
        else
            tbitem->setText(kDefault, QString::fromStdString(f.defaultValue()));

        tbitem->setText(kCheck, QString::fromStdString(f.check()));

        QComboBox* collationBox = new QComboBox(ui->treeWidget);
        collationBox->setProperty("column", QString::fromStdString(f.name()));
        collationBox->addItems(m_collationList);
        index = collationBox->findText(QString::fromStdString(f.collation()), Qt::MatchCaseSensitive);
        if(index == -1)
        {
            // some non-existing collation
            collationBox->addItem(QString::fromStdString(f.collation()));
            index = collationBox->count() - 1;
        }
        collationBox->setCurrentIndex(index);
        collationBox->installEventFilter(this);
        connect(collationBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypeAndCollation()));
        ui->treeWidget->setItemWidget(tbitem, kCollation, collationBox);

        auto fk = m_table.foreignKey({f.name()});
        if(fk)
            tbitem->setText(kForeignKey, QString::fromStdString(fk->toString()));
        ui->treeWidget->addTopLevelItem(tbitem);
    }

    // and reconnect
    ui->treeWidget->blockSignals(false);
}

void EditTableDialog::populateConstraints()
{
    // Disable the itemChanged signal or the table item will be updated while filling the treewidget
    ui->tableConstraints->blockSignals(true);

    const auto& indexConstraints = m_table.indexConstraints();
    const auto& foreignKeys = m_table.foreignKeys();
    const auto& checkConstraints = m_table.checkConstraints();

    ui->tableConstraints->setRowCount(static_cast<int>(indexConstraints.size() + foreignKeys.size() + checkConstraints.size()));

    auto setRow = [this](int row, const auto& columns, const sqlb::ConstraintPtr constraint, const std::string& sql_text) {
        // Columns
        QTableWidgetItem* column = new QTableWidgetItem(QString::fromStdString(sqlb::joinStringVector(columns, ",")));
        column->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        column->setData(Qt::UserRole, QVariant::fromValue(columns));        // Remember columns of constraint object. This is used for modifying it later
        ui->tableConstraints->setItem(row, kConstraintColumns, column);

        // Type
        QComboBox* type = new QComboBox(this);
        type->addItem(tr("Primary Key"), "pk");
        type->addItem(tr("Unique"), "u");
        type->addItem(tr("Foreign Key"), "fk");
        type->addItem(tr("Check"), "c");
        type->setCurrentIndex(constraint->type());
        connect(type, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this, type, columns, constraint](int index) {
            // Handle change of constraint type. Effectively this means removing the old constraint and replacing it by an entirely new one.
            // Only the column list and the name can be migrated to the new constraint.

            // Make sure there is only one primary key at a time
            if(index == 0 && m_table.primaryKey())
            {
                QMessageBox::warning(this, qApp->applicationName(), tr("There can only be one primary key for each table. Please modify the existing primary "
                                                                       "key instead."));

                // Set combo box back to original constraint type
                type->blockSignals(true);
                type->setCurrentIndex(constraint->type());
                type->blockSignals(false);
                return;
            }

            // Remove old constraint
            m_table.removeConstraint(constraint);

            // Add new constraint depending on selected type
            sqlb::ConstraintPtr new_constraint;
            if(type->itemData(index).toString() == "pk")
            {
                auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
                pk->setName(constraint->name());
                m_table.addConstraint(columns, pk);
            } else if(type->itemData(index).toString() == "u") {
                auto u = std::make_shared<sqlb::UniqueConstraint>();
                u->setName(constraint->name());
                m_table.addConstraint(columns, u);
            } else if(type->itemData(index).toString() == "fk") {
                auto fk = std::make_shared<sqlb::ForeignKeyClause>();
                fk->setName(constraint->name());
                m_table.addConstraint(columns, fk);
            } else if(type->itemData(index).toString() == "c") {
                auto c = std::make_shared<sqlb::CheckConstraint>();
                c->setName(constraint->name());
                m_table.addConstraint(c);
            }

            // Update SQL and view
            populateFields();
            populateConstraints();
            updateSqlText();
        });
        QTableWidgetItem* typeColumn = new QTableWidgetItem();
        typeColumn->setData(Qt::UserRole, QVariant::fromValue<sqlb::ConstraintPtr>(constraint));      // Remember address of constraint object. This is used for modifying it later
        ui->tableConstraints->setCellWidget(row, kConstraintType, type);
        ui->tableConstraints->setItem(row, kConstraintType, typeColumn);

        // Name
        QTableWidgetItem* name = new QTableWidgetItem(QString::fromStdString(constraint->name()));
        name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        ui->tableConstraints->setItem(row, kConstraintName, name);

        // SQL
        QTableWidgetItem* sql = new QTableWidgetItem(QString::fromStdString(sql_text));
        sql->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableConstraints->setItem(row, kConstraintSql, sql);
    };

    int row = 0;
    for(const auto& it : indexConstraints)
        setRow(row++, it.first, it.second, it.second->toSql(it.first));
    for(const auto& it : foreignKeys)
        setRow(row++, it.first, it.second, it.second->toSql(it.first));
    for(const auto& it : checkConstraints)
        setRow(row++, sqlb::StringVector{}, it, it->toSql());

    ui->tableConstraints->blockSignals(false);
}

void EditTableDialog::accept()
{
    // Are we editing an already existing table or designing a new one? In the first case there is a table name set,
    // in the latter the current table name is empty
    if(m_bNewTable)
    {
        // Creation of new table
        if(!pdb.executeSQL(m_table.sql(ui->comboSchema->currentText().toStdString())))
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
        if(!pdb.alterTable(curTable, m_table, trackColumns, ui->comboSchema->currentText().toStdString()))
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
    ui->sqlTextEdit->setText(QString::fromStdString(m_table.sql(ui->comboSchema->currentText().toStdString())));
}

void EditTableDialog::checkInput()
{
    std::string normTableName = ui->editTableName->text().toStdString();
    bool valid = true;
    if(normTableName.empty())
        valid = false;
    if(ui->treeWidget->topLevelItemCount() == 0)
        valid = false;
    if (normTableName != m_table.name()) {
        const std::string oldTableName = m_table.name();
        m_table.setName(normTableName);

        // update fk's that refer to table itself recursively
        const auto& fields = m_table.fields;
        for(const sqlb::Field& f : fields) {
            auto fk = m_table.foreignKey({f.name()});
            if(fk && oldTableName == fk->table())
                fk->setTable(normTableName);
        }

        populateFields();
    }

    updateSqlText();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

void EditTableDialog::updateTypeAndCollation(QObject* object)
{
    // Get sender combo box and retrieve field name from it
    QComboBox* combo = qobject_cast<QComboBox*>(object);
    if(!combo)
        return;
    QString column = combo->property("column").toString();

    // Get type *and* collation combo box for this field
    auto item = ui->treeWidget->findItems(column, Qt::MatchExactly, kName);
    if(item.size() != 1)
        return;
    QComboBox* typeBox = qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(item.front(), kType));
    QComboBox* collationBox = qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(item.front(), kCollation));

    // Update table
    if(typeBox && collationBox)
    {
        QString type = typeBox->currentText();
        QString collation = collationBox->currentText();

        for(size_t index=0; index < m_table.fields.size(); ++index)
        {
            if(m_table.fields.at(index).name() == column.toStdString())
            {
                m_table.fields.at(index).setType(type.toStdString());
                m_table.fields.at(index).setCollation(collation.toStdString());
                break;
            }
        }

        checkInput();
    }
}

void EditTableDialog::updateTypeAndCollation()
{
    updateTypeAndCollation(sender());
}

bool EditTableDialog::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::FocusOut)
    {
        updateTypeAndCollation(object);
    }
    return false;
}

void EditTableDialog::fieldItemChanged(QTreeWidgetItem *item, int column)
{
    size_t index = static_cast<size_t>(ui->treeWidget->indexOfTopLevelItem(item));
    if(index < m_table.fields.size())
    {
        sqlb::Field& field = m_table.fields.at(index);
        QString oldFieldName = QString::fromStdString(field.name());

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
            auto foundField = sqlb::findField(m_table, item->text(column).toStdString());
            if(foundField != m_table.fields.end() && foundField-m_table.fields.begin() != static_cast<int>(index))
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
                const auto pk = m_table.primaryKey();
                const auto pkColumns = m_table.primaryKeyColumns();
                for(const auto& it : pdb.schemata[curTable.schema()].tables)
                {
                    const sqlb::TablePtr& fkobj = it.second;

                    auto constraints = fkobj->foreignKeys();
                    for(const auto& fk : constraints)
                    {
                        if(fk.second->table() == m_table.name())
                        {
                            if(contains(fk.second->columns(), field.name()) || (pk && contains(pkColumns, field.name())))
                            {
                                QMessageBox::warning(this, qApp->applicationName(), tr("This column is referenced in a foreign key in table %1 and thus "
                                                                                       "its name cannot be changed.")
                                                     .arg(QString::fromStdString(fkobj->name())));
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

            field.setName(item->text(column).toStdString());
            m_table.renameKeyInAllConstraints(oldFieldName.toStdString(), item->text(column).toStdString());
            qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(item, kType))->setProperty("column", item->text(column));
            qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(item, kCollation))->setProperty("column", item->text(column));

            // Update the field name in the map of old column names to new column names
            if(!m_bNewTable)
            {
                for(const auto& it : trackColumns)
                {
                    if(trackColumns[it.first] == oldFieldName)
                        trackColumns[it.first] = QString::fromStdString(field.name());
                }
            }

            // Update the constraints view
            populateConstraints();
            } break;
        case kType:
        case kCollation:
            // see updateTypeAndCollation() SLOT
            break;
        case kPrimaryKey:
        {
            // Check if there already is a primary key
            auto pk = m_table.primaryKey();
            if(pk)
            {
                // There already is a primary key for this table. So edit that one as there always can only be one primary key anyway.
                if(item->checkState(column) == Qt::Checked)
                    m_table.addKeyToConstraint(pk, field.name());
                else
                    m_table.removeKeyFromConstraint(pk, field.name());
            } else if(item->checkState(column) == Qt::Checked) {
                // There is no primary key in the table yet. This means we need to add a default one.
                m_table.addConstraint({sqlb::IndexedColumn(field.name(), false)}, std::make_shared<sqlb::PrimaryKeyConstraint>());
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

            // Update the constraints view
            populateConstraints();
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
                m.setQuery(QString("SELECT COUNT(%1) FROM %2 WHERE coalesce(NULL,%3) IS NULL;").arg(
                           QString::fromStdString(sqlb::joinStringVector(sqlb::escapeIdentifier(pdb.getTableByName(curTable)->rowidColumns()), ",")),
                           QString::fromStdString(curTable.toString()),
                           QString::fromStdString(sqlb::escapeIdentifier(field.name()))));
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
                    m.setQuery(QString("SELECT COUNT(*) FROM %1 WHERE %2 <> CAST(%3 AS INTEGER);").arg(
                               QString::fromStdString(curTable.toString()),
                               QString::fromStdString(sqlb::escapeIdentifier(field.name())),
                               QString::fromStdString(sqlb::escapeIdentifier(field.name()))));
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
            if(m_table.primaryKey())
                m_table.primaryKey()->setAutoIncrement(ischecked);
        }
        break;
        case kUnique:
        {
            // When editing an existing table and trying to set a column to unique an extra check is needed
            if(!m_bNewTable && item->checkState(column) == Qt::Checked)
            {
                // Because our renameColumn() function fails when setting a column to unique when it already contains the same values
                SqliteTableModel m(pdb, this);
                m.setQuery(QString("SELECT COUNT(%2) FROM %1;").arg(
                               QString::fromStdString(curTable.toString()),
                               QString::fromStdString(sqlb::escapeIdentifier(field.name()))));
                if(!m.completeCache())
                {
                    // If we couldn't load all data because the cancel button was clicked, just unset the checkbox again and stop.
                    item->setCheckState(column, Qt::Unchecked);
                    return;
                }
                int rowcount = m.data(m.index(0, 0)).toInt();
                m.setQuery(QString("SELECT COUNT(DISTINCT %2) FROM %1;").arg(
                               QString::fromStdString(curTable.toString()),
                               QString::fromStdString(sqlb::escapeIdentifier(field.name()))));
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
                    QMessageBox::information(this, qApp->applicationName(), tr("Column '%1' has duplicate data.\n").arg(QString::fromStdString(field.name()))
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
                            new_value = sqlb::escapeString(new_value);
                            item->setText(column, new_value);
                        }
                    }
                }
            }
            field.setDefaultValue(new_value.toStdString());
        }
        break;
        case kCheck:
            field.setCheck(item->text(column).toStdString());
            break;
        case kForeignKey:
            // handled in delegate
            break;
        }
    }

    checkInput();
}

void EditTableDialog::constraintItemChanged(QTableWidgetItem* item)
{
    // Find modified constraint
    sqlb::ConstraintPtr constraint = ui->tableConstraints->item(item->row(), kConstraintType)->data(Qt::UserRole).value<sqlb::ConstraintPtr>();

    // Which column has been modified?
    switch(item->column())
    {
    case kConstraintName:
        constraint->setName(item->text().toStdString());
        break;
    }

    // Update SQL
    populateConstraints();
    checkInput();
}

void EditTableDialog::addField()
{
    QTreeWidgetItem *tbitem = new QTreeWidgetItem(ui->treeWidget);
    tbitem->setFlags(tbitem->flags() | Qt::ItemIsEditable);

    // Find an unused name for the field by starting with 'Fieldx' where x is the number of fields + 1.
    // If this name happens to exist already, increase x by one until we find an unused name.
    {
        int field_number = ui->treeWidget->topLevelItemCount();
        std::string field_name;
        do
        {
            field_name = "Field" + std::to_string(field_number);
            field_number++;
        } while(sqlb::findField(m_table, field_name) != m_table.fields.end());
        tbitem->setText(kName, QString::fromStdString(field_name));
    }

    QComboBox* typeBox = new QComboBox(ui->treeWidget);
    typeBox->setProperty("column", tbitem->text(kName));
    typeBox->setEditable(!m_table.isStrict());  // Strict tables do not allow arbitrary types
    typeBox->addItems(m_table.isStrict() ? DBBrowserDB::DatatypesStrict : DBBrowserDB::Datatypes);

    int defaultFieldTypeIndex = Settings::getValue("db", "defaultfieldtype").toInt();

    if (defaultFieldTypeIndex < DBBrowserDB::Datatypes.count())
    {
        typeBox->setCurrentIndex(defaultFieldTypeIndex);
    }

    ui->treeWidget->setItemWidget(tbitem, kType, typeBox);
    typeBox->installEventFilter(this);
    connect(typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypeAndCollation()));

    QComboBox* collationBox = new QComboBox(ui->treeWidget);
    collationBox->setProperty("column", tbitem->text(kName));
    collationBox->addItems(m_collationList);
    collationBox->setCurrentIndex(collationBox->findText(""));
    ui->treeWidget->setItemWidget(tbitem, kCollation, collationBox);
    collationBox->installEventFilter(this);
    connect(collationBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypeAndCollation()));

    tbitem->setCheckState(kNotNull, Qt::Unchecked);
    tbitem->setCheckState(kPrimaryKey, Qt::Unchecked);
    tbitem->setCheckState(kAutoIncrement, Qt::Unchecked);
    tbitem->setCheckState(kUnique, Qt::Unchecked);

    ui->treeWidget->addTopLevelItem(tbitem);
    ui->treeWidget->scrollToBottom();
    ui->treeWidget->editItem(tbitem, 0);

    // add field to table object
    m_table.fields.emplace_back(tbitem->text(kName).toStdString(), typeBox->currentText().toStdString());

    // Add the new column to the list of tracked columns to indicate it has been added
    if(!m_bNewTable)
        trackColumns.insert({QString(), tbitem->text(kName)});

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
        for(const auto& it : trackColumns)
        {
            if(trackColumns[it.first] == name)
                trackColumns[it.first] = QString();
        }
    }

    // Just delete that item. At this point there is no DB table to edit or data to be lost anyway
    m_table.fields.erase(m_table.fields.begin() + ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem()));
    m_table.removeKeyFromAllConstraints(ui->treeWidget->currentItem()->text(kName).toStdString());
    delete ui->treeWidget->currentItem();

    // Update the constraints view
    populateConstraints();

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
        ui->buttonMoveTop->setEnabled(ui->buttonMoveUp->isEnabled());
        ui->buttonMoveDown->setEnabled(ui->treeWidget->selectionModel()->currentIndex().row() != ui->treeWidget->topLevelItemCount() - 1);
        ui->buttonMoveBottom->setEnabled(ui->buttonMoveDown->isEnabled());
    }
}

void EditTableDialog::moveUp()
{
    moveCurrentField(MoveUp);
}

void EditTableDialog::moveDown()
{
    moveCurrentField(MoveDown);
}

void EditTableDialog::moveTop()
{
    moveCurrentField(MoveTop);
}

void EditTableDialog::moveBottom()
{
    moveCurrentField(MoveBottom);
}

void EditTableDialog::moveCurrentField(MoveFieldDirection dir)
{
    int currentRow = ui->treeWidget->currentIndex().row();
    int currentCol = ui->treeWidget->currentIndex().column();
    int newRow;
    if(dir == MoveUp)
        newRow = currentRow - 1;
    else if(dir == MoveDown)
        newRow = currentRow + 1;
    else if(dir == MoveTop)
        newRow = 0;
    else if(dir == MoveBottom)
        newRow = ui->treeWidget->topLevelItemCount() - 1;
    else
        return;

    // Save the comboboxes first by making copies
    QComboBox* newCombo[2];
    for(int c=0;c<2;c++)
    {
        int column = (c == 0 ? kType : kCollation);

        QComboBox* oldCombo = qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(ui->treeWidget->topLevelItem(currentRow), column));
        newCombo[c] = new QComboBox(ui->treeWidget);
        newCombo[c]->setProperty("column", oldCombo->property("column"));
        newCombo[c]->installEventFilter(this);
        connect(newCombo[c], SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypeAndCollation()));
        newCombo[c]->setEditable(oldCombo->isEditable());
        for(int i=0; i < oldCombo->count(); ++i)
            newCombo[c]->addItem(oldCombo->itemText(i));
        newCombo[c]->setCurrentIndex(oldCombo->currentIndex());
    }

    // Now, just remove the item and insert it at it's new position, then restore the combobox
    QTreeWidgetItem* item = ui->treeWidget->takeTopLevelItem(currentRow);
    ui->treeWidget->insertTopLevelItem(newRow, item);
    ui->treeWidget->setItemWidget(item, kType, newCombo[0]);
    ui->treeWidget->setItemWidget(item, kCollation, newCombo[1]);

    // Select the old item at its new position
    ui->treeWidget->setCurrentIndex(ui->treeWidget->currentIndex().sibling(newRow, currentCol));

    // Finally update the table SQL
    sqlb::Field temp = m_table.fields[static_cast<size_t>(currentRow)];
    m_table.fields.erase(m_table.fields.begin() + currentRow);
    m_table.fields.insert(m_table.fields.begin() + newRow, temp);

    // Update the SQL preview
    updateSqlText();
}

void EditTableDialog::setWithoutRowid(bool without_rowid)
{
    if(without_rowid)
    {
        // Before setting the without rowid flag, first perform a check to see if the table meets all the required criteria for without rowid tables
        const auto pk = m_table.primaryKey();
        if(!pk || pk->autoIncrement())
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

        // If it does, set the without rowid flag of the table
        m_table.setWithoutRowidTable(true);
    } else {
        // If the without rowid flag is unset no further checks are required. Just unset the without rowid flag
        m_table.setWithoutRowidTable(false);
    }

    // Update the SQL preview
    updateSqlText();
}

void EditTableDialog::setStrict(bool strict)
{
    // Set the strict option
    m_table.setStrict(strict);

    // Replace list of possible data types in each type combo box
    for(int i=0;i<ui->treeWidget->topLevelItemCount();i++)
    {
        QComboBox* w = qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(ui->treeWidget->topLevelItem(i), kType));
        QString value = w->currentText();
        w->blockSignals(true);
        w->clear();
        w->addItems(strict ? DBBrowserDB::DatatypesStrict : DBBrowserDB::Datatypes);
        w->setEditable(!strict);    // Strict tables do not allow arbitrary types
        int pos = w->findText(value, Qt::MatchFixedString);
        w->blockSignals(false);

        if(pos >= 0)
        {
            w->setCurrentIndex(pos);
        } else {
            // When the old value cannot be found we default to ANY for strict tables. For ordinary tables we just add the former value.
            if(strict)
                w->setCurrentText("ANY");
            else
                w->setCurrentText(value);
        }
    }

    // Update the SQL preview
    updateSqlText();
}

void EditTableDialog::changeSchema(const QString& /*schema*/)
{
    // Update the SQL preview
    updateSqlText();
}

void EditTableDialog::setOnConflict(const QString& on_conflict)
{
    if(m_table.primaryKey())
    {
        m_table.primaryKey()->setConflictAction(on_conflict.toStdString());
    } else {
        QMessageBox::information(this, QApplication::applicationName(),
                                 tr("Please add a field which meets the following criteria before setting the on conflict action:\n"
                                    " - Primary key flag set"));

        ui->comboOnConflict->blockSignals(true);
        ui->comboOnConflict->setCurrentText(QString());
        ui->comboOnConflict->blockSignals(false);
        return;
    }

    // Update the SQL preview
    updateSqlText();
}

void EditTableDialog::removeConstraint()
{
    // Is there any item selected to delete?
    if(!ui->tableConstraints->currentItem())
        return;

    // Find constraint to delete
    int row = ui->tableConstraints->currentRow();
    sqlb::ConstraintPtr constraint = ui->tableConstraints->item(row, kConstraintType)->data(Qt::UserRole).value<sqlb::ConstraintPtr>();

    // Remove the constraint. If there is more than one constraint with this combination of columns and constraint type, only delete the first one.
    m_table.removeConstraint(constraint);
    ui->tableConstraints->removeRow(ui->tableConstraints->currentRow());

    // Update SQL and view
    updateSqlText();
    populateFields();
}

void EditTableDialog::addConstraint(TableConstraintType type)
{
    // There can only be one primary key
    if(type == TableConstraintType::PrimaryKey)
    {
        if(m_table.primaryKey())
        {
            QMessageBox::information(this, qApp->applicationName(), tr("There can only be one primary key for each table. Please modify the existing primary "
                                                                       "key instead."));
            return;
        }

        // Create new constraint
        m_table.addConstraint(sqlb::IndexedColumnVector{}, std::make_shared<sqlb::PrimaryKeyConstraint>());
    } else if(type == TableConstraintType::Unique)
        m_table.addConstraint(sqlb::IndexedColumnVector{}, std::make_shared<sqlb::UniqueConstraint>());
    else if(type == TableConstraintType::ForeignKey)
        m_table.addConstraint(sqlb::StringVector{}, std::make_shared<sqlb::ForeignKeyClause>());
    else if(type == TableConstraintType::Check)
        m_table.addConstraint(std::make_shared<sqlb::CheckConstraint>());

    // Update SQL and view
    populateFields();
    populateConstraints();
    updateSqlText();
}
