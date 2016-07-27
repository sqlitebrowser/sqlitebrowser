#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

class QHexEdit;

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget* parent = 0);
    ~EditDialog();

    int getCurrentCol() { return curCol; }
    int getCurrentRow() { return curRow; }

public slots:
    virtual void reset();
    virtual void loadText(const QByteArray& data, int row, int col);
    virtual void setFocus();
    virtual void reject();
    virtual void allowEditing(bool on);

protected:
    virtual void closeEvent(QCloseEvent* ev);
    virtual void showEvent(QShowEvent* ev);

private slots:
    virtual void importData();
    virtual void exportData();
    virtual void setNull();
    virtual void accept();
    virtual void editTextChanged();
    virtual void hexDataChanged();
    virtual int checkDataType();
    virtual void toggleOverwriteMode();
    virtual void editModeChanged(int editMode);
    virtual void updateBinaryEditWarning(int editMode, int dataType);
    virtual void updateCellInfo(int cellType);

signals:
    void goingAway();
    void updateRecordText(int row, int col, bool isBlob, const QByteArray& data);

private:
    Ui::EditDialog* ui;
    QHexEdit* hexEdit;
    QByteArray oldData;
    int curCol;
    int curRow;

    enum DataType {
        Binary,
        Image,
        Null,
        Text
    };
};

#endif
