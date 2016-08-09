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
    virtual void loadDataFromCell(const QByteArray& data, int row, int col);
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
    virtual int checkDataType(const QByteArray& data);
    virtual void loadData(const QByteArray& data);
    virtual void toggleOverwriteMode();
    virtual void editModeChanged(int newMode);
    virtual void updateCellInfo(const QByteArray& data);
    virtual QString humanReadableSize(double byteCount);

signals:
    void goingAway();
    void recordTextUpdated(int row, int col, bool isBlob, const QByteArray& data);

private:
    Ui::EditDialog* ui;
    QHexEdit* hexEdit;
    QByteArray oldData;
    int curCol;
    int curRow;
    int dataSource;
    int dataType;

    enum DataSources {
        TextBuffer,
        HexBuffer
    };

    enum DataTypes {
        Binary,
        Image,
        Null,
        Text
    };

    enum EditModes {
        TextEditor = 0,
        HexEditor = 1,
        ImageViewer = 2
    };
};

#endif
