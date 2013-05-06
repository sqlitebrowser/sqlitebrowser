#ifndef __SQLEXECUTIONAREA_H__
#define __SQLEXECUTIONAREA_H__

#include <QWidget>
#include "sqltextedit.h"
class SQLiteSyntaxHighlighter;
class SqliteTableModel;
class DBBrowserDB;

namespace Ui {
class SqlExecutionArea;
}

class SqlExecutionArea : public QWidget
{
    Q_OBJECT

public:
    explicit SqlExecutionArea(QWidget* parent = 0, DBBrowserDB* db = 0);
    ~SqlExecutionArea();

    QString getSql() const;
    QString getSelectedSql() const;

    SqliteTableModel* getModel() { return model; }
    SqlTextEdit* getEditor();

public slots:
    virtual void setTableNames(const QStringList& tables);
    virtual void finishExecution(const QString& result);

private:
    Ui::SqlExecutionArea* ui;
    SQLiteSyntaxHighlighter* highlighter;
    SqliteTableModel* model;
};

#endif
