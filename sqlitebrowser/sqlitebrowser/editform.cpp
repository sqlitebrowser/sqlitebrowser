#include "editform.h"

#include <qvariant.h>
#include <q3filedialog.h>
#include <qmessagebox.h>
#include <qimage.h>
#include <qpixmap.h>
#include <Q3TextStream>

#include "sqlitedb.h"
/*
 *  Constructs a editForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
editForm::editForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
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
    textEditor->setText("");
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
  if (format=="Auto")
  {
    textEditor->setTextFormat(Qt::AutoText);
  } else {
    textEditor->setTextFormat(Qt::PlainText);
  }
}

void editForm::setDataType(int type, int size)
{
    dataType = type;
    dataSize = size;
    QString  charstr;
    switch (dataType ) {
        case kSQLiteMediaType_String:
             currentTypeLabel->setText("Type of data currently in cell: Text / Numeric");
             if (textEditor->length()>1) charstr = QString("chars"); else charstr = QString("char");
             currentDataInfo->setText(QString("%1 %2").arg(textEditor->length()).arg(charstr));
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
    textEditor->setText(text);
    curRow = row;
    curCol = col;
    if (textEditor->length() > 0)
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
        QString fileName = Q3FileDialog::getOpenFileName(
                    "",
                    QString( "Text files (*.txt);;"
          "All files (*.*);;"),
                    this,
                    "open file dialog"
                    "Choose an image file" );
    if (QFile::exists(fileName) )
    {
        type = kSQLiteMediaType_String;

        QFile file( fileName );
        if ( file.open( QIODevice::ReadOnly ) ) {
            Q3TextStream stream( &file );
            textEditor->setText(stream.read());
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


    QString fileName = Q3FileDialog::getSaveFileName(
                    defaultlocation,
                    filter,
                    this,
                    "save file dialog"
                    "Choose a filename to export data" );

    if (fileName.size() > 0)
    {
            switch (dataType)
            {
            case kSQLiteMediaType_String:
           {
                QFile file(fileName);
                if ( file.open( QIODevice::WriteOnly ) ) {
                       Q3TextStream stream( &file );
                       stream << textEditor->text();
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
    textEditor->setText("");
    setDataType(kSQLiteMediaType_Void, 0);
    setModified(true);
}


void editForm::saveChanges()
{
    if (dataType==kSQLiteMediaType_String)
        emit updateRecordText(curRow, curCol, textEditor->text());

    if (dataType==kSQLiteMediaType_Void)
        emit updateRecordText(curRow, curCol, QString(""));

    setModified(false);
    emit goingAway();
}



void editForm::editTextChanged()
{
    int newtype = kSQLiteMediaType_String;
    if (textEditor->length() == 0)
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

