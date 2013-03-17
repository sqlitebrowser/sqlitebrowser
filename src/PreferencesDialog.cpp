#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include <QDir>
#include <QSettings>
#include <QFileDialog>
#include "sqlitedb.h"

QHash<QString, QVariant> PreferencesDialog::m_hCache;

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

void PreferencesDialog::chooseLocation()
{
    QString s = QFileDialog::getExistingDirectory(
                this,
                tr("Choose a directory"),
                getSettingsValue("db", "defaultlocation").toString(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!s.isEmpty())
        ui->locationEdit->setText(s);
}

void PreferencesDialog::loadSettings()
{
    ui->encodingComboBox->setCurrentIndex(ui->encodingComboBox->findText(getSettingsValue("db", "defaultencoding").toString(), Qt::MatchFixedString));
    ui->defaultdataComboBox->setCurrentIndex(ui->defaultdataComboBox->findText(getSettingsValue("db", "defaultnewdata").toString(), Qt::MatchFixedString));
    ui->locationEdit->setText(getSettingsValue("db", "defaultlocation").toString());
    ui->foreignKeysCheckBox->setChecked(getSettingsValue("db", "foreignkeys").toBool());
}

void PreferencesDialog::saveSettings()
{
    setSettingsValue("db", "defaultencoding", ui->encodingComboBox->currentText());
    setSettingsValue("db", "defaultnewdata", ui->defaultdataComboBox->currentText());
    setSettingsValue("db", "defaultlocation", ui->locationEdit->text());
    setSettingsValue("db", "foreignkeys", ui->foreignKeysCheckBox->isChecked());
    accept();
}

QVariant PreferencesDialog::getSettingsValue(const QString& group, const QString& name)
{
    // Have a look in the cache first
    QHash<QString, QVariant>::iterator cacheIndex = m_hCache.find(group + name);
    if(cacheIndex != m_hCache.end())
    {
        return cacheIndex.value();
    } else {
        // Nothing found in the cache, so get the value from the settings file or get the default value if there is no value set yet
        QSettings settings(QApplication::organizationName(), QApplication::organizationName());
        QVariant value = settings.value(group + "/" + name, getSettingsDefaultValue(group, name));

        // Store this value in the cache for further usage and return it afterwards
        m_hCache.insert(group + name, value);
        return value;
    }
}

void PreferencesDialog::setSettingsValue(const QString& group, const QString& name, const QVariant& value)
{
    // Set the group and save the given value
    QSettings settings(QApplication::organizationName(), QApplication::organizationName());
    settings.beginGroup(group);
    settings.setValue(name, value);
    settings.endGroup();

    // Also change it in the cache
    m_hCache[group + name] = value;
}

QVariant PreferencesDialog::getSettingsDefaultValue(const QString& group, const QString& name)
{
    // db/defaultencoding?
    if(group == "db" && name == "defaultencoding")
        return "UTF-8";

    // db/defaultnewdata?
    if(group == "db" && name == "defaultnewdata")
        return "NULL";

    // db/defaultlocation?
    if(group == "db" && name == "defaultlocation")
        return QDir::homePath();

    // db/foreignkeys?
    if(group == "db" && name == "foreignkeys")
        return true;

    // MainWindow/geometry?
    if(group == "MainWindow" && name == "geometry")
        return "";

    // MainWindow/windowState?
    if(group == "MainWindow" && name == "windowState")
        return "";

    // SQLLogDock/Log?
    if(group == "SQLLogDock" && name == "Log")
        return "Application";

    // General/recentFileList?
    if(group == "General" && name == "recentFileList")
        return QStringList();

    // Unknown combination of group and name? Return an invalid QVariant!
    return QVariant();
}
