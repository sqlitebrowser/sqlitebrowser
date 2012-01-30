#include "editform.h"

#include <QTextStream>
#include <qvariant.h>
#include <qmessagebox.h>
#include <qimage.h>
#include <qpixmap.h>
#include <QFileDialog>

#include "sqlitedb.h"
/*
 *  Constructs a editForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
editForm::editForm(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
editForm::~editForm()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void editForm::languageChange()
{
    retranslateUi(this);
}

void editForm::init()
{
    setModified(false);
    dataType = kSQLiteMediaType_Void;
    dataSize = 0;
    dataDepth = 0;
    curRow = -1;
    curCol = -1;
    textEditor->setPlainText("");
    textEditor->setFocus();
    setDataType(kSQLiteMediaType_Void, 0);
}

void editForm::destroy()
{

}

void editForm::reset()
{
    init();
}

void editForm::setModified(bool modifiedstate)
{
    modified = modifiedstate;
    saveChangesButton->setEnabled(modified);
}

void editForm::enableExport(bool enabled)
{
    exportButton->setEnabled(enabled);
}

void editForm::enableTextEditor(bool enabled)
{
    textEditor->setReadOnly(!enabled);
}

void editForm::setTextFormat(QString format)
{
//  if (format=="Auto")
//  {
//    textEditor->setTextFormat(Qt::AutoText);
//  } else {
//    textEditor->setTextFormat(Qt::PlainText);
//  }
}

void editForm::setDataType(int type, int size)
{
    dataType = type;
    dataSize = size;
    QString  charstr;
    switch (dataType ) {
        case kSQLiteMediaType_String:
             currentTypeLabel->setText("Type of data currently in cell: Text / Numeric");
             if (textEditor->toPlainText().length()>1) charstr = QString("chars"); else charstr = QString("char");
             currentDataInfo->setText(QString("%1 %2").arg(textEditor->toPlainText().length()).arg(charstr));
             editWidgetStack->setCurrentIndex(0);
             enableExport(true);
             enableTextEditor(true);
            break;
        case kSQLiteMediaType_Void:
            currentTypeLabel->setText("Type of data currently in cell: Empty");
             currentDataInfo->setText("");
             editWidgetStack->setCurrentIndex(0);
             enableExport(false);
             enableTextEditor(true);
            break;
    }
}

void editForm::closeEvent( QCloseEvent * )
{
    emit goingAway();
}

void editForm::loadText(QString  text, int row, int col)
{
    textEditor->setPlainText(text);
    textEditor->setFocus();
    textEditor->selectAll();
    curRow = row;
    curCol = col;
    if (textEditor->toPlainText().length() > 0)
    {
    setDataType(kSQLiteMediaType_String, 0);
} else {
    setDataType(kSQLiteMediaType_Void, 0);
}
    setModified(false);
}


void editForm::importData()
{
    int type = kSQLiteMediaType_Void;
        QString fileName = QFileDialog::getOpenFileName(
                    this,
                    "Choose an image file",
                    QString(),
                    QString( "Text files (*.txt);;All files (*.*);;"));
    if (QFile::exists(fileName) )
    {
        type = kSQLiteMediaType_String;

        QFile file( fileName );
        if ( file.open( QIODevice::ReadOnly ) ) {
            QTextStream stream( &file );
            textEditor->setPlainText(stream.readAll());
            file.close();
        }

        setModified(true);
    }
}


void editForm::exportData()
{
    QString filter;
    switch (dataType)
    {
    case kSQLiteMediaType_String:
        filter =  "Text files (*.txt)";
        break;

    default:
        return;
    }


    QString fileName = QFileDialog::getSaveFileName(
                this,
                "Choose a filename to export data",
                defaultlocation,
                filter);

    if (fileName.size() > 0)
    {
            switch (dataType)
            {
            case kSQLiteMediaType_String:
           {
                QFile file(fileName);
                if ( file.open( QIODevice::WriteOnly ) ) {
                       QTextStream stream( &file );
                       stream << textEditor->toPlainText();
                       file.close();
                   }
            }
                break;

            default:
                return;
            }
    }
}


void editForm::clearData()
{
    textEditor->setPlainText("");
    setDataType(kSQLiteMediaType_Void, 0);
    setModified(true);
}


void editForm::saveChanges()
{
    if (dataType==kSQLiteMediaType_String)
        emit updateRecordText(curRow, curCol, textEditor->toPlainText());

    if (dataType==kSQLiteMediaType_Void)
        emit updateRecordText(curRow, curCol, QString(""));

    setModified(false);
    emit goingAway();
}



void editForm::editTextChanged()
{
    int newtype = kSQLiteMediaType_String;
    if (textEditor->toPlainText().length() == 0)
    {
    newtype = kSQLiteMediaType_Void;
   }
    if (newtype!=dataType)
    {
    setDataType(newtype, 0);
}
    setModified(true);
}


void editForm::editorClicked()
{
    enableTextEditor(true);
}

