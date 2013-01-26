#ifndef __FINDDIALOG_H__
#define __FINDDIALOG_H__

#include <QDialog>
#include "sqlitedb.h"
class QTableWidgetItem;

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget* parent = 0);
    ~FindDialog();

public slots:
    virtual void showResults(const resultMap& rmap);
    virtual void resetFields(const QStringList& fieldlist = QStringList());

private slots:
    virtual void find();
    virtual void recordSelected(QTableWidgetItem* witem);
    virtual void closeEvent(QCloseEvent*);

signals:
    void lookfor(const QString&, const QString&, const QString&);
    void showrecord(int);
    void goingAway();

private:
    Ui::FindDialog* ui;
};

#endif
