#include "preferencesform.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qdir.h"
#include "qsettings.h"
#include "q3filedialog.h"
/*
 *  Constructs a preferencesForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
preferencesForm::preferencesForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
preferencesForm::~preferencesForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void preferencesForm::languageChange()
{
    retranslateUi(this);
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
   QString s = Q3FileDialog::getExistingDirectory(
                    defaultlocation,
                    this,
                    "get existing directory",
                    "Choose a directory",
                    TRUE );
		        
     defaultlocation = s;
     locationEdit->setText(defaultlocation);
}


void preferencesForm::loadSettings()
{
    QSettings settings;
    settings.setPath( "Tabuleiro.com", "Arca Database Browser 2", QSettings::UserScope );

   defaultencoding = settings.readEntry( "/db/defaultencoding", "UTF8" );
   defaultnewdata = settings.readEntry( "/db/defaultnewdata", "NULL" );
   defaultlocation = settings.readEntry( "/db/defaultlocation", QDir::homeDirPath () );
   defaulttext = settings.readEntry( "/db/defaulttext", "Plain" );
   
   if (defaultencoding=="Latin1")
   {
   encodingComboBox->setCurrentItem(1) ;
    } else {
    encodingComboBox->setCurrentItem(0) ;
    defaultencoding = QString("UTF8");
   }
    
    if (defaultnewdata=="\'\'")
   {
   defaultdataComboBox->setCurrentItem(2) ;
    } else if (defaultnewdata=="0")
   {
   defaultdataComboBox->setCurrentItem(1) ;
    } else {
    defaultdataComboBox->setCurrentItem(0) ;
    defaultnewdata = QString("NULL");
   }

    if (defaulttext=="Auto")
   {
   defaultTextComboBox->setCurrentItem(1) ;
    } else {
    defaultTextComboBox->setCurrentItem(0) ;
    defaulttext = QString("Plain");
   }
    
    locationEdit->setText(defaultlocation);
}


void preferencesForm::saveSettings()
{
    QSettings settings;
    settings.setPath( "sqlitebrowser.sf.net", "SQLite Database Browser 2", QSettings::UserScope );

   settings.writeEntry( "/db/defaultencoding", defaultencoding  );
   settings.writeEntry( "/db/defaultnewdata", defaultnewdata );
   settings.writeEntry( "/db/defaultlocation", defaultlocation  );
   settings.writeEntry( "/db/defaulttext", defaulttext  );
   accept();
}



