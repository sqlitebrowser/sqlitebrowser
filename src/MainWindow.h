#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include "sqlitedb.h"

#define ORDERMODE_ASC 0
#define ORDERMODE_DESC 1

class QDragEnterEvent;
class SQLLogDock;
class editForm;
class findForm;
class SQLiteSyntaxHighlighter;
class QStandardItemModel;
class QIntValidator;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow* ui;
    SQLLogDock * logWin;

    QStandardItemModel *queryResultListModel;
    QMenu *popupTableMenu;
    QMenu *popupFieldMenu;
    QMenu *recentFilesMenu;

    SQLiteSyntaxHighlighter* sqliteHighlighter;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QAction *recentSeparatorAct;

    int curBrowseOrderByIndex;
    int curBrowseOrderByMode;

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

    editForm * editWin;
    QClipboard * clipboard;
    findForm * findWin;
    QIntValidator * gotoValidator;
    QString defaultlocation;

private:
    void init();

    void updateRecentFileActions();
    void setCurrentFile(const QString& fileName);
    void activateFields(bool enable = true);

protected:
    void closeEvent(QCloseEvent *);

public slots:
    virtual void on_tree_context_menu(const QPoint & qPoint);
    virtual void on_tree_selection_changed();
    virtual void on_add_field();
    virtual void on_edit_field();
    virtual void on_delete_field();

    virtual void fileOpen( const QString & fileName );
    virtual void fileOpen();
    virtual void fileNew();
    virtual void populateStructure();
    virtual void populateTable(const QString & tablename , bool keepColumnWidths = false);
    virtual void resetBrowser();
    virtual void fileClose();
    virtual void fileExit();
    virtual void addRecord();
    virtual void deleteRecord();
    virtual void updateTableView(int lineToSelect , bool keepColumnWidths = false);
    virtual void selectTableLine( int lineToSelect );
    virtual void navigatePrevious();
    virtual void navigateNext();
    virtual void navigateGoto();
    virtual void setRecordsetLabel();
    virtual void browseFind( bool open );
    virtual void browseFindAway();
    virtual void browseRefresh();
    virtual void lookfor( const QString & wfield, const QString & woperator, const QString & wsearchterm );
    virtual void showrecord( int dec );
    virtual void createTable();
    virtual void createIndex();
    virtual void compact();
    virtual void deleteTable();
    virtual void editTable();
    virtual void deleteIndex();
    virtual void copy();
    virtual void paste();
    virtual void helpWhatsThis();
    virtual void helpAbout();
    virtual void updateRecordText( int row, int col, QString newtext );
    virtual void logWinAway();
    virtual void editWinAway();
    virtual void editText( int row, int col );
    virtual void doubleClickTable( int row, int col );
    virtual void executeQuery();
    virtual void importTableFromCSV();
    virtual void exportTableToCSV();
    virtual void dbState( bool dirty );
    virtual void fileSave();
    virtual void fileRevert();
    virtual void exportDatabaseToSQL();
    virtual void importDatabaseFromSQL();
    virtual void openPreferences();
    virtual void updatePreferences();
    virtual void openRecentFile();
    virtual void loadPragmas();
    virtual void savePragmas();

protected:
    DBBrowserDB db;

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);

protected slots:
    virtual void mainTabSelected( int tabindex );
    virtual void browseTableHeaderClicked(int logicalindex);

};

#endif // MAINFORM_H
