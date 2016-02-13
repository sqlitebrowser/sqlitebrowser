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
    explicit EditDialog(QWidget* parent = 0, bool forUseInDockWidget = false);
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
    virtual void clearData();
    virtual void accept();
    virtual void editTextChanged();
    virtual void hexDataChanged();
    virtual void checkDataType();
    virtual void toggleOverwriteMode();

signals:
    void goingAway();
    void updateRecordText(int row, int col, bool isBlob, const QByteArray& data);

private:
    Ui::EditDialog* ui;
    bool useInDock;
    QHexEdit* hexEdit;
    QByteArray oldData;
    int curCol;
    int curRow;
};

#endif
