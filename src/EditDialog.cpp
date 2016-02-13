#include "EditDialog.h"
#include "ui_EditDialog.h"
#include "sqlitedb.h"
#include "PreferencesDialog.h"
#include "src/qhexedit.h"
#include "FileDialog.h"

#include <QKeySequence>
#include <QShortcut>

EditDialog::EditDialog(QWidget* parent, bool forUseInDockWidget)
    : QDialog(parent),
      ui(new Ui::EditDialog),
      useInDock(forUseInDockWidget)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setVisible(!forUseInDockWidget);

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
    curRow = -1;
    curCol = -1;
    ui->editorText->clear();
    ui->editorText->setFocus();
    ui->editorImage->clear();
    hexEdit->setData(QByteArray());
    oldData = "";
    checkDataType();
}

void EditDialog::closeEvent(QCloseEvent*)
{
    emit goingAway();
}

void EditDialog::showEvent(QShowEvent*)
{
    // Whenever the dialog is shown, position it at the center of the parent dialog
    QPoint center = mapToGlobal(rect().center());
    QMainWindow* parentDialog = qobject_cast<QMainWindow*>(parent());
    if(parentDialog)
    {
        QPoint parentCenter = parentDialog->window()->mapToGlobal(parentDialog->window()->rect().center());
        move(parentCenter - center);
    }
}

void EditDialog::reject()
{
    // This is called when pressing the cancel button or hitting the escape key

    // If we're in dock mode, reset all fields and move the cursor back to the table view.
    // If we're in window mode, call the default implementation to just close the window normally.
    if(useInDock)
    {
        loadText(oldData, curRow, curCol);
        emit goingAway();
    } else {
        QDialog::reject();
    }
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

    // If we're in window mode (not in dock mode) focus the editor widget
    if(!useInDock)
    {
        ui->editorText->setFocus();
        ui->editorText->selectAll();
    }

    // Assume it's binary data and only call checkDatyType afterwards. This means the correct input widget is selected here in all cases
    // but once the user changed it to text input it will stay there.
    ui->editorStack->setCurrentIndex(1);
    checkDataType();
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
            checkDataType();
            file.close();
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

void EditDialog::clearData()
{
    ui->editorText->clear();
    ui->editorImage->clear();
    hexEdit->setData(QByteArray());
    checkDataType();
    ui->editorText->setFocus();
}

void EditDialog::accept()
{
    // Don't update if the data hasn't changed
    // To differentiate NULL and empty byte arrays, we also compare the NULL flag
    if(hexEdit->data() != oldData || hexEdit->data().isNull() != oldData.isNull())
    {
        const QString dataType = ui->comboEditor->currentText();
        bool isBlob = dataType == tr("Binary") || !ui->comboEditor->isVisible();
        emit updateRecordText(curRow, curCol, isBlob, hexEdit->data());
    }
    emit goingAway();
}

void EditDialog::editTextChanged()
{
    if(ui->editorText->hasFocus())
    {
        hexEdit->blockSignals(true);
        hexEdit->setData(ui->editorText->toPlainText().toUtf8());
        hexEdit->blockSignals(false);
        checkDataType();
    }
}

void EditDialog::hexDataChanged()
{
    // Update the text editor accordingly
    ui->editorText->blockSignals(true);
    ui->editorText->setPlainText(QString::fromUtf8(hexEdit->data().constData(), hexEdit->data().size()));
    ui->editorText->blockSignals(false);
}

void EditDialog::checkDataType()
{
    ui->comboEditor->setVisible(true);

    // Assume NULL type first
    if (hexEdit->data().isNull())
        ui->labelType->setText(tr("Type of data currently in cell: Null"));

    // Check if data is text only
    if(QString(hexEdit->data()).toUtf8() == hexEdit->data())     // Any proper way??
    {
        ui->editorStack->setCurrentIndex(0);

        ui->labelBinayWarning->setVisible(false);

        if (!hexEdit->data().isNull())
            ui->labelType->setText(tr("Type of data currently in cell: Text / Numeric"));

        ui->labelSize->setText(tr("%n char(s)", "", hexEdit->data().length()));
    } else {
        // It's not. So it might be an image.
        QImage img;
        if(img.loadFromData(hexEdit->data()))
        {
            // It is.
            ui->editorImage->setPixmap(QPixmap::fromImage(img));
            ui->editorStack->setCurrentIndex(2);

            ui->labelType->setText(tr("Type of data currently in cell: Image"));
            ui->labelSize->setText(tr("%1x%2 pixel").arg(ui->editorImage->pixmap()->size().width()).arg(ui->editorImage->pixmap()->size().height()));

            ui->comboEditor->setVisible(false);
        } else {
            // It's not. So it's probably some random binary data.

            ui->labelBinayWarning->setVisible(true);

            if (!hexEdit->data().isNull())
                ui->labelType->setText(tr("Type of data currently in cell: Binary"));

            ui->labelSize->setText(tr("%n byte(s)", "", hexEdit->data().length()));
        }
    }
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

    // When in dock mode set the focus to the editor widget. The idea here is that setting focus to the
    // dock itself doesn't make much sense as it's just a frame; you'd have to tab to the editor which is what you
    // most likely want to use. So in order to save the user from doing this we explicitly set the focus to the editor.
    if(useInDock)
    {
        ui->editorText->setFocus();
        ui->editorText->selectAll();
    }
}

void EditDialog::allowEditing(bool on)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(on);
    ui->buttonClear->setEnabled(on);
    ui->buttomImport->setEnabled(on);
}
