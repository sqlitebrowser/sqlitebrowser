#include "createindexform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qmessagebox.h"
/*
 *  Constructs a createIndexForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
createIndexForm::createIndexForm(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
createIndexForm::~createIndexForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void createIndexForm::languageChange()
{
    retranslateUi(this);
}


void createIndexForm::tableSelected( const QString & entry )
{
        tableMap::Iterator it;
         for ( it = mtablemap.begin(); it != mtablemap.end(); ++it ) {
             QString tname = it.value().getname() ;

             //populate the fields with first table name
             if ((tname.compare(entry)==0)){
                 comboFields->clear();
                 fieldMap::Iterator fit;
                 fieldMap fmap = it.value().fldmap;
                 for ( fit = fmap.begin(); fit != fmap.end(); ++fit ) {
                     comboFields->addItem( fit.value().getname() );
                 }
             }
        }
}

void createIndexForm::confirmCreate()
{
    bool ok = true;
    QString idxname = indexLineEdit->text();
    if (idxname.isEmpty()) {
        ok = false;
        QMessageBox::information( this, QApplication::applicationName(), "Please select a name for the index" );
        return;
    }
    if (idxname.contains(" ")>0) {
        ok = false;
        QMessageBox::warning( this, QApplication::applicationName(), "Spaces are not allowed in the index name" );
        return;
    }
    if (ok){
        createStatement = "CREATE ";
        if (comboUnique->currentIndex()==1){
            createStatement.append("UNIQUE ");
        }
        createStatement.append("INDEX ");
        createStatement.append(indexLineEdit->text());
        createStatement.append(" ON ");
        createStatement.append(comboTables->currentText());
        createStatement.append("(");
        createStatement.append(comboFields->currentText());
        createStatement.append(" ");
        if (comboOrder->currentIndex()==0){
            createStatement.append("ASC");
        } else {
            createStatement.append("DESC");
        }
        createStatement.append(");");
        accept();
    }
}

void createIndexForm::populateTable(tableMap rmap)
{
    mtablemap = rmap;
    tableMap::Iterator it;
         for ( it = mtablemap.begin(); it != mtablemap.end(); ++it ) {
             comboTables->addItem( it.value().getname() );

             //populate the fields with first table name
             if (it==mtablemap.begin()){
                 fieldMap::Iterator fit;
                 fieldMap fmap = it.value().fldmap;
                 for ( fit = fmap.begin(); fit != fmap.end(); ++fit ) {
                     comboFields->addItem( fit.value().getname() );
                 }
             }
        }
}


