#include "EditTableDialog.h"
#include "ui_EditTableDialog.h"
#include "SQLiteSyntaxHighlighter.h"
#include <QMessageBox>
#include <QPushButton>
#include <QComboBox>
#include <QDateTime>

#include "sqlitetablemodel.h"
#include "sqlitedb.h"

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
        QString sTablesql = pdb->getTableSQL(curTable);
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
    ui->editTableName->setText(normTableName);
    bool valid = true;
    if(normTableName.isEmpty() || normTableName.contains(" "))
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
        for(index=0;index<m_table.fields().size();index++)
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
    connect(typeBox, SIGNAL(activated(int)), this, SLOT(updateTypes()));
    typeBox->setEditable(false);
    typeBox->addItems(sqlb::Field::Datatypes);
    ui->treeWidget->setItemWidget(tbitem, kType, typeBox);

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
            if(!pdb->dropColumn(curTable, ui->treeWidget->currentItem()->text(0)))
            {
                QMessageBox::warning(0, QApplication::applicationName(), pdb->lastErrorMessage);
            } else {
                //relayout
                QString sTablesql = pdb->getTableSQL(curTable);
                m_table = sqlb::Table::parseSQL(sTablesql);
                populateFields();
            }
        }
    }

    checkInput();
}

void EditTableDialog::fieldSelectionChanged()
{
    ui->removeFieldButton->setEnabled(ui->treeWidget->selectionModel()->hasSelection());
}
