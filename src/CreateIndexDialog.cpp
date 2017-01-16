#include "CreateIndexDialog.h"
#include "ui_CreateIndexDialog.h"
#include "sqlitedb.h"

#include <QMessageBox>
#include <QPushButton>

CreateIndexDialog::CreateIndexDialog(DBBrowserDB& db, QWidget* parent)
    : QDialog(parent),
      pdb(db),
      ui(new Ui::CreateIndexDialog),
      index(sqlb::Index(QString("")))
{
    // Create UI
    ui->setupUi(this);

    // Get list of tables, sort it alphabetically and fill the combobox
    QMultiMap<QString, DBBrowserObject> dbobjs;
    QList<DBBrowserObject> tables = pdb.objMap.values("table");
    for(auto it=tables.constBegin();it!=tables.constEnd();++it)
        dbobjs.insert((*it).getname(), (*it));
    for(auto it=dbobjs.constBegin();it!=dbobjs.constEnd();++it)
        ui->comboTableName->addItem(QIcon(QString(":icons/table")), (*it).getname());

    QHeaderView *tableHeaderView = ui->tableIndexColumns->horizontalHeader();
    tableHeaderView->setSectionResizeMode(0, QHeaderView::Stretch);

    updateSqlText();
}

CreateIndexDialog::~CreateIndexDialog()
{
    delete ui;
}

void CreateIndexDialog::tableChanged(const QString& new_table)
{
    // Set the table name and clear all index columns
    index.setTable(new_table);
    index.clearColumns();

    // And fill the table again
    QStringList fields = pdb.getObjectByName(new_table).table.fieldNames();
    ui->tableIndexColumns->setRowCount(fields.size());
    for(int i=0; i < fields.size(); ++i)
    {
        // Put the name of the field in the first column
        QTableWidgetItem* name = new QTableWidgetItem(fields.at(i));
        name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableIndexColumns->setItem(i, 0, name);

        // Put a checkbox to enable usage in the index of this field in the second column
        QTableWidgetItem* enabled = new QTableWidgetItem("");
        enabled->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        enabled->setCheckState(Qt::Unchecked);
        ui->tableIndexColumns->setItem(i, 1, enabled);

        // And put a combobox to select the order in which to index the field in the last column
        QComboBox* order = new QComboBox(this);
        order->addItem("");
        order->addItem("ASC");
        order->addItem("DESC");
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

void CreateIndexDialog::checkInput()
{
    // Check if index name is set
    bool valid = true;
    if(ui->editIndexName->text().isEmpty())
        valid = false;

    // Check if index has any columns
    index.clearColumns();
    for(int i=0; i < ui->tableIndexColumns->rowCount(); ++i)
    {
        if(ui->tableIndexColumns->item(i, 1) && ui->tableIndexColumns->item(i, 1)->data(Qt::CheckStateRole) == Qt::Checked)
        {
            index.addColumn(sqlb::IndexedColumnPtr(new sqlb::IndexedColumn(
                                                       ui->tableIndexColumns->item(i, 0)->text(),
                                                       qobject_cast<QComboBox*>(ui->tableIndexColumns->cellWidget(i, 2))->currentText())));
        }
    }
    if(index.columns().size() == 0)
        valid = false;

    // Only activate OK button if index data is valid
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);

    // Set the index name and the unique flag
    index.setName(ui->editIndexName->text());
    index.setUnique(ui->checkIndexUnique->isChecked());
    updateSqlText();
}

void CreateIndexDialog::accept()
{
    if(pdb.executeSQL(index.sql()))
        QDialog::accept();
    else
        QMessageBox::warning(this, QApplication::applicationName(), tr("Creating the index failed:\n%1").arg(pdb.lastError()));
}

void CreateIndexDialog::updateSqlText()
{
    ui->sqlTextEdit->setText(index.sql());
}
