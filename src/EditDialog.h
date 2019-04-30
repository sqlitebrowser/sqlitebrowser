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
    ~EditDialog() override;

    void setCurrentIndex(const QModelIndex& idx);

public slots:
    void setFocus();
    void reject() override;
    void setReadOnly(bool ro);
    void reloadSettings();

protected:
    void showEvent(QShowEvent* ev) override;

private slots:
    void importData();
    void exportData();
    void setNull();
    void updateApplyButton();
    void accept() override;
    void loadData(const QByteArray& bArrdata);
    void toggleOverwriteMode();
    void editModeChanged(int newMode);
    void editTextChanged();
    void switchEditorMode(bool autoSwitchForType);
    void updateCellInfoAndMode(const QByteArray& bArrdata);
    void setMustIndentAndCompact(bool enable);
    void openPrintDialog();
    void openPrintImageDialog();
    void copyHexAscii();
    void setWordWrapping(bool value);

signals:
    void recordTextUpdated(const QPersistentModelIndex& idx, const QByteArray& bArrdata, bool isBlob);

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
        SVG,
        XML
    };

    // Edit modes and editor stack (this must be aligned with the UI).
    // Note that text modes (plain, JSON and XML) share the Scintilla widget,
    // Consequently the editor stack range is TextEditor..ImageViewer.
    enum EditModes {
        TextEditor = 0,
        HexEditor = 1,
        ImageViewer = 2,
        JsonEditor = 3,
        XmlEditor = 4
    };

    int checkDataType(const QByteArray& bArrdata);
    QString humanReadableSize(double byteCount) const;
    bool promptInvalidData(const QString& data_type, const QString& errorString);
    void setDataInBuffer(const QByteArray& bArrdata, DataSources source);
    void setStackCurrentIndex(int editMode);
};

#endif
