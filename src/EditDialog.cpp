#include "EditDialog.h"
#include "ui_EditDialog.h"
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include "sqlitedb.h"

EditDialog::EditDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditDialog)
{
    ui->setupUi(this);
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
    ui->editData->setPlainText("");
    ui->editData->setFocus();
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
        if(ui->editData->toPlainText().length() == 1) charstr = QString("char"); else charstr = QString("chars");
        ui->labelSize->setText(QString("%1 %2").arg(ui->editData->toPlainText().length()).arg(charstr));
        enableExport(true);
        break;
    case kSQLiteMediaType_Void:
        ui->labelType->setText("Type of data currently in cell: Empty");
        ui->labelSize->setText("");
        enableExport(false);
        break;
    }
}

void EditDialog::closeEvent(QCloseEvent*)
{
    emit goingAway();
}

void EditDialog::loadText(QString  text, int row, int col)
{
    ui->editData->setPlainText(text);
    ui->editData->setFocus();
    ui->editData->selectAll();
    curRow = row;
    curCol = col;
    if(ui->editData->toPlainText().length() > 0)
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
                QString("Text files(*.txt);;All files(*)"));
    if(QFile::exists(fileName))
    {
        type = kSQLiteMediaType_String;
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            ui->editData->setPlainText(stream.readAll());
            file.close();
        }
        setDataType(type, ui->editData->toPlainText().length());
    }
}

void EditDialog::exportData()
{
    QString filter;
    switch (dataType)
    {
    case kSQLiteMediaType_String:
        filter = "Text files(*.txt)";
        break;
    default:
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
                this,
                "Choose a filename to export data",
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
                    QTextStream stream(&file);
                    stream << ui->editData->toPlainText();
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
    ui->editData->setPlainText("");
    setDataType(kSQLiteMediaType_Void, 0);
}

void EditDialog::accept()
{
    if(dataType == kSQLiteMediaType_String)
        emit updateRecordText(curRow, curCol, ui->editData->toPlainText());

    if (dataType == kSQLiteMediaType_Void)
        emit updateRecordText(curRow, curCol, QString(""));

    emit goingAway();
}

void EditDialog::editTextChanged()
{
    int newtype = kSQLiteMediaType_String;
    if(ui->editData->toPlainText().length() == 0)
        newtype = kSQLiteMediaType_Void;
    setDataType(newtype, ui->editData->toPlainText().length());
}
