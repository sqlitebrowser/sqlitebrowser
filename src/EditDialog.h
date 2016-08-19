#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QPersistentModelIndex>

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

    void setCurrentIndex(const QModelIndex& idx);

public slots:
    virtual void setFocus();
    virtual void reject();
    void setReadOnly(bool ro);

protected:
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
    virtual void editTextChanged();
    virtual void updateCellInfo(const QByteArray& data);
    virtual QString humanReadableSize(double byteCount);

signals:
    void recordTextUpdated(const QPersistentModelIndex& idx, const QByteArray& data, bool isBlob);

private:
    Ui::EditDialog* ui;
    QHexEdit* hexEdit;
    QPersistentModelIndex currentIndex;
    int dataSource;
    int dataType;
    bool textNullSet;
    bool isReadOnly;

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
