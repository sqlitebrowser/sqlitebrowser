#include "EditDialog.h"
#include "ui_EditDialog.h"
#include "sqlitedb.h"
#include "PreferencesDialog.h"
#include "src/qhexedit.h"
#include "FileDialog.h"

#include <QMainWindow>
#include <QKeySequence>
#include <QShortcut>
#include <QImageReader>
#include <QBuffer>

EditDialog::EditDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditDialog)
{
    ui->setupUi(this);

    // Add Ctrl-Enter (Cmd-Enter on OSX) as a shortcut for the Apply button
    ui->buttonApply->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));

    QHBoxLayout* hexLayout = new QHBoxLayout(ui->editorBinary);
    hexEdit = new QHexEdit(this);
    hexLayout->addWidget(hexEdit);
    hexEdit->setOverwriteMode(false);
    connect(hexEdit, SIGNAL(dataChanged()), this, SLOT(hexDataChanged()));

    QShortcut* ins = new QShortcut(QKeySequence(Qt::Key_Insert), this);
    connect(ins, SIGNAL(activated()), this, SLOT(toggleOverwriteMode()));

    reset();
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::reset()
{
    // Set the font for the text and hex editors
    QFont editorFont(PreferencesDialog::getSettingsValue("databrowser", "font").toString());
    editorFont.setPointSize(PreferencesDialog::getSettingsValue("databrowser", "fontsize").toInt());
    ui->editorText->setFont(editorFont);
    hexEdit->setFont(editorFont);

    curRow = -1;
    curCol = -1;
    ui->editorText->clear();
    ui->editorText->setFocus();
    ui->editorImage->clear();
    hexEdit->setData(QByteArray());
    oldData = "";

    // Hide the warning about editing non-text data in text mode
    ui->labelBinaryWarning->setVisible(false);

    // Update the cell data info in the bottom left of the Edit Cell
    int dataType = checkDataType();
    updateCellInfo(dataType);
}

void EditDialog::closeEvent(QCloseEvent*)
{
    emit goingAway();
}

void EditDialog::showEvent(QShowEvent*)
{
    // Whenever the dialog is shown, position it at the center of the parent dialog
    QMainWindow* parentDialog = qobject_cast<QMainWindow*>(parent());
    if(parentDialog)
    {
        move(parentDialog->x() + parentDialog->width() / 2 - width() / 2,
            parentDialog->y() + parentDialog->height() / 2 - height() / 2);
    }
}

void EditDialog::reject()
{
    // We override this, to ensure the Escape key doesn't make the Edit Cell
    // dock go away
    return;
}

void EditDialog::loadText(const QByteArray& data, int row, int col)
{
    curRow = row;
    curCol = col;
    oldData = data;

    // Load data
    QString textData = QString::fromUtf8(data.constData(), data.size());
    ui->editorText->setPlainText(textData);
    hexEdit->setData(data);

    // Determine the data type in the cell
    int dataType = checkDataType();

    // Do some data type specific handling
    QImage img;
    switch (dataType) {
    case Text:
        // For text, ensure it's all selected in the Edit Cell
        ui->editorText->selectAll();

        // Clear any image from the image viewing widget
        ui->editorImage->setPixmap(QPixmap(0,0));
        break;

    case Image:
        // For images, load the image into the image viewing widget
        if (img.loadFromData(hexEdit->data())) {
            ui->editorImage->setPixmap(QPixmap::fromImage(img));
        }
        break;

    default:
        // Clear the image viewing widget for everything else too
        ui->editorImage->setPixmap(QPixmap(0,0));
        break;
    }

    // Get the current editor mode (eg text, hex, or image mode)
    int editMode = ui->editorStack->currentIndex();

    // Show or hide the warning about editing binary data in text mode
    updateBinaryEditWarning(editMode, dataType);

    // Update the cell data info in the bottom left of the Edit Cell
    updateCellInfo(dataType);
}

void EditDialog::importData()
{
    // Get list of supported image file formats to include them in the file dialog filter
    QString image_formats;
    QList<QByteArray> image_formats_list = QImageReader::supportedImageFormats();
    for(int i=0;i<image_formats_list.size();++i)
        image_formats.append(QString("*.%1 ").arg(QString::fromUtf8(image_formats_list.at(i))));

    QString fileName = FileDialog::getOpenFileName(
                this,
                tr("Choose a file")
#ifndef Q_OS_MAC // Filters on OS X are buggy
                , tr("Text files(*.txt);;Image files(%1);;All files(*)").arg(image_formats)
#endif
                );
    if(QFile::exists(fileName))
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray d = file.readAll();
            hexEdit->setData(d);
            ui->editorText->setPlainText(d);
            file.close();

            // Update the cell data info in the bottom left of the Edit Cell
            int dataType = checkDataType();
            updateCellInfo(dataType);
        }
    }
}

void EditDialog::exportData()
{
    QString fileName = FileDialog::getSaveFileName(
                this,
                tr("Choose a filename to export data"),
                tr("Text files(*.txt);;All files(*)"));

    if(fileName.size() > 0)
    {
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(hexEdit->data());
            file.close();
        }
    }
}

