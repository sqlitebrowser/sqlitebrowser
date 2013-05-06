#include "SqlExecutionArea.h"
#include "ui_SqlExecutionArea.h"
#include "sqltextedit.h"
#include "ExtendedTableWidget.h"
#include "SQLiteSyntaxHighlighter.h"
#include "sqlitetablemodel.h"
#include "sqlitedb.h"
#include "PreferencesDialog.h"
#include "ExportCsvDialog.h"
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>

SqlExecutionArea::SqlExecutionArea(QWidget* parent, DBBrowserDB* _db) :
    QWidget(parent),
    db(_db),
    ui(new Ui::SqlExecutionArea)
{
    // Create UI
    ui->setupUi(this);

    // Create syntax highlighter
    highlighter = new SQLiteSyntaxHighlighter(ui->editEditor->document());

    // Create model
    model = new SqliteTableModel(this, db, PreferencesDialog::getSettingsValue("db", "prefetchsize").toInt());
    ui->tableResult->setModel(model);

    // Create popup menu for save button
    menuPopupSave = new QMenu(this);
    menuPopupSave->addAction(ui->actionExportCsv);
    menuPopupSave->addAction(ui->actionSaveAsView);
    ui->buttonSave->setMenu(menuPopupSave);
}

SqlExecutionArea::~SqlExecutionArea()
{
    delete ui;
}

void SqlExecutionArea::setTableNames(const QStringList& tables)
{
    highlighter->setTableNames(tables);
}

QString SqlExecutionArea::getSql() const
{
    return ui->editEditor->toPlainText().trimmed();
}

QString SqlExecutionArea::getSelectedSql() const
{
    return ui->editEditor->textCursor().selectedText().trimmed();
}

void SqlExecutionArea::finishExecution(const QString& result)
{
    ui->labelErrors->setText(result);
    ui->tableResult->resizeColumnsToContents();
}

SqlTextEdit* SqlExecutionArea::getEditor()
{
    return ui->editEditor;
}

void SqlExecutionArea::enableSaveButton(bool enable)
{
    ui->buttonSave->setEnabled(enable);
}

void SqlExecutionArea::saveAsCsv()
{
    ExportCsvDialog dialog(db, this, model->query());
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
            break;
        if(!db->getObjectByName(name).getname().isEmpty())
            QMessageBox::warning(this, qApp->applicationName(), tr("There is already an object with that name. Please choose a different name."));
        else
            break;
    }
    if(name.isEmpty())
        return;

    // Create the view
    if(db->executeSQL(QString("CREATE VIEW `%1` AS %2;").arg(name).arg(model->query())))
        QMessageBox::information(this, qApp->applicationName(), tr("View successfully created."));
    else
        QMessageBox::warning(this, qApp->applicationName(), tr("Error creating view: %1").arg(db->lastErrorMessage));
}
