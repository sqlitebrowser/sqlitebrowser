#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QPersistentModelIndex>

#include "jsontextedit.h"

class QHexEdit;

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget* parent = nullptr);
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
    void loadData(const QByteArray& data);
    void toggleOverwriteMode();
    void editModeChanged(int newMode);
    void editTextChanged();
    void updateCellInfo(const QByteArray& data);
    void setMustIndentAndCompact(bool enable);

signals:
    void recordTextUpdated(const QPersistentModelIndex& idx, const QByteArray& data, bool isBlob);

private:
    Ui::EditDialog* ui;
    QHexEdit* hexEdit;
    JsonTextEdit* jsonEdit;
    QPersistentModelIndex currentIndex;
    int dataSource;
    int dataType;
    bool textNullSet;
    bool isReadOnly;
    bool mustIndentAndCompact;

    enum DataSources {
        TextBuffer,
        HexBuffer,
        JsonBuffer
    };

    enum DataTypes {
        Binary,
        Image,
        Null,
        Text,
        JSON
    };

    enum EditModes {
        TextEditor = 0,
        HexEditor = 1,
        JsonEditor = 2,
        ImageViewer = 3
    };

    int checkDataType(const QByteArray& data);
    QString humanReadableSize(double byteCount) const;
    bool promptInvalidData(const QString& dataType, const QString& errorString);
};

#endif
