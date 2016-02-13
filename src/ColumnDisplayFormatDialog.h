#ifndef COLUMNDISPLAYFORMATDIALOG_H
#define COLUMNDISPLAYFORMATDIALOG_H

#include <QDialog>

namespace Ui {
class ColumnDisplayFormatDialog;
}

class ColumnDisplayFormatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColumnDisplayFormatDialog(const QString& colname, QString current_format, QWidget* parent = 0);
    ~ColumnDisplayFormatDialog();

    QString selectedDisplayFormat() const;

private slots:
    void updateSqlCode();

private:
    Ui::ColumnDisplayFormatDialog* ui;
    QString column_name;
};

#endif
