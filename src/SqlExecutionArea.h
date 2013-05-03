#ifndef __SQLEXECUTIONAREA_H__
#define __SQLEXECUTIONAREA_H__

#include <QFrame>
#include "sqltextedit.h"
class QSplitter;
class SqlTextEdit;
class ExtendedTableWidget;
class QLabel;
class QVBoxLayout;
class SQLiteSyntaxHighlighter;
class SqliteTableModel;
class DBBrowserDB;
class QStandardItemModel;

class SqlExecutionArea : public QFrame
{
    Q_OBJECT

public:
    explicit SqlExecutionArea(QWidget* parent = 0, DBBrowserDB* db = 0);

    QString getSql() const;
    QString getSelectedSql() const;

    SqliteTableModel* getModel() { return model; }
    SqlTextEdit* getEditor() { return editor; }

public slots:
    virtual void setTableNames(const QStringList& tables);
    virtual void finishExecution(const QString& result);

private:
    QVBoxLayout* layout;
        QSplitter* splitter;
            SqlTextEdit* editor;
                SQLiteSyntaxHighlighter* highlighter;
            ExtendedTableWidget* table;
                SqliteTableModel* model;
        QLabel* errors;
};

#endif
