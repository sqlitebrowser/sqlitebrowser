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
#include <QJsonDocument>
#include <QMessageBox>

EditDialog::EditDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditDialog),
      currentIndex(QModelIndex()),
      dataSource(TextBuffer),
      dataType(Null),
      textNullSet(false),
      isReadOnly(true)
{
    ui->setupUi(this);

    // Add Ctrl-Enter (Cmd-Enter on OSX) as a shortcut for the Apply button
    ui->buttonApply->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));

    QHBoxLayout* hexLayout = new QHBoxLayout(ui->editorBinary);
    hexEdit = new QHexEdit(this);
    hexLayout->addWidget(hexEdit);
    hexEdit->setOverwriteMode(false);

    QHBoxLayout* jsonLayout = new QHBoxLayout(ui->editorJSON);
    jsonEdit = new JsonTextEdit(this);
    jsonLayout->addWidget(jsonEdit);

    QShortcut* ins = new QShortcut(QKeySequence(Qt::Key_Insert), this);
    connect(ins, SIGNAL(activated()), this, SLOT(toggleOverwriteMode()));

    connect(ui->editorText, SIGNAL(textChanged()), this, SLOT(updateApplyButton()));
    connect(hexEdit, SIGNAL(dataChanged()), this, SLOT(updateApplyButton()));
    connect(jsonEdit, SIGNAL(textChanged()), this, SLOT(updateApplyButton()));
    connect(jsonEdit, SIGNAL(textChanged()), this, SLOT(editTextChanged()));

    mustIndentAndCompact = Settings::getValue("databrowser", "indent_compact").toBool();
    ui->buttonIndent->setChecked(mustIndentAndCompact);

    reloadSettings();
}

