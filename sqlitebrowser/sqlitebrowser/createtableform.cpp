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
}

//** Create The Table
void createTableForm::confirmCreate()
{
    //TODO change these warnings to status bar message
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

    if (treeWidget->invisibleRootItem()->childCount() == 0) {
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
        for(int i = 0; i < treeWidget->invisibleRootItem()->childCount(); i++){
            QTreeWidgetItem *item = treeWidget->invisibleRootItem()->child(i);
            createStatement.append(item->text(0));
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
    addFieldForm * addForm = new addFieldForm( this, "addfield", TRUE );
    addForm->setInitialValues(QString(""),QString(""));
    if (addForm->exec())
    {
        QTreeWidgetItem *newItem = new QTreeWidgetItem();
        newItem->setText(0, addForm->fname);
        newItem->setText(1, addForm->ftype);
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
