#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include "FileDialog.h"
#include "Settings.h"
#include "Application.h"
#include "MainWindow.h"
#include "RemoteDatabase.h"

#include <QDir>
#include <QColorDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QStandardPaths>

PreferencesDialog::PreferencesDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    ui->treeSyntaxHighlighting->setColumnHidden(0, true);
    ui->labelDatabaseDefaultSqlText->setVisible(false);
    ui->editDatabaseDefaultSqlText->setVisible(false);
    ui->tableClientCerts->setColumnHidden(0, true);

    ui->fr_bin_bg->installEventFilter(this);
    ui->fr_bin_fg->installEventFilter(this);
    ui->fr_reg_bg->installEventFilter(this);
    ui->fr_reg_fg->installEventFilter(this);
    ui->fr_null_bg->installEventFilter(this);
    ui->fr_null_fg->installEventFilter(this);

#ifndef CHECKNEWVERSION
    ui->labelUpdates->setVisible(false);
    ui->checkUpdates->setVisible(false);
#endif

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
    QString s = FileDialog::getExistingDirectory(
                this,
                tr("Choose a directory"),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!s.isEmpty())
        ui->locationEdit->setText(s);
}

void PreferencesDialog::loadSettings()
{
    ui->encodingComboBox->setCurrentIndex(ui->encodingComboBox->findText(Settings::getValue("db", "defaultencoding").toString(), Qt::MatchFixedString));
    ui->comboDefaultLocation->setCurrentIndex(Settings::getValue("db", "savedefaultlocation").toInt());
    ui->locationEdit->setText(Settings::getValue("db", "defaultlocation").toString());
    ui->checkUpdates->setChecked(Settings::getValue("checkversion", "enabled").toBool());

    ui->checkHideSchemaLinebreaks->setChecked(Settings::getValue("db", "hideschemalinebreaks").toBool());
    ui->foreignKeysCheckBox->setChecked(Settings::getValue("db", "foreignkeys").toBool());
    ui->spinPrefetchSize->setValue(Settings::getValue("db", "prefetchsize").toInt());
    ui->editDatabaseDefaultSqlText->setText(Settings::getValue("db", "defaultsqltext").toString());

    ui->defaultFieldTypeComboBox->addItems(sqlb::Field::Datatypes);

    int defaultFieldTypeIndex = Settings::getValue("db", "defaultfieldtype").toInt();

    if (defaultFieldTypeIndex < sqlb::Field::Datatypes.count())
    {
        ui->defaultFieldTypeComboBox->setCurrentIndex(defaultFieldTypeIndex);
    }

    // Gracefully handle the preferred Data Browser font not being available
    int matchingFont = ui->comboDataBrowserFont->findText(Settings::getValue("databrowser", "font").toString(), Qt::MatchExactly);
    if (matchingFont == -1)
        matchingFont = ui->comboDataBrowserFont->findText(Settings::getDefaultValue("databrowser", "font").toString());
    ui->comboDataBrowserFont->setCurrentIndex(matchingFont);

    ui->spinDataBrowserFontSize->setValue(Settings::getValue("databrowser", "fontsize").toInt());
    loadColorSetting(ui->fr_null_fg, "null_fg");
    loadColorSetting(ui->fr_null_bg, "null_bg");
    loadColorSetting(ui->fr_reg_fg, "reg_fg");
    loadColorSetting(ui->fr_reg_bg, "reg_bg");
    loadColorSetting(ui->fr_bin_fg, "bin_fg");
    loadColorSetting(ui->fr_bin_bg, "bin_bg");

    ui->spinSymbolLimit->setValue(Settings::getValue("databrowser", "symbol_limit").toInt());
    ui->txtNull->setText(Settings::getValue("databrowser", "null_text").toString());
    ui->editFilterEscape->setText(Settings::getValue("databrowser", "filter_escape").toString());
    ui->spinFilterDelay->setValue(Settings::getValue("databrowser", "filter_delay").toInt());

    for(int i=0; i < ui->treeSyntaxHighlighting->topLevelItemCount(); ++i)
    {
        QString name = ui->treeSyntaxHighlighting->topLevelItem(i)->text(0);
        QString colorname = Settings::getValue("syntaxhighlighter", name + "_colour").toString();
        QColor color = QColor(colorname);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setTextColor(2, color);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setBackgroundColor(2, color);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setText(2, colorname);
        if (name != "null") {
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(3, Settings::getValue("syntaxhighlighter", name + "_bold").toBool() ? Qt::Checked : Qt::Unchecked);
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(4, Settings::getValue("syntaxhighlighter", name + "_italic").toBool() ? Qt::Checked : Qt::Unchecked);
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(5, Settings::getValue("syntaxhighlighter", name + "_underline").toBool() ? Qt::Checked : Qt::Unchecked);
        }
    }

    // Remote settings
    ui->checkUseRemotes->setChecked(Settings::getValue("remote", "active").toBool());
    {
        auto ca_certs = static_cast<Application*>(qApp)->mainWindow()->getRemote().caCertificates();
        ui->tableCaCerts->setRowCount(ca_certs.size());
        for(int i=0;i<ca_certs.size();i++)
        {
            QSslCertificate cert = ca_certs.at(i);

            QTableWidgetItem* cert_cn = new QTableWidgetItem(cert.subjectInfo(QSslCertificate::CommonName).at(0));
            cert_cn->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableCaCerts->setItem(i, 0, cert_cn);

            QTableWidgetItem* cert_o = new QTableWidgetItem(cert.subjectInfo(QSslCertificate::Organization).at(0));
            cert_o->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableCaCerts->setItem(i, 1, cert_o);

            QTableWidgetItem* cert_from = new QTableWidgetItem(cert.effectiveDate().toString());
            cert_from->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableCaCerts->setItem(i, 2, cert_from);

            QTableWidgetItem* cert_to = new QTableWidgetItem(cert.expiryDate().toString());
            cert_to->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableCaCerts->setItem(i, 3, cert_to);

            QTableWidgetItem* cert_serialno = new QTableWidgetItem(QString(cert.serialNumber()));
            cert_serialno->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableCaCerts->setItem(i, 4, cert_serialno);
        }
    }
    {
        QStringList client_certs = Settings::getValue("remote", "client_certificates").toStringList();
        foreach(const QString& file, client_certs)
        {
            auto certs = QSslCertificate::fromPath(file);
            foreach(const QSslCertificate& cert, certs)
                addClientCertToTable(file, cert);
        }
    }
    ui->editRemoteCloneDirectory->setText(Settings::getValue("remote", "clonedirectory").toString());

    // Gracefully handle the preferred Editor font not being available
    matchingFont = ui->comboEditorFont->findText(Settings::getValue("editor", "font").toString(), Qt::MatchExactly);
    if (matchingFont == -1)
        matchingFont = ui->comboDataBrowserFont->findText(Settings::getDefaultValue("editor", "font").toString());
    ui->comboEditorFont->setCurrentIndex(matchingFont);

    ui->spinEditorFontSize->setValue(Settings::getValue("editor", "fontsize").toInt());
    ui->spinTabSize->setValue(Settings::getValue("editor", "tabsize").toInt());
    ui->spinLogFontSize->setValue(Settings::getValue("log", "fontsize").toInt());
    ui->checkAutoCompletion->setChecked(Settings::getValue("editor", "auto_completion").toBool());
    ui->checkErrorIndicators->setChecked(Settings::getValue("editor", "error_indicators").toBool());
    ui->checkHorizontalTiling->setChecked(Settings::getValue("editor", "horizontal_tiling").toBool());

    ui->listExtensions->addItems(Settings::getValue("extensions", "list").toStringList());
    ui->checkRegexDisabled->setChecked(Settings::getValue("extensions", "disableregex").toBool());
    fillLanguageBox();
}

