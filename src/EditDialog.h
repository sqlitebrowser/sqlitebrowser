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
    void reloadSettings();

protected:
    virtual void showEvent(QShowEvent* ev);

private slots:
    void importData();
    void exportData();
    void setNull();
    void updateApplyButton();
    virtual void accept();
    int checkDataType(const QByteArray& data);
    void loadData(const QByteArray& data);
    void toggleOverwriteMode();
    void editModeChanged(int newMode);
    void editTextChanged();
    void updateCellInfo(const QByteArray& data);
    QString humanReadableSize(double byteCount);

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
