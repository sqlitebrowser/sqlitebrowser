#include "SqlExecutionArea.h"
#include "sqltextedit.h"
#include "ExtendedTableWidget.h"
#include "SQLiteSyntaxHighlighter.h"
#include "sqlitetablemodel.h"
#include "sqlitedb.h"
#include "PreferencesDialog.h"
#include <QLabel>
#include <QSplitter>
#include <QVBoxLayout>

SqlExecutionArea::SqlExecutionArea(QWidget* parent, DBBrowserDB* db) :
    QFrame(parent)
{
    // Create widgets
    splitter = new QSplitter(Qt::Vertical, this);
    editor = new SqlTextEdit(this);
    table = new ExtendedTableWidget(this);
    errors = new QLabel(this);

    // Set up widgets
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(8);
    editor->setFont(font);

    highlighter = new SQLiteSyntaxHighlighter(editor->document());

    model = new SqliteTableModel(this, db, PreferencesDialog::getSettingsValue("db", "prefetchsize").toInt());
    table->setModel(model);

    // Build layout
    splitter->addWidget(editor);
    splitter->addWidget(table);

    layout = new QVBoxLayout(this);
    layout->addWidget(splitter);
    layout->addWidget(errors);

    setLayout(layout);
}

void SqlExecutionArea::setTableNames(const QStringList& tables)
{
    highlighter->setTableNames(tables);
}

QString SqlExecutionArea::getSql() const
{
    return editor->toPlainText().trimmed();
}

QString SqlExecutionArea::getSelectedSql() const
{
    return editor->textCursor().selectedText().trimmed();
}

void SqlExecutionArea::finishExecution(const QString& result)
{
    errors->setText(result);
    table->resizeColumnsToContents();
}