void PreferencesDialog::saveSettings()
{
    Settings::setValue("db", "defaultencoding", ui->encodingComboBox->currentText());
    Settings::setValue("db", "defaultlocation", ui->locationEdit->text());
    Settings::setValue("db", "savedefaultlocation", ui->comboDefaultLocation->currentIndex());
    Settings::setValue("db", "hideschemalinebreaks", ui->checkHideSchemaLinebreaks->isChecked());
    Settings::setValue("db", "foreignkeys", ui->foreignKeysCheckBox->isChecked());
    Settings::setValue("db", "prefetchsize", ui->spinPrefetchSize->value());
    Settings::setValue("db", "defaultsqltext", ui->editDatabaseDefaultSqlText->text());

    Settings::setValue("db", "defaultfieldtype", ui->defaultFieldTypeComboBox->currentIndex());

    Settings::setValue("checkversion", "enabled", ui->checkUpdates->isChecked());

    Settings::setValue("databrowser", "font", ui->comboDataBrowserFont->currentText());
    Settings::setValue("databrowser", "fontsize", ui->spinDataBrowserFontSize->value());
    saveColorSetting(ui->fr_null_fg, "null_fg");
    saveColorSetting(ui->fr_null_bg, "null_bg");
    saveColorSetting(ui->fr_reg_fg, "reg_fg");
    saveColorSetting(ui->fr_reg_bg, "reg_bg");
    saveColorSetting(ui->fr_bin_fg, "bin_fg");
    saveColorSetting(ui->fr_bin_bg, "bin_bg");
    Settings::setValue("databrowser", "symbol_limit", ui->spinSymbolLimit->value());
    Settings::setValue("databrowser", "null_text", ui->txtNull->text());
    Settings::setValue("databrowser", "filter_escape", ui->editFilterEscape->text());
    Settings::setValue("databrowser", "filter_delay", ui->spinFilterDelay->value());

    for(int i=0; i < ui->treeSyntaxHighlighting->topLevelItemCount(); ++i)
    {
        QString name = ui->treeSyntaxHighlighting->topLevelItem(i)->text(0);
        Settings::setValue("syntaxhighlighter", name + "_colour", ui->treeSyntaxHighlighting->topLevelItem(i)->text(2));
        Settings::setValue("syntaxhighlighter", name + "_bold", ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(3) == Qt::Checked);
        Settings::setValue("syntaxhighlighter", name + "_italic", ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(4) == Qt::Checked);
        Settings::setValue("syntaxhighlighter", name + "_underline", ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(5) == Qt::Checked);
    }
    Settings::setValue("editor", "font", ui->comboEditorFont->currentText());
    Settings::setValue("editor", "fontsize", ui->spinEditorFontSize->value());
    Settings::setValue("editor", "tabsize", ui->spinTabSize->value());
    Settings::setValue("log", "fontsize", ui->spinLogFontSize->value());
    Settings::setValue("editor", "auto_completion", ui->checkAutoCompletion->isChecked());
    Settings::setValue("editor", "error_indicators", ui->checkErrorIndicators->isChecked());
    Settings::setValue("editor", "horizontal_tiling", ui->checkHorizontalTiling->isChecked());

    QStringList extList;
    foreach(QListWidgetItem* item, ui->listExtensions->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard))
        extList.append(item->text());
    Settings::setValue("extensions", "list", extList);
    Settings::setValue("extensions", "disableregex", ui->checkRegexDisabled->isChecked());

    // Save remote settings
    Settings::setValue("remote", "active", ui->checkUseRemotes->isChecked());
    QStringList old_client_certs = Settings::getValue("remote", "client_certificates").toStringList();
    QStringList new_client_certs;
    for(int i=0;i<ui->tableClientCerts->rowCount();i++)
    {
        // Loop through the new list of client certs

        // If this certificate was already imported, remove it from the list of old certificates. All remaining certificates on this
        // list will be deleted later on.
        QString path = ui->tableClientCerts->item(i, 0)->text();
        if(old_client_certs.contains(path))
        {
            // This is a cert that is already imported
            old_client_certs.removeAll(path);
            new_client_certs.push_back(path);
        } else {
            // This is a new certificate. Copy file to a safe place.

            // Generate unique destination file name
            QString copy_to = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).append("/").append(QFileInfo(path).fileName());
            int suffix = 0;
            do
            {
                suffix++;
            } while(QFile::exists(copy_to + QString::number(suffix)));

            // Copy file
            copy_to.append(QString::number(suffix));
            QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
            QFile::copy(path, copy_to);

            new_client_certs.push_back(copy_to);
        }
    }
    foreach(const QString& file, old_client_certs)
    {
        // Now only the deleted client certs are still in the old list. Delete the cert files associated with them.
        QFile::remove(file);
    }
    Settings::setValue("remote", "client_certificates", new_client_certs);
    Settings::setValue("remote", "clonedirectory", ui->editRemoteCloneDirectory->text());

    // Warn about restarting to change language
    QVariant newLanguage = ui->languageComboBox->itemData(ui->languageComboBox->currentIndex());
    if (newLanguage != Settings::getValue("General", "language"))
        QMessageBox::information(this, QApplication::applicationName(),
                                 tr("The language will change after you restart the application."));

    Settings::setValue("General", "language", newLanguage);

    accept();
}

