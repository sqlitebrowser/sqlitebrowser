#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include <QDir>
#include <QSettings>
#include <QFileDialog>
#include "sqlitedb.h"

PreferencesDialog::PreferencesDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    loadSettings();
}

/*
 *  Destroys the object and frees any allocated resources
 */
PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::defaultDataChanged( int which )
{
    if (which==2) {
        defaultnewdata = QString("\'\'");
    } else if (which==1) {
        defaultnewdata = QString("0");
    } else {
        defaultnewdata = QString("NULL");
    }
}

void PreferencesDialog::defaultTextChanged( int which )
{
    if (which==1) {
        defaulttext = QString("Auto");
    } else {
        defaulttext = QString("Plain");
    }

}

void PreferencesDialog::encodingChanged( int which )
{
    if (which==1) {
        defaultencoding = QString("Latin1");
    } else {
        defaultencoding = QString("UTF8");
    }
}

void PreferencesDialog::chooseLocation()
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

void PreferencesDialog::loadSettings()
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

void PreferencesDialog::saveSettings()
{
    QSettings settings(QApplication::organizationName(), g_sApplicationNameShort);

    settings.setValue( "/db/defaultencoding", defaultencoding  );
    settings.setValue( "/db/defaultnewdata", defaultnewdata );
    settings.setValue( "/db/defaultlocation", defaultlocation  );
    settings.setValue( "/db/defaulttext", defaulttext  );
    settings.sync();
    accept();
}
