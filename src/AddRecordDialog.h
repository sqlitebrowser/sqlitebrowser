#ifndef ADDRECORDDIALOG_H
#define ADDRECORDDIALOG_H

#include "sql/sqlitetypes.h"

#include <QDialog>

class DBBrowserDB;
class QTreeWidgetItem;

namespace Ui {
class AddRecordDialog;
}
class QAbstractButton;


class AddRecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddRecordDialog(DBBrowserDB& pdb, const sqlb::ObjectIdentifier& tableName, QWidget* parent = nullptr, const QString& pseudo_pk = QString());
    ~AddRecordDialog() override;

protected:
    void keyPressEvent(QKeyEvent *evt) override;

private:
    enum Columns {
        kName = 0,
        kType = 1,
        kValue = 2,
    };

    void updateSqlText();
    void populateFields();
    void setDefaultsStyle(QTreeWidgetItem* item);

private slots:
    void accept() override;
    void itemChanged(QTreeWidgetItem* item, int column);
    void help();
    void on_buttonBox_clicked(QAbstractButton* button);

private:
    Ui::AddRecordDialog* ui;
    DBBrowserDB& pdb;
    sqlb::ObjectIdentifier curTable;
    QString pseudo_pk;
};

#endif