void PreferencesDialog::showColourDialog(QTreeWidgetItem* item, int column)
{
    if(item->text(column).left(1) != "#")
        return;

    QColor colour = QColorDialog::getColor(QColor(item->text(column)), this);
    if(colour.isValid())
    {
        item->setTextColor(column, colour);
        item->setBackgroundColor(column, colour);
        item->setText(column, colour.name());
    }
}

bool PreferencesDialog::eventFilter(QObject *obj, QEvent *event)
{
    // Use mouse click and enter press on the frames to pop up a colour dialog
    if (obj == ui->fr_bin_bg  || obj == ui->fr_bin_fg ||
        obj == ui->fr_reg_bg  || obj == ui->fr_reg_fg ||
        obj == ui->fr_null_bg || obj == ui->fr_null_fg)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *key = static_cast<QKeyEvent *>(event);
            // Not interesting, so send to the parent (might be shortcuts)
            if((key->key() != Qt::Key_Enter) && (key->key() != Qt::Key_Return))
            {
                return QDialog::eventFilter(obj, event);
            }
        }
        else if (event->type() != QEvent::MouseButtonPress)
        {
            // Not a key event neither a mouse event, send to the parent
            return QDialog::eventFilter(obj, event);
        }

        QFrame *frame = qobject_cast<QFrame *>(obj);
        QColor oldColour = frame->palette().color(frame->backgroundRole());
        QColor colour = QColorDialog::getColor(oldColour, frame);

        if (colour.isValid())
        {
            QPalette palette = frame->palette();
            palette.setColor(frame->backgroundRole(), colour);
            frame->setPalette(palette);
        }
        // Consume
        return true;
    }

    // Send any other events to the parent
    return QDialog::eventFilter(obj, event);
}

