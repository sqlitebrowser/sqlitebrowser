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
    //menuPopupSave->addAction(ui->actionSaveAsView);
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
    // TODO
}
