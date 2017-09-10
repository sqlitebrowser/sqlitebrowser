#include "EditDialog.h"
#include "ui_EditDialog.h"
#include "sqlitedb.h"
#include "Settings.h"
#include "src/qhexedit.h"
#include "FileDialog.h"

#include <QMainWindow>
#include <QKeySequence>
#include <QShortcut>
#include <QImageReader>
#include <QBuffer>
#include <QModelIndex>

EditDialog::EditDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditDialog),
      currentIndex(QModelIndex()),
      dataType(Null),
      isReadOnly(true)
{
    ui->setupUi(this);

    // Add Ctrl-Enter (Cmd-Enter on OSX) as a shortcut for the Apply button
    ui->buttonApply->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));

    QHBoxLayout* hexLayout = new QHBoxLayout(ui->editorBinary);
    hexEdit = new QHexEdit(this);
    hexLayout->addWidget(hexEdit);
    hexEdit->setOverwriteMode(false);

    QShortcut* ins = new QShortcut(QKeySequence(Qt::Key_Insert), this);
    connect(ins, SIGNAL(activated()), this, SLOT(toggleOverwriteMode()));

    connect(ui->editorText, SIGNAL(textChanged()), this, SLOT(updateApplyButton()));
    connect(hexEdit, SIGNAL(dataChanged()), this, SLOT(updateApplyButton()));

    reloadSettings();
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::setCurrentIndex(const QModelIndex& idx)
{
    currentIndex = QPersistentModelIndex(idx);

    QByteArray data = idx.data(Qt::EditRole).toByteArray();
    loadData(data);
    updateCellInfo(data);

    ui->buttonApply->setDisabled(true);
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

// Loads data from a cell into the Edit Cell window
void EditDialog::loadData(const QByteArray& data)
{
    QImage img;
    QString textData;

    // Determine the data type, saving that info in the class variable
    dataType = checkDataType(data);

    // Get the current editor mode (eg text, hex, or image mode)
    int editMode = ui->editorStack->currentIndex();

    // Data type specific handling
    switch (dataType) {
    case Null:
        switch (editMode) {
        case TextEditor:
            // The text widget buffer is now the main data source
            dataSource = TextBuffer;

            // Empty the text editor contents, then enable text editing
            ui->editorText->clear();
            ui->editorText->setEnabled(true);

            break;

        case HexEditor:
            // The hex widget buffer is now the main data source
            dataSource = HexBuffer;

            // Load the Null into the hex editor
            hexEdit->setData(data);

            break;

        case ImageViewer:
            // The hex widget buffer is now the main data source
            dataSource = HexBuffer;

            // Clear any image from the image viewing widget
            ui->editorImage->setPixmap(QPixmap(0,0));

            // Load the Null into the hex editor
            hexEdit->setData(data);

            break;
        }
        break;

    case Text:
        switch (editMode) {
        case TextEditor:
            // The text widget buffer is now the main data source
            dataSource = TextBuffer;

            // Load the text into the text editor
            textData = QString::fromUtf8(data.constData(), data.size());
            ui->editorText->setPlainText(textData);

            // Enable text editing
            ui->editorText->setEnabled(true);

            // Select all of the text by default
            ui->editorText->selectAll();

            break;

        case HexEditor:
            // The hex widget buffer is now the main data source
            dataSource = HexBuffer;

            // Load the text into the hex editor
            hexEdit->setData(data);

            break;

        case ImageViewer:
            // The text widget buffer is now the main data source
            dataSource = TextBuffer;

            // Clear any image from the image viewing widget
            ui->editorImage->setPixmap(QPixmap(0,0));

            // Load the text into the text editor
            textData = QString::fromUtf8(data.constData(), data.size());
            ui->editorText->setPlainText(textData);

            // Enable text editing
            ui->editorText->setEnabled(true);

            // Select all of the text by default
            ui->editorText->selectAll();

            break;
        }
        break;

    case Image:
        // Image data is kept in the hex widget, mainly for safety.  If we
        // stored it in the editorImage widget instead, it would be a pixmap
        // and there's no good way to restore that back to the original
        // (pristine) image data.  eg image metadata would be lost
        hexEdit->setData(data);

        // The hex widget buffer is now the main data source
        dataSource = HexBuffer;

        // Update the display if in text edit or image viewer mode
        switch (editMode) {
        case TextEditor:
            // Disable text editing, and use a warning message as the contents
            ui->editorText->setText(QString("<i>" %
                    tr("Image data can't be viewed with the text editor") %
                    "</i>"));
            ui->editorText->setEnabled(false);
            break;

        case ImageViewer:
            // Load the image into the image viewing widget
            if (img.loadFromData(data)) {
                ui->editorImage->setPixmap(QPixmap::fromImage(img));
            }
            break;
        }
        break;

    default:
        // The hex widget buffer is now the main data source
        dataSource = HexBuffer;

        // The data seems to be general binary data, which is always loaded
        // into the hex widget (the only safe place for it)

        // Load the data into the hex editor
        hexEdit->setData(data);

        switch (editMode) {
        case TextEditor:
            // Disable text editing, and use a warning message as the contents
            ui->editorText->setText(QString("<i>" %
                    tr("Binary data can't be viewed with the text editor") %
                    "</i>"));
            ui->editorText->setEnabled(false);
            break;

        case ImageViewer:
            // Clear any image from the image viewing widget
            ui->editorImage->setPixmap(QPixmap(0,0));
            break;
        }
    }
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
            loadData(d);
            file.close();

            // Update the cell data info in the bottom left of the Edit Cell
            updateCellInfo(d);
        }
    }
}

