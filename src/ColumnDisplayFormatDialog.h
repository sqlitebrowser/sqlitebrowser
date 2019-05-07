#ifndef COLUMNDISPLAYFORMATDIALOG_H
#define COLUMNDISPLAYFORMATDIALOG_H

#include <QDialog>

#include <unordered_map>

#include "sql/ObjectIdentifier.h"

class QString;

class DBBrowserDB;

namespace Ui {
class ColumnDisplayFormatDialog;
}

class ColumnDisplayFormatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColumnDisplayFormatDialog(DBBrowserDB& db, const sqlb::ObjectIdentifier& tableName, const QString& colname, const QString& current_format, QWidget* parent = nullptr);
    ~ColumnDisplayFormatDialog() override;

    QString selectedDisplayFormat() const;

private slots:
    void updateSqlCode();
    void accept() override;
    void setCustom(bool modified);

private:
    Ui::ColumnDisplayFormatDialog* ui;
    QString column_name;
    std::unordered_map<std::string, QString> formatFunctions;
    DBBrowserDB& pdb;
    sqlb::ObjectIdentifier curTable;
};

#endif
