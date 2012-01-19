#include "findform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a findForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
findForm::findForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
findForm::~findForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void findForm::languageChange()
{
    retranslateUi(this);
}

void findForm::showResults(resultMap rmap)
{
    findListView->clear();
    findListView->setSorting(-1);
    resultMap::Iterator it;
    Q3ListViewItem * lasttbitem = 0;
        for ( it = rmap.begin(); it != rmap.end(); ++it ) {
            Q3ListViewItem * tbitem = new Q3ListViewItem( findListView, lasttbitem );
            //tbitem->setOpen( TRUE );
            tbitem->setText( 0, QString::number(it.key() + 1,10) ); //increase from index 0
            QString firstline = it.data().section( '\n', 0,0 );
                  if (firstline.length()>MAX_DISPLAY_LENGTH)
                {
                    firstline.truncate(MAX_DISPLAY_LENGTH);
                   firstline.append("...");
                }
             tbitem->setText( 1, firstline );
            lasttbitem = tbitem;
        }
        QString results = "Found: ";
        results.append(QString::number(findListView->childCount()));
        resultsLabel->setText(results);
}


void findForm::find()
{
   emit lookfor( findFieldCombobox->currentText(), findOperatorComboBox->currentText(),searchLine->text() );
}

void findForm::resetFields(QStringList fieldlist)
{
    findFieldCombobox->clear();
    for ( QStringList::Iterator ct = fieldlist.begin(); ct != fieldlist.end(); ++ct ) {
        findFieldCombobox->insertItem(*ct,-1);
    }
}

void findForm::resetResults()
{
    findListView->clear();
    resultsLabel->setText("Found: 0");
}


void findForm::recordSelected( Q3ListViewItem * witem)
{
    if (witem) {
    int recNum = (witem->text(0)).toInt() ;
    emit showrecord(recNum);
}
}

void findForm::closeEvent( QCloseEvent * )
{
        emit goingAway();
}


