#ifndef SQLEXECUTIONAREA_H
#define SQLEXECUTIONAREA_H

#include <QWidget>
#include <QFileSystemWatcher>

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
    void setSql(const QString& sql);

    void openFile(const QString& filename);
    void saveFile(const QString& filename);

    QString fileName() const { return sqlFileName; }
    void setFileName(const QString& filename) { sqlFileName = filename; }

    SqliteTableModel* getModel() { return model; }
    SqlTextEdit* getEditor();
    ExtendedTableWidget *getTableResult();
    QTextEdit* getStatusEdit();

    bool inErrorState() const { return error_state; }

    // Save window state to settings
    static void saveState();

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

    void fileChanged(const QString& filename);

signals:
    void findFrameVisibilityChanged(bool visible);

private:
    void find(QString expr, bool forward);
    DBBrowserDB& db;
    SqliteTableModel* model;
    QString sqlFileName;
    QFileSystemWatcher fileSystemWatch;
    Ui::SqlExecutionArea* ui;
    bool m_columnsResized;              // This is set to true if the columns of the table view were already adjusted to fit their contents
    bool showErrorIndicators;
    bool error_state;
    bool follow_mode;
};

#endif