EditDialog::~EditDialog()
{
    Settings::setValue("databrowser", "indent_compact",  mustIndentAndCompact);
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
        // Set enabled any of the text widgets
        ui->editorText->setEnabled(true);
        jsonEdit->setEnabled(true);
        switch (editMode) {
        case TextEditor:
            // The text widget buffer is now the main data source
            dataSource = TextBuffer;

            // Empty the text editor contents, then enable text editing
            ui->editorText->clear();

            break;

        case JsonEditor:
            // The JSON widget buffer is now the main data source
            dataSource = JsonBuffer;

            // Empty the text editor contents, then enable text editing
            jsonEdit->clear();

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
    case JSON:

        // Set enabled any of the text widgets
        ui->editorText->setEnabled(true);
        jsonEdit->setEnabled(true);

        switch (editMode) {
        case TextEditor:
            // The text widget buffer is now the main data source
            dataSource = TextBuffer;

            // Load the text into the text editor
            textData = QString::fromUtf8(data.constData(), data.size());
            ui->editorText->setPlainText(textData);

            // Select all of the text by default
            ui->editorText->selectAll();

            break;

        case JsonEditor:
            // The JSON widget buffer is now the main data source
            dataSource = JsonBuffer;
            {
                QJsonParseError parseError;
                QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(data.constData(), data.size()), &parseError);

                if (mustIndentAndCompact && !jsonDoc.isNull()) {
                    // Load indented JSON into the JSON editor
                    textData = QString(jsonDoc.toJson(QJsonDocument::Indented));
                    jsonEdit->setText(textData);
                } else {
                    // Fallback case. The data is not yet valid JSON.
                    textData = QString::fromUtf8(data.constData(), data.size());
                    jsonEdit->setText(textData);
                }

                jsonEdit->clearErrorIndicators();
                if (parseError.error != QJsonParseError::NoError)
                    jsonEdit->setErrorIndicator(parseError.offset-1);

            }

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

        case JsonEditor:
            // Disable text editing, and use a warning message as the contents
            jsonEdit->setText(tr("Image data can't be viewed with the JSON editor"));
            jsonEdit->setEnabled(false);
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

         case JsonEditor:
            // Disable text editing, and use a warning message as the contents
            jsonEdit->setText(QString(tr("Binary data can't be viewed with the JSON editor")));
            jsonEdit->setEnabled(false);
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
                tr("Choose a file to import")
#ifndef Q_OS_MAC // Filters on OS X are buggy
                , tr("Text files(*.txt);;Image files(%1);;JSON files(*.json);;All files(*)").arg(image_formats)
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
          switch (dataSource) {
          case HexBuffer:
                // Data source is the hex buffer
                file.write(hexEdit->data());
                break;
          case TextBuffer:
                // Data source is the text buffer
                file.write(ui->editorText->toPlainText().toUtf8());
                break;
          case JsonBuffer:
                // Data source is the JSON buffer
                file.write(jsonEdit->text().toUtf8());
                break;

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
    jsonEdit->clear();
    dataType = Null;

    // Check if in text editor mode
    int editMode = ui->editorStack->currentIndex();
    if (editMode == TextEditor || editMode == JsonEditor) {
        // Setting NULL in the text editor switches the data source to it
        dataSource = TextBuffer;

        // Ensure the text editor is enabled
        ui->editorText->setEnabled(true);
        // Ensure the JSON editor is enabled
        jsonEdit->setEnabled(true);

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

bool EditDialog::promptInvalidData(const QString& dataType, const QString& errorString)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
      this,
      tr("Invalid data for this mode"),
      tr("The cell contains invalid %1 data. Reason: %2. Do you really want to apply it to the cell?").arg(dataType, errorString),
      QMessageBox::Apply | QMessageBox::Cancel);
    return (reply == QMessageBox::Apply);
}

void EditDialog::accept()
{
    if(!currentIndex.isValid())
        return;

    switch (dataSource) {
    case TextBuffer:
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
        break;
    case JsonBuffer:
        // Check if a NULL is set in the text editor
        if (textNullSet) {
            emit recordTextUpdated(currentIndex, hexEdit->data(), true);
        } else {
            // It's not NULL, so proceed with normal text string checking
            QString oldData = currentIndex.data(Qt::EditRole).toString();

            QString newData;
            QJsonParseError parseError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonEdit->text().toUtf8(), &parseError);
            bool proceed;

            jsonEdit->clearErrorIndicators();
            if (parseError.error != QJsonParseError::NoError)
                jsonEdit->setErrorIndicator(parseError.offset-1);

            if (!jsonDoc.isNull()) {
                if (mustIndentAndCompact)
                    // Compact the JSON data before storing
                    newData = QString(jsonDoc.toJson(QJsonDocument::Compact));
                else
                    newData = jsonEdit->text();
                proceed = (oldData != newData);

            } else {
                newData = jsonEdit->text();
                proceed = (oldData != newData && promptInvalidData("JSON", parseError.errorString()));
            }
            if (proceed)
                // The data is different, so commit it back to the database
                emit recordTextUpdated(currentIndex, newData.toUtf8(), false);
        }
        break;

    case HexBuffer:
        // The data source is the hex widget buffer, thus binary data
        QByteArray oldData = currentIndex.data(Qt::EditRole).toByteArray();
        QByteArray newData = hexEdit->data();
        if (newData != oldData)
            emit recordTextUpdated(currentIndex, newData, true);
        break;
    }
}

// Called when the user manually changes the "Mode" drop down combobox
void EditDialog::editModeChanged(int newMode)
{
    ui->buttonIndent->setEnabled(newMode == JsonEditor);

    // * If the dataSource is the text buffer, the data is always text *
    switch (dataSource) {
    case TextBuffer:
        switch (newMode) {
        case TextEditor: // Switching to the text editor
            // Nothing to do, as the text is already in the text buffer
            break;

        case JsonEditor: // Switching to the JSON editor

            // Convert the text widget buffer for the JSON widget
            // * If the dataSource is the TextBuffer, the contents could
            // be still compacted so we just pass it to our loadData()
            // function to handle, for indenting if necessary *
            // Switch to the selected editor first, as loadData() relies
            // on it being current
            ui->editorStack->setCurrentIndex(newMode);

            // Load the data into the appropriate widget, as done by loadData()
            loadData(ui->editorText->toPlainText().toUtf8());
            // jsonEdit->setText(ui->editorText->toPlainText().toUtf8());

            // The JSON widget buffer is now the main data source
            dataSource = JsonBuffer;
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
        break;
    case HexBuffer:

        // * If the dataSource is the hex buffer, the contents could be anything
        //   so we just pass it to our loadData() function to handle *
        // Switch to the selected editor first, as loadData() relies on it
        // being current
        ui->editorStack->setCurrentIndex(newMode);

        // Load the data into the appropriate widget, as done by loadData()
        loadData(hexEdit->data());
        break;
    case JsonBuffer:
        switch (newMode) {
        case TextEditor: // Switching to the text editor
            // Convert the text widget buffer for the JSON widget
            ui->editorText->setText(jsonEdit->text());

            // The Text widget buffer is now the main data source
            dataSource = TextBuffer;
            break;

        case JsonEditor: // Switching to the JSON editor
             // Nothing to do, as the text is already in the JSON buffer
            break;


        case HexEditor: // Switching to the hex editor
            // Convert the text widget buffer for the hex widget
            hexEdit->setData(jsonEdit->text().toUtf8());

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
        }
}

// Called for every keystroke in the text editor (only)
void EditDialog::editTextChanged()
{
    if (dataSource == TextBuffer || dataSource == JsonBuffer) {
        // Data has been changed in the text editor, so it can't be a NULL
        // any more
        textNullSet = false;

        // Update the cell info in the bottom left manually.  This is because
        // updateCellInfo() only works with QByteArray's (for now)
        int dataLength;
        if (dataSource == TextBuffer)
            dataLength = ui->editorText->toPlainText().length();
        else
            dataLength = jsonEdit->text().length();
        ui->labelType->setText(tr("Type of data currently in cell: Text / Numeric"));
        ui->labelSize->setText(tr("%n char(s)", "", dataLength));
    }
}

void EditDialog::setMustIndentAndCompact(bool enable)
{
    mustIndentAndCompact = enable;

    // Indent or compact if necessary. If data has changed, reload from the widget, else from the table.
    if (ui->buttonApply->isEnabled())
        loadData(jsonEdit->text().toUtf8());
    else
        setCurrentIndex(currentIndex);

}

// Determine the type of data in the cell
int EditDialog::checkDataType(const QByteArray& data)
{
    QByteArray cellData = data;

    // Check for NULL data type
    if (cellData.isNull()) {
        return Null;
    }

    // Check if it's an image. First do a quick test by calling canRead() which only checks the first couple of bytes or so. Only if
    // that returned true, do a more sophisticated test of the data. This way we get both, good performance and proper data checking.
    QBuffer imageBuffer(&cellData);
    QImageReader readerBuffer(&imageBuffer);
    if(readerBuffer.canRead() && !readerBuffer.read().isNull())
        return Image;

    // Check if it's text only
    if (QString(cellData).toUtf8() == cellData) { // Is there a better way to check this?

        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString(cellData).toUtf8());
        if (!jsonDoc.isNull())
            return JSON;
        else
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
    jsonEdit->setOverwriteMode(currentMode);
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
    Qt::TextInteractionFlags textFlags = ro? Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard : Qt::TextEditorInteraction;
    ui->editorText->setTextInteractionFlags(textFlags);

    ui->editorBinary->setEnabled(!ro);  // We disable the entire hex editor here instead of setting it to read only because it doesn't have a setReadOnly() method
    jsonEdit->setReadOnly(ro);
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

    // Use a switch statement for the other data types to keep things neat :)
    switch (dataType) {
    case Null:
        // NULL data type
        ui->labelType->setText(tr("Type of data currently in cell: NULL"));
        ui->labelSize->setText(tr("%n byte(s)", "", 0));
        break;

    case Text: {
        // Text only
        // Determine the length of the cell text in characters (possibly different to number of bytes).
        int textLength = QString(cellData).length();
        ui->labelType->setText(tr("Type of data currently in cell: Text / Numeric"));
        ui->labelSize->setText(tr("%n char(s)", "", textLength));
        break;
    }
    case JSON: {
        // Valid JSON
        // Determine the length of the cell text in characters (possibly different to number of bytes).
        int jsonLength = QString(cellData).length();
        ui->labelType->setText(tr("Type of data currently in cell: Valid JSON"));
        ui->labelSize->setText(tr("%n char(s)", "", jsonLength));
        break;
    }
    default:

        // Determine the length of the cell data
        int dataLength = cellData.length();
        // If none of the above data types, consider it general binary data
        ui->labelType->setText(tr("Type of data currently in cell: Binary"));
        ui->labelSize->setText(tr("%n byte(s)", "", dataLength));
        break;
    }
}

QString EditDialog::humanReadableSize(double byteCount) const
{
    QStringList units;
    units << "" << "Ki" << "Mi" << "Gi" << "Ti" << "Pi" << "Ei" << "Zi";

    for(const QString& unit : units)
    {
        if(fabs(byteCount) < 1024.0)
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
    // Set the databrowser font for the text editor but the (SQL) editor
    // font for hex editor, since it needs a Monospace font and the
    // databrowser font would be usually of variable width.
    QFont textFont(Settings::getValue("databrowser", "font").toString());
    textFont.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
    ui->editorText->setFont(textFont);

    QFont hexFont(Settings::getValue("editor", "font").toString());
    hexFont.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
    hexEdit->setFont(hexFont);

    jsonEdit->reloadSettings();

}
