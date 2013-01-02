#include "FindForm.h"

/*
 *  Constructs a findForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
findForm::findForm(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
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
    findTableWidget->clearContents();
    findTableWidget->setSortingEnabled(false);
    resultMap::Iterator it;
    findTableWidget->setRowCount(rmap.size());
    for ( it = rmap.begin(); it != rmap.end(); ++it ) {
        QString firstline = it.value().section( '\n', 0,0 );
        findTableWidget->setItem( it.key(), 0, new QTableWidgetItem( QString::number(it.key() + 1) ) );
        findTableWidget->setItem( it.key(), 1, new QTableWidgetItem( firstline) );
    }
    QString results = "Found: ";
    results.append(QString::number(findTableWidget->rowCount()));
    resultsLabel->setText(results);
    findTableWidget->setSortingEnabled(true);
}


void findForm::find()
{
   emit lookfor( findFieldCombobox->currentText(), findOperatorComboBox->currentText(),searchLine->text() );
}

void findForm::resetFields(QStringList fieldlist)
{
    findFieldCombobox->clear();
    findFieldCombobox->addItems(fieldlist);
}

void findForm::resetResults()
{
    findTableWidget->clearContents();
    resultsLabel->setText("Found: 0");
}


void findForm::recordSelected( QTableWidgetItem * witem)
{
    if (witem) {
        int recNum = witem->text().toInt();
        emit showrecord(recNum);
    }
}

void findForm::closeEvent( QCloseEvent * )
{
        emit goingAway();
}