void PreferencesDialog::addExtension()
{
    QString file = FileDialog::getOpenFileName(
                this,
                tr("Select extension file"),
                tr("Extensions(*.so *.dll);;All files(*)"));

    if(QFile::exists(file))
        ui->listExtensions->addItem(file);
}

void PreferencesDialog::removeExtension()
{
    if(ui->listExtensions->currentIndex().isValid())
        ui->listExtensions->takeItem(ui->listExtensions->currentIndex().row());
}

void PreferencesDialog::fillLanguageBox()
{
    QDir translationsDir(QCoreApplication::applicationDirPath() + "/translations",
                         "sqlb_*.qm");

    QLocale systemLocale = QLocale::system();

    // Add default language
    if (systemLocale.name() == "en_US")
    {
        ui->languageComboBox->addItem(QIcon(":/flags/en_US"),
                                      "English (United States) [System Language]",
                                      "en_US");
    }
    else
    {
        ui->languageComboBox->addItem(QIcon(":/flags/en_US"),
                                      "English (United States) [Default Language]",
                                      "en_US");
    }

    // Get available *.qm files from translation dir near executable as well as from resources
    QFileInfoList file_infos = translationsDir.entryInfoList();
    file_infos += QDir(":/translations").entryInfoList();
    foreach(const QFileInfo &file, file_infos)
    {
        QLocale locale(file.baseName().remove("sqlb_"));

        // Skip invalid locales
        if(locale.name() == "C")
            continue;

        // Skip translations that were already loaded
        if (ui->languageComboBox->findData(locale.name(), Qt::UserRole, Qt::MatchExactly) != -1)
            continue;

        QString language = QLocale::languageToString(locale.language()) + " (" +
                           QLocale::countryToString(locale.country()) + ")";

        if (locale == systemLocale)
            language += " [System language]";

        ui->languageComboBox->addItem(QIcon(":/flags/" + locale.name()), language, locale.name());
    }

    ui->languageComboBox->model()->sort(0);

    // Try to select the language for the stored locale
    int index = ui->languageComboBox->findData(Settings::getValue("General", "language"),
                                               Qt::UserRole, Qt::MatchExactly);

    // If there's no translation for the current locale, default to English
    if(index < 0)
        index = ui->languageComboBox->findData("en_US", Qt::UserRole, Qt::MatchExactly);

    QString chosenLanguage = ui->languageComboBox->itemText(index);
    QVariant chosenLocale = ui->languageComboBox->itemData(index);
    QIcon chosenIcon = ui->languageComboBox->itemIcon(index);

    // There's no "move" method, so we remove and add the chosen language again at the top
    ui->languageComboBox->removeItem(index);
    ui->languageComboBox->insertItem(0, chosenIcon, chosenLanguage, chosenLocale);
    ui->languageComboBox->setCurrentIndex(0);
}

