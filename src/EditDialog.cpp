#include "EditDialog.h"
#include "ui_EditDialog.h"
#include "Settings.h"
#include "qhexedit.h"
#include "docktextedit.h"
#include "FileDialog.h"
#include "Data.h"
#include "ImageViewer.h"

#include <QMainWindow>
#include <QKeySequence>
#include <QShortcut>
#include <QImageReader>
#include <QModelIndex>
#include <QtXml/QDomDocument>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QClipboard>
#include <QTextDocument>
#include <QMenu>

#include <Qsci/qsciscintilla.h>
#include <json.hpp>

using json = nlohmann::json;

EditDialog::EditDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditDialog),
      m_currentIndex(QModelIndex()),
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

    QHBoxLayout* imageLayout = new QHBoxLayout(ui->editorImage);
    imageEdit = new ImageViewer(this);
    imageLayout->addWidget(imageEdit);

    QShortcut* ins = new QShortcut(QKeySequence(Qt::Key_Insert), this);
    connect(ins, &QShortcut::activated, this, &EditDialog::toggleOverwriteMode);

    connect(sciEdit, &DockTextEdit::textChanged, this, &EditDialog::updateApplyButton);
    connect(sciEdit, &DockTextEdit::textChanged, this, &EditDialog::editTextChanged);
    connect(ui->qtEdit, &QTextEdit::textChanged, this, &EditDialog::updateApplyButton);
    connect(hexEdit, &QHexEdit::dataChanged, this, &EditDialog::updateApplyButton);

    // Create shortcuts for the widgets that doesn't have its own print action or printing mechanism.
    QShortcut* shortcutPrint = new QShortcut(QKeySequence::Print, this, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutPrint, &QShortcut::activated, this, &EditDialog::openPrintDialog);

    // Add actions to editors that have a context menu based on actions. This also activates the shortcuts.
    ui->editorBinary->addAction(ui->actionPrint);
    ui->editorBinary->addAction(ui->actionCopyHexAscii);

    // Set up popup menus
    QMenu* popupImportFileMenu = new QMenu(this);
    popupImportFileMenu->addAction(ui->actionImportInMenu);
    popupImportFileMenu->addAction(ui->actionImportAsLink);
    ui->actionImport->setMenu(popupImportFileMenu);

    connect(ui->actionImportAsLink, &QAction::triggered, this, [&]() {
        importData(/* asLink */ true);
    });

    connect(ui->actionOpenInApp, &QAction::triggered, this, [&]() {
        switch (dataSource) {
        case SciBuffer:
            emit requestUrlOrFileOpen(sciEdit->text());
            break;
        case QtBuffer:
            emit requestUrlOrFileOpen(ui->qtEdit->toPlainText());
            break;
        default:
            return;
        }
    });
    connect(ui->actionOpenInExternal, &QAction::triggered, this, &EditDialog::openDataWithExternal);

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

void EditDialog::promptSaveData()
{
    if (m_currentIndex.isValid() && isModified()) {

        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            tr("Unsaved data in the cell editor"),
            tr("The cell editor contains data not yet applied to the database.\n"
               "Do you want to apply the edited data to row=%1, column=%2?")
            .arg(m_currentIndex.row() + 1).arg(m_currentIndex.column()),
            QMessageBox::Apply | QMessageBox::Discard, QMessageBox::Apply);

        if (reply == QMessageBox::Apply)
            accept();
        else
            reject();
    }
}

void EditDialog::setCurrentIndex(const QModelIndex& idx)
{
    if (m_currentIndex != QPersistentModelIndex(idx)) {
        promptSaveData();
    }

    setDisabled(!idx.isValid());

    m_currentIndex = QPersistentModelIndex(idx);

    QByteArray bArrData = idx.data(Qt::EditRole).toByteArray();
    loadData(bArrData);
    if (idx.isValid()) {
        ui->labelCell->setText(tr("Editing row=%1, column=%2")
                               .arg(m_currentIndex.row() + 1).arg(m_currentIndex.column()));
    } else {
        ui->labelCell->setText(tr("No cell active."));
    }
    updateCellInfoAndMode(bArrData);

    setModified(false);
}

