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
#include <QtXml/QDomDocument>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QClipboard>
#include <QTextDocument>

#include <Qsci/qsciscintilla.h>

EditDialog::EditDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditDialog),
      currentIndex(QModelIndex()),
      dataSource(SciBuffer),
      dataType(Null),
      isReadOnly(true)
{
    ui->setupUi(this);

    // Add Ctrl-Enter (Cmd-Enter on OSX) as a shortcut for the Apply button
    ui->buttonApply->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));
    ui->buttonApply->setToolTip(ui->buttonApply->toolTip() + " [" + ui->buttonApply->shortcut().toString(QKeySequence::NativeText) + "]");

    QHBoxLayout* hexLayout = new QHBoxLayout(ui->editorBinary);
    hexEdit = new QHexEdit(this);
    hexLayout->addWidget(hexEdit);
    hexEdit->setOverwriteMode(false);

    QHBoxLayout* sciLayout = new QHBoxLayout(ui->editorSci);
    sciEdit = new DockTextEdit(this);
    sciLayout->addWidget(sciEdit);

    QShortcut* ins = new QShortcut(QKeySequence(Qt::Key_Insert), this);
    connect(ins, SIGNAL(activated()), this, SLOT(toggleOverwriteMode()));

    connect(hexEdit, SIGNAL(dataChanged()), this, SLOT(updateApplyButton()));
    connect(sciEdit, SIGNAL(textChanged()), this, SLOT(updateApplyButton()));
    connect(sciEdit, SIGNAL(textChanged()), this, SLOT(editTextChanged()));

    // Create shortcuts for the widgets that doesn't have its own print action or printing mechanism.
    QShortcut* shortcutPrint = new QShortcut(QKeySequence::Print, this, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutPrint, &QShortcut::activated, this, &EditDialog::openPrintDialog);

    // Add actions to editors that have a context menu based on actions. This also activates the shortcuts.
    ui->editorImage->addAction(ui->actionPrintImage);
    ui->editorBinary->addAction(ui->actionPrint);
    ui->editorBinary->addAction(ui->actionCopyHexAscii);

    mustIndentAndCompact = Settings::getValue("databrowser", "indent_compact").toBool();
    ui->actionIndent->setChecked(mustIndentAndCompact);

    ui->buttonAutoSwitchMode->setChecked(Settings::getValue("databrowser", "auto_switch_mode").toBool());
    ui->actionWordWrap->setChecked(Settings::getValue("databrowser", "editor_word_wrap").toBool());
    setWordWrapping(ui->actionWordWrap->isChecked());

    reloadSettings();
}

EditDialog::~EditDialog()
{
    Settings::setValue("databrowser", "indent_compact",  mustIndentAndCompact);
    Settings::setValue("databrowser", "auto_switch_mode", ui->buttonAutoSwitchMode->isChecked());
    Settings::setValue("databrowser", "editor_word_wrap", ui->actionWordWrap->isChecked());
    delete ui;
}