void PreferencesDialog::loadColorSetting(QFrame *frame, const QString & settingName)
{
    QPalette palette = frame->palette();
    palette.setColor(frame->backgroundRole(),
        QColor(Settings::getValue("databrowser", settingName + "_colour").toString()));
    frame->setPalette(palette);
}

void PreferencesDialog::saveColorSetting(QFrame *frame, const QString & settingName)
{
    Settings::setValue("databrowser", settingName + "_colour",
        frame->palette().color(frame->backgroundRole()));
}

void PreferencesDialog::activateRemoteTab(bool active)
{
    ui->tabWidget->setTabEnabled(ui->tabWidget->indexOf(ui->tabRemote), active);
}

void PreferencesDialog::addClientCertificate()
{
    // Get certificate file to import and abort here if no file gets selected
    // NOTE: We assume here that this file contains both, certificate and private key!
    QString path = FileDialog::getOpenFileName(this, tr("Import certificate file"), "*.pem");
    if(path.isEmpty())
        return;

    // Open file and check if any certificates were imported
    auto certs = QSslCertificate::fromPath(path);
    if(certs.size() == 0)
    {
        QMessageBox::warning(this, qApp->applicationName(), tr("No certificates found in this file."));
        return;
    }

    // Add certificates to list
    for(int i=0;i<certs.size();i++)
        addClientCertToTable(path, certs.at(i));
}

void PreferencesDialog::removeClientCertificate()
{
    // Any row selected?
    int row = ui->tableClientCerts->currentRow();
    if(row == -1)
        return;

    // Double check
    if(QMessageBox::question(this, qApp->applicationName(), tr("Are you sure you want do remove this certificate? All certificate "
                                                               "data will be deleted from the application settings!"),
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        ui->tableClientCerts->removeRow(row);
    }
}

void PreferencesDialog::addClientCertToTable(const QString& path, const QSslCertificate& cert)
{
    // Do nothing if the file doesn't even exist
    if(!QFile::exists(path))
        return;

    // Add new row
    int row = ui->tableClientCerts->rowCount();
    ui->tableClientCerts->setRowCount(row + 1);

    // Fill row with data
    QTableWidgetItem* cert_file = new QTableWidgetItem(path);
    cert_file->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableClientCerts->setItem(row, 0, cert_file);

    QTableWidgetItem* cert_subject_cn = new QTableWidgetItem(cert.subjectInfo(QSslCertificate::CommonName).at(0));
    cert_subject_cn->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableClientCerts->setItem(row, 1, cert_subject_cn);

    QTableWidgetItem* cert_issuer_cn = new QTableWidgetItem(cert.issuerInfo(QSslCertificate::CommonName).at(0));
    cert_issuer_cn->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableClientCerts->setItem(row, 2, cert_issuer_cn);

    QTableWidgetItem* cert_from = new QTableWidgetItem(cert.effectiveDate().toString());
    cert_from->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableClientCerts->setItem(row, 3, cert_from);

    QTableWidgetItem* cert_to = new QTableWidgetItem(cert.expiryDate().toString());
    cert_to->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableClientCerts->setItem(row, 4, cert_to);

    QTableWidgetItem* cert_serialno = new QTableWidgetItem(QString(cert.serialNumber()));
    cert_serialno->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableClientCerts->setItem(row, 5, cert_serialno);
}

void PreferencesDialog::chooseRemoteCloneDirectory()
{
    QString s = FileDialog::getExistingDirectory(
                this,
                tr("Choose a directory"),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!s.isEmpty())
        ui->editRemoteCloneDirectory->setText(s);
}