bool EditDialog::isModified() const
{
    return (sciEdit->isModified() || ui->qtEdit->document()->isModified());
}

void EditDialog::setModified(bool modified)
{
    ui->buttonApply->setEnabled(modified);
    sciEdit->setModified(modified);
    ui->qtEdit->document()->setModified(modified);
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
    setCurrentIndex(m_currentIndex);
}

// Loads data from a cell into the Edit Cell window
void EditDialog::loadData(const QByteArray& bArrdata)
{
    QImage img;

    // Clear previously removed BOM
    removedBom.clear();

    // Determine the data type, saving that info in the class variable
    dataType = checkDataType(bArrdata);

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
        case SqlEvaluator:

            // The JSON widget buffer is now the main data source
            dataSource = SciBuffer;

            // Empty the text editor contents, then enable text editing
            sciEdit->clear();

            break;

        case RtlTextEditor:
            // The text widget buffer is now the main data source
            dataSource = QtBuffer;

            // Empty the Qt text editor contents, then enable text editing
            ui->qtEdit->clear();

            break;

        case HexEditor:
            // The hex widget buffer is now the main data source
            dataSource = HexBuffer;

            // Load the Null into the hex editor
            hexEdit->setData(bArrdata);

            break;

        case ImageEditor:
            // The hex widget buffer is now the main data source
            dataSource = HexBuffer;

            // Clear any image from the image viewing widget
            imageEdit->resetImage();

            // Load the Null into the hex editor
            hexEdit->setData(bArrdata);

            break;
        }
        break;

    case Text:
    case RtlText:
    case JSON:
    case XML:
        // Can be stored in any widget, except the ImageViewer

        sciEdit->clearTextInMargin();

        switch (editMode) {
        case TextEditor:
        case JsonEditor:
        case XmlEditor:
        case SqlEvaluator:
            setDataInBuffer(bArrdata, SciBuffer);
            break;
         case RtlTextEditor:
             setDataInBuffer(bArrdata, QtBuffer);
            break;
        case HexEditor:
            setDataInBuffer(bArrdata, HexBuffer);
            break;
        case ImageEditor:
            // The image viewer cannot hold data nor display text.

            // Clear any image from the image viewing widget
            imageEdit->resetImage();

            // Load the text into the text editor
            setDataInBuffer(bArrdata, SciBuffer);

            break;
       }
        break;

    case Image:
        // Image data is kept in the hex widget, mainly for safety.  If we
        // stored it in the editorImage widget instead, it would be a pixmap
        // and there's no good way to restore that back to the original
        // (pristine) image data.  eg image metadata would be lost
        setDataInBuffer(bArrdata, HexBuffer);

        // Update the display if in text edit or image viewer mode
        switch (editMode) {
        case TextEditor:
        case JsonEditor:
        case XmlEditor:
        case SqlEvaluator:
            // Disable text editing, and use a warning message as the contents
            sciEdit->setText(tr("Image data can't be viewed in this mode.") % '\n' %
                             tr("Try switching to Image or Binary mode."));
            sciEdit->setTextInMargin(tr("Image"));
            sciEdit->setEnabled(false);
            break;

        case RtlTextEditor:
                    // Disable text editing, and use a warning message as the contents
            ui->qtEdit->setText(QString("<i>" %
                    tr("Image data can't be viewed in this mode.") % "<br/>" %
                    tr("Try switching to Image or Binary mode.") %
                    "</i>"));
            ui->qtEdit->setEnabled(false);
            break;

        case ImageEditor:
            // Load the image into the image viewing widget
            if (img.loadFromData(bArrdata)) {
                imageEdit->setImage(img);
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
        case SqlEvaluator:
            setDataInBuffer(bArrdata, SciBuffer);
            break;

        case HexEditor:

            setDataInBuffer(bArrdata, HexBuffer);
            break;

        case ImageEditor:
            // Set data in the XML (Sci) Buffer and load the SVG Image
            setDataInBuffer(bArrdata, SciBuffer);
            sciEdit->setLanguage(DockTextEdit::XML);

            // Load the image into the image viewing widget
            if (img.loadFromData(bArrdata)) {
                imageEdit->setImage(img);
            }
            break;
        case RtlTextEditor:
            setDataInBuffer(bArrdata, QtBuffer);
            break;
        }
        break;

    default:

        // The data seems to be general binary data, which is always loaded
        // into the hex widget (the only safe place for it)

        // Load the data into the hex buffer
        setDataInBuffer(bArrdata, HexBuffer);

        switch (editMode) {
        case TextEditor:
        case JsonEditor:
        case XmlEditor:
        case SqlEvaluator:
            // Disable text editing, and use a warning message as the contents
            sciEdit->setText(QString(tr("Binary data can't be viewed in this mode.") % '\n' %
                                     tr("Try switching to Binary mode.")));
            sciEdit->setTextInMargin(Settings::getValue("databrowser", "blob_text").toString());
            sciEdit->setEnabled(false);
            break;

        case RtlTextEditor:
            // Disable text editing, and use a warning message as the contents
            ui->qtEdit->setText(QString("<i>" %
                    tr("Binary data can't be viewed in this mode.") % "<br/>" %
                    tr("Try switching to Binary mode.") %
                    "</i>"));
            ui->qtEdit->setEnabled(false);
            break;

        case ImageEditor:
            // Clear any image from the image viewing widget
            imageEdit->resetImage();
            break;
        }
    }
}

