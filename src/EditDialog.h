#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QPersistentModelIndex>

class QHexEdit;
class DockTextEdit;

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
    void switchEditorMode(bool autoSwitchForType);
    void updateCellInfoAndMode(const QByteArray& data);
    void setMustIndentAndCompact(bool enable);

signals:
    void recordTextUpdated(const QPersistentModelIndex& idx, const QByteArray& data, bool isBlob);

private:
    Ui::EditDialog* ui;
    QHexEdit* hexEdit;
    DockTextEdit* sciEdit;
    QPersistentModelIndex currentIndex;
    int dataSource;
    int dataType;
    bool isReadOnly;
    bool mustIndentAndCompact;
    QByteArray removedBom;

    enum DataSources {
        TextBuffer,
        HexBuffer,
        SciBuffer
    };

    // SVG is both an Image and an XML document so it is treated separately
    enum DataTypes {
        Binary,
        Image,
        Null,
        Text,
        JSON,
        SVG
    };

    // Edit modes and editor stack (this must be aligned with the UI)
    // Note that JSON and XML share the Scintilla widget.
    enum EditModes {
        TextEditor = 0,
        HexEditor = 1,
        ImageViewer = 2,
        JsonEditor, SciEditor = 3,
        XmlEditor = 4
    };

    int checkDataType(const QByteArray& data);
    QString humanReadableSize(double byteCount) const;
    bool promptInvalidData(const QString& dataType, const QString& errorString);
    void setDataInBuffer(const QByteArray& data, DataSources source);
    void setStackCurrentIndex(int editMode);
    void openPrintDialog();
};

#endif
