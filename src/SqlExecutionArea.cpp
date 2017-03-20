#include "SqlExecutionArea.h"
#include "ui_SqlExecutionArea.h"
#include "sqltextedit.h"
#include "sqlitetablemodel.h"
#include "sqlitedb.h"
#include "Settings.h"
#include "ExportDataDialog.h"

#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>

SqlExecutionArea::SqlExecutionArea(DBBrowserDB& _db, QWidget* parent) :
    QWidget(parent),
    db(_db),
    ui(new Ui::SqlExecutionArea)
{
    // Create UI
    ui->setupUi(this);

    // Create model
    model = new SqliteTableModel(db, this, Settings::getValue("db", "prefetchsize").toInt());
    ui->tableResult->setModel(model);

    // Create popup menu for save button
    menuPopupSave = new QMenu(this);
    menuPopupSave->addAction(ui->actionExportCsv);
    menuPopupSave->addAction(ui->actionSaveAsView);
    ui->buttonSave->setMenu(menuPopupSave);

    // Load settings
    reloadSettings();
}

SqlExecutionArea::~SqlExecutionArea()
{
    delete ui;
}

QString SqlExecutionArea::getSql() const
{
    return ui->editEditor->text();
}

QString SqlExecutionArea::getSelectedSql() const
{
    return ui->editEditor->selectedText().trimmed().replace(QChar(0x2029), '\n');
}

void SqlExecutionArea::finishExecution(const QString& result)
{
    ui->editErrors->setPlainText(result);

    // Set column widths according to their contents but make sure they don't exceed a certain size
    ui->tableResult->resizeColumnsToContents();
    for(int i=0;i<model->columnCount();i++)
    {
        if(ui->tableResult->columnWidth(i) > 300)
            ui->tableResult->setColumnWidth(i, 300);
    }
}

SqlTextEdit *SqlExecutionArea::getEditor()
{
    return ui->editEditor;
}

ExtendedTableWidget *SqlExecutionArea::getTableResult()
{
    return ui->tableResult;
}

void SqlExecutionArea::enableSaveButton(bool enable)
{
    ui->buttonSave->setEnabled(enable);
}

void SqlExecutionArea::saveAsCsv()
{
    ExportDataDialog dialog(db, ExportDataDialog::ExportFormatCsv, this, model->query());
    dialog.exec();
}

void SqlExecutionArea::saveAsView()
{
    // Let the user select a name for the new view and make sure it doesn't already exist
    QString name;
    while(true)
    {
        name = QInputDialog::getText(this, qApp->applicationName(), tr("Please specify the view name")).trimmed();
        if(name.isEmpty())
            return;
        if(db.getObjectByName(name) != nullptr)
            QMessageBox::warning(this, qApp->applicationName(), tr("There is already an object with that name. Please choose a different name."));
        else
            break;
    }

    // Create the view
    if(db.executeSQL(QString("CREATE VIEW %1 AS %2;").arg(sqlb::escapeIdentifier(name)).arg(model->query())))
        QMessageBox::information(this, qApp->applicationName(), tr("View successfully created."));
    else
        QMessageBox::warning(this, qApp->applicationName(), tr("Error creating view: %1").arg(db.lastError()));
}

void SqlExecutionArea::reloadSettings()
{
    // Reload editor settings
    ui->editEditor->reloadSettings();

    // Set font
    QFont logfont(Settings::getValue("editor", "font").toString());
    logfont.setStyleHint(QFont::TypeWriter);
    logfont.setPointSize(Settings::getValue("log", "fontsize").toInt());
    ui->editErrors->setFont(logfont);

    // Apply horizontal/vertical tiling option
    if(Settings::getValue("editor", "horizontal_tiling").toBool())
        ui->splitter->setOrientation(Qt::Horizontal);
    else
        ui->splitter->setOrientation(Qt::Vertical);

    // Set prefetch settings
    model->setChunkSize(Settings::getValue("db", "prefetchsize").toInt());
}
