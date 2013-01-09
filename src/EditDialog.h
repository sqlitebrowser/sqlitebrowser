#ifndef __EDITDIALOG_H__
#define __EDITDIALOG_H__

#include <QDialog>

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    EditDialog(QWidget* parent = 0);
    ~EditDialog();

    int curCol;
    int curRow;
    QString defaultlocation;

public slots:
    virtual void reset();
    virtual void enableExport(bool enabled);
    virtual void setDataType(int type, int size);
    virtual void closeEvent(QCloseEvent*);
    virtual void loadText(QString text, int row, int col);
    virtual void importData();
    virtual void exportData();
    virtual void clearData();
    virtual void accept();
    virtual void editTextChanged();

signals:
    void goingAway();
    void updateRecordText(int, int, QString);

protected:
    int dataType;
    int dataSize;

private:
    Ui::EditDialog* ui;
};

#endif
