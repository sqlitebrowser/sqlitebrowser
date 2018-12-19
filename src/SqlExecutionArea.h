#ifndef SQLEXECUTIONAREA_H
#define SQLEXECUTIONAREA_H

#include <QWidget>

class SqlTextEdit;
class SqliteTableModel;
class DBBrowserDB;
class ExtendedTableWidget;

class QTextEdit;

namespace Ui {
class SqlExecutionArea;
}

class SqlExecutionArea : public QWidget
{
    Q_OBJECT

public:
    explicit SqlExecutionArea(DBBrowserDB& _db, QWidget* parent = nullptr);
    ~SqlExecutionArea() override;

    QString getSql() const;
    QString getSelectedSql() const;

    QString fileName() const { return sqlFileName; }
    void setFileName(const QString& filename) { sqlFileName = filename; }

    SqliteTableModel* getModel() { return model; }
    SqlTextEdit* getEditor();
    ExtendedTableWidget *getTableResult();
    QTextEdit* getStatusEdit();

    bool inErrorState() const { return error_state; }

public slots:
    void finishExecution(const QString& result, const bool ok);
    void saveAsCsv();
    void reloadSettings();
    void fetchedData();
    void setFindFrameVisibility(bool show);

private slots:
    void findPrevious();
    void findNext();
    void findLineEdit_textChanged(const QString& text);
    void hideFindFrame();

signals:
    void findFrameVisibilityChanged(bool visible);

private:
    void find(QString expr, bool forward);
    DBBrowserDB& db;
    SqliteTableModel* model;
    QString sqlFileName;
    Ui::SqlExecutionArea* ui;
    bool m_columnsResized;              // This is set to true if the columns of the table view were already adjusted to fit their contents
    bool showErrorIndicators;
    bool error_state;
};

#endif
