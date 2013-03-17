#include "EditDialog.h"
#include "ui_EditDialog.h"
#include <QFileDialog>
#include <QKeySequence>
#include <QShortcut>
#include "sqlitedb.h"
#include <src/qhexedit.h>

EditDialog::EditDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditDialog)
{
    ui->setupUi(this);

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

void EditDialog::loadText(const QByteArray& data, int row, int col)
{
    curRow = row;
    curCol = col;
    oldData = data;

    ui->editorText->setPlainText(data);
    ui->editorText->setFocus();
    ui->editorText->selectAll();
    hexEdit->setData(data);
    checkDataType();
}

void EditDialog::importData()
{
    // Get list of supported image file formats to include them in the file dialog filter
    QString image_formats;
    QList<QByteArray> image_formats_list = QImageReader::supportedImageFormats();
    for(int i=0;i<image_formats_list.size();++i)
        image_formats.append(QString("*.%1 ").arg(QString::fromUtf8(image_formats_list.at(i))));

    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Choose a file"),
                defaultlocation,
                tr("Text files(*.txt);;Image files(%1);;All files(*)").arg(image_formats));
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
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Choose a filename to export data"),
                defaultlocation,
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
}

void EditDialog::accept()
{
    if(hexEdit->data() != oldData)
        emit updateRecordText(curRow, curCol, hexEdit->data());
    emit goingAway();
}

void EditDialog::editTextChanged()
{
    if(ui->editorText->hasFocus())
    {
        hexEdit->setData(ui->editorText->toPlainText().toUtf8());
        checkDataType();
    }
}

void EditDialog::hexDataChanged()
{
    // Update the text editor accordingly
    ui->editorText->setPlainText(hexEdit->data());
}

void EditDialog::checkDataType()
{
    // Check if data is text only
    ui->comboEditor->setVisible(true);
    if(QString(hexEdit->data()).toAscii() == hexEdit->data())     // Any proper way??
    {
        ui->editorStack->setCurrentIndex(0);

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
            ui->editorStack->setCurrentIndex(1);

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
