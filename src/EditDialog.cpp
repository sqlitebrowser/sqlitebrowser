#include "EditDialog.h"
#include "ui_EditDialog.h"
#include "sqlitedb.h"
#include "Settings.h"
#include "src/qhexedit.h"
#include "docktextedit.h"
#include "FileDialog.h"
#include "Data.h"

#include <QMainWindow>
#include <QKeySequence>
#include <QShortcut>
#include <QImageReader>
#include <QBuffer>
#include <QModelIndex>
#include <QJsonDocument>
#include <QDomDocument>
#include <QMessageBox>

EditDialog::EditDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditDialog),
      currentIndex(QModelIndex()),
      dataSource(TextBuffer),
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

    QHBoxLayout* sciLayout = new QHBoxLayout(ui->editorSci);
    sciEdit = new DockTextEdit(this);
    sciLayout->addWidget(sciEdit);

    QShortcut* ins = new QShortcut(QKeySequence(Qt::Key_Insert), this);
    connect(ins, SIGNAL(activated()), this, SLOT(toggleOverwriteMode()));

    connect(ui->editorText, SIGNAL(textChanged()), this, SLOT(updateApplyButton()));
    connect(hexEdit, SIGNAL(dataChanged()), this, SLOT(updateApplyButton()));

    connect(sciEdit, SIGNAL(textChanged()), this, SLOT(updateApplyButton()));
    connect(sciEdit, SIGNAL(textChanged()), this, SLOT(editTextChanged()));

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

    // Clear previously removed BOM
    removedBom.clear();

    // Determine the data type, saving that info in the class variable
    dataType = checkDataType(data);

    // Get the current editor mode (eg text, hex, image, json or xml mode)
    int editMode = ui->comboMode->currentIndex();

    // Data type specific handling
    switch (dataType) {
    case Null:
        // Set enabled any of the text widgets
        ui->editorText->setEnabled(true);
        sciEdit->setEnabled(true);
        switch (editMode) {
        case TextEditor:
            // The text widget buffer is now the main data source
            dataSource = TextBuffer;

            // Empty the text editor contents, then enable text editing
            ui->editorText->clear();

            break;

        case JsonEditor:
        case XmlEditor:

            // The JSON widget buffer is now the main data source
            dataSource = SciBuffer;

            // Empty the text editor contents, then enable text editing
            sciEdit->clear();

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
        // Can be stored in any widget, except the ImageViewer

        switch (editMode) {
        case TextEditor:
            setDataInBuffer(data, TextBuffer);
            break;
        case JsonEditor:
        case XmlEditor:
            setDataInBuffer(data, SciBuffer);
            break;
        case HexEditor:
            setDataInBuffer(data, HexBuffer);
            break;
        case ImageViewer:
            // The image viewer cannot hold data nor display text.

            // Clear any image from the image viewing widget
            ui->editorImage->setPixmap(QPixmap(0,0));

            // Load the text into the text editor
            setDataInBuffer(data, TextBuffer);

            break;
        }
        break;

    case Image:
        // Image data is kept in the hex widget, mainly for safety.  If we
        // stored it in the editorImage widget instead, it would be a pixmap
        // and there's no good way to restore that back to the original
        // (pristine) image data.  eg image metadata would be lost
        setDataInBuffer(data, HexBuffer);

        // Update the display if in text edit or image viewer mode
        switch (editMode) {
        case TextEditor:
            // Disable text editing, and use a warning message as the contents
            ui->editorText->setText(QString("<i>" %
                    tr("Image data can't be viewed in this mode.") % "<br/>" %
                    tr("Try switching to Image or Binary mode.") %
                    "</i>"));
            ui->editorText->setEnabled(false);
            break;

        case XmlEditor:
        case JsonEditor:
            // Disable text editing, and use a warning message as the contents
            sciEdit->setText(tr("Image data can't be viewed in this mode.") % '\n' %
                             tr("Try switching to Image or Binary mode."));
            sciEdit->setEnabled(false);

            break;

        case ImageViewer:
            // Load the image into the image viewing widget
            if (img.loadFromData(data)) {
                ui->editorImage->setPixmap(QPixmap::fromImage(img));
            }
            break;
        }
        break;
    case SVG:
        // Set the XML data in any buffer or update image in image viewer mode
        switch (editMode) {
        case TextEditor:
            setDataInBuffer(data, TextBuffer);
            break;

        case JsonEditor:
        case XmlEditor:

            setDataInBuffer(data, SciBuffer);
            break;

        case HexEditor:

            setDataInBuffer(data, HexBuffer);
            break;

        case ImageViewer:
            // Set data in the XML (Sci) Buffer and load the SVG Image
            setDataInBuffer(data, SciBuffer);
            sciEdit->setLanguage(DockTextEdit::XML);

            // Load the image into the image viewing widget
            if (img.loadFromData(data)) {
                ui->editorImage->setPixmap(QPixmap::fromImage(img));
            }
            break;
        }
        break;

    default:

        // The data seems to be general binary data, which is always loaded
        // into the hex widget (the only safe place for it)

        // Load the data into the hex buffer
        setDataInBuffer(data, HexBuffer);

        switch (editMode) {
        case TextEditor:
            // Disable text editing, and use a warning message as the contents
            ui->editorText->setText(QString("<i>" %
                    tr("Binary data can't be viewed in this mode.") % "<br/>" %
                    tr("Try switching to Binary mode.") %
                    "</i>"));
            ui->editorText->setEnabled(false);
            break;

         case JsonEditor:
         case XmlEditor:
            // Disable text editing, and use a warning message as the contents
             sciEdit->setText(QString(tr("Binary data can't be viewed in this mode.") % '\n' %
                                      tr("Try switching to Binary mode.")));
             sciEdit->setEnabled(false);
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
                , tr("Text files (*.txt);;Image files (%1);;JSON files (*.json);;XML files (*.xml);;Binary files (*.bin);;All files (*)").arg(image_formats)
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
    switch (dataType) {
    case Image: {
        // Determine the likely filename extension
        QByteArray cellData = hexEdit->data();
        QBuffer imageBuffer(&cellData);
        QImageReader imageReader(&imageBuffer);
        QString imageFormat = imageReader.format();
        fileExt = imageFormat.toUpper() % " " % tr("Image") % "(*." % imageFormat.toLower() % ");;All files(*)";
        break;
    }
    case Binary:
    case Null:
        fileExt = tr("Binary files(*.bin);;All files(*)");
        break;
    case Text:
        fileExt = tr("Text files(*.txt);;All files(*)");
        break;
    case JSON:
        fileExt = tr("JSON files(*.json);;All files(*)");
        break;
    case SVG:
        fileExt = tr("SVG files(*.svg);;All files(*)");
        break;
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
          case SciBuffer:
                // Data source is the Scintilla buffer
                file.write(sciEdit->text().toUtf8());
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
    sciEdit->clear();
    dataType = Null;
    removedBom.clear();

    // The text editors don't know the difference between an empty string
    // and a NULL, so we need to record NULL outside of that
    dataType = Null;

    // Ensure the text (plain and Scintilla) editors are enabled
    ui->editorText->setEnabled(true);
    sciEdit->setEnabled(true);

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

    if (dataType == Null) {
        emit recordTextUpdated(currentIndex, hexEdit->data(), true);
        return;
    }

    switch (dataSource) {
    case TextBuffer:
    {
        QString oldData = currentIndex.data(Qt::EditRole).toString();
        QString newData = removedBom + ui->editorText->toPlainText();
        if (oldData != newData)
            // The data is different, so commit it back to the database
            emit recordTextUpdated(currentIndex, removedBom + newData.toUtf8(), false);
        break;
    }
    case SciBuffer:
        switch (sciEdit->language()) {
        case DockTextEdit::JSON:
        {
            QString oldData = currentIndex.data(Qt::EditRole).toString();

            QString newData;
            QJsonParseError parseError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(sciEdit->text().toUtf8(), &parseError);
            bool proceed;

            sciEdit->clearErrorIndicators();
            if (parseError.error != QJsonParseError::NoError)
                sciEdit->setErrorIndicator(parseError.offset-1);

            if (!jsonDoc.isNull()) {
                if (mustIndentAndCompact)
                    // Compact the JSON data before storing
                    newData = QString(jsonDoc.toJson(QJsonDocument::Compact));
                else
                    newData = sciEdit->text();
                proceed = (oldData != newData);

            } else {
                newData = sciEdit->text();
                proceed = (oldData != newData && promptInvalidData("JSON", parseError.errorString()));
            }
            if (proceed)
                // The data is different, so commit it back to the database
                emit recordTextUpdated(currentIndex, newData.toUtf8(), false);
        }
        break;
        case DockTextEdit::XML:
        {
            QString oldData = currentIndex.data(Qt::EditRole).toString();

            QString newData;
            QDomDocument xmlDoc;
            QString errorMsg;
            int errorLine, errorColumn;

            bool isValid = xmlDoc.setContent(sciEdit->text().toUtf8(), true, &errorMsg, &errorLine, &errorColumn);
            bool proceed;

            sciEdit->clearErrorIndicators();
            if (!isValid) {
                sciEdit->setErrorIndicator(errorLine-1, errorColumn-1, errorLine, 0);
                newData = sciEdit->text();
                proceed = (oldData != newData && promptInvalidData("XML", errorMsg));
            } else {
                if (mustIndentAndCompact)
                    // Compact the XML data before storing. If indent is -1, no whitespace at all is added.
                    newData = xmlDoc.toString(-1);
                else
                    newData = sciEdit->text();
                proceed = (oldData != newData);
            }
            if (proceed)
                // The data is different, so commit it back to the database
                emit recordTextUpdated(currentIndex, newData.toUtf8(), false);
        }
        break;
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

void EditDialog::setDataInBuffer(const QByteArray& data, DataSources source)
{
    dataSource = source;
    QString textData;

    // 1) Perform validation and text formatting (if applicable).
    // 2) Set the text in the corresponding editor widget (the text widget for the Image case).
    // 3) Enable the widget.
    switch (dataSource) {
    case TextBuffer:
    {
        // Load the text into the text editor, remove BOM first if there is one
        QByteArray dataWithoutBom = data;
        removedBom = removeBom(dataWithoutBom);

        textData = QString::fromUtf8(dataWithoutBom.constData(), dataWithoutBom.size());
        ui->editorText->setPlainText(textData);

        // Select all of the text by default (this is useful for simple text data that we usually edit as a whole)
        ui->editorText->selectAll();
        ui->editorText->setEnabled(true);
        break;
    }
    case SciBuffer:
        switch (sciEdit->language()) {
        case DockTextEdit::JSON:
        {
            QJsonParseError parseError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(data.constData(), data.size()), &parseError);

            if (mustIndentAndCompact && !jsonDoc.isNull()) {
                // Load indented JSON into the JSON editor
                textData = QString(jsonDoc.toJson(QJsonDocument::Indented));
            } else {
                // Fallback case. The data is not yet valid JSON or no auto-formatting applied.
                textData = QString::fromUtf8(data.constData(), data.size());
            }
            sciEdit->setText(textData);

            sciEdit->clearErrorIndicators();
            if (parseError.error != QJsonParseError::NoError)
                sciEdit->setErrorIndicator(parseError.offset-1);
            sciEdit->setEnabled(true);

        }

        break;

        case DockTextEdit::XML:
        {
            QString errorMsg;
            int errorLine, errorColumn;
            QDomDocument xmlDoc;
            bool isValid = xmlDoc.setContent(data, true, &errorMsg, &errorLine, &errorColumn);

            if (mustIndentAndCompact && isValid) {
                // Load indented XML into the XML editor
                textData = xmlDoc.toString(Settings::getValue("editor", "tabsize").toInt());
            } else {
                // Fallback case. The data is not yet valid JSON or no auto-formatting applied.
                textData = QString::fromUtf8(data.constData(), data.size());
            }
            sciEdit->setText(textData);

            sciEdit->clearErrorIndicators();
            if (!isValid)
                // Adjust line and column by one (Scintilla starts at 1 and QDomDocument at 0)
                sciEdit->setErrorIndicator(errorLine-1, errorColumn-1, errorLine, 0);
            sciEdit->setEnabled(true);

        }
        break;
        }
        break;
    case HexBuffer:
        hexEdit->setData(data);
        hexEdit->setEnabled(true);

        break;
    }

}

// Called when the user manually changes the "Mode" drop down combobox
void EditDialog::editModeChanged(int newMode)
{
    ui->buttonIndent->setEnabled(newMode == JsonEditor || newMode == XmlEditor);
    setStackCurrentIndex(newMode);

    // * If the dataSource is the text buffer, the data is always text *
    switch (dataSource) {
    case TextBuffer:
        switch (newMode) {
        case TextEditor: // Switching to the text editor
            // Nothing to do, as the text is already in the text buffer
            break;

        case JsonEditor: // Switching to one of the Scintilla editor modes
        case XmlEditor:

            setDataInBuffer(ui->editorText->toPlainText().toUtf8(), SciBuffer);
            break;

        case HexEditor: // Switching to the hex editor
            // Convert the text widget buffer for the hex widget
            // The hex widget buffer is now the main data source
            setDataInBuffer(removedBom + ui->editorText->toPlainText().toUtf8(), HexBuffer);
            break;

        case ImageViewer:
            // Clear any image from the image viewing widget
            ui->editorImage->setPixmap(QPixmap(0,0));
            break;
        }
        break;
    case HexBuffer:

        // * If the dataSource is the hex buffer, the contents could be anything
        //   so we just pass it to our loadData() function to handle *
        // Note that we have already set the editor, as loadData() relies on it
        // being current

        // Load the data into the appropriate widget, as done by loadData()
        loadData(hexEdit->data());
        break;
    case SciBuffer:
        switch (newMode) {
        case TextEditor: // Switching to the text editor
            // Convert the text widget buffer for the JSON widget
            setDataInBuffer(sciEdit->text().toUtf8(), TextBuffer);
            break;

        case HexEditor: // Switching to the hex editor
            // Convert the text widget buffer for the hex widget
            setDataInBuffer(sciEdit->text().toUtf8(), HexBuffer);
            break;
        case ImageViewer:
        {
            // When SVG format, load the image, else clear it.
            QByteArray data = sciEdit->text().toUtf8();
            dataType = checkDataType(data);
            if (dataType == SVG) {
                QImage img;

                if (img.loadFromData(data))
                    ui->editorImage->setPixmap(QPixmap::fromImage(img));
                else
                    // Clear any image from the image viewing widget
                    ui->editorImage->setPixmap(QPixmap(0,0));
            }
        }
        break;

        case JsonEditor: // Switching to the JSON editor
        case XmlEditor: // Switching to the XML editor
            // The text is already in the Sci buffer but we need to perform the necessary formatting.
            setDataInBuffer(sciEdit->text().toUtf8(), SciBuffer);

            break;
        }
    }
}

// Called for every keystroke in the text editor (only)
void EditDialog::editTextChanged()
{
    if (dataSource == TextBuffer || dataSource == SciBuffer) {
        // Data has been changed in the text editor, so it can't be a NULL
        // any more. It hasn't been validated yet, so it cannot be JSON nor XML.
        if (dataType == Null) {
            dataType = Text;
            ui->labelType->setText(tr("Type of data currently in cell: Text / Numeric"));
        }
        // Update the cell info in the bottom left manually.  This is because
        // updateCellInfo() only works with QByteArray's (for now)
        int dataLength;
        switch (dataSource) {
        case TextBuffer:
            dataLength = ui->editorText->toPlainText().length();
            break;
        case SciBuffer:
            dataLength = sciEdit->text().length();
            break;
        }
        ui->labelSize->setText(tr("%n char(s)", "", dataLength));
    }
}

void EditDialog::setMustIndentAndCompact(bool enable)
{
    mustIndentAndCompact = enable;

    // Indent or compact if necessary. If data has changed (button Apply indicates so), reload from the widget, else from the table.
    if (ui->buttonApply->isEnabled()) {
        setDataInBuffer(sciEdit->text().toUtf8(), SciBuffer);
    } else
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
    QString imageFormat = readerBuffer.format();
    if(readerBuffer.canRead() && !readerBuffer.read().isNull())
        return imageFormat == "svg" ? SVG : Image;

    // Check if it's text only
    if(isTextOnly(cellData))
    {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(cellData);
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
    sciEdit->setOverwriteMode(currentMode);
}

void EditDialog::setFocus()
{
    QDialog::setFocus();

    // Set the focus to the editor widget. The idea here is that setting focus
    // to the dock itself doesn't make much sense as it's just a frame; you'd
    // have to tab to the editor which is what you most likely want to use. So
    // in order to save the user from doing this we explicitly set the focus
    // to the current editor.
    int editMode = ui->editorStack->currentIndex();

    switch (editMode) {
    case TextEditor:
        ui->editorText->setFocus();
        ui->editorText->selectAll();
        break;
    case HexEditor:
        hexEdit->setFocus();
        break;
    case SciEditor:
        sciEdit->setFocus();
        break;
    case ImageViewer:
        // Nothing to do
        break;
    }

}

// Enables or disables the Apply, Null, & Import buttons in the Edit Cell dock.
// Sets or unsets read-only properties for the editors.
void EditDialog::setReadOnly(bool ro)
{
    isReadOnly = ro;
    QPalette textEditPalette = ui->editorText->palette();

    ui->buttonApply->setEnabled(!ro);
    ui->buttonNull->setEnabled(!ro);
    ui->buttonImport->setEnabled(!ro);

    ui->editorText->setReadOnly(ro);
    sciEdit->setReadOnly(ro);
    hexEdit->setReadOnly(ro);

    // This makes the caret being visible for selection, although the editor is read-only.
    Qt::TextInteractionFlags textFlags = ro? Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard : Qt::TextEditorInteraction;
    ui->editorText->setTextInteractionFlags(textFlags);

    // If read-only, set the Disabled palette settings for the (in)active groups, so the user gets a hint about the text being read-only.
    // This should be set also for the Scintilla widget, but it isn't working for that.
    if (ro) {
        textEditPalette.setColor(QPalette::Active, QPalette::Base, textEditPalette.color(QPalette::Disabled, QPalette::Base));
        textEditPalette.setColor(QPalette::Inactive, QPalette::Base, textEditPalette.color(QPalette::Disabled, QPalette::Base));
        textEditPalette.setColor(QPalette::Active, QPalette::Highlight, textEditPalette.color(QPalette::Disabled, QPalette::Highlight));
        textEditPalette.setColor(QPalette::Inactive, QPalette::Highlight, textEditPalette.color(QPalette::Disabled, QPalette::Highlight));
        textEditPalette.setColor(QPalette::Active, QPalette::HighlightedText, textEditPalette.color(QPalette::Disabled, QPalette::HighlightedText));
        textEditPalette.setColor(QPalette::Inactive, QPalette::HighlightedText, textEditPalette.color(QPalette::Disabled, QPalette::HighlightedText));
        ui->editorText->setPalette(textEditPalette);
    } else {
        // Restore default palette
        ui->editorText->setPalette(QPalette());
    }
}

// Update the information labels in the bottom left corner of the dialog
void EditDialog::updateCellInfo(const QByteArray& data)
{
    QByteArray cellData = data;

    // Image data needs special treatment
    if (dataType == Image || dataType == SVG) {
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

    sciEdit->reloadSettings();
}

void EditDialog::setStackCurrentIndex(int editMode)
{
    switch (editMode) {
    case TextEditor:
    case HexEditor:
    case ImageViewer:
        // General case: switch to the selected editor
        ui->editorStack->setCurrentIndex(editMode);
        break;
    case JsonEditor:
        // Scintilla case: switch to the single Scintilla editor and set language
        ui->editorStack->setCurrentIndex(SciEditor);
        sciEdit->setLanguage(DockTextEdit::JSON);
        break;
    case XmlEditor:
        // Scintilla case: switch to the single Scintilla editor and set language
        ui->editorStack->setCurrentIndex(SciEditor);
        sciEdit->setLanguage(DockTextEdit::XML);
        break;
    }
}
