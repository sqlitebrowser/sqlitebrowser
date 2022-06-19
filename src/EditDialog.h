#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QPersistentModelIndex>

class QHexEdit;
class DockTextEdit;
class ImageViewer;

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
    bool isModified() const;
    void setModified(bool modified);
    QPersistentModelIndex currentIndex() const { return m_currentIndex; }
    void promptSaveData();

public slots:
    void setFocus();
    void reject() override;
    void setReadOnly(bool ro);
    void reloadSettings();

protected:
    void showEvent(QShowEvent* ev) override;

private slots:
    void importData(bool asLink = false);
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
    void copyHexAscii();
    void setWordWrapping(bool value);

signals:
    void recordTextUpdated(const QPersistentModelIndex& idx, const QByteArray& bArrdata, bool isBlob);
    void evaluateText(const QPersistentModelIndex& idx, const std::string& text);
    void requestUrlOrFileOpen(const QString& urlString);

private:
    Ui::EditDialog* ui;
    QHexEdit* hexEdit;
    DockTextEdit* sciEdit;
    ImageViewer* imageEdit;
    QPersistentModelIndex m_currentIndex;
    int dataSource;
    int dataType;
    bool isReadOnly;
    bool mustIndentAndCompact;
    QByteArray removedBom;

    enum DataSources {
        QtBuffer,
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
        XML,
        RtlText
    };

    // Edit modes and editor stack (this must be aligned with the UI).
    // Note that text modes (plain, JSON, XML and SQL evaluator) share the Scintilla widget,
    // Consequently the editor stack range is TextEditor..ImageViewer.
    enum EditModes {
        // Modes with their own widget in the stack:
        TextEditor = 0,
        RtlTextEditor = 1,
        HexEditor = 2,
        ImageEditor = 3,
        // Modes in the Scintilla editor:
        JsonEditor = 4,
        XmlEditor = 5,
        SqlEvaluator = 6
    };

    int checkDataType(const QByteArray& bArrdata) const;
    bool promptInvalidData(const QString& data_type, const QString& errorString);
    void setDataInBuffer(const QByteArray& bArrdata, DataSources source);
    void setStackCurrentIndex(int editMode);
    void openDataWithExternal();
};

#endif
