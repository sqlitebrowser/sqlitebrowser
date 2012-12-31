#include "createtableform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qmessagebox.h"
#include "editfieldform.h"
/*
 *  Constructs a createTableForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
createTableForm::createTableForm(DBBrowserDB *db, QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl),
      pdb(db)
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
}

//** Create The Table
void createTableForm::confirmCreate()
{
    bool ok = true;
    QString tabname = tablenameLineEdit->text();
    if (tabname.isEmpty()) {
        ok = false;
        statusBar->showMessage("Please enter a name for the table", 4000);
        tablenameLineEdit->setFocus();
        return;
    }
    if (tabname.contains(" ")>0) {
        ok = false;
        statusBar->showMessage("Spaces are not allowed in the table name", 4000);
        tablenameLineEdit->setFocus();
        return;
    }

    if (treeWidget->invisibleRootItem()->childCount() == 0) {
        ok = false;
        statusBar->showMessage("No fields defined", 4000);
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
        //QString esc("\"");
        // #TODO The colnames need to be escaped eg create table 'group'
        createStatement = "CREATE TABLE ";
        //createStatement.append(esc).append(tabname).append(esc);
        createStatement.append(tabname);
        createStatement.append(" (");
        for(int i = 0; i < treeWidget->invisibleRootItem()->childCount(); i++){
            QTreeWidgetItem *item = treeWidget->invisibleRootItem()->child(i);
            //createStatement.append(esc).append(item->text(0)).append(esc);
            createStatement.append( item->text(0) );
            createStatement.append(" ");
            createStatement.append(item->text(1));
            if(i < treeWidget->invisibleRootItem()->childCount() -1){
                createStatement.append(", ");
            }
        }
        createStatement.append(");");
        accept();
    }
}


void createTableForm::addField()
{
    //TODO maybe embedd locally
    editFieldForm * addForm = new editFieldForm( this );
    addForm->setModal(true);
    addForm->setInitialValues(pdb, true, QString(""), QString(""),QString(""));
    if (addForm->exec())
    {
        QTreeWidgetItem *newItem = new QTreeWidgetItem();
        newItem->setText(0, addForm->field_name);
        newItem->setText(1, addForm->field_type);
        newItem->setIcon(0, QIcon(":/icons/field"));
        treeWidget->addTopLevelItem(newItem);
    }
}


void createTableForm::deleteField()
{
    if( !treeWidget->currentItem() ){
        return;
    }
    treeWidget->invisibleRootItem()->removeChild(treeWidget->currentItem());
}


void createTableForm::fieldSelectionChanged()
{
    buttonDeleteField->setEnabled( treeWidget->selectionModel()->hasSelection() );
}
