#include "createtableform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qmessagebox.h"
#include "addfieldform.h"
/*
 *  Constructs a createTableForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
createTableForm::createTableForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
createTableForm::~createTableForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void createTableForm::languageChange()
{
    retranslateUi(this);
}

void createTableForm::init()
{
   fieldListView->clear();
   fieldListView->setSorting (-1, FALSE);
}

void createTableForm::confirmCreate()
{
    bool ok = true;
    QString tabname = tablenameLineEdit->text();
    if (tabname.isEmpty()) {
        ok = false;
        QMessageBox::information( this, applicationName, "Please select a name for the table" );
        return;
    }
    if (tabname.contains(" ")>0) {
        ok = false;
        QMessageBox::warning( this, applicationName, "Spaces are not allowed in the table name" );
        return;
    }

    if (fieldListView->firstChild()==0) {
        ok = false;
        QMessageBox::information( this, applicationName, "No fields defined" );
        return;
    }

    /*check field names for empty or illegal names
   TODO: move to add

    for (int r=0; r<fieldsTable->numRows();r++){
        QString rowname = fieldsTable->text(r, 0);
        if (rowname.isEmpty()) {
            ok = false;
            QMessageBox::warning( this, applicationName, "Empty field names are not allowed" );
            break;
        }
        if (rowname.contains(" ")>0) {
            ok = false;
            QMessageBox::warning( this, applicationName, "Spaces are not allowed in the field names" );
            break;
        }
    }*/

    if (!ok){
        return;
    }

    if (ok){
        createStatement = "CREATE TABLE ";
        createStatement.append(tabname);
        createStatement.append(" (");
        Q3ListViewItemIterator it( fieldListView );
        Q3ListViewItem * item;
        while ( it.current() ) {
            item = it.current();
          createStatement.append(item->text(0));
          createStatement.append(" ");
          createStatement.append(item->text(1));
            if (item->nextSibling() != 0)
           {
                createStatement.append(", ");
            }
            ++it;
        }
        /*for (int r=0; r<fieldsTable->numRows();r++){
            createStatement.append(fieldsTable->text(r, 0));
            createStatement.append(" ");
            createStatement.append(fieldsTable->text(r, 1));
            if (r<(fieldsTable->numRows() - 1))
                createStatement.append(", ");
        }*/
        createStatement.append(");");
        accept();
    }
}


void createTableForm::addField()
{
   addFieldForm * addForm = new addFieldForm( this, "addfield", TRUE );
   addForm->setInitialValues(QString(""),QString(""));
    if (addForm->exec())
   {
       //qDebug(addForm->fname + addForm->ftype);
       Q3ListViewItem * tbitem = new Q3ListViewItem( fieldListView, fieldListView->lastItem() );
       tbitem->setText( 0, addForm->fname  );
       tbitem->setText( 1, addForm->ftype );
   }
}


void createTableForm::deleteField()
{
    Q3ListViewItem * item = fieldListView->selectedItem();
    if (item==0) {
        //should never happen, the button would not be active, but...
        return;
    } else {
        delete item;
        }
}



void createTableForm::fieldSelectionChanged()
{
     Q3ListViewItem * item = fieldListView->selectedItem();
    if (item==0) {
        buttonDeleteField->setEnabled(false);
    } else {
        buttonDeleteField->setEnabled(true);
    }

}
