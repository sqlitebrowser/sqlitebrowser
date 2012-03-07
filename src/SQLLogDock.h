#ifndef SQLLOGFORM_H
#define SQLLOGFORM_H

#include <QTextEdit>
#include <QStackedWidget>
#include <QtGui/QAction>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class SQLiteSyntaxHighlighter;

class SQLLogDock : public QDockWidget
{
    Q_OBJECT

public:
    SQLLogDock(QWidget* parent = 0);
    ~SQLLogDock();

    QComboBox* comboLogType() { return m_comboLogType; }
    SQLiteSyntaxHighlighter* userSqliteHighlighter() { return mUserSqliteHighlighter; }
    SQLiteSyntaxHighlighter* appSqliteHighlighter() { return mAppSqliteHighlighter; }

private:
    QWidget* contentWidget;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *textLabel1;
    QComboBox *m_comboLogType;
    QSpacerItem *spacer10;
    QPushButton *clearButton;
    QStackedWidget *logStack;
    QWidget *WStackPage;
    QGridLayout *gridLayout;
    QTextEdit *userLogText;
    QWidget *WStackPage1;
    QVBoxLayout *vboxLayout1;
    QTextEdit *appLogText;

    SQLiteSyntaxHighlighter* mUserSqliteHighlighter;
    SQLiteSyntaxHighlighter* mAppSqliteHighlighter;

    void setupUi();
    void retranslateUi();

public slots:
    virtual void closeEvent( QCloseEvent * );
    virtual void log( QString & statement, int msgtype );
    virtual void msgDBDirtyState( bool dirty );
    virtual void clearLog();

signals:
    void goingAway();
    void dbState(bool dirty);

protected slots:
    virtual void languageChange();

};

#endif // SQLLOGFORM_H
