#include "EditTableDialog.h"
#include "ui_EditTableDialog.h"
#include "SQLiteSyntaxHighlighter.h"
#include <QMessageBox>
#include <QPushButton>
#include <QComboBox>

#include "sqlitedb.h"

#include <QDebug>

EditTableDialog::EditTableDialog(DBBrowserDB* db, const QString& tableName, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditTableDialog),
      pdb(db),
      curTable(tableName),
      m_table(tableName)
{
    // Create UI
    ui->setupUi(this);
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(itemChanged(QTreeWidgetItem*,int)));

    m_sqliteSyntaxHighlighter = new SQLiteSyntaxHighlighter(ui->sqlTextEdit->document());

    // Editing an existing table?
    if(curTable != "")
    {
        // Existing table, so load and set the current layout
        QString sTablesql = pdb->getTableSQL(curTable);
        //qDebug() << sTablesql;
        m_table = sqlb::Table::parseSQL(sTablesql);
        populateFields();
    }

    // And create a savepoint
    pdb->executeSQL(QString("SAVEPOINT edittable_%1_save;").arg(curTable), false);

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
        QObject::connect(typeBox, SIGNAL(activated(int)), this, SLOT(updateTypes()));
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
        ui->treeWidget->setItemWidget(tbitem, kType, typeBox);

        tbitem->setCheckState(kNotNull, f->notnull() ? Qt::Checked : Qt::Unchecked);
        tbitem->setCheckState(kPrimaryKey, m_table.primarykey().contains(f) ? Qt::Checked : Qt::Unchecked);
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
    if(curTable == "")
    {
        // Creation of new table
        // we commit immediatly so no need to setdirty
        if(!pdb->executeSQL(m_table.sql(), false))
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
    }

    // Release the savepoint
    pdb->executeSQL(QString("RELEASE SAVEPOINT edittable_%1_save;").arg(curTable), false);

    QDialog::accept();
}

void EditTableDialog::reject()
{    
    // Then rollback to our savepoint
    pdb->executeSQL(QString("ROLLBACK TO SAVEPOINT edittable_%1_save;").arg(curTable), false);

    QDialog::reject();
}

void EditTableDialog::updateSqlText()
{
    ui->sqlTextEdit->clear();
    ui->sqlTextEdit->insertPlainText(m_table.sql());
}

void EditTableDialog::updateTableObject()
{
    sqlb::FieldVector fields;
    sqlb::FieldVector pk;
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
        QComboBox* typeBox = qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(item, kType));
        QString sType = "INTEGER";
        if(typeBox)
            sType = typeBox->currentText();
        sqlb::FieldPtr f(new sqlb::Field(
                          item->text(kName),
                          sType,
                          item->checkState(kNotNull) == Qt::Checked
                          ));
        f->setAutoIncrement(item->checkState(kAutoIncrement) == Qt::Checked);
        if(item->checkState(kPrimaryKey) == Qt::Checked)
            pk.append(f);
        f->setDefaultValue(item->text(kDefault));
        f->setCheck(item->text(kCheck));
        fields.append(f);
    }

    m_table.setFields(fields);
    m_table.setPrimaryKey(pk);
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
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
        QComboBox* typeBox = qobject_cast<QComboBox*>(ui->treeWidget->itemWidget(item, kType));
        QString sType = "INTEGER";
        if(typeBox)
            sType = typeBox->currentText();
        m_table.fields().at(i)->setType(sType);
    }
    checkInput();
}

void EditTableDialog::itemChanged(QTreeWidgetItem *item, int column)
{
    int index = ui->treeWidget->indexOfTopLevelItem(item);
    if(index < m_table.fields().count())
    {
        sqlb::FieldPtr field = m_table.fields().at(index);
        switch(column)
        {
        case kName: field->setName(item->text(column)); break;
        case kType:
        {
            // we don't know which combobox got the update
            // so we have to update all at once
            // see updateTypes() SLOT
        }
        break;
        case kPrimaryKey:
        {
            sqlb::FieldVector pks = m_table.primarykey();
            if(item->checkState(column) == Qt::Checked)
                pks.append(field);
            else
            {
                int index = pks.indexOf(field);
                if(index != -1)
                pks.remove(index);
            }
            m_table.setPrimaryKey(pks);
        }
        break;
        case kNotNull:
        {
            field->setNotNull(item->checkState(column) == Qt::Checked);
        }
        break;
        case kAutoIncrement:
        {
            field->setAutoIncrement(item->checkState(column) == Qt::Checked);
        }
        break;
        case kDefault: field->setDefaultValue(item->text(column)); break;
        case kCheck: field->setCheck(item->text(column)); break;
        }
    }

    checkInput();
}

void EditTableDialog::addField()
{
    QTreeWidgetItem *tbitem = new QTreeWidgetItem(ui->treeWidget);
    tbitem->setFlags(tbitem->flags() | Qt::ItemIsEditable);
    tbitem->setText(kName, "Field" + QString::number(ui->treeWidget->topLevelItemCount()));
    QComboBox* typeBox = new QComboBox(ui->treeWidget);
    QObject::connect(typeBox, SIGNAL(activated(int)), this, SLOT(updateTypes()));
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

    checkInput();
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
