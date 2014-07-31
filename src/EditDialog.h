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

public:
    int getCurrentCol() { return curCol; }
    int getCurrentRow() { return curRow; }

public slots:
    virtual void reset();
    virtual void loadText(const QByteArray& data, int row, int col);

private slots:
    virtual void closeEvent(QCloseEvent*);
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
    void updateRecordText(int, int, const QByteArray&);

private:
    Ui::EditDialog* ui;
    QHexEdit* hexEdit;
    QByteArray oldData;
    int curCol;
    int curRow;
};

#endif
