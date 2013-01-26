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
    explicit EditDialog(QWidget* parent = 0);
    ~EditDialog();

    QString defaultlocation;

public:
    int getCurrentCol() { return curCol; }
    int getCurrentRow() { return curRow; }

public slots:
    virtual void reset();
    virtual void loadText(const QString& text, int row, int col);

private slots:
    virtual void enableExport(bool enabled);
    virtual void setDataType(int type, int size);
    virtual void closeEvent(QCloseEvent*);
    virtual void importData();
    virtual void exportData();
    virtual void clearData();
    virtual void accept();
    virtual void editTextChanged();

signals:
    void goingAway();
    void updateRecordText(int, int, const QString&);

private:
    Ui::EditDialog* ui;
    int dataType;
    int dataSize;
    int curCol;
    int curRow;
};

#endif
