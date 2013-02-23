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
    if (which == 0) {
        defaultencoding = QString("UTF-8");
    } else {
        defaultencoding = QString("UTF-16");
    }
}

void PreferencesDialog::foreignkeysStateChanged(int state)
{
    foreignkeys = state > 0;
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
    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
    settings.sync();

    defaultencoding = settings.value( "/db/defaultencoding", "UTF-8" ).toString();
    defaultnewdata = settings.value( "/db/defaultnewdata", "NULL" ).toString();
    defaultlocation = settings.value( "/db/defaultlocation", QDir::homePath() ).toString();
    defaulttext = settings.value( "/db/defaulttext", "Plain" ).toString();
    foreignkeys = settings.value( "/db/foreignkeys", true ).toBool();

    if (defaultencoding=="Latin1" || defaultencoding == "UTF-8")
    {
        ui->encodingComboBox->setCurrentIndex(0);
    } else {
        ui->encodingComboBox->setCurrentIndex(1);
        defaultencoding = QString("UTF-16");
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

    ui->foreignKeysCheckBox->setChecked(foreignkeys);
    
    ui->locationEdit->setText(defaultlocation);
}

void PreferencesDialog::saveSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::organizationName());

    settings.setValue( "/db/defaultencoding", defaultencoding  );
    settings.setValue( "/db/defaultnewdata", defaultnewdata );
    settings.setValue( "/db/defaultlocation", defaultlocation  );
    settings.setValue( "/db/defaulttext", defaulttext  );
    settings.setValue( "/db/foreignkeys", foreignkeys );
    settings.sync();
    accept();
}