void EditDialog::importData(bool asLink)
{
    // Get list of supported image file formats to include them in the file dialog filter
    QString image_formats;
    QList<QByteArray> image_formats_list = QImageReader::supportedImageFormats();
    for(int i=0;i<image_formats_list.size();++i)
        image_formats.append(QString("*.%1 ").arg(QString::fromUtf8(image_formats_list.at(i))));
    // Chop last space
    image_formats.chop(1);

    QStringList filters;
    filters << FILE_FILTER_TXT
            << FILE_FILTER_JSON
            << FILE_FILTER_XML
            << tr("Image files (%1)").arg(image_formats)
            << FILE_FILTER_BIN
            << FILE_FILTER_ALL;

    QString selectedFilter;

    // Get the current editor mode (eg text, hex, image, json or xml mode)
    int mode = ui->comboMode->currentIndex();

    // Set as selected filter the appropriate for the current mode.
    switch (mode) {
    case TextEditor:
    case RtlTextEditor:
    case SqlEvaluator:
        selectedFilter = FILE_FILTER_TXT;
        break;
    case HexEditor:
        selectedFilter = FILE_FILTER_BIN;
        break;
    case ImageEditor:
        selectedFilter = tr("Image files (%1)").arg(image_formats);
        break;
    case JsonEditor:
        selectedFilter = FILE_FILTER_JSON;
        break;
    case XmlEditor:
        selectedFilter = FILE_FILTER_XML;
        break;
    }
    QString fileName = FileDialog::getOpenFileName(
                OpenDataFile,
                this,
                tr("Choose a file to import")
#ifndef Q_OS_MAC // Filters on macOS are buggy
                , filters.join(";;")
                , &selectedFilter
#endif
                );
    if(QFile::exists(fileName))
    {
        if(asLink) {
            QByteArray fileNameBa = fileName.toUtf8();
            loadData(fileNameBa);
            updateCellInfoAndMode(fileNameBa);
        } else {
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
    case RtlText:
    case Text:
        // Include the XML case on the text data type, since XML detection is not very sophisticated.
        if (ui->comboMode->currentIndex() == XmlEditor)
            filters << FILE_FILTER_XML
                    << FILE_FILTER_TXT;
        else
            filters << FILE_FILTER_TXT
                    << FILE_FILTER_XML;
        break;
    case JSON:
        filters << FILE_FILTER_JSON;
        break;
    case SVG:
        filters << FILE_FILTER_SVG;
        break;
    case XML:
        filters << FILE_FILTER_XML;
        break;
    case Null:
        return;
    }

    if (dataSource == HexBuffer)
        filters << FILE_FILTER_HEX;

    filters << FILE_FILTER_ALL;

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
              if (selectedFilter == FILE_FILTER_HEX)
                  file.write(hexEdit->toReadableString().toUtf8());
              else
                  file.write(hexEdit->data());
              break;
          case SciBuffer:
              // Data source is the Scintilla buffer
              file.write(sciEdit->text().toUtf8());
              break;
          case QtBuffer:
              // Data source is the text buffer
              file.write(ui->qtEdit->toPlainText().toUtf8());
              break;
          }
            file.close();
        }
    }
}

