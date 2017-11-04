#ifndef COLUMNDISPLAYFORMATDIALOG_H
#define COLUMNDISPLAYFORMATDIALOG_H

#include <QDialog>
#include <QString>
#include <QMap>

namespace Ui {
class ColumnDisplayFormatDialog;
}

class ColumnDisplayFormatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColumnDisplayFormatDialog(const QString& colname, QString current_format, QWidget* parent = nullptr);
    ~ColumnDisplayFormatDialog();

    QString selectedDisplayFormat() const;

private slots:
    void updateSqlCode();

private:
    Ui::ColumnDisplayFormatDialog* ui;
    QString column_name;
    QMap<QString, QString> formatFunctions;
};

#endif
