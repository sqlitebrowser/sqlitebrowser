/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

void findForm::showResults(resultMap rmap)
{
    findListView->clear();
    findListView->setSorting(-1);
    resultMap::Iterator it;
    QListViewItem * lasttbitem = 0;
        for ( it = rmap.begin(); it != rmap.end(); ++it ) {
	    QListViewItem * tbitem = new QListViewItem( findListView, lasttbitem );
	    //tbitem->setOpen( TRUE );
	    tbitem->setText( 0, QString::number(it.key() + 1,10) ); //increase from index 0
	     tbitem->setText( 1, it.data() );
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


void findForm::recordSelected( QListViewItem * witem)
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
