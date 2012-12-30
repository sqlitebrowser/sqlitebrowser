#include "preferencesform.h"
#include "ui_preferencesform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qdir.h"
#include "qsettings.h"
#include <QFileDialog>
#include "sqlitedb.h"

/*
 *  Constructs a preferencesForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
preferencesForm::preferencesForm(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl),
      ui(new Ui::preferencesForm)
{
    ui->setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
preferencesForm::~preferencesForm()
{
    delete ui;
}

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

void preferencesForm::init()
{
    loadSettings();
}


void preferencesForm::defaultDataChanged( int which )
{
    if (which==2) {
        defaultnewdata = QString("\'\'");
    } else if (which==1) {
        defaultnewdata = QString("0");
    } else {
        defaultnewdata = QString("NULL");
    }
}

void preferencesForm::defaultTextChanged( int which )
{
    if (which==1) {
        defaulttext = QString("Auto");
    } else {
        defaulttext = QString("Plain");
    }

}


void preferencesForm::encodingChanged( int which )
{
    if (which==1) {
        defaultencoding = QString("Latin1");
    } else {
        defaultencoding = QString("UTF8");
    }
}


void preferencesForm::chooseLocation()
{
    QString s = QFileDialog::getExistingDirectory(
                this,
                tr("Choose a directory"),
                defaultlocation,
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!s.isEmpty())
    {
        defaultlocation = s;
	ui->locationEdit->setText(defaultlocation);
    }
}


void preferencesForm::loadSettings()
{
    QSettings settings(QApplication::organizationName(), g_sApplicationNameShort);
    settings.sync();

    defaultencoding = settings.value( "/db/defaultencoding", "UTF8" ).toString();
    defaultnewdata = settings.value( "/db/defaultnewdata", "NULL" ).toString();
    defaultlocation = settings.value( "/db/defaultlocation", QDir::homePath() ).toString();
    defaulttext = settings.value( "/db/defaulttext", "Plain" ).toString();

    if (defaultencoding=="Latin1")
    {
	ui->encodingComboBox->setCurrentIndex(1);
    } else {
	ui->encodingComboBox->setCurrentIndex(0) ;
        defaultencoding = QString("UTF8");
    }
    
    if (defaultnewdata=="\'\'")
    {
	ui->defaultdataComboBox->setCurrentIndex(2) ;
    } else if (defaultnewdata=="0")
    {
	ui->defaultdataComboBox->setCurrentIndex(1) ;
    } else {
	ui->defaultdataComboBox->setCurrentIndex(0) ;
        defaultnewdata = QString("NULL");
    }

    if (defaulttext=="Auto")
    {
	ui->defaultTextComboBox->setCurrentIndex(1) ;
    } else {
	ui->defaultTextComboBox->setCurrentIndex(0) ;
        defaulttext = QString("Plain");
    }
    
    ui->locationEdit->setText(defaultlocation);
}


void preferencesForm::saveSettings()
{
    QSettings settings(QApplication::organizationName(), g_sApplicationNameShort);

    settings.setValue( "/db/defaultencoding", defaultencoding  );
    settings.setValue( "/db/defaultnewdata", defaultnewdata );
    settings.setValue( "/db/defaultlocation", defaultlocation  );
    settings.setValue( "/db/defaulttext", defaulttext  );
    settings.sync();
    accept();
}