void EditDialog::exportData()
{
    // Images get special treatment
    QString fileExt;
    if (dataType == Image) {
        // Determine the likely filename extension
        QByteArray cellData = hexEdit->data();
        QBuffer imageBuffer(&cellData);
        QImageReader imageReader(&imageBuffer);
        QString imageFormat = imageReader.format();
        fileExt = imageFormat.toUpper() % " " % tr("Image") % "(*." % imageFormat.toLower() % ");;All files(*)";
    } else {
        fileExt = tr("Text files(*.txt);;All files(*)");
    }

    QString fileName = FileDialog::getSaveFileName(
                this,
                tr("Choose a filename to export data"),
                fileExt);
    if(fileName.size() > 0)
    {
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly))
        {
            if (dataSource == HexBuffer) {
                // Data source is the hex buffer
                file.write(hexEdit->data());
            } else {
                // Data source is the text buffer
                file.write(ui->editorText->toPlainText().toUtf8());
            }
            file.close();
        }
    }
}

void EditDialog::setNull()
{
    ui->editorText->clear();
    ui->editorImage->clear();
    hexEdit->setData(QByteArray());
    dataType = Null;

    // Check if in text editor mode
    int editMode = ui->editorStack->currentIndex();
    if (editMode == TextEditor) {
        // Setting NULL in the text editor switches the data source to it
        dataSource = TextBuffer;

        // Ensure the text editor is enabled
        ui->editorText->setEnabled(true);

        // The text editor doesn't know the difference between an empty string
        // and a NULL, so we need to record NULL outside of that
        textNullSet = true;
    }

    // Update the cell data info in the bottom left of the Edit Cell
    updateCellInfo(hexEdit->data());

    ui->editorText->setFocus();
}

void EditDialog::updateApplyButton()
{
    if (!isReadOnly)
        ui->buttonApply->setEnabled(true);
}

void EditDialog::accept()
{
    if(!currentIndex.isValid())
        return;

    if (dataSource == TextBuffer) {
        // Check if a NULL is set in the text editor
        if (textNullSet) {
            emit recordTextUpdated(currentIndex, hexEdit->data(), true);
        } else {
            // It's not NULL, so proceed with normal text string checking
            QString oldData = currentIndex.data(Qt::EditRole).toString();
            QString newData = ui->editorText->toPlainText();
            if (oldData != newData)
                // The data is different, so commit it back to the database
                emit recordTextUpdated(currentIndex, newData.toUtf8(), false);
        }
    } else {
        // The data source is the hex widget buffer, thus binary data
        QByteArray oldData = currentIndex.data(Qt::EditRole).toByteArray();
        QByteArray newData = hexEdit->data();
        if (newData != oldData)
            emit recordTextUpdated(currentIndex, newData, true);
    }
}

