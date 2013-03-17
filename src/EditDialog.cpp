#include "EditDialog.h"
#include "ui_EditDialog.h"
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include "sqlitedb.h"
#include <src/qhexedit.h>

EditDialog::EditDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditDialog)
{
    ui->setupUi(this);

    QHBoxLayout* hexLayout = new QHBoxLayout;
    hexEdit = new QHexEdit(this);
    hexLayout->addWidget(hexEdit);
    ui->editorBinary->setLayout(hexLayout);

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
    setDataType(kSQLiteMediaType_Void, 0);
}

void EditDialog::enableExport(bool enabled)
{
    ui->buttonExport->setEnabled(enabled);
}

void EditDialog::setDataType(int type, int size)
{
    dataType = type;
    dataSize = size;
    QString charstr;
    switch(dataType)
    {
    case kSQLiteMediaType_String:
        ui->labelType->setText(tr("Type of data currently in cell: Text / Numeric"));
        ui->labelSize->setText(tr("%n char(s)", "", hexEdit->data().length()));
        enableExport(true);
        break;
    case kSQLiteMediaType_Void:
        ui->labelType->setText(tr("Type of data currently in cell: Empty"));
        ui->labelSize->setText(tr(""));
        enableExport(false);
        break;
    }
}

void EditDialog::closeEvent(QCloseEvent*)
{
    emit goingAway();
}

void EditDialog::loadText(const QByteArray& data, int row, int col)
{
    // Check if data is text only
    if(QString(data).toAscii() == data)     // Any proper way??
    {
        ui->editorStack->setCurrentIndex(0);
    } else {
        // It's not. So it might be an image.
        QImage img;
        if(img.loadFromData(data))
        {
            // It is.
            ui->editorImage->setPixmap(QPixmap::fromImage(img));
            ui->editorStack->setCurrentIndex(1);
        } else {
            // It's not. So it's probably some random binary data.
            ui->editorStack->setCurrentIndex(2);
        }
    }

    ui->editorText->setPlainText(data);
    ui->editorText->setFocus();
    ui->editorText->selectAll();
    hexEdit->setData(data);
    curRow = row;
    curCol = col;
    if(hexEdit->data().length() > 0)
        setDataType(kSQLiteMediaType_String, 0);
    else
        setDataType(kSQLiteMediaType_Void, 0);
}

void EditDialog::importData()
{
    int type = kSQLiteMediaType_Void;
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Choose a file"),
                defaultlocation,
                tr("Text files(*.txt);;All files(*)"));
    if(QFile::exists(fileName))
    {
        type = kSQLiteMediaType_String;
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray d = file.readAll();
            hexEdit->setData(d);
            ui->editorText->setPlainText(d);
            file.close();
        }
        setDataType(type, hexEdit->data().length());
    }
}

void EditDialog::exportData()
{
    QString filter;
    switch (dataType)
    {
    case kSQLiteMediaType_String:
        filter = tr("Text files(*.txt)");
        break;
    default:
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Choose a filename to export data"),
                defaultlocation,
                filter);

    if(fileName.size() > 0)
    {
        switch (dataType)
        {
        case kSQLiteMediaType_String:
            {
                QFile file(fileName);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(hexEdit->data());
                    file.close();
                }
            }
            break;
        default:
            return;
        }
    }
}

void EditDialog::clearData()
{
    ui->editorText->clear();
    ui->editorImage->clear();
    hexEdit->setData(QByteArray());
    setDataType(kSQLiteMediaType_Void, 0);
}

void EditDialog::accept()
{
    if(dataType == kSQLiteMediaType_String)
        emit updateRecordText(curRow, curCol, hexEdit->data());

    if (dataType == kSQLiteMediaType_Void)
        emit updateRecordText(curRow, curCol, "");

    emit goingAway();
}

void EditDialog::editTextChanged()
{
    if(ui->editorText->hasFocus())
        hexEdit->setData(ui->editorText->toPlainText().toUtf8());

    int newtype = kSQLiteMediaType_String;
    if(hexEdit->data().length() == 0)
        newtype = kSQLiteMediaType_Void;
    setDataType(newtype, hexEdit->data().length());
}