void EditDialog::setNull()
{
    ui->editorText->clear();
    ui->editorImage->clear();
    hexEdit->setData(QByteArray());

    // Update the cell data info in the bottom left of the Edit Cell
    int dataType = checkDataType();
    updateCellInfo(dataType);

    ui->editorText->setFocus();
}

void EditDialog::accept()
{
    // Don't update if the data hasn't changed
    // To differentiate NULL and empty byte arrays, we also compare the NULL flag
    if(hexEdit->data() != oldData || hexEdit->data().isNull() != oldData.isNull())
    {
        const QString dataType = ui->comboMode->currentText();
        bool isBlob = dataType == tr("Binary");
        emit updateRecordText(curRow, curCol, isBlob, hexEdit->data());
    }
}

// Called when the user manually changes the "Mode" drop down combobox
void EditDialog::editModeChanged(int editMode)
{
    // Switch to the selected editor
    ui->editorStack->setCurrentIndex(editMode);

    // Show or hide the warning about editing binary data in text mode
    int dataType = checkDataType();
    updateBinaryEditWarning(editMode, dataType);
}

void EditDialog::editTextChanged()
{
    if(ui->editorText->hasFocus())
    {
        hexEdit->blockSignals(true);
        hexEdit->setData(ui->editorText->toPlainText().toUtf8());
        hexEdit->blockSignals(false);

        // Update the cell data info in the bottom left of the Edit Cell
        int dataType = checkDataType();
        updateCellInfo(dataType);
    }
}

void EditDialog::hexDataChanged()
{
    // Update the text editor accordingly
    ui->editorText->blockSignals(true);
    ui->editorText->setPlainText(QString::fromUtf8(hexEdit->data().constData(), hexEdit->data().size()));
    ui->editorText->blockSignals(false);
}

// Determine the type of data in the cell
int EditDialog::checkDataType()
{
    QByteArray cellData = hexEdit->data();

    // Check for NULL data type
    if (cellData.isNull()) {
        return Null;
    }

    // Check if it's an image
    QBuffer imageBuffer(&cellData);
    QImageReader readerBuffer(&imageBuffer);
    bool someCheck = readerBuffer.canRead();
    if (someCheck == true) {
        return Image;
    }

    // Check if it's text only
    if (QString(cellData).toUtf8() == cellData) { // Is there a better way to check this?
        return Text;
    }

    // It's none of the above, so treat it as general binary data
    return Binary;
}

void EditDialog::toggleOverwriteMode()
{
    static bool currentMode = false;
    currentMode = !currentMode;

    hexEdit->setOverwriteMode(currentMode);
    ui->editorText->setOverwriteMode(currentMode);
}

void EditDialog::setFocus()
{
    QDialog::setFocus();

    // Set the focus to the editor widget. The idea here is that setting focus
    // to the dock itself doesn't make much sense as it's just a frame; you'd
    // have to tab to the editor which is what you most likely want to use. So
    // in order to save the user from doing this we explicitly set the focus
    // to the editor.
    ui->editorText->setFocus();
    ui->editorText->selectAll();
}

// Enables or disables the Apply, Null, & Import buttons in the Edit Cell dock
void EditDialog::allowEditing(bool on)
{
    ui->buttonApply->setEnabled(on);
    ui->buttonNull->setEnabled(on);
    ui->buttonImport->setEnabled(on);
}

// Shows or hides the warning about editing binary data in text mode
void EditDialog::updateBinaryEditWarning(int editMode, int dataType)
{
    // If we're in the text editor, and the data type is not plain text
    // display a warning about editing it
    if ((editMode == 0) && ((dataType != Text) && (dataType != Null))) {
        // Display the warning
        ui->labelBinaryWarning->setVisible(true);
    } else {
        // Hide the warning
        ui->labelBinaryWarning->setVisible(false);
    }
}

// Update the information labels in the bottom left corner of the dialog
void EditDialog::updateCellInfo(int cellType)
{
    QByteArray cellData = hexEdit->data();

    // Image data needs special treatment
    if (cellType == Image) {
        QBuffer imageBuffer(&cellData);
        QImageReader image(&imageBuffer);

        // Display the image format
        QString imageFormat = image.format();
        ui->labelType->setText(tr("Type of data currently in cell: %1 Image").arg(imageFormat.toUpper()));

        // Display the image dimensions
        QSize imageSize = image.size();
        ui->labelSize->setText(tr("%1x%2 pixel(s)").arg(imageSize.width()).arg(imageSize.height()));
        return;
    }

    // Determine the legth of the cell data
    int dataLength = cellData.length();

    // Use a switch statement for the other data types to keep things neat :)
    switch (cellType) {
    case Null:
        // NULL data type
        ui->labelType->setText(tr("Type of data currently in cell: NULL"));
        ui->labelSize->setText(tr("%n byte(s)", "", dataLength));
        break;

    case Text:
        // Text only
        ui->labelType->setText(tr("Type of data currently in cell: Text / Numeric"));
        ui->labelSize->setText(tr("%n char(s)", "", dataLength));
        break;

    default:
        // If none of the above data types, consider it general binary data
        ui->labelType->setText(tr("Type of data currently in cell: Binary"));
        ui->labelSize->setText(tr("%n byte(s)", "", dataLength));
        break;
    }
}
