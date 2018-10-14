#include "SqlExecutionArea.h"
#include "ui_SqlExecutionArea.h"
#include "sqltextedit.h"
#include "sqlitetablemodel.h"
#include "sqlitedb.h"
#include "Settings.h"
#include "ExportDataDialog.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QShortcut>

SqlExecutionArea::SqlExecutionArea(DBBrowserDB& _db, QWidget* parent) :
    QWidget(parent),
    db(_db),
    ui(new Ui::SqlExecutionArea),
    m_columnsResized(false),
    error_state(false)
{
    // Create UI
    ui->setupUi(this);

    // Create model
    model = new SqliteTableModel(db, this, Settings::getValue("db", "prefetchsize").toInt());
    ui->tableResult->setModel(model);
    connect(model, &SqliteTableModel::finishedFetch, this, &SqlExecutionArea::fetchedData);

    ui->findFrame->hide();

    QShortcut* shortcutHideFind = new QShortcut(QKeySequence("ESC"), ui->findLineEdit);
    connect(shortcutHideFind, SIGNAL(activated()), this, SLOT(hideFindFrame()));

    connect(ui->findLineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(findLineEdit_textChanged(const QString &)));
    connect(ui->previousToolButton, SIGNAL(clicked()), this, SLOT(findPrevious()));
    connect(ui->nextToolButton, SIGNAL(clicked()), this, SLOT(findNext()));
    connect(ui->findLineEdit, SIGNAL(returnPressed()), this, SLOT(findNext()));
    connect(ui->hideFindButton, SIGNAL(clicked()), this, SLOT(hideFindFrame()));

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

void SqlExecutionArea::finishExecution(const QString& result, const bool ok)
{
    error_state = !ok;
    m_columnsResized = false;
    ui->editErrors->setPlainText(result);
    // Set reddish background when not ok
    if (showErrorIndicators)
    {
        if (ok)
            ui->editErrors->setStyleSheet("");
        else
            ui->editErrors->setStyleSheet("QTextEdit {color: white; background-color: rgb(255, 102, 102)}");
    }

}

void SqlExecutionArea::fetchedData()
{
    // Don't resize the columns more than once to fit their contents. This is necessary because the finishedFetch signal of the model
    // is emitted for each loaded prefetch block and we want to avoid resizes while scrolling down.
    if(m_columnsResized)
        return;
    m_columnsResized = true;

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

QTextEdit* SqlExecutionArea::getStatusEdit()
{
    return ui->editErrors;
}

void SqlExecutionArea::saveAsCsv()
{
    ExportDataDialog dialog(db, ExportDataDialog::ExportFormatCsv, this, model->query());
    dialog.exec();
}

void SqlExecutionArea::reloadSettings()
{
    // Reload editor and table settings
    ui->editEditor->reloadSettings();
    ui->tableResult->reloadSettings();

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

    // Check if error indicators are enabled for the not-ok background clue
    showErrorIndicators = Settings::getValue("editor", "error_indicators").toBool();
    if (!showErrorIndicators)
        ui->editErrors->setStyleSheet("");

}

void SqlExecutionArea::find(QString expr, bool forward)
{

    bool found = ui->editEditor->findText
      (expr,
       ui->regexpCheckBox->isChecked(),
       ui->caseCheckBox->isChecked(),
       ui->wholeWordsCheckBox->isChecked(),
       /* wrap */ true,
       forward);

    // Set reddish background when not found
    if (found || expr == "")
        ui->findLineEdit->setStyleSheet("");
    else
        ui->findLineEdit->setStyleSheet("QLineEdit {color: white; background-color: rgb(255, 102, 102)}");

}

void SqlExecutionArea::findPrevious()
{
    find(ui->findLineEdit->text(), false);
}

void SqlExecutionArea::findNext()
{
    find(ui->findLineEdit->text(), true);
}

void SqlExecutionArea::findLineEdit_textChanged(const QString &)
{
    // When the text changes, perform an incremental search from cursor
    // position, or from begin of the selection position.

    // For incremental search while typing we need to start from the
    // begining of the current selection, otherwise we'd jump to the
    // next occurrence
    if (ui->editEditor->hasSelectedText()) {
        int lineFrom;
        int indexFrom;
        int lineTo;
        int indexTo;
        ui->editEditor->getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
        ui->editEditor->setCursorPosition(lineFrom, indexFrom);
    }

    find(ui->findLineEdit->text(), true);
}

void SqlExecutionArea::hideFindFrame()
{
    ui->editEditor->setFocus();
    ui->findFrame->hide();
    emit findFrameVisibilityChanged(false);
}

void SqlExecutionArea::setFindFrameVisibility(bool show)
{
    if (show) {
        ui->findFrame->show();
        ui->findLineEdit->setFocus();
        ui->findLineEdit->selectAll();
        emit findFrameVisibilityChanged(true);
    } else {
        hideFindFrame();
    }
}
