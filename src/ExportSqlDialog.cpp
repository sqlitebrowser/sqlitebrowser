#include "ExportSqlDialog.h"
#include "ui_ExportSqlDialog.h"
#include "sqlitedb.h"
#include "FileDialog.h"

#include <QFile>
#include <QMessageBox>
#include <QSettings>

static QString sSettingsGroup("exportsql");
static QString sSettingsInsertColNames("insertcolnames");
static QString sSettingsInsertMultiple("insertmultiple");
static QString sSettingsOldSchema("oldschema");

enum WhatComboEntries
{
    ExportEverything,
    ExportSchemaOnly,
    ExportDataOnly,
};

ExportSqlDialog::ExportSqlDialog(DBBrowserDB* db, QWidget* parent, const QString& selection)
    : QDialog(parent),
      ui(new Ui::ExportSqlDialog),
      pdb(db)
{
    // Create UI
    ui->setupUi(this);

    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
    settings.beginGroup(sSettingsGroup);
    ui->checkColNames->setChecked(settings.value(sSettingsInsertColNames, false).toBool());
    ui->checkMultiple->setChecked(settings.value(sSettingsInsertMultiple, false).toBool());
    ui->comboOldSchema->setCurrentIndex(settings.value(sSettingsOldSchema, 0).toInt());

    // Get list of tables to export
    objectMap objects = pdb->getBrowsableObjects();
    for(auto it=objects.constBegin();it!=objects.constEnd();++it) {
        ui->listTables->addItem(new QListWidgetItem(QIcon(QString(":icons/%1").arg((*it)->type())), (*it)->name()));
    }

    // Sort list of tables and select the table specified in the
    // selection parameter or all tables if table not specified
    ui->listTables->model()->sort(0);
    if(selection.isEmpty())
    {
        for (int i = 0; i < ui->listTables->count(); ++i)
            ui->listTables->item(i)->setSelected(true);
    }
    else
    {
        QList<QListWidgetItem*> items = ui->listTables->findItems(selection, Qt::MatchExactly);
        ui->listTables->setCurrentItem(items.at(0));
    }
    ui->listTables->setFocus();
}

ExportSqlDialog::~ExportSqlDialog()
{
    delete ui;
}

void ExportSqlDialog::doSelectAll()
{
	for (int i = 0; i < ui->listTables->count(); ++i)
		ui->listTables->item(i)->setSelected(true);
}

void ExportSqlDialog::doDeselectAll()
{
	for (int i = 0; i < ui->listTables->count(); ++i)
		ui->listTables->item(i)->setSelected(false);
}

void ExportSqlDialog::accept()
{
    QList<QListWidgetItem*> selectedItems = ui->listTables->selectedItems();
    if(selectedItems.isEmpty())
    {
        QMessageBox::warning(this, QApplication::applicationName(),
                             tr("Please select at least 1 table."));
        return;
    }

    // Try to find a default file name
    QString defaultFileName;
    if(selectedItems.count() == 1)  // One table -> Suggest table name
        defaultFileName = selectedItems.at(0)->text() + ".sql";
    else if(selectedItems.count() == ui->listTables->count())   // All tables -> Suggest database name
        defaultFileName = pdb->currentFile() + ".sql";;

    QString fileName = FileDialog::getSaveFileName(
                this,
                tr("Choose a filename to export"),
                tr("Text files(*.sql *.txt)"),
                defaultFileName);
    if(fileName.isEmpty())
        return;

    // save settings
    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
    settings.beginGroup(sSettingsGroup);
    settings.setValue(sSettingsInsertColNames, ui->checkColNames->isChecked());
    settings.setValue(sSettingsInsertMultiple, ui->checkMultiple->isChecked());
    settings.setValue(sSettingsOldSchema, ui->comboOldSchema->currentIndex());
    settings.endGroup();

    QStringList tables;
    foreach (const QListWidgetItem * item, ui->listTables->selectedItems())
        tables.push_back(item->text());

    // Check what to export. The indices here depend on the order of the items in the combobox in the ui file
    bool exportSchema = ui->comboWhat->currentIndex() == ExportEverything || ui->comboWhat->currentIndex() == ExportSchemaOnly;
    bool exportData = ui->comboWhat->currentIndex() == ExportEverything || ui->comboWhat->currentIndex() == ExportDataOnly;
    bool keepSchema = ui->comboOldSchema->currentIndex() == 0;

    // Perform actual export
    bool dumpOk = pdb->dump(fileName,
                            tables,
                            ui->checkColNames->isChecked(),
                            ui->checkMultiple->isChecked(),
                            exportSchema,
                            exportData,
                            keepSchema);
    if (dumpOk)
        QMessageBox::information(this, QApplication::applicationName(), tr("Export completed."));
    else
        QMessageBox::warning(this, QApplication::applicationName(), tr("Export cancelled or failed."));

    QDialog::accept();
}

void ExportSqlDialog::whatChanged(int index)
{
    // Only show the combobox for deciding what to do with the old schema when importing into an existing database when we're
    // actually exporting the schema here
    if(index != ExportDataOnly)
        ui->comboOldSchema->setVisible(true);
    else
        ui->comboOldSchema->setVisible(false);
}