void EditDialog::setNull()
{
    ui->qtEdit->clear();
    imageEdit->resetImage();
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

bool EditDialog::promptInvalidData(const QString& data_type, const QString& errorString)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
      this,
      tr("Invalid data for this mode"),
      tr("The cell contains invalid %1 data. Reason: %2. Do you really want to apply it to the cell?").arg(data_type, errorString),
      QMessageBox::Apply | QMessageBox::Cancel);
    return (reply == QMessageBox::Apply);
}

void EditDialog::accept()
{
    if(!m_currentIndex.isValid())
        return;

    if (dataType == Null) {
        emit recordTextUpdated(m_currentIndex, hexEdit->data(), true);
        return;
    }

    // New data for the plain text cases. This is processed after the case.
    QString newTextData;

    switch (dataSource) {
    case QtBuffer:
        newTextData = removedBom + ui->qtEdit->toPlainText();
        break;
    case SciBuffer:
        switch (sciEdit->language()) {
        case DockTextEdit::PlainText:
            newTextData = removedBom + sciEdit->text();
            break;
        case DockTextEdit::JSON:
        {
            sciEdit->clearErrorIndicators();

            QString oldData = m_currentIndex.data(Qt::EditRole).toString();

            QString newData;
            bool proceed = true;
            json jsonDoc;

            try {
                jsonDoc = json::parse(sciEdit->text().toStdString());
            } catch(json::parse_error& parseError) {
                sciEdit->setErrorIndicator(static_cast<int>(parseError.byte - 1));

                proceed = promptInvalidData("JSON", parseError.what());
            }

            if (!jsonDoc.is_null()) {
                if (mustIndentAndCompact)
                    // Compact the JSON data before storing
                    newData = QString::fromStdString(jsonDoc.dump());
                else
                    newData = sciEdit->text();
            } else {
                newData = sciEdit->text();
            }
            proceed = proceed && (oldData != newData);

            if (proceed)
                // The data is different, so commit it back to the database
                emit recordTextUpdated(m_currentIndex, newData.toUtf8(), false);
        }
        break;
        case DockTextEdit::XML:
        {
            QString oldData = m_currentIndex.data(Qt::EditRole).toString();

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
                emit recordTextUpdated(m_currentIndex, newData.toUtf8(), false);
        }
        break;
        case DockTextEdit::SQL:
            emit evaluateText(m_currentIndex, sciEdit->text().toStdString());
            break;
        }
        break;
    case HexBuffer:
        // The data source is the hex widget buffer, thus binary data
        QByteArray oldData = m_currentIndex.data(Qt::EditRole).toByteArray();
        QByteArray newData = hexEdit->data();
        if (newData != oldData)
            emit recordTextUpdated(m_currentIndex, newData, true);
        break;
    }

    if (!newTextData.isEmpty()) {
        QString oldData = m_currentIndex.data(Qt::EditRole).toString();
        // Check first for null case, otherwise empty strings cannot overwrite NULL values
        if ((m_currentIndex.data(Qt::EditRole).isNull() && dataType != Null) || oldData != newTextData)
            // The data is different, so commit it back to the database
            emit recordTextUpdated(m_currentIndex, removedBom + newTextData.toUtf8(), false);
    }
    setModified(false);
}