void EditDialog::setCurrentIndex(const QModelIndex& idx)
{
    currentIndex = QPersistentModelIndex(idx);

    QByteArray data = idx.data(Qt::EditRole).toByteArray();
    loadData(data);
    updateCellInfoAndMode(data);

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
        // Set enabled the text widget
        sciEdit->setEnabled(true);
        switch (editMode) {
        case TextEditor:
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
    case XML:
        // Can be stored in any widget, except the ImageViewer

        switch (editMode) {
        case TextEditor:
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
            setDataInBuffer(data, SciBuffer);

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
        case XmlEditor:
        case JsonEditor:
            // Disable text editing, and use a warning message as the contents
            sciEdit->setText(tr("Image data can't be viewed in this mode.") % '\n' %
                             tr("Try switching to Image or Binary mode."));
            sciEdit->setTextInMargin(tr("Image"));
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
        case JsonEditor:
        case XmlEditor:
            // Disable text editing, and use a warning message as the contents
            sciEdit->setText(QString(tr("Binary data can't be viewed in this mode.") % '\n' %
                                     tr("Try switching to Binary mode.")));
            sciEdit->setTextInMargin(Settings::getValue("databrowser", "blob_text").toString());
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
    // Chop last space
    image_formats.chop(1);

    QStringList filters;
    filters << tr("Text files (*.txt)") <<
        tr("JSON files (*.json)") <<
        tr("XML files (*.xml)") <<
        tr("Image files (%1)").arg(image_formats) <<
        tr("Binary files (*.bin)") << tr("All files (*)");

    QString selectedFilter;

    // Get the current editor mode (eg text, hex, image, json or xml mode)
    int mode = ui->comboMode->currentIndex();

    // Set as selected filter the appropriate for the current mode.
    switch (mode) {
    case TextEditor:
        selectedFilter = tr("Text files (*.txt)");
        break;
    case HexEditor:
        selectedFilter = tr("Binary files (*.bin)");
        break;
    case ImageViewer:
        selectedFilter = tr("Image files (%1)").arg(image_formats);
        break;
    case JsonEditor:
        selectedFilter = tr("JSON files (*.json)");
        break;
    case XmlEditor:
        selectedFilter = tr("XML files (*.xml)");
        break;
    }
    QString fileName = FileDialog::getOpenFileName(
                OpenDataFile,
                this,
                tr("Choose a file to import")
#ifndef Q_OS_MAC // Filters on OS X are buggy
                , filters.join(";;")
                , &selectedFilter
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
            // and update mode (if required) to the just imported data type.
            updateCellInfoAndMode(d);
        }
    }
}

void EditDialog::exportData()
{
    QStringList filters;
    switch (dataType) {
    case Image: {
        // Images get special treatment.
        // Determine the likely filename extension.
        QByteArray cellData = hexEdit->data();
        QBuffer imageBuffer(&cellData);
        QImageReader imageReader(&imageBuffer);
        QString imageFormat = imageReader.format();
        filters << tr("%1 Image").arg(imageFormat.toUpper()) % " (*." % imageFormat.toLower() % ")";
        break;
    }
    case Binary:
        filters << tr("Binary files (*.bin)");
        break;
    case Text:
        // Include the XML case on the text data type, since XML detection is not very sofisticated.
        if (ui->comboMode->currentIndex() == XmlEditor)
            filters << tr("XML files (*.xml)") << tr("Text files (*.txt)");
        else
            filters << tr("Text files (*.txt)") << tr("XML files (*.xml)");
        break;
    case JSON:
        filters << tr("JSON files (*.json)");
        break;
    case SVG:
        filters << tr("SVG files (*.svg)");
        break;
    case XML:
        filters << tr("XML files (*.xml)");
        break;
    case Null:
        return;
    }

    if (dataSource == HexBuffer)
        filters << tr("Hex dump files (*.txt)");

    filters << tr("All files (*)");

    QString selectedFilter = filters.first();
    QString fileName = FileDialog::getSaveFileName(
                CreateDataFile,
                this,
                tr("Choose a filename to export data"),
                filters.join(";;"),
                /* defaultFileName */ QString(),
                &selectedFilter);
    if(fileName.size() > 0)
    {
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly))
        {
          switch (dataSource) {
          case HexBuffer:
              // Data source is the hex buffer
              // If text option has been selected, the readable representation of the content is saved to file.
              if (selectedFilter == tr("Hex dump files (*.txt)"))
                  file.write(hexEdit->toReadableString().toUtf8());
              else
                  file.write(hexEdit->data());
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
    ui->editorImage->clear();
    hexEdit->setData(QByteArray());
    sciEdit->clear();
    dataType = Null;
    removedBom.clear();

    // The text editors don't know the difference between an empty string
    // and a NULL, so we need to record NULL outside of that
    dataType = Null;

    // Ensure the text (Scintilla) editor is enabled
    sciEdit->setEnabled(true);

    // Update the cell data info in the bottom left of the Edit Cell
    // The mode is also (if required) updated to text since it gives
    // the better visual clue of containing a NULL value (placeholder).
    updateCellInfoAndMode(hexEdit->data());

    sciEdit->setFocus();
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
    case SciBuffer:
        switch (sciEdit->language()) {
        case DockTextEdit::PlainText:
        {
            QString oldData = currentIndex.data(Qt::EditRole).toString();
            QString newData = removedBom + sciEdit->text();
            // Check first for null case, otherwise empty strings cannot overwrite NULL values
            if ((currentIndex.data(Qt::EditRole).isNull() && dataType != Null) || oldData != newData)
                // The data is different, so commit it back to the database
                emit recordTextUpdated(currentIndex, removedBom + newData.toUtf8(), false);
            break;
        }
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
    case SciBuffer:
        switch (sciEdit->language()) {
        case DockTextEdit::PlainText:
        {
            // Load the text into the text editor, remove BOM first if there is one
            QByteArray dataWithoutBom = data;
            removedBom = removeBom(dataWithoutBom);

            textData = QString::fromUtf8(dataWithoutBom.constData(), dataWithoutBom.size());
            sciEdit->setText(textData);

            // Select all of the text by default (this is useful for simple text data that we usually edit as a whole)
            if (!isReadOnly)
                sciEdit->selectAll();
            sciEdit->setEnabled(true);
            break;
        }
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
    ui->actionIndent->setEnabled(newMode == JsonEditor || newMode == XmlEditor);
    setStackCurrentIndex(newMode);

    // * If the dataSource is the text buffer, the data is always text *
    switch (dataSource) {
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

        case TextEditor: // Switching to the text editor
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
    if (dataSource == SciBuffer) {

        // Update the cell info in the bottom left manually.  This is because
        // updateCellInfoAndMode() only works with QByteArray's (for now)
        int dataLength = sciEdit->text().length();
        bool isModified = sciEdit->isModified();

        // If data has been entered in the text editor, it can't be a NULL
        // any more. It hasn't been validated yet, so it cannot be JSON nor XML.
        if (dataType == Null && isModified && dataLength != 0)
            dataType = Text;

        if (dataType != Null) {
            sciEdit->clearTextInMargin();
            ui->labelType->setText(tr("Type of data currently in cell: Text / Numeric"));
        }
        ui->labelSize->setText(tr("%n character(s)", "", dataLength));
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
        if (cellData.startsWith("<?xml"))
            return XML;
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
        sciEdit->setFocus();
        if (sciEdit->language() == DockTextEdit::PlainText && !isReadOnly)
            sciEdit->selectAll();
        break;
    case HexEditor:
        hexEdit->setFocus();
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

    ui->buttonApply->setEnabled(!ro);
    ui->actionNull->setEnabled(!ro);
    ui->actionImport->setEnabled(!ro);

    sciEdit->setReadOnly(ro);
    hexEdit->setReadOnly(ro);
}

void EditDialog::switchEditorMode(bool autoSwitchForType)
{
    if (autoSwitchForType) {
        // Switch automatically the editing mode according to the detected data.
        switch (dataType) {
        case Image:
            ui->comboMode->setCurrentIndex(ImageViewer);
            break;
        case Binary:
            ui->comboMode->setCurrentIndex(HexEditor);
            break;
        case Null:
        case Text:
            ui->comboMode->setCurrentIndex(TextEditor);
            break;
        case JSON:
            ui->comboMode->setCurrentIndex(JsonEditor);
            break;
        case SVG:
        case XML:
            ui->comboMode->setCurrentIndex(XmlEditor);
            break;
        }
    }
}

// Update the information labels in the bottom left corner of the dialog
// and switches the editor mode, if required, according to the detected data type.
void EditDialog::updateCellInfoAndMode(const QByteArray& data)
{
    QByteArray cellData = data;

    switchEditorMode(ui->buttonAutoSwitchMode->isChecked());

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
    case Null: {
        // NULL data type
        ui->labelType->setText(tr("Type of data currently in cell: NULL"));
        ui->labelSize->setText(tr("%n byte(s)", "", 0));

        // Use margin to set the NULL text.
        sciEdit->setTextInMargin(Settings::getValue("databrowser", "null_text").toString());
        break;
    }
    case XML:
    case Text: {
        // Text only
        // Determine the length of the cell text in characters (possibly different to number of bytes).
        int textLength = QString(cellData).length();
        ui->labelType->setText(tr("Type of data currently in cell: Text / Numeric"));
        ui->labelSize->setText(tr("%n character(s)", "", textLength));
        break;
    }
    case JSON: {
        // Valid JSON
        // Determine the length of the cell text in characters (possibly different to number of bytes).
        int jsonLength = QString(cellData).length();
        ui->labelType->setText(tr("Type of data currently in cell: Valid JSON"));
        ui->labelSize->setText(tr("%n character(s)", "", jsonLength));
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
    // Set the (SQL) editor font for hex editor, since it needs a
    // Monospace font and the databrowser font would be usually of
    // variable width.
    QFont hexFont(Settings::getValue("editor", "font").toString());
    hexFont.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
    hexEdit->setFont(hexFont);

    ui->editCellToolbar->setToolButtonStyle(static_cast<Qt::ToolButtonStyle>
                                            (Settings::getValue("General", "toolbarStyleEditCell").toInt()));

    sciEdit->reloadSettings();
}

void EditDialog::setStackCurrentIndex(int editMode)
{
    switch (editMode) {
    case TextEditor:
        // Scintilla case: switch to the single Scintilla editor and set language
        ui->editorStack->setCurrentIndex(TextEditor);
        sciEdit->setLanguage(DockTextEdit::PlainText);
        break;
    case HexEditor:
    case ImageViewer:
        // General case: switch to the selected editor
        ui->editorStack->setCurrentIndex(editMode);
        break;
    case JsonEditor:
        // Scintilla case: switch to the single Scintilla editor and set language
        ui->editorStack->setCurrentIndex(TextEditor);
        sciEdit->setLanguage(DockTextEdit::JSON);
        break;
    case XmlEditor:
        // Scintilla case: switch to the single Scintilla editor and set language
        ui->editorStack->setCurrentIndex(TextEditor);
        sciEdit->setLanguage(DockTextEdit::XML);
        break;
    }
}

void EditDialog::openPrintDialog()
{
    int editMode = ui->editorStack->currentIndex();
    if (editMode == ImageViewer) {
        openPrintImageDialog();
        return;
    }

    QPrinter printer;
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer);

    QTextDocument *document = new QTextDocument();
    switch (dataSource) {
    case SciBuffer:
        // This case isn't really expected because the Scintilla widget has it's own printing slot
        document->setPlainText(sciEdit->text());
        break;
    case HexBuffer:
        document->setPlainText(hexEdit->toReadableString());
        document->setDefaultFont(hexEdit->font());
        break;
    }

    connect(dialog, &QPrintPreviewDialog::paintRequested, [&](QPrinter *previewPrinter) {
        document->print(previewPrinter);
    });

    dialog->exec();

    delete dialog;
    delete document;
}

void EditDialog::openPrintImageDialog()
{
    QPrinter printer;
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer);

    connect(dialog, &QPrintPreviewDialog::paintRequested, [&](QPrinter *previewPrinter) {
            QPainter painter(previewPrinter);
            QRect rect = painter.viewport();
            QSize size = ui->editorImage->pixmap()->size();
            size.scale(rect.size(), Qt::KeepAspectRatio);
            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
            painter.setWindow(ui->editorImage->pixmap()->rect());
            painter.drawPixmap(0, 0, *ui->editorImage->pixmap());
        });

    dialog->exec();

    delete dialog;
}

void EditDialog::copyHexAscii()
{
    QApplication::clipboard()->setText(hexEdit->selectionToReadableString());
}

void EditDialog::setWordWrapping(bool value)
{
    // Set wrap lines
    sciEdit->setWrapMode(value ? QsciScintilla::WrapWord : QsciScintilla::WrapNone);
}