// Called when the user manually changes the "Mode" drop down combobox
void EditDialog::editModeChanged(int newMode)
{
    // * If the dataSource is the text buffer, the data is always text *
    if (dataSource == TextBuffer) {
        switch (newMode) {
        case TextEditor: // Switching to the text editor
            // Nothing to do, as the text is already in the text buffer
            break;

        case HexEditor: // Switching to the hex editor
            // Convert the text widget buffer for the hex widget
            hexEdit->setData(ui->editorText->toPlainText().toUtf8());

            // The hex widget buffer is now the main data source
            dataSource = HexBuffer;
            break;

        case ImageViewer:
            // Clear any image from the image viewing widget
            ui->editorImage->setPixmap(QPixmap(0,0));
            break;
        }

        // Switch to the selected editor
        ui->editorStack->setCurrentIndex(newMode);
        return;
    }

    // * If the dataSource is the hex buffer, the contents could be anything
    //   so we just pass it to our loadData() function to handle *
    if (dataSource == HexBuffer) {
        // Switch to the selected editor first, as loadData() relies on it
        // being current
        ui->editorStack->setCurrentIndex(newMode);

        // Load the data into the appropriate widget, as done by loadData()
        loadData(hexEdit->data());
    }
}

// Called for every keystroke in the text editor (only)
void EditDialog::editTextChanged()
{
    if (dataSource == TextBuffer) {
        // Data has been changed in the text editor, so it can't be a NULL
        // any more
        textNullSet = false;

        // Update the cell info in the bottom left manually.  This is because
        // updateCellInfo() only works with QByteArray's (for now)
        int dataLength = ui->editorText->toPlainText().length();
        ui->labelType->setText(tr("Type of data currently in cell: Text / Numeric"));
        ui->labelSize->setText(tr("%n char(s)", "", dataLength));
    }
}

// Determine the type of data in the cell
int EditDialog::checkDataType(const QByteArray& data)
{
    QByteArray cellData = data;

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
void EditDialog::setReadOnly(bool ro)
{
    isReadOnly = ro;

    ui->buttonApply->setEnabled(!ro);
    ui->buttonNull->setEnabled(!ro);
    ui->buttonImport->setEnabled(!ro);
    ui->editorText->setReadOnly(ro);
    ui->editorBinary->setEnabled(!ro);  // We disable the entire hex editor here instead of setting it to read only because it doesn't have a setReadOnly() method
}

// Update the information labels in the bottom left corner of the dialog
void EditDialog::updateCellInfo(const QByteArray& data)
{
    QByteArray cellData = data;

    // Image data needs special treatment
    if (dataType == Image) {
        QBuffer imageBuffer(&cellData);
        QImageReader imageReader(&imageBuffer);

        // Display the image format
        QString imageFormat = imageReader.format();

        ui->labelType->setText(tr("Type of data currently in cell: %1 Image").arg(imageFormat.toUpper()));

        // Display the image dimensions and size
        QSize imageDimensions = imageReader.size();
        int imageSize = cellData.size();

        QString labelSizeText = tr("%1x%2 pixel(s)").arg(imageDimensions.width()).arg(imageDimensions.height()) + ", " + humanReadableSize(imageSize);

        ui->labelSize->setText(labelSizeText);

        return;
    }

    // Determine the length of the cell data
    int dataLength = cellData.length();

    // Use a switch statement for the other data types to keep things neat :)
    switch (dataType) {
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

QString EditDialog::humanReadableSize(double byteCount)
{
    QList<QString> units;

    units<<""<<"Ki"<<"Mi"<<"Gi"<<"Ti"<<"Pi"<<"Ei"<<"Zi";

    foreach (QString unit, units)
    {
        if (fabs(byteCount) < 1024.0)
        {
            QString size = QString::number(byteCount, 'f', 2);

            return size + " " + unit + "B";
        }

        byteCount /= 1024.0;
    }

    QString yiUnit = "Yi";
    QString size = QString::number(byteCount, 'f', 2);

    return size + " " + yiUnit + "B";
}

void EditDialog::reloadSettings()
{
    // Set the font for the text and hex editors
    QFont editorFont(Settings::getValue("databrowser", "font").toString());
    editorFont.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
    ui->editorText->setFont(editorFont);
    hexEdit->setFont(editorFont);
}