void EditDialog::setDataInBuffer(const QByteArray& bArrdata, DataSources source)
{
    dataSource = source;
    QString textData;

    // 1) Perform validation and text formatting (if applicable).
    // 2) Set the text in the corresponding editor widget (the text widget for the Image case).
    // 3) Enable the widget.
    switch (dataSource) {
    case QtBuffer:
    {
        // Load the text into the text editor, remove BOM first if there is one
        QByteArray dataWithoutBom = bArrdata;
        removedBom = removeBom(dataWithoutBom);

        textData = QString::fromUtf8(dataWithoutBom.constData(), dataWithoutBom.size());
        ui->qtEdit->setPlainText(textData);

        // Select all of the text by default (this is useful for simple text data that we usually edit as a whole).
        // We don't want this when the QtBuffer has been automatically switched due to the insertion of RTL text,
        // (detected through the state of the apply button) otherwise that would break the typing flow of the user.
        if (!isReadOnly)
        {
            if (!ui->buttonApply->isEnabled())
                ui->qtEdit->selectAll();
            else
                ui->qtEdit->moveCursor(QTextCursor::End);
        }
        ui->qtEdit->setEnabled(true);
        break;
    }
    case SciBuffer:
        switch (sciEdit->language()) {
        case DockTextEdit::PlainText:
        case DockTextEdit::SQL:
        {
            // Load the text into the text editor, remove BOM first if there is one
            QByteArray dataWithoutBom = bArrdata;
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
            sciEdit->clearErrorIndicators();

            json jsonDoc;

            try {
                jsonDoc = json::parse(std::string(bArrdata.constData(), static_cast<size_t>(bArrdata.size())));
            } catch(json::parse_error& parseError) {
                sciEdit->setErrorIndicator(static_cast<int>(parseError.byte - 1));
            }

            if (mustIndentAndCompact && !jsonDoc.is_null() && !jsonDoc.is_discarded()) {
                // Load indented JSON into the JSON editor
                textData = QString::fromStdString(jsonDoc.dump(4));
            } else {
                // Fallback case. The data is not yet valid JSON or no auto-formatting applied.
                textData = QString::fromUtf8(bArrdata.constData(), bArrdata.size());
            }

            sciEdit->setText(textData);
            sciEdit->setEnabled(true);
        }

        break;

        case DockTextEdit::XML:
        {
            QString errorMsg;
            int errorLine, errorColumn;
            QDomDocument xmlDoc;
            bool isValid = xmlDoc.setContent(bArrdata, true, &errorMsg, &errorLine, &errorColumn);

            if (mustIndentAndCompact && isValid) {
                // Load indented XML into the XML editor
                textData = xmlDoc.toString(Settings::getValue("editor", "tabsize").toInt());
            } else {
                // Fallback case. The data is not yet valid JSON or no auto-formatting applied.
                textData = QString::fromUtf8(bArrdata.constData(), bArrdata.size());
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
        hexEdit->setData(bArrdata);
        hexEdit->setEnabled(true);

        break;
    }

}

// Called when the user manually changes the "Mode" drop down combobox
void EditDialog::editModeChanged(int newMode)
{
    ui->actionIndent->setEnabled(newMode == JsonEditor || newMode == XmlEditor);
    setStackCurrentIndex(newMode);

    // Change focus from the mode combo to the editor to start typing.
    if (ui->comboMode->hasFocus())
        setFocus();

    // * If the dataSource is the text buffer, the data is always text *
    switch (dataSource) {
    case QtBuffer:
        switch (newMode) {
        case RtlTextEditor: // Switching to the RTL text editor
            // Nothing to do, as the text is already in the Qt buffer
            break;

        case TextEditor: // Switching to one of the Scintilla editor modes
        case JsonEditor:
        case XmlEditor:
        case SqlEvaluator:
            setDataInBuffer(ui->qtEdit->toPlainText().toUtf8(), SciBuffer);
            break;

        case HexEditor: // Switching to the hex editor
            // Convert the text widget buffer for the hex widget
            // The hex widget buffer is now the main data source
            setDataInBuffer(removedBom + ui->qtEdit->toPlainText().toUtf8(), HexBuffer);
            break;

        case ImageEditor:
            // Clear any image from the image viewing widget
            imageEdit->resetImage();
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
        case RtlTextEditor: // Switching to the RTL text editor
            // Convert the Scintilla widget buffer for the Qt widget
            setDataInBuffer(sciEdit->text().toUtf8(), QtBuffer);
            break;
        case HexEditor: // Switching to the hex editor
            // Convert the text widget buffer for the hex widget
            setDataInBuffer(sciEdit->text().toUtf8(), HexBuffer);
            break;
        case ImageEditor:
        {
            // When SVG format, load the image, else clear it.
            QByteArray bArrdata = sciEdit->text().toUtf8();
            dataType = checkDataType(bArrdata);
            if (dataType == SVG) {
                QImage img;

                if (img.loadFromData(bArrdata))
                    imageEdit->setImage(img);
                else
                    // Clear any image from the image viewing widget
                    imageEdit->resetImage();
            }
        }
        break;

        case TextEditor:
        case JsonEditor:
        case XmlEditor:
        case SqlEvaluator:
            // The text is already in the Sci buffer but we need to perform the necessary formatting.
            setDataInBuffer(sciEdit->text().toUtf8(), SciBuffer);

            break;
        }
    }
}

// Called for every keystroke in the text editor (only)
void EditDialog::editTextChanged()
{
    if (dataSource == SciBuffer || dataSource == QtBuffer) {

        // Update the cell info in the bottom left manually.  This is because
        // updateCellInfoAndMode() only works with QByteArray's (for now)
        int dataLength;
        bool isModified;

        if(dataSource == QtBuffer) {
            // QtBuffer
            dataLength = ui->qtEdit->toPlainText().length();
            isModified = ui->qtEdit->document()->isModified();
        } else {
            // SciBuffer
            dataLength = sciEdit->text().length();
            isModified = sciEdit->isModified();

            // Switch to the Qt Editor if we detect right-to-left text,
            // since the QScintilla editor does not support it.
            if (containsRightToLeft(sciEdit->text()))
                ui->comboMode->setCurrentIndex(RtlTextEditor);
        }

        // If data has been entered in the text editor, it can't be a NULL
        // any more. It hasn't been validated yet, so it cannot be JSON nor XML.
        if (dataType == Null && isModified && dataLength != 0) {
            dataType = Text;
            sciEdit->clearTextInMargin();
        }
        if (dataType == Null)
            ui->labelInfo->setText(tr("Type: NULL; Size: 0 bytes"));
        else
            ui->labelInfo->setText(tr("Type: Text / Numeric; Size: %n character(s)", "", dataLength));
    }
}

void EditDialog::setMustIndentAndCompact(bool enable)
{
    mustIndentAndCompact = enable;

    // Indent or compact if necessary. If data has changed (button Apply indicates so), reload from the widget, else from the table.
    if (ui->buttonApply->isEnabled()) {
        setDataInBuffer(sciEdit->text().toUtf8(), SciBuffer);
    } else
        setCurrentIndex(m_currentIndex);
}

// Determine the type of data in the cell
int EditDialog::checkDataType(const QByteArray& bArrdata) const
{
    QByteArray cellData = bArrdata;

    // Check for NULL data type
    if (cellData.isNull()) {
        return Null;
    }

    // Check if it's an image
    QString imageFormat = isImageData(cellData);
    if(!imageFormat.isNull())
        return imageFormat == "svg" ? SVG : Image;

    // Check if it's text only
    if(isTextOnly(cellData))
    {
        if (cellData.startsWith("<?xml"))
            return XML;

        auto json_parse_result = json::parse(cellData, nullptr, false);
        if(!json_parse_result.is_discarded() && !json_parse_result.is_number())
            return JSON;
        else {
            if (containsRightToLeft(QString::fromUtf8(cellData)))
                return RtlText;
            else
                return Text;
        }
    }

    // It's none of the above, so treat it as general binary data
    return Binary;
}

void EditDialog::toggleOverwriteMode()
{
    static bool currentMode = false;
    currentMode = !currentMode;

    hexEdit->setOverwriteMode(currentMode);
    ui->qtEdit->setOverwriteMode(currentMode);
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
    case RtlTextEditor:
        ui->qtEdit->setFocus();
        ui->qtEdit->selectAll();
        break;
    case HexEditor:
        hexEdit->setFocus();
        break;
    case ImageEditor:
        // Nothing to do
        break;
    }

}

// Enables or disables the Apply, Null, & Import buttons in the Edit Cell dock.
// Sets or unsets read-only properties for the editors.
void EditDialog::setReadOnly(bool ro)
{
    if (ro && !isReadOnly) {
        promptSaveData();
    }
    isReadOnly = ro;

    ui->buttonApply->setEnabled(!ro);
    ui->actionNull->setEnabled(!ro);
    ui->actionImport->setEnabled(!ro);

    ui->qtEdit->setReadOnly(ro);
    sciEdit->setReadOnly(ro);
    hexEdit->setReadOnly(ro);

    // This makes the caret being visible for selection, although the editor is read-only. The read-only state is hinted by the
    // caret not blinking. The same is done in ExtendedScintilla.
    Qt::TextInteractionFlags textFlags = ro? Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard : Qt::TextEditorInteraction;
    ui->qtEdit->setTextInteractionFlags(textFlags);
}

void EditDialog::switchEditorMode(bool autoSwitchForType)
{
    if (autoSwitchForType) {
        // Switch automatically the editing mode according to the detected data.
        switch (dataType) {
        case Image:
        case SVG:
            ui->comboMode->setCurrentIndex(ImageEditor);
            break;
        case Binary:
            ui->comboMode->setCurrentIndex(HexEditor);
            break;
        case Null:
        case Text:
            ui->comboMode->setCurrentIndex(TextEditor);
            break;
       case RtlText:
            ui->comboMode->setCurrentIndex(RtlTextEditor);
            break;
        case JSON:
            ui->comboMode->setCurrentIndex(JsonEditor);
            break;
        case XML:
            ui->comboMode->setCurrentIndex(XmlEditor);
            break;
        }
    }
}

// Update the information labels in the bottom left corner of the dialog
// and switches the editor mode, if required, according to the detected data type.
void EditDialog::updateCellInfoAndMode(const QByteArray& bArrdata)
{
    QByteArray cellData = bArrdata;

    switchEditorMode(ui->buttonAutoSwitchMode->isChecked());

    // Image data needs special treatment
    if (dataType == Image || dataType == SVG) {
        QBuffer imageBuffer(&cellData);
        QImageReader imageReader(&imageBuffer);

        // Display the image format
        QString imageFormat = imageReader.format();

        // Display the image dimensions and size
        QSize imageDimensions = imageReader.size();
        unsigned int imageSize = static_cast<unsigned int>(cellData.size());

        QString labelInfoText = tr("Type: %1 Image; Size: %2x%3 pixel(s)")
            .arg(imageFormat.toUpper())
            .arg(imageDimensions.width()).arg(imageDimensions.height())
            + ", " + humanReadableSize(imageSize);

        ui->labelInfo->setText(labelInfoText);

        return;
    }

    // Use a switch statement for the other data types to keep things neat :)
    switch (dataType) {
    case Null: {
        // NULL data type
        ui->labelInfo->setText(tr("Type: NULL; Size: 0 bytes"));

        // Use margin to set the NULL text.
        sciEdit->setTextInMargin(Settings::getValue("databrowser", "null_text").toString());
        break;
    }
    case XML:
    case Text:
    case RtlText: {
        // Text only
        // Determine the length of the cell text in characters (possibly different to number of bytes).
        int textLength = QString(cellData).length();
        ui->labelInfo->setText(tr("Type: Text / Numeric; Size: %n character(s)", "", textLength));
        break;
    }
    case JSON: {
        // Valid JSON
        // Determine the length of the cell text in characters (possibly different to number of bytes).
        int jsonLength = QString(cellData).length();
        ui->labelInfo->setText(tr("Type: Valid JSON; Size: %n character(s)", "", jsonLength));
        break;
    }
    default:

        // Determine the length of the cell data
        int dataLength = cellData.length();
        // If none of the above data types, consider it general binary data
        ui->labelInfo->setText(tr("Type: Binary; Size: %n byte(s)", "", dataLength));
        break;
    }
}

void EditDialog::reloadSettings()
{
    // Set the (SQL) editor font for hex editor, since it needs a
    // Monospace font and the databrowser font would be usually of
    // variable width.
    QFont hexFont(Settings::getValue("editor", "font").toString());
    hexFont.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
    hexEdit->setFont(hexFont);

    // Set the databrowser font for the RTL text editor.
    QFont textFont(Settings::getValue("databrowser", "font").toString());
    textFont.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
    ui->qtEdit->setFont(textFont);

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
    case ImageEditor:
    case RtlTextEditor:
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
    case SqlEvaluator:
        // Scintilla case: switch to the single Scintilla editor and set language
        ui->editorStack->setCurrentIndex(TextEditor);
        sciEdit->setLanguage(DockTextEdit::SQL);
        break;
    }
}

void EditDialog::openPrintDialog()
{
    int editMode = ui->editorStack->currentIndex();
    if (editMode == ImageEditor) {
        imageEdit->openPrintImageDialog();
        return;
    }

    QPrinter printer;
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer);

    connect(dialog, &QPrintPreviewDialog::paintRequested, this, [this](QPrinter *previewPrinter) {
        QTextDocument document;
        switch (dataSource) {
        case SciBuffer:
            // This case isn't really expected because the Scintilla widget has it's own printing slot
            document.setPlainText(sciEdit->text());
            break;
        case HexBuffer:
            document.setPlainText(hexEdit->toReadableString());
            document.setDefaultFont(hexEdit->font());
            break;
        case QtBuffer:
            document.setPlainText(ui->qtEdit->toPlainText());
            break;
        }

        document.print(previewPrinter);
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
    ui->qtEdit->setWordWrapMode(value ? QTextOption::WrapAtWordBoundaryOrAnywhere : QTextOption::NoWrap);
}

void EditDialog::openDataWithExternal()
{
    QString extension;
    switch (dataType) {
    case Image: {
        // Images get special treatment.
        // Determine the likely filename extension.
        QByteArray cellData = hexEdit->data();
        QBuffer imageBuffer(&cellData);
        QImageReader imageReader(&imageBuffer);
        extension = imageReader.format().toLower().prepend(".");
        break;
    }
    case Binary:
        extension = FILE_EXT_BIN_DEFAULT;
        break;
    case RtlText:
    case Text:
        if (ui->comboMode->currentIndex() == XmlEditor)
            extension = FILE_EXT_XML_DEFAULT;
        else
            extension = FILE_EXT_TXT_DEFAULT;
        break;
    case JSON:
        extension = FILE_EXT_JSON_DEFAULT;
        break;
    case SVG:
        extension = FILE_EXT_SVG_DEFAULT;
        break;
    case XML:
        extension = FILE_EXT_XML_DEFAULT;
        break;
    case Null:
        return;
    }
    QTemporaryFile* file = new QTemporaryFile (QDir::tempPath() + QString("/DB4S-XXXXXX") + extension);

    if(!file->open()) {
        QMessageBox::warning(this, qApp->applicationName(),
                             tr("Couldn't save file: %1.").arg(file->fileName()));
        delete file;
    } else {
        switch (dataSource) {
        case HexBuffer:
            file->write(hexEdit->data());
            break;
        case SciBuffer:
            file->write(sciEdit->text().toUtf8());
            break;
        case QtBuffer:
            file->write(ui->qtEdit->toPlainText().toUtf8());
            break;
        }
        // We don't want the file to be automatically removed by Qt when destroyed.
        file->setAutoRemove(false);
        // But we don't want Qt to keep the file open (and locked in Windows),
        // and the only way is to destroy the object.
        QString fileName = file->fileName();
        delete file;
        emit requestUrlOrFileOpen(fileName);

        QMessageBox::StandardButton reply = QMessageBox::information
            (nullptr,
             QApplication::applicationName(),
             tr("The data has been saved to a temporary file and has been opened with the default application. "
                "You can now edit the file and, when you are ready, apply the saved new data to the cell or cancel any changes."),
             QMessageBox::Apply | QMessageBox::Cancel);

        QFile readFile(fileName);
        if(reply == QMessageBox::Apply && readFile.open(QIODevice::ReadOnly)){
            QByteArray d = readFile.readAll();
            loadData(d);
            readFile.close();
            accept();
        }
        readFile.remove();
    }
}
